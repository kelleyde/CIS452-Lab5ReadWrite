// Only Need One Read File
// #include <stdio.h>  
// #include <stdlib.h>  
// #include <sys/types.h>  
// #include <sys/stat.h> 
// #include <sys/ipc.h>  
// #include <sys/shm.h> 
// #include <sys/ipc.h>
// #include <stdbool.h>

// bool test_and_set(bool *target);

// int main() {
//     bool lock = false;
//     do {
//         while (test_and_set(&lock)) { } // do nothing
//         /* critical section */
//         lock = false;
//         /* remainder section */
//     } while (true);
// }

// bool test_and_set (bool *target)
// {
// bool returnValue = *target;
// *target = true;
// return returnValue;
// }