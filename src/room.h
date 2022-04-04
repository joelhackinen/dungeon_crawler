#ifndef ROOM_H
#define ROOM_H
#include "wallTile.h"
#include "tile.h"
#include "floorTile.h"
#include <vector>
#include <memory>
#include <math.h>


class Room {
public:
	int x1_;
	int x2_;
	int y1_;
	int y2_;

	int width_;
	int height_;

	int center_x_;
	int center_y_;

	Room(int, int, int, int);

	bool intersects(int, int, int, int);


};


#endif
