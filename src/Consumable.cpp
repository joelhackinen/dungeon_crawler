#include "Consumable.h"

Consumable::Consumable(std::string name, const sf::Texture& t) : Item(name, t) { }

const std::string Consumable::getType() const {
	return "consumable";
}
