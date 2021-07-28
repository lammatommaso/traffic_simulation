#ifndef CAR_H
#define CAR_H

#include<time.h>
#include<iostream>

#define MAX_DELAY 0

class Car 
{
    int _steps;
    int _stops;
    int _offset;
    bool _at_destination;
    int _delay;
  public:
    Car();
    Car(int steps);
    Car(Car const& Car);
    void move_forward();
    void halt();
    void reset_offset();
    void delay();
    void set_at_destination(bool at_destination);
    bool get_at_destination()const;
    int get_delay()const;
    int get_steps()const;
    int get_stops()const;
    int get_offset()const;
};

#endif
