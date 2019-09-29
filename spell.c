#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "dictionary.h"
#include <string.h>

// char *lower_string(const char *s);
char *removePunctuation(char *s);
void display(hashmap_t hashtable[]);
char *substring(char *s,int start, int end);
bool unload(hashmap_t hashtable[]);

int main(int argc, char **argv)
{

    
    printf("%s\n", "Spell Checker Program");
    if (argc != 3){
        printf("%s\n","Please give two arguments");
        return 0;
    }

    
  
    
    hashmap_t hashtable[HASH_SIZE] = {NULL};
    char *misspelled[MAX_MISSPELLED] = {""};
    if (load_dictionary(argv[2], hashtable))
    {
       // display(hashtable);
        printf("%s\n", "All Good");
        FILE *fp;
        fp = fopen(argv[1], "r");
        if (fp == NULL)
        {
            printf("%s\n", "Error, File does not exists");
            unload(hashtable);
        }
        else
        {
            int num = check_words(fp, hashtable, misspelled);
            printf("%d\n", num);
        }
        fclose(fp);
    }
    else
    {
        printf("%s\n", "Not Good");
    }

   
   
    return 0;
}

bool load_dictionary(const char *dictionary_file, hashmap_t hashtable[])
{
    
    FILE *fptr = fopen(dictionary_file, "r");
    if (fptr == NULL)
    {
        return false;
    }
    char line[LENGTH+1];

    while(fscanf(fptr,"%s\n",line) != EOF){
       // printf("%s\n",line);
        hashmap_t newNode = malloc(sizeof(node));
        strcpy(newNode->word,line);

        int bucket = hash_function(line);
        if (hashtable[bucket] == NULL){
            hashtable[bucket] =  newNode;
            newNode->next = NULL;
        }else{
            newNode->next = hashtable[bucket];
            hashtable[bucket] = newNode;
        }
    }
    
    
    
    fclose(fptr);
    
    return true;
}

bool check_word(const char *word, hashmap_t hashtable[])
{
    
    char temp[LENGTH+1];
    
    int len = strlen(word);
    int i = 0;
    for (i=0 ; i < len; i++){
        temp[i] = tolower(word[i]);
    }
    temp[len] = '\0';


    int bucket = hash_function(temp);
    if (hashtable[bucket] == NULL){
        return false;
    }
    hashmap_t cursor = hashtable[bucket];
    
    while (cursor != NULL)
    {
        
        
        if (strcmp(temp,cursor->word) == 0)
        {
            return true;
        }else{
            cursor = cursor->next;
        }
    
    }
    
    
    
    free(cursor);
    
    

    return false;
}

int check_words(FILE *fp, hashmap_t hashtable[], char *misspelled[])
{
    int num_misspelled = 0;
    int spelledCorrectly = 0;
    char line[LENGTH + 1];
    char delim[] = " ";

    //printf("%s\n",hashtable[8]->word);
    
    while (fgets(line, sizeof(line), fp))
    {
        char *ptr = strtok(line, delim);
       // printf("%s\n",ptr);
        int i = 0;
       
       
        while (ptr != NULL)
        {

            ptr = removePunctuation(ptr);
           
           // printf("%s\n",ptr);
            if (!check_word(ptr, hashtable))
            {
                
                misspelled[i] = ptr;
               // printf("Misspelled word is %s\n",misspelled[i]);
                num_misspelled++;
                i++;
            }else{
               // printf("%s\n",ptr);
                spelledCorrectly++;
            }
            ptr = strtok(NULL, delim);
           
        }

        free(ptr);
    }
    
    
    return num_misspelled;
}





char *removePunctuation(char *t){
   
    int i,j;
    int counter = 0,counter1=0;
    for (i = 0; t[i] != '\0'; i++){
        if (isalpha(t[i])){
            counter = i;
            break;
        }    
    }

    char *firstPart = substring(t,counter,strlen(t));

    for (j = strlen(firstPart)-1;j >= 0; j--){
        if (isalpha(firstPart[j])){
            counter1 = j;
            break;
        }
    }

    char *secondPart = substring(firstPart,0,counter1);



    return secondPart;
}

char *substring(char *s,int start,int end){
    int c = 0;
    char *sub = malloc(sizeof(char) * LENGTH+1);
    while(c < end - start + 1){
        sub[c] = s[start + c];
        c++;
    }
    return sub;
}

void display(hashmap_t hashtable[]){
    int i = 0;
    node * n1 = NULL;
    FILE *fp;
    fp = fopen("dummy.txt","a");
    for (i = 0; i < HASH_SIZE; i++){
        n1 = hashtable[i];
        while(n1 != NULL){
          //  printf("%d %s\n",i,n1->word);
            char * text = n1->word;
            fputs(text,fp);
            fputs("\n",fp);
            n1 = n1->next;
        }
    }
    fclose(fp);
}

bool unload(hashmap_t hashtable[]){
    for (int i = 0; i < HASH_SIZE; i++){
        hashmap_t cursor = hashtable[i];
        while(cursor != NULL){
            hashmap_t temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    return true;
}




 