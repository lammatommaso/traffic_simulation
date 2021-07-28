#include <nodejs/src/node_api.h>

#include <node.h>
#include <assert.h>
//#include <stsim.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <list>
#include <filesystem>
#include <nlohmann/json.hpp>
//#include <malloc.h>

#include "simulator.h"
#include "road.h"
#include "car.h"

#include "Batch_Simulation.h"

using namespace std;
using json = nlohmann::json;

    struct {
        bool operator()(Car_Info a, Car_Info b) const { 
            //malloc_stats();
            //cout << "=================================\n";
            if (a.car->get_offset() == NULL || b.car->get_offset() == NULL){
                return false;
            }
            return a.car->get_offset() > b.car->get_offset(); }
    } customGreater;

bool _order(Car_Info i, Car_Info j)
{
    //malloc_stats();
    if (i.car->get_offset() == NULL || j.car->get_offset() == NULL){
        return false;
    }
    return ((i.car->get_offset()) > (j.car->get_offset()));
}

Simulator sim = NULL;
int counter;

napi_value batchMain(napi_env env, napi_callback_info info){
     napi_status status;

    cout << "Batch iniziato\n";

    //prendi parametri (callback, variabili...)
    size_t argc = 16;
    napi_value args[16];
    status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    assert(status == napi_ok);

    int simulation_type, cols, rows, min_car, max_car, increment, 
        car_number, gaussian_mean, gaussian_sigma, min_road_length, 
        max_road_length, str_len, lanes;
    double p;

    status = napi_get_value_double(env, args[2],  &p);    
    status = napi_get_value_int32(env,  args[1],  &simulation_type);
    status = napi_get_value_int32(env,  args[3],  &cols);
    status = napi_get_value_int32(env,  args[4],  &rows);
    status = napi_get_value_int32(env,  args[5],  &min_car); 
    status = napi_get_value_int32(env,  args[6],  &max_car); 
    status = napi_get_value_int32(env,  args[7],  &increment); 
    status = napi_get_value_int32(env,  args[8],  &car_number);
    status = napi_get_value_int32(env,  args[9],  &gaussian_mean);
    status = napi_get_value_int32(env,  args[10], &gaussian_sigma);
    status = napi_get_value_int32(env,  args[11], &min_road_length);
    status = napi_get_value_int32(env,  args[12], &max_road_length);
    status = napi_get_value_int32(env,  args[14], &lanes);

    status = napi_get_value_int32(env, args[13], &str_len);
    char base_dir[str_len];
    status = napi_get_value_string_latin1(env, args[0], base_dir, str_len, NULL);

    string base_dir_s = "";
    std::cout << "base_dir: " << base_dir << "\n";
    
    base_dir_s.append(base_dir);

    //gestione callback
    napi_value cb = args[15];
    napi_value global;
    status = napi_get_global(env, &global);
    assert(status == napi_ok);

    std::cout << "Sto per creare la classe batch...\n";


    //Inizializziamo il "main"
    Batch_Simulation b = Batch_Simulation(base_dir_s, simulation_type, 
                                    (float)p, cols, rows, min_car, max_car, increment, 
                                    car_number, gaussian_mean, gaussian_sigma, min_road_length, 
                                    max_road_length, lanes);

    int perc_progress = 0;
    do {
        perc_progress = b.execute_step();
        //chiamiamo la callback
        napi_value argv[1];
        //json nodes = {{"nodes", j}}; //valore da restituire
        status = napi_create_string_latin1(env, std::to_string(perc_progress).c_str() , NAPI_AUTO_LENGTH, argv);
        napi_value result;
        napi_call_function(env, global, cb, 1, argv, &result);
        assert(status == napi_ok);

        cout << "la percentuale dei progressi è " << perc_progress << "\n" ;
    } while(

        b.perc_progress() < 100 //progressi in percentuale
    );

    cout << "Batch terminato\n";
    
    b.save_data(base_dir_s, "");

    //chiamiamo la callback
    napi_value argv[1];
    //json nodes = {{"nodes", j}}; //valore da restituire
    status = napi_create_string_latin1(env, "ok", NAPI_AUTO_LENGTH, argv);
    //TODO al momento non restituiamo alcun valore, ma solo un 'ok' per dire che la simulazione è terminata
    napi_value result;
    napi_call_function(env, global, cb, 1, argv, &result);
    assert(status == napi_ok);

    return nullptr;
}

napi_value myMain(napi_env env, napi_callback_info info){
    napi_status status;

    cout << "js_interface.cpp: myMain\n" ;

    //esegue una sola volta la car_increment_simulation; viene graficata durante l'esecuzione

    //prendi parametri (callback, variabili...)
    size_t argc = 10;
    napi_value args[10];
    status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    assert(status == napi_ok);

    int n_cars, rows, columns, gaussian_mean, gaussian_sigma, min_road_l, max_road_l, lanes ;
    double increment;

    status = napi_get_value_double(env, args[0], &increment);
    status = napi_get_value_int32(env, args[1], &n_cars);
    status = napi_get_value_int32(env, args[2], &rows);
    status = napi_get_value_int32(env, args[3], &columns);
    status = napi_get_value_int32(env, args[4], &gaussian_mean); 
    status = napi_get_value_int32(env, args[5], &gaussian_sigma); 
    status = napi_get_value_int32(env, args[6], &min_road_l); 
    status = napi_get_value_int32(env, args[7], &max_road_l);
    status = napi_get_value_int32(env, args[8], &lanes);

    sim = Simulator(n_cars);
    
    City::set_oneway_width(lanes);

    sim.create_city(rows, columns, increment, gaussian_mean, gaussian_sigma, min_road_l, max_road_l);

    //sim.get_city().set_oneway_width(width);
    
    //gestione callback
    napi_value cb = args[9];
    napi_value global;
    status = napi_get_global(env, &global);
    assert(status == napi_ok);

    //grafo creato, lo invio alla grafica
    
    napi_value argv[1];
    Road** r = sim.get_city().get_adj_matrix();

    json j = {};

    for (int x = 0; x < sim.get_city().get_n_rows()*sim.get_city().get_n_coloumns(); x++){
        for (int y=0; y < sim.get_city().get_n_rows()*sim.get_city().get_n_coloumns(); y++){
            if (sim.get_city().get_road(x,y).get_car_number() >= 0)
                j.push_back({{"x", x}, {"y", y}});
                
        }
    }

    json nodes = {{"nodes", j}};

    status = napi_create_string_latin1(env, nodes.dump().c_str(), NAPI_AUTO_LENGTH, argv);
    
    napi_value result;

    napi_call_function(env, global, cb, 1, argv, &result);
    assert(status == napi_ok);


    cout << "Callback chiamata\n";

    return nullptr;
}

napi_value create_path(napi_env env, napi_callback_info info){
    napi_status status;

    //prendi parametri (callback, variabili...)
    size_t argc = 4;
    napi_value args[4];
    status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    assert(status == napi_ok);

    int start_lenght, end_lenght;
    status = napi_get_value_int32(env, args[0], &start_lenght);
    status = napi_get_value_int32(env, args[1], &end_lenght);

    int source_nodes[start_lenght], dest_nodes[end_lenght];
    napi_value e;
    int v;

    for (int i=0; i < start_lenght; i++){
        status = napi_get_element(env, args[2], i, &e);
        status = napi_get_value_int32(env, e, &v);
        source_nodes[i] = v;

    }

    for (int i=0; i < end_lenght; i++){
        status = napi_get_element(env, args[3], i, &e);
        status = napi_get_value_int32(env, e, &v);
        dest_nodes[i] = v;

    }


    sim.create_path(start_lenght, end_lenght, source_nodes, dest_nodes);

    return nullptr;
}

json get_updates(){
    
    json tmp = {};

    for (int i = 0; i < sim.get_city().get_n_rows()*sim.get_city().get_n_coloumns(); i++){
        for (int j = 0; j < sim.get_city().get_n_rows()*sim.get_city().get_n_coloumns(); j++){
            if (/*sim.get_city().get_road(i, j).get_car_number() > 0 */ sim.get_city().get_road_ptr(i, j)->cars_in_road > 0 ){
                int max = sim.get_city().get_road(i, j).get_road_length();
                int cars = sim.get_city().get_road_ptr(i, j)->cars_in_road;
                //int cars = sim.get_city().get_road(i, j).get_car_number();

                tmp.push_back({{"street", {{"x", i}, {"y", j}}}, {"max", max}, {"cars", cars}});
            }
        }    
    }
    cout << "Macchine a destinazione: " << sim.get_cars_at_destination() << "\n";
    return {{"cars_at_dest", sim.get_cars_at_destination()}, {"streets", tmp}};
} 

napi_value start_simulation(napi_env env, napi_callback_info info){
    napi_status status;

    size_t argc = 1;
    napi_value args[1];
    status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    assert(status == napi_ok);

    //gestione callback
    napi_value cb = args[0];
    napi_value global;
    status = napi_get_global(env, &global);
    assert(status == napi_ok);
    napi_value argv[1];
    
    counter = 0;
    while (sim.get_cars_at_destination() < sim.get_car_number())
    {

        std::vector<Car_Info> car_vector = sim.get_car_vector();

        //std::sort(sim.get_car_vector().begin(), sim.get_car_vector().end(), _order);
        std::sort(car_vector.begin(), car_vector.end(), _order);

        // std::vector<Car_Info>::iterator end = sim.get_car_vector().begin();
        // std::advance(end, sim.get_car_vector().size() - 1);
        // std::sort(sim.get_car_vector().begin(), end, _order);

        for (int i = 0; i < sim.get_car_number(); i++)
        { 
            if (!(sim.get_car(i).car->get_at_destination()) && sim.get_car(i).car->get_delay() == 0)
            {
               sim.mv_car(i);
            }
            else
            {
                sim.get_car(i).car->delay();
                
            }
        }  

        napi_value result;

        json j = get_updates();
        status = napi_create_string_latin1(env, j.dump().c_str(), NAPI_AUTO_LENGTH, argv);
        
        //chiamo la callback...
        napi_call_function(env, global, cb, 1, argv, &result);
        assert(status == napi_ok);
        
        counter++;
        break;
    }

    float steps_mean = 0;
    float steps_squared_mean = 0;
    float stops_mean = 0;
    float stops_squared_mean = 0;
    for (int i = 0; i < sim.get_car_number(); i++)
    {
        steps_mean += sim.get_car(i).car->get_steps();
        steps_squared_mean += pow(sim.get_car(i).car->get_steps(),2);
        stops_mean += sim.get_car(i).car->get_stops();
        stops_squared_mean += pow(sim.get_car(i).car->get_stops(),2);

    }

   

    steps_mean /= sim.get_car_number();
    steps_squared_mean /= sim.get_car_number();
    stops_mean /= sim.get_car_number();
    stops_squared_mean /= sim.get_car_number();

    

    Result r = sim.get_result();

    
    r.steps_mean = steps_mean;
    r.steps_sigma = sqrt(steps_squared_mean - pow(steps_mean,2));
    r.stops_mean = stops_mean;
    r.steps_sigma = sqrt(stops_squared_mean - pow(stops_mean,2));


    sim.set_result(r);


    return nullptr;
}

napi_value next_simulation_step(napi_env env, napi_callback_info info){
    napi_status status;
    
    size_t argc = 1;
    napi_value args[1];
    status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    assert(status == napi_ok);
    
    //gestione callback
    napi_value cb = args[0];
    napi_value global;
    status = napi_get_global(env, &global);
    assert(status == napi_ok);
    napi_value argv[1];

    if (sim.get_cars_at_destination() < sim.get_car_number())
    {
        // std::sort(sim.get_car_vector().begin(), sim.get_car_vector().end(), _order);
        // sort(sim.get_car_vector().begin(), sim.get_car_vector().end(), _order);

        // std::vector<Car_Info>::iterator end = sim.get_car_vector().begin();
        // std::advance(end, sim.get_car_vector().size() - 1);
        // sort(sim.get_car_vector().begin(), end, customGreater);

        for (int i = 0; i < sim.get_car_number(); i++)
        { 
            if (!(sim.get_car(i).car->get_at_destination()) && sim.get_car(i).car->get_delay() == 0)
            {
                sim.mv_car(i);
            }
            else
            {
                sim.get_car(i).car->delay();
            }
        }  

        napi_value result;

        json j = get_updates();
        status = napi_create_string_latin1(env, j.dump().c_str(), NAPI_AUTO_LENGTH, argv);
        
        //chiamo la callback...
        napi_call_function(env, global, cb, 1, argv, &result);
        assert(status == napi_ok);
        
        counter++;
    }
    // else {
    
    //     napi_value result;

    //     json j = get_updates();
    //     status = napi_create_string_latin1(env, j.dump().c_str(), NAPI_AUTO_LENGTH, argv);
        
    //     //chiamo la callback...
    //     napi_call_function(env, global, cb, 1, argv, &result);
    //     assert(status == napi_ok);
    // }
    return nullptr;
}

napi_value init(napi_env env, napi_value exports) {
    cout << "Inizializzo addon\n"; 

    napi_status status;

    napi_value fn1;

    status = napi_create_function(env, nullptr, 0, start_simulation, nullptr, &fn1);
    if (status != napi_ok) return nullptr;

    status = napi_set_named_property(env, exports, "start_simulation", fn1);
    if (status != napi_ok) return nullptr;

    napi_value fn2;

    status = napi_create_function(env, nullptr, 0, create_path, nullptr, &fn2);
    if (status != napi_ok) return nullptr;

    status = napi_set_named_property(env, exports, "create_path", fn2);
    if (status != napi_ok) return nullptr;

    napi_value fn3;

    status = napi_create_function(env, nullptr, 0, myMain, nullptr, &fn3);
    if (status != napi_ok) return nullptr;

    status = napi_set_named_property(env, exports, "mymain", fn3);
    if (status != napi_ok) return nullptr;

    napi_value fn4;

    status = napi_create_function(env, nullptr, 0, batchMain, nullptr, &fn4);
    if (status != napi_ok) return nullptr;

    status = napi_set_named_property(env, exports, "batchmain", fn4);
    if (status != napi_ok) return nullptr;

    napi_value fn5;

    status = napi_create_function(env, nullptr, 0, next_simulation_step, nullptr, &fn5);
    if (status != napi_ok) return nullptr;

    status = napi_set_named_property(env, exports, "next_simulation_step", fn5);
    if (status != napi_ok) return nullptr;

    

    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init)