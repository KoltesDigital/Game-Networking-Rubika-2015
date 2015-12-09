#pragma once

namespace engine
{
	namespace network
	{
		class Client;
		class Message;

		class EventListener
		{
		public:
			virtual void onConnected(Client &client) = 0;
			virtual void onDisconnected(Client &client) = 0;
		};
	}
}
