#ifndef GAMESTATE_H
#define GAMESTATE_H


#include "State.h"
#include "Player.h"
#include <iostream>
#include <math.h>
#include "map.h"
#include <sstream>

class GameState :
    public State
{
public:
    GameState(sf::RenderWindow* window, std::stack<State*>* stateStack);

    virtual ~GameState();

    void endState();

    void update(const float& dt);

    void render();

    void renderMap();

    void renderUI();

    Map map_;

    sf::View gameView_;

    sf::View minimapView_;

private:
    void sortItemsByDistance();
    bool playerCollision(std::vector< std::shared_ptr< Tile > >&);
    Player player_;
    float duration_;
    sf::RenderWindow* window_;
    std::stack<State*>* stateStack_;
    bool isPause_;
    sf::RectangleShape healthBar_;
    sf::RectangleShape xpBar_;
    sf::RectangleShape backgroundHpBar_;
    sf::RectangleShape backgroundXpBar_;
    sf::Texture hpFrameTexture_;
    sf::Sprite hpFrame_;
    sf::Text hpText_;
    sf::Text levelText_;
    sf::Font font_;
    sf::RectangleShape slot1_;
    sf::RectangleShape slot2_;
    sf::RectangleShape weaponSlot_;
    sf::Texture minimapFrameTexture_;
    sf::Sprite minimapFrame_;
    sf::Texture panelTexture_;
    sf::Texture buttonTexture_;
    sf::Texture buttonPressedTexture_;
    sf::Sprite panel_;
    sf::Sprite quitButton_;
    sf::Sprite buttonPressed_;
    sf::Text summaryText_;
    sf::Text pauseText_;
};

#endif
