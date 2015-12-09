#include "engine/network/data/Container.h"
#include "engine/network/Protocol.h"

namespace engine
{
	namespace network
	{
		namespace data
		{
			void Container::serialize(sf::Packet &packet)
			{
				Protocol protocol;
				declare(protocol);
				protocol.serialize(packet);
			}

			void Container::deserialize(sf::Packet &packet)
			{
				Protocol protocol;
				declare(protocol);
				protocol.deserialize(packet);
			}
		}
	}
}
