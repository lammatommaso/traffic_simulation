#include "Batch_Simulation.h"


Simulator simulator;

int Batch_Simulation::execute_step(){
   
   int actual = 0, max = 0;

   if (simulator.get_cars_at_destination() >= simulator.get_car_number()){
      if (_sim_type == ONEWAY_INCREMENT_SIMULATION)
         _inc += _increment; 

      x.push_back(simulator.get_city().get_oneway_fraction());
      mean_steps.push_back(simulator.get_result().steps_mean);
      sigma_steps.push_back(simulator.get_result().steps_sigma);
      mean_stops.push_back(simulator.get_result().stops_mean);
      sigma_stops.push_back(simulator.get_result().stops_sigma);

      simulator = Simulator();
      /*if (_sim_type == CAR_INCREMENT_SIMULATION)
         simulator.set_car_number(_inc);
      */
   }

   switch (_sim_type){     
      case ONEWAY_INCREMENT_SIMULATION:
      {
         if (simulator.get_cars_at_destination() >= simulator.get_car_number()){
            std::cout << "Simulazione ONEWAY_INCREMENT_SIMULATION completata\n";
            simulator.set_car_number(_car_number);
            simulator.create_city(_n_rows,_n_columns,_inc);
            simulator.create_path();
         }

         if (_inc < 1){
            simulation();
         } 

         max = (int)((1/_increment)*simulator.get_car_number());
         //FIXME questo calcola solo l'incremento, non tiene conto delle auto a destinazione
         //std::cout << "cars at destination è " << simulator.get_cars_at_destination() << "; car_number è " << simulator.get_car_number() << "\n";
         actual = (int)((_inc/_increment)*(simulator.get_car_number())) + simulator.get_cars_at_destination();
         _percent_progress = (int)(((double)actual/(double)max)*100) ;
         
         //std::cout << "la percentuale dei progressi è " << _percent_progress << "\n";

      }
         break;
      case CAR_INCREMENT_SIMULATION:
      {   
         //in questo caso consideriamo _inc come _max_car_number della simulazione attuale

         if (simulator.get_cars_at_destination() == simulator.get_car_number()){  
            _inc += _increment; 
            simulator.set_car_number(_inc);
            simulator.create_city(_n_rows,_n_columns,_inc);
            simulator.create_path();
         }

         std::cout << "_inc è " << _inc << "; _max_car_number è " << _max_car_number << "\n";
         if (_inc <= _max_car_number){
            std::cout << "chiamo 'simulation()'\n";
            simulation();
            std::cout << "funzione 'simulation()' chiamata\n";

         } 
         
         max = 0 ;
         int counter = 0; 

         for (int i = _min_car_number ; i <= _max_car_number ; i+=_increment){
            max += i ;
         }

         actual = 0; 
         counter = 0;
         for (int i = _min_car_number ; i < _inc ; i+=_increment){
            actual += i ;
         }
         
         std::cout << "cars at destination è " << simulator.get_cars_at_destination() << "\n";
         actual += simulator.get_cars_at_destination();
         _percent_progress = (int)(((double)actual/(double)max)*100) ;

         
      }   
         break;
   }

   return _percent_progress ; 
}

bool __order(Car_Info i, Car_Info j)
{
    if (i.car->get_offset() == NULL || j.car->get_offset() == NULL){
        return false;
    }
    return ((i.car->get_offset()) > (j.car->get_offset()));
}

void Batch_Simulation::simulation()
{
 
   int counter = 0;
   if (simulator.get_cars_at_destination() < simulator.get_car_number())
   {
      std::vector<Car_Info> car_vector = simulator.get_car_vector();
      
      std::sort(car_vector.begin(), car_vector.end(), __order); //FIXME funziona solo se metto una cout dopo
      //std::cout << "";
      
      for (int i = 0; i < simulator.get_car_number(); i++)
      { 
         if (!(car_vector[i].car->get_at_destination()) && car_vector[i].car->get_delay() == 0)
         {
               //std::cout << "mv_car()\n";
               simulator.mv_car(i);
         }
         else
         {
               //std::cout << "halt_car()\n";
               car_vector[i].car->delay();
         }
      }  
      counter++;

   }
   if (simulator.get_cars_at_destination() >= simulator.get_car_number()) {
      float steps_mean = 0;
      float steps_squared_mean = 0;
      float stops_mean = 0;
      float stops_squared_mean = 0;
      for (int i = 0; i < simulator.get_car_number(); i++)
      {
         steps_mean += simulator.get_car_vector()[i].car->get_steps();
         steps_squared_mean += pow(simulator.get_car_vector()[i].car->get_steps(),2);
         stops_mean += simulator.get_car_vector()[i].car->get_stops();
         stops_squared_mean += pow(simulator.get_car_vector()[i].car->get_stops(),2);

      }
      steps_mean /= simulator.get_car_number();
      steps_squared_mean /= simulator.get_car_number();
      stops_mean /= simulator.get_car_number();
      stops_squared_mean /= simulator.get_car_number();
      Result result = simulator.get_result();

      result.steps_mean = steps_mean;
      result.steps_sigma = sqrt(steps_squared_mean - pow(steps_mean,2));
      result.stops_mean = stops_mean;
      result.stops_sigma = sqrt(stops_squared_mean - pow(stops_mean,2));

      simulator.set_result(result);
   }
}

int Batch_Simulation::perc_progress(){
   return _percent_progress ;
}

void Batch_Simulation::save_data(std::string sim_dir, std::string file_name){
   stop = std::chrono::steady_clock::now();
   std::chrono::duration<float> execution_time = stop-start;   
   std::ofstream simdata;

   switch(_sim_type){
      case ONEWAY_INCREMENT_SIMULATION:
         simdata.open(sim_dir+"sim"+std::to_string(_car_number)+".py");
         simdata<<Numpy_Parser::numpy();
         /*simdata<<Numpy_Parser::print_array("x", x)
           <<Numpy_Parser::print_array("mean_steps", mean_steps)
           <<Numpy_Parser::print_array("sigma_steps", sigma_steps)
           <<Numpy_Parser::print_array("mean_stops", mean_stops)
           <<Numpy_Parser::print_array("sigma_stops", sigma_stops)
           <<Numpy_Parser::print_var("execution_time", execution_time.count());
         simdata.close();*/
         break;
      case CAR_INCREMENT_SIMULATION:
         simdata.open(sim_dir+"simulation"+file_name+".py"); //file_name è city_number
         break;
      default:
         break;
   }

   simdata<<Numpy_Parser::numpy();
   simdata<<Numpy_Parser::print_array("x", x)
           <<Numpy_Parser::print_array("mean_steps", mean_steps)
           <<Numpy_Parser::print_array("sigma_steps", sigma_steps)
           <<Numpy_Parser::print_array("mean_stops", mean_stops)
           <<Numpy_Parser::print_array("sigma_stops", sigma_stops)
           <<Numpy_Parser::print_var("execution_time", execution_time.count());
   simdata.close();
}

Batch_Simulation::Batch_Simulation(std::string base_dir, int simulation_type, 
                                    float p, int cols, int rows, int min_car, int max_car, int increment, 
                                    int car_number, int gaussian_mean, int gaussian_sigma, int min_road_length, 
                                    int max_road_length, int width)
{
   std::cout << "Sono nel costruttore\n" ;
   simulator = Simulator();

   _sim_type = simulation_type ;

   Road::set_statistics(gaussian_mean,gaussian_sigma,min_road_length,max_road_length);
   // int car_number = 200;
   // std::string sim_dir = "../oneway_increment_simulation/";
   // oneway_increment_simulation(sim_dir,0.5, car_number, 10, 10);

   City::set_oneway_width(width);
   start = std::chrono::steady_clock::now();


   _n_columns = cols; _n_rows = rows; 
   _car_number = car_number; _min_car_number = min_car; _max_car_number = max_car; 

   switch (simulation_type)
   {
   case CAR_INCREMENT_SIMULATION:

      //_car_increment_simulation(0, base_dir, min_car,max_car,increment,p,rows,cols);
      
      _inc = min_car;
      _increment = increment;
      _car_number = min_car;

      
      simulator.set_car_number(min_car);
      //*(int*)((long)&simulator + 0x48) = min_car;
      //simulator._car_number = min_car;
      std::cout << "Entro in create_city\n";
      simulator.create_city(rows, cols, p);
      std::cout << "Sono uscito da create_city\n";
      /*while (simulator.get_car_number() == 0){
         std::cout << "Set non ha funzionato\n";
         simulator.set_car_number(min_car);

      }*/
      
      
      break;
   case ONEWAY_INCREMENT_SIMULATION:
      
      //_oneway_increment_simulation(base_dir, p, car_number, cols, rows);
      
      _inc = 0;
      _increment = 0.1; 

      simulator.set_car_number(car_number);
      simulator.create_city(rows,cols,0);
   
   default:
      break;
   }

   simulator.create_path();
   std::cout << "chiamo per la prima volta simulation\n";
   simulation();
   std::cout << "simulation chiamato\n";
   
}
