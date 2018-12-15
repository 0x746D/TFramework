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
	Window* window = new Window("Hello!", 500, 500);
	window->CenterHorizontally();
	window->CenterVertically();

	window->CreateImage("test", "test.jpeg", 125, 125, 250, 250);

	window->CreateButton("PlayPause", 225, 250 + 125, 50, 50)->SetBackgroundColor(149, 165, 166)->SetText("Play");
	window->CreateButton("Next", 275, 250 + 125, 50, 50)->SetBackgroundColor(149, 165, 166)->SetText("Next");
	window->CreateButton("Previous", 175, 250 + 125, 50, 50)->SetBackgroundColor(149, 165, 166)->SetText("Prev");
	window->CreateButton("Exit", 475, 0, 25, 25, &ExitButtonProc);

	if (window->DoesButtonExist("Exit"))
	{
		window->GetButtonById("Exit")->SetBackgroundColor(CLR_RED);
		window->GetButtonById("Exit")->SetHighlightColor(CLR_DARK_RED);
		window->GetButtonById("Exit")->Text.setString("X");
	}

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