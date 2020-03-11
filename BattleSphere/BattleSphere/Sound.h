#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include <Audio.h>

#include "DX.h"

using namespace DirectX;

enum class soundEffect
{
	e_pistol = 0,
	e_rifle = 1,
	e_movement = 2,
	e_dash = 3,
	e_reflect = 4,
	e_pickup = 5,
	e_turnin = 6,
	e_damage = 7,
	e_impact = 8,
	e_sawcut = 9,
	e_sniper = 10,
	e_energy = 11,
	e_explosion = 12,
	e_death = 13
};

enum class soundAmbient
{
	e_background = 0,
	e_drone = 1,
	e_shield = 2,
	e_melee = 3,
	e_car = 4,
	e_path = 5
};

enum class soundMusic
{
	e_menu = 0,
	e_game = 1
};

enum class soundUI
{
	e_traverse = 0,
	e_front = 1,
	e_back = 2,
};

#define NR_EFFECT_SOUNDS 14
#define NR_AMBIENT_SOUNDS 3
#define NR_MUSIC_SOUNDS 2
#define NR_UI_SOUNDS 3
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
	std::unique_ptr<SoundEffect> m_ui[NR_UI_SOUNDS];

	std::unique_ptr<SoundEffectInstance> m_effectInstances[NR_EFFECT_INSTANCES];
	std::unique_ptr<SoundEffectInstance> m_ambientInstances[NR_AMBIENT_SOUNDS];
	std::unique_ptr<SoundEffectInstance> m_musicInstances[NR_MUSIC_SOUNDS];
	int m_index;

public:
	static Sound* getInstance();

	void play(soundUI sound, float volume = 1.0f, float pitch = 0.0f, float pan = 0.0f);
	void play(soundMusic sound, float volume = 1.0f, float pitch = 0.0f, float pan = 0.0f);
	void play(soundAmbient sound, float volume = 1.0f, float pitch = 0.0f, float pan = 0.0f);
	void play(soundAmbient sound, XMVECTOR pos, float volume = 1.0f, float falloff = 0.4f, float pitch = 0.0f, float pan = 0.0f);
	void play(soundEffect sound, XMVECTOR pos, float volume = 1.0f, float pitch = 0.0f, float pan = 0.0f);

	void stop(soundAmbient sound);
	void stop(soundMusic sound);

	void update(float dt);

	void release();
};

