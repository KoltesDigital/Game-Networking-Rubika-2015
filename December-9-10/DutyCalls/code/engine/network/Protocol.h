#pragma once

#include <list>

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
			class Value;
		}

		class Protocol
		{
		public:
			void declare(data::Value &value);

			void serialize(sf::Packet &packet) const;
			void deserialize(sf::Packet &packet) const;

		private:
			std::list<data::Value*> _values;
		};
	}
}
