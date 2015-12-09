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

	const int serverPort = 3000;

	ENetAddress address;
	address.host = ENET_HOST_ANY;
	address.port = serverPort;

	ENetHost *server;
	server = enet_host_create(
		&address, // client host
		32, // client connections
		1, // one channel
		0, // automatic downstream bandwidth
		0 // automatic upstream bandwidth
		);
	if (server == nullptr)
	{
		std::cerr << "An error occurred while trying to create an ENet server host on port " << serverPort << "." << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "Listening on port " << serverPort << "." << std::endl;

	ENetEvent event;

	// Main loop
	bool running = true;
	while (running)
	{
		int serviceResponse = enet_host_service(server, &event, 0);
		if (serviceResponse < 0) {
			std::cerr << "Event handling failed." << std::endl;
		}

		if (serviceResponse > 0)
		{
			char hostIp[32];
			enet_address_get_host_ip(&event.peer->address, hostIp, sizeof(hostIp));
			std::cout << hostIp << ":" << event.peer->address.port << " ";

			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				std::cout << "Connected." << std::endl;

				// Store any relevant client information here.
				event.peer->data = "Client information";

				break;

			case ENET_EVENT_TYPE_RECEIVE:
				std::cout
					<< "Received "
					<< event.packet->dataLength
					<< " bytes on channel "
					<< static_cast<int>(event.channelID)
					<< ": "
					<< std::string(reinterpret_cast<char*>(event.packet->data), event.packet->dataLength)
					<< "."
					<< std::endl;

				// Send the packet back
				enet_peer_send(event.peer, 0, event.packet);
				
				// Or broadcast the packet to all clients
				// enet_host_broadcast(server, 0, event.packet);

				// Or clean up the packet!
				// enet_packet_destroy(event.packet);

				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				std::cout << "Disconnected." << std::endl;

				// Reset the peer's client information.
				event.peer->data = NULL;

				break;
			}
		}
	}
	
	enet_host_destroy(server);
	enet_deinitialize();

	return EXIT_SUCCESS;
}
