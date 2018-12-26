#pragma once

#include <string>
#include <map>
#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>

enum class TextLocation
{
	Center,
	TopLeft,
	TopRight,
	Left,
	Right,
	BottomLeft,
	BottomRight
};

enum class LocationAxis
{
	TopLeft,
	Center
};

enum class ButtonEdgeShape
{
	SquareEdge,
	RoundEdge,
};


class Object
{
public:
	virtual ~Object();
	//Place holder
};

class Button;
class Window;
class Image;

class Button : public Object
{
public:
	std::string Id;
	Window* ParentWindow;
	sf::RectangleShape Shape;

	/*For round edge*/
	sf::RectangleShape Rect1;
	sf::RectangleShape Rect2;
	sf::CircleShape Cir1;
	sf::CircleShape Cir2;
	sf::CircleShape Cir3;
	sf::CircleShape Cir4;

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
	LocationAxis Axis;
	ButtonEdgeShape ButtonShape;
	int ZIndex;
	Button* SetBackgroundColor(int r, int g, int b);
	Button* SetHighlightColor(int r, int g, int b);
	Button* SetTextColor(int r, int g, int b);
	Button* SetText(std::string text);
	Button* SetLocationAxis(LocationAxis axis);
	Button* SetButtonShape(ButtonEdgeShape);

	Button(float x, float y, float width, float height, Window*, std::string);
	~Button();

	void(*ClickHandler)(Window*, Button*);
	void(*HeldHandler)(Window*, Button*);
	void(*RightClickHandler)(Window*, Button*);

	void SetClickHandler(void(*f)(Window*, Button*));
	void SetHeldHandler(void(*f)(Window*, Button*));
	void SetRightClickHandler(void(*f)(Window*, Button*));

	Button* SetZIndex(int);
};

class Image : public Object
{
public:
	std::string Id;
	Window* ParentWindow;
	sf::Sprite Sprite;
	sf::Texture Texture;
	sf::Vector2f Position;
	sf::Vector2f Size;
	sf::Vector2f Scale;
	float Rotation;
	int ZIndex;
	LocationAxis Axis;

	Image* SetLocationAxis(LocationAxis axis);
	Image* SetZIndex(int);
	Image(std::string image, float x, float y, float width, float height, Window*, std::string);
	~Image();
};


class Window
{
public:
	sf::RenderWindow MainWindow;
	sf::Vector2i Position;
	sf::Vector2u Size;

	std::map<std::string, Object*> ObjectMap;
	std::vector<std::string> DrawOrder;

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