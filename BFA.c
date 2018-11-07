#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <omp.h>
#include <crypt.h>

static void recursive(char* current_guess, int index, int guess_length, char* alphabet, int* alphabetSize, char* salt, char* hash);
static const char alphabet[] =   "abcdefghijklmnopqrstuvwxyz"
                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "0123456789"
                    "#&/()=?!@$|[]{}";
static int alphabetSize;
time_t gStartTime, gEndTime;
int omp_get_num_threads(void);

void bruteforce_attack(char* salt, char* hash) {
    int max_length = 9; // set to 9 due to time constraint.
    omp_set_dynamic(0);     // Disable dynamic teams

    // Timer start
    time( &gStartTime );

    printf("Please select the length from 1 to 9 of the password you want to crack:\n");
	int set_guess_length = scanf("%d", &set_guess_length);

    for(int CURRENT_GUESS_LENGTH = set_guess_length; CURRENT_GUESS_LENGTH <= max_length; ++CURRENT_GUESS_LENGTH ) {
        // Create teams with 2 threads
        #pragma omp parallel private(alphabetSize) shared(salt,hash) num_threads(2)
        {
            alphabetSize = sizeof(alphabet) - 1;
            // Make alphabet private so there is no race condition. Probably a better way in openMP.
            char* threadAlphabet = malloc(sizeof(alphabet));
            strncpy(threadAlphabet, alphabet, alphabetSize);

            // single = only one thread will do this, nowait = other threads can skip and continue
            #pragma omp single nowait
            for(int i = 0; i < alphabetSize; ++i ) {
                // Store the str_inis in memory
                char* str_ini = malloc(CURRENT_GUESS_LENGTH + 1);
                str_ini[0] = alphabet[i];
                for(int j = 1; j <= CURRENT_GUESS_LENGTH; j++)
                    str_ini[j] = '\0'; // set string to NULL

                // At this point there will be #77 tasks to be solved.
                // Each team member can be assigned to a task.
                // PS: Not NUMA friendly, but fast as no new threads have to be created.
                #pragma omp task
                {
                    // debug
                    printf("THREAD %d AT str_ini = %s\n", omp_get_thread_num(), str_ini);
                    double tOmpStartTime = omp_get_wtime();

                    // Start the iteration of the recursive function
                    recursive(str_ini, 1, CURRENT_GUESS_LENGTH, &*threadAlphabet, &alphabetSize, salt, hash);

                    // More debug
                    double tOmpEndTime = omp_get_wtime();
                    double tDifTime = tOmpEndTime-tOmpStartTime;
                    printf("THREAD %d finished: Time: \t %f \n", omp_get_thread_num(), tDifTime);

                    // Memory free
                    free(str_ini);
                }
            }
        }
    }
    // Timer stopped
    time( &gEndTime );
    printf("Elapsed time: %lds", gEndTime-gStartTime);
    puts( "Password not found.");
    return;
}

void recursive(char* current_guess, int index, int guess_length, char* alphabet, int* alphabetSize, char* salt, char* hash) {
    struct crypt_data data;
    data.initialized = 0;
    // Loop through all letters to add them to the guess string.
    for (int i = 0; i < *alphabetSize; ++i) {
        current_guess[index] = alphabet[i];
        if (index == guess_length - 1) {
            char *guess = (char*)crypt_r(current_guess,salt, &data);
                    if(strcmp(guess,hash) == 0){
                        printf("Found Password: %s\n",current_guess);
                        time( &gEndTime );
                        printf("Elapsed time: %lds\n", gEndTime-gStartTime);
                        exit(0);
                    }
        }
        // Increase index + recursive
        else recursive(current_guess, index + 1, guess_length, alphabet, alphabetSize, salt, hash);
    }
}