#pragma once

#ifndef _ELEMENT_H_
#define _ELEMENT_H_

#include "UIManager.h"

class UIElement
{
protected:
	RECT m_srcRect;
	bool m_interactable;
	bool m_mouseOver;
	bool m_enabled;
	float2 m_pos;

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

	float2 GetPos() { return m_pos; }
	void setPos(float2 value) { m_pos = value; }
	#pragma endregion

	//Pure Virtual Function
	virtual void Update() = 0;

	virtual void Render() = 0;
};

class TextElement : UIElement
{
private:
	int m_font;
	const char* m_text;

public:
	TextElement(RECT srcRect, bool interactable, bool enabled, float2 pos, int font, const char* text);

	void Update();
	void Render();

	int GetFont() { return m_font; }
	void SetFont(int value) { m_font = value; }


};

class ImageElement : UIElement
{
private:
	ID3D11ShaderResourceView* m_texture;

public:
	ImageElement(RECT srcRect, bool interactable, bool enabled, float2 pos, const char* filePath, ID3D11Device* device);

	void Update();
	void Render();
};

#endif // !_ELEMENT_H_