#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER_SIZE 4096//4kb buffer size

typedef struct {
    int show_numbers;
} RatConfig;



RatConfig parse_flags(int argc , char **argv);
void process_files(const char *filename,int *line_counter,RatConfig config);



/*
to-do :
move cli arguments parsing and error handling logic from main to seperate functions [done]
use 4kb buffer to ouput file instead of storing them in memory [done]
*/

//to-do :
//make flags more similer to cat []


int main(int argc , char *argv[]){
    int line_num = 1;
    
    RatConfig config = parse_flags(argc,argv);

    if (optind >= argc) {
        process_files("-",&line_num,config);
    }else{
        for(int i = optind;i < argc;i++){
            process_files(argv[i],&line_num,config);
        }
    }
    
    return 0;
}

//as per _dl request createfile is finally removed XD


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
            printf("Usage : ./rat [-n] <file> <file2>...\n");
            exit(EXIT_FAILURE);
            break;
        }
    }

    return config;
}

void process_files(const char *filename,int *line_counter,RatConfig config){
    FILE *fp = fopen(filename,"r");
    int is_stdin = (filename[0] == '-' && filename[1] == '\0');

    if(is_stdin){
        fp = stdin;
    }else{
        fp = fopen(filename,"r");
        if(fp == NULL){
            fprintf(stderr,"Error : Could not open file %s\n",filename);
            return;
        }
    }


    char buffer[BUFFER_SIZE];
 
    int is_new_line = 1;

    while(fgets(buffer,sizeof(buffer),fp) != NULL){
        if(config.show_numbers){
            if(is_new_line){
                printf("\033[33m%4d \033[0m", (*line_counter));
                is_new_line = 0;
            }

            printf("%s",buffer);

            size_t len = strlen(buffer);
            if(len > 0 && buffer[len - 1] == '\n') {
                (*line_counter)++;
                is_new_line = 1;
            }else{
                is_new_line = 0;
            }
        }else{
            printf("%s",buffer);
        }
    }

    if(!is_new_line){
        printf("\n");
        if(config.show_numbers){
            (*line_counter)++;
        }
    }

    if(!is_stdin){
        fclose(fp);
    }
}