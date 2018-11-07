/**
 *  Password cracker with both dictionary attack and bruteforce attack
 *  Written by: Sander Sjøthun
 *  Date: 09.11.2018
 * */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DA.h"

int main(int argc, char* argv[]) {
    if (argc != 2)
    {
        printf("Usage: ./crack 'hash'\n");
        exit(-1);
    }
    char hash[35]; // 34 = The length of the hash.
    char salt[13];
    memset(hash, 0, 34+1);
    memset(salt, 0, 12+1);
    memcpy(hash,argv[1],34*sizeof(char));
    strncpy(salt,hash,12*sizeof(char));
    printf("Searching for hash: %s\n", hash);
    dictionary_attack(salt, hash);
    
    return 0;
}