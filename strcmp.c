#include<stdio.h>
#include<string.h>
#include<ctype.h>

int strcmpi(char* s1, char* s2){
    int i;
     
    if(strlen(s1)!=strlen(s2))
        return -1;
         
    for(i=0;i<strlen(s1);i++){
        if(toupper(s1[i])!=toupper(s2[i]))
            printf("%c %d\n",s1[i],s1[i]);
            printf("%c %d\n",s2[i],s2[i]);
            return s1[i]-s2[i];
    }
    return 0;
}

int main(){
    printf("%d",strcmpi("akash","lucky"));
}