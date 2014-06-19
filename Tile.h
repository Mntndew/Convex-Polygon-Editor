#pragma once

enum TileType
{
	Floor,
	Wall,
};

struct Tile
{
	Tile();
	Tile(const unsigned int & p_x, const unsigned int & p_y, const TileType & p_tileType);
	~Tile();

	unsigned int m_x, m_y;

	TileType m_tileType;
};