%define REP_MOVSB_THRESHOLD 1024

                    section text
                    global  __asm_memcpy_short
                    global  __asm_memcpy
__asm_memcpy_short:
L_GE1_LE7: cmp     r8, 1
            je      L_EQ1
            
            cmp     r8, 4
            jae     L_GE4_LE7

L_GE2_LE3: mov    r10w,   [rdx]
            mov    r11w,   [rdx + r8 - 2]
            mov    [rcx],  r10w
            mov    [rcx + r8 - 2], r11w
            jmp		EXIT

            align   2
L_EQ1:     mov    r10b,   [rdx]
            mov    [rcx],  r10b
            jmp		EXIT
            
            align   2
L_GE4_LE7: mov    r10d,   [rdx]
            mov    r11d, [rdx + r8 - 4]
            mov    [rcx],  r10d
            mov    [rcx + r8 - 4], r11d
            jmp		EXIT

                align   64
__asm_memcpy:   push    r12
                mov     rax,    rcx     ;jmp		EXITurn first parameter src
                
                test    r8,     r8
                je      L_EQ0

                cmp     r8,     8
                jb      L_GE1_LE7

L_GE8:         cmp     r8,     32
                ja      L_GE33

L_GE8_LE32:    cmp     r8,     16
                ja      L_GE17_LE32

L_GE8_LE16:    mov     r10,    [rdx]
                mov     r11,    [rdx + r8 - 8]
                mov     [rcx],  r10
                mov     [rcx + r8 - 8], r11

L_EQ0:         jmp		EXIT

                align   2
L_GE17_LE32:   movdqu  xmm0,   [rdx]
                movdqu  xmm1,   [rdx + r8 - 16]
                movdqu  [rcx],  xmm0
                movdqu  [rcx + r8 - 16],    xmm1
                jmp		EXIT

L_GE193_LE256: vmovdqu [rcx + 96], ymm3
                vmovdqu [rcx + r8 - 128],   ymm4

L_GE129_LE192: vmovdqu [rcx + 64], ymm2
                vmovdqu [rcx + r8 - 96],    ymm5

L_GE65_LE128:  vmovdqu [rcx + 32], ymm1
                vmovdqu [rcx + r8 - 64],    ymm6

L_GE33_LE64:   vmovdqu [rcx],  ymm0
                vmovdqu [rcx + r8 - 32],    ymm7

                vzeroupper
                jmp		EXIT

L_GE33:        cmp     r8, 256
                ja      L_GE257

L_GE33_LE256:  vmovdqu ymm0,   [rdx]
                vmovdqu ymm7,   [rdx + r8 -32]

                cmp     r8, 64
                jbe     L_GE33_LE64

                vmovdqu ymm1,   [rdx + 32]
                vmovdqu ymm6,   [rdx + r8 - 64]

                cmp     r8, 128
                jbe     L_GE65_LE128

                vmovdqu ymm2,   [rdx + 64]
                vmovdqu ymm5,   [rdx + r8 - 96]

                cmp     r8, 192
                jbe     L_GE129_LE192

                vmovdqu ymm3,   [rdx + 96]
                vmovdqu ymm4,   [rdx + r8 -128]

                cmp     r8, 256
                jbe     L_GE193_LE256

L_GE257:       cmp     rdx,    rcx
                jae     L_COPY_FORWARD
                
                lea     r12,    [rdx + r8]
                cmp     r12,    rcx

                ja      L_OVERLAP_BWD

L_COPY_FORWARD:    mov     r9,     r8
                    cmp     r8,     REP_MOVSB_THRESHOLD
                    jb      L_COPY_FORWARD_WITH_LOOP

L_COPY_FORWARD_WITH_REP_MOVSB:
                    rep movsb
                    jmp		EXIT

L_COPY_FORWARD_WITH_LOOP:
                    vmovdqu ymm4,   [rdx + r8 - 32]
                    xor     r10,    r10
                    shr     r9,     7
                    shl     r9,     7

                    align   16
L_COPY_FORWARD_LOOP_BODY:
                    vmovdqu ymm0,   [rdx + r10]
                    vmovdqu ymm1,   [rdx + r10 + 32]
                    vmovdqu ymm2,   [rdx + r10 + 64]
                    vmovdqu ymm3,   [rdx + r10 + 96]
                    vmovdqu [rcx + r10], ymm0
                    vmovdqu [rcx + r10 + 32],   ymm1
                    vmovdqu [rcx + r10 + 64],   ymm2
                    vmovdqu [rcx + r10 + 96],   ymm3
                    add     r10,    128
                    cmp     r10,    r9
                    jb      L_COPY_FORWARD_LOOP_BODY

                    mov     r9, r8
                    sub     r8, r10
                    cmp     r8, 32
                    jbe     L_TAIL_LE32
                    add     rdx,    r10
                    add     rcx,    r10
                    jmp     L_GE33_LE256

L_TAIL_LE32:   vmovdqu [rcx + r9 - 32], ymm4
                vzeroupper
                jmp     EXIT

L_OVERLAP_BWD: vmovdqu ymm8,   [rdx + r8 - 32]
                lea     r11,    [rcx + r8 - 32]
                vmovdqu ymm0,   [rdx]
                vmovdqu ymm1,   [rdx + 32]
                vmovdqu ymm2,   [rdx + 64]
                vmovdqu ymm3,   [rdx + 96]

                lea     r10,    [rdx + 128]

                mov     r9,     r11
                and     r9,     31

                sub     r8,     r9
                add     rdx,    r8
                add     rcx,    r8

                align   16
L_OVERLAP_BWD_ALIGNED_DST_LOOP:
                vmovdqu ymm4,   [rdx - 32]
                vmovdqu ymm5,   [rdx - 64]
                vmovdqu ymm6,   [rdx - 96]
                vmovdqu ymm7,   [rdx - 128]
                sub     rdx,    128

                vmovdqa [rcx - 32], ymm4
                vmovdqa [rcx - 64], ymm5
                vmovdqa [rcx - 96], ymm6
                vmovdqa [rcx - 128], ymm7
                sub     rcx,    128

                cmp     rdx,    r10
                ja      L_OVERLAP_BWD_ALIGNED_DST_LOOP

                vmovdqu [rax],      ymm0
                vmovdqu [rax + 32], ymm1
                vmovdqu [rax + 64], ymm2
                vmovdqu [rax + 96], ymm3
                vmovdqu [r11],    ymm8

                vzeroupper
                jmp     EXIT

EXIT:           pop     r12
                ret
