#pragma once

#ifndef _ELEMENT_H_
#define _ELEMENT_H_

#define F_ARIAL 0
#define F_COMICSANS 1

#include "GeneralIncludes.h"
#include "InputManager.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>


class UIElement
{
protected:
	RECT m_destRect;
	bool m_interactable;
	bool m_mouseOver;
	bool m_enabled;

public:
	std::function<void()> m_OnMouseEnter = nullptr;
	std::function<void()> m_OnMouseClick = nullptr;

	DirectX::SimpleMath::Vector2 m_pos;

public:
	UIElement(RECT srcRect, bool interactable, bool enabled, float2 pos);
	~UIElement();

	#pragma region Acessors_And_Mutators
	
	RECT* GetSrcRect() 
	{
		RECT* temp = &m_destRect;
		return temp; 
	}

	bool GetInteractable() { return m_interactable; }
	void SetInteractable(bool value) { m_interactable = value; }
	
	bool GetEnabled() { return m_enabled; }
	void SetEnabled(bool value) { m_enabled = value; }
	
	bool GetMouseOver() { return m_mouseOver; }
	void SetMouseOver(bool value) { m_mouseOver = value; }

	float2 GetPos() 
	{
		float2 temp = { m_pos.x, m_pos.y };
		return temp;
	}
	void SetPos(float2 value) 
	{ 
		m_destRect.left = value.x;
		m_destRect.top = value.y;
		m_destRect.right += value.x;
		m_destRect.bottom += value.y;
		m_pos = { value.x, value.y };
	}
	
	void SetSize(float2 value) 
	{ 
		m_destRect = RECT
		{ 
			static_cast<LONG>(m_pos.x), 
			static_cast<LONG>(m_pos.y),
			static_cast<LONG>(m_pos.x + value.x),
			static_cast<LONG>(m_pos.y + value.y)
		}; 
	}
	float2 GetSize() 
	{ 
		return float2
		{ 
			static_cast<float>(m_destRect.right - m_destRect.left),
			static_cast<float>(m_destRect.bottom - m_destRect.top) 
		}; 
	}

	#pragma endregion


	//Virtual Function
	virtual void Update(/*Going to be a function pointer passed into the constructor and eventually a mesage manager*/) {}
	virtual void Render() {}
};

class TextElement : public UIElement
{
private:
	int m_font;
	const char* m_text;

public:
	TextElement(RECT srcRect, bool interactable, bool enabled, float2 pos, int font, const char* text);

	void Update();
	void Render(std::unique_ptr<DirectX::SpriteBatch>& batch, std::unique_ptr<DirectX::SpriteFont>& font);

	int GetFont() { return m_font; }
	void SetFont(int value) { m_font = value; }

	const char* GetText() { return m_text; }
	void SetText(const char* value) { m_text = value; }

};

class ImageElement : public UIElement
{
private:
	ID3D11ShaderResourceView* m_texture;

public:
	ImageElement(RECT srcRect, bool interactable, bool enabled, float2 pos, const char* filePath, ID3D11Device* device);

	void Update();
	void Render(std::unique_ptr<DirectX::SpriteBatch>& batch);

	ID3D11ShaderResourceView* GetTexture() { return m_texture; }
	void SetTexture(ID3D11ShaderResourceView* value) { m_texture = value; }

	~ImageElement();
};

class ButtonElement : public UIElement
{
	enum TEXTURES{DEFAULT, MOUSEOVER, MOUSECLICK};

	ID3D11ShaderResourceView* m_textures[3];
	ID3D11Device* m_device;
	InputManager* m_input;
	TextElement* m_buttonText = nullptr;

public:
	ButtonElement(RECT srcRect, bool interactable, bool enabled, float2 pos, ID3D11Device* device, InputManager* input, int font, const char* text);

	void Update();
	void RenderText(std::unique_ptr<DirectX::SpriteBatch>& batch, std::unique_ptr<DirectX::SpriteFont>& arial, std::unique_ptr<DirectX::SpriteFont>& comicSans);
	void Render(std::unique_ptr<DirectX::SpriteBatch>& batch, std::unique_ptr<DirectX::SpriteFont>& arial, std::unique_ptr<DirectX::SpriteFont>& comicSans);

	void SetDefaultTexture(const char* filePath);
	void SetMouseOverTexture(const char* filePath);
	void SetMouseClickTexture(const char* filePath);

	~ButtonElement();
};
#endif // !_ELEMENT_H_