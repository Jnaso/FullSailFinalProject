#include "UIManager.h"

UI::UIManager::UIManager(Graphics * graphics, InputManager * input)
{
	myGraphics = graphics;
	myInput = input;
}

void UI::UIManager::CreateImage(RECT dimensions, bool interact, const char * filePath, UI::UIType::TYPE dataType, float2 pos)
{
	UIComponent temp(myGraphics, dimensions, interact, filePath, UI::UIType::IMAGE, pos);
	UIElements.push_back(temp);
}


UI::UIManager::~UIManager()
{

}
