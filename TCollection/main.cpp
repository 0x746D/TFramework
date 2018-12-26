#include "GUIManager.h"
#include "TimingManager.h"
#include "ConsoleManager.h"
#include "Utilities.h"

bool ExitProgram = false;

void ExitButtonProc(Window* wn, Button* btn)
{
	ExitProgram = true;
}

void HeldProc(Window* wn, Button* btn)
{
	btn->Position = sf::Vector2f(sf::Mouse::getPosition(wn->MainWindow).x, sf::Mouse::getPosition(wn->MainWindow).y);
}

int main()
{
	Window* window = new Window("Hello!", 300, 325);
	window->CenterHorizontally();
	window->CenterVertically();

	window->CreateImage("test", "art1.jpeg", 150, 150, 250, 250)->SetZIndex(1);
	window->CreateButton("Backdrop", 150 + 5, 150 + 5, 250, 250)->SetBackgroundColor(CLR_BLACK)->SetButtonShape(ButtonEdgeShape::SquareEdge)->SetHighlightColor(CLR_BLACK)->SetZIndex(2);
	window->CreateButton("Play", 150, 295, 125, 25)->SetBackgroundColor(149, 165, 166)->SetButtonShape(ButtonEdgeShape::RoundEdge)->SetText("Play")->SetZIndex(0);
	window->CreateButton("Prev", 50, 295, 50, 25)->SetBackgroundColor(149, 165, 166)->SetButtonShape(ButtonEdgeShape::RoundEdge)->SetText("Prev")->SetZIndex(0);
	window->CreateButton("Next", 250, 295, 50, 25)->SetBackgroundColor(149, 165, 166)->SetButtonShape(ButtonEdgeShape::RoundEdge)->SetText("Next")->SetZIndex(0);
	window->CreateButton("Loop", 250 + (25 / 2), 275 - (25 / 2), 25, 25)->SetBackgroundColor(CLR_RED)->SetButtonShape(ButtonEdgeShape::RoundEdge)->SetText("O")->SetZIndex(0);
	window->CreateButton("Exit", 300 - 25, 25, 25, 25, &ExitButtonProc)->SetButtonShape(ButtonEdgeShape::RoundEdge)->SetZIndex(0);

	if (window->DoesButtonExist("Exit"))
	{
		window->GetButtonById("Exit")->SetBackgroundColor(CLR_RED);
		window->GetButtonById("Exit")->SetHighlightColor(CLR_DARK_RED);
		window->GetButtonById("Exit")->Text.setString("X");
	}

	for (int i = 0; i < window->DrawOrder.size(); i++)
	{
		std::cout << window->DrawOrder[i] << " ";
	}
	std::cout << std::endl;
	while (window->Render())
	{
		TimingManager::Update(window);
		if (ExitProgram)
		{
			window->Close();
		}
	}
	return 0;
}