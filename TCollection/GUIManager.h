#pragma once

#include <string>
#include <map>
#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>

enum TextLocation
{
	Center,
	TopLeft,
	TopRight,
	Left,
	Right,
	BottomLeft,
	BottomRight
};

class Button;
class Window;
class Image;

class Button
{
public:
	sf::RectangleShape Shape;
	sf::Vector2f Position;
	sf::Vector2f Size;
	sf::Color BackgroundColor;
	sf::Color TextColor;
	sf::Color HighlightColor;
	sf::Font Font;
	sf::Text Text;
	bool Enabled;
	bool Highlighted;
	bool Clicked;
	bool Held;
	TextLocation TextLocation;

	Button* SetBackgroundColor(int r, int g, int b);
	Button* SetHighlightColor(int r, int g, int b);
	Button* SetTextColor(int r, int g, int b);
	Button* SetText(std::string text);

	Button(float x, float y, float width, float height);
	~Button();

	void(*ClickHandler)(Window*, Button*);
	void(*HeldHandler)(Window*, Button*);
	void(*RightClickHandler)(Window*, Button*);

	void SetClickHandler(void(*f)(Window*, Button*));
	void SetHeldHandler(void(*f)(Window*, Button*));
	void SetRightClickHandler(void(*f)(Window*, Button*));
};

class Image
{
public:
	sf::Sprite Sprite;
	sf::Texture Texture;
	sf::Vector2f Position;
	sf::Vector2f Size;
	sf::Vector2f Scale;
	float Rotation;

	Image(std::string image, float x, float y, float width, float height);
	~Image();
};

class Window
{
public:
	sf::RenderWindow MainWindow;
	sf::Vector2i Position;
	sf::Vector2u Size;

	std::map<std::string, Button*> ButtonMap;
	std::map<std::string, Image*> ImageMap;

	Window(std::string name, unsigned int width, unsigned int height);
	~Window();

	Button* CreateButton(std::string id, float x, float y, float width, float height, void(*f)(Window*, Button*) = NULL);
	Image* CreateImage(std::string id, std::string image, float x, float y, float width, float height);

	void CenterHorizontally();
	void CenterVertically();

	Button* GetButtonById(std::string id);
	bool DoesButtonExist(std::string id);
	bool DoesImageExist(std::string id);
	Image* GetImageById(std::string id);
	bool Render();
	void Close();
};