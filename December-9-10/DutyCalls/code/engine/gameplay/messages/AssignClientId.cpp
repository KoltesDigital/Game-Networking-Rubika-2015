#include "engine/gameplay/messages/AssignClientId.h"

#include "engine/network/Manager.h"

namespace engine
{
	namespace gameplay
	{
		class Entity;

		namespace messages
		{
			const util::StringId AssignClientId::CONTAINER_ID("AssignClientIdMessage");

			AssignClientId::AssignClientId()
			{
				// empty
			}

			AssignClientId::AssignClientId(util::Guid clientId)
				: _clientId(clientId)
			{
				// empty
			}

			util::StringId AssignClientId::getContainerId() const
			{
				return CONTAINER_ID;
			}

			void AssignClientId::declare(network::Protocol &protocol)
			{
				protocol.declare(_clientId);
			}

			void AssignClientId::process()
			{
				network::Manager::instance->localId = _clientId;
			}
		}
	}
}
