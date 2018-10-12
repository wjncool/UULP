#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

ino_t get_inode(char *);
void printpathto(ino_t);
void inum_to_name(ino_t, char *, int);

int main()
{
    printpathto(get_inode("."));
    putchar('\n');
    return 0;
}

void printpathto(ino_t this_indo)
{
    ino_t my_inode;
    char its_name[BUFSIZ];
    if(get_inode("..") != this_indo)
    {
        chdir("..");
        inum_to_name(this_indo, its_name, BUFSIZ);
        my_inode = get_inode(".");
        printpathto(my_inode);
        printf("/%s", its_name);
    }
}

void inum_to_name(ino_t inode_to_find, char * namebuf, int buflen)
{
    DIR * dir_ptr;
    struct dirent * direntp;
    dir_ptr = opendir(".");
    if(dir_ptr == NULL){
        perror(".");
        _exit(1);
    }

    while((direntp = readdir(dir_ptr)) != NULL)
        if(direntp -> d_ino == inode_to_find)
        {
            strncpy(namebuf, direntp -> d_name, buflen);
            namebuf[buflen - 1] = '\0';
            closedir(dir_ptr);
            return;
        }
    fprintf(stderr, "error looking for inum %llu\n", inode_to_find);
    _exit(1);
}

ino_t get_inode(char * fname)
{
    struct stat info;
    if(stat(fname, &info) == -1){
        fprintf(stderr, "Cannot stat ");
        perror(fname);
        _exit(1);
    }
    return info.st_ino;
}