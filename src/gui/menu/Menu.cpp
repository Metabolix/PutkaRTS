#include <functional>

#include "util/Path.hpp"

#include "Menu.hpp"
#include "gui/GUI.hpp"

GUI::TextureCache GUI::Menu::Menu::textures;

GUI::Menu::Menu::Menu(std::shared_ptr<Widget> parent_) {
	const sf::Texture& logoTexture(textures.get(Path::findDataPath("graphics/logo.png")));
	logoSprite.setTexture(logoTexture);
	logoSprite.setOrigin(logoTexture.getSize().x / 2, 0);
	logoSprite.setPosition(320, 1);

	openMenu(parent_);
}

void GUI::Menu::Menu::openMenu(std::shared_ptr<Widget> parent_) {
	menuIsOpen = true;
	parent = parent_;
}

void GUI::Menu::Menu::closeMenu() {
	menuIsOpen = false;
	if (currentWidget.get() == this) {
		GUI::currentWidget = parent;
	}
}

void GUI::Menu::Menu::draw(sf::RenderWindow& window) {
	// Make view that is as close to 640x480 as possible and centered.
	view = window.getDefaultView();
	view.zoom(1.0f / std::min(view.getSize().x / 640, view.getSize().y / 480));
	view.setCenter(320, 240);
	window.setView(view);

	window.clear(sf::Color(0xcc, 0x66, 0x33));
	Container::draw(window);
	window.draw(logoSprite);
}
