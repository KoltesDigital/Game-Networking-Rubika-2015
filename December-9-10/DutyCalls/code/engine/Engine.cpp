#include "engine/Engine.h"

#include <cassert>
#include <SFML/System.hpp>

#include "engine/gameplay/Manager.h"
#include "engine/graphics/Manager.h"
#include "engine/input/Manager.h"
#include "engine/network/Client.h"
#include "engine/network/Manager.h"
#include "engine/util/Guid.h"

namespace engine
{
	Engine *Engine::instance = nullptr;

	bool Engine::initialize()
	{
		assert(!gameplay::Manager::instance);
		assert(!graphics::Manager::instance);
		assert(!input::Manager::instance);
		assert(!network::Manager::instance);

		network::Manager::instance = new network::Manager();
		graphics::Manager::instance = new graphics::Manager(this);
		input::Manager::instance = new input::Manager();
		gameplay::Manager::instance = new gameplay::Manager();

		if (!network::Manager::instance->initialize(gameplay::Manager::instance))
			return false;

		if (!graphics::Manager::instance->initialize())
			return false;

		return true;
	}

	void Engine::deinitialize()
	{
		assert(gameplay::Manager::instance);
		assert(graphics::Manager::instance);
		assert(input::Manager::instance);
		assert(network::Manager::instance);
		
		gameplay::Manager::instance->deinitialize();
		graphics::Manager::instance->deinitialize();
		network::Manager::instance->deinitialize();

		delete gameplay::Manager::instance;
		delete input::Manager::instance;
		delete graphics::Manager::instance;
		delete network::Manager::instance;
	}

	bool Engine::createClient(const std::string &host, sf::Uint16 port)
	{
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

			gameplay::Manager::instance->update();

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
}
