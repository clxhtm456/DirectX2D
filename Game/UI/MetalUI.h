#pragma once

#include "Renders/ParallaxObject.h"

class IAmmoObserver
{
public:
	virtual int AmmoCalc(int val) = 0;
};

class MetalUI : public Node, public IAmmoObserver
{
public:
	static MetalUI* Create();
	bool Init();

	~MetalUI();

	void CreateCenterNumber(Node* center);

	void InvisibleNumber();
	void VisibleNumber(int number);
private:
	int lastCount;
	vector<Sprite*> sprites[3];
	// IAmmoObserver을(를) 통해 상속됨
	virtual int AmmoCalc(int val) override;
};