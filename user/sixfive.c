#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

const char* seperators = " -\r\t\n./,";

int main(int argc, char* argv[]){
    
    for(int i=1; i < argc; i++){
        int f = open(argv[i], O_RDONLY);
        if (f == -1){
            fprintf(2, "sixfive: cannot open %s\n", argv[i]); 
            return -1;
        }

        char c;
        char buf[32];
        int len=0;
        int is_valid_num = 1;

        while(read(f, &c, sizeof(char)) > 0){
            if(strchr(seperators, c) != 0){
                if(len > 0 && is_valid_num){
                    buf[len] = '\0';
                    int num = atoi(buf);
                    if (num % 5 == 0 || num % 6 == 0) {
                        printf("%d\n", num);
                    }
                }
                len = 0;
                is_valid_num = 1;

            }
            else if(c >= '0' && c <= '9'){
                if(len < sizeof(buf)-1){
                    buf[len++] = c;
                }
                else{
                    fprintf(2, "buf too small\n");
                    exit(1);
                }
            }
            else{
                is_valid_num = 0;
            }
        }
        if(len > 0 && is_valid_num){
            buf[len] = '\0';
            int num = atoi(buf);
            if (num % 5 == 0 || num % 6 == 0) {
                printf("%d\n", num);
            }        
        }
        close(f);
    }
    
    return 0;

}