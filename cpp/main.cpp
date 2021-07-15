
#include<iostream>
#include<thread>
#include<string>
#include<chrono>
#include"simulator.h"

void oneway_increment_simulation(std::string sim_dir,float increment, short car_number, short n_rows, short n_coloumns)
{
   std::vector<float> x;
   std::vector<float> mean_steps;
   std::vector<float> sigma_steps;
   std::vector<float> mean_stops;
   std::vector<float> sigma_stops;
   auto start = std::chrono::steady_clock::now();

   for(float i=0; i<1; i+=increment)
   {
      Simulator simulator;
      simulator.set_car_number(car_number);
      simulator.create_city(n_rows,n_coloumns,i);
      simulator.create_path();
      simulator.simulation();

      std::cout<<"oneway fraction : "<<i<<" COMPLETED"<<"\n";

      x.push_back(simulator.get_city().get_oneway_fraction());
      mean_steps.push_back(simulator.get_result().steps_mean);
      sigma_steps.push_back(simulator.get_result().steps_sigma);
      mean_stops.push_back(simulator.get_result().stops_mean);
      sigma_stops.push_back(simulator.get_result().stops_sigma);
   }
   auto stop = std::chrono::steady_clock::now();
   std::chrono::duration<float> execution_time = stop-start;   
   std::ofstream simdata;
   simdata.open(sim_dir+"sim"+std::to_string(car_number)+".py");
   simdata<<Numpy_Parser::numpy();
   simdata<<Numpy_Parser::print_array("x", x)
           <<Numpy_Parser::print_array("mean_steps", mean_steps)
           <<Numpy_Parser::print_array("sigma_steps", sigma_steps)
           <<Numpy_Parser::print_array("mean_stops", mean_stops)
           <<Numpy_Parser::print_array("sigma_stops", sigma_stops)
           <<Numpy_Parser::print_var("execution_time", execution_time.count());
   simdata.close();
}

void car_increment_simulation(int city_number, std::string sim_dir, short min_car_number, short max_car_number, short increment, float oneway_fraction, short n_rows, short n_coloumns)
{
   
   std::vector<float> x;
   std::vector<float> mean_steps;
   std::vector<float> sigma_steps;
   std::vector<float> mean_stops;
   std::vector<float> sigma_stops;
   auto start = std::chrono::steady_clock::now();
   Simulator simulator;
   simulator.set_car_number(min_car_number);
   simulator.create_city(n_rows, n_coloumns, oneway_fraction);
   
   std::ofstream citydata;
   citydata.open(sim_dir+"city"+std::to_string(city_number)+".py");
   citydata<<simulator.print();
   citydata.close();
   for(short i = min_car_number; i <= max_car_number;i+=increment)
   {
      simulator.set_car_number(i);
      simulator.create_path();
      simulator.simulation();
   
      x.push_back(simulator.get_car_number());
      mean_steps.push_back(simulator.get_result().steps_mean);
      sigma_steps.push_back(simulator.get_result().steps_sigma);
      mean_stops.push_back(simulator.get_result().stops_mean);
      sigma_stops.push_back(simulator.get_result().stops_sigma);
   }
   
   auto stop = std::chrono::steady_clock::now();
   std::chrono::duration<float> execution_time = stop-start;   
   std::ofstream simdata;
   simdata.open(sim_dir+"simulation"+std::to_string(city_number)+".py");
   simdata<<Numpy_Parser::numpy();
   simdata<<Numpy_Parser::print_array("x", x)
           <<Numpy_Parser::print_array("mean_steps", mean_steps)
           <<Numpy_Parser::print_array("sigma_steps", sigma_steps)
           <<Numpy_Parser::print_array("mean_stops", mean_stops)
           <<Numpy_Parser::print_array("sigma_stops", sigma_stops)
           <<Numpy_Parser::print_var("execution_time", execution_time.count());
   simdata.close();
}


int main()
{
   Road::set_statistics(20,10,10,30);
   City::set_oneway_width(3);
   // for(int i=1; i <=5; i++)
   // {
   //    std::string sim_dir = "/home/tommaso/simulations/oneway_increment/width3/";
   //    oneway_increment_simulation(sim_dir,0.1,100,5,5);
   // }
   for(int i=1; i <=1; i++){
   car_increment_simulation(i,"/home/tommaso/simulations/car_increment/", 1, 2000, 1, 0.4, 7,7);
   std::cout<<i<<"\n";}
   
}
