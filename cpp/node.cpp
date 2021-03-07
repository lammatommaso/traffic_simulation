#include"node.h"

Node::Node() : _index(-1) {}
Node::Node(short index) : _index(index) {}
Node::Node(Node const& Node) : _index(Node._index) {} 
short  Node::get_index() const { return _index; }
void Node::set_index(short index){_index = index;}
