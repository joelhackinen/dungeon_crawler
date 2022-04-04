#include "Ghost.h"
#include "Player.h"
#include "map.h"


Ghost::Ghost(sf::Vector2f initialPos) : Creature(10, 2, 100, 25, 120.f, sf::CircleShape(9.f), initialPos) {
	getShape()->setFillColor(sf::Color::Cyan);
}

const std::string Ghost::getName() const {
	return "Ghost";
}

void Ghost::decideAction(Player& p, float dt, Map& map) {
	if (playerIntersection(p)) {
		attack(p);
	}
	else if (playerDistance(p) < 200.f) {
		move(playerDirection(p) * dt, map);
	}
	else if (initialPosDistance() > 5.f) {
		move(directionToInitialPos() * dt, map);
	}
}

void Ghost::attack(Player& p) {
	if (attackTimer_.getElapsedTime().asMilliseconds() > 1000.f / getAttspeed()) {
		p.takeDamage(getDamage());
		attackTimer_.restart();
	}
}

void Ghost::move(sf::Vector2f direction, Map& map) {
	std::vector<std::shared_ptr<Tile>> nbs = getNeighboringTiles(map);
	getShape()->move(direction * getMovementSpeed());
	if (wallIntersection(nbs))
		getShape()->setFillColor(sf::Color::Magenta);
	else
		getShape()->setFillColor(sf::Color::Cyan);

}
