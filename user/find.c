#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"


void find(char* path, char* name, char** exec_argv){

    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, O_RDONLY)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    while(read(fd, &de, sizeof(de)) == sizeof(de)){

        if(de.inum == 0) continue;
        if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;

        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            fprintf(2, "find: path too long\n");
            break;
        }

        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        if (stat(buf, &st) < 0) {
            fprintf(2, "find: cannot stat %s\n", buf);
            continue;
        }

        if(st.type == T_DIR){
            find(buf, name, exec_argv);
        }
        else{
            if(strcmp(de.name, name) == 0){
                if(exec_argv == 0){
                    printf("%s\n", buf);
                }
                else{
                    char *cmd[32];
                    int k = 0;

                    for(int i=0; exec_argv[i] != 0; i++){
                        cmd[k++] = exec_argv[i];
                    }
                    cmd[k++] = buf;
                    cmd[k] = 0;

                    if(fork() == 0){
                        exec(cmd[0], cmd);
                        fprintf(2, "exec %s failed\n", cmd[0]);
                        exit(1);
                    }
                    //子进程有 exec，不会返回，不会到达这里
                    wait(0);
                }
            }
        }
    }
    close(fd);
}


int main(int argc, char* argv[]){
    if(argc < 3){
        fprintf(2, "Usage: find <directory> <target_file_name>\n");
        exit(1);
    }
    else if (argc == 3){
        char *name = argv[2];
        char *path = argv[1];
        find(path, name, 0);
    }
    else if (argc >= 5 && strcmp(argv[3], "-exec") == 0){
        char *name = argv[2];
        char *path = argv[1];
        char **argv_exec = &argv[4];
        find(path, name, argv_exec);
    }
    else {
        fprintf(2, "Usage: find <dir> <name> [-exec <cmd> [args...]]\n");
        exit(1);
    }
    return 0;
}