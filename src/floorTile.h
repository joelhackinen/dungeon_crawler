#ifndef FLOORTILE_H
#define FLOORTILE_H
#include "tile.h"

class floorTile : public Tile {
public:
	floorTile(const sf::Texture& t, float x, float y) : Tile(t, x, y) {}
	bool isPassable() const { return true; }
};


#endif
