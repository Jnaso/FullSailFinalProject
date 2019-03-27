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

public:
	
	UIManager(InputManager* input, ID3D11Device* device, ID3D11DeviceContext* context);

	void Render(std::unique_ptr<DirectX::SpriteBatch>& batch, std::unique_ptr<DirectX::SpriteFont>& arial, std::unique_ptr<DirectX::SpriteFont>& comicSans);

	void Update();

	UIElement* CreateText(RECT srcRect, bool interactable, bool enabled, float2 pos, int font, const char* text, void(*MouseOver)()=nullptr, void(*Click)()=nullptr);
	//Keep All Images within powers of 2(*FIX*)
	UIElement* CreateImage(RECT srcRect, bool interactable, bool enabled, float2 pos, const char * filePath, ID3D11Device* device, void(*MouseOver)()=nullptr, void(*Click)()=nullptr);

	void DestroyUIElement(UIElement* item, int index);
	
	void HideMainMenu();

	~UIManager();

	std::vector<UIElement*> GetUIElements() { return m_UIElements; }
};

#endif