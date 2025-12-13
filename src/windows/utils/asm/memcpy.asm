%define REP_MOVSB_THRESHOLD 1024

section .text

global __asm_memcpy

; Windows x64 ABI:
; Arguments: RCX (dst), RDX (src), R8 (len)
; Volatile: RAX, RCX, RDX, R8, R9, R10, R11
; Non-Volatile (Callee-saved): RBX, RBP, RSI, RDI, R12-R15

; ----------------------------------------------------------------------
; Short copy helper logic (used by jumps from __asm_memcpy)
; Uses RDI, RSI, R8 as arguments (dst, src, len) - adjusted from call site.
; ----------------------------------------------------------------------
L_GE1_LE7:
    cmp     r8, 1
    je      L_EQ1

    cmp     r8, 4
    jae     L_GE4_LE7

L_GE2_LE3:
    movzx   r10, word [rsi]
    movzx   r11, word [rsi + r8 - 2]
    mov     [rdi], r10w
    mov     [rdi + r8 - 2], r11w
    jmp     EXIT_SHORT

align 2
L_EQ1:
    movzx   r10, byte [rsi]
    mov     [rdi], r10b
    jmp     EXIT_SHORT

align 2
L_GE4_LE7:
    mov     r10d, [rsi]
    mov     r11d, [rsi + r8 - 4]
    mov     [rdi], r10d
    mov     [rdi + r8 - 4], r11d
    jmp     EXIT_SHORT

; ----------------------------------------------------------------------
; Main Entry Point
; ----------------------------------------------------------------------
align 64
__asm_memcpy:
    ; --- Windows ABI Bookkeeping ---
    ; Save non-volatile registers used by the main logic (RSI, RDI, R12)
    push    rdi
    push    rsi
    push    r12
    
    ; Setup internal registers for the main logic (using the old RDI/RSI/RCX roles)
    ; RCX (arg 1, dst) -> RDI (internal dst)
    ; RDX (arg 2, src) -> RSI (internal src)
    ; R8  (arg 3, len) -> RCX (internal len/count)
    mov     rdi, rcx
    mov     rsi, rdx
    mov     rcx, r8     ; RCX is now the length
    
    ; Save original DST pointer to RAX for return value
    mov     rax, rdi

    ; Save original length to R8 (volatile, but needed for tail handling)
    mov     r8, rcx

    ; ---------------------------------
    ; Start of main memcpy logic
    ; ---------------------------------
    test    rcx, rcx
    jz      L_EQ0

    cmp     rcx, 8
    jb      L_GE1_LE7   ; RDI, RSI, RCX (len) are set for L_GE1_LE7

L_GE8:
    cmp     rcx, 32
    ja      L_GE33

L_GE8_LE32:
    cmp     rcx, 16
    ja      L_GE17_LE32

L_GE8_LE16:
    mov     r10, [rsi]
    mov     r11, [rsi + rcx - 8]
    mov     [rdi], r10
    mov     [rdi + rcx - 8], r11
    jmp     EXIT

L_EQ0:
    jmp     EXIT

align 2
L_GE17_LE32:
    movdqu  xmm0, [rsi]
    movdqu  xmm1, [rsi + rcx - 16]
    movdqu  [rdi], xmm0
    movdqu  [rdi + rcx - 16], xmm1
    jmp     EXIT

; --- AVX (33-256 byte) Loads/Stores ---
L_GE193_LE256: 
    vmovdqu [rdi + 96], ymm3
    vmovdqu [rdi + r8 - 128], ymm4 ; Use original length in R8 for tail

L_GE129_LE192: 
    vmovdqu [rdi + 64], ymm2
    vmovdqu [rdi + r8 - 96], ymm5

L_GE65_LE128:
    vmovdqu [rdi + 32], ymm1
    vmovdqu [rdi + r8 - 64], ymm6

L_GE33_LE64:
    vmovdqu [rdi], ymm0
    vmovdqu [rdi + r8 - 32], ymm7 ; Use original length in R8 for tail

    vzeroupper
    jmp     EXIT

align 2
L_GE33:
    cmp     rcx, 256
    ja      L_GE257

L_GE33_LE256:
    vmovdqu ymm0, [rsi]
    vmovdqu ymm7, [rsi + rcx - 32]

    cmp     rcx, 64
    jbe     L_GE33_LE64

    vmovdqu ymm1, [rsi + 32]
    vmovdqu ymm6, [rsi + rcx - 64]

    cmp     rcx, 128
    jbe     L_GE65_LE128

    vmovdqu ymm2, [rsi + 64]
    vmovdqu ymm5, [rsi + rcx - 96]

    cmp     rcx, 192
    jbe     L_GE129_LE192

    vmovdqu ymm3, [rsi + 96]
    vmovdqu ymm4, [rsi + rcx - 128]

    cmp     rcx, 256
    jbe     L_GE193_LE256

; --- Large Copy (Forward/Backward) ---
L_GE257:
    cmp     rsi, rdi
    jae     L_COPY_FORWARD      ; if src >= dst, copy forward (safe)

    lea     r12, [rsi + rcx]    ; r12 = src + len
    cmp     r12, rdi            ; Check for overlap: src + len > dst?
    ja      L_OVERLAP_BWD       ; If overlap, must copy backward (memmove)
    ; otherwise, fall through to copy forward (memcpy)

L_COPY_FORWARD:
    mov     r9, rcx             ; r9 = copy length (volatile, safe to use)
    cmp     rcx, REP_MOVSB_THRESHOLD
    jb      L_COPY_FORWARD_WITH_LOOP

L_COPY_FORWARD_WITH_REP_MOVSB:
    ; RSI/RDI already hold src/dst, RCX holds count
    rep movsb
    jmp     EXIT

L_COPY_FORWARD_WITH_LOOP:
    ; Use R8 as the original length to calculate the tail
    vmovdqu ymm4, [rsi + r8 - 32]
    xor     r10, r10            ; r10 = length copied
    mov     r9, rcx             ; r9 = original count (loop length)
    shr     r9, 7
    shl     r9, 7               ; r9 = loop limit (n & ~127)

align 16
L_COPY_FORWARD_LOOP_BODY:
    vmovdqu ymm0, [rsi + r10]
    vmovdqu ymm1, [rsi + r10 + 32]
    vmovdqu ymm2, [rsi + r10 + 64]
    vmovdqu ymm3, [rsi + r10 + 96]
    vmovdqu [rdi + r10], ymm0
    vmovdqu [rdi + r10 + 32], ymm1
    vmovdqu [rdi + r10 + 64], ymm2
    vmovdqu [rdi + r10 + 96], ymm3
    add     r10, 128
    cmp     r10, r9
    jb      L_COPY_FORWARD_LOOP_BODY

    ; Tail handling: re-set RCX to original total length (R8)
    mov     rcx, r8
    sub     rcx, r10            ; rcx = tail length (volatile, safe to use)
    cmp     rcx, 32
    jbe     L_TAIL_LE32         ; Use original tail calculation
    
    ; Jump back to L_GE33_LE256 to copy the rest, but update pointers first
    add     rsi, r10
    add     rdi, r10
    mov     r8, rcx             ; R8 must be the new total length for L_GE33_LE256 logic
    jmp     L_GE33_LE256

L_TAIL_LE32:
    vmovdqu [rdi + r8 - 32], ymm4 ; r8 = original length, rdi = start
    vzeroupper
    jmp     EXIT

; --- Overlap Backward Copy (memmove logic) ---
L_OVERLAP_BWD:
    ; Initial tail save (uses original pointers/length)
    vmovdqu ymm8, [rsi + rcx - 32]
    lea     r9, [rdi + rcx - 32]    ; R9 = address of last 32 bytes (volatile, safe)
    
    ; Save initial 128 bytes (used at the very end to fill the gap)
    vmovdqu ymm0, [rsi]
    vmovdqu ymm1, [rsi + 32]
    vmovdqu ymm2, [rsi + 64]
    vmovdqu ymm3, [rsi + 96]

    ; R10 is the end condition for the loop (src_start + 128)
    lea     r10, [rsi + 128]

    ; Align DST end to 32 bytes
    ; RCX = alignment offset (0-31)
    mov     r11, r9             ; R11 = dst_end - 32
    and     r11, 31             ; R11 = misalignment offset (0-31)
    
    ; RCX (len) is reduced by the tail alignment offset (RCX = RCX - R11)
    sub     rcx, r11
    
    ; RSI/RDI now point to the end of the aligned block
    add     rsi, rcx
    add     rdi, rcx

align 16
L_OVERLAP_BWD_ALIGNED_DST_LOOP:
    ; Load source blocks (Unaligned loads are fine)
    vmovdqu ymm4, [rsi - 32]
    vmovdqu ymm5, [rsi - 64]
    vmovdqu ymm6, [rsi - 96]
    vmovdqu ymm7, [rsi - 128]
    sub     rsi, 128

    ; Store destination blocks (MUST BE UNALIGNED (vmovdqu) for safety)
    vmovdqu [rdi - 32], ymm4
    vmovdqu [rdi - 64], ymm5
    vmovdqu [rdi - 96], ymm6
    vmovdqu [rdi - 128], ymm7
    sub     rdi, 128

    cmp     rsi, r10
    ja      L_OVERLAP_BWD_ALIGNED_DST_LOOP

    ; Write the saved 128 bytes (ymm0-ymm3) and the last 32 bytes (ymm8)
    vmovdqu [rax], ymm0         ; RAX holds original RDI (dst)
    vmovdqu [rax + 32], ymm1
    vmovdqu [rax + 64], ymm2
    vmovdqu [rax + 96], ymm3
    vmovdqu [r9], ymm8          ; R9 holds the address of the saved tail

    vzeroupper
    jmp     EXIT

; ----------------------------------------------------------------------
; Exit Handlers
; ----------------------------------------------------------------------
EXIT:
    ; Restore non-volatile registers (LIFO: R12, RSI, RDI)
    pop     r12
    pop     rsi
    pop     rdi
    ret

EXIT_SHORT:
    ; This is for the short paths (1-7 bytes) which use R8 (len)
    ; We need to move R8 back to RCX for the pop stack logic to work correctly
    ; No, we don't. We just need to restore the pushed registers.

    ; Since the short copy section jumped over the push/move logic, it needs
    ; to use the full EXIT sequence to restore the stack.
    ; RDI, RSI, R8 (len) are the active registers here.
    
    ; Restore non-volatile registers (LIFO: R12, RSI, RDI)
    pop     r12
    pop     rsi
    pop     rdi
    ret
