
#ifndef CITY_H
#define CITY_H

#include "road.h"
#include "node.h"
#include <list>
#include <cmath>
#include<iterator>

class City 
{
    Node** _path;
    short** _distance;
    Node* _node_set;
    void _floyd_warshall();
    Road** _adj_matrix;
    short _n_rows;
    short _n_coloumns;
    float _oneway_fraction;
  public:
    City();
    City(short n_rows, short n_coloumns, float oneway_fraction);
    std::list<Node> print_path(Node source, Node destination);
    Road get_road(short i, short j)const;
    short get_n_rows()const;
    short get_n_coloumns()const;
    float get_oneway_fraction()const;
    Node get_node(short i)const;
    Node get_path(short i, short j)const;
    short get_distance(short i, short j)const;
};

#endif
