#include "GUIManager.h"
#include "ConsoleManager.h"
#include "Utilities.h"

Button::Button(float x, float y, float width, float height, Window* parent, std::string id)
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
	ClickHandler = nullptr;
	HeldHandler = nullptr;
	RightClickHandler = nullptr;
	Held = false;
	Axis = LocationAxis::Center;
	ButtonShape = ButtonEdgeShape::SquareEdge;
	ZIndex = 0;
	ParentWindow = parent;
	Id = id;
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

Button* Button::SetButtonShape(ButtonEdgeShape shape)
{
	ButtonShape = shape;
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

Button* Button::SetLocationAxis(LocationAxis axis)
{
	Axis = axis;
	return this;
}

Button* Button::SetZIndex(int index)
{
	ZIndex = index;
	std::vector<std::string>::iterator position = std::find(ParentWindow->DrawOrder.begin(), ParentWindow->DrawOrder.end(), Id);
	if (position != ParentWindow->DrawOrder.end())
		ParentWindow->DrawOrder.erase(position);

	if (ParentWindow->DrawOrder.size() == 0)
	{
		ParentWindow->DrawOrder.push_back(Id);
		return this;
	}
	else
	{
		for (unsigned int i = 0; i < ParentWindow->DrawOrder.size(); i++)
		{
			if (i == ParentWindow->DrawOrder.size() - 1)
			{
				Button* btn = dynamic_cast<Button*>(ParentWindow->ObjectMap[ParentWindow->DrawOrder[i]]);
				if (btn != nullptr)
				{
					if (btn->ZIndex > index)
					{
						ParentWindow->DrawOrder.insert(ParentWindow->DrawOrder.begin(), Id);
						return this;
					}
					else
					{
						ParentWindow->DrawOrder.push_back(Id);
						return this;
					}
				}
				else
				{
					Image* img = dynamic_cast<Image*>(ParentWindow->ObjectMap[ParentWindow->DrawOrder[i]]);
					if (img != nullptr)
					{
						if (img->ZIndex > index)
						{
							ParentWindow->DrawOrder.insert(ParentWindow->DrawOrder.begin(), Id);
							return this;
						}
						else
						{
							ParentWindow->DrawOrder.push_back(Id);
							return this;
						}
					}
				}
				return this;
			}
			else
			{
				Button* btn = dynamic_cast<Button*>(ParentWindow->ObjectMap[ParentWindow->DrawOrder[i]]);
				if (btn != nullptr)
				{
					if (btn->ZIndex <= index)
					{
						Button* btn2 = dynamic_cast<Button*>(ParentWindow->ObjectMap[ParentWindow->DrawOrder[i + 1]]);
						if (btn2 != nullptr)
						{
							if (btn2->ZIndex > index)
							{
								ParentWindow->DrawOrder.insert(ParentWindow->DrawOrder.begin() + i, Id);
								return this;
							}
						}
						else
						{
							Image* img2 = dynamic_cast<Image*>(ParentWindow->ObjectMap[ParentWindow->DrawOrder[i + 1]]);
							if (img2 != nullptr)
							{
								if (img2->ZIndex > index)
								{
									ParentWindow->DrawOrder.insert(ParentWindow->DrawOrder.begin() + i, Id);
									return this;
								}
							}
						}
					}
					else
					{
						ParentWindow->DrawOrder.insert(ParentWindow->DrawOrder.begin() + i, Id);
						return this;
					}
				}
				else
				{
					Image* img = dynamic_cast<Image*>(ParentWindow->ObjectMap[ParentWindow->DrawOrder[i]]);
					if (img != nullptr)
					{
						if (img->ZIndex <= index)
						{
							Button* btn2 = dynamic_cast<Button*>(ParentWindow->ObjectMap[ParentWindow->DrawOrder[i + 1]]);
							if (btn2 != nullptr)
							{
								if (btn2->ZIndex > index)
								{
									ParentWindow->DrawOrder.insert(ParentWindow->DrawOrder.begin() + i, Id);
									return this;
								}
							}
							else
							{
								Image* img2 = dynamic_cast<Image*>(ParentWindow->ObjectMap[ParentWindow->DrawOrder[i + 1]]);
								if (img2 != nullptr)
								{
									if (img2->ZIndex > index)
									{
										ParentWindow->DrawOrder.insert(ParentWindow->DrawOrder.begin() + i, Id);
										return this;
									}
								}
							}
						}
						else
						{
							ParentWindow->DrawOrder.insert(ParentWindow->DrawOrder.begin() + i, Id);
							return this;
						}
					}
				}
			}
		}
	}

	return this;
}

Button::~Button()
{

}
Image::Image(std::string image, float x, float y, float width, float height, Window* parent, std::string id)
{
	std::string Res = "Resources/Images/";
	Texture.loadFromFile(Res + image);
	Texture.setSmooth(true);
	Sprite = sf::Sprite(Texture);
	Position = sf::Vector2f(x, y);
	Size = sf::Vector2f(width, height);
	Scale = sf::Vector2f(1.0f, 1.0f);
	Rotation = 0;
	Axis = LocationAxis::Center;
	ZIndex = 0;
	Id = id;
	ParentWindow = parent;
}

Image::~Image()
{

}

Image* Image::SetLocationAxis(LocationAxis axis)
{
	Axis = axis;
	return this;
}

Image* Image::SetZIndex(int index)
{
	ZIndex = index;
	
	std::vector<std::string>::iterator position = std::find(ParentWindow->DrawOrder.begin(), ParentWindow->DrawOrder.end(), Id);
	if (position != ParentWindow->DrawOrder.end())
		ParentWindow->DrawOrder.erase(position);

	if (ParentWindow->DrawOrder.size() == 0)
	{
		ParentWindow->DrawOrder.push_back(Id);
		return this;
	}
	else
	{
		for (unsigned int i = 0; i < ParentWindow->DrawOrder.size(); i++)
		{
			if (i == ParentWindow->DrawOrder.size() - 1)
			{
				ParentWindow->DrawOrder.push_back(Id);
				return this;
			}
			else
			{
				Button* btn = dynamic_cast<Button*>(ParentWindow->ObjectMap[ParentWindow->DrawOrder[i]]);
				if (btn != nullptr)
				{
					if (btn->ZIndex <= index)
					{
						Button* btn2 = dynamic_cast<Button*>(ParentWindow->ObjectMap[ParentWindow->DrawOrder[i + 1]]);
						if (btn2 != nullptr)
						{
							if (btn2->ZIndex > index)
							{
								ParentWindow->DrawOrder.insert(ParentWindow->DrawOrder.begin() + i, Id);
								return this;
							}
						}
						else
						{
							Image* img2 = dynamic_cast<Image*>(ParentWindow->ObjectMap[ParentWindow->DrawOrder[i + 1]]);
							if (img2 != nullptr)
							{
								if (img2->ZIndex > index)
								{
									ParentWindow->DrawOrder.insert(ParentWindow->DrawOrder.begin() + i, Id);
									return this;
								}
							}
						}
					}
					else
					{
						ParentWindow->DrawOrder.insert(ParentWindow->DrawOrder.begin() + i, Id);
						return this;
					}
				}
				else
				{
					Image* img = dynamic_cast<Image*>(ParentWindow->ObjectMap[ParentWindow->DrawOrder[i]]);
					if (img != nullptr)
					{
						if (img->ZIndex <= index)
						{
							Button* btn2 = dynamic_cast<Button*>(ParentWindow->ObjectMap[ParentWindow->DrawOrder[i + 1]]);
							if (btn2 != nullptr)
							{
								if (btn2->ZIndex > index)
								{
									ParentWindow->DrawOrder.insert(ParentWindow->DrawOrder.begin() + i, Id);
									return this;
								}
							}
							else
							{
								Image* img2 = dynamic_cast<Image*>(ParentWindow->ObjectMap[ParentWindow->DrawOrder[i + 1]]);
								if (img2 != nullptr)
								{
									if (img2->ZIndex > index)
									{
										ParentWindow->DrawOrder.insert(ParentWindow->DrawOrder.begin() + i, Id);
										return this;
									}
								}
							}
						}
						else
						{
							ParentWindow->DrawOrder.insert(ParentWindow->DrawOrder.begin() + i, Id);
							return this;
						}
					}
				}
			}
		}
	}

	return this;
}

Object::~Object()
{

}

Window::Window(std::string name, unsigned int width, unsigned int height)
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;
	Position = sf::Vector2i(0, 0);
	Size = sf::Vector2u(width, height);
	MainWindow.create(sf::VideoMode(width, height), name, sf::Style::None, settings);
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

			}
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				std::map<std::string, Object*>::iterator it;
				Button* btnPress = nullptr;
				Image* img = nullptr;

				for (it = ObjectMap.begin(); it != ObjectMap.end(); it++)
				{
					Button* btn = dynamic_cast<Button*>(it->second);
					if (btn != nullptr)
					{
						if (btn->ButtonShape == ButtonEdgeShape::SquareEdge)
						{
							if (btn->Shape.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))))
							{
								if (btnPress == nullptr)
								{
									btnPress = btn;
									std::cout << "Held!" << std::endl;
								}
								if (btn->ZIndex < btnPress->ZIndex)
								{
									std::cout << "Held!" << std::endl;
									btnPress = btn;
								}
							}
							btn->Held = false;
						}
						else if (btn->ButtonShape == ButtonEdgeShape::RoundEdge)
						{
							if (btn->Rect1.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) ||
								btn->Rect2.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) ||
								btn->Cir1.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) ||
								btn->Cir2.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) ||
								btn->Cir3.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) ||
								btn->Cir4.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))))
							{
								if (btnPress == nullptr)
								{
									btnPress = btn;
									std::cout << "Held!" << std::endl;
								}
								if (btn->ZIndex < btnPress->ZIndex)
								{
									std::cout << "Held!" << std::endl;
									btnPress = btn;
								}
							}
							btn->Held = false;
						}
					}
					else
					{
						img = dynamic_cast<Image*>(it->second);
						if (img != nullptr)
						{

						}
						else
						{

						}
					}
				}

				if (event.mouseButton.button == sf::Mouse::Left)
				{
					if (btnPress != nullptr)
					{
						if (btnPress->HeldHandler != nullptr)
							btnPress->HeldHandler(this, btnPress);
						btnPress->Held = true;
						Console::WriteLine(S("Left Press: ") + btnPress->Id);
					}
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					Console::WriteLine("Right Press");
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased)
			{
				std::map<std::string, Object*>::iterator it;
				Button* btnRel = nullptr;
				Image* img = nullptr;

				for (it = ObjectMap.begin(); it != ObjectMap.end(); it++)
				{
					Button* btn = dynamic_cast<Button*>(it->second);
					if (btn != nullptr)
					{
						if (btn->ButtonShape == ButtonEdgeShape::SquareEdge)
						{
							if (btn->Shape.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))))
							{
								if (btnRel == nullptr)
									btnRel = btn;
								if (btn->ZIndex < btnRel->ZIndex)
								{
									btnRel = btn;
								}
							}
							else
							{
								btn->Held = false;
							}
						}
						else if (btn->ButtonShape == ButtonEdgeShape::RoundEdge)
						{
							if (btn->Rect1.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) ||
								btn->Rect2.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) ||
								btn->Cir1.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) ||
								btn->Cir2.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) ||
								btn->Cir3.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) ||
								btn->Cir4.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))))
							{
								if (btnRel == nullptr)
									btnRel = btn;
								if (btn->ZIndex < btnRel->ZIndex)
								{
									btnRel = btn;
								}
							}
							else
							{
								btn->Held = false;
							}
						}
					}
					else
					{
						img = dynamic_cast<Image*>(it->second);
						if (img != nullptr)
						{

						}
						else
						{

						}
					}
				}

				if (event.mouseButton.button == sf::Mouse::Left)
				{
					if (btnRel != nullptr)
					{
						if (btnRel->Held)
						{
							std::cout << "Clicked!" << std::endl;
							if (btnRel->ClickHandler != nullptr)
								btnRel->ClickHandler(this, btnRel);
							btnRel->Held = false;
						}
						Console::WriteLine(S("Left Release: ") + btnRel->Id);
					}
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					Console::WriteLine("Right Release");
				}
			}
			else if (event.type == sf::Event::LostFocus)
			{
				std::cout << "Lost Focus" << std::endl;
			}
			else if (event.type == sf::Event::GainedFocus)
			{
				std::cout << "Gained Focus" << std::endl;
			}
		}
		/*while (MainWindow.pollEvent(event))
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
							if (it->second->ButtonShape == ButtonEdgeShape::SquareEdge)
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
							else if (it->second->ButtonShape == ButtonEdgeShape::RoundEdge)
							{
								if (it->second->Rect1.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y))) ||
									it->second->Rect2.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y))) ||
									it->second->Cir1.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y))) ||
									it->second->Cir2.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y))) ||
									it->second->Cir3.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y))) || 
									it->second->Cir4.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y))))
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
								if (it->second->ButtonShape == ButtonEdgeShape::SquareEdge)
								{
									if (it->second->Shape.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))))
									{
										if (it->second->Held)
										{
											std::cout << "Clicked!" << std::endl;
											if (it->second->ClickHandler != NULL)
												it->second->ClickHandler(this, it->second);
										}
										it->second->Clicked = false;
										it->second->Held = false;
									}
									else
									{
										it->second->Clicked = false;
										it->second->Held = false;
									}
								}
								else if (it->second->ButtonShape == ButtonEdgeShape::RoundEdge)
								{
									if (it->second->Rect1.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) ||
										it->second->Rect2.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) ||
										it->second->Cir1.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) ||
										it->second->Cir2.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) ||
										it->second->Cir3.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) ||
										it->second->Cir4.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))))
									{
										if (it->second->Held)
										{
											std::cout << "Clicked!" << std::endl;
											if (it->second->ClickHandler != NULL)
												it->second->ClickHandler(this, it->second);
										}
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
								if (it->second->ButtonShape == ButtonEdgeShape::SquareEdge)
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
								else if (it->second->ButtonShape == ButtonEdgeShape::RoundEdge)
								{
									if (it->second->Rect1.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) ||
										it->second->Rect2.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) ||
										it->second->Cir1.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) ||
										it->second->Cir2.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) ||
										it->second->Cir3.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) ||
										it->second->Cir4.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))))
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
									if (it->second->ButtonShape == ButtonEdgeShape::SquareEdge)
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
									else if (it->second->ButtonShape == ButtonEdgeShape::RoundEdge)
									{
										if (it->second->Rect1.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) ||
											it->second->Rect2.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) ||
											it->second->Cir1.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) ||
											it->second->Cir2.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) ||
											it->second->Cir3.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) ||
											it->second->Cir4.getGlobalBounds().contains(MainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))))
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
		*/
		MainWindow.clear(sf::Color(44, 62, 80));

		for (auto it = DrawOrder.rbegin(); it != DrawOrder.rend(); ++it)
		{
			Object* mapObj = ObjectMap[*it];
			if (mapObj != nullptr)
			{
				Button* obj = dynamic_cast<Button*>(mapObj);
				if (obj)
				{
					if (obj->Enabled)
					{
						if (obj->Highlighted)
						{
							obj->Shape.setFillColor(obj->HighlightColor);
							obj->Rect1.setFillColor(obj->HighlightColor);
							obj->Rect2.setFillColor(obj->HighlightColor);
							obj->Cir1.setFillColor(obj->HighlightColor);
							obj->Cir2.setFillColor(obj->HighlightColor);
							obj->Cir3.setFillColor(obj->HighlightColor);
							obj->Cir4.setFillColor(obj->HighlightColor);
						}
						else
						{
							obj->Shape.setFillColor(obj->BackgroundColor);
							obj->Rect1.setFillColor(obj->BackgroundColor);
							obj->Rect2.setFillColor(obj->BackgroundColor);
							obj->Cir1.setFillColor(obj->BackgroundColor);
							obj->Cir2.setFillColor(obj->BackgroundColor);
							obj->Cir3.setFillColor(obj->BackgroundColor);
							obj->Cir4.setFillColor(obj->BackgroundColor);
						}
						if (obj->Held)
						{
							if (obj->HeldHandler != nullptr)
								obj->HeldHandler(this, obj);
						}
						sf::Vector2f ff = obj->Position;
						if (obj->Axis == LocationAxis::Center)
						{
							ff.x -= obj->Size.x / 2;
							ff.y -= obj->Size.y / 2;
						}
						/*Needs to be tested more thoroughly*/
						ff.x = ceilf(ff.x);
						ff.y = floorf(ff.y);
						/*----------------------------------*/
						obj->Shape.setPosition(ff);
						obj->Shape.setSize(obj->Size);
						if (obj->TextLocation == TextLocation::Center)
						{
							sf::Vector2f pos = obj->Position;
							if (obj->Axis == LocationAxis::TopLeft)
							{
								pos.x -= obj->Size.x / 2;
								pos.y -= obj->Size.y / 2;
							}
							pos.y -= obj->Text.getLocalBounds().height / 4;
							obj->Text.setOrigin(roundf(obj->Text.getLocalBounds().width / 2), roundf(obj->Text.getLocalBounds().height / 2));
							obj->Text.setPosition(roundf(pos.x), roundf(pos.y));
							obj->Text.setFillColor(obj->TextColor);
						}

						if (obj->ButtonShape == ButtonEdgeShape::SquareEdge)
						{
							MainWindow.draw(obj->Shape);
						}
						else if (obj->ButtonShape == ButtonEdgeShape::RoundEdge)
						{
							sf::Vector2f pos2 = ff;
							//sf::RectangleShape sf1;
							//obj->Rect1.setFillColor(sf::Color::Red);
							obj->Rect1.setSize(sf::Vector2f(obj->Size.x - 20.0f, obj->Size.y));
							pos2 = ff;
							pos2.x += 10.0f;
							obj->Rect1.setPosition(pos2);
							MainWindow.draw(obj->Rect1);

							//sf::RectangleShape sf2;
							//obj->Rect2.setFillColor(sf::Color::Red);
							obj->Rect2.setSize(sf::Vector2f(obj->Size.x, obj->Size.y - 20.0f));
							pos2 = ff;
							pos2.y += 10.0f;
							obj->Rect2.setPosition(pos2);
							MainWindow.draw(obj->Rect2);

							//sf::CircleShape cir1;
							obj->Cir1.setRadius(10.0f);
							//obj->Cir1.setFillColor(sf::Color::Red);
							//sf::CircleShape cir2;
							obj->Cir2.setRadius(10.0f);
							//obj->Cir2.setFillColor(sf::Color::Red);
							//sf::CircleShape cir3;
							obj->Cir3.setRadius(10.0f);
							//obj->Cir3.setFillColor(sf::Color::Red);
							//sf::CircleShape cir4;
							obj->Cir4.setRadius(10.0f);
							//obj->Cir4.setFillColor(sf::Color::Red);


							pos2 = ff;
							//pos2.x -= 10.0f;
							//pos2.y -= 10.0f;
							obj->Cir1.setPosition(pos2);
							MainWindow.draw(obj->Cir1);

							pos2 = ff;
							pos2.x += obj->Size.x - 20.0f;
							//pos2.y -= 10.0f;
							obj->Cir2.setPosition(pos2);
							MainWindow.draw(obj->Cir2);

							pos2 = ff;
							pos2.x += obj->Size.x - 20.0f;
							pos2.y += obj->Size.y - 20.0f;
							obj->Cir3.setPosition(pos2);
							MainWindow.draw(obj->Cir3);

							pos2 = ff;
							//pos2.x -= 10.0f;
							pos2.y += obj->Size.y - 20.0f;
							obj->Cir4.setPosition(pos2);
							MainWindow.draw(obj->Cir4);
						}

						MainWindow.draw(obj->Text);
					}
				}
				else
				{
					Image* img = dynamic_cast<Image*>(mapObj);
					if (img)
					{
						sf::Vector2u curSize = img->Texture.getSize();
						img->Sprite.setScale(img->Size.x / curSize.x, img->Size.y / curSize.y);
						sf::Vector2f pos = img->Position;
						if (img->Axis == LocationAxis::Center)
						{
							pos.x -= img->Size.x / 2;
							pos.y -= img->Size.y / 2;
						}
						img->Sprite.setPosition(pos);
						img->Sprite.setRotation(img->Rotation);
						MainWindow.draw(img->Sprite);
					}
				}
			}
			else
			{
				ObjectMap.erase(*it);
				std::vector<std::string>::iterator pos = std::find(DrawOrder.begin(), DrawOrder.end(), *it);
				if (pos != DrawOrder.end())
				{
					DrawOrder.erase(pos);
				}
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
	std::map<std::string, Object*>::iterator it = ObjectMap.find(id);
	if (it != ObjectMap.end() && dynamic_cast<Button*>(it->second))
		return dynamic_cast<Button*>(it->second);
	return nullptr;
}

bool Window::DoesButtonExist(std::string id)
{
	std::map<std::string, Object*>::iterator it = ObjectMap.find(id);
	if (it != ObjectMap.end() && dynamic_cast<Button*>(it->second))
		return true;
	return false;
}

Image* Window::GetImageById(std::string id)
{
	std::map<std::string, Object*>::iterator it = ObjectMap.find(id);
	if (it != ObjectMap.end() && dynamic_cast<Image*>(it->second))
		return dynamic_cast<Image*>(it->second);
	return nullptr;
}

bool Window::DoesImageExist(std::string id)
{
	std::map<std::string, Object*>::iterator it = ObjectMap.find(id);
	if (it != ObjectMap.end() && dynamic_cast<Image*>(it->second))
		return true;
	return false;
}

Button* Window::CreateButton(std::string id, float x, float y, float width, float height, void(*f)(Window*, Button*))
{
	if (ObjectMap.find(id) == ObjectMap.end())
	{
		Button* button = new Button(x, y, width, height, this, id);
		button->ClickHandler = f;
		ObjectMap.insert(ObjectMap.begin(), std::pair<std::string, Button*>(id, button));
		DrawOrder.insert(DrawOrder.begin(), id);
		return button;
	}
	Object* obj = ObjectMap.find(id)->second;
	Button* btn = dynamic_cast<Button*>(obj);
	if (btn != nullptr)
		return btn;
	else
		return nullptr;
}

Image* Window::CreateImage(std::string id, std::string image, float x, float y, float width, float height)
{
	if (ObjectMap.find(id) == ObjectMap.end())
	{
		Image* img = new Image(image, x, y, width, height, this, id);
		ObjectMap.insert(ObjectMap.begin(), std::pair<std::string, Image*>(id, img));
		DrawOrder.insert(DrawOrder.begin(), id);
		return img;
	}
	Object* obj = ObjectMap.find(id)->second;
	Image* im = dynamic_cast<Image*>(obj);
	if (im != nullptr)
		return im;
	else
		return nullptr;
}

void Window::CenterHorizontally()
{
	Position.x = sf::VideoMode::getDesktopMode().width / 2 - Size.x / 2;
}

void Window::CenterVertically()
{
	Position.y = sf::VideoMode::getDesktopMode().height / 2 - Size.y / 2;
}