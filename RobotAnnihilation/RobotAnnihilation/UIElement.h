#pragma once

#ifndef _ELEMENT_H_
#define _ELEMENT_H_

#define F_ARIAL 0
#define F_COMICSANS 1

#include "GeneralIncludes.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>

class UIElement
{
protected:
	RECT m_srcRect;
	bool m_interactable;
	bool m_mouseOver;
	bool m_enabled;
	DirectX::SimpleMath::Vector2 m_pos;

public:
	UIElement(RECT srcRect, bool interactable, bool enabled, float2 pos);
	~UIElement();

	#pragma region Acessors_And_Mutators
	RECT GetSrcRect() { return m_srcRect; }
	void SetSrcRect(RECT value) { m_srcRect = value; }
	
	bool GetInteractable() { return m_interactable; }
	void SetInteractable(bool value) { m_interactable = value; }
	
	bool GetEnabled() { return m_enabled; }
	void SetEnabled(bool value) { m_enabled = value; }
	
	float2 GetPos() 
	{
		float2 temp = { m_pos.x, m_pos.y };
		return temp;
	}
	void setPos(float2 value) 
	{ 
		SimpleMath::Vector2 temp(value.x, value.y);
		m_pos = temp; 
	}
	#pragma endregion



	//Virtual Function
	virtual void Update() {};
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
};

#endif // !_ELEMENT_H_