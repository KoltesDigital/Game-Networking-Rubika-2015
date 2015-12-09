#include "engine/network/Client.h"

#include <enet/enet.h>

namespace engine
{
	namespace network
	{
		Client::Client(ENetPeer *peer)
			: _peer(peer)
		{
			// empty
		}

		ENetPeer *Client::getPeer() const
		{
			return _peer;
		}
	}
}
