/* driver.c */

#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include "libapportion.h"

unsigned int random(){
	// Returns random numbers based on current system time
	return clock() % 5;
}

int main(){
	printf("The driver program is starting now!!\n");


	// verify the program break location before
	void *b = sbrk(0);
	printf("brk before: %p\n", b);

	// call our implementation of malloc
	short n = 4;
	int *ptr = apportion(sizeof(int) * (n + 1));

	// verify the program break location after
	// You should figure by hand what a should be
	// considering the lines of code up until this
	// point and the value for b that was printed previously
	void *a = sbrk(0);
	printf("brk  after: %p\n", a);


	// Write some values to this memory location
	printf("\nWriting Values: ");
	int i = 0;
	for(i = 0; i < n; i++){
		unsigned int tmp = random();
		ptr[i] = tmp;
		printf("%d ", tmp);
	}
	ptr[i] = sizeof(int) * n;
	printf("\n");


	// Read those same values from memory (verify via printf)
	printf("Reading Values: ");
	for(i = 0; i < n; i++){
		printf("%d ", ptr[i]);
	}
	printf("\n\n");

	// Sneak a peak at the meta-data
	// This is pretty taboo!!1!
	printf("peaking at meta-data   ptr[-1]: %d\n", ptr[-1]);


	// uncomment these lines for part 3

	// call our implementation of free
	release(ptr);

	// check program break at the end
	void *end = sbrk(0);
	printf("\nbrk end:    %p\n", end);
	assert(end == b);
}