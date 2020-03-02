#include "Sound.h"

Sound* Sound::m_instance = nullptr;

Sound::Sound()
{
	// TODO: ADD SUSPEND AND RESUME
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
	#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
	#endif
	m_audEngine = std::make_unique<AudioEngine>(eflags);

	m_listener.SetPosition(XMVectorSet(0, 0, 0, 0));

	m_effect[(int)soundEffect::e_pistol] = std::make_unique<SoundEffect>(m_audEngine.get(), L"Sounds/weakshot.wav");
	m_effect[(int)soundEffect::e_rifle] = std::make_unique<SoundEffect>(m_audEngine.get(), L"Sounds/quickshot.wav");
	m_effect[(int)soundEffect::e_dash] = std::make_unique<SoundEffect>(m_audEngine.get(), L"Sounds/woosh.wav");
	m_effect[(int)soundEffect::e_pickup] = std::make_unique<SoundEffect>(m_audEngine.get(), L"Sounds/pickup.wav");
	m_effect[(int)soundEffect::e_turnin] = std::make_unique<SoundEffect>(m_audEngine.get(), L"Sounds/turnin.wav");
	
	m_ambient[(int)soundAmbient::e_background] = std::make_unique<SoundEffect>(m_audEngine.get(), L"Sounds/City_Amb_01.wav");
	m_ambient[(int)soundAmbient::e_drone] = std::make_unique<SoundEffect>(m_audEngine.get(), L"Sounds/helicopter-hovering-01.wav");
	m_ambient[(int)soundAmbient::e_shield] = std::make_unique<SoundEffect>(m_audEngine.get(), L"Sounds/shield.wav");
	m_ambientInstances[(int)soundAmbient::e_background] = m_ambient[0]->CreateInstance();
	//m_ambient[1] = std::make_unique<SoundEffect>(m_audEngine.get(), L"Sounds/helicopter-hovering-01.wav");
	m_ambientInstances[(int)soundAmbient::e_drone] = m_ambient[(int)soundAmbient::e_drone]->CreateInstance(SoundEffectInstance_Use3D);
	m_ambientInstances[(int)soundAmbient::e_shield] = m_ambient[(int)soundAmbient::e_shield]->CreateInstance(SoundEffectInstance_Use3D);

	m_index = 0;
}

Sound* Sound::getInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new Sound();
	}

	return m_instance;
}

void Sound::play(soundMusic sound, float volume, float pitch, float pan)
{
	m_musicInstances[(int)sound]->Play(true);
}

void Sound::play(soundAmbient sound, float volume, float pitch, float pan)
{
	m_ambientInstances[(int)sound]->SetVolume(volume);
	m_ambientInstances[(int)sound]->SetPitch(pitch);
	m_ambientInstances[(int)sound]->SetPan(pan);

	m_ambientInstances[(int)sound]->Play(true);
}

void Sound::play(soundAmbient sound, XMVECTOR pos, float volume, float falloff, float pitch, float pan)
{
	pos = XMVectorSetY(pos, XMVectorGetY(pos) * 0.0f);
	AudioEmitter emitter;
	XMVECTOR dir = pos - m_listenerPos;
	float length = XMVectorGetX(XMVector3Length(dir));
	if (length < 1.0f)
		emitter.SetPosition(m_listenerPos);
	else
		emitter.SetPosition(m_listenerPos + XMVector3Normalize(dir) * powf(length, falloff));
		//emitter.SetPosition(pos);

	m_ambientInstances[(int)sound]->SetVolume(volume);
	m_ambientInstances[(int)sound]->SetPitch(pitch);
	m_ambientInstances[(int)sound]->SetPan(pan);

	m_ambientInstances[(int)sound]->Play(true);
	m_ambientInstances[(int)sound]->Apply3D(m_listener, emitter);
}

void Sound::play(soundEffect sound, XMVECTOR pos, float volume, float pitch, float pan)
{
	m_effectInstances[m_index] = m_effect[(int)sound]->CreateInstance(SoundEffectInstance_Use3D);
	XMVECTOR dir = pos - m_listenerPos;
	float length = XMVectorGetX(XMVector3Length(dir));
	if (length < 1.0f)
		m_emitter.SetPosition(m_listenerPos);
	else
		m_emitter.SetPosition(m_listenerPos + XMVector3Normalize(dir) * powf(length, 0.4f));
		//m_emitter.SetPosition(pos);

	m_effectInstances[m_index]->SetVolume(volume);
	m_effectInstances[m_index]->SetPitch(pitch);
	m_effectInstances[m_index]->SetPan(pan);

	m_effectInstances[m_index]->Stop();
	m_effectInstances[m_index]->Play();
	m_effectInstances[m_index]->Apply3D(m_listener, m_emitter);
	m_index = (m_index + 1) % 10;
}

void Sound::stop(soundAmbient sound)
{
	m_ambientInstances[(int)sound]->Stop(true);
}

void Sound::update3D(soundAmbient sound, XMVECTOR pos)
{
	XMVECTOR dir = pos - m_listenerPos;
	float length = XMVectorGetX(XMVector3Length(dir));
	if (length < 1.0f)
		m_emitter.SetPosition(m_listenerPos);
	else
		m_emitter.SetPosition(m_listenerPos + XMVector3Normalize(dir) * powf(length, 0.3f));
	m_ambientInstances[(int)sound]->Apply3D(m_listener, m_emitter);
}

void Sound::update(float dt)
{
	m_listenerPos = DX::getInstance()->getCam()->getLookAt();
	m_listener.SetPosition(m_listenerPos);
	if (!m_audEngine->Update())
	{
		// No audio device is active
		if (m_audEngine->IsCriticalError())
		{
			// TODO
		}
	}

	/*
	TODO for music later I think
	if (m_retryAudio)
	{
		m_retryAudio = false;

		if (m_audEngine->Reset())
		{
			// TODO: restart any looped sounds here
			if (m_nightLoop)
				m_nightLoop->Play(true);
		}
	}
	*/
}

void Sound::release()
{
	if (m_audEngine)
	{
		m_audEngine->Suspend();
	}
	m_ambientInstances[0].reset();
}
