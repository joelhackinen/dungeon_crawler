#ifndef HEALTHPOTION_H
#define HEALTHPOTION_H

#include "Consumable.h"

class Healthpotion : public Consumable {
public:
	Healthpotion(const sf::Texture&);
	void use(Player&);
};

#endif
