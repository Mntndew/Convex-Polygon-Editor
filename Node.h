#pragma once
#include <math.h>
#include <iostream>

struct Node{
	public:
	Node();
	Node(int p_x, int p_y, int p_id);
	~Node();
	int m_x, m_y, m_id, m_G, m_H, m_F;
	Node* ptr_parent;
	bool m_closed, m_open;
};