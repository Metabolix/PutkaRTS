#include <memory>
#include <functional>
#include <string>
#include <boost/format.hpp>

#include "util/Path.hpp"

#include "SettingsMenu.hpp"
#include "gui/GUI.hpp"

#include "gui/widget/Button.hpp"
#include "gui/widget/Checkbox.hpp"
#include "gui/widget/DropDown.hpp"
#include "gui/widget/Slider.hpp"
#include "gui/widget/Label.hpp"

GUI::Menu::SettingsMenu::SettingsMenu(std::shared_ptr<Widget> parent_, sf::RenderWindow& window) :
	Menu(parent_),
	tabManager(new GUI::Widget::TabPanel(50, 100, 200, 16)) {

	insert(tabManager);
	insert(new GUI::Widget::Button("Accept", 400, 300, 100, 30, std::bind(&GUI::Menu::SettingsMenu::applyChanges, this, std::ref(window))));
	insert(new GUI::Widget::Button("Cancel", 400, 340, 100, 30, std::bind(&GUI::Menu::SettingsMenu::closeMenu, this)));

	buildGraphicsTab(1);
	buildInputTab(2);
}

void GUI::Menu::SettingsMenu::buildGraphicsTab(const GUI::Widget::TabPanel::TabKeyType& key) {

	//Get current settings.
	fullscreen = GUI::config.getBool("window.fullscreen", false);

	//List video modes.
	std::shared_ptr<GUI::Widget::DropDown> videoModeList(new GUI::Widget::DropDown(100, 200, 200, 25, 200, std::bind(&GUI::Menu::SettingsMenu::setVideoMode, this, std::placeholders::_1)));
	for (unsigned i = 0; i < sf::VideoMode::getFullscreenModes().size(); i++) {
		const sf::VideoMode& mode = sf::VideoMode::getFullscreenModes().at(i);

		if (mode.bitsPerPixel != 32) {
			continue;
		}

		if (mode.width > desktopMode.width || mode.height > desktopMode.height) {
			continue;
		}

		boost::format text("%1%x%2%");
		text % mode.width % mode.height;

		boost::format key("%1%");
		key % i;

		videoModeList->insertItem(key.str(), text.str());

		if (mode.width == window.getSize().x && mode.height == window.getSize().y) {
			videoModeList->selectItem(key.str());
			videoMode = std::stoi(key.str());
		}
	}

	//create tab
	tabManager->addTab(key, "Graphics");

	//insert resolution list
	tabManager->getTab(key)->insert(videoModeList);
	//insert fullscreen box
	tabManager->getTab(key)->insert(new GUI::Widget::Checkbox(100, 150, 20, 20, fullscreen, std::bind(&GUI::Menu::SettingsMenu::setFullScreen, this, std::placeholders::_1)));
	tabManager->getTab(key)->insert(new GUI::Widget::Label("Fullscreen (needs restart to take effect)", 125, 150, 20));
}

void GUI::Menu::SettingsMenu::buildInputTab(const GUI::Widget::TabPanel::TabKeyType& key) {

	//get current settings.
	zoomSpeed = GUI::config.getDouble("gameUI.zoomSpeed", 2);
	keyboardScrollSpeed = GUI::config.getDouble("gameUI.keyboardScrollSpeed", 1);
	borderScrollSpeed = GUI::config.getDouble("gameUI.borderScrollSpeed", 1);
	reverseDrag = GUI::config.getBool("gameUI.reverseDrag", false);

	//create tab
	tabManager->addTab(key, "Input");

	//zoomSpeed
	tabManager->getTab(key)->insert(new GUI::Widget::Label("Zooming speed", 100, 150, 16));
	tabManager->getTab(key)->insert(new GUI::Widget::Slider(100, 170, 200, 15, false, std::bind(&GUI::Menu::SettingsMenu::setZoomSpeed, this, std::placeholders::_1), 0.5f, 4.0f, zoomSpeed));
	//keyboardScrollSpeed
	tabManager->getTab(key)->insert(new GUI::Widget::Label("Keyboard scrolling speed", 100, 200, 16));
	tabManager->getTab(key)->insert(new GUI::Widget::Slider(100, 220, 200, 15, false, std::bind(&GUI::Menu::SettingsMenu::setKeyboardScrollSpeed, this, std::placeholders::_1), 0.5f, 2.0f, keyboardScrollSpeed));
	//borderScrollSpeed
	tabManager->getTab(key)->insert(new GUI::Widget::Label("Border scrolling speed", 100, 250, 16));
	tabManager->getTab(key)->insert(new GUI::Widget::Slider(100, 270, 200, 15, false, std::bind(&GUI::Menu::SettingsMenu::setBorderScrollSpeed, this, std::placeholders::_1), 0.5f, 2.0f, borderScrollSpeed));
	//reverseDrag
	tabManager->getTab(key)->insert(new GUI::Widget::Checkbox(100, 300, 20, 20, fullscreen, std::bind(&GUI::Menu::SettingsMenu::setReverseDrag, this, std::placeholders::_1)));
	tabManager->getTab(key)->insert(new GUI::Widget::Label("Reverse dragging", 125, 300, 20));
}

void GUI::Menu::SettingsMenu::applyChanges(sf::RenderWindow& window) {
	sf::VideoMode mode;
	try {
		mode = sf::VideoMode::getFullscreenModes().at(videoMode);
	} catch (std::out_of_range& e) {
		// TODO: What about current mode in windowed mode?
	}

	if (!mode.isValid()) {
		fullscreen = false;
	}

	bool changed = false;
	if (mode.width != window.getSize().x || mode.height != window.getSize().y) {
		GUI::config.setInt("window.size.x", mode.width);
		GUI::config.setInt("window.size.y", mode.height);
		changed = true;
	}

	if (fullscreen || GUI::config.getBool("window.fullscreen", false)) {
		GUI::config.setBool("window.fullscreen", fullscreen);
		changed = true;
	}

	if (changed) {
		GUI::createWindow();
	}

	GUI::config.setDouble("gameUI.zoomSpeed", zoomSpeed);
	GUI::config.setDouble("gameUI.keyboardScrollSpeed", keyboardScrollSpeed);
	GUI::config.setDouble("gameUI.borderScrollSpeed", borderScrollSpeed);
	GUI::config.setBool("gameUI.reverseDrag", reverseDrag);

	GUI::config.save();
	closeMenu();
}

void GUI::Menu::SettingsMenu::setFullScreen(bool state) {
	fullscreen = state;
}

void GUI::Menu::SettingsMenu::setVideoMode(const GUI::Widget::List::Item& item) {
	if (item.key.empty()) {
		return;
	}

	// Non-empty keys are valid.
	videoMode = std::stoi(item.key);
}

void GUI::Menu::SettingsMenu::setBorderScrollSpeed(float value) {
	borderScrollSpeed = value;
}

void GUI::Menu::SettingsMenu::setKeyboardScrollSpeed(float value) {
	keyboardScrollSpeed = value;
}

void GUI::Menu::SettingsMenu::setZoomSpeed(float value) {
	zoomSpeed = value;
}

void GUI::Menu::SettingsMenu::setReverseDrag(bool state) {
	reverseDrag = state;
}
