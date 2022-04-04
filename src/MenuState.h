#ifndef MENUSTATE_H
#define MENUSTATE_H
#include "State.h"
#include <iostream>


class MenuState :
    public State
{
public:
    MenuState(sf::RenderWindow* window, std::stack<State*>* stateStack);

    virtual ~MenuState();

    void endState();

    void update(const float& dt);

    void render();

private:
    sf::RenderWindow* window_;
    std::stack<State*>* stateStack_;
    sf::Texture menuTexture_;
    sf::Texture buttonTexture_;
    sf::Texture buttonPressedTexture_;
    sf::Sprite menuBackground_;
    sf::Sprite playButton_;
    sf::Sprite quitButton_;
    sf::Sprite buttonPressed_;
    sf::Text playText_;
    sf::Text quitText_;
    sf::Font font_;
    sf::Vector2f mousePos_;
};

#endif
