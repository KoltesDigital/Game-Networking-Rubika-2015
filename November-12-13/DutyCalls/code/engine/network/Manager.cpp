#include "engine/network/Manager.h"

#include <cassert>
#include <iostream>
#include <enet/enet.h>
#include <SFML/Network/Packet.hpp>

#include "engine/network/Client.h"
#include "engine/network/EventListener.h"

namespace engine
{
	namespace network
	{
		Manager *Manager::instance = nullptr;

		Manager::Manager(EventListener *eventListener)
			: _eventListener(eventListener)
			, _host(nullptr)
			, _server(nullptr)
		{
			assert(eventListener);
		}

		bool Manager::initialize()
		{
			if (enet_initialize() != 0)
			{
				std::cerr << "An error occurred while initializing ENet." << std::endl;
				return false;
			}

			return true;
		}

		void Manager::deinitialize()
		{
			if (_server != nullptr)
				enet_peer_disconnect(_server, 0);

			enet_host_destroy(_host);
			enet_deinitialize();
		}

		bool Manager::createClient(const std::string &host, sf::Uint16 port)
		{
			_host = enet_host_create(
				NULL, // client host
				1, // one outgoing connection
				1, // one channel
				0, // automatic downstream bandwidth
				0 // automatic upstream bandwidth
				);
			if (_host == nullptr)
			{
				std::cerr << "An error occurred while trying to create an ENet client host." << std::endl;
				return false;
			}

			ENetAddress address;
			enet_address_set_host(&address, host.c_str());
			address.port = port;

			_server = enet_host_connect(_host, &address, 2, 0);
			if (_server == nullptr)
			{
				std::cerr << "No available peers for initiating an ENet connection." << std::endl;
				return false;
			}

			std::cout << "Connecting to host " << host << " on port " << port << "." << std::endl;
			return true;
		}

		bool Manager::createServer(sf::Uint16 port)
		{
			ENetAddress address;
			address.host = ENET_HOST_ANY;
			address.port = port;

			_host = enet_host_create(
				&address, // client host
				32, // client connections
				1, // one channel
				0, // automatic downstream bandwidth
				0 // automatic upstream bandwidth
				);
			if (_host == nullptr)
			{
				std::cerr << "An error occurred while trying to create an ENet server host on port " << port << "." << std::endl;
				return false;
			}

			std::cout << "Listening on port " << port << "." << std::endl;
			return true;
		}

		void Manager::broadcast(const sf::Packet &packet)
		{
			assert(_host);
			ENetPacket *netPacket = enet_packet_create(packet.getData(), packet.getDataSize(), 0);
			enet_host_broadcast(_host, 0, netPacket);
		}

		void Manager::send(const Client &client, const sf::Packet &packet)
		{
			assert(client.getPeer());
			ENetPacket *netPacket = enet_packet_create(packet.getData(), packet.getDataSize(), 0);
			enet_peer_send(client.getPeer(), 0, netPacket);
		}

		void Manager::update()
		{
			assert(_host);

			ENetEvent event;

			for (;;)
			{
				int serviceResponse = enet_host_service(_host, &event, 0);
				if (serviceResponse == 0)
					break;

				if (serviceResponse < 0) {
					std::cerr << "Event handling failed." << std::endl;
					break;
				}

				if (serviceResponse > 0)
				{
					switch (event.type)
					{
					case ENET_EVENT_TYPE_CONNECT:
					{
						Client *client = new Client(event.peer);
						event.peer->data = client;
						_eventListener->onConnected(*client);
						break;
					}

					case ENET_EVENT_TYPE_RECEIVE:
					{
						Client *client = reinterpret_cast<Client*>(event.peer->data);
						sf::Packet packet;
						packet.append(event.packet->data, event.packet->dataLength);
						_eventListener->onMessageReceived(*client, packet);
						enet_packet_destroy(event.packet);
						break;
					}

					case ENET_EVENT_TYPE_DISCONNECT:
					{
						Client *client = reinterpret_cast<Client*>(event.peer->data);
						_eventListener->onDisconnected(*client);
						delete client;
						break;
					}
					}
				}
			}
		}
	}
}
