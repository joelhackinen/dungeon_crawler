#ifndef SPEEDPOTION_H
#define SPEEDPOTION_H

#include "Consumable.h"

class Speedpotion : public Consumable {
public:
	Speedpotion(const sf::Texture&);
	void use(Player&);
};

#endif
