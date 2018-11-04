#include <stdio.h>
#include <stdlib.h>

int get_dimension (FILE* fp, int* flag)
{
    char ch, buffer[16];
    int dim;

    ch=fgetc(fp);
    if (ch == '@')
    {
        sscanf(buffer, "%*[2,3,4]%d", &dim);
        *flag = 1;
    }
    else
        dim = 2; //default dimension

    return dim;
}


int *get_conf(char* conf_fname){
    char buffer[64];

    int value, i = 0;
    int* config = (int *)(malloc(3 * sizeof(int)));

    FILE *fp = fopen(conf_fname, "r");
    if (fp == NULL)
    {
        printf("Couldn't open configuration file.");
        return NULL;
    }
     while(fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        sscanf(buffer, "%*s %d", &value);
        config[i] = value;
        i++;

    }
    if (i == 1){ //default values
        config[1] = 2;
        config[2] = 3;
    }
    else if (i == 2)
        config [2] = 3;

    return config;
}

int get_flag(int *centers, int *new_centers, int k)
{
    int i, j;
    int *flags = calloc(k, sizeof(int));

    for (i = 0; i < k; i++){
        for (j = 0; j < k; j++){
            if(centers[i] == new_centers[j]){
                flags[i] = 1;
                break;
            }
        }
    }
    int flag1 = 1;
    for(i = 0; i < k; i++){
        if(flags[i] == 0){
            flag1 = 0;
            break;
        }
    }

    free(flags);

    return flag1;
}
