#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>

void try_err(const char *);
bool is_DIR(const char *);
void copy_file(const char * filename);

int main(int argc,  const char *argv[])
{
    char * src = (char *) argv[1];
    char * dst = (char *) argv[2];
    
    if (argc != 3)
        try_err("usage: mv filaname target\n");
    
    if(is_DIR(argv[1]))
        try_err("para 2 should be file, not a directory!\n");
    
    if(is_DIR(argv[2])){
        if(dst[strlen(src)-1] != '/')
            strcat(dst, "/");
        strcat(dst, src);
    }
    
    if(rename(src, dst) != 0)
        perror("rename");
    return 0;
}

void try_err(const char * err_str)
{
    printf(err_str);
    _exit(1);
}
bool is_DIR(const char * filename)
{
    struct stat info_p;
    if(stat(filename, &info_p) == 0)
        if(S_ISDIR(info_p.st_mode))
            return true;
    return false;
}
