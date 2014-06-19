#include "Tile.h"

Tile::Tile() :
	m_x(0),
	m_y(0),
	m_tileType(TileType::Floor)
{

}

Tile::Tile(const unsigned int & p_x, const unsigned int & p_y, const TileType & p_tileType) :
	m_x(p_x),
	m_y(p_y),
	m_tileType(p_tileType)
{

}

Tile::~Tile()
{

}