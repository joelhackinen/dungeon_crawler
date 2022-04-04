#include "tile.h"

Tile::Tile(const sf::Texture& texture, float x, float y) {
	sprite_.setTexture(texture);
	sprite_.setPosition(sf::Vector2f(x, y));
}
