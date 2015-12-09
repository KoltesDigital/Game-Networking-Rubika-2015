#pragma once

namespace sf
{
	class Packet;
}

namespace engine
{
	namespace network
	{
		namespace data
		{
			class Value
			{
			public:
				virtual void serialize(sf::Packet &packet) = 0;
				virtual void deserialize(sf::Packet &packet) = 0;
			};
		}
	}
}
