#include "Textureholder.h"

// pay attention for example if the name is "floor", the image file has to be named "floor.png"
TextureHolder::TextureHolder() {
	std::vector<std::string> names = { "floor", "wall", "gun", "bow", "healthpotion", "speedpotion", "mosswall"};
	for (auto& n : names) {
		std::unique_ptr<sf::Texture> texture(new sf::Texture());
		std::stringstream ss;
		ss << "../images/" << n << ".png";
		std::string filename = ss.str();
		texture->loadFromFile(filename);
		texture->setSmooth(true);
		textures_.insert(std::make_pair(n, std::move(texture)));
	}
}

sf::Texture& TextureHolder::get(const std::string& n) {
	auto found = textures_.find(n);
	return *found->second;
}

const sf::Texture& TextureHolder::get(const std::string& n) const {
	auto found = textures_.find(n);
	return *found->second;
}