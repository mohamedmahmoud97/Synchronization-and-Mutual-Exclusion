#include <pthread.h>
#include "reaction.h"

// Forward declaration. This function is implemented in reaction-runner.c,
// but you needn't care what it does. Just be sure it's called when
// appropriate within reaction_o()/reaction_h().
void make_water();

void reaction_init(struct reaction *reaction)
{
	// FILL ME IN
	reaction->no_of_h = 0;
	reaction->no_of_o = 0;
	pthread_mutex_init(&(reaction->mutex), NULL);
	pthread_cond_init(&(reaction->condH), NULL);
	pthread_cond_init(&(reaction->condO), NULL);
}

void reaction_h(struct reaction *reaction)
{
	//one H atom will increment this value at a time and 
	pthread_mutex_lock( &reaction->mutex );

	reaction->no_of_h++;

	if(reaction->no_of_h < 2){
		pthread_cond_wait(&reaction->condO,&reaction->mutex);
	}
	else if(reaction->no_of_h >=2 && (reaction->no_of_o)<1){
		pthread_cond_wait(&reaction->condO,&reaction->mutex);
	}
	else{
		pthread_cond_signal(&reaction->condO);
		pthread_cond_signal(&reaction->condH);
		reaction->no_of_h -= 2;
		reaction->no_of_o -= 1;
		make_water();
	}

  	// wake up the main thread (if it is sleeping) to test the value of done  
	pthread_mutex_unlock( & reaction->mutex );

}

void reaction_o(struct reaction *reaction)
{
	//one O atom will increment this value at a time and 

	pthread_mutex_lock( &reaction->mutex );

	reaction->no_of_o++;

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

  	// wake up the main thread (if it is sleeping) to test the value of done  
	pthread_mutex_unlock( & reaction->mutex );
}
