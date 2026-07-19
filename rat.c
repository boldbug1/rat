#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER_SIZE 4096//4kb buffer size

typedef struct {
    int show_numbers;
    int append_mode;
    char *output_file;
} RatConfig;

int createfile(const char *filename,const char *filecontent);
char *get_filecontent(const char *filename);
char *numbered_out(const char *filecontent);
int new_line_count(const char *filecontent);
RatConfig parse_flags(int argc , char **argv);
void process_files(const char *filename,int *line_counter,RatConfig config);
int concat(const char *filename1,const char *filename2);


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
    if(argc < 2){
        printf("Usage : ./rat [-n] [-a] <file> <file2>\n");
        return -1;
    }

    if (optind >= argc) {
        fprintf(stderr, "Error: Expected at least one filename.\n");
        fprintf(stderr, "Usage: ./rat [-n] [-a] [-o output_file] <file1> [file2 ...]\n");
        return -1;
    }

    if(config.output_file!=NULL){
        const char *mode = config.append_mode ? "a" : "w";
        if(freopen(config.output_file,mode,stdout) ==NULL){
            fprintf(stderr, "Error: Could not redirect output stream to file %s\n", config.output_file);
            return -1;
        }
    }
    
    for(int i = optind;i < argc;i++){
        process_files(argv[i],&line_num,config);
    }
    return 0;
}

int createfile(const char *filename,const char *filecontent){
    FILE *fp;
    fp = fopen(filename,"w");
    if(!fp){
        return -1;
    }
    fprintf(fp,"%s",filecontent);
    fclose(fp);
    return 0;

}

char *get_filecontent(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);

    char *content = malloc(file_size + 1);
    if (!content) {
        fclose(fp);
        return NULL; 
    }

    size_t read_size = fread(content, sizeof(char), file_size, fp);
    content[read_size] = '\0';

    fclose(fp);
    return content;
}

int concat(const char *filename1,const char *filename2){
    char *filecontent1 = get_filecontent(filename1);
    if (filecontent1 == NULL) {
        printf("Error: getfilecontent returned an error status (-1)\n");
        return -1;
    }
    char *filecontent2 = get_filecontent(filename2);
    if(filecontent2 == NULL){
        printf("Error: getfilecontent returned an error status (-1)\n");
        free(filecontent1);
        return -1;
    }
    size_t finalsize = strlen(filecontent1) + strlen(filecontent2);
    char finalfile[finalsize + 1];
    int i = 0;
    int char_pointer = 0;
    while(filecontent1[i] != '\0'){
        finalfile[char_pointer++] = filecontent1[i++];
    }
    i = 0;
    while (filecontent2[i] != '\0')
    {
        finalfile[char_pointer++] = filecontent2[i++];
    }

    finalfile[char_pointer] = '\0';

    printf("%s",finalfile);
    free(filecontent1);
    free(filecontent2);
    return 0;
    
}

int new_line_count(const char *filecontent){
    int n =0;
    if(strlen(filecontent) == 0){
        return 0;
    }
    for(int i =0;filecontent[i] != '\0';i++){
        if(filecontent[i] == '\n'){
            n++;
        }
    }
    return n;
}

char *numbered_out(const char *filecontent){
    int line_num = 1;
    int j = 0;
    char *numbered_out = malloc(strlen(filecontent) + ((4 * sizeof(char)) * new_line_count(filecontent)));
    if(!numbered_out)return NULL;
    char *output_ptr = numbered_out;
    while(filecontent[j] != '\0'){
        int written = sprintf(output_ptr, "\033[33m%4d \033[0m", line_num);
        output_ptr+=written;
        while(filecontent[j] != '\n' && filecontent[j] != '\0'){
            *output_ptr++ = filecontent[j++];
        }
        *output_ptr++ = '\n';
        if(filecontent[j] == '\n'){
            j++;
        }
        line_num++;
    }
    *output_ptr = '\0';
    return numbered_out;
}

RatConfig parse_flags(int argc , char **argv){
    int option;
    RatConfig config = {0,0,NULL};
    while((option = getopt(argc,argv,"ano:")) != -1){
        switch (option)
        {
        case 'n':
            config.show_numbers = 1;        
            break;
        case 'a':
            config.append_mode = 1;
            break;
        case 'o':
            config.output_file = optarg;
            break;
        default:
            printf("Usage : ./rat [-n] [-a] <file> <file2>\n");
            exit(EXIT_FAILURE);
            break;
        }
    }

    return config;
}

void process_files(const char *filename,int *line_counter,RatConfig config){
    FILE *fp = fopen(filename,"r");
    if(fp == NULL){
        fprintf(stderr,"Error : Could not open file %s\n",filename);
        return;
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
            }
        }else{
            printf("%s",buffer);
        }
    }

    if(!is_new_line && config.show_numbers || !is_new_line && config.append_mode){
        printf("\n");
        (*line_counter)++;
    }

    fclose(fp);
}