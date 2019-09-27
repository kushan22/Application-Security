#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(){
    FILE *fp;
    fp = fopen("a_tale_of_two_cities.txt","r");
    char line[46];
    char delim[] = " ";
    while(fgets(line,sizeof(line),fp)){
        char *ptr = strtok(line,delim);
        while(ptr != NULL){
            printf("%s\n",ptr);
            ptr = strtok(NULL,delim);

        }
    }
}