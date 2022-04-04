#include "map.h"
#include <iostream>

//width at least 80
Map::Map() : width_(200), height_((int)floor(width_ / 10)), roomAmount_((int)floor(width_ * height_ / 100)) {
	srand(std::time(nullptr));
	generateRooms();
	generateCorridors();
	setUpTiles();
	placeWeapons();
	placeConsumables();
	placeEnemies();
}

void Map::placeEnemies() {
	for (int i = 1; i < roomAmount_; i++) {
		std::shared_ptr<Room>& room = rooms_[i];
		if (rand() % 10 >= 8)
			enemies_.emplace_back(std::make_unique<Ghost>(sf::Vector2f((room->x1_ + (rand() % (room->width_ - 1) + 1)) * 50.f, (room->y1_ + (rand() % (room->height_ - 1) + 1)) * 50.f)));
		else
			enemies_.emplace_back(std::make_unique<Troll>(sf::Vector2f((room->x1_ + (rand() % (room->width_ - 1) + 1)) * 50.f, (room->y1_ + (rand() % (room->height_ - 1) + 1)) * 50.f)));
	}
}

void Map::placeWeapons() {
	// initial weapon
	if (rand() % 2 == 0) {
		groundWeapons_.push_back(std::make_shared<Gun>(textures_.get("gun")));
	} else {
		groundWeapons_.push_back(std::make_shared<Bow>(textures_.get("bow")));
	}
	groundWeapons_[0]->setPosition(rooms_[0]->center_x_ * 50.f, rooms_[0]->center_y_ * 50.f);

	// other weapons, random locations
	int amount = rand() % ((int)floor(roomAmount_ / 6)) + 1;
	for (int i = 1; i <= amount; i++) {
		std::shared_ptr<Room>& room = rooms_[rand() % (roomAmount_ - 1) + 1];
		if (rand() % 2 == 0)
			groundWeapons_.push_back(std::make_shared<Gun>(textures_.get("gun")));
		else
			groundWeapons_.push_back(std::make_shared<Bow>(textures_.get("bow")));
		groundWeapons_[i]->setPosition((room->x1_ + (rand() % (room->width_ - 1) + 1)) * 50.f, (room->y1_ + (rand() % (room->height_ - 1) + 1)) * 50.f);
	}
}

void Map::placeConsumables() {
	int amount = ((int)floor(roomAmount_ / 5));
	for (int i = 0; i < amount; i++) {
		std::shared_ptr<Room>& room = rooms_[rand() % (roomAmount_ - 1) + 1];
		if (rand() % 10 < 7)
			groundItems_.push_back(std::make_shared<Healthpotion>(textures_.get("healthpotion")));
		else 
			groundItems_.push_back(std::make_shared<Speedpotion>(textures_.get("speedpotion")));
		groundItems_[i]->setPosition((room->x1_ + (rand() % (room->width_ - 1) + 1)) * 50.f, (room->y1_ + (rand() % (room->height_ - 1) + 1)) * 50.f);
	}
	std::cout << groundItems_.size() << std::endl;
}

void Map::generateRooms() {
	int amount = 0;

	while (amount < roomAmount_) {
		int w = 2 + rand() % (4);					// width of the room
		int h = 2 + rand() % (4);					// height of the room
		int x = rand() % (width_ - w - 2) + 1;		// x-coordinate of left side of the room
		int y = rand() % (height_ - h - 2) + 1;		// y-coordinate of bottom side of the room

		// checks if any of the existing rooms will intersect with the new one
		auto intersecting = [x, y, w, h](std::shared_ptr<Room> other) { return other->intersects(x, y, w, h); };
		bool failed = std::any_of(rooms_.begin(), rooms_.end(), intersecting);

		// if it doesnt intersect, a new room is created
		if (!failed) {
			rooms_.push_back(std::make_shared<Room>(x, y, w, h));
			amount++;
		}
	}
}

void Map::generateCorridors() {
	// sorting the vector of rooms
	auto cmp_x = [](std::shared_ptr<Room> room1, std::shared_ptr<Room> room2) { return room1->center_x_ < room2->center_x_; };
	std::sort(rooms_.begin(), rooms_.end(), cmp_x);

	for (int i = 1; i < roomAmount_; i++) {
		int newCenterX = rooms_[i]->center_x_;
		int newCenterY = rooms_[i]->center_y_;
		int prevCenterX = rooms_[i - 1]->center_x_;
		int prevCenterY = rooms_[i - 1]->center_y_;

		// determines if the corridor starts with vertical or horizontal path
		// for example:
		// ROOM-------				ROOM
		//			  |				 |
		//			  |		OR		 |
		//			  |				 |
		//			ROOM			  -------ROOM

		if (rand() % 2 == 0) {
			if (newCenterX >= prevCenterX) {
				rooms_.push_back(std::make_shared<Room>(prevCenterX, newCenterY, newCenterX - prevCenterX, 0));
				if (newCenterY >= prevCenterY) {
					rooms_.push_back(std::make_shared<Room>(prevCenterX, prevCenterY, 0, newCenterY - prevCenterY));
				}
				else {
					rooms_.push_back(std::make_shared<Room>(prevCenterX, newCenterY, 0, prevCenterY - newCenterY));
				}
			}
			else {
				rooms_.push_back(std::make_shared<Room>(newCenterX, newCenterY, prevCenterX - newCenterX, 0));
				if (newCenterY >= prevCenterY) {
					rooms_.push_back(std::make_shared<Room>(prevCenterX, prevCenterY, 0, newCenterY - prevCenterY));
				}
				else {
					rooms_.push_back(std::make_shared<Room>(prevCenterX, newCenterY, 0, prevCenterY - newCenterY));
				}
			}
		}
		else {
			if (newCenterY >= prevCenterY) {
				rooms_.push_back(std::make_shared<Room>(newCenterX, prevCenterY, 0, newCenterY - prevCenterY));
				if (newCenterX >= prevCenterX) {
					rooms_.push_back(std::make_shared<Room>(prevCenterX, prevCenterY, newCenterX - prevCenterX, 0));
				}
				else {
					rooms_.push_back(std::make_shared<Room>(newCenterX, prevCenterY, prevCenterX - newCenterX, 0));
				}
			}
			else {
				rooms_.push_back(std::make_shared<Room>(newCenterX, newCenterY, 0, prevCenterY - newCenterY));
				if (newCenterX >= prevCenterX) {
					rooms_.push_back(std::make_shared<Room>(prevCenterX, prevCenterY, newCenterX - prevCenterX, 0));
				}
				else {
					rooms_.push_back(std::make_shared<Room>(newCenterX, prevCenterY, prevCenterX - newCenterX, 0));
				}
			}
		}
	}
}

void Map::setUpTiles() {
	std::vector< std::shared_ptr<Tile> > row;
	for (int j = 0; j < height_; j++) {
		for (int i = 0; i < width_; i++) {
			auto isFloor = [i, j](std::shared_ptr<Room> room) { return ((i >= room->x1_ && i <= room->x2_) && (j >= room->y1_ && j <= room->y2_)); };
			bool roomInThisPosition = std::any_of(rooms_.begin(), rooms_.end(), isFloor);
			if (roomInThisPosition) {
				row.push_back(std::make_shared<floorTile>(textures_.get("floor"), i * 50, j * 50));
			}
			else {
				if (rand() % 2 == 0)
					row.push_back(std::make_shared<wallTile>(textures_.get("wall"), i * 50, j * 50));
				else
					row.push_back(std::make_shared<wallTile>(textures_.get("mosswall"), i * 50, j * 50));
			}
		}
		tiles_.push_back(row);
		row.clear();
	}
}
