#ifndef CONSUMABLE_H
#define CONSUMABLE_H

#include "Item.h"

class Consumable : public Item {
public:
	Consumable(std::string, const sf::Texture&);
	const std::string getType() const;
	virtual void use(Player&) = 0;
};

#endif
