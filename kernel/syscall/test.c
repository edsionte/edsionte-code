#include <stdio.h>
#include <stdlib.h>

int main()
{
    unsigned long x = 0;
    x = syscall(223);
    printf("hello, %ld\n", x);
    return 0;
}     
