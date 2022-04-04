#include "room.h"

Room::Room(int x, int y, int w, int h) : x1_(x), x2_(x + w), y1_(y), y2_(y + h), center_x_((int)floor((x1_ + x2_) / 2)), center_y_((int)floor((y1_ + y2_) / 2)), width_(w), height_(h) {}

bool Room::intersects(int x, int y, int w, int h) {
	return (x1_ < x + w + 2 && x2_ + 2 > x && y1_ < y + h + 2 && y2_ + 2 > y);
}
