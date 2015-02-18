#pragma once

#include <cassert>
#include <algorithm>
#include <memory>
#include <ctime>
#include <unordered_map>
#include <utility>

#include "../map/Map.hpp"


class AStar
{
private:
	struct Node
	{
		std::weak_ptr<Node> parent;
		float f, g, h;

		sf::Vector2i position;

		Node(const sf::Vector2i& position, std::weak_ptr<Node> parent) :
			position(position),
			parent(parent)
		{}

		bool operator==(const Node& node) const
		{
			return node.position == position;
		}
	};

	struct Key
	{
		int x, y;

		bool operator==(const Key& key) const
		{
			return x == key.x && y == key.y;
		}
	};

	struct Hasher
	{
		std::size_t operator()(const Key& key) const
		{
			int hash = 17;
			hash = ((hash + key.x) << 5) - (hash + key.x);
			hash = ((hash + key.y) << 5) - (hash + key.y);
			return hash;
		}
	};

	Map& m_map;
	std::vector<std::shared_ptr<Node>> m_open;
	std::unordered_map<Key, std::shared_ptr<Node>, Hasher> m_closed;

	static bool compare(const std::shared_ptr<Node> a, const std::shared_ptr<Node> b)
	{
		return a->f > b->f;
	}
	
	sf::Vector2i m_source, m_destination;

	void recalculate(std::shared_ptr<Node> node, const std::shared_ptr<Node> parent)
	{
		node->g = parent->g + ((node->position.x == parent->position.x || node->position.y == parent->position.y) ? 1:std::sqrt(2));
		node->f = node->g + node->h;
	}

	void calculateHeuristic(std::shared_ptr<Node> node)
	{
		node->h = .05f*(std::pow(m_destination.x - node->position.x, 1) + std::pow(m_destination.y - node->position.y, 1));
	}

	bool illegalDiagonal(const sf::Vector2i& a, const sf::Vector2i& b) const
	{
		int x = a.x > b.x ? a.x:b.x;
		int y = a.y > b.y ? a.y:b.y;
		
		if ((x == a.x && y == a.y) || (x == b.x && y == b.y)) // Max coordinates belong to the same node.
			return (m_map.tiles[x - 1][y].type == 1 || m_map.tiles[x][y - 1].type == 1);
		else
			return (m_map.tiles[x - 1][y - 1].type == 1 || m_map.tiles[x][y].type == 1);
	}

	bool inOpen(const sf::Vector2i& position) const
	{
		for (int i = 0; i < m_open.size(); ++i)
		{
			if (m_open[i]->position == position)
				return true;
		}

		return false;
	}

	std::shared_ptr<Node> getOpenNode(const sf::Vector2i& position)
	{
		for (int i = 0; i < m_open.size(); ++i)
		{
			if (m_open[i]->position == position)
				return m_open[i];
		}
	}

	bool checkAdjacentNodes(const std::shared_ptr<Node> node)
	{
		for (int i = 0; i < 9; ++i)
		{
			if (!(i%3 == 1 && i/3 == 1))
			{
				sf::Vector2i position(node->position.x - 1 + i%3, node->position.y - 1 + i/3);

				if (m_map.tiles[position.x][position.y].type != 1 && !illegalDiagonal(node->position, position))
				{
					std::shared_ptr<Node> adjacent(new Node(position, node));

					if (m_closed.find({position.x, position.y}) != m_closed.end())
						continue;

					if (position == m_destination)
					{
						m_closed.insert(std::make_pair<Key, std::shared_ptr<Node>&>(Key({adjacent->position.x, adjacent->position.y}), adjacent));
						return true;
					}

					if (!inOpen(position))
					{
						calculateHeuristic(adjacent);
						recalculate(adjacent, node);
						m_open.push_back(adjacent);
					}
					else
					{
						adjacent = getOpenNode(position);
						int g = node->g + ((position.x == node->position.x || position.y == node->position.y) ? 10:14);

						if (g < adjacent->g)
						{
							adjacent->parent = node;
							adjacent->g = g;
							adjacent->f = adjacent->g + adjacent->h;
						}
					}
				}
			}
		}

		return false;
	}

	void constructPath()
	{
		auto node = m_closed.find({m_destination.x, m_destination.y})->second;
		int index = 0;

		while (auto parent = node->parent.lock())
		{
			if (node->position != m_destination)
				m_map.tiles[node->position.x][node->position.y].type = 6;

			node = parent;
		}
	}

	void search()
	{
		m_open.push_back(std::shared_ptr<Node>(new Node(m_source, std::weak_ptr<Node>())));

		calculateHeuristic(m_open[0]);
		m_open[0]->f = m_open[0]->h;

		while (!m_open.empty())
		{
			std::shared_ptr<Node> selectedNode = m_open.back();
			m_open.pop_back();
			m_closed.insert(std::make_pair<Key, std::shared_ptr<Node>&>(Key({selectedNode->position.x, selectedNode->position.y}), selectedNode));

			if (checkAdjacentNodes(selectedNode))
			{
				constructPath();
				return;
			}

			std::sort(m_open.begin(), m_open.end(), compare);
		}
	}

public:
	AStar(Map& map, const sf::Vector2i& source, const sf::Vector2i& destination) :
		m_map(map),
		m_source(source),
		m_destination(destination)
	{
		search();
	}
};

