#include "Bow.h"

Bow::Bow(const sf::Texture& t) : Weapon("Bow", 30, false, sf::CircleShape(4.f), t, 3) {}
