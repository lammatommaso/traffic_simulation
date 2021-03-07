#include"road.h"

short Road::_gaussian_mean = 0;
short Road::_gaussian_sigma = 0;
short Road::_min_road_length = 0;
short Road::_max_road_length = 0;
Road::Road() : _car_number(0)
{
    std::random_device generator;
    std::normal_distribution<double> gaussian(_gaussian_mean,_gaussian_sigma);
    do
    {
        _road_length = (short)gaussian(generator);
    }
    while(_road_length <= _min_road_length || _road_length > _max_road_length);
}  
Road::Road(short car_number) : _car_number(car_number)
{
    std::random_device generator;
    std::normal_distribution<double> gaussian(_gaussian_mean,_gaussian_sigma);
    do
    {
        _road_length = (short)gaussian(generator);
    }
    while(_road_length <= _min_road_length || _road_length > _max_road_length);
}
Road::Road(Road const& Road) : _car_number(Road._car_number), _road_length(Road._road_length){}
short Road::get_car_number() const { return _car_number; }
short Road::get_road_length(){return _road_length;}
void Road::set_statistics(short gaussian_mean, short gaussian_sigma, short min_road_length, short max_road_length)
{
    _gaussian_mean = gaussian_mean;
    _gaussian_sigma = gaussian_sigma;
    _min_road_length = min_road_length;
    _max_road_length = max_road_length;
}
short Road::get_gaussian_mean()
{
    return _gaussian_mean;
}
short Road::get_gaussian_sigma()
{
    return _gaussian_sigma;
}
short Road::get_min_road_length()
{
    return _min_road_length;
}
short Road::get_max_road_length()
{
    return _max_road_length;
}
void Road::add_car() { _car_number++; }
void Road::rm_car() { _car_number--; }
