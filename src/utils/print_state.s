.data
text1:
    .ascii "\r[status]"
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

    POPQ %rsi
    MOVQ $1, %rdi
    MOVQ $1, %rax
    POPQ %rdx
    SYSCALL

    RET
