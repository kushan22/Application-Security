#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "dictionary.h"
#include <string.h>

// char *lower_string(const char *s);
char *removePunctuation(char *s);
// void display(hashmap_t hashtable[]);
char *substring(char *s, int start, int end);
bool unload(hashmap_t hashtable[]);
bool unloadMisspelled(char *mispelled[],int num);
int main(int argc, char **argv)
{

   
    if (argc < 3)
    {
        printf("%s\n", "Please give two arguments");
        return 0;
    }

    hashmap_t hashtable[HASH_SIZE];
    char *misspelled[MAX_MISSPELLED];
    load_dictionary(argv[2], hashtable);
       
       
    FILE *fp;
    fp = fopen(argv[1], "r");
    int num = check_words(fp, hashtable, misspelled);
    printf("%d\n",num);
    // for (int i = 0; i < num; i++){
    //     printf("%s\n",misspelled[i]);
    // }
     for (int i = 0; i < num; i++){
        printf("%s\n",misspelled[i]);
    }
  
    if(unload(hashtable)){
        printf("%s\n","Successfully Unloaded");
    }
    if (unloadMisspelled(misspelled,num)){
        printf("%s\n","Misspelled Characters successfully Unloaded");
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
    for (int i = 0; i < HASH_SIZE; i++){
        hashtable[i] = NULL;
    }
    
    char line[LENGTH + 1];
    
    
    while (fgets(line, sizeof(line), fptr))
    {
        int len = strlen(line);
        if (line[len - 1] == '\n')
        {
            line[len - 1] = '\0';
        }
        for (int i = 0;i < len; i++){
            line[i] = tolower(line[i]);
        }
        line[len] = '\0';
       
       // printf("%s\n",line); realloc(base, strlen(base) + strlen(to_add) + 1);
       
        if (strlen(line) <= LENGTH)
        {   
           hashmap_t newNode = malloc(sizeof(node));
          
           strncpy(newNode->word, line, sizeof(line));

            int bucket = hash_function(line);
            if (hashtable[bucket] == NULL)
            {
                hashtable[bucket] = newNode;
                newNode->next = NULL;
            }
            else
            {
                newNode->next = hashtable[bucket];
                hashtable[bucket] = newNode;
            }
         
        }
        else
        {
               
            return false;
        }
      
    }
   

    
    fclose(fptr);
   
    return true;
}

bool check_word(const char *word, hashmap_t hashtable[])
{

   
    char temp[LENGTH + 1];

    int len = sizeof(word);
    int i = 0;
    for (i = 0; i < len; i++)
    {
        temp[i] = tolower(word[i]);
    }
    temp[len] = '\0';
    //printf("%ld %s\n",strlen(temp),temp);
    int bucket = hash_function(temp);
    if (hashtable[bucket] == NULL)
    {
        return false;
    }
    hashmap_t cursor = hashtable[bucket];

    while (cursor != NULL)
    {

        if (strcmp(temp, cursor->word) == 0)
        {
            return true;
        }
        else
        {
            cursor = cursor->next;
        }
    }

   
   
    return false;
}

int check_words(FILE *fp, hashmap_t hashtable[], char *misspelled[])
{

    if (fp == NULL){
        return -1;
    }

    int num_misspelled = 0;
    
    char line[100];
    

    //printf("%s\n",hashtable[8]->word);

    int i = 0;
    while (fgets(line, sizeof(line), fp))
    {
        int len = strlen(line);
        if (line[len - 1] == '\n')
        {
            line[len - 1] = '\0';
        }
        
        if (len <= 100)
        {
            char *ptr = strtok(line, " ");

            while (ptr != NULL)
            {

                char *trimmed = removePunctuation(ptr);
                //printf("%s\n",trimmed);
                if (!check_word(trimmed, hashtable))
                {

                      printf("%s\n",trimmed);
                      misspelled[i] = trimmed;
                      num_misspelled++;
                    
                   
                     // printf("%ld:%s\n",strlen(misspelled[i]),misspelled[i]);
                   
                   
                    i++;
                }else{
                    free(trimmed);
                }
            
                ptr = strtok(NULL, " ");
                
                
            }

           
           
        }else{
            return -1;
        }

        
    }
    fclose(fp);
    return num_misspelled;
}


char *removePunctuation(char *t)
{

    int i, j;
    int counter = 0, counter1 = 0;
    for (i = 0; t[i] != '\0'; i++)
    {
        if (isalpha(t[i]))
        {
            counter = i;
            break;
        }
    }

    char *firstPart = substring(t, counter, strlen(t));

    for (j = strlen(firstPart) - 1; j >= 0; j--)
    {
        if (isalpha(firstPart[j]))
        {
            counter1 = j;
            break;
        }
    }

    char *secondPart = substring(firstPart, 0, counter1);

    free(firstPart);
    return secondPart;
}

char *substring(char *s, int start, int end)
{
    int c = 0;
    
    char *sub = malloc(LENGTH+1);
    while (c < end - start + 1)
    {
        sub[c] = s[start + c];
        c++;
    }
    sub[c] = '\0';
   // printf("%s\n",sub);
    return sub;
}

// void display(hashmap_t hashtable[])
// {
//     int i = 0;
//     node *n1 = NULL;
//     FILE *fp;
//     fp = fopen("dummy.txt", "a");
    
//     for (i = 0; i < HASH_SIZE; i++)
//     {   
        
//         n1 = hashtable[i];
//         while (n1 != NULL)
//         {
//             //  printf("%d %s\n",i,n1->word);
//             char *text = n1->word;
           
//             fputs(text, fp);
//             fputs("\n", fp);
//             n1 = n1->next;
//         }
//     }
//     fclose(fp);
// }

bool unload(hashmap_t hashtable[])
{
    for (int i = 0; i < HASH_SIZE; i++)
    {
        hashmap_t cursor = hashtable[i];
        while (cursor != NULL)
        {
            hashmap_t temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    return true;
}

bool unloadMisspelled(char *misspelled[],int num){
    for (int i = 0; i < num; i++){
        free(misspelled[i]);
    }
    return true;
}
