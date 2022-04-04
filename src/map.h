#ifndef MAP_H
#define MAP_H
#include <cstdlib>
#include <ctime>
#include <memory>
#include <vector>
#include <algorithm>
#include "room.h"
#include "Bow.h"
#include "Gun.h"
#include "Healthpotion.h"
#include "Speedpotion.h"
#include "Troll.h"
#include "Ghost.h"
#include "Textureholder.h"

class Map {
public:
	Map();
	std::vector< std::shared_ptr<Room> > rooms_;
	std::vector< std::vector< std::shared_ptr<Tile> > > tiles_;
	int getWidth() const { return width_; }
	int getHeight() const { return height_; }
	std::vector< std::unique_ptr < Creature > > enemies_;
	std::vector< std::shared_ptr< Weapon > > groundWeapons_;
	std::vector< std::shared_ptr< Consumable > > groundItems_;
private:
	void placeEnemies();
	void placeWeapons();
	void placeConsumables();
	void generateRooms();
	void generateCorridors();
	void setUpTiles();
	TextureHolder textures_;
	int width_;
	int height_;
	int roomAmount_;
	std::unique_ptr<sf::Texture> wallTexture_;
	std::unique_ptr<sf::Texture> floorTexture_;
	std::unique_ptr<sf::Texture> mossTexture_;
};



#endif
