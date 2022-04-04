#include "Healthpotion.h"
#include "Player.h"
#include <iostream>

Healthpotion::Healthpotion(const sf::Texture& t) : Consumable("healthpotion", t) {}


void Healthpotion::use(Player& p) {
	p.heal(30);
}
