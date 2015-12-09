#pragma once

#include <SFML/Config.hpp>
#include "engine/network/data/Container.h"

namespace sf
{
	class Packet;
}

namespace engine
{
	namespace network
	{
		class Message : public data::Container
		{
		public:
			virtual void process() = 0;
		};
	}
}
