#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>

#define SHOWHOST
void show_info(struct utmp *);

int main(int argc, char const *argv[])
{
    struct utmp curren_record;
    int utmpxfd;
    int reclen = sizeof(curren_record);

    if((utmpxfd = open(UTMP_FILE, O_RDONLY)) == -1){
        perror(UTMP_FILE);
        _exit(1);
    }

    while(read(utmpxfd, &curren_record, reclen) == reclen)
        show_info(&curren_record);
    close(utmpxfd);
    return 0;
}

void show_info(struct utmp * utmpbuf)
{
    printf("% -8.8s", utmpbuf -> ut_name);
    printf(" ");
    printf("% -8.8s", utmpbuf -> ut_line);
    printf(" ");
    printf("% 10ld", utmpbuf -> ut_time);
#ifdef SHOWHOST
    printf("(% s)", utmpbuf -> ut_host);
#endif
    printf("\n");
}