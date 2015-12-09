#include "engine/network/Manager.h"

#include <cassert>
#include <iostream>
#include <enet/enet.h>
#include <SFML/Network/Packet.hpp>

#include "engine/network/Client.h"
#include "engine/network/EventListener.h"
#include "engine/network/Message.h"
#include "engine/network/Protocol.h"
#include "engine/network/async/HTTPTask.h"

namespace engine
{
	namespace network
	{
		Manager *Manager::instance = nullptr;

		Manager::Manager()
			: httpClient("localhost", 3000)
			, _eventListener(nullptr)
			, _host(nullptr)
			, _server(nullptr)
		{
			// empty
		}

		bool Manager::initialize(EventListener *eventListener)
		{
			assert(eventListener);
			_eventListener = eventListener;

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
		
		bool Manager::isServer() const
		{
			return !_server;
		}

		void Manager::broadcast(Message &message, bool reliable)
		{
			assert(_host);
			sf::Packet packet = createPacket(message);
			ENetPacket *netPacket = enet_packet_create(packet.getData(), packet.getDataSize(), reliable ? ENET_PACKET_FLAG_RELIABLE : 0);
			enet_host_broadcast(_host, 0, netPacket);
		}

		void Manager::send(const Client &client, Message &message, bool reliable)
		{
			assert(client.getPeer());
			sf::Packet packet = createPacket(message);
			ENetPacket *netPacket = enet_packet_create(packet.getData(), packet.getDataSize(), reliable ? ENET_PACKET_FLAG_RELIABLE : 0);
			enet_peer_send(client.getPeer(), 0, netPacket);
		}

		void Manager::runTask(async::HTTPTask *task)
		{
			assert(task);
			_tasks.push_back(task);
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
						Message *message = _messageFactory.createMessage(packet);
						assert(message);
						message->process();
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

			_tasks.remove_if([](async::HTTPTask *task)
			{
				task->update();
				if (task->isTerminated())
				{
					delete task;
					return true;
				}
				return false;
			});
		}

		sf::Packet Manager::createPacket(Message &message)
		{
			sf::Packet packet;
			packet << message.getContainerId();

			Protocol protocol;
			message.declare(protocol);
			protocol.serialize(packet);

			return packet;
		}
	}
}
