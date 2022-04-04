#ifndef CREATURE_H
#define CREATURE_H

#include <SFML/Graphics.hpp>
#include <string>
#include <math.h>
class Player;
class Map;
class Tile;


class Creature {
public:
    Creature(int damage, int attspeed, int hp, int xp, float movementspeed, sf::CircleShape shape, sf::Vector2f initialPos);
    virtual const std::string getName() const = 0;
    virtual void attack(Player&) = 0;
    virtual void decideAction(Player&, float, Map&) = 0;
    virtual void move(sf::Vector2f, Map&);
    sf::CircleShape* getShape();
    int getHp() const;
    int getAttspeed() const;
    int getDamage() const;
    float getMovementSpeed() const;
    const int& getXP() const;
    sf::Vector2f getInitialPos() const;
    void setPosition(sf::Vector2f);
    void setPosition(float, float);
    sf::Vector2f getPosition() const;
    sf::Vector2f directionToInitialPos() const;
    float initialPosDistance() const;
    float playerDistance(Player&) const;
    sf::Vector2f playerDirection(Player&) const;
    bool playerIntersection(Player&);
    bool takeDamage(int);
    std::vector<std::shared_ptr<Tile>> getNeighboringTiles(Map&) const;
    bool wallIntersection(std::vector<std::shared_ptr<Tile>>&);
private:
    int damage_;
    int attspeed_;
    int hp_;
    int xp_;
    float movementspeed_;
    sf::CircleShape shape_;
    sf::Vector2f initialPos_;

};

#endif
