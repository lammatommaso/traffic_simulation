#ifndef ROAD_H
#define ROAD_H

#include<random>

class Road 
{
    int _car_number; 
    int _road_length;
    static int _gaussian_mean;
    static int _gaussian_sigma;
    static int _min_road_length;
    static int _max_road_length;
    int _width;
  public:
    Road();
    Road(int car_number);
    Road(Road const& Road);
    int get_car_number()const;
    int get_road_length();
    static void set_statistics(int gaussian_mean, int gaussian_sigma, int min_road_length, int max_road_length);
    static int get_gaussian_mean();
    static int get_gaussian_sigma();
    static int get_min_road_length();
    static int get_max_road_length();
    int get_width()const;
    void set_width(int width);
    void add_car();
    void rm_car();

    int cars_in_road = 0;
};

#endif