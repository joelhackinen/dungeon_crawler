#ifndef PROJECTILE_H
#define PROJECTILE_H
#include <SFML/Graphics.hpp>
#include "map.h"
#include <math.h>

class Projectile {
public:
	Projectile(sf::CircleShape, float, int, sf::Vector2f, Player*);
	void setPosition(sf::Vector2f);
	bool move(float, Map&);
	sf::Vector2f getPosition() const;	// actually position of the origin
	sf::CircleShape shape_;
private:
	float speed_;
	sf::Vector2f direction_;
	int damage_;
	Player* player_;
};

#endif
