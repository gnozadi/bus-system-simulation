#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "bus.c"

volatile int threads_completed = 0;

void *
passenger_thread(void *arg)
{
    struct station *station = (struct station *)arg;
    passenger_waitfor_bus(station);
    __sync_add_and_fetch(&threads_completed, 1);
    return NULL;
}

struct load_bus_args
{
    struct station *station;
    int free_seats;
};

volatile int load_bus_returned = 0;

void *
load_bus_thread(void *args)
{
    struct load_bus_args *ltargs = (struct load_bus_args *)args;
    bus_load_passengers(ltargs->station, ltargs->free_seats);
    load_bus_returned = 1;
    return NULL;
}

const char *alarm_error_str;
int alarm_timeout;

void _alarm(int seconds, const char *error_str)
{
    alarm_timeout = seconds;
    alarm_error_str = error_str;
    alarm(seconds);
}

void alarm_handler(int foo)
{
    fprintf(stderr, "Error: Failed to complete after %d seconds. Something's "
                    "wrong, or your system is terribly slow. Possible error hint: [%s]\n",
            alarm_timeout, alarm_error_str);
    exit(1);
}

#ifndef MIN
#define MIN(_x, _y) ((_x) < (_y)) ? (_x) : (_y)
#endif

int main()
{
    struct station station;
    station_setup(&station);

    srandom(getpid() ^ time(NULL));

    // signal(SIGALRM, alarm_handler);

    _alarm(1, "bus_load_passengers() did not return immediately when no waiting passengers");
    bus_load_passengers(&station, 0);
    bus_load_passengers(&station, 10);
    _alarm(0, NULL);

    int i;
    const int total_passengers = 1000;
    int passengers_left = total_passengers;
    for (i = 0; i < total_passengers; i++)
    {
        pthread_t tid;
        int ret = pthread_create(&tid, NULL, passenger_thread, &station);
        if (ret != 0)
        {
            perror("pthread_create");
            exit(1);
        }
    }

    _alarm(2, "bus_load_passengers() did not return immediately when no free seats");
    bus_load_passengers(&station, 0);
    _alarm(0, NULL);

    int total_passengers_boarded = 0;
    const int max_free_seats_per_bus = 50;
    int pass = 0;
    while (passengers_left > 0)
    {
        _alarm(2, "Some more complicated issue appears to have caused passengers "
                  "not to board when given the opportunity");

        int free_seats = random() % max_free_seats_per_bus;

        printf("bus entering station with %d free seats\n", free_seats);
        load_bus_returned = 0;
        struct load_bus_args args = {&station, free_seats};
        pthread_t lt_tid;
        int ret = pthread_create(&lt_tid, NULL, load_bus_thread, &args);
        if (ret != 0)
        {
            perror("pthread_create");
            exit(1);
        }

        int threads_to_reap = MIN(passengers_left, free_seats);
        int threads_reaped = 0;
        while (threads_reaped < threads_to_reap)
        {
            if (load_bus_returned)
            {
                fprintf(stderr, "Error: bus_load_passengers returned early!\n");
                exit(1);
            }
            if (threads_completed > 0)
            {
                if ((pass % 2) == 0)
                    usleep(random() % 2);
                threads_reaped++;
                passenger_on_board(&station);
                __sync_sub_and_fetch(&threads_completed, 1);
            }
        }

        for (i = 0; i < 1000; i++)
        {
            if (i > 50 && load_bus_returned)
                break;
            usleep(100);
        }

        if (!load_bus_returned)
        {
            fprintf(stderr, "Error: bus_load_passengers failed to return\n");
            exit(1);
        }

        while (threads_completed > 0)
        {
            threads_reaped++;
            __sync_sub_and_fetch(&threads_completed, 1);
        }

        passengers_left -= threads_reaped;
        total_passengers_boarded += threads_reaped;
        printf("bus departed station with %d new passenger(s) (expected %d)%s\n",
               threads_reaped, threads_to_reap,
               (threads_to_reap != threads_reaped) ? " *****" : "");

        if (threads_to_reap != threads_reaped)
        {
            fprintf(stderr, "Error: Too many passengers on this bus!\n");
            exit(1);
        }

        pass++;
    }

    if (total_passengers_boarded == total_passengers)
    {
        printf("Looks good!\n");
        return 0;
    }
    else
    {
        fprintf(stderr, "Error: expected %d total boarded passengers, but got %d!\n",
                total_passengers, total_passengers_boarded);
        return 1;
    }
}
