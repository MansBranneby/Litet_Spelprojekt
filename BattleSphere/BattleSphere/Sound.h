#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include <Audio.h>

#include "DX.h"

using namespace DirectX;

enum class soundType
{
	e_music = 0,
	e_effect = 1,
	e_ambient = 2
};

enum class soundEffect
{
	e_pistol = 0,
	e_rifle = 1,
	e_movement = 2,
	e_shield = 3,
	e_dash = 4,
	e_reflect = 5,
	e_pickup = 6,
	e_turnin = 7,
	e_damageS = 8,
	e_damageM = 9,
	e_damageL = 10,
	e_impactS = 11,
	e_impactM = 12,
	e_impactL = 13,
	e_explotion = 14,
	e_blast = 15,
	e_traverse = 16
};

enum class soundAmbient
{
	e_background = 0,
	e_drone = 1,
	e_car = 2,
	e_melee = 3,
	e_path = 4
};

enum class soundMusic
{
	e_menu = 0,
	e_game = 1
};

#define NR_EFFECT_SOUNDS 2
#define NR_AMBIENT_SOUNDS 2
#define NR_MUSIC_SOUNDS 2
#define NR_EFFECT_INSTANCES 50

class Sound
{
private:
	Sound();
	static Sound* m_instance;

	std::unique_ptr<AudioEngine> m_audEngine;

	XMVECTOR m_listenerPos;
	AudioListener m_listener;
	AudioEmitter m_emitter;

	std::unique_ptr<SoundEffect> m_effect[NR_EFFECT_SOUNDS];
	std::unique_ptr<SoundEffect> m_ambient[NR_AMBIENT_SOUNDS];
	std::unique_ptr<SoundEffect> m_music[NR_MUSIC_SOUNDS];

	std::unique_ptr<SoundEffectInstance> m_effectInstances[NR_EFFECT_INSTANCES];
	std::unique_ptr<SoundEffectInstance> m_ambientInstances[NR_AMBIENT_SOUNDS];
	std::unique_ptr<SoundEffectInstance> m_musicInstances[NR_MUSIC_SOUNDS];
	int m_index;
	float m_slowStop;

public:
	static Sound* getInstance();

	void play(soundMusic sound, float volume = 1.0f, float pitch = 0.0f, float pan = 0.0f);
	void play(soundAmbient sound, float volume = 1.0f, float pitch = 0.0f, float pan = 0.0f);
	void play(soundAmbient sound, XMVECTOR pos, float volume = 1.0f, float pitch = 0.0f, float pan = 0.0f);
	void play(soundEffect sound, XMVECTOR pos, float volume = 1.0f, float pitch = 0.0f, float pan = 0.0f);

	void stop(soundAmbient sound);

	void update3D(soundAmbient sound, XMVECTOR pos = XMVectorSet(0, -100, 0, 0));
	void update(float dt);

	void release();
};

