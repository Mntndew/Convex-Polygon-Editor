#include "AStar.hpp"


AStar::AStar(Map& map, const sf::Vector2i& source, const sf::Vector2i& destination) :
	m_map(map),
	m_source(source),
	m_destination(destination),
	m_reserved(0)
{
	search();
}

void AStar::search()
{
	std::shared_ptr<AStar::Node> sourceNode(new AStar::Node(m_source, std::weak_ptr<AStar::Node>()));
	m_nodes.push_back(sourceNode);
	m_open.insert(std::make_pair<Key, std::weak_ptr<AStar::Node>>(Key({m_source.x, m_source.y}), std::weak_ptr<AStar::Node>(sourceNode)));

	sourceNode->h = heuristic(sourceNode->position);
	sourceNode->f = sourceNode->h;

	while (!m_open.empty())
	{
		std::shared_ptr<AStar::Node> selectedNode = m_nodes.back();
		
		auto open = m_open.find({selectedNode->position.x, selectedNode->position.y});
		if (open != m_open.end())
			m_open.erase(open);

		m_closed.insert(std::make_pair<Key, std::weak_ptr<AStar::Node>>(Key({selectedNode->position.x, selectedNode->position.y}), std::weak_ptr<AStar::Node>(selectedNode)));
		selectedNode->open = false;
		selectedNode->closed = true;

		m_nodes.reserve(8 - m_reserved);
		m_reserved = 8;

		if (checkAdjacentNodes(selectedNode))
		{
			constructPath();
			return;
		}

		std::sort(m_nodes.begin(), m_nodes.end(), compare);
	}
}

void AStar::constructPath()
{
	auto node = m_closed.find({m_destination.x, m_destination.y})->second;
	
	while (auto parent = node.lock()->parent.lock())
	{
		m_map.tiles[parent->position.x][parent->position.y].type = 6;
		node = parent;
	}
}

bool AStar::checkAdjacentNodes(const std::shared_ptr<AStar::Node> node)
{
	for (int i = 0; i < 9; ++i)
	{
		if (!(i%3 == 1 && i/3 == 1))
		{
			sf::Vector2i position(node->position.x - 1 + i%3, node->position.y - 1 + i/3);

			if (m_map.tiles[position.x][position.y].type != 1 && !illegalDiagonal(node->position, position))
			{
				if (m_closed.find({position.x, position.y}) != m_closed.end())
					continue;

				std::shared_ptr<AStar::Node> adjacent;

				if (position == m_destination)
				{
					adjacent.reset(new Node(position, node));
					m_nodes.push_back(adjacent);
					m_closed.insert(std::make_pair<Key, std::shared_ptr<AStar::Node>&>(Key({position.x, position.y}), adjacent));
					return true;
				}

				auto it = m_open.find({position.x, position.y});

				if (it == m_open.end())
				{
					adjacent.reset(new Node(position, node));
					adjacent->h = heuristic(adjacent->position);
					adjacent->g = movementCost(position, node->position, node->g);
					adjacent->f = adjacent->g + adjacent->h;
					m_open.insert(std::make_pair<Key, std::shared_ptr<AStar::Node>&>(Key({adjacent->position.x, adjacent->position.y}), adjacent));
					m_nodes.push_back(adjacent);
					m_reserved--;
				}
				else
				{
					adjacent = it->second.lock();
					
					if (adjacent)
					{
						int g = movementCost(position, node->position, node->g);

						if (g < adjacent->g)
						{
							adjacent->parent = node;
							adjacent->g = g;
							adjacent->f = adjacent->g + adjacent->h;
						}
					}
					else
						m_open.erase(it);
				}
			}
		}
	}

	return false;
}

int AStar::movementCost(const sf::Vector2i& position, const sf::Vector2i& parentPosition, const int parentMovementCost) const
{
	return parentMovementCost + ((position.x == parentPosition.x || position.y == parentPosition.y) ? 10:14);
}

float AStar::heuristic(const sf::Vector2i& position) const
{
	return (std::pow(m_destination.x - position.x, 2) + std::pow(m_destination.y - position.y, 2));
}

bool AStar::illegalDiagonal(const sf::Vector2i& a, const sf::Vector2i& b) const
{
	if (a.x == b.x || a.y == b.y)
		return false;

	int x = a.x > b.x ? a.x:b.x;
	int y = a.y > b.y ? a.y:b.y;
	
	if ((x == a.x && y == a.y) || (x == b.x && y == b.y)) // Max coordinates belong to the same node.
		return (m_map.tiles[x - 1][y].type == 1 || m_map.tiles[x][y - 1].type == 1);
	else
		return (m_map.tiles[x - 1][y - 1].type == 1 || m_map.tiles[x][y].type == 1);
}

