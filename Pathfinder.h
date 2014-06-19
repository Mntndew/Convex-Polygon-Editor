#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include "Node.h"
#include "Tile.h"

class Pathfinder
{
public:
	Pathfinder();
	Pathfinder(std::vector<std::vector<Tile>>* ptr_tiles);
	~Pathfinder();

	std::vector<sf::Vector2i> GetPath(const sf::Vector2i & p_start, const sf::Vector2i & p_destination);

	void GetMap(std::vector<std::vector<Tile>>* ptr_tiles);
	void Clear();

	std::vector<sf::Vector2i> m_path;

private:
	int m_current_node;

	std::vector<Node> m_nodes;
	std::vector<int> m_open, m_closed;
	sf::Vector2<int> m_destination;
	std::vector<std::vector<Tile>>* ptr_tiles;

	void CalculateHGF(Node* ptr_node, Node* ptr_destination);
	void SetPath(const sf::Vector2i & p_destination);
	void Clear(std::vector<int>* ptr_list);

	int GetLowestFOpen();
	int GetIndexOf(Node* ptr_node);
	int GetIndexOf(sf::Vector2i* ptr_node);
	int GetIndexOf(const sf::Vector2i & p_node);
	int GetIndexOf(std::vector<int> & p_list, Node* ptr_node);
};