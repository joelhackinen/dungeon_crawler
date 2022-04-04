#include "Player.h"
#include <iostream>


Player::Player()
{
	//Initialize player
	shape_.setSize(sf::Vector2f(20.f, 20.f));
	shape_.setFillColor(sf::Color::White);
	auto bounds = shape_.getLocalBounds();
	shape_.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	weapon_ = nullptr;
	totalKill_ = 0;

	//Initialize the stats
	movementSpeed_ = 150.f;

	//Initialize HP
	maxHp_ = 100;
	currentHp_ = 100;
	alive_ = true;

	//Initialize XP
	currentXP_ = 0;
	xpThreshold_ = 100;
	totalXP_ = 0;
	level_ = 1;
}

Player::~Player()
{
	inventory_.clear();
	projectiles_.clear();
}

sf::Vector2f Player::getPosition() const {
	return shape_.getPosition();
}

void Player::move(const float x, const float y, const float& dt)
{
	shape_.move(x * movementSpeed_ * dt, y * movementSpeed_ * dt);
}


void Player::update(float dt, Map& map)
{

	for (size_t i = 0; i < projectiles_.size(); i++) {
		if (!projectiles_[i]->move(dt, map)) {
			projectiles_.erase(projectiles_.begin() + i);
		}
	}

}

void Player::render(sf::RenderWindow* window)
{
	window->draw(shape_);

	for (size_t i = 0; i < projectiles_.size(); i++) {
		window->draw(projectiles_[i]->shape_);
	}
}

void Player::takeDamage(int damage)
{
	if (damage < currentHp_) {
		currentHp_ -= damage;
	}
	else {
		alive_ = false;
		currentHp_ = 0;
	}
}

void Player::earnXP(int xp)
{
	if (currentXP_ + xp < xpThreshold_) {
		currentXP_ += xp;
		totalXP_ += xp;
	}
	else if (currentXP_ + xp == xpThreshold_) {
		currentXP_ = 0;
		level_ += 1;
		movementSpeed_ += 5;
		currentHp_ += 10;
		maxHp_ += 10;
	}
	else {
		int temp = currentXP_ + xp;
		int increase = temp / xpThreshold_;
		currentXP_ = temp % xpThreshold_;
		level_ += increase;
		maxHp_ += 10 * increase;
		currentHp_ += 10 * increase;
		movementSpeed_ += 5 * increase;
	}
}

void Player::heal(int amount)
{
	if (getHp() + amount <= getMaxHp())
		currentHp_ += amount;
	else
		currentHp_ = maxHp_;
}

void Player::speedUp(float amount) {
	movementSpeed_ += amount;
}

const int& Player::getHp() const
{
	return currentHp_;
}

const int& Player::getMaxHp() const
{
	return maxHp_;
}


const bool& Player::isAlive() const
{
	return alive_;
}

const float Player::getHpPercent() const
{
	return float(getHp()) / getMaxHp();
}

const int& Player::getcurrentXP() const
{
	return currentXP_;
}

const int& Player::getTotalXP() const
{
	return totalXP_;
}

const int& Player::getXPThreshold() const
{
	return xpThreshold_;
}

const float Player::getXPPercent() const
{
	return float(getcurrentXP()) / getXPThreshold();
}

const int& Player::getLevel() const
{
	return level_;
}

int& Player::getTotalKill()
{
	return totalKill_;
}

std::vector<std::shared_ptr<Consumable>>& Player::getInventory()
{
	return inventory_;
}


std::vector<std::shared_ptr<Tile>> Player::getNeighboringTiles(Map& map) const
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

void Player::shoot(sf::Vector2f direction)
{
	if (hasWeapon()) {
		if (shootingTimer_.getElapsedTime().asMilliseconds() >= 1000.f / weapon_->getAttSpeed()) {
			std::unique_ptr<Projectile> p = std::make_unique<Projectile>(weapon_->getProjectile(), 600, weapon_->GetDamage(), direction, this);
			p->setPosition(getPosition());
			projectiles_.push_back(std::move(p));
			shootingTimer_.restart();
		}
	}
	else {
		std::cout << "You can't shoot without a weapon!" << std::endl;
	}
}

bool Player::pickItem(std::shared_ptr<Consumable>& item) {
	if (itemTimer_.getElapsedTime().asMilliseconds() > 500) {
		std::cout << "Item picked" << std::endl;
		inventory_.push_back(std::move(item));
		itemTimer_.restart();
		return true;
	}
	return false;
}

bool Player::pickWeapon(std::shared_ptr<Weapon>& pickedWeapon) {
	if (itemTimer_.getElapsedTime().asMilliseconds() > 500) {
		std::cout << "Weapon picked" << std::endl;
		weapon_ = std::move(pickedWeapon);
		itemTimer_.restart();
		return true;
	}
	return false;
}

void Player::dropWeapon(std::vector<std::shared_ptr<Weapon>>& weapons) {
	if (itemTimer_.getElapsedTime().asMilliseconds() > 500) {
		if (hasWeapon()) {
			weapon_->setPosition(getPosition().x, getPosition().y);
			weapons.push_back(std::move(weapon_));
		}
		else {
			std::cout << "You don't have a weapon!" << std::endl;
		}
		itemTimer_.restart();
	}
}

void Player::dropItem(std::vector<std::shared_ptr<Consumable>>& items, std::string name) {
	auto potion = std::find_if(getInventory().begin(), getInventory().end(),
		[name](std::shared_ptr<Consumable>& a) { return a->GetName() == name; });
	if (potion == getInventory().end())
		std::cout << "You don't have any " << name << " to drop!" << std::endl;
	else {
		std::cout << "Dropping " << name << std::endl;
		(*potion)->setPosition(getPosition().x, getPosition().y);
		items.push_back(*potion);
		getInventory().erase(potion);
	}
}

void Player::useItem(std::string name) {
	auto potion = std::find_if(getInventory().begin(), getInventory().end(),
		[name](std::shared_ptr<Consumable>& a) { return a->GetName() == name; });
	if (potion == getInventory().end())
		std::cout << "You don't have any " << name << " to use!" << std::endl;
	else {
		std::cout << "Using " << name << std::endl;
		(*potion)->use(*this);
		getInventory().erase(potion);
	}
}

bool Player::hasWeapon() const {
	return weapon_ != nullptr;
}
