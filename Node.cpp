#include "Node.h"

Node::Node() : m_F(0), m_G(0), m_H(0), m_x(0), m_y(0), m_id(0), m_closed(false), m_open(false), ptr_parent(NULL){
	
}

Node::Node(int p_x, int p_y, int p_id) : m_F(0), m_G(0), m_H(0), m_x(p_x), m_y(p_y), m_id(p_id), m_closed(false), m_open(false), ptr_parent(NULL){
	
}

Node::~Node(){
	
}