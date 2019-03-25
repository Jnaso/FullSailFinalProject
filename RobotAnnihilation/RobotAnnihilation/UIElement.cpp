#include "UIElement.h"



#pragma region Element_Base_Class
UIElement::UIElement(RECT srcRect, bool interactable, bool enabled, float2 pos)
{
	m_srcRect = srcRect;
	m_interactable = interactable;
	m_enabled = enabled;
	SimpleMath::Vector2 temp(pos.x, pos.y);
	m_pos = temp;
}


UIElement::~UIElement()
{
}
#pragma endregion

#pragma region Text_Child_Class
TextElement::TextElement(RECT srcRect, bool interactable, bool enabled, float2 pos, int font, const char* text)
	: UIElement(srcRect, interactable, enabled, pos)
{
	m_font = font;
	m_text = text;
}

void TextElement::Update()
{
}
void TextElement::Render(std::unique_ptr<DirectX::SpriteBatch>& batch, std::unique_ptr<DirectX::SpriteFont>& font)
{
	font->DrawString(batch.get(), this->m_text, this->m_pos);
}
#pragma endregion

#pragma region Image_Child_Class
ImageElement::ImageElement(RECT srcRect, bool interactable, bool enabled, float2 pos, const char* filePath, ID3D11Device* device)
	: UIElement(srcRect, interactable, enabled, pos)
{
	wchar_t* temp = new wchar_t[301];
	mbstowcs(temp, filePath, 301);
	HRESULT hr = CreateDDSTextureFromFile(device, temp, nullptr, &m_texture);
	delete temp;
}
void ImageElement::Update()
{
}
void ImageElement::Render(std::unique_ptr<DirectX::SpriteBatch>& batch)
{
	batch->Draw(this->m_texture, this->m_pos);
}
#pragma endregion
