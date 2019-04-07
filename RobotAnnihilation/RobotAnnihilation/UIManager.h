#pragma once

#ifndef _UIMANAGER_H_
#define _UIMANAGER_H_

#include "InputManager.h"
#include "UIElement.h"


struct UIType { enum TYPE { IMAGE, TEXT /*Add types if so needed*/ }; };

class UIManager
{
private:
	InputManager* m_Input;
	ID3D11Device* m_Device;
	ID3D11DeviceContext* m_context;

	//Vector of Ui Elements to render
	std::vector<UIElement*> m_UIElements;

	unsigned long textCount = 0;
	unsigned long imageCount = 0;
	unsigned long buttonCount = 0;

public:
	
	bool m_mainMenu = true;
	bool m_pauseMenu = false;

	UIManager(InputManager* input, ID3D11Device* device, ID3D11DeviceContext* context);

	void Render(std::unique_ptr<DirectX::SpriteBatch>& batch, std::unique_ptr<DirectX::SpriteFont>& arial, std::unique_ptr<DirectX::SpriteFont>& comicSans);

	void Update();

	UIElement* CreateText(RECT srcRect, bool interactable, bool enabled, float2 pos, int font, const char* text);
	UIElement* CreateText(RECT srcRect, bool interactable, bool enabled, float2 pos, int font, std::string text);
	//Keep All Images within powers of 2(*FIX*)
	UIElement* CreateImage(RECT srcRect, bool interactable, bool enabled, float2 pos, const char * filePath, ID3D11Device* device);

	UIElement* CreateButton(RECT srcRect, bool interactable, bool enabled, float2 pos, ID3D11Device* device, int font = F_ARIAL, const char* text = "");

	void DestroyUIElement(UIElement* item, int index);
	
	void HideMainMenu();

	std::vector<UIElement*> GetUIElements(); 

	~UIManager();

	
};

#endif