#ifndef TILE_H
#define TILE_H
#include <SFML/Graphics.hpp>


class Tile {
public:
	Tile(const sf::Texture&, float, float);
	sf::Sprite sprite_;
	virtual bool isPassable() const = 0;
};



#endif
