#ifndef ROAD_H
#define ROAD_H

#include<random>

class Road 
{
    short _car_number; 
    short _road_length;
    static short _gaussian_mean;
    static short _gaussian_sigma;
    static short _min_road_length;
    static short _max_road_length;
  public:
    Road();
    Road(short car_number);
    Road(Road const& Road);
    short get_car_number()const;
    short get_road_length();
    static void set_statistics(short gaussian_mean, short gaussian_sigma, short min_road_length, short max_road_length);
    static short get_gaussian_mean();
    static short get_gaussian_sigma();
    static short get_min_road_length();
    static short get_max_road_length();
    void add_car();
    void rm_car();
};

#endif