#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

char *lower_string(char *s);
char *removePunctuation(char *s);
char *substring(char *s,int start, int end);
int main(){
    char s[] = "     KUSHA'N   ??";
    
    char *filteredString = removePunctuation(s);
    printf("%s\n",lower_string(filteredString));


    return 0;
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
    char *sub = malloc(strlen(s));
    while(c < end - start + 1){
        sub[c] = s[start + c];
        c++;
    }
    return sub;
}

char *lower_string(char *s)
{
    int c = 0;

    while (s[c] != '\0')
    {
        if (s[c] >= 'A' && s[c] <= 'Z')
        {
            s[c] = s[c] + 32;
        }
        c++;
    }
    return s;
}