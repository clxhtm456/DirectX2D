#pragma once

#include "Item.h"

class Item_H : public Item
{
public:
	static Item_H* Create();
	bool Init();

	~Item_H();

	// Item��(��) ���� ��ӵ�
	virtual void ItemEffect(PhysicsObject * b) override;
};