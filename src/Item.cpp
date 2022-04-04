#include "Item.h"
#include "Player.h"

Item::Item(std::string name, const sf::Texture& t) : name_(name) {
    sprite_.setTexture(t);
    auto bounds = sprite_.getLocalBounds();
    sprite_.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

const std::string Item::GetName() const {
    return name_;
}

void Item::setPosition(float x, float y) {
    sprite_.setPosition(sf::Vector2f(x, y));
}

sf::Vector2f Item::getPosition() const {
    return sprite_.getPosition();
}

float Item::playerDistance(const Player& p) const {
    sf::Vector2f diff = getPosition() - p.getPosition();
    return sqrt(pow(diff.x, 2) + pow(diff.y, 2));
}
