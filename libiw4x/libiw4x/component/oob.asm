; OOB packet interception (CL_DispatchConnectionlessPacket)

BITS 64
DEFAULT REL

EXTERN oob_dispatch

SECTION .text

; Export the stub function
GLOBAL oob_dispatch_stub

oob_dispatch_stub:
    ; Save return address.
    push    rax
    mov     rax, [rsp+8]
    mov     [rsp-8], rax
    pop     rax

    ; Save all registers.
    push    rax
    push    rcx
    push    rdx
    push    rbx
    push    rbp
    push    rsi
    push    rdi
    push    r8
    push    r9
    push    r10
    push    r11
    push    r12
    push    r13
    push    r14
    push    r15

    ; Allocate shadow space and align stack.
    sub     rsp, 0x28

    ; Setup parameters: RCX = RBX (addr), RDX = R15 (msg).
    mov     rcx, rbx
    mov     rdx, r15

    ; Call C wrapper.
    call    oob_dispatch

    ; Save result.
    mov     cl, al

    ; Restore stack.
    add     rsp, 0x28

    ; Restore all registers.
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     r11
    pop     r10
    pop     r9
    pop     r8
    pop     rdi
    pop     rsi
    pop     rbp
    pop     rbx
    pop     rdx
    pop     rcx
    pop     rax

    ; Check result.
    test    cl, cl
    jz      .handler_declined

    ; Handler signalled consumption. Return directly to caller.
    mov     al, 1
    ret

.handler_declined:
    ; Handler declined. Resume original control flow.
    mov     r11, 0x1400F6065
    jmp     r11
