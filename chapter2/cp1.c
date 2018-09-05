#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFRSIZE 4096
#define COPYMODE  0644

void oops(char *, const char *);

int main(int argc, char const *argv[])
{
    int in_fd, out_fd, n_chars;
    char buf[BUFFRSIZE];

    if(argc != 3){
        fprintf(stderr, "usage: %s source destination\n", *argv);
        _exit(1);
    }

    if((in_fd = open(argv[1], O_RDONLY)) == -1)
        oops("Cannot open ", argv[1]);
    if((out_fd = creat(argv[2], COPYMODE)) == -1)
        oops("Cannot create", argv[2]);

    while((n_chars = read(in_fd, buf, BUFFRSIZE)) > 0)
        if(write(out_fd, buf, n_chars) != n_chars)
            oops("Write error to", argv[2]);
    if(n_chars == -1)
        oops("Read error from ", argv[1]);
    if(close(in_fd) == -1 || close(out_fd) == -1)
        oops("Error closing files","");
    return 0;
}

void oops(char * s1, const char * s2)
{
    fprintf(stderr, "Error: %s ", s1);
    perror(s2);
    _exit(1);
}

