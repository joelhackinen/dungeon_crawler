#include "Gun.h"

Gun::Gun(const sf::Texture& t) : Weapon("Gun", 10, false, sf::CircleShape(2.f), t, 10) {}
