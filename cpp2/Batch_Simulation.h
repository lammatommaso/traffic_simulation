#ifndef BATCH_SIMULATION
#define BATCH_SIMULATION

#include<iostream>
#include<thread>
#include<string>
#include<chrono>
#include"simulator.h"

#define ONEWAY_INCREMENT_SIMULATION 0
#define CAR_INCREMENT_SIMULATION 1

class Batch_Simulation {
    private:
        std::vector<float> x;
        std::vector<float> mean_steps;
        std::vector<float> sigma_steps;
        std::vector<float> mean_stops;
        std::vector<float> sigma_stops;
        std::chrono::_V2::steady_clock::time_point start; 
        std::chrono::_V2::steady_clock::time_point stop; 
        //Simulator simulator;

        int _percent_progress ;
        int _sim_type ; 
        double _inc, _increment ;
        int _car_number, _n_rows, _n_columns , _max_car_number, _min_car_number;

        void _car_increment_simulation(int city_number, std::string sim_dir, int min_car_number, int max_car_number, int increment, float oneway_fraction, int n_rows, int n_coloumns);
        void _oneway_increment_simulation(std::string sim_dir,float increment, int car_number, int n_rows, int n_coloumns);
        void simulation();
        
    public:
        void save_data(std::string sim_dir, std::string file_name); 
        Batch_Simulation(std::string base_dir, int simulation_type, 
                                        float p, int cols, int rows, int min_car, int max_car, int increment, 
                                        int car_number, int gaussian_mean, int gaussian_sigma, int min_road_length, 
                                        int max_road_length, int width);  
        int execute_step();
        int perc_progress();
};

#endif