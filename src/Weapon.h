#ifndef WEAPON_H
#define WEAPON_H

#include "Item.h"

class Weapon : public Item {
public:
    Weapon(std::string, int, bool, sf::CircleShape, const sf::Texture&, int);
    int GetDamage() const;
    bool isMelee() const;
    const sf::CircleShape& getProjectile() const;
    const std::string getType() const;
    const int getAttSpeed() const;
private:
    int attackSpeed_;   // shots per second
    sf::CircleShape projectile_shape_;
    int damage_;
    bool melee_;

};

#endif
