#include "car.h"

Car::Car() : _steps(0), _stops(0), _offset(0), _at_destination(false)
{
    if(MAX_DELAY != 0)
    {
        srand(time(NULL));
        _delay = rand() % MAX_DELAY;
    }
    else
    {
        _delay = 0;
    }
}
Car::Car(int steps) : _steps(steps),_offset(0), _at_destination(false),_delay(0){}
Car::Car(Car const& Car) : _steps(Car._steps), _offset(Car._offset), _at_destination(Car._at_destination),_delay(Car._delay) {}
void Car::move_forward() { _steps++; _offset++;}
void Car::halt() { _stops++; }
void Car::reset_offset(){ _offset = 0; }
void Car::delay(){_delay--;}
void Car::set_at_destination(bool at_destination){ _at_destination = at_destination;}
bool Car::get_at_destination()const{return _at_destination;}
int Car::get_delay()const{return _delay;};
int Car::get_steps()const{return _steps; }
int Car::get_stops()const{return _stops; }
int Car::get_offset()const{return _offset;}


