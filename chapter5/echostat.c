#include <stdio.h>
#include <termios.h>

int main()
{
    struct termios info;
    int rv;

    rv = tcgetattr(0, &info);
    if(rv == -1){
        perror("tcgetattr");
        _exit(1);
    }
    if(info.c_lflag & ECHO)
        printf(" echo is on, sincr its bit is 1\n");
    else
        printf(" echo is off, sincr its bit is 0\n");
        
    return 0;
}