#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

void make_dir(char *);
int main(int argc,  const char *argv[])
{

    if(argc != 3 || strcmp(argv[1], "-p") != 0) {
        printf("usage: mkdir -p directory ...\n");
        _exit(1);
    }
    make_dir(argv[2]);
    return 0;
}

void make_dir(char * path)
{
    while(chdir(path) != 0){
        char * Dirname = strrchr(path, '/');
        char * dirname = &Dirname[1];
        if(Dirname != NULL){
            path[(int)(Dirname - path)] = '\0';
            make_dir(path);
        }
        else
            dirname = path;
        
        if (mkdir(dirname, 0755) != 0)
            perror("create directory");
        chdir(dirname);
        break;
    }
}