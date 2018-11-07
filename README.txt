***********************************************************************
*  Password cracker with both dictionary attack and bruteforce attack *
*  Written by: Sander Sjøthun                                         *
*  Date: 09.11.2018                                                   *
***********************************************************************
 How to compile:
 gcc cracker.c DA.c BFA.c -o cracker -lcrypt -fopenmp

 How to run:
 ./cracker 'hash'

 How to create hashes:

 ./encrypt "password"

Development history:
1. Startet out with just getting barebones done. After the dictionary attack and bruteforce attack
   functions were done, the need for speed became aperant.
2. Converting my code to multithreading with pthreads. It became a mess I spent valuable time trying
   to fix it.
3. Discovered OpenMP, a intel API for pthreads. The converting of my barebones code was suprisingly
   easier than I thought. My biggest hurdle was changing my while loops to for loops as OpenMP don't like them.
4. TODO: Fixing memory leak of 2288 bytes, and create a make file.