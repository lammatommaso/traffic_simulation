#include"node.h"

Node::Node() : _index(-1) {}
Node::Node(int index) : _index(index) {}
Node::Node(Node const& Node) : _index(Node._index) {} 
int  Node::get_index() const { return _index; }
void Node::set_index(int index){_index = index;}
