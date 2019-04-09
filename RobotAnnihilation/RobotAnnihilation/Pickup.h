#pragma once
#include "GameObject.h"
class Pickup :
	public GameObject
{
public:
	enum PickupType
	{
		HEALTH, DAMAGE, COUNT
	};
	PickupType pickupType;
	Pickup();
	~Pickup();
	void SetType(PickupType type) { pickupType = type; }
	PickupType GetType() { return pickupType; }
};

