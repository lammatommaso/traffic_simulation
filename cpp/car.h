#ifndef CAR_H
#define CAR_H

#include<time.h>
#include<iostream>

#define MAX_DELAY 0

class Car 
{
    short _steps;
    short _stops;
    short _offset;
    bool _at_destination;
    short _delay;
  public:
    Car();
    Car(short steps);
    Car(Car const& Car);
    void move_forward();
    void halt();
    void reset_offset();
    void delay();
    void set_at_destination(bool at_destination);
    bool get_at_destination()const;
    short get_delay()const;
    short get_steps()const;
    short get_stops()const;
    short get_offset()const;
};

#endif
