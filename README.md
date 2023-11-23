# Multithread_Matrix_Addition

This C program uses 8 threads to add two identical 256x256 matrices using 1-D data partitioning. Each thread *i* adds the block composed of rows 32 * *i* to 32 * *i* + 31. The threads each print "Thread *i*: Done" in decreasing order when they have completed their local sums. The main thread generates the initial matrices and prints the final result into a file called resutl.txt.
