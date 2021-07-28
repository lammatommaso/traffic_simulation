#include"road.h"

int Road::_gaussian_mean = 0;
int Road::_gaussian_sigma = 0;
int Road::_min_road_length = 0;
int Road::_max_road_length = 0;
Road::Road() : _car_number(0),_width(1)
{
    std::random_device generator;
    std::normal_distribution<double> gaussian(_gaussian_mean,_gaussian_sigma);
    do
    {
        _road_length = (int)gaussian(generator);
    }
    while(_road_length <= _min_road_length || _road_length > _max_road_length);
}  
Road::Road(int car_number) : _car_number(car_number),_width(1)
{
    std::random_device generator;
    std::normal_distribution<double> gaussian(_gaussian_mean,_gaussian_sigma);
    do
    {
        _road_length = (int)gaussian(generator);
    }
    while(_road_length <= _min_road_length || _road_length > _max_road_length);
}
Road::Road(Road const& Road) : _car_number(Road._car_number), _road_length(Road._road_length),_width(Road._width){}
int Road::get_car_number() const { return _car_number; }
int Road::get_road_length(){return _road_length;}
void Road::set_statistics(int gaussian_mean, int gaussian_sigma, int min_road_length, int max_road_length)
{
    _gaussian_mean = gaussian_mean;
    _gaussian_sigma = gaussian_sigma;
    _min_road_length = min_road_length;
    _max_road_length = max_road_length;
}
int Road::get_gaussian_mean()
{
    return _gaussian_mean;
}
int Road::get_gaussian_sigma()
{
    return _gaussian_sigma;
}
int Road::get_min_road_length()
{
    return _min_road_length;
}
int Road::get_max_road_length()
{
    return _max_road_length;
}
int Road::get_width()const
{
    return _width;
}
void Road::set_width(int width)
{
    _width = width;
}
void Road::add_car() { _car_number++; }
void Road::rm_car() { _car_number--; }
