#ifndef __INTERFACE_FOLLOW_H__
#define __INTERFACE_FOLLOW_H__

#include "stdafx.h"

class IFollow
{
public:
	virtual void Focus(OUT D3DXVECTOR2& position, OUT D3DXVECTOR2& size) = 0;
};

#endif