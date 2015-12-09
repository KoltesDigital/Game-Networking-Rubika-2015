#pragma once

#include <SFML/Network/Packet.hpp>

namespace engine
{
	namespace network
	{
		class Client;

		class EventListener
		{
		public:
			virtual void onConnected(Client &client) = 0;
			virtual void onDisconnected(Client &client) = 0;
			virtual void onMessageReceived(Client &client, sf::Packet &packet) = 0;
		};
	}
}
