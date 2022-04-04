#include "Weapon.h"

Weapon::Weapon(std::string name, int damage, bool melee, sf::CircleShape projectile_shape, const sf::Texture& t, int attSpeed)
    : Item(name, t), damage_(damage), melee_(melee), projectile_shape_(projectile_shape), attackSpeed_(attSpeed) {}

const sf::CircleShape& Weapon::getProjectile() const {
    return projectile_shape_;
}

int Weapon::GetDamage() const {
    return damage_;
}

bool Weapon::isMelee() const {
    return melee_;
}

const std::string Weapon::getType() const {
    return "weapon";
}

const int Weapon::getAttSpeed() const {
    return attackSpeed_;
}
