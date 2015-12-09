#pragma once

#include <list>
#include <string>
#include <SFML/Config.hpp>
#include <SFML/Network/Http.hpp>

#include "engine/util/Guid.h"
#include "engine/network/MessageFactory.h"
#include "engine/network/services/Leaderboard.h"

struct _ENetHost;
struct _ENetPeer;
typedef struct _ENetHost ENetHost;
typedef struct _ENetPeer ENetPeer;

namespace engine
{
	namespace network
	{
		class Client;
		class EventListener;
		class Message;

		namespace async
		{
			class HTTPTask;
		}

		class Manager
		{
		public:
			util::Guid localId;

			sf::Http httpClient;
			services::Leaderboard leaderboardService;

			Manager();

			bool initialize(EventListener *eventListener);
			void deinitialize();

			bool createClient(const std::string &host, sf::Uint16 port);
			bool createServer(sf::Uint16 port);
			bool isServer() const;

			void broadcast(Message &message, bool reliable);
			void send(const Client &client, Message &message, bool reliable);

			void runTask(async::HTTPTask *task);

			void update();

			static Manager *instance;
			
		protected:
			static sf::Packet createPacket(Message &message);

		private:
			EventListener *_eventListener;
			ENetHost *_host;
			ENetPeer *_server;
			MessageFactory _messageFactory;
			std::list<async::HTTPTask *> _tasks;
		};
	}
}
