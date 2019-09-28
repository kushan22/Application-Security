#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "dictionary.h"
#include <string.h>

char *lower_string(const char *s);
char *removePunctuation(char *s);
void display(hashmap_t hashtable[]);
char *trimwhitespace(const char *s);
char *substring(char *s,int start, int end);

int main(int argc, char **argv)
{
    printf("%s\n", "Spell Checker Program");
    if (argc != 3){
        printf("%s\n","Please give two arguments");
        return 0;
    }

    
  
    
    hashmap_t hashtable[HASH_SIZE];
    char *misspelled[MAX_MISSPELLED] = {""};
    if (load_dictionary(argv[2], hashtable))
    {
        display(hashtable);
        printf("%s\n", "All Good");
        FILE *fp;
        fp = fopen(argv[1], "r");
        if (fp == NULL)
        {
            printf("%s\n", "Error, File does not exists");
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
    int i;
    for (i = 0; i < HASH_SIZE; i++)
    {
        hashtable[i] = NULL;
    }
    FILE *fptr;
    fptr = fopen(dictionary_file, "r");
    if (fptr == NULL)
    {
        return false;
    }
    char line[LENGTH+1];
   
  
    while (fgets(line, sizeof(line), fptr))
    {
        
        hashmap_t newNode = malloc(sizeof(node));
        newNode->next = NULL;
        
        strcpy(newNode->word, trimwhitespace(line));
       
        int bucket = hash_function(trimwhitespace(line));
        if (hashtable[bucket] == NULL)
        {
            hashtable[bucket] = newNode;
        }
        else
        {
            newNode->next = hashtable[bucket];
            hashtable[bucket] = newNode;
           
           
        }
       
    }
    
    fclose(fptr);
    return true;
}

bool check_word(const char *word, hashmap_t hashtable[])
{
    
    word = trimwhitespace(word);
    
    int bucket = hash_function(word);
    
    hashmap_t cursor = hashtable[bucket];
    
   
    
    while (cursor != NULL)
    {
        
        
        if (strcmp(lower_string(word),cursor->word) == 0)
        {
           // printf("Found it %s\n",word);
            return true;
        }else{
            cursor = cursor->next;
        }
    
    }
    
    int bucket1 = hash_function(word);
    //printf("%d\n",bucket);
    hashmap_t cursor1 = hashtable[bucket1];
   
    
    while (cursor1 != NULL)
    {
        // printf("%s\n",cursor->word);
        
        if (strcmp(word,cursor1->word) == 0)
        {
          //  printf("Found it %s\n",word);
            return true;
        }else{
            cursor1 = cursor1->next;
        }
        
    }
  
    
    free(cursor);
    free(cursor1);
   
    

    return false;
}

int check_words(FILE *fp, hashmap_t hashtable[], char *misspelled[])
{
    int num_misspelled = 0;
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
                //printf("%s\n",ptr);
            }
            ptr = strtok(NULL, delim);
           
        }
    }


    return num_misspelled;
}

char *lower_string(const char *s)
{
    int c = 0;
    char *t = calloc(LENGTH,sizeof(char));
    
    memcpy(t,s,sizeof(*s));
    while (t[c] != '\0')
    {
        if (t[c] >= 'A' && t[c] <= 'Z')
        {
            t[c] = t[c] + 32;
        }
        c++;
    }
    return t;
}



char *removePunctuation(char *s){
    int i,j;
    int counter = 0,counter1=0;
    for (i = 0; i < strlen(s); i++){
        if (isalpha(s[i])){
            counter = i;
            break;
        }    
    }

    char *firstPart = substring(s,counter,strlen(s));

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
            n1 = n1->next;
        }
    }
    fclose(fp);
}

char *trimwhitespace(const char *str)
{
  char *dummy = calloc(LENGTH,sizeof(char));
  memcpy(dummy,str,sizeof(*str));
  char *end = calloc(LENGTH,sizeof(char));
  

  // Trim leading space
  while(isspace((unsigned char)*dummy)) dummy++;

  if(*dummy == 0)  // All spaces then return the
    return dummy;

  // Trim trailing space
  end = dummy + strlen(dummy) - 1;
  while(end > dummy && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return dummy;
}
 