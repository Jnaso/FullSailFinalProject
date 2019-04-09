#pragma once

#include "GeneralIncludes.h"
#include "UIManager.h"

#ifndef _SHOP_H_
#define _SHOP_H_

class Shop
{
	UIManager* m_UIManager;

	enum STATS{DAMAGE,AMMO_COUNT,FIRE_RATE,COUNT};

	struct Item
	{
		//Access stats by *stats[STATS::(Stat)]*
		float m_stats[STATS::COUNT];
		int m_cost = 0;
		std::string m_description;
		ImageElement* m_image = nullptr;
		Item(float baseDamage = 0.0f, float baseAmmoCount = 0.0f, float baseFireRate = 0.0f, float baseCost = 0.0f, std::string description = "")
		{
			m_stats[STATS::DAMAGE] = baseDamage;
			m_stats[STATS::AMMO_COUNT] = baseAmmoCount;
			m_stats[STATS::FIRE_RATE] = baseDamage;
			m_cost = baseCost;
			m_description = description;
		}
		~Item()
		{
			delete m_image;
		}

	};
	struct DescriptionBox
	{
		float m_stats[STATS::COUNT];
		int m_cost = 0;

		ImageElement* m_shopBckgrnd = nullptr;

		ImageElement* m_StatBars[STATS::COUNT];
		ImageElement* m_weaponImage = nullptr;

		DescriptionBox()
		{
			
		}

		~DescriptionBox()
		{

		}
	};

	std::map<std::string, ButtonElement*> m_buttons;
	std::map<std::string, Item*> m_items;

public:

	

	Shop(UIManager*);

	bool Initialize();

	void Update();

	~Shop();

};

#endif


