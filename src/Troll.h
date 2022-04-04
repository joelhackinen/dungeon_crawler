#ifndef TROLL_H
#define TROLL_H
#include "Creature.h"

class Troll : public Creature {
public:
	Troll(sf::Vector2f initialPos);
	const std::string getName() const;
	void decideAction(Player&, float, Map&);
	void attack(Player&);
private:
	sf::Clock attackTimer_;
};

#endif
