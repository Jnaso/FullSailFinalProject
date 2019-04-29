#pragma once
#include "Model.h"
class Gun
{
public:
	enum GunClass
	{
		PISTOL, SUBMACHINE, MACHINE, COUNT
	};
protected:
	float FireRate;
	float DamageAmount;
	int CurrentAmmo;
	int MaxClipAmmo;
	//int MaxReserve;
	//int ReserveAmmo;
	float ReloadTime;
	bool Reloading;
	float TimeLeftReloading;

	float3 ShootPos;

	Model* gunModel;
	GunClass gunClass;
public:
	Gun();
	~Gun();
	bool Initialize(const char* filePath, ID3D11Device* device);
	void Render(ID3D11DeviceContext* context);
	void Shutdown();

	void Update(float delta);

	float GetFireRate() { return FireRate; };
	void SetFireRate(float NewFireRate) { FireRate = NewFireRate; };

	float GetDamageAmount() { return DamageAmount; };
	void SetDamageAmount(float damage) { DamageAmount = damage; };

	int GetCurrentAmmo() { return CurrentAmmo; }
	void SetCurrentAmmo(int ammo) { CurrentAmmo = ammo; }
	void ShootBullet() { CurrentAmmo--; }
	bool isReloading() { return Reloading; }/*

	int GetReserveAmmo() { return ReserveAmmo; }
	void SetReserveAmmo(int reserve) { ReserveAmmo = reserve; }

	int GetMaxReserveAmmo() { return MaxReserve; }
	void SetMaxReserveAmmo(int reserve) { ReserveAmmo = reserve; MaxReserve = reserve; }*/

	int GetMaxClipAmmo() { return MaxClipAmmo; }
	void SetMaxClipAmmo(int clip) {
		CurrentAmmo = clip;
		MaxClipAmmo = clip; }

	float GetReloadTime() { return ReloadTime; }
	void SetReloadTime(float reload) { ReloadTime = reload; }

	float3 GetShootPos() { return ShootPos; };
	void SetShootPos(float3 newPos) { ShootPos = newPos; };

	GunClass GetGunClass() { return gunClass; };
	void SetGunClass(GunClass newGun) { gunClass = newGun; };

	void Reload();
};

