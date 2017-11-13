.data
//.text
text1:
    .ascii "\r[status]"
    //.byte 5
text2:
    .ascii "MB/"
text3:
    .ascii "MB"
procent:
    .byte 0x25
null_help:
    .ascii "0"
.text
    .globl print_num
print_num:
    PUSHQ %rdx
    PUSHQ %rsi
    PUSHQ %rdi
    

    MOVABSQ $text1, %rax
    MOVQ %rax, %rsi
    MOVQ $1, %rdi
    MOVQ $1, %rax
    MOVQ $9, %rdx
    SYSCALL

    MOVABSQ $null_help, %rax
    MOVQ %rax,%r10
    MOVQ %rax,%rsi
    
    POPQ %rax
    MOVQ $10,%rbx

    XOR %r12, %r12
divide:
    XORQ %rdx,%rdx
    
    DIVQ %rbx
    INCQ %r12
    PUSHQ %rdx
    
    CMPQ $0,%rax
    JE isnull
    JNE divide
isnull:        
    MOVQ %r10,%rsi
loop:
    POPQ %r9
    ADDQ %r9,(%rsi)
    MOVQ $1,%rax
    MOVQ $1,%rdi
    MOVQ $1,%rdx
    SYSCALL

    SUBQ %r9,(%rsi)

    DECQ %r12
    CMPQ $0,%r12
    JNE loop

    MOVABSQ $text2, %rax
    MOVQ %rax, %rsi
    MOVQ $1, %rdi
    MOVQ $1, %rax
    MOVQ $3, %rdx
    SYSCALL

    POPQ %rsi
    MOVQ $1, %rdi
    MOVQ $1, %rax
    POPQ %rdx
    SYSCALL

    MOVABSQ $text3, %rax
    MOVQ %rax, %rsi
    MOVQ $1, %rdi
    MOVQ $1, %rax
    MOVQ $2, %rdx
    SYSCALL

    RET
