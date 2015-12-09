#include "engine/Engine.h"

#include <cassert>
#include <SFML/System.hpp>

#include "engine/graphics/Manager.h"
#include "engine/network/Client.h"
#include "engine/network/Manager.h"
#include "engine/util/Guid.h"

namespace engine
{
	Engine *Engine::instance = nullptr;

	bool Engine::initialize()
	{
		assert(!graphics::Manager::instance);
		assert(!network::Manager::instance);

		network::Manager::instance = new network::Manager(this);
		graphics::Manager::instance = new graphics::Manager(this);

		if (!network::Manager::instance->initialize())
			return false;

		if (!graphics::Manager::instance->initialize())
			return false;

		return true;
	}

	void Engine::deinitialize()
	{
		for (gameplay::PlayerEntity *entity : _entities)
			delete entity;
		_entities.clear();

		assert(graphics::Manager::instance);
		assert(network::Manager::instance);

		graphics::Manager::instance->deinitialize();
		network::Manager::instance->deinitialize();

		delete graphics::Manager::instance;
		delete network::Manager::instance;
	}

	bool Engine::createClient(const std::string &host, sf::Uint16 port)
	{
		gameplay::PlayerEntity *entity = new gameplay::PlayerEntity();
		_entities.push_back(entity);

		assert(graphics::Manager::instance);
		assert(network::Manager::instance);
		graphics::Manager::instance->setWindowTitle("Client");
		return network::Manager::instance->createClient(host, port);
	}

	bool Engine::createServer(sf::Uint16 port)
	{
		assert(graphics::Manager::instance);
		assert(network::Manager::instance);
		graphics::Manager::instance->setWindowTitle("Server");
		return network::Manager::instance->createServer(port);
	}

	void Engine::run()
	{
		_running = true;

		sf::Clock clock;
		while (_running)
		{
			_deltaTime = clock.restart().asSeconds();

			network::Manager::instance->update();

			for (gameplay::PlayerEntity *entity : _entities)
				entity->update();

			graphics::Manager::instance->update();
		}
	}

	float Engine::getDeltaTime() const
	{
		return _deltaTime;
	}

	void Engine::onWindowClosed()
	{
		_running = false;
	}

	void Engine::onConnected(network::Client &client)
	{

	}

	void Engine::onDisconnected(network::Client &client)
	{

	}

	void Engine::onMessageReceived(network::Client &client, sf::Packet &packet)
	{
		
	}
}
