#ifndef __SOUND_MANAGER_H__
#define __SOUND_MANAGER_H__

using namespace FMOD;

class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	void AddSound(string name,string soundFile,bool loop = false);
	void Play(string name, float volume = 1.0f);
	void Stop(string name);
	void Pause(string name);
	void Resume(string name);

	bool IsPlay(string key);
	bool Paused(string key);

	void Update();

	void Volume(string name, float volume);
	float Volume() { return volume; }

private:
	System * system;
	Sound** sound;
	Channel** channel;

	UINT soundBuffer;
	std::map<string, Sound**> soundList;
	std::map<string, Sound**>::iterator iter;

	float volume;

};

#endif