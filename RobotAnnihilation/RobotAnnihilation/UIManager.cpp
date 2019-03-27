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

void UIManager::Update()
{
#ifdef DEBUG
	std::cout << "Mouse Position: {" << m_Input->GetMousePos().x << ", " << m_Input->GetMousePos().y << "}" << std::endl;
#endif // !DEBUG

	for (unsigned int i = 0; i < m_UIElements.size(); i++)
	{
		if (m_UIElements[i]->GetInteractable())
		{
			if (PtInRect((const RECT*)m_UIElements[i]->GetSrcRect(), m_Input->GetMousePos()))
			{
				m_UIElements[i]->SetEnabled(false);
			}
			else
			{
				m_UIElements[i]->SetEnabled(true);
			}
		}
	}
}

UIElement* UIManager::CreateText(RECT srcRect, bool interactable, bool enabled, float2 pos, int font, const char* text)
{
	UIElement* temp = new TextElement(srcRect, interactable, enabled, pos, font, text);
	m_UIElements.push_back(temp);
	return temp;
}

UIElement* UIManager::CreateImage(RECT srcRect, bool interactable, bool enabled, float2 pos, const char * filePath, ID3D11Device* device)
{
	UIElement* temp = new ImageElement(srcRect, interactable, enabled, pos, filePath, device);
	m_UIElements.push_back(temp);
	return temp;
}

UIManager::~UIManager()
{
	for (unsigned int i = 0; i < m_UIElements.size(); i++)
	{
		delete m_UIElements[i];
	}
}
