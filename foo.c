void _start() {

    /* main body of program: call main(), etc */

    /* exit system call */
    asm("movl $1,%eax;"
        "movl $3,%ebx;"
        "int  $0x80"
    );
}
