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
void TextElement::Render(std::unique_ptr<DirectX::SpriteBatch>& batch, std::unique_ptr<DirectX::SpriteFont>& arial, std::unique_ptr<DirectX::SpriteFont>& comicSans)
{
	if (m_enabled)
	{
		if (this->m_font == F_ARIAL)
		{
			arial->DrawString(batch.get(), this->m_text, this->m_pos);
		}
		else if (this->m_font == F_COMICSANS)
		{
			comicSans->DrawString(batch.get(), this->m_text, this->m_pos);
		}
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

	//Set Defult Rect Dimensions
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

	m_destRect =
	{
		(int)m_pos.x,				//Left
		(int)m_pos.y,				//Top
		(int)m_pos.x + (int)dim.x,	//Right
		(int)m_pos.y + (int)dim.y	//Bottom
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
ImageElement::~ImageElement()
{
	if (m_texture) { m_texture->Release(); };
}
#pragma endregion

#pragma region Button_Child_Class

//MAKE SURE TO SET THE DEFAULT TEXTURE!! (SetDefaultTexture(File Path))
ButtonElement::ButtonElement(RECT srcRect, bool interactable, bool enabled, float2 pos, ID3D11Device* device, InputManager* input, /*Text Variables*/ int font, const char* text)
	: UIElement(srcRect, interactable, enabled, pos)
{
	m_device = device;
	m_input = input;
	//Create Text Element
	//If Constructor recevies defaults will be Arial font and blank text
	m_buttonText = new TextElement(RECT{ 0,0,0,0 }, false, true, pos, font, text);
	
}

void ButtonElement::Update()
{
	m_interactable = m_enabled;
	m_buttonText->SetEnabled(m_enabled);
	
	if (m_interactable)
	{
		PtInRect(this->GetSrcRect(), m_input->GetMousePos()) ? m_mouseOver = true : m_mouseOver = false;
		if (m_mouseOver)
		{
			if (m_OnMouseEnter)
			{
				this->m_OnMouseEnter();
			}
			if (this->m_input->GetKeyState(_LMOUSE))
			{
				if (m_OnMouseClick != nullptr)
				{
					this->m_OnMouseClick();
				}
				return;
			}
		}
	}
}
void ButtonElement::RenderText(std::unique_ptr<DirectX::SpriteBatch>& batch, std::unique_ptr<DirectX::SpriteFont>& arial, std::unique_ptr<DirectX::SpriteFont>& comicSans)
{
	//Draw Text
	int currfont = m_buttonText->GetFont();
	SimpleMath::Vector2 tempPos = { this->GetPos().x,this->GetPos().y };
	if (currfont == F_ARIAL)
	{
		arial->DrawString(batch.get(), m_buttonText->GetText(), tempPos);
	}
	else if (currfont == F_COMICSANS)
	{
		comicSans->DrawString(batch.get(), m_buttonText->GetText(), tempPos);
	}
}
void ButtonElement::Render(std::unique_ptr<DirectX::SpriteBatch>& batch, std::unique_ptr<DirectX::SpriteFont>& arial, std::unique_ptr<DirectX::SpriteFont>& comicSans)
{
	//Draw Image
	if (this->m_buttonText->GetEnabled())
	{
		RenderText(batch, arial, comicSans);
	}
	if (this->m_enabled)
	{
		//If the mouse is over the button(within the RECT)
		if (m_mouseOver)
		{
			if (m_input->GetKeyState(_LMOUSE))
			{
				if (m_textures[ButtonElement::MOUSECLICK])
				{
					//If button is clicked (Left Mouse Button)
					
					batch->Draw(m_textures[ButtonElement::MOUSECLICK], this->m_destRect);
					return;
				}
			}
			else
			{
				if (m_textures[ButtonElement::MOUSEOVER])
				{
					
					batch->Draw(m_textures[ButtonElement::MOUSEOVER], this->m_destRect);
					return;
				}
			}
			
		}
		//Else Draw the normal texture
		
		batch->Draw(m_textures[ButtonElement::DEFAULT], this->m_destRect);
	}
}
//Sets The Dimentions of the Image
void ButtonElement::SetDefaultTexture(const char * filePath)
{
	wchar_t* temp = new wchar_t[301];
	mbstowcs(temp, filePath, 301);
	HRESULT hr = CreateDDSTextureFromFile(m_device, temp, nullptr, &m_textures[ButtonElement::DEFAULT]);
	delete temp;

	//Set Rect Dimensions
		//Set Defult Rect Dimensions
	ID3D11Resource* res = nullptr;
	m_textures[ButtonElement::DEFAULT]->GetResource(&res);

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


	saferelease(tex2d);
	saferelease(res);

	m_destRect = 
	{
		(int)m_pos.x,				//Left
		(int)m_pos.y,				//Top
		(int)m_pos.x + (int)dim.x,	//Right
		(int)m_pos.y + (int)dim.y	//Bottom
	};
}

//Make sure image is same dimensions as m_textures[BUTTONELEMENT::DEFAULT]
void ButtonElement::SetMouseOverTexture(const char * filePath)
{
	wchar_t* temp = new wchar_t[301];
	mbstowcs(temp, filePath, 301);
	HRESULT hr = CreateDDSTextureFromFile(m_device, temp, nullptr, &m_textures[ButtonElement::MOUSEOVER]);
	delete temp;
}

//Make sure image is same dimensions as m_textures[BUTTONELEMENT::DEFAULT]
void ButtonElement::SetMouseClickTexture(const char * filePath)
{
	wchar_t* temp = new wchar_t[301];
	mbstowcs(temp, filePath, 301);
	HRESULT hr = CreateDDSTextureFromFile(m_device, temp, nullptr, &m_textures[ButtonElement::MOUSECLICK]);
	delete temp;
}

ButtonElement::~ButtonElement()
{
	//Delete all Textures
	for (unsigned int i = 0; i < ARRAYSIZE(m_textures); i++)
	{
		saferelease(m_textures[i]);
	}
	//Delete text assigned to button
	delete m_buttonText;
}
#pragma endregion


