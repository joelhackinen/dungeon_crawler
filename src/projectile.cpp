#include "projectile.h"
#include "Player.h"

Projectile::Projectile(sf::CircleShape shape, float speed, int damage, sf::Vector2f dir, Player* player) : shape_(shape), speed_(speed), damage_(damage), direction_(dir), player_(player)
{
	auto bounds = shape_.getLocalBounds();
	shape_.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Projectile::setPosition(sf::Vector2f playerPos)
{
	shape_.setPosition(playerPos);
}

bool Projectile::move(float dt, Map& map)
{
	shape_.move(direction_ * dt * speed_);
	int gridPosX = (int)floor(getPosition().x / 50.f);
	int gridPosY = (int)floor(getPosition().y / 50.f);

	for (size_t i = 0; i < map.enemies_.size(); i++) {
		if (shape_.getGlobalBounds().intersects(map.enemies_[i]->getShape()->getGlobalBounds())) {
			if (!map.enemies_[i]->takeDamage(damage_)) { // in other words: enemy died
				player_->getTotalKill()++;
				player_->earnXP(map.enemies_[i]->getXP());
				map.enemies_.erase(map.enemies_.begin() + i);
			}
			return false;
		}
	}

	return map.tiles_[gridPosY][gridPosX]->isPassable();
}

sf::Vector2f Projectile::getPosition() const
{
	return shape_.getPosition();
}
