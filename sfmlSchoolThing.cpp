#include <Graphics.hpp>
#include <Window.hpp>
#include <iostream>
#include <windows.h>
#include <chrono>
#include <iostream>
struct Player
{
	sf::Vector2f velocity;
	virtual void initPlayer(int width, int length, int xstart, int ystart, int r, int g, int b, sf::RectangleShape& player)
	{
	player = sf::RectangleShape(sf::Vector2f(width, length));
	player.setPosition(sf::Vector2f(xstart, ystart));
	player.setFillColor(sf::Color(r, g, b));
	}
	inline void affectGravity(sf::RectangleShape& player, float gravity)
	{	
		velocity.y += gravity;
		if (velocity.y > 9)
		velocity.y = 9;
		player.move(sf::Vector2f(0, velocity.y));
	}
};	

int main() {

sf::RectangleShape object(sf::Vector2f(0, 0));
sf::RectangleShape fl(sf::Vector2f(0, 0));
sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Gamus");
Player* p = new Player;
Player* floor = new Player;
p->velocity = sf::Vector2f(0.0f, 0.0f);
floor->velocity = sf::Vector2f(0.0f, 0.0f);
floor->initPlayer(500, 90, 0, 300, 200, 55, 0, fl);
p->initPlayer(64, 64, 0, 0, 255, 0, 0, object);

auto previousTime = std::chrono::high_resolution_clock::now();
while (window.isOpen())
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			break;
		}
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::Left:
				object.move(sf::Vector2f(-9.0f, 0.0f));
				break;
			case sf::Keyboard::Right:
				object.move(sf::Vector2f(9.0f, 0.0f));
				break;
			case sf::Keyboard::Up:
				object.move(sf::Vector2f(0.0f, -5.0f));
				break;
			default:
				break;
			}
		}
	}
	window.clear();
	if (object.getGlobalBounds().intersects(fl.getGlobalBounds()))
	{
		float playerBottom = object.getPosition().y + object.getSize().y;
		float floorTop = fl.getPosition().y;

		if (playerBottom > floorTop)
		{
			object.setPosition(object.getPosition().x, fl.getPosition().y - object.getSize().y);
			p->velocity.y = 0.0f;
		}
	}
	else { p->affectGravity(object, 0.1f); }
	window.draw(object);
	window.draw(fl);
	window.display();
}
return 0;
}
