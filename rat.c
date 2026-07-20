#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#define BUFFER_SIZE 4096 //4kb buffer size

typedef struct {
    int show_numbers;
} RatConfig;



RatConfig parse_flags(int argc , char **argv);
int process_files(const char *filename,int *line_counter,int *is_start_of_line,RatConfig config);


int main(int argc , char *argv[]){
    int line_num = 1;
    int is_start_of_line = 1;
    
    RatConfig config = parse_flags(argc,argv);

    if (optind >= argc) {
        process_files("-",&line_num,&is_start_of_line,config);
    }else{
        for(int i = optind;i < argc;i++){
            process_files(argv[i],&line_num,&is_start_of_line,config);
        }
    }
    
    return 0;
}

//Thanks to _dl and NgTV for suggestion for making it this optimized.

RatConfig parse_flags(int argc , char **argv){
    int option;
    RatConfig config = {0};
    while((option = getopt(argc,argv,"n")) != -1){
        switch (option)
        {
        case 'n':
            config.show_numbers = 1;        
            break;
        default:
            fprintf(stderr,"Usage : rat [-n] <filename> <filename2>....\n");
            exit(EXIT_FAILURE);
            break;
        }
    }

    return config;
}

int process_files(const char *filename,int *line_counter,int *is_start_of_line,RatConfig config){
    FILE *fp;
    int is_stdin = (filename[0] == '-' && filename[1] == '\0');

    if(is_stdin){
        fp = stdin;
    }else{
        fp = fopen(filename,"r");
        if(fp == NULL){
            fprintf(stderr,"rat: %s: %s\n",filename,strerror(errno));
            return -1;
        }
    }

    char buffer[BUFFER_SIZE]; 


    while(fgets(buffer,sizeof(buffer),fp) != NULL){
        if(config.show_numbers){
            if(*is_start_of_line){
                printf("%4d ", (*line_counter)++);
                *is_start_of_line = 0;
            }

            fputs(buffer,stdout);

            size_t len = strlen(buffer);
            if(len > 0 && buffer[len - 1] == '\n') {
                *is_start_of_line = 1;
            }
        }else{
            fputs(buffer,stdout);
        }
    }

    if(!is_stdin){
        fclose(fp);
    }
}