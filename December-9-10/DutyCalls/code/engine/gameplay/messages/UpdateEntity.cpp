#include "engine/gameplay/messages/UpdateEntity.h"

#include "engine/Engine.h"
#include "engine/network/Protocol.h"
#include "engine/gameplay/Entity.h"
#include "engine/gameplay/Manager.h"

namespace engine
{
	namespace gameplay
	{
		namespace messages
		{
			const util::StringId UpdateEntity::CONTAINER_ID("UpdateEntityMessage");

			UpdateEntity::UpdateEntity()
			{
				// empty
			}

			UpdateEntity::UpdateEntity(const gameplay::Entity *entity, network::data::Container &container)
				: _entityId(entity->id)
				, _container(container)
			{
				// empty
			}

			util::StringId UpdateEntity::getContainerId() const
			{
				return CONTAINER_ID;
			}

			void UpdateEntity::declare(network::Protocol &protocol)
			{
				protocol.declare(_entityId);
				protocol.declare(_container);
			}

			void UpdateEntity::process()
			{
				gameplay::Manager::instance->updateEntity(_entityId, _container);
			}
		}
	}
}
