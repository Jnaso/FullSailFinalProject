#include "Shop.h"

Shop::Shop(UIManager * ui, ID3D11Device* device)
{
	m_UIManager = ui;
	m_device = device;
}

bool Shop::Initialize()
{
	#pragma region Adding_Weapons_To_Items_Vector
	//Assult Rifles
	m_items["M16"] = new Item(WEAPONTYPE::AR, 100, 20, 0.3f, 1, "M16 Is A Basic Assult Rifle");
	m_items["FN Scar-L"] = new Item(WEAPONTYPE::AR, 150, 30, 0.3f, 1, "FN Scar-L Is An Upgraded Assult Rifle");
	m_items["ACR"] = new Item(WEAPONTYPE::AR, 225, 40, 0.3f, 1, "ACR Is An Ultra Upgraded Assult Rifle");

	//Sub-Machine Guns
	m_items["MP7"] = new Item(WEAPONTYPE::SMG, 70, 19, 0.25f, 1, "MP7 Is A Basic Sub-Machine Gun");
	m_items["PP-19"] = new Item(WEAPONTYPE::SMG, 100, 30, 0.25f, 1, "PP-19 Is An Upgraded Sub-Machine Gun");
	m_items["P90"] = new Item(WEAPONTYPE::SMG, 160, 40, 0.25f, 1, "P90 Is An Ultra Upgraded Sub-Machine Gun");

	//Hand Guns
	m_items["M9"] = new Item(WEAPONTYPE::PISTOL, 25, 12, 0.5f, 1, "M9 Is A Basic Hand Gun");
	m_items["Glock-18"] = new Item(WEAPONTYPE::PISTOL, 35, 18, 0.5f, 1, "Glock-18  Is An Upgraded Hand Gun");
	m_items["Desert Eagle"] = new Item(WEAPONTYPE::PISTOL, 60, 10, 0.5f, 1, "Desert Eagle Is An Ultra Upgraded Hand Gun");

	//Melee Weapons
	m_items["Base Ball Bat"] = new Item(WEAPONTYPE::MELEE, 10, 0, 0.5f, 1, "Base Ball Bat Is A Basic Melee Weapon");
	m_items["Fire Axe"] = new Item(WEAPONTYPE::MELEE, 10, 0, 0.5f, 1, "Fire Axe Is An Upgraded Melee Weapon");
	m_items["BroadSword"] = new Item(WEAPONTYPE::MELEE, 10, 0, 0.5f, 1, "Broad Sword Is An Ultra Upgraded Melee Weapon");

	m_itemToDisplay = m_items["NULL"] = new Item(WEAPONTYPE::NONE, 0, 0, 0, 0, "BLAH");
	#pragma endregion

	#pragma region Creation_Of_UI_Elements

	#pragma region DESC_TEXT
	m_shopUI["desc text"] = m_UIManager->CreateText(RECT{ 0,0,0,0 }, false, false, float2{ 0,0 }, 0, "Description");
	TextElement* desText = static_cast<TextElement*>(m_shopUI["desc text"]);
	if (desText)
	{
		desText->SetPos(float2{ 500, 200 });
	}
	#pragma endregion

	#pragma region BACKROUND_IMAGE
		m_shopUI["Background"] = m_UIManager->CreateImage(RECT{ 0,0,0,0 }, false, false, float2{ 0,0 }, "DrawingStuff/ShopBkrnd.dds", m_device);
	#pragma endregion
	
	#pragma region AR_BUTTON
	m_shopUI["ARButton"] = m_UIManager->CreateButton(RECT{ 0,0,0,0 }, false, false, float2{ 0,0 }, m_device, 0, "Assult Rifle");
	ButtonElement* arB = static_cast<ButtonElement*>(m_shopUI["ARButton"]);
	if (arB)
	{
		arB->SetDefaultTexture("DrawingStuff/ButtonDefault.dds");
		arB->SetMouseOverTexture("DrawingStuff/ButtonMouseOver.dds");
		arB->SetMouseClickTexture("DrawingStuff/ButtonMouseClick.dds");
		arB->m_OnMouseClick = [this]()
		{
			// Show Details In Description Window
			ShowARStats();
		};
		arB->SetSize(float2{ 200,50 });
		arB->SetPos(float2{ 300,0 });
	}
	else
	{
		return false;
	}
	#pragma endregion
	
	#pragma region SMG_BUTTON
	m_shopUI["SMGButton"] = m_UIManager->CreateButton(RECT{ 0,0,0,0 }, false, false, float2{ 0,0 }, m_device, 0, "Sub-Machine Gun Rifle");
	ButtonElement* sMB = static_cast<ButtonElement*>(m_shopUI["SMGButton"]);
	if (sMB)
	{
		sMB->SetDefaultTexture("DrawingStuff/ButtonDefault.dds");
		sMB->SetMouseOverTexture("DrawingStuff/ButtonMouseOver.dds");
		sMB->SetMouseClickTexture("DrawingStuff/ButtonMouseClick.dds");
		sMB->m_OnMouseClick = [this]()
		{
			//Show Details In Description Window
			ShowSMGStats();
		};
	
		sMB->SetSize(float2{ 200,50 });
		sMB->SetPos(float2{ 300,50 });
	}
	else
	{
		return false;
	}
	#pragma endregion
	
	#pragma region PISTOL_BUTTON
	m_shopUI["PistolButton"] = m_UIManager->CreateButton(RECT{ 0,0,0,0 }, false, false, float2{ 0,0 }, m_device, 0, "Hand Gun Rifle");
	ButtonElement* hGB = static_cast<ButtonElement*>(m_shopUI["PistolButton"]);
	if (hGB)
	{
		hGB->SetDefaultTexture("DrawingStuff/ButtonDefault.dds");
		hGB->SetMouseOverTexture("DrawingStuff/ButtonMouseOver.dds");
		hGB->SetMouseClickTexture("DrawingStuff/ButtonMouseClick.dds");
		hGB->m_OnMouseClick = [this]()
		{
			//Show Details In Description Window
			ShowPistolStats();
		};
	
		hGB->SetSize(float2{ 200,50 });
		hGB->SetPos(float2{ 300,100 });
	}
	else
	{
		return false;
	}
	#pragma endregion
	
	#pragma region MELEE_BUTTON
	m_shopUI["MeleeButton"] = m_UIManager->CreateButton(RECT{ 0,0,0,0 }, false, false, float2{ 0,0 }, m_device, 0, "Melee Weapon");
	ButtonElement* mWB = static_cast<ButtonElement*>(m_shopUI["MeleeButton"]);
	if (mWB)
	{
		mWB->SetDefaultTexture("DrawingStuff/ButtonDefault.dds");
		mWB->SetMouseOverTexture("DrawingStuff/ButtonMouseOver.dds");
		mWB->SetMouseClickTexture("DrawingStuff/ButtonMouseClick.dds");
		mWB->m_OnMouseClick = [this]()
		{
			//Show Details In Description Window
			ShowMeleStats();
		};
	
		mWB->SetSize(float2{ 200,50 });
		mWB->SetPos(float2{ 300,150 });
	}
	else
	{
		return false;
	}
	#pragma endregion
	
	#pragma region UPGRADE_BUTTON
	m_shopUI["Upgrade"] = m_UIManager->CreateButton(RECT{ 0,0,0,0 }, false, false, float2{ 0,0 }, m_device, 0, "UPGRADE");
	ButtonElement* uGB = static_cast<ButtonElement*>(m_shopUI["Upgrade"]);
	if (uGB)
	{
		uGB->SetDefaultTexture("DrawingStuff/ButtonDefault.dds");
		uGB->SetMouseOverTexture("DrawingStuff/ButtonMouseOver.dds");
		uGB->SetMouseClickTexture("DrawingStuff/ButtonMouseClick.dds");
		uGB->m_OnMouseClick = [this]()
		{
			//Show Details In Description Window
			Upgrade(this->m_itemToDisplay->m_weaponType);
		};
	
		uGB->SetSize(float2{ 200,50 });
		uGB->SetPos(float2{ 300,200 });
	}
	else
	{
		return false;
	}
	#pragma endregion

	#pragma endregion

	return true;
}

void Shop::ShowARStats()
{
	if (m_assultRifleLevel <= 5)
	{
		m_itemToDisplay = m_items["M16"];
	}
	else if (m_assultRifleLevel > 5 && m_assultRifleLevel < 20)
	{
		m_itemToDisplay = m_items["FN Scar-L"];
	}
	else if (m_assultRifleLevel >= 20)
	{
		m_itemToDisplay = m_items["ACR"];
	}
}
void Shop::ShowSMGStats()
{
	if (m_subMachineGunLevel <= 5)
	{
		m_itemToDisplay = m_items["MP7"];
	}
	else if (m_subMachineGunLevel > 5 && m_subMachineGunLevel < 20)
	{
		m_itemToDisplay = m_items["PP-19"];
	}
	else if (m_subMachineGunLevel >= 20)
	{
		m_itemToDisplay = m_items["P90"];
	}
}
void Shop::ShowPistolStats()
{
	if (m_pistolLevel <= 5)
	{
		m_itemToDisplay = m_items["M9"];
	}
	else if (m_pistolLevel > 5 && m_pistolLevel < 20)
	{
		m_itemToDisplay = m_items["Glock-18"];
	}
	else if (m_pistolLevel >= 20)
	{
		m_itemToDisplay = m_items["Desert Eagle"];
	}
}
void Shop::ShowMeleStats()
{
	if (m_meleeLevel <= 5)
	{
		m_itemToDisplay = m_items["Base Ball Bat"];
	}
	else if (m_meleeLevel > 5 && m_meleeLevel < 20)
	{
		m_itemToDisplay = m_items["Fire Axe"];
	}
	else if (m_meleeLevel >= 20)
	{
		m_itemToDisplay = m_items["BroadSword"];
	}
}

void Shop::Upgrade(WEAPONTYPE type)
{
	switch (type)
	{
	case Shop::AR:
		m_assultRifleLevel += 1;
		break;
	case Shop::SMG:
		m_subMachineGunLevel += 1;
		break;
	case Shop::PISTOL:
		m_pistolLevel += 1;
		break;
	case Shop::MELEE:
		m_meleeLevel += 1;
		break;
	case Shop::NONE:
		break;
	}
}

void Shop::Update()
{
	TextElement* tempTx = static_cast<TextElement*>(m_shopUI["desc text"]);
	tempTx->SetText(m_itemToDisplay->m_description);
}

void Shop::SetShopVisibility(bool value)
{
	for (std::map<std::string, UIElement*>::iterator i = m_shopUI.begin(); i != m_shopUI.end(); i++)
	{
		i->second->SetEnabled(value);
	}

}

Shop::~Shop()
{
	//Clean Up Items
	for (std::map<std::string, Item*>::iterator i = m_items.begin(); i != m_items.end(); ++i)
	{
		delete i->second;
	}
	m_items.clear();
	//Clean Up UI Elements
	for (std::map<std::string, UIElement*>::iterator i = m_shopUI.begin(); i != m_shopUI.end(); ++i)
	{
		delete i->second;
	}
	m_items.clear();
}
