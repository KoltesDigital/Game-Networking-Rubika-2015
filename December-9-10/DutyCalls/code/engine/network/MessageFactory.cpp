#include "engine/network/MessageFactory.h"

#include <cassert>
#include <SFML/Network/Packet.hpp>

#include "engine/network/Protocol.h"
#include "engine/gameplay/messages/AssignClientId.h"
#include "engine/gameplay/messages/CreateEntity.h"
#include "engine/gameplay/messages/DestroyEntity.h"
#include "engine/gameplay/messages/UpdateEntity.h"

namespace engine
{
	namespace network
	{
		MessageFactory::MessageFactory()
		{
#define DECLARE(TYPE) \
			_typeConstructors.insert(std::pair<util::StringId, TypeConstructor>(gameplay::messages::TYPE::CONTAINER_ID, []() { \
				return new gameplay::messages::TYPE(); \
			}));

			DECLARE(AssignClientId);

			DECLARE(CreateEntity);
			DECLARE(DestroyEntity);
			DECLARE(UpdateEntity);

#undef DECLARE
		}

		Message *MessageFactory::createMessage(sf::Packet &packet) const
		{
			util::StringId type;
			packet >> type;

			const TypeConstructor &typeConstructor = _typeConstructors.at(type);
			assert(typeConstructor);
			Message *message = typeConstructor();

			Protocol protocol;
			message->declare(protocol);
			protocol.deserialize(packet);

			return message;
		}
	}
}
