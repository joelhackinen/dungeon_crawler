#ifndef TEXTUREHOLDER_H
#define TEXTUREHOLDER_H
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <map>

class TextureHolder {
public:
	TextureHolder();
	sf::Texture& get(const std::string&);
	const sf::Texture& get(const std::string&) const;
private:
	std::map<std::string, std::unique_ptr<sf::Texture>> textures_;
};


#endif