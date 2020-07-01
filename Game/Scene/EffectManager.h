#pragma once

class EffectManager
{
public:
	static EffectManager* GetInstance();
	static void Destroy()
	{
		delete _instance;
	}
	~EffectManager();
	Clip* SearchEffect(string effectName);
	Clip* CreateEffect(string effectName,Clip* effect);

private:
	static EffectManager* _instance;
	vector<pair<string, Clip*>> effectList;


};