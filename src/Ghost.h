#ifndef GHOST_H
#define GHOST_H
#include "Creature.h"

class Ghost : public Creature {
public:
	Ghost(sf::Vector2f initialPos);
	const std::string getName() const;
	void decideAction(Player&, float, Map&);
	void attack(Player&);
	void move(sf::Vector2f, Map&);
private:
	sf::Clock attackTimer_;
};

#endif
