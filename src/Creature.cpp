#include "Creature.h"
#include "Player.h"
#include "map.h"
#include "tile.h"


Creature::Creature(int damage, int attspeed, int hp, int xp, float movementspeed, sf::CircleShape shape, sf::Vector2f initialPos)
	: damage_(damage), attspeed_(attspeed), hp_(hp), xp_(xp), movementspeed_(movementspeed), shape_(shape), initialPos_(initialPos)
{
	auto bounds = shape.getLocalBounds();
	shape_.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	shape_.setPosition(initialPos);
	shape_.setFillColor(sf::Color::Red);
}

sf::CircleShape* Creature::getShape() {
	return &shape_;
}

int Creature::getHp() const {
	return hp_;
}

int Creature::getAttspeed() const {
	return attspeed_;
}

int Creature::getDamage() const {
	return damage_;
}

const int& Creature::getXP() const
{
	return xp_;
}

sf::Vector2f Creature::getInitialPos() const {
	return initialPos_;
}

void Creature::setPosition(sf::Vector2f pos) {
	shape_.setPosition(pos);
}

void Creature::setPosition(float x, float y) {
	shape_.setPosition(sf::Vector2f(x, y));
}

sf::Vector2f Creature::getPosition() const {
	return shape_.getPosition();
}

float Creature::getMovementSpeed() const {
	return movementspeed_;
}

sf::Vector2f Creature::directionToInitialPos() const {
	sf::Vector2f diff = getInitialPos() - getPosition();
	return diff / (float)sqrt(pow(diff.x, 2) + pow(diff.y, 2));
}

float Creature::playerDistance(Player& p) const {
	sf::Vector2f diff = getPosition() - p.getPosition();
	return (float)sqrt(pow(diff.x, 2) + pow(diff.y, 2));
}

float Creature::initialPosDistance() const {
	sf::Vector2f diff = getInitialPos() - getPosition();
	return (float)sqrt(pow(diff.x, 2) + pow(diff.y, 2));
}

sf::Vector2f Creature::playerDirection(Player& p) const {
	sf::Vector2f diff = p.getPosition() - getPosition();
	return diff / (float)sqrt(pow(diff.x, 2) + pow(diff.y, 2));
}

bool Creature::playerIntersection(Player& p) {
	return shape_.getGlobalBounds().intersects(p.getPlayerShape()->getGlobalBounds());
}

void Creature::move(sf::Vector2f direction, Map& map) {
	std::vector<std::shared_ptr<Tile>> nbs = getNeighboringTiles(map);
	shape_.move(sf::Vector2f(direction.x, 0.f) * movementspeed_);
	if (wallIntersection(nbs))
		shape_.move(sf::Vector2f(-direction.x, 0.f) * movementspeed_);
	
	shape_.move(sf::Vector2f(0.f, direction.y) * movementspeed_);
	if (wallIntersection(nbs))
		shape_.move(sf::Vector2f(0.f, -direction.y) * movementspeed_);
}

bool Creature::takeDamage(int damage) {
	this->hp_ -= damage;
	return this->hp_ > 0;
}

std::vector<std::shared_ptr<Tile>> Creature::getNeighboringTiles(Map& map) const
{
	int flooredX = (int)floor(getPosition().x / 50.f);
	int leftBound = flooredX - 1;
	int rightBound = flooredX + 1;
	int flooredY = (int)floor(getPosition().y / 50.f);
	int topBound = flooredY - 1;
	int bottomBound = flooredY + 1;

	std::vector < std::shared_ptr < Tile > > neighbours;
	for (int j = topBound; j <= bottomBound; j++) {
		for (int i = leftBound; i <= rightBound; i++) {
			neighbours.push_back(map.tiles_[j][i]);
		}
	}
	return neighbours;
}

bool Creature::wallIntersection(std::vector< std::shared_ptr< Tile > >& nbs) {
	sf::CircleShape* shape = &shape_;
	auto check = [shape](std::shared_ptr<Tile> tile) { return !tile->isPassable() && tile->sprite_.getGlobalBounds().intersects(shape->getGlobalBounds()); };
	return std::any_of(nbs.begin(), nbs.end(), check);
}
