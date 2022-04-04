#ifndef ITEM_H
#define ITEM_H

#include <SFML/Graphics.hpp>
#include <string>
#include <math.h>
class Player;

class Item {
public:
    Item(std::string, const sf::Texture &);
    const std::string GetName() const;
    void setPosition(float, float);
    sf::Vector2f getPosition() const;
    virtual const std::string getType() const = 0;
    float playerDistance(const Player&) const;
    sf::Sprite sprite_;
private:
    std::string name_;
};

#endif
