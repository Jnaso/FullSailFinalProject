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
		UIManager* m_UI;
		float m_stats[STATS::COUNT];
		int m_cost = 0;

		ImageElement* m_shopBckgrnd;

		ImageElement* m_StatBars[STATS::COUNT];
		ImageElement* m_weaponImage;

		DescriptionBox(UIManager* ui, ID3D11Device* device)
		{
			const wchar_t* temp = L"ShopBkrnd.dds";
			HRESULT hr = CreateDDSTextureFromFile(device, temp, nullptr, m_shopBckgrnd->GetTexture());
		}

		~DescriptionBox()
		{

		}
	};

	std::map<std::string, UIElement*> m_shopUI;
	std::map<std::string, Item*> m_items;

public:

	//Variables For Stats Bar
	float m_selectedStats[STATS::COUNT];
	int m_selectedCost = 0;

	

	Shop(UIManager*, ID3D11Device*);

	

	bool Initialize();

	void Update();

	void SetShopVisibility(bool);

	~Shop();

};

#endif


