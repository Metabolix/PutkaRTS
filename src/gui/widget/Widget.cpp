#include "Widget.hpp"

namespace GUI {
	namespace Widget {
		sf::Font font;
		namespace Color {
			const sf::Color background(0x55, 0x44, 0x33);
			const sf::Color backgroundHover(0xcc, 0x99, 0x66);
			const sf::Color border(0, 0, 0);
			const sf::Color text(0xff, 0xff, 0xff);
		}
	}
}

sf::String GUI::Widget::stringFromUtf8(const std::string& s) {
	std::size_t l = sf::Utf<8>::count(s.begin(), s.end());
	std::basic_string<sf::Uint32> u32(l, ' ');
	sf::Utf<8>::toUtf32(s.begin(), s.end(), u32.begin());
	return sf::String(u32);
}

void GUI::Widget::Widget::setPosition(float x, float y, float width, float height) {
	position.left = x;
	position.top = y;
	setSize(width, height);
}

void GUI::Widget::Widget::move(float dx, float dy) {
	position.left += dx;
	position.top += dy;
}

void GUI::Widget::Widget::setSize(float width, float height) {
	position.width = width;
	position.height = height;
}
