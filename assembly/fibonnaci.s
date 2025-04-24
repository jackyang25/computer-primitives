fibonnaci:
        pushl %ebp
        movl %esp,%ebp
        pushl %esi
        pushl %edi
        cmpl $0,8(%ebp)
        je .L3
        cmpl $1,8(%ebp)
        je .L3
        jmp .L2
.L3:
        movl 8(%ebp),%eax
        jmp .L1
        jmp .L4
.L2:
        movl 8(%ebp),%eax
        decl %eax
        pushl %eax
        call fibonnaci
        movl %eax,%esi
        movl 8(%ebp),%eax
        addl $-2,%eax
        pushl %eax
        call fibonnaci
        movl %eax,%edi
        leal (%edi,%esi),%eax
        jmp .L1
.L4:
.L1:
        leal -8(%ebp),%esp
        popl %edi
        popl %esi
        leave
        ret
