#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <random>
#include <queue>
#include <cstring>
#include <string>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

using namespace std;

enum Point {L,D};


struct Plane {
    int id;

	Point status;
    time_t currentTime;
    time_t startTime;
	time_t simulationTime;

    
};

double p = 0.5;  //probability
int t = 0;

std::queue<Plane> queue_l;  //landing queue 
std::queue<Plane> queue_d;  //departing queue (take off)

bool isLanding(double p);//Tells whether a plane is landing to a lane given a probability.

Plane createPlane(int id,Point status){
    Plane plane;
    plane.id=planeIDCounter;
    plane.status=status;
    planeIDCounter++;
    return plane;
}

bool isLanding(double p) {

	return ((double)(rand() % 100) > p * 100.) ? false : true;
}




int pthread_sleep (int seconds)
{
    pthread_mutex_t mutex;
    pthread_cond_t conditionvar;
    struct timespec timetoexpire;
    if(pthread_mutex_init(&mutex,NULL))
    {
        return -1;
    }
    if(pthread_cond_init(&conditionvar,NULL))
    {
        return -1;
    }
    struct timeval tp;
    //When to expire is an absolute time, so get the current time and add //it to our delay time
    gettimeofday(&tp, NULL);
    timetoexpire.tv_sec = tp.tv_sec + seconds; timetoexpire.tv_nsec = tp.tv_usec * 1000;
    
    pthread_mutex_lock (&mutex);
    int res =  pthread_cond_timedwait(&conditionvar, &mutex, &timetoexpire);
    pthread_mutex_unlock (&mutex);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&conditionvar);
    
    //Upon successful completion, a value of zero shall be returned
    return res;
    
}

void* airTrafficControl{

	while (currentTime < startTime + simulationTime) {
		if (queue_l.empty()) {                //if no plane in landing queue jump to departing queue for takeoff
			queue_d.push(createPlane(0, D));
			pthread_sleep(2t);
		}
		else {                              // number of planes in landing queue land the first plane in landing queue
			queue_l.pop();
			pthread_sleep(2t);

		}
	}

}

void landingFunc{

	queue_l.push(createPlane(0, L));  // enqueue to landing queue
if (queue_l.empty()) {                 //if this is the first queue in landing 
	pthread_mutex_unlock(&l_lock);
}
}

void departingFunc{

	queue_d.push(createPlane(0, D));  // enqueue to departing queue
if (queue_d.empty()) {                 //if this is the first plane in departing queue
	pthread_mutex_unlock(&d_lock);
}

}


int main (int argc, char **argv){

	while (currentTime < startTime + simulationTime) {
		if (isLanding(probability)) {    //create one plane thread running departing function
			pthread_mutex_lock(&d_lock);
			queue_d.push(createPlane(0, D));
			pthread_mutex_unlock(&d_lock);

		}
		else {                        // circumstance in random<p create one plane thread running landing function

			pthread_mutex_lock(&l_lock);
			queue_l.push(createPlane(0, D));
			pthread_mutex_unlock(&l_lock);

		}
		pthread_sleep(t);

	}
    
    return 0;
}
