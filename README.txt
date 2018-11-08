*********************************************************************************
*  MD5 hash1 password cracker with both dictionary attack and bruteforce attack *
*  Written by: Sander Sjøthun                                                   *
*  Date: 09.11.2018                                                             *
*********************************************************************************
How to compile:
 make

How to run:
 ./cracker 'hash'

How to debug:
 make dbcracker                // for debugging
 (valgrind) ./dbcracker 'hash' // () = optional

How to create hashes:
 make encrypt           // for hash generator
 ./encrypt "password"   // Please try to keep it below 5 chars.

How to clean:
 make clean        // for main program
 make cleandb      // for debug compiled program
 make cleanencrypt // for hash generator

Misc:
Have set max password to 9 chars, longer will just take to much time.
The number of threads are default to the number of cores on your (virtuel)machine.
The warning from scanf() in bruteforce return can be ignored as it returns to void.

Development history:
1. Startet out with just getting barebones done. After the dictionary attack and bruteforce attack
   functions were done, the need for speed became aperant.
2. Some refractoring of my code with splitting up the dictionary and bruteforce part. Made some part of the
   code into function ie; getlines and format. 
3. Converting my code to multithreading with pthreads. It became a mess and I spent valuable time trying
   to fix it. The positive part was that I learned a little bit of true C programming.
4. Discovered OpenMP, an Intel API for pthreads. The converting of my barebones code was suprisingly
   easier than I thought. My biggest hurdle was changing my while loops to for loops as OpenMP don't like them.
   Second caveat was using omp task in bruteforce so that the threads could keep the previous guess string from recursion.
   Biggest negative with OpenMP is that the programmer gets less control over the program. See below where this comes true.
5. Adding comments and make file.

Important! Valgrind:
The OpenMP API will create false positive memory leaks in its thread handeling.
When running valgrind, the summary will show the GOMP_parallel and GOMP_task start_thread, clone, and calloc and malloc dl-tls, 
but as far as I know, don't free them. This will make valgrind report multiple errors. 

Sources:
https://gcc.gnu.org/bugzilla/show_bug.cgi?id=36298
https://stackoverflow.com/questions/6973489/valgrind-and-openmp-still-reachable-and-possibly-lost-is-that-bad?noredirect=1&lq=1
https://stackoverflow.com/questions/26391389/compiling-with-openmp-results-in-a-memory-leak?noredirect=1&lq=1
