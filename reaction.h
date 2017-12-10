#include <stdio.h>
#include <pthread.h>

struct reaction {
	// FILL ME IN
	int no_of_h, no_of_o;
	pthread_mutex_t mutex;
	pthread_cond_t condH;
	pthread_cond_t condO;
	pthread_cond_t reaction_done;
};

void reaction_init(struct reaction *reaction);

void reaction_h(struct reaction *reaction);

void reaction_o(struct reaction *reaction);
