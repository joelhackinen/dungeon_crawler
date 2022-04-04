#ifndef WALLTILE_H
#define WALLTILE_H
#include "tile.h"

class wallTile : public Tile {
public:
	wallTile(const sf::Texture& t, float x, float y) : Tile(t, x, y) {}
	bool isPassable() const { return false; }
};


#endif
