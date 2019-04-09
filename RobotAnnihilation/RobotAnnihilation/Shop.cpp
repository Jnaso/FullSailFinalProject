#include "Shop.h"

Shop::Shop(UIManager * ui)
{
	m_UIManager = ui;

	//Assult Rifles
	m_items["M16"] = new Item(100, 20, 0.3f, 1, "M16 Is A Basic Assult Rifle");
	m_items["FN Scar-L"] = new Item(150, 30, 0.3f, 1, "FN Scar-L Is An Upgraded Assult Rifle");
	m_items["ACR"] = new Item(225, 40, 0.3f, 1, "ACR Is An Ultra Upgraded Assult Rifle");

	//Sub-Machine Guns
	m_items["MP7"] = new Item(70, 19, 0.25f, 1, "MP7 Is A Basic Sub-Machine Gun");
	m_items["PP-19"] = new Item(100, 30, 0.25f, 1, "PP-19 Is An Upgraded Sub-Machine Gun");
	m_items["P90"] = new Item(160, 40, 0.25f, 1, "P90 Is An Ultra Upgraded Sub-Machine Gun");

	//Hand Guns
	m_items["M9"] = new Item(25, 12, 0.5f, 1, "M9 Is A Basic Hand Gun");
	m_items["Glock-18"] = new Item(35, 18, 0.5f, 1, "Glock-18  Is An Upgraded Hand Gun");
	m_items["Desert Eagle"] = new Item(60, 10, 0.5f, 1, "Desert Eagle Is An Ultra Upgraded Hand Gun");

	//Melee Weapons
	m_items["Base Ball Bat"] = new Item(10, 0, 0.5f, 1, "Base Ball Bat Is A Basic Melee Weapon");
	m_items["Fire Axe"] = new Item(10, 0, 0.5f, 1, "Fire Axe Is An Upgraded Melee Weapon");
	m_items["BroadSword"] = new Item(10, 0, 0.5f, 1, "Broad Sword Is An Ultra Upgraded Melee Weapon");
}

Shop::~Shop()
{
	for (std::map<std::string, Item*>::iterator i = m_items.begin(); i != m_items.end(); ++i)
	{
		delete i->second;
	}
	m_items.clear();
}
