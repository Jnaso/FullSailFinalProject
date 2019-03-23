#include "UIElement.h"



UIElement::UIElement(RECT srcRect, bool interactable, bool enabled, float2 pos)
{
	m_srcRect = srcRect;
	m_interactable = interactable;
	m_enabled = enabled;
	m_pos = pos;
}


UIElement::~UIElement()
{
}


TextElement::TextElement(RECT srcRect, bool interactable, bool enabled, float2 pos, int font, const char* text)
	: UIElement(srcRect, interactable, enabled, pos)
{
	m_font = font;
	m_text = text;
}

ImageElement::ImageElement(RECT srcRect, bool interactable, bool enabled, float2 pos, const char* filePath, ID3D11Device* device)
	: UIElement(srcRect, interactable, enabled, pos)
{
	wchar_t* temp = new wchar_t[301];
	mbstowcs(temp, filePath, 301);
	HRESULT hr = CreateDDSTextureFromFile(device, temp, nullptr, &m_texture);
	delete temp;
}
