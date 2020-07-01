#include "stdafx.h"
#include "SoundManager.h"

using namespace FMOD;

SoundManager::SoundManager():
	system(nullptr),
	sound(nullptr),
	channel(nullptr),
	soundBuffer(15),
	volume(1.0f)
{
	System_Create(&system);

	system->init(soundBuffer, FMOD_INIT_NORMAL, NULL);
	sound = new Sound*[soundBuffer];
	channel = new Channel*[soundBuffer];

	memset(sound, NULL, sizeof(Sound*)*soundBuffer);//ZeroMemory(sound,sizeof(Sound*)*soundBuffer);
	memset(channel, NULL, sizeof(Channel*)*soundBuffer);
}

SoundManager::~SoundManager()
{
	for (UINT i = 0; i < soundBuffer; i++)
	{
		if (channel != nullptr)
		{
			if (channel[i])
				channel[i]->stop();
		}

		if (sound != nullptr)
		{
			if (sound[i])
				sound[i]->release();
		}
	}
	SAFE_DELETE(sound);
	SAFE_DELETE(channel);

	soundList.clear();

	if (system != nullptr)
	{
		system->release();
		system->close();
	}
}

void SoundManager::AddSound(string name, string soundFile, bool loop)
{
	system->createStream
	(
		soundFile.c_str(),
		(loop==true)? FMOD_LOOP_NORMAL: FMOD_LOOP_OFF,
		NULL,
		&sound[soundList.size()]
	);

	soundList.insert(make_pair(name, &sound[soundList.size()]));
}

void SoundManager::Play(string name, float volume)
{
	int count = 0;
	iter = soundList.begin();

	for (iter; iter != soundList.end(); ++iter,count++)
	{
		if (name == iter->first)
		{
			system->playSound
			(
				FMOD_CHANNELINDEX::FMOD_CHANNEL_FREE,
				*iter->second,
				false,
				&channel[count]
			);
			channel[count]->setVolume(volume);
		}
	}
}

void SoundManager::Stop(string name)
{
	int count = 0;
	iter = soundList.begin();
	for (iter; iter != soundList.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			channel[count]->stop();
		}
	}
}

void SoundManager::Pause(string name)
{
	int count = 0;
	iter = soundList.begin();
	for (iter; iter != soundList.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			channel[count]->setPaused(true);
		}
	}
}

void SoundManager::Resume(string name)
{
	int count = 0;
	iter = soundList.begin();
	for (iter; iter != soundList.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			channel[count]->setPaused(false);
		}
	}
}

bool SoundManager::IsPlay(string name)
{
	bool bPlay = false;
	int count = 0;
	iter = soundList.begin();
	for (iter; iter != soundList.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			channel[count]->getPaused(&bPlay);
		}
	}
	return bPlay;
}

bool SoundManager::Paused(string name)
{
	bool bPause = false;
	int count = 0;
	iter = soundList.begin();
	for (iter; iter != soundList.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			channel[count]->getPaused(&bPause);
		}
	}
	return bPause;
}

void SoundManager::Update()
{
	system->update();
}

void SoundManager::Volume(string name, float volume)
{
	int count = 0;
	iter = soundList.begin();
	for (iter; iter != soundList.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			channel[count]->setVolume(volume);
			break;
		}
	}
}
