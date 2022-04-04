#ifndef PLAYER_H
#define PLAYER_H


#include "SFML/Graphics.hpp"
#include "tile.h"
#include <math.h>
#include "map.h"
#include "projectile.h"


class Player
{
public:
	Player();

	virtual ~Player();

	sf::Vector2f getPosition() const;

	void move(const float x, const float y, const float& dt);

	void update(float, Map&);

	void render(sf::RenderWindow* window);

	sf::RectangleShape* getPlayerShape() { return &shape_; }

	void takeDamage(int damage);

	void earnXP(int xp);

	void heal(int amount);

	void speedUp(float amount);

	const int& getHp() const;

	const int& getMaxHp() const;

	const bool& isAlive() const;

	const float getHpPercent() const;

	const int& getcurrentXP() const;

	const int& getTotalXP() const;

	const int& getXPThreshold() const;

	const float getXPPercent() const;

	const int& getLevel() const;

	int& getTotalKill();

	std::vector<std::shared_ptr<Consumable>>& getInventory();

	std::vector<std::shared_ptr<Tile>> getNeighboringTiles(Map&) const;

	void shoot(sf::Vector2f);

	bool pickWeapon(std::shared_ptr<Weapon>&);

	bool pickItem(std::shared_ptr<Consumable>&);

	void dropWeapon(std::vector<std::shared_ptr<Weapon>>&);

	void dropItem(std::vector<std::shared_ptr<Consumable>>&, std::string);

	void useItem(std::string);

	bool hasWeapon() const;

	std::shared_ptr<Weapon> weapon_;

private:
	std::vector<std::shared_ptr<Consumable>> inventory_;
	std::vector<std::unique_ptr<Projectile>> projectiles_;
	sf::RectangleShape shape_;
	sf::Clock shootingTimer_;
	sf::Clock itemTimer_;
	float movementSpeed_;
	bool alive_;
	int maxHp_;
	int currentHp_;
	int currentXP_;
	int totalXP_;
	int xpThreshold_;
	int level_;
	int totalKill_;
};

#endif
