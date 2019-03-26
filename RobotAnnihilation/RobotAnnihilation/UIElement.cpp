#include "UIElement.h"



#pragma region Element_Base_Class
UIElement::UIElement(RECT srcRect, bool interactable, bool enabled, float2 pos)
{
	m_destRect = srcRect;
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
	if (m_enabled)
	{
		font->DrawString(batch.get(), this->m_text, this->m_pos);
	}
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

	//Set Defult Rect Dimentions
	ID3D11Resource* res = nullptr;
	m_texture->GetResource(&res);

	ID3D11Texture2D* tex2d = nullptr;
	hr = res->QueryInterface(&tex2d);

	float2 dim = { 0, 0 };
	if (SUCCEEDED(hr))
	{
		D3D11_TEXTURE2D_DESC desc;
		tex2d->GetDesc(&desc);
		dim.x = static_cast<float>(desc.Width);
		dim.y = static_cast<float>(desc.Height);
	}

	if (tex2d) { tex2d->Release(); }
	if (res) { res->Release(); }

	float4 tempF = { (int)m_pos.x, (int)m_pos.y, (int)dim.x, (int)dim.y  };

	m_destRect = RECT
	{
		(int)m_pos.x, (int)m_pos.y, (int)m_pos.x + (int)dim.x, (int)m_pos.y + (int)dim.y  
	};
}
void ImageElement::Update()
{
}
void ImageElement::Render(std::unique_ptr<DirectX::SpriteBatch>& batch)
{
	if (m_enabled)
	{
		batch->Draw(this->m_texture, this->m_destRect);
	}
}
#pragma endregion
