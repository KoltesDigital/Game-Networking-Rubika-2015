#pragma once

#include <list>
#include <set>
#include "engine/graphics/EventListener.h"
#include "engine/network/EventListener.h"
#include "engine/gameplay/PlayerEntity.h"

namespace engine
{
	namespace graphics
	{
		class Manager;
	}

	namespace network
	{
		class Manager;
	}

	class Engine : public graphics::EventListener, public network::EventListener
	{
	public:
		enum class InstanceType
		{
			CLIENT,
			SERVER,
		};

		bool initialize();
		void deinitialize();

		bool createClient(const std::string &host, sf::Uint16 port);
		bool createServer(sf::Uint16 port);

		void run();
		float getDeltaTime() const;

		// graphics::EventListener
		virtual void onWindowClosed();

		// network::EventListener
		virtual void onConnected(network::Client &client);
		virtual void onDisconnected(network::Client &client);
		virtual void onMessageReceived(network::Client &client, sf::Packet &packet);

		static Engine *instance;

	private:
		InstanceType _instanceType;
		bool _running;
		float _deltaTime;

		std::list<gameplay::PlayerEntity*> _entities;
	};
}
