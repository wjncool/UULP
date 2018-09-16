#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#define BUFFRSIZE 4096
#define COPYMODE  0644

void do_copy(char * ,  char * );
void copy_file(char * , char * );
void oops(char *, const char *);

int main(int argc, char *argv[])
{
    if(argc != 3){
        fprintf(stderr, "usage: %s source destination\n", *argv);
        _exit(1);
    }
    do_copy(argv[1], argv[2]);

    return 0;
}

void do_copy(char * srcname, char * dstname)
{
    struct stat info_src;
    struct stat info_dst;
    
    if((stat(srcname, &info_src) != -1) && (stat(dstname, &info_dst) != -1)){
        if(S_ISDIR(info_src.st_mode) && S_ISDIR(info_dst.st_mode)){
            if(chdir(srcname) == -1)
                perror(srcname);
            
            DIR *dir_ptr;
            struct dirent * direntp;

            if((dir_ptr = opendir(".")) == NULL)
                fprintf(stderr, "cannot open %s\n", srcname);
            else
            {
                while((direntp = readdir(dir_ptr)) != NULL){
                    char dst_file_name[BUFFRSIZE];
                    strcpy(dst_file_name, dstname);

                    char * filename = direntp -> d_name;
                    if(strcmp(filename, ".") == 0 || (strcmp(filename, "..") == 0))
                        continue;
                    else{
                        strcat(dst_file_name, "/");
                        strcat(dst_file_name, filename);
                    }
                    copy_file(filename, dst_file_name);
                }
            }
        }
        else if((! S_ISDIR(info_src.st_mode) && S_ISDIR(info_dst.st_mode))){
            char dst_file_name[BUFFRSIZE];
            strcpy(dst_file_name, dstname);

            if(S_ISDIR(info_dst.st_mode)){
                strcat(dst_file_name, "/");
                strcat(dst_file_name, srcname);
            }
            copy_file(srcname, dst_file_name);
        }
    }
}

void copy_file(char * src_file, char * dst_file)
{
    int in_fd, out_fd, n_chars;
    char buf[BUFFRSIZE];

    if((in_fd = open(src_file, O_RDONLY)) == -1)
        oops("Cannot open ", src_file);
    if((out_fd = creat(dst_file, COPYMODE)) == -1)
        oops("Cannot create", dst_file);

    while((n_chars = read(in_fd, buf, BUFFRSIZE)) > 0)
        if(write(out_fd, buf, n_chars) != n_chars)
            oops("Write error to", dst_file);
    if(n_chars == -1)
        oops("Read error from ",src_file);
    if(close(in_fd) == -1 || close(out_fd) == -1)
        oops("Error closing files","");
}

void oops(char * s1, const char * s2)
{
    fprintf(stderr, "Error: %s ", s1);
    perror(s2);
    _exit(1);
}