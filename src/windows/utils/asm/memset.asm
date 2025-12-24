                    section .text
                    global  __asm_memset
                    align   32, db  0x90
__asm_memset:       push            rsi
                    push            rdi
                    mov             rdi,    rcx
                    mov             rsi,    rdx
                    mov             rdx,    r8
                    vmovd           xmm0,   esi
                    vpbroadcastb    ymm0,   xmm0
                    mov             rax,    rcx
                    cmp             r8,    0x40
                    jae             above_64

below_64:           cmp             rdx,    0x20
                    jb              below_32
                    vmovdqu         [rdi],  ymm0
                    vmovdqu         [rdi + rdx - 0x20], ymm0
                    vzeroupper
                    jmp             exit
                    
                    align   32
below_32:           cmp             rdx,    0x10
                    jae             in_16_to_32

below_16:           cmp             rdx,    0x4
                    jbe             below_4

in_4_to_16:         vmovq           rsi,    xmm0
                    cmp             rdx,    0x7
                    jbe             in_4_to_8
                    mov             [rdi + rdx - 0x8],  rsi
                    mov             [rdi],  rsi
                    vzeroupper
                    jmp             exit

                    align   32
below_4:            vmovq           rsi,   xmm0
                    vzeroupper
                    cmp             rdx,    0x1
                    jbe             none_or_one
                    mov             [rdi],  si
                    mov             [rdi + rdx - 0x2],  si
                    jmp             exit

exit:               pop     rdi
                    pop     rsi
                    ret

                    align   16
in_4_to_8:          mov             [rdi + rdx - 0x4],  esi
                    mov             [rdi],  esi
                    vzeroupper
                    ret
                    
                    align   32
in_16_to_32:        vmovups         [rdi],  xmm0
                    vmovups         [rdi + rdx - 0x10], xmm0
                    vzeroupper
                    jmp             exit

above_64:           cmp             rdx,    0xb0
                    ja              above_192
                    cmp             rdx,    0x80
                    jbe             in_64_to_128
                    lea             rsi,    [rdi + rdx - 0x20]
                    vmovdqu         [rdi],  ymm0
                    vmovdqu         [rdi + 0x20],   ymm0
                    vmovdqu         [rdi + 0x40],   ymm0
                    add             rdi,    0x60

                    align   32
fill_32:            vmovdqu         [rdi],  ymm0
                    add             rdi,    0x20
                    cmp             rsi,    rdi
                    ja              fill_32
                    vmovdqu         [rsi],  ymm0
                    vzeroupper
                    jmp     exit

                    align   32
in_64_to_128:       vmovdqu     [rdi],  ymm0
                    vmovdqu     [rdi + 0x20], ymm0
                    vmovdqu     [rdi + rdx - 0x40], ymm0
                    vmovdqu     [rdi + rdx - 0x20], ymm0
                    vzeroupper
                    jmp     exit

                    align   32
above_192:          cmp     rdx,    0x1000
                    jae     large_stosq
                    vmovdqu [rdi],  ymm0
                    mov     rsi,    rdi
                    mov     rax,    rdi
                    and     rsi,    0xffffffffffffffe0
                    lea     rsi,    [rsi + 0x20]
                    lea     rdx,    [rdx - 0x20]
                    add     rdi,    rdx
                    lea     rcx,    [rsi + 0xa0]
                    cmp     rdi,    rcx
                    jb      stamp_4

fill_192:           vmovdqa [rsi],  ymm0
                    vmovdqa [rsi + 0x20],   ymm0
                    vmovdqa [rsi + 0x40],   ymm0
                    vmovdqa [rsi + 0x60],   ymm0
                    vmovdqa [rsi + 0x80],   ymm0
                    add     rsi,    0xa0
                    lea     rcx,    [rsi + 0xa0]
                    cmp     rdi,    rcx
                    ja      fill_192

fill_192_tail:      cmp     rdi,    rsi
                    jb      fill_192_done
                    vmovdqa [rsi],  ymm0

                    lea     rcx,    [rsi + 0x20]
                    cmp     rdi,    rcx
                    jb      fill_192_done
                    vmovdqa [rsi + 0x20],   ymm0

                    lea     rcx,    [rsi + 0x40]
                    cmp     rdi,    rcx
                    jb      fill_192_done
                    vmovdqa [rsi + 0x40],   ymm0

                    lea     rcx,    [rsi + 0x60]
                    cmp     rdi,    rcx
                    jb      fill_192_done
                    vmovdqa [rsi + 0x60],   ymm0

last_wide_store:    lea     rcx,    [rsi + 0x80]
                    cmp     rdi,    rcx
                    jb      fill_192_done
                    vmovdqa [rsi + 0x80],   ymm0

                    align   16
fill_192_done:      vmovdqu [rdi],  ymm0
                    vzeroupper
                    jmp     exit

stamp_4:            vmovdqa [rsi],  ymm0
                    vmovdqa [rsi + 0x20],   ymm0
                    vmovdqa [rsi + 0x40],   ymm0
                    vmovdqa [rsi + 0x60],   ymm0
                    jmp     last_wide_store

large_stosq:        vmovd   eax,    xmm0
                    mov     [rdi],  rax
                    mov     rsi,    rdi
                    and     rdi,    0xfffffffffffffff8
                    lea     rdi,    [rdi + 0x8]
                    mov     rcx,    rdx
                    sub     rcx,    0x8
                    shr     rcx,    0x3
                    rep     stosq
                    vmovdqu [rsi + rdx - 0x10], xmm0
                    vzeroupper
                    mov     rax,    rsi
                    jmp     exit

                    align   16
none_or_one:        test    rdx,    rdx
                    je      exit
                    mov     [rdi],  sil
                    jmp     exit

                    section .note.GNU-stack,"",@progbits
