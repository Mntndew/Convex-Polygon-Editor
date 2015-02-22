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
	sf::Vector2i m_source, m_destination;

	static bool compare(const std::shared_ptr<Node> a, const std::shared_ptr<Node> b)
	{
		return a->f > b->f;
	}

	void recalculate(std::shared_ptr<Node> node, const std::shared_ptr<Node> parent);

	void calculateHeuristic(std::shared_ptr<Node> node);

	bool illegalDiagonal(const sf::Vector2i& a, const sf::Vector2i& b) const;

	bool inOpen(const sf::Vector2i& position) const;

	std::shared_ptr<Node> getOpenNode(const sf::Vector2i& position);

	bool checkAdjacentNodes(const std::shared_ptr<Node> node);

	void constructPath();

	void search();

public:
	AStar(Map& map, const sf::Vector2i& source, const sf::Vector2i& destination);
};

