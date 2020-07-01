#include "stdafx.h"
#include "EffectManager.h"

EffectManager* EffectManager::_instance;

EffectManager* EffectManager::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new EffectManager();
	}
	return _instance;
}

EffectManager::~EffectManager()
{
	for (auto iter = effectList.begin(); iter != effectList.end(); iter++)
	{
		(*iter).second->autorelease();
		//iter = effectList.erase(iter);
	}
}

Clip* EffectManager::SearchEffect(string effectName)
{
	for (auto iter = effectList.begin(); iter != effectList.end(); iter++)
	{
		if ((*iter).first == effectName)
		{
			if ((*iter).second->IsPlaying() == false)
			{
				(*iter).second->SetRunning(true);
				(*iter).second->SetVisible();
				return (*iter).second;
			}
		}
	}
	return nullptr;
}

Clip* EffectManager::CreateEffect(string effectName, Clip* effect)
{
	effect->SetEndCallBack([=]()->void {
		effect->SetRunning(false);
		effect->SetInvisible();
		}
	);
	auto item = pair<string, Clip*>(effectName, effect);
	effectList.push_back(item);
	return item.second;
}
