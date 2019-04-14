#pragma once

#ifndef _ELEMENT_H_
#define _ELEMENT_H_

#define F_ARIAL 0
#define F_COMICSANS 1

#include "GeneralIncludes.h"
#include "InputManager.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>

inline LONG LLerp(LONG l1, LONG l2, float r)
{
	return (l2 - l1) * r + l1;
}

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
	std::function<void()> m_OnMouseExit = nullptr;

	DirectX::SimpleMath::Vector2 m_pos;

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
	void SetEnabled(bool value) {if (this != nullptr){m_enabled = value;}}
	
	bool GetMouseOver() { return m_mouseOver; }
	void SetMouseOver(bool value) { m_mouseOver = value; }

	float2 GetPos() 
	{
		return { m_pos.x, m_pos.y };
	}
	float2 GetTopLeft()
	{
		return float2{ static_cast<float>(m_destRect.left), static_cast<float>(m_destRect.top) };
	}
	float2 GetBottomRight()
	{
		return float2{ static_cast<float>(m_destRect.right), static_cast<float>(m_destRect.bottom) };
	}
	void SetPos(float2 value) 
	{ 
		m_destRect.right = static_cast<LONG>(value.x + GetSize().x);
		m_destRect.bottom = static_cast<LONG>(value.y + GetSize().y);
		m_destRect.left = value.x;
		m_destRect.top = value.y;
		
		m_pos = { value.x, value.y };
	}
	
	void SetSize(float2 value) 
	{ 
		m_destRect = 
		{ 
			static_cast<LONG>(m_pos.x), 
			static_cast<LONG>(m_pos.y),
			static_cast<LONG>(m_pos.x + value.x),
			static_cast<LONG>(m_pos.y + value.y)
		}; 
	}
	void SetSize(LONG x, LONG y)
	{
		m_destRect = 
		{
			static_cast<LONG>(m_pos.x),
			static_cast<LONG>(m_pos.y),
			static_cast<LONG>(m_pos.x + x),
			static_cast<LONG>(m_pos.y + y)
		};
	}
	void SetSize(LONG x)
	{
		m_destRect = 
		{
			static_cast<LONG>(m_pos.x),
			static_cast<LONG>(m_pos.y),
			static_cast<LONG>(m_pos.x + x),
			static_cast<LONG>(m_pos.y)
		};
	}

	void SetTop(LONG top)
	{
		m_destRect.top = top;
	}
	void SetLeft(LONG left)
	{
		m_destRect.left = left;
	}
	void SetBottom(LONG bottom)
	{
		m_destRect.bottom = bottom;
	}
	void SetRight(LONG right)
	{
		m_destRect.right = right;
	}

	LONG GetTop() { return m_destRect.top; }
	LONG GetLeft() { return m_destRect.left; }
	LONG GetBottom() { return m_destRect.bottom; }
	LONG GetRight() { return m_destRect.right; }

	//Float value between 0 and 100
	bool SetFillLinear(int value)
	{
		if (value < 0 || value > 100)
		{
			throw std::exception("Value is not between 0 or 100", value);
		}

		m_destRect.right = LLerp(m_destRect.left, m_destRect.right, value * .01f);
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
	virtual void Update(float time) {}
	virtual void Render() {}
};

class TextElement : public UIElement
{
private:
	int m_font;
	char* m_text = new char[256];

public:
	TextElement(RECT srcRect, bool interactable, bool enabled, float2 pos, int font, char* text);

	void Update(float time);
	void Render(std::unique_ptr<DirectX::SpriteBatch>& batch, std::unique_ptr<DirectX::SpriteFont>& arial, std::unique_ptr<DirectX::SpriteFont>& comicSans);

	int GetFont() { return m_font; }
	void SetFont(int value) { m_font = value; }

	char* GetText() { return m_text; }
	void SetText(char* value) { m_text = value; }
	void SetText(std::string value) 
	{
		memcpy(m_text, value.c_str(), 256);
		//m_text = (char*)value.c_str(); 
	}

	~TextElement();
};

class ImageElement : public UIElement
{
private:
	ID3D11ShaderResourceView* m_texture;

public:
	ImageElement(RECT srcRect, bool interactable, bool enabled, float2 pos, const char* filePath, ID3D11Device* device);

	void Update(float time);
	void Render(std::unique_ptr<DirectX::SpriteBatch>& batch);

	ID3D11ShaderResourceView** GetTexture() { return &m_texture; }
	void SetTexture(ID3D11ShaderResourceView* value) { m_texture = value; }

	~ImageElement();
};

class ButtonElement : public UIElement
{
	const float CLICKTIME = 0.5f;
	float m_clickTimeLeft = 0.5f;

	enum TEXTURES{DEFAULT, MOUSEOVER, MOUSECLICK};

	ID3D11ShaderResourceView* m_textures[3];
	ID3D11Device* m_device;
	InputManager* m_input;

	

public:
	TextElement* m_buttonText = nullptr;

public:
	ButtonElement(RECT srcRect, bool interactable, bool enabled, float2 pos, ID3D11Device* device, InputManager* input, int font, const char* text);

	void Update(float time);
	void RenderText(std::unique_ptr<DirectX::SpriteBatch>& batch, std::unique_ptr<DirectX::SpriteFont>& arial, std::unique_ptr<DirectX::SpriteFont>& comicSans);
	void Render(std::unique_ptr<DirectX::SpriteBatch>& batch, std::unique_ptr<DirectX::SpriteFont>& arial, std::unique_ptr<DirectX::SpriteFont>& comicSans);

	void SetDefaultTexture(const char* filePath);
	void SetMouseOverTexture(const char* filePath);
	void SetMouseClickTexture(const char* filePath);

	~ButtonElement();
};
#endif // !_ELEMENT_H_