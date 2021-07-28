#ifndef NODE_H
#define NODE_H

class Node 
{
    int _index;
  public:
    Node();
    Node(int index);
    Node(Node const& Node);
    int  get_index() const;
    void set_index(int index);
};

#endif