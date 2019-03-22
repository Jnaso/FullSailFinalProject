#include "UIManager.h"

UI::UIManager::UIManager(Graphics * graphics, InputManager * input)
{
	myGraphics = graphics;
	myInput = input;
}

void UI::UIManager::CreateImage(RECT dimensions, bool interact, const char * filePath, float2 pos)
{
	UIComponent temp(myGraphics, dimensions, interact, filePath, pos);
	UIElements.push_back(temp);
}

void UI::UIManager::CreateText(const char* text, bool interact, int font, float2 pos)
{
	UIComponent temp(myGraphics, interact, font, text, pos);
	UIElements.push_back(temp);
}

UI::UIManager::~UIManager()
{

}
