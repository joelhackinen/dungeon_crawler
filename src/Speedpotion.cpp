#include "Speedpotion.h"
#include "Player.h"
#include <iostream>

Speedpotion::Speedpotion(const sf::Texture& t) : Consumable("speedpotion", t) {}


void Speedpotion::use(Player& p) {
	p.speedUp(20);
}
