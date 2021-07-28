#include "city.h"
#include<cassert>

int City::_oneway_width = 0;

Road** City::get_adj_matrix(){
    return _adj_matrix;
}

Road* City::get_road_ptr(int i, int j)const
{
    return &(_adj_matrix[i][j]);
}

void City::_floyd_warshall()
{
    for (int i = 0; i < _n_rows*_n_coloumns; i++)
    {
        for (int j = 0; j < _n_rows*_n_coloumns; j++)
        {
            if (i == j)
            {
                _distance[i][j] = 0;
                _path[i][j] = Node(-2);
            } 
            else if (_adj_matrix[i][j].get_car_number() == -1)
            { 
                _distance[i][j] = _n_rows*_n_coloumns*Road::get_max_road_length() + 1; //infty
                _path[i][j] = Node(-1);
            } 
            else 
            {
                _distance[i][j] = _adj_matrix[i][j].get_road_length();
                _path[i][j] = Node(i);
            }
        }
    }
    for (int k = 0; k < _n_rows*_n_coloumns; k++)
    {
        for (int i = 0; i < _n_rows*_n_coloumns; i++)
        {
            for (int j = 0; j < _n_rows*_n_coloumns; j++)
            {
                if (_distance[i][j] > _distance[i][k] + _distance[k][j])
                {
                    _distance[i][j] = _distance[i][k] + _distance[k][j];
                    _path[i][j] = _path[k][j];
                }   
            }
        }
    }
}

std::list<Node> City::print_path(Node source, Node destination)
{
    assert(!(source.get_index() >= _n_rows*_n_coloumns || destination.get_index() >= _n_rows*_n_coloumns));
    assert(!(_path[source.get_index()][destination.get_index()].get_index() == -1));
    std::list<Node> optimal_path;
    optimal_path.push_back(destination);
    while(_path[source.get_index()][destination.get_index()].get_index() != source.get_index())
    {
        optimal_path.push_front(_path[source.get_index()][destination.get_index()]);       
        destination = _path[source.get_index()][destination.get_index()];
    }
    return optimal_path;
}

City::City() {
    _oneway_width = 0; 
}

City::City(int righe, int colonne, float oneway_fraction):_n_rows(righe), _n_coloumns(colonne)
{ 

    // Road::set_statistics(0,0,0,0);
    _adj_matrix = new Road*[_n_rows*_n_coloumns];
    _path = new Node*[_n_rows*_n_coloumns];
    _distance = new int*[_n_rows*_n_coloumns];
    _node_set = new Node[_n_rows*_n_coloumns];
    for( int i = 0; i < _n_rows*_n_coloumns; i++)
    {
        _adj_matrix[i] = new Road[_n_rows*_n_coloumns];
        _path[i] = new Node[_n_rows*_n_coloumns];
        _distance[i] = new int[_n_rows*_n_coloumns];
    }

    srand(time(NULL));
    float local_roads = 0;
    float local_oneway_roads = 0;
    
    for (int i = 0; i < _n_rows*_n_coloumns; i++ )
    {
        _adj_matrix[i][i] = Road(-1);
        for (int j = i+1; j < _n_rows*_n_coloumns; j++)
        {              
            float r = rand();
            float rl = rand();
            if ((j == i+1 && j%_n_coloumns != 0) || j == i+_n_coloumns )
            {
                
                if(rl/RAND_MAX <= 0.5) 
                //1. scegli a caso una strada, (i,j) oppure (j,i) - perché almeno una strada tra i due nodi deve esserci sempre
                {
                    _adj_matrix[i][j] = Road(0);
                    if (r/RAND_MAX > oneway_fraction) //2. se hai scelto (i,j), lancia una moneta e decidi se fare anche (j,i)
                    {
                        _adj_matrix[j][i] = Road(0);
                        
                        local_roads++;
                    } 
                    else //3. se non fai anche (j,i), allora aumenta le corsie di (i,j)
                    { 
                        _adj_matrix[j][i] = Road(-1);
                        _adj_matrix[i][j].set_width(City::_oneway_width);
                        local_oneway_roads++;
                    }
                }
                else{
                
                    _adj_matrix[j][i] = Road(0);
                    if (r/RAND_MAX > oneway_fraction) 
                    //4. se hai scelto (j,i) tira una moneta
                    {
                        _adj_matrix[i][j] = Road(0); //5. costruisci anche (i,j)
                        local_roads++;
                    } 
                    else 
                    {
                        _adj_matrix[i][j] = Road(-1); //6. non costruire (i,j) ma aumenta le corsie di (j,i)
                        _adj_matrix[j][i].set_width(City::_oneway_width);
                        local_oneway_roads++;
                    }
                }
            } 
            else 
            {
                _adj_matrix[i][j] = Road(-1);
                _adj_matrix[i][j].set_width(City::_oneway_width);
                _adj_matrix[j][i] = Road(-1);
                _adj_matrix[j][i].set_width(City::_oneway_width);
            }
        }
    }
    _oneway_fraction = local_oneway_roads/(local_oneway_roads+local_roads);
    for(int i=0;i<_n_rows*_n_coloumns;i++)
    {
        _node_set[i].set_index(i); 
    }
    _floyd_warshall();
}

/*
City::City(int righe, int colonne, float oneway_fraction):_n_rows(righe), _n_coloumns(colonne)
{ 

    // Road::set_statistics(0,0,0,0);
    _adj_matrix = new Road*[_n_rows*_n_coloumns];
    _path = new Node*[_n_rows*_n_coloumns];
    _distance = new int*[_n_rows*_n_coloumns];
    _node_set = new Node[_n_rows*_n_coloumns];
    for( int i = 0; i < _n_rows*_n_coloumns; i++)
    {
        _adj_matrix[i] = new Road[_n_rows*_n_coloumns];
        _path[i] = new Node[_n_rows*_n_coloumns];
        _distance[i] = new int[_n_rows*_n_coloumns];
    }

    srand(time(NULL));
    float local_roads = 0;
    float local_oneway_roads = 0;
    
    for (int i = 0; i < _n_rows*_n_coloumns; i++ )
    {
        _adj_matrix[i][i] = Road(-1);
        for (int j = i+1; j < _n_rows*_n_coloumns; j++)
        {              
            float r = rand();
            float rl = rand();
            if ((j == i+1 && j%_n_coloumns != 0) || j == i+_n_coloumns )
            {
                if(rl/RAND_MAX <= 0.5)
                {
                    _adj_matrix[i][j] = Road(0);
                    if (r/RAND_MAX > oneway_fraction)
                    {
                        _adj_matrix[j][i] = Road(0);
                        _adj_matrix[j][i].set_width(City::_oneway_width);
                        local_roads++;
                    } 
                    else 
                    {
                        _adj_matrix[j][i] = Road(-1);
                        local_oneway_roads++;
                    }
                }
                else
                {
                    _adj_matrix[j][i] = Road(0);
                    if (r/RAND_MAX > oneway_fraction)
                    {
                        _adj_matrix[i][j] = Road(0);
                        local_roads++;
                    } 
                    else 
                    {
                        _adj_matrix[i][j] = Road(-1);
                        _adj_matrix[i][j].set_width(City::_oneway_width);
                        local_oneway_roads++;
                    }
                }
            } 
            else 
            {
                _adj_matrix[i][j] = Road(-1);
                _adj_matrix[i][j].set_width(City::_oneway_width);
                _adj_matrix[j][i] = Road(-1);
                _adj_matrix[j][i].set_width(City::_oneway_width);
            }
        }
    }
    _oneway_fraction = local_oneway_roads/(local_oneway_roads+local_roads);
    for(int i=0;i<_n_rows*_n_coloumns;i++)
    {
        _node_set[i].set_index(i); 
    }
    _floyd_warshall();
}

*/
Road City::get_road(int i, int j)const
{
    return _adj_matrix[i][j];
}
int City::get_n_rows()const
{
    return _n_rows;
}
int City::get_n_coloumns()const
{
    return _n_coloumns;
}
float City::get_oneway_fraction()const
{
    return _oneway_fraction;
}
Node City::get_node(int i)const
{
    return _node_set[i];
}
Node City::get_path(int i, int j)const
{
    return _path[i][j];
}
int City::get_distance(int i, int j)const
{
    return _distance[i][j];
}
void City::set_oneway_width(int oneway_width)
{
    _oneway_width = oneway_width;
}

City::City(int righe, int colonne, float oneway_fraction, int gaussian_mean, int gaussian_sigma, int min_road_length, int max_road_length):_n_rows(righe), _n_coloumns(colonne)
{ 
    
    _oneway_width = 0; 
    Road::set_statistics(gaussian_mean, gaussian_sigma, min_road_length, max_road_length);

    _adj_matrix = new Road*[_n_rows*_n_coloumns];
    _path = new Node*[_n_rows*_n_coloumns];
    _distance = new int*[_n_rows*_n_coloumns];
    _node_set = new Node[_n_rows*_n_coloumns];

    
    for( int i = 0; i < _n_rows*_n_coloumns; i++)
    {
    
        _adj_matrix[i] = new Road[_n_rows*_n_coloumns];
        
        _path[i] = new Node[_n_rows*_n_coloumns];

    
        _distance[i] = new int[_n_rows*_n_coloumns];

        
    }

    
    srand(time(NULL));
    float local_roads = 0;
    float local_oneway_roads = 0;

    
    for (int i = 0; i < _n_rows*_n_coloumns; i++ )
    {

        _adj_matrix[i][i] = Road(-1);
        for (int j = i+1; j < _n_rows*_n_coloumns; j++)
        {              
            float r = rand();
            float rl = rand();
            if ((j == i+1 && j%_n_coloumns != 0) || j == i+_n_coloumns )
            {
                if(rl/RAND_MAX <= 0.5)
                {
                    _adj_matrix[i][j] = Road(0);
                    if (r/RAND_MAX > oneway_fraction)
                    {
                        _adj_matrix[j][i] = Road(0);
                        local_roads++;
                    } 
                    else 
                    {
                        _adj_matrix[j][i] = Road(-1);
                        local_oneway_roads++;
                    }
                }
                else
                {
                    _adj_matrix[j][i] = Road(0);
                    if (r/RAND_MAX > oneway_fraction)
                    {
                        _adj_matrix[i][j] = Road(0);
                        local_roads++;
                    } 
                    else 
                    {
                        _adj_matrix[i][j] = Road(-1);
                        local_oneway_roads++;
                    }
                }
            } 
            else 
            {
                _adj_matrix[i][j] = Road(-1);
                _adj_matrix[j][i] = Road(-1);
            }
        }
    }
    _oneway_fraction = local_oneway_roads/(local_oneway_roads+local_roads);
    for(int i=0;i<_n_rows*_n_coloumns;i++)
    {
        _node_set[i].set_index(i); 
    }
    _floyd_warshall();
}
