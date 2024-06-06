# Cache-Simulator

Name: TASEEN HAKIM

Run the program by using the makefile

MAKEFILE:

all: cache.c
    gcc -std=c99 -Wall -o cache cache.c
 
run: cache  
    ./cache
 
clean:  
    rm -f cache
    

Use the following command:

./cache -s 4 -E 1 -b 4 -t traces/yi.trace

Output:

hits:2 misses:5 evictions:3

Screenshots included of some of the output below:

Some resources used:

How to parse in c:
https://www.freecodecamp.org/news/strcmp-in-c-how-to-compare-strings-in-c/

https://www.geeksforgeeks.org/strcmp-in-c/


FIFO method: approach in programming using c:
https://www.geeksforgeeks.org/fifo-first-in-first-out-approach-in-programming/

Using timestamp:
https://stackoverflow.com/questions/26235345/write-complex-structures-on-a-fifo

https://www.geeksforgeeks.org/cache-memory-in-computer-organization/






