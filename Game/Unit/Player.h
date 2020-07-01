#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Viewer/IFollow.h"
#include "Object/PhysicsObject.h"
#include "Renders/Rect.h"
#include "Game/Unit/Bullet.h"
#include "Vehicle.h"
#include "Game/Gun/Weapon.h"
#include "Game/Gun/HandGun.h"
#include "Game/UI/MetalUI.h"
#include "Game/Unit/MetalUnit.h"

#define TAG_PLAYER	 5

enum class AnimationState
{
	IDLE,
	RUN,
	STARTLOOKTOP,
	LOOKTOP,
	SHOOT,
	TOPSHOOT,
	LOOK_BEHIND,
	SHOOT_BEHIND
};

enum class LegState
{
	IDLE,
	RUN,
	RUN_JUMP,
	NORMAL_JUMP
};

class Player : public MetalUnit ,public IFollow
{
public:
	static Player* Create();
	bool Init();
	Player();
	~Player();

	void Update();
	void Render();

	void LookTop();
	void LookDown();
	void StartCrouch();
	void EndCrouch();

	void Jump();

	void StartAnimation();

	void SetShootable(bool val);
	void Shoot();
	void BehindShoot();
	void BodyPlayAnimation(int value);
	void LegPlayAnimation(int value);

	

	float GetMoveSpeed(){return moveSpeed;}

	Sprite* GetSprite() { return legAnim->GetSprite(); }
public:

public:
	//Camera
	float landHeight;
	//IFollow
	virtual void Focus(OUT D3DXVECTOR2& position, OUT D3DXVECTOR2& size) override;
	
public:
	// PhysicsObject을(를) 통해 상속됨
	virtual void EnterPhysicsCollision(PhysicsObject * b, ColDirection direction) override;

private://playAnimation
	Animation * bodyAnim;
	Animation * legAnim;
	Animation* extraAnim;
	Rect* rect;

	AnimationState state;
	LegState legState;

	void CreateShot();
	bool bPlayBodyAnim;
	bool bShootable;
	bool bLookTop;
	bool bLookDown;
	bool bDrawExtraAnim;

	bool bDrawBound;
	bool bMove;
	float moveSpeed;

	float extraHeight;
	float bodyOffSet;

	D3DXVECTOR2 focusOffSet;
public:
	void SetWeapon(Weapon* weapon);
	void EquipHandgun();
private:
	Weapon* getWeapon();
	Weapon* _weapon;
	Vehicle* _vehicle;
private:
	IAmmoObserver* ammoObserver;
public:
	void SetAmmoObserver(IAmmoObserver* observer);

//Bloom
private:
	bool bShoot;
public:
	void DrawBloom(bool val);
	void DrawTwinkle(bool val);

	// PhysicsObject을(를) 통해 상속됨
	virtual void ExitPhysicsCollision(PhysicsObject * b) override;

	// PhysicsObject을(를) 통해 상속됨
	virtual void OnPhysicsCollision(PhysicsObject* b, ColDirection direction) override;

	// MetalUnit을(를) 통해 상속됨
	virtual void DamageTaken(float damage) override;
	virtual void Died() override;
};

#endif