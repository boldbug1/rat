#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 1000

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
    free(finalfile);
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

int main(int argc , char *argv[]){
    if(argc < 2 || argc > 4){
        printf("Error : No arguments passed\nUsage : rat <filename>\n");
        return -1;
    }

    if(strcmp(argv[1],"-a")==0){
        if(argc == 4){
            const char *filename1 = argv[2];
            const char *filename2 = argv[3];
            int ret_code = concat(filename1,filename2);
            if(ret_code == -1){
                printf("Error : %s does not exist",filename1);
                return -1;
            }
            if(ret_code == -2){
                printf("Error : %s does not exist",filename2);
                return -1;
            }
        }else{
            printf("Usage : rat -a <filename1> <filename2>\n");
            return -1;
        }
    }else if(strcmp("-n",argv[1]) == 0){
        if(argc == 3){
            const char *filename = argv[2];
            char *filecontent = get_filecontent(filename);
             if(filecontent == NULL){
                printf("Error : Could not read file %s\n",filename);
             }
            char *numbered_text = numbered_out(filecontent);
            printf("\n%s\n",numbered_text);
            free(filecontent);
        }else{
            printf("Usage : rat -n <filename>");
        }
    }else{
        const char *filename = argv[1];
        char *filecontent = get_filecontent(filename);
        if(filename){
            if(filecontent){
                if(createfile(filename,filecontent) == -1){
                    printf("Error while creating file");
                    return -1;
                }
            }
        }
        if(strlen(filecontent) != 0){
            printf("%s",filecontent);
        }
        free(filecontent);
    }
    
   
    return 0;
}