#include "GUIManager.h"

Button::Button(float x, float y, float width, float height)
{
	Position = sf::Vector2f(x, y);
	Size = sf::Vector2f(width, height);
	BackgroundColor = sf::Color(39, 174, 96);
	HighlightColor = sf::Color(46, 204, 113);
	TextColor = sf::Color(255, 255, 255);
	Shape = sf::RectangleShape(Size);
	Font.loadFromFile("Resources/Fonts/Roboto-Regular.ttf");
	Shape.setPosition(Position);
	Shape.setFillColor(BackgroundColor);
	Enabled = true;
	Clicked = false;
	Text.setFont(Font);
	Text.setString("");
	Text.setCharacterSize(14);
	Text.setFillColor(sf::Color::White);
	Highlighted = false;
	TextLocation = TextLocation::Center;
	ClickHandler = NULL;
	HeldHandler = NULL;
	RightClickHandler = NULL;
	Held = false;
}

Button* Button::SetBackgroundColor(int r, int g, int b)
{
	BackgroundColor = sf::Color(r, g, b);
	return this;
}

Button* Button::SetText(std::string text)
{
	Text.setString(text);
	return this;
}

void Button::SetClickHandler(void(*f)(Window*, Button*))
{
	ClickHandler = f;
}

void Button::SetHeldHandler(void(*f)(Window*, Button*))
{
	HeldHandler = f;
}

void Button::SetRightClickHandler(void(*f)(Window*, Button*))
{
	RightClickHandler = f;
}

Button* Button::SetHighlightColor(int r, int g, int b)
{
	HighlightColor = sf::Color(r, g, b);
	return this;
}

Button* Button::SetTextColor(int r, int g, int b)
{
	TextColor = sf::Color(r, g, b);
	return this;
}

Button::~Button()
{

}
Image::Image(std::string image, float x, float y, float width, float height)
{
	std::string Res = "Resources/Images/";
	Texture.loadFromFile(Res + image);
	Texture.setSmooth(true);
	Sprite = sf::Sprite(Texture);
	Position = sf::Vector2f(x, y);
	Size = sf::Vector2f(width, height);
	Scale = sf::Vector2f(1.0f, 1.0f);
	Rotation = 0;
}

Image::~Image()
{

}

Window::Window(std::string name, unsigned int width, unsigned int height)
{
	Position = sf::Vector2i(0, 0);
	Size = sf::Vector2u(width, height);
	MainWindow.create(sf::VideoMode(width, height), name, sf::Style::None);
	MainWindow.setPosition(Position);
}

Window::~Window()
{

}

void Window::Close()
{
	if (MainWindow.isOpen())
	{
		MainWindow.close();
	}
}

bool Window::Render()
{
	if (MainWindow.isOpen())
	{
		sf::Event event;
		while (MainWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				MainWindow.close();
			}
			else if (event.type == sf::Event::Resized)
			{
				MainWindow.setView(sf::View(sf::FloatRect(0, 0, (float)event.size.width, (float)event.size.height)));
			}
			else if (event.type == sf::Event::MouseMoved)
			{
				std::map<std::string, Button*>::iterator it;
				for (it = ButtonMap.begin(); it != ButtonMap.end(); it++)
				{
					if (it->second != nullptr)
					{
						if (it->second->Enabled)
						{
							if (it->second->Shape.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y))))
							{
								std::cout << "Hovered!" << std::endl;
								it->second->Highlighted = true;
							}
							else
							{
								it->second->Highlighted = false;
							}
						}
					}
					else
					{
						ButtonMap.erase(it->first);
					}
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					std::map<std::string, Button*>::iterator it;
					for (it = ButtonMap.begin(); it != ButtonMap.end(); it++)
					{
						if (it->second != nullptr)
						{
							if (it->second->Enabled)
							{
								if (it->second->Shape.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))))
								{
									std::cout << "Clicked!" << std::endl;
									if (it->second->ClickHandler != NULL)
										it->second->ClickHandler(this, it->second);
									it->second->Clicked = false;
									it->second->Held = false;
								}
								else
								{
									it->second->Clicked = false;
									it->second->Held = false;
								}
							}
						}
						else
						{
							ButtonMap.erase(it->first);
						}
					}
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					std::map<std::string, Button*>::iterator it;
					for (it = ButtonMap.begin(); it != ButtonMap.end(); it++)
					{
						if (it->second != nullptr)
						{
							if (it->second->Enabled)
							{
								if (it->second->Shape.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))))
								{
									std::cout << "Right Clicked!" << std::endl;
									if (it->second->RightClickHandler != NULL)
										it->second->RightClickHandler(this, it->second);
									it->second->Clicked = false;
								}
								else
								{
									it->second->Clicked = false;
								}
							}
						}
						else
						{
							ButtonMap.erase(it->first);
						}
					}
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					std::map<std::string, Button*>::iterator it;
					for (it = ButtonMap.begin(); it != ButtonMap.end(); it++)
					{
						if (it->second != nullptr)
						{
							if (it->second->Enabled)
							{
								if (it->second->Shape.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))))
								{
									std::cout << "Held!" << std::endl;
									if (it->second->HeldHandler != NULL)
										it->second->HeldHandler(this, it->second);
									it->second->Held = true;
								}
								else
								{
									it->second->Held = false;
								}
							}
						}
						else
						{
							ButtonMap.erase(it->first);
						}
					}
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						std::map<std::string, Button*>::iterator it;
						for (it = ButtonMap.begin(); it != ButtonMap.end(); it++)
						{
							if (it->second != nullptr)
							{
								if (it->second->Enabled)
								{
									if (it->second->Shape.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))))
									{
										std::cout << "Held!" << std::endl;
										it->second->Clicked = true;
									}
									else
									{
										it->second->Clicked = false;
									}
								}
							}
							else
							{
								ButtonMap.erase(it->first);
							}
						}
					}
				}
			}
		}

		MainWindow.clear();

		std::map<std::string, Button*>::iterator it;
		for (it = ButtonMap.begin(); it != ButtonMap.end(); it++)
		{
			if (it->second != nullptr)
			{
				if (it->second->Enabled)
				{
					if (it->second->Highlighted)
						it->second->Shape.setFillColor(it->second->HighlightColor);
					else
						it->second->Shape.setFillColor(it->second->BackgroundColor);
					if (it->second->Held)
					{
						if (it->second->HeldHandler != NULL)
							it->second->HeldHandler(this, it->second);
					}
					it->second->Shape.setPosition(it->second->Position);
					it->second->Shape.setSize(it->second->Size);
					if (it->second->TextLocation == TextLocation::Center)
					{
						sf::Vector2f pos = it->second->Position;
						pos.x += it->second->Size.x / 2;
						//pos.x -= it->second->Text.getLocalBounds().width / 2;
						pos.y += it->second->Size.y / 2;
						pos.y -= it->second->Text.getLocalBounds().height / 4;
						it->second->Text.setOrigin(roundf(it->second->Text.getLocalBounds().width / 2), roundf(it->second->Text.getLocalBounds().height / 2));
						it->second->Text.setPosition(roundf(pos.x), roundf(pos.y));
						it->second->Text.setFillColor(it->second->TextColor);
					}
					MainWindow.draw(it->second->Shape);
					MainWindow.draw(it->second->Text);
				}
			}
			else
			{
				ButtonMap.erase(it->first);
			}
		}
		std::map<std::string, Image*>::iterator it2;
		for (it2 = ImageMap.begin(); it2 != ImageMap.end(); it2++)
		{
			if (it2->second != nullptr)
			{
				sf::Vector2u curSize = it2->second->Texture.getSize();
				it2->second->Sprite.setScale(it2->second->Size.x / curSize.x, it2->second->Size.y / curSize.y);
				it2->second->Sprite.setPosition(it2->second->Position);
				it2->second->Sprite.setRotation(it2->second->Rotation);
				MainWindow.draw(it2->second->Sprite);
			}
		}
		MainWindow.setPosition(Position);
		MainWindow.setSize(Size);
		MainWindow.display();
	}

	return MainWindow.isOpen();
}

Button* Window::GetButtonById(std::string id)
{
	std::map<std::string, Button*>::iterator it = ButtonMap.find(id);
	if (it != ButtonMap.end())
		return it->second;
	return nullptr;
}

bool Window::DoesButtonExist(std::string id)
{
	std::map<std::string, Button*>::iterator it = ButtonMap.find(id);
	if (it != ButtonMap.end())
		return true;
	return false;
}

Image* Window::GetImageById(std::string id)
{
	std::map<std::string, Image*>::iterator it = ImageMap.find(id);
	if (it != ImageMap.end())
		return it->second;
	return nullptr;
}

bool Window::DoesImageExist(std::string id)
{
	std::map<std::string, Image*>::iterator it = ImageMap.find(id);
	if (it != ImageMap.end())
		return true;
	return false;
}

Button* Window::CreateButton(std::string id, float x, float y, float width, float height, void(*f)(Window*, Button*))
{
	if (ButtonMap.find(id) == ButtonMap.end())
	{
		Button* button = new Button(x, y, width, height);
		button->ClickHandler = f;
		ButtonMap.insert(std::pair<std::string, Button*>(id, button));
		return button;
	}
	return ButtonMap.find(id)->second;
}

Image* Window::CreateImage(std::string id, std::string image, float x, float y, float width, float height)
{
	if (ImageMap.find(id) == ImageMap.end())
	{
		Image* img = new Image(image, x, y, width, height);
		ImageMap.insert(std::pair<std::string, Image*>(id, img));
		return img;
	}
	return ImageMap.find(id)->second;
}

void Window::CenterHorizontally()
{
	Position.x = sf::VideoMode::getDesktopMode().width / 2 - Size.x / 2;
}

void Window::CenterVertically()
{
	Position.y = sf::VideoMode::getDesktopMode().height / 2 - Size.y / 2;
}