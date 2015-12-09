#include "engine/gameplay/messages/DestroyEntity.h"

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
			const util::StringId DestroyEntity::CONTAINER_ID("DestroyEntityMessage");

			DestroyEntity::DestroyEntity()
			{
				// empty
			}

			DestroyEntity::DestroyEntity(const gameplay::Entity *entity)
				: _entityId(entity->id)
			{
				// empty
			}

			util::StringId DestroyEntity::getContainerId() const
			{
				return CONTAINER_ID;
			}

			void DestroyEntity::declare(network::Protocol &protocol)
			{
				protocol.declare(_entityId);
			}

			void DestroyEntity::process()
			{
				gameplay::Manager::instance->destroyEntity(_entityId);
			}
		}
	}
}
