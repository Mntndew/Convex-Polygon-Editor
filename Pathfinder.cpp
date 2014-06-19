#include "Pathfinder.h"

Pathfinder::Pathfinder()
{

}

Pathfinder::Pathfinder(std::vector<std::vector<Tile>>* ptr_tiles)
	: m_current_node(-1), ptr_tiles(ptr_tiles)
{
	GetMap(ptr_tiles);
}

Pathfinder::~Pathfinder()
{

}

void Pathfinder::GetMap(std::vector<std::vector<Tile>>* ptr_tiles)
{
	this->ptr_tiles = ptr_tiles;
	m_nodes.clear();
	Node node;
	for (int y = 0; y < ptr_tiles->back().size(); y++)
	{
		for (int x = 0; x < ptr_tiles->size(); x++)
		{
			node.m_x = x;
			node.m_y = y;
			node.m_id = (*ptr_tiles)[x][y].m_tileType;
			m_nodes.push_back(node);
		}
	}
}

void Pathfinder::CalculateHGF(Node* node, Node* destination)
{
	node->m_F = 0;
	node->m_H = 0;
	if (node->ptr_parent != nullptr)
	{
		if (node->m_y - node->ptr_parent->m_y == 0 || node->m_x - node->ptr_parent->m_x == 0)
		{
			node->m_G = 10 + node->ptr_parent->m_G;
		}else
		{
			node->m_G = 14 + node->ptr_parent->m_G;
		}
	}else
	{
		if (node->m_y - destination->m_y == 0 || node->m_x - destination->m_x == 0)
		{
			node->m_G = 10;
		}else
		{
			node->m_G = 14;
		}
	}
	node->m_H = (int)std::sqrt(std::pow((destination->m_x - node->m_x), 2) + std::pow((destination->m_y - node->m_y), 2));
	node->m_F = node->m_G + node->m_H;
}

int Pathfinder::GetLowestFOpen()
{
	if (!m_open.empty())
	{
		unsigned int low = m_open[0];
		for (unsigned int i = 0; i < m_open.size(); i++)
		{
			if (m_nodes[m_open[i]].m_F < m_nodes[low].m_F)
			{
				low = m_open[i];
			}
		}
		return low;
	}else
	{
		return 0;
	}
}

int Pathfinder::GetIndexOf(Node* node)
{
	return node->m_x + (node->m_y * ptr_tiles->size());
}

int Pathfinder::GetIndexOf(sf::Vector2i* node)
{
	return node->x + (node->y * ptr_tiles->size());
}

int Pathfinder::GetIndexOf(const sf::Vector2i& node)
{
	return node.x + (node.y * ptr_tiles->size());
}

int Pathfinder::GetIndexOf(std::vector<int> & list, Node* node)
{
	for (unsigned int i = 0; i < list.size(); i++)
	{
		if (GetIndexOf(node) == list[i])
		{
			return i;
		}
	}
	return -1;
}

void Pathfinder::Clear()
{
	m_nodes.clear();
	m_open.clear();
	m_closed.clear();
}

std::vector<sf::Vector2i> Pathfinder::GetPath(const sf::Vector2i & p_start, const sf::Vector2i & p_destination)
{

	if (p_start.x >= 0 && p_start.x < ptr_tiles->size() && p_start.y >= 0 && p_start.y < ptr_tiles->back().size() &&
		p_destination.x >= 0 && p_destination.x < ptr_tiles->size() && p_destination.y >= 0 && p_destination.y < ptr_tiles->back().size())
	{
		if (m_nodes[p_destination.x + p_destination.y * ptr_tiles->size()].m_id == 0 && m_nodes[p_start.x + p_start.y * ptr_tiles->size()].m_id == 0)
		{
			m_path.clear();
			for (unsigned int i = 0; i < m_open.size(); i++)
			{
				m_nodes[m_open[i]].m_open = false;
				m_nodes[m_open[i]].ptr_parent = nullptr;
			}
			for (unsigned int i = 0; i < m_closed.size(); i++)
			{
				m_nodes[m_closed[i]].m_closed = false;
				m_nodes[m_closed[i]].ptr_parent = nullptr;
			}
			m_open.clear();
			m_closed.clear();
			m_current_node = p_start.x + (p_start.y*ptr_tiles->size());	
			m_nodes[m_current_node].m_open = true;
			m_nodes[m_current_node].m_closed = false;
			m_open.push_back(m_current_node);

			while (!m_nodes[GetIndexOf(p_destination)].m_closed && !m_nodes[GetIndexOf(p_destination)].m_open)
			{
				m_current_node = GetLowestFOpen();
				m_nodes[m_current_node].m_open = false;
				m_nodes[m_current_node].m_closed = true;
				m_closed.push_back(m_current_node);
				if (!m_open.empty())
				{
					m_open.erase(m_open.begin() + GetIndexOf(m_open, &m_nodes[m_current_node]));
				}

				if (m_current_node <= 0 || m_current_node > ptr_tiles->size()* ptr_tiles->back().size())
					break;

				if (m_nodes[m_current_node - 1].m_id == 0 && !m_nodes[m_current_node - 1].m_closed)
				{
					if (m_nodes[m_current_node - 1].m_open)
					{
						if (m_nodes[m_current_node - 1].m_G - m_nodes[m_current_node].m_G > m_nodes[m_current_node].m_G)
						{
							m_nodes[m_current_node - 1].ptr_parent = &m_nodes[m_current_node];
						}
					}else
					{
						m_nodes[m_current_node - 1].m_open = true;
						m_open.push_back(m_current_node - 1);
						m_nodes[m_current_node - 1].ptr_parent = &m_nodes[m_current_node];
					}
					CalculateHGF(&m_nodes[m_current_node - 1], &m_nodes[m_current_node]);
				}
				if (m_nodes[m_current_node + 1].m_id == 0 && !m_nodes[m_current_node + 1].m_closed)
				{
					if (m_nodes[m_current_node + 1].m_open)
					{
						if (m_nodes[m_current_node + 1].m_G - m_nodes[m_current_node].m_G > m_nodes[m_current_node].m_G)
						{
							m_nodes[m_current_node + 1].ptr_parent = &m_nodes[m_current_node];
						}
					}else
					{
						m_nodes[m_current_node + 1].m_open = true;
						m_open.push_back(m_current_node + 1);
						m_nodes[m_current_node + 1].ptr_parent = &m_nodes[m_current_node];
					}
					CalculateHGF(&m_nodes[m_current_node + 1], &m_nodes[m_current_node]);
				}
				if (m_nodes[m_current_node - ptr_tiles->size()].m_id == 0 && !m_nodes[m_current_node - ptr_tiles->size()].m_closed)
				{
					if (m_nodes[m_current_node - ptr_tiles->size()].m_open)
					{
						if (m_nodes[m_current_node - ptr_tiles->size()].m_G - m_nodes[m_current_node].m_G > m_nodes[m_current_node].m_G)
						{
							m_nodes[m_current_node - ptr_tiles->size()].ptr_parent = &m_nodes[m_current_node];
						}
					}else
					{
						m_nodes[m_current_node - ptr_tiles->size()].m_open = true;
						m_open.push_back(m_current_node - ptr_tiles->size());
						m_nodes[m_current_node - ptr_tiles->size()].ptr_parent = &m_nodes[m_current_node];
					}
					CalculateHGF(&m_nodes[m_current_node - ptr_tiles->size()], &m_nodes[m_current_node]);
				}
				if (m_nodes[m_current_node + ptr_tiles->size()].m_id == 0 && !m_nodes[m_current_node + ptr_tiles->size()].m_closed)
				{
					if (m_nodes[m_current_node + ptr_tiles->size()].m_open)
					{
						if (m_nodes[m_current_node + ptr_tiles->size()].m_G - m_nodes[m_current_node].m_G > m_nodes[m_current_node].m_G)
						{
							m_nodes[m_current_node + ptr_tiles->size()].ptr_parent = &m_nodes[m_current_node];
						}
					}else
					{
						m_nodes[m_current_node + ptr_tiles->size()].m_open = true;
						m_open.push_back(m_current_node + ptr_tiles->size());
						m_nodes[m_current_node + ptr_tiles->size()].ptr_parent = &m_nodes[m_current_node];
					}
					CalculateHGF(&m_nodes[m_current_node + ptr_tiles->size()], &m_nodes[m_current_node]);
				}
				if (m_nodes[m_current_node - 1 - ptr_tiles->size()].m_id == 0 && !m_nodes[m_current_node - 1 - ptr_tiles->size()].m_closed && m_nodes[m_current_node - ptr_tiles->size()].m_id == 0 && m_nodes[m_current_node - 1].m_id == 0)
				{
					if (m_nodes[m_current_node - 1 - ptr_tiles->size()].m_open)
					{
						if (m_nodes[m_current_node - 1 - ptr_tiles->size()].m_G - m_nodes[m_current_node].m_G > m_nodes[m_current_node].m_G)
						{
							m_nodes[m_current_node - 1 - ptr_tiles->size()].ptr_parent = &m_nodes[m_current_node];
						}
					}else
					{
						m_nodes[m_current_node - 1 - ptr_tiles->size()].m_open = true;
						m_open.push_back(m_current_node - 1 - ptr_tiles->size());
						m_nodes[m_current_node - 1 - ptr_tiles->size()].ptr_parent = &m_nodes[m_current_node];
					}
					CalculateHGF(&m_nodes[m_current_node - 1 - ptr_tiles->size()], &m_nodes[m_current_node]);
				}
				if (m_nodes[m_current_node + 1 - ptr_tiles->size()].m_id == 0 && !m_nodes[m_current_node + 1 - ptr_tiles->size()].m_closed && m_nodes[m_current_node - ptr_tiles->size()].m_id == 0 && m_nodes[m_current_node + 1].m_id == 0)
				{
					if (m_nodes[m_current_node + 1 - ptr_tiles->size()].m_open)
					{
						if (m_nodes[m_current_node + 1 - ptr_tiles->size()].m_G - m_nodes[m_current_node].m_G > m_nodes[m_current_node].m_G)
						{
							m_nodes[m_current_node + 1 - ptr_tiles->size()].ptr_parent = &m_nodes[m_current_node];
						}
					}else
					{
						m_nodes[m_current_node + 1 - ptr_tiles->size()].m_open = true;
						m_open.push_back(m_current_node + 1 - ptr_tiles->size());
						m_nodes[m_current_node + 1 - ptr_tiles->size()].ptr_parent = &m_nodes[m_current_node];
					}
					CalculateHGF(&m_nodes[m_current_node + 1 - ptr_tiles->size()], &m_nodes[m_current_node]);
				}
				if (m_nodes[m_current_node - 1 + ptr_tiles->size()].m_id == 0 && !m_nodes[m_current_node - 1 + ptr_tiles->size()].m_closed && m_nodes[m_current_node + ptr_tiles->size()].m_id == 0 && m_nodes[m_current_node - 1].m_id == 0)
				{
					if (m_nodes[m_current_node - 1 + ptr_tiles->size()].m_open)
					{
						if (m_nodes[m_current_node - 1 + ptr_tiles->size()].m_G - m_nodes[m_current_node].m_G > m_nodes[m_current_node].m_G)
						{
							m_nodes[m_current_node - 1 + ptr_tiles->size()].ptr_parent = &m_nodes[m_current_node];
						}
					}else
					{
						m_nodes[m_current_node - 1 + ptr_tiles->size()].m_open = true;
						m_open.push_back(m_current_node - 1 + ptr_tiles->size());
						m_nodes[m_current_node - 1 + ptr_tiles->size()].ptr_parent = &m_nodes[m_current_node];
					}
					CalculateHGF(&m_nodes[m_current_node - 1 + ptr_tiles->size()], &m_nodes[m_current_node]);
				}
				if (m_nodes[m_current_node + 1 + ptr_tiles->size()].m_id == 0 && !m_nodes[m_current_node + 1 + ptr_tiles->size()].m_closed&& m_nodes[m_current_node + ptr_tiles->size()].m_id == 0 && m_nodes[m_current_node + 1].m_id == 0)
				{
					if (m_nodes[m_current_node + 1 + ptr_tiles->size()].m_open)
					{
						if (m_nodes[m_current_node + 1 + ptr_tiles->size()].m_G - m_nodes[m_current_node].m_G > m_nodes[m_current_node].m_G)
						{
							m_nodes[m_current_node + 1 + ptr_tiles->size()].ptr_parent = &m_nodes[m_current_node];
						}
					}else
					{
						m_nodes[m_current_node + 1 + ptr_tiles->size()].m_open = true;
						m_open.push_back(m_current_node + 1 + ptr_tiles->size());
						m_nodes[m_current_node + 1 + ptr_tiles->size()].ptr_parent = &m_nodes[m_current_node];
					}
					CalculateHGF(&m_nodes[m_current_node + 1 + ptr_tiles->size()], &m_nodes[m_current_node]);
				}
			}
			SetPath(p_destination);
		}
	}
	return m_path;
}

void Pathfinder::SetPath(const sf::Vector2i & p_destionation)
{
	Node current = m_nodes[GetIndexOf(p_destionation)];
	m_path.push_back(sf::Vector2i(current.m_x, current.m_y));
	while (current.ptr_parent != nullptr)
	{
		m_path.push_back(sf::Vector2i(current.ptr_parent->m_x, current.ptr_parent->m_y));
		current = *current.ptr_parent;
	}
}