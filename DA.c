#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>
#include <crypt.h>
#include "BFA.h"

// Avoid having newline be in the guess word
void format(char *word){
 int len_word = strlen(word);

 if(word[len_word-1] == '\n'){
    word[len_word-1] = '\0';
 }
}

// The dictionary.txt has newline at every word.
int getnumwords(FILE *file){
    if(file==NULL) {
        return -1;
    }
    char c = 0;
    int lines = 0; 
    while (c != EOF){
        c = fgetc(file);
        if(c == '\n') {
            lines++;    
        }
    }
    return lines;
}

void dictionary_attack(char* salt, char* hash) {
    int found=0; // Solution to the problem of #pragma omp can not have return
    char word[39]; // The longest word in dictionary.
    struct crypt_data data;
    data.initialized = 0;
    FILE* fp = fopen("dictionary.txt","r");
    //File error handling
    if(fp==NULL){
        printf("Could not open the dictionary file.\n");   
        exit(-1);   
    }
    int size = getnumwords(fp); // Number of words in dictionary.txt
    if(fseek(fp,0,SEEK_SET)==-1)    {
        return;    //seek error handling
    }

    // Parallel Region:
    // Each thread will have word private and share the static varibles.
    // With static scheduling, the compiler will split up the loop iterations at compile time.
    // If the iterations roughly take the same amount of time, it will be the most efficient at runtime. 
    #pragma omp parallel for private(word) shared(found, salt, hash) schedule(static) 
        for(int i=0;i<size;i++) {
            if(fgets(word,39,fp) != NULL){
                format(word);
                if(found==1)  {
                    fclose(fp);
                    memset(word, 0, 39); // Null out word
                    exit(0);
                }
                if(i%1000 == 0){
                    printf("THREAD %i IS AT LINE: %d OF %d\n",omp_get_thread_num(), i, size );
                }
                char *guess = (char*)crypt_r(word, salt, &data);
                if(strcmp(guess,hash) == 0) {
                    printf("Found Password: %s\n",word);
                    found=1;
                }
            }
        }
    // End of parallell region
    fclose(fp);
    if(found==0) {
        printf("\nFailed with dictionary attack, continue with bruteforce attack:\n");
        bruteforce_attack(salt, hash);
        memset(word, 0, 39); // Null out word
        return;
    }
    return;
} 