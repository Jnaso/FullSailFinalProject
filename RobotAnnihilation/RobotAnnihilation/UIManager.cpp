#include "UIManager.h"

UIManager::UIManager(InputManager * input, ID3D11Device* device, ID3D11DeviceContext* context)
{
	m_Input = input;
	m_Device = device;
	m_context = context;
}



void UIManager::Render(std::unique_ptr<DirectX::SpriteBatch>& batch, std::unique_ptr<DirectX::SpriteFont>& arial, std::unique_ptr<DirectX::SpriteFont>& comicSans)
{
	for (unsigned int i = 0; i < m_UIElements.size(); i++)
	{
		TextElement* tElem = dynamic_cast<TextElement*>(m_UIElements[i]);
		if (tElem)
		{
			if (tElem->GetFont() == F_ARIAL)
			{
				tElem->Render(batch, arial);
			}
			else if (tElem->GetFont() == F_COMICSANS)
			{
				tElem->Render(batch, comicSans);
			}
			continue;
		}
		ImageElement* iElem = dynamic_cast<ImageElement*>(m_UIElements[i]);
		if (iElem)
		{
			iElem->Render(batch);
		}
		
	}
}

void UIManager::Update(bool paused)
{
	std::cout << "Mouse Position: {" << m_Input->GetMousePos().x << ", " << m_Input->GetMousePos().y << "}" << std::endl;
	for (unsigned int i = 0; i < m_UIElements.size(); i++)
	{
		UIElement* temp = m_UIElements[i];

		if (temp->GetInteractable())
		{
			if (PtInRect((const RECT*)temp->GetSrcRect(), m_Input->GetMousePos()))
			{
				if (temp->m_OnMouseEnter)
				{ 
					temp->m_OnMouseEnter(); 
				}

				if (temp->GetInteractable())
				{
					if (m_Input->GetKeyState(_LMOUSE))
					{
						if (temp->m_OnMouseClick) { temp->m_OnMouseClick(); }
					}
				}
			}
		}
	}
}

UIElement* UIManager::CreateText(RECT srcRect, bool interactable, bool enabled, float2 pos, int font, const char* text, void(*MouseOver)(), void(*Click)())
{
	UIElement* temp = new TextElement(srcRect, interactable, enabled, pos, MouseOver, Click, font, text);
	m_UIElements.push_back(temp);
	return temp;
}

UIElement* UIManager::CreateImage(RECT srcRect, bool interactable, bool enabled, float2 pos, const char * filePath, ID3D11Device* device, void(*MouseOver)(), void(*Click)())
{
	UIElement* temp = new ImageElement(srcRect, interactable, enabled, pos, MouseOver, Click, filePath, device);
	m_UIElements.push_back(temp);
	return temp;
}

void UIManager::DestroyUIElement(UIElement* item, int index)
{
	m_UIElements.erase(m_UIElements.begin() + index);
	delete item;
}

void UIManager::HideMainMenu()
{
	for (unsigned int i = 0; i < 4; i++)
	{
		m_UIElements[i]->SetEnabled(false);
	}
}

UIManager::~UIManager()
{
	for (unsigned int i = 0; i < m_UIElements.size(); i++)
	{
		delete m_UIElements[i];
	}
}
