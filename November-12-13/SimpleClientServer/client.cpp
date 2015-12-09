
#include <cassert>
#include <iostream>
#include <string>

#include <enet/enet.h>

int main()
{
	if (enet_initialize() != 0)
	{
		std::cerr << "An error occurred while initializing ENet." << std::endl;
		return EXIT_FAILURE;
	}

	ENetHost *client;
	client = enet_host_create(
		NULL, // client host
		1, // one outgoing connection
		1, // one channel
		0, // automatic downstream bandwidth
		0 // automatic upstream bandwidth
	);
	if (client == nullptr)
	{
		std::cerr << "An error occurred while trying to create an ENet client host." << std::endl;
		return EXIT_FAILURE;
	}

	const char* serverHost = "127.0.0.1";
	const int serverPort = 3000;

	ENetAddress address;
	enet_address_set_host(&address, serverHost);
	address.port = serverPort;

	ENetPeer *server = enet_host_connect(client, &address, 2, 0);
	if (server == nullptr)
	{
		std::cerr << "No available peers for initiating an ENet connection." << std::endl;
		return EXIT_FAILURE;
	}

	ENetEvent networkEvent;

	// Wait up to 1 second for the connection attempt to succeed.
	if (enet_host_service(client, &networkEvent, 1000) > 0 &&
		networkEvent.type == ENET_EVENT_TYPE_CONNECT)
	{
		std::cerr << "Connection to " << serverHost << ":" << serverPort << " successful." << std::endl;
	}
	else
	{
		enet_peer_reset(server);
		std::cerr << "Connection to " << serverHost << ":" << serverPort << " failed." << std::endl;
		return EXIT_FAILURE;
	}

	// const char* message = "hello";
	// ENetPacket *packet = enet_packet_create(message, strlen(message), 0);
	std::string message("hello");
	ENetPacket *packet = enet_packet_create(message.c_str(), message.length(), 0);
	enet_peer_send(server, 0, packet);

	bool connected = true;
	while (connected)
	{
		int serviceResponse = enet_host_service(client, &networkEvent, 0);
		if (serviceResponse < 0) {
			std::cerr << "Event handling failed." << std::endl;
		}

		if (serviceResponse > 0)
		{
			switch (networkEvent.type)
			{
			case ENET_EVENT_TYPE_RECEIVE:
				std::cout
					<< "Received "
					<< networkEvent.packet->dataLength
					<< " bytes on channel "
					<< static_cast<int>(networkEvent.channelID)
					<< ": "
					<< std::string(reinterpret_cast<char*>(networkEvent.packet->data), networkEvent.packet->dataLength)
					<< "."
					<< std::endl;

				// Clean up the packet!
				enet_packet_destroy(networkEvent.packet);

				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				std::cout << "Disconnected." << std::endl;
				connected = false;
				break;
			}
		}
	}

	enet_host_destroy(client);
	enet_deinitialize();

	return EXIT_SUCCESS;
}
