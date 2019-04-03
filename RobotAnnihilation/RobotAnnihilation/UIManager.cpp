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
			tElem->Render(batch, arial, comicSans);
			continue;
		}
		ImageElement* iElem = dynamic_cast<ImageElement*>(m_UIElements[i]);
		if (iElem)
		{
			iElem->Render(batch);
			continue;
		}
		ButtonElement* bElem = dynamic_cast<ButtonElement*>(m_UIElements[i]);
		if (bElem)
		{
			bElem->Render(batch, arial, comicSans);
			continue;
		}
		
	}
}

void UIManager::Update()
{
	#ifdef DEBUG
	std::cout << "Mouse Position: {" << m_Input->GetMousePos().x << ", " << m_Input->GetMousePos().y << "}" << std::endl;
	#endif // !DEBUG

	for (unsigned int i = 0; i < m_UIElements.size(); i++)
	{
		m_UIElements[i]->Update();
	}
	//for (unsigned int i = 0; i < m_UIElements.size(); i++)
	//{
	//	UIElement* temp = m_UIElements[i];
	//
	//	if (temp->GetInteractable())
	//	{
	//		if (PtInRect((const RECT*)temp->GetSrcRect(), m_Input->GetMousePos()))
	//		{
	//			if (temp->m_OnMouseEnter)
	//			{ 
	//				temp->m_OnMouseEnter(); 
	//			}
	//
	//			if (temp->GetInteractable())
	//			{
	//				if (m_Input->GetKeyState(_LMOUSE))
	//				{
	//					if (temp->m_OnMouseClick) { temp->m_OnMouseClick(); }
	//				}
	//			}
	//		}
	//	}
	//}
}

UIElement* UIManager::CreateText(RECT srcRect, bool interactable, bool enabled, float2 pos, int font, const char* text)
{
	UIElement* temp = new TextElement(srcRect, interactable, enabled, pos, font, text);
	m_UIElements.push_back(temp);
	++textCount;
	return temp;
}

UIElement* UIManager::CreateImage(RECT srcRect, bool interactable, bool enabled, float2 pos, const char * filePath, ID3D11Device* device)
{
	UIElement* temp = new ImageElement(srcRect, interactable, enabled, pos, filePath, device);
	m_UIElements.push_back(temp);
	return temp;
}

UIElement * UIManager::CreateButton(RECT srcRect, bool interactable, bool enabled, float2 pos, ID3D11Device * device, int font, const char * text)
{
	UIElement* temp = new ButtonElement(srcRect, interactable, enabled, pos, device, m_Input, font, text);
	m_UIElements.insert(m_UIElements.begin() + textCount, temp);
	return temp;
}

void UIManager::DestroyUIElement(UIElement* item, int index)
{
	m_UIElements.erase(m_UIElements.begin() + index);
	delete item;
}

void UIManager::HideMainMenu()
{
	for (unsigned int i = 0; i < m_UIElements.size(); i++)
	{
		m_UIElements[i]->SetEnabled(false);
	}
}

std::vector<UIElement*> UIManager::GetUIElements()
{
	return m_UIElements;
}

UIManager::~UIManager()
{
	for (unsigned int i = 0; i < m_UIElements.size(); i++)
	{
		delete m_UIElements[i];
	}
}
