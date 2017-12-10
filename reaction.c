#include <pthread.h>
#include "reaction.h"

// Forward declaration. This function is implemented in reaction-runner.c,
// but you needn't care what it does. Just be sure it's called when
// appropriate within reaction_o()/reaction_h().
void make_water();

//initialize the struct variables and reset the counters to 0
void reaction_init(struct reaction *reaction)
{
	// FILL ME IN
	reaction->no_of_h = 0;
	reaction->no_of_o = 0;
	pthread_mutex_init(&(reaction->mutex), NULL);
	pthread_cond_init(&(reaction->condH), NULL);
	pthread_cond_init(&(reaction->condO), NULL);
}

//this function is to create a new hydrogen atom and increment the counter by 1
//and checks if there is sufficient H and O atoms to make the water reaction
void reaction_h(struct reaction *reaction)
{
	//aquire the lock
	pthread_mutex_lock( &reaction->mutex );

	reaction->no_of_h++;

	if(reaction->no_of_h < 2 || (reaction->no_of_o)<1){
		pthread_cond_wait(&reaction->condO,&reaction->mutex);
	}

	//if there is sufficient H and O atoms make water and signal the waited threads
	//on the condition condO and condH and decrement the counters 
	else{
		pthread_cond_signal(&reaction->condO);
		pthread_cond_signal(&reaction->condH);
		reaction->no_of_h -= 2;
		reaction->no_of_o -= 1;
		make_water();
	}

  	// release the lock  
	pthread_mutex_unlock( & reaction->mutex );

}

//this function is to create a new oxygen atom and increment the counter by 1
//and checks if there is sufficient H and O atoms to make the water reaction
void reaction_o(struct reaction *reaction)
{
	//aquire the lock
	pthread_mutex_lock( &reaction->mutex );

	reaction->no_of_o++;

	//if there is sufficient H atoms make water and signal the waited threads
	//on the condition condO and decrement the counters 
	if(reaction->no_of_h >= 2){
		pthread_cond_signal(&reaction->condO);
		pthread_cond_signal(&reaction->condO);
		reaction->no_of_h -= 2;
		reaction->no_of_o -= 1;
		make_water();
	}

	else{
		pthread_cond_wait(&reaction->condH, &reaction->mutex);
	}
  
  	// release the lock
	pthread_mutex_unlock( & reaction->mutex );
}
