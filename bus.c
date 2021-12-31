#include "pintos_headers.h"


struct station {
    
    //variables
    int passengers;      //passengers who are waiting for bus at the station
    int seats;           //number of seats that are empty

    //structures
    struct lock lock;
    struct condition cond1;
    struct condition cond2;

};


void station_setup(struct station *station) {
    
    printf("Setting up the station...\n");

     //Initial values
    station->passengers = 0;
    station->seats = 0;
    
    //initializing stuctures
    lock_init(&(station->lock));
    cond_init(&(station->cond1));
    cond_init(&(station->cond2));
}

void bus_load_passengers(struct station *station, int count) {

    //locking
    lock_acquire(&station->lock);

    station->seats = count;
    

   //\\ printf("There is a bus with %d seats in a station with %d passengers\n" ,count,station->passengers);
    
    //while there are passengers at the station and bus is not full yet
    //the bus hasn't left the station yet
    while(station->passengers > 0 && station->seats > 0){ 
        
        //signalling that a bus is in station and ready for passengers
        cond_signal(&station->cond1,&station->lock);

        //waiting for passengers to get on the bus
        cond_wait(&station->cond2,&station->lock);
    }

    lock_release(&station->lock);

    //\\printf("Bus is leaving\n");
}

void passenger_waitfor_bus(struct station *station) {
    
    //locking...
    lock_acquire(&station->lock);
    
    //passengers are coming to the station
    station->passengers++;

    //waitning for a bus to arrive at the station
    cond_wait(&station->cond1,&station->lock);

    lock_release(&station->lock);

}

void passenger_on_board(struct station *station) {
    
    //locking
    lock_acquire(&station->lock);

    //passengers are getting on the bus
    station->passengers--;
    station->seats--;
    
    //signalling that passenger is now in the bus
    cond_signal(&station->cond2,&station->lock);

    //\\printf("%d passengers are in the bus now\n",station->seats);

    lock_release(&station->lock);

}
