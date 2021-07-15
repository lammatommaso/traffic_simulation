#include "simulator.h"
//using namespace Simulator;

bool order(Car_Info i, Car_Info j)
{
    return (i.car->get_offset() > j.car->get_offset());
}

Car_Info::Car_Info(): path(std::list<Node>()), position(Node()), car(new Car()){}

Result::Result(): steps_mean(0), steps_sigma(0), stops_mean(0), stops_sigma(0){}

std::ostream& operator<<(std::ostream& output, const Result& result)
{
    output<<result.steps_mean<<" "<<result.steps_sigma<<" "<<result.stops_mean<<" "<<result.stops_sigma<<std::endl;
    return output;
}

std::istream& operator>>(std::istream& input, Result& result)
{
    input>>result.steps_mean>>result.steps_sigma>>result.stops_mean>>result.stops_sigma;
    return input;
}

Simulator::Simulator(): _car_number(0){}

    
Simulator::Simulator(short car_number)
{
    _car_number = car_number;
    for (int i = 0; i < car_number; i++)
    {
        _car_vector.push_back(Car_Info());
    }
}

void Simulator::create_city(short n_rows, short n_coloumns, float oneway_fraction)
{
    _city = City(n_rows, n_coloumns, oneway_fraction);
    /*for(int i=0;i<_city.get_n_rows()*_city.get_n_coloumns();i++)
    {
        for(int j=0;j<_city.get_n_rows()*_city.get_n_coloumns();j++)
        {
            if(_city.get_distance(i,j) < _city.get_n_rows()*_city.get_n_coloumns()*MAX_ROAD_LENGTH)
            {
                std::cout<<_city.get_distance(i,j)<<' ';
            }
            else
            {
                std::cout<<"+inf"<<' ';
            }
        }
        std::cout<<std::endl;
    }*/
}

Road Simulator::_find_road(int car_index)const
{     
    return _city.get_road(_car_vector[car_index].position.get_index(),_find_next(car_index).get_index());
}

Road Simulator::_find_next_road(int car_index)const
{     
    return _city.get_road(_find_next(car_index).get_index(),_find_second_next(car_index).get_index());
}


Node Simulator::_find_next(int car_index)const
{
    bool search_end = false;
    Node next_node;
    for (Node n : _car_vector[car_index].path)
    {
        next_node = n;    
        if(search_end)
        {
            break;
        }
        if(n.get_index() == _car_vector[car_index].position.get_index())
        {
            search_end = true;
        } 
    }
    return next_node;
}

Node Simulator::_find_second_next(int car_index)const
{
    bool search_end = false;
    Node next_node;
    int counter = 0;
    for (Node n : _car_vector[car_index].path)
    {
        next_node = n;  
        if(search_end)
        {
            counter++;
        }  
        if(counter == 2)
        {
            break;
        }
        if(n.get_index() == _car_vector[car_index].position.get_index())
        {
            search_end=true;
        } 
    }
    return next_node;
}

void Simulator::_mv_car(int car_index)
{
    Node next_node = _find_next(car_index);
    Node second_next_node = _find_second_next(car_index);
    if(_car_vector[car_index].car->get_offset()<_find_road(car_index).get_road_length())
    {                
        int car_in_front = 0;
        short width = _find_road(car_index).get_width();
        for(int i=0;i<_car_number;i++)
        {
            if(_car_vector[car_index].position.get_index() == _car_vector[i].position.get_index() &&  _find_next(car_index).get_index() == _find_next(i).get_index())
            {
                if(_car_vector[car_index].car->get_offset() == _car_vector[i].car->get_offset() - 1)
                {
                    car_in_front++;
                }
                if(car_in_front > width) break;
            }
        }
        if(car_in_front <= width)
        {
            _car_vector[car_index].car->move_forward();
        }
        else
        {
            _car_vector[car_index].car->halt();
        }
    }
    else
    {            
        int car_in_front = 0;
        for(int i=0;i<_car_number;i++)
        {
            if( _find_next(car_index).get_index() == _car_vector[i].position.get_index() &&  _find_second_next(car_index).get_index() == _find_next(i).get_index())
            {
                if(_car_vector[i].car->get_offset() == 0)
                {
                    car_in_front++;
                }
                if(car_in_front > _find_road(i).get_width()) break;
            }
        }
        if(car_in_front <= _find_next_road(car_index).get_width())
        {
            _car_vector[car_index].position = next_node;
            _car_vector[car_index].car->move_forward();
            _car_vector[car_index].car->reset_offset();
        }
        else
        {
            _car_vector[car_index].car->halt();
        }
    }
    if (_car_vector[car_index].position.get_index() ==  _car_vector[car_index].path.back().get_index())
    {
        _cars_at_destination++;
        _car_vector[car_index].car->set_at_destination(true);
    }
}


class car_by_source {
    public:
        int source;
        int next_step;
        std::list<int> cars;

        car_by_source(){
            cars = std::list<int>();
        }
};

class set_source {
    private:
        std::list<car_by_source> myList; //anziché una lista si può implementare come matrice
    public:
        set_source(){
            myList = std::list<car_by_source>();
        }

        std::list<int> get_cars_at_source_next(int source, int next){
            for (car_by_source car: myList){
                if (car.source == source && car.next_step == next){
                    return car.cars;
                }
            }
            return std::list<int>();
        }

        void add_car(int source, int next, int car_index){
            for (car_by_source car: myList){
                //aggiungiamo la macchina ad un sottoinsieme esistente
                if (car.source == source && car.next_step == next){
                    car.cars.push_back(car_index);
                    return;
                }
            }


            //aggiungiamo la macchina ad un nuovo sottoinsieme
            car_by_source newList = car_by_source();
            newList.source = source;
            newList.next_step = next;
            newList.cars.push_back(car_index);
            myList.push_back(newList);
        }

};


//set_source setsource = set_source();

void Simulator::create_path()
{
    srand(time(NULL)); 
    for (int i = 0; i < _car_number; i++)
    {
        Node source, destination;
        do
        {
            source = _city.get_node(rand()%(_city.get_n_rows()*_city.get_n_coloumns()));
            destination = _city.get_node(rand()%(_city.get_n_rows()*_city.get_n_coloumns()));
        }
        while(source.get_index() == destination.get_index() || _city.get_path(source.get_index(),destination.get_index()).get_index() == -1);
        //std::cout << source.get_index() << ' ' << destination.get_index() << std::endl;
        _car_vector[i].position = source;
        
        // setsource.add_car(source.get_index(), _find_next(i).get_index(), i);
        
        _car_vector[i].path = _city.print_path(source, destination); //nel path manca il nodo sorgente!
        _car_vector[i].path.push_front(source);

        /*for (auto it=_car_vector[i].path.begin(); it!=_car_vector[i].path.end(); ++it)
             std::cout << ' ' << (*it).get_index();
        std::cout << std::endl;*/
    }
}


// /* 
// 1000 macchine dentro 1
//  1         2
// ###       ###
// ###-------###
// ###       ###
//  |
//  |
//  |
// ###
// ###
// ###
//  3

// */


// void Simulator::init_simulation(){
//     for (int car_index = 0; car_index<_car_vector.size(); car_index++ ){
//         auto car = _car_vector[car_index]; //.position.get_index()
//         int source = car.position.get_index();
//         int next = _find_next(car_index).get_index();
//         std::list<int> _car_list = setsource.get_cars_at_source_next(source, next);
        
//         int delay = 0;
//         for (int c : _car_list){
            
//             _car_vector[c].car->set_delay(delay++);
            
//         }
//     }
// }

void Simulator::simulation()
{
    std::cout<<"1"<<"\n";
    int counter = 0;
    // init_simulation();
    while (_cars_at_destination < _car_number)
    {
        std::sort(_car_vector.begin(), _car_vector.end(), order);
        for (int i = 0; i < _car_number; i++)
        { 
            if (!(_car_vector[i].car->get_at_destination()) && _car_vector[i].car->get_delay() == 0)
            {
                _mv_car(i);
            }
            else
            {
                _car_vector[i].car->delay();
            }
        }  
        std::cout<<_cars_at_destination<<"\n";
        counter++;
    }
    std::cout<<"2"<<"\n";
    float steps_mean = 0;
    float steps_squared_mean = 0;
    float stops_mean = 0;
    float stops_squared_mean = 0;
    for (int i = 0; i < _car_number; i++)
    {
        steps_mean += _car_vector[i].car->get_steps();
        steps_squared_mean += pow(_car_vector[i].car->get_steps(),2);
        stops_mean += _car_vector[i].car->get_stops();
        stops_squared_mean += pow(_car_vector[i].car->get_stops(),2);

    }
    steps_mean /= _car_number;
    steps_squared_mean /= _car_number;
    stops_mean /= _car_number;
    stops_squared_mean /= _car_number;
    _result.steps_mean = steps_mean;
    _result.steps_sigma = sqrt(steps_squared_mean - pow(steps_mean,2));
    _result.stops_mean = stops_mean;
    _result.stops_sigma = sqrt(stops_squared_mean - pow(stops_mean,2));
}

Result Simulator::get_result()const
{
    return _result;
}

short Simulator::get_car_number()const
{
    return _car_number;
}

void Simulator::set_car_number(short car_number)
{
    _car_number = car_number;
    _car_vector.clear();
    for (int i = 0; i < car_number; i++)
    {
        _car_vector.push_back(Car_Info());
    }
}

std::vector<Car_Info> Simulator::get_car_vector()const
{
    return _car_vector;
}

City Simulator::get_city()const
{
    return _city;
}

std::string Simulator::print_adj_matrix()const
{
    std::vector<short> adj_matrix;
    for(int i=0; i<_city.get_n_rows()*_city.get_n_coloumns(); i++)
    {
        for(int j=0; j<_city.get_n_rows()*_city.get_n_coloumns(); j++)
        {
            if(_city.get_road(i,j).get_car_number() == -1)
            {
                adj_matrix.push_back(0);
            }
            else
            {
                adj_matrix.push_back(1);
            }
        }
    }
    return Numpy_Parser::print_matrix("adj_matrix", adj_matrix, _city.get_n_rows()*_city.get_n_coloumns());
}

std::string Simulator::print_car_data()const
{
    std::vector<short> car_steps;
    std::vector<short> car_stops;
    for(int i=0; i<_car_number; i++)
    {
        car_steps.push_back(_car_vector[i].car->get_steps());
        car_stops.push_back(_car_vector[i].car->get_stops());
    }
    return Numpy_Parser::print_array("car_steps", car_steps) +  Numpy_Parser::print_array("car_stops", car_stops);
}

std::string Simulator::print()const
{
    std::string local = Numpy_Parser::numpy();
    local += Numpy_Parser::print_var("car_number", _car_number);
    local += Numpy_Parser::print_var("n_rows", _city.get_n_rows());
    local += Numpy_Parser::print_var("n_coloumns", _city.get_n_coloumns());
    local += Numpy_Parser::print_var("one_way_fraction", _city.get_oneway_fraction());
    local += Numpy_Parser::print_var("gaussian_mean", Road::get_gaussian_mean());
    local += Numpy_Parser::print_var("gaussian_sigma", Road::get_gaussian_sigma());
    local += Numpy_Parser::print_var("min_road_length", Road::get_min_road_length());
    local += Numpy_Parser::print_var("max_road_length", Road::get_max_road_length());
    local += print_car_data();
    local += print_adj_matrix();
    return local;
}


    

