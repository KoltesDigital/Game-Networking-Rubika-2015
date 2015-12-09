#pragma once

#include <functional>
#include <map>
#include "engine/util/StringId.h"

namespace sf
{
	class Packet;
}

namespace engine
{
	namespace network
	{
		class Message;

		class MessageFactory
		{
		public:
			MessageFactory();
			Message *createMessage(sf::Packet &packet) const;

		private:
			typedef std::function<Message*()> TypeConstructor;
			std::map<util::StringId, TypeConstructor> _typeConstructors;
		};
	}
}
