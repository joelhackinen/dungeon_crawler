#include "Troll.h"
#include "Player.h"


Troll::Troll(sf::Vector2f initialPos) : Creature(20, 1, 100, 20, 100.f, sf::CircleShape(10.f), initialPos) {}

const std::string Troll::getName() const {
	return "Troll";
}

void Troll::decideAction(Player& p, float dt, Map& map) {
	if (playerIntersection(p)) {
		attack(p);
	}
	else if (playerDistance(p) < 300.f) {
		move(playerDirection(p) * dt, map);
	}
	else if (initialPosDistance() > 5.f) {
		move(directionToInitialPos() * dt, map);
	}
}

void Troll::attack(Player& p) {
	if (attackTimer_.getElapsedTime().asMilliseconds() > 1000.f / getAttspeed()) {
		p.takeDamage(getDamage());
		attackTimer_.restart();
	}
}
