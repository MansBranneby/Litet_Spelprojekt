#include "GameState.h"

void GameState::handleMovement(Game* game, float dt, int id)
{
	// Robot movement
	game->getRobots()[id]->move(
		XMVectorSet(game->getInput()->getThumbLX(id), 0.0f, game->getInput()->getThumbLY(id), 0.0f) *
		game->getRobots()[id]->getVelocity() * dt * (game->getInput()->getTriggerL(id) + 0.2f) * 8 // TODO remove trigger
	);

	//Weapon movement
	std::vector<Weapon*> weapons = game->getRobots()[id]->getWeapons();

	weapons[game->getRobots()[id]->getCurrentWeapon(RIGHT)]->setPosition(
		weapons[game->getRobots()[id]->getCurrentWeapon(RIGHT)]->getRelativePos()
	);

	//game->getRobots()[id]->getWeapons()[game->getRobots()[id]->getCurrentWeapon(RIGHT)]->setPosition(
	//	game->getRobots()[id]->getWeapons()[game->getRobots()[id]->getCurrentWeapon(RIGHT)]->getRelativePos()
	//);

	//weapons[game->getRobots()[id]->getCurrentWeapon(RIGHT)]->setPositionRelative(
	//	game->getRobots()[id]->getPosition()
	//);

	//if (game->getRobots()[id]->getCurrentWeapon(LEFT) != -1)
	//{
	//	weapons[game->getRobots()[id]->getCurrentWeapon(LEFT)]->setPosition(
	//		game->getRobots()[id]->getPosition()
	//	);
	//
	//	weapons[game->getRobots()[id]->getCurrentWeapon(LEFT)]->setPositionRelative(
	//		weapons[game->getRobots()[id]->getCurrentWeapon(LEFT)]->getRelativePos()
	//	);
	//}

	// Rotation
	if (abs(game->getInput()->getThumbRX(id)) > 0.1f || abs(game->getInput()->getThumbRY(id)) > 0.1f)
	{
		game->getRobots()[id]->setCurrentRot(XMConvertToDegrees(atan2(game->getInput()->getThumbRX(id), game->getInput()->getThumbRY(id))));

		game->getRobots()[id]->setRotation(0, 1, 0, game->getRobots()[id]->getCurrentRot());

		//weapons[game->getRobots()[id]->getCurrentWeapon(RIGHT)]->setRotation(0, 1, 0, game->getRobots()[id]->getCurrentRot());
		//if (game->getRobots()[id]->getCurrentWeapon(LEFT) != -1)
		//{
		//	weapons[game->getRobots()[id]->getCurrentWeapon(LEFT)]->setRotation(0, 1, 0, game->getRobots()[id]->getCurrentRot());
		//}
	}

	// Projectile movement
	for (int i = 0; i < m_projectiles.size(); i++)
	{
		m_projectiles[i]->move(m_projectiles[i]->getDirection() * dt);
	}
}

void GameState::handleInputs(Game* game, float dt)
{
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (game->getRobots()[i] != nullptr)
		{
			if (!game->getInput()->refresh(i))
			{
				//OutputDebugStringA("Please reconnect controller\n");
				game->getInput()->reconnectController(i);
			}
			else
			{
				std::vector<Weapon*> weapons = game->getRobots()[i]->getWeapons();

				handleMovement(game, dt, i);

				if (game->getInput()->getTriggerR(i) > 0.2 && weapons[game->getRobots()[i]->getCurrentWeapon(RIGHT)]->shoot())
				{

					Projectile* bullet = new Projectile(weapons[game->getRobots()[i]->getCurrentWeapon(RIGHT)]->getType(), weapons[game->getRobots()[i]->getCurrentWeapon(RIGHT)]->getDamage());

					float rotInRad = XMConvertToRadians(game->getRobots()[i]->getCurrentRot());

					bullet->setPosition(
						XMVector3Rotate(
							XMVectorSet(
								XMVectorGetX(weapons[game->getRobots()[i]->getCurrentWeapon(RIGHT)]->getRelativePos()),
								XMVectorGetY(weapons[game->getRobots()[i]->getCurrentWeapon(RIGHT)]->getRelativePos()),
								0.0f, 0.0f),
							XMVectorSet(0, sin(rotInRad / 2), 0, cos(rotInRad / 2))
						) + game->getRobots()[i]->getPosition()
					);

					bullet->setRotation(0.0, 1.0, 0.0f, game->getRobots()[i]->getCurrentRot());

					// TODO add recoil here 
					bullet->setDirection(XMVector3Cross(bullet->getPosition() - game->getRobots()[i]->getPosition(), XMVectorSet(0, 1, 0, 0)));
					m_projectiles.push_back(bullet);
				}

				/*
				if (input.getTriggerL(i) > 0.2 && weapons[game->getRobots()[i]->getCurrentWeapon(LEFT)]->shoot())
				{

					Projectile* bullet = new Projectile(weapons[game->getRobots()[i]->getCurrentWeapon(LEFT)]->getType(), weapons[game->getRobots()[i]->getCurrentWeapon(RIGHT)]->getDamage());

					float rotInRad = XMConvertToRadians(game->getRobots()[i]->getCurrentRot());

					bullet->setPosition(
						XMVector3Rotate(
							XMVectorSet(
								XMVectorGetX(weapons[game->getRobots()[i]->getCurrentWeapon(LEFT)]->getRelativePos()),
								XMVectorGetY(weapons[game->getRobots()[i]->getCurrentWeapon(LEFT)]->getRelativePos()),
								0.0f, 0.0f),
							XMVectorSet(0, sin(rotInRad / 2), 0, cos(rotInRad / 2))
						) + game->getRobots()[i]->getPosition()
					);

					bullet->setDirection(XMVector3Cross(bullet->getPosition() - game->getRobots()[i]->getPosition(), XMVectorSet(0, 1, 0, 0)));
					m_projectiles.push_back(bullet);
				}
				*/
				//if (game->getInput()->isPressed(i, XINPUT_GAMEPAD_A))
				//{
				//	OutputDebugStringA("A\n");
				//}
			}
		}
	}
}

GameState::GameState()
{
	m_type = stateType::e_gameState;
	m_transparency.initialize();
	m_transparency.bindConstantBuffer();
	m_lights = Lights::getInstance();
	int index = m_lights->addPointLight(-10, 25, 0, 55, 1, 0.5f, 0.125f, 1);
	m_lights->setColor(index, float(255) / 255, float(0) / 255, float(97) / 255);
	index = m_lights->addDirectionalLight(0.5f, -1, 0.5f, 0.6f, 0.6f, 1.0f, 4.0f);
	index = m_lights->addSpotLight(-1.5f, 6, -40, 10, -0.33f, -1, 0.0f, 1.0f, 1.0f, 0.0f, 27, 20);
	index = m_lights->addSpotLight(1.5f, 6, -40, 10, 0.33f, -1, 0.0f, 1.0f, 1.0f, 0.0f, 27, 20);
	m_lights->addPointLight(-31, 6, -43, 10, 1, 1, 0, 5);
	m_lights->addPointLight(27.5f, 4, -36, 10, 1, 0, 1, 5);
	m_lights->addPointLight(50, 15.0, 42, 30, 1, 0.5f, 0, 25);
	m_lights->addPointLight(-3, 10.0, 45, 20, 0, 1, 1, 10);
	m_lights->addPointLight(20, 5.0, 40, 30, 0, 0, 1, 15);
	m_lights->addPointLight(107, 5.0, 40, 30, 1, 1, 0, 20);
	m_lights->addPointLight(90, 5.0, 33, 10, 1, 0, 0, 20);
	m_lights->addPointLight(-40, 6.0, -1, 30, 1, 1, 0.6f, 15);
	m_lights->addPointLight(-71.5, 1.0, 59.5, 10, 1, 0.6f, 0, 10);

	//m_lights->setColor(index, float(19) / 255, float(62) / 255, float(124) / 255);
	/*
	index = m_lights->addSpotLight(-35, 30, -5, 50, -0.3f, -1, 0.3f, 1.0f, 0.9f, 0.9f, 25, 1);
	m_lights->setColor(index, float(234) / 255, float(185) / 255, float(217) / 255);
	index = m_lights->addSpotLight(0, 5, -45, 50, 0, -0.5f, 1.0f, 1.0f, 0.9f, 0.9f, 25, 1);
	m_lights->setColor(index, float(234) / 255, float(185) / 255, float(217) / 255);*/
	
	
}

GameState::~GameState()
{
	for (int i = 0; i < m_projectiles.size(); i++)
	{
		delete m_projectiles[i];
	}
}

void GameState::pause()
{
}

void GameState::resume()
{
}

void GameState::handleInput(Game* game)
{
}

void GameState::update(Game* game, float dt)
{
	handleInputs(game, dt);
	game->updatePlayerStatus();

	Robot** robots = game->getRobots();
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (robots[i] != nullptr)
		{
			robots[i]->update();

			std::vector<Weapon*> weapons = robots[i]->getWeapons();
			weapons[robots[i]->getCurrentWeapon(RIGHT)]->updateTime(dt);
			if (robots[i]->getCurrentWeapon(LEFT) != -1)
			{
				weapons[robots[i]->getCurrentWeapon(LEFT)]->updateTime(dt);
			}
		}
	}
	if (robots[0] != nullptr) {
		XMVECTOR position = robots[0]->getPosition();
		m_lights->setPosition(0, position.m128_f32[0], position.m128_f32[1], position.m128_f32[2]);
	}
	
	// TODO remove with collision instead aswell as game field?
	for (int i = 0; i < m_projectiles.size(); i++)
	{
		if (XMVectorGetX(XMVector3Length(m_projectiles[i]->getPosition())) > 50.0f)
		{
			delete m_projectiles[i];
			m_projectiles.erase(m_projectiles.begin() + i);
		}
	}
	returnInfo info;
	if (game->getRobots()[0] != nullptr) {
		XMVECTOR pos = game->getRobots()[0]->getPosition();
		info.x = pos.m128_f32[0];
		info.y = pos.m128_f32[1];
		info.z = pos.m128_f32[2];
	}
	else {
		info.x = 0;
		info.y = 0;
		info.z = 0;
	}
}

void GameState::draw(Game* game, renderPass pass)
{
	if (pass != renderPass::e_transparent)
	{

		for (int i = 0; i < XUSER_MAX_COUNT; i++)
		{
			if (game->getRobots()[i] != nullptr)
			{
				std::vector<Weapon*> weapons = game->getRobots()[i]->getWeapons();

				game->getPreLoader()->draw(objectType::e_robot, game->getRobots()[i]->getData());
				game->getPreLoader()->draw(objectType::e_weapon, weapons[game->getRobots()[i]->getCurrentWeapon(RIGHT)]->getData(), game->getRobots()[i]->getData());

				if (game->getRobots()[i]->getCurrentWeapon(LEFT) != -1)
				{
					game->getPreLoader()->draw(objectType::e_weapon, weapons[game->getRobots()[i]->getCurrentWeapon(LEFT)]->getData(), game->getRobots()[i]->getData());
				}
			}
		}
		
	}
	for (int i = 0; i < m_projectiles.size(); i++)
	{
		game->getPreLoader()->draw(objectType::e_projectile, m_projectiles[i]->getData());
	}
	if (pass != renderPass::e_opaque)
	{
		game->getPreLoader()->draw(objectType::e_scene);
	}
}
