#include "city.h"
#include<cassert>

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

City::City() {}

City::City(short righe, short colonne, float oneway_fraction):_n_rows(righe), _n_coloumns(colonne)
{ 

    // Road::set_statistics(0,0,0,0);
    _adj_matrix = new Road*[_n_rows*_n_coloumns];
    _path = new Node*[_n_rows*_n_coloumns];
    _distance = new short*[_n_rows*_n_coloumns];
    _node_set = new Node[_n_rows*_n_coloumns];
    for( int i = 0; i < _n_rows*_n_coloumns; i++)
    {
        _adj_matrix[i] = new Road[_n_rows*_n_coloumns];
        _path[i] = new Node[_n_rows*_n_coloumns];
        _distance[i] = new short[_n_rows*_n_coloumns];
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
Road City::get_road(short i, short j)const
{
    return _adj_matrix[i][j];
}
short City::get_n_rows()const
{
    return _n_rows;
}
short City::get_n_coloumns()const
{
    return _n_coloumns;
}
float City::get_oneway_fraction()const
{
    return _oneway_fraction;
}
Node City::get_node(short i)const
{
    return _node_set[i];
}
Node City::get_path(short i, short j)const
{
    return _path[i][j];
}
short City::get_distance(short i, short j)const
{
    return _distance[i][j];
}