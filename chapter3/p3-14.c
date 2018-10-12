#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define BUFFSIZE 4096
#define COPYMODE 0644

void oops(char *, char *);
int is_dir(char *);
char * new_path(char * , char *);
void copy_file(char *, char *);

int main(int ac, char *av[])
{
    /*if(ac != 3){
        fprintf(stderr, "usage: %s source destination\n", *av);
        exit(1);
    }*/
    av[1] = "./";
    av[2] = "/Users/gnncool/Desktop/test2";
    
    if (is_dir(av[1]) && is_dir(av[2])){
        DIR *dp;
        struct dirent* dirent_ptr;
        if((dp = opendir(av[1])) == NULL)
            oops("can't open", av[1]);
        
        /*if (chdir(av[1]) == -1)
            oops("can't enter", av[1]);*/
        //改变目录后复制上一个文件夹的文件会出错
        
        while((dirent_ptr = readdir(dp)) != NULL){
            char * filename = dirent_ptr -> d_name;
            copy_file(new_path(av[1], filename), new_path(av[2], filename));
        }
        closedir(dp);
    }
    else
        printf("argues must be directories");
    return 0;
}

void oops(char *s1, char *s2)
{
    fprintf(stderr, "Error: %s", s1);
    perror(s2);
    exit(1);
}

int is_dir(char *name)
{
    struct stat info;
    if(stat(name, &info) != -1){
        mode_t mode = info.st_mode;
        if(S_ISDIR(mode))
            return 1;
    }
    return 0;
}

char * new_path(char *path, char *file)
{
    if((strcmp(file, ".") == 0) || (strcmp(file, "..") == 0))
        return file;
    
    char * n_path;
    n_path =(char *)malloc(strlen(path) + strlen(file) + 2);
    strcpy(n_path, path);
    strcat(n_path, "/");
    strcat(n_path, file);
    return n_path;
}

void copy_file(char * src_path, char * dst_path)
{
    if((strcmp(src_path, ".") == 0) || (strcmp(src_path, "..") == 0))
        return;
    
    if(is_dir(src_path)){
        DIR *dp;
        struct dirent* dirent_ptr;
        
        if((dp = opendir(src_path)) == NULL)
            oops("can't open", src_path);
        
        /*if (chdir(file_name) == -1)
            oops("can't enter", file_name);*/
        //改变目录后复制上一个文件夹的文件会出错
        
        mkdir(dst_path, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);
        
        while((dirent_ptr = readdir(dp)) != NULL){
            char * file = dirent_ptr -> d_name;
            copy_file(new_path(src_path, file), new_path(dst_path, file));
        }
        closedir(dp);
        return;
    }
    
    int in_fd, out_fd, n_chars;
    char buf[BUFFSIZE];
    
    if((in_fd = open(src_path, O_RDONLY)) == -1)
        oops("Can't open ", src_path);
    
    if((out_fd = creat(dst_path, COPYMODE)) == -1)
        oops("Can't create ", dst_path);
    
    while((n_chars = read(in_fd, buf, BUFFSIZE)) > 0)
        if(write(out_fd, buf, n_chars) != n_chars)
            oops("Write error to ", dst_path);
    if(n_chars == -1)
        oops("Read error from ", src_path);
    
    if(close(in_fd) == -1 || close(out_fd) == -1)
        oops("Error close files","");
}