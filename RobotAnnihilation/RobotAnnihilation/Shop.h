#pragma once

#include "GeneralIncludes.h"
#include "UIManager.h"

#ifndef _SHOP_H_
#define _SHOP_H_

class Shop
{
	UIManager* m_UIManager;
	ID3D11Device* m_device;


	int m_assultRifleLevel = 0;
	int m_subMachineGunLevel = 0;
	int m_pistolLevel = 0;
	int m_meleeLevel = 0;

	

	enum STATS{DAMAGE,AMMO_COUNT,FIRE_RATE,COUNT};
	enum WEAPONTYPE { AR, SMG, PISTOL, MELEE, NONE };

	struct Item
	{
		//Access stats by *stats[STATS::(Stat)]*
		float m_stats[STATS::COUNT];
		int m_cost = 0;

		WEAPONTYPE m_weaponType;

		std::string m_description;

		ImageElement* m_image = nullptr;

		Item(WEAPONTYPE weaponType, float baseDamage = 0.0f, float baseAmmoCount = 0.0f, float baseFireRate = 0.0f, float baseCost = 0.0f, std::string description = "")
		{
			m_stats[STATS::DAMAGE] = baseDamage;
			m_stats[STATS::AMMO_COUNT] = baseAmmoCount;
			m_stats[STATS::FIRE_RATE] = baseDamage;
			m_cost = baseCost;
			m_description = description;
			m_weaponType = weaponType;
		}
		~Item()
		{
			delete m_image;
		}

	};

	std::map<std::string, UIElement*> m_shopUI;
	std::map<std::string, Item*> m_items;

public:

	//Variables For Stats Bar
	float m_selectedStats[STATS::COUNT];
	int m_selectedCost = 0;
	std::string m_description;

	Item* m_itemToDisplay;
	

	Shop(UIManager*, ID3D11Device*);

	bool Initialize();

	void ShowARStats();
	void ShowSMGStats();
	void ShowPistolStats();
	void ShowMeleStats();

	
	void Upgrade(WEAPONTYPE);

	void Update();

	void SetShopVisibility(bool);

	~Shop();

};

#endif


