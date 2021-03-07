#ifndef NODE_H
#define NODE_H

class Node 
{
    short _index;
  public:
    Node();
    Node(short index);
    Node(Node const& Node);
    short  get_index() const;
    void set_index(short index);
};

#endif