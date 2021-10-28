// Header file for corresponding uthread.c


// So that ucontext.h can be included below
#define _XOPEN_SOURCE
#define _GNU_SOURCE

#include <ucontext.h>

// This enum (which just assigns int values to the variables inside)
// gives the possible states of a thread
enum thread_state {
	T_ACTIVE, 	// 0 Thread is running
	T_READY,	// 1 Thread is able to run, waiting for CPU)
	T_BLOCKED,	// 2 Thread is not able to run, waiting on something
	T_DEAD		// 3 Thread is done running
};
typedef enum thread_state thread_state;

// The actual thread struct
struct uthread_t {
	unsigned int tid; 	// ID number of this thread, should be unique
	ucontext_t *ctx; 	// ucontext is a type that we do not define, it is the current context of the thread
	enum thread_state state; // state of the thread (see enum thread_state above)
	int joining_tid; // The TID Of the thread that is joining with this thread ("blocked on me")
};
typedef struct uthread_t uthread_t;


/* You will implement these five methods */
void uthread_init();

uthread_t *uthread_create(void *(*func)(void *), void *argp);

int uthread_get_id(void);

int uthread_yield(void);

int uthread_join (uthread_t thread);
/* You will implement these five methods */



// Below is the linked list stuff
struct node {
	struct node *next;
	struct uthread_t *t;
};
typedef struct node node;

node *head;
node *tail;

