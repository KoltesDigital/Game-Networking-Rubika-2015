#include "engine/gameplay/messages/CreateEntity.h"

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
			const util::StringId CreateEntity::CONTAINER_ID("CreateEntityMessage");

			CreateEntity::CreateEntity()
			{
				// empty
			}

			CreateEntity::CreateEntity(const gameplay::Entity *entity)
				: _entityTypeId(entity->getEntityTypeId())
				, _entityId(entity->id)
				, _ownerId(entity->ownerId)
			{
				// empty
			}

			util::StringId CreateEntity::getContainerId() const
			{
				return CONTAINER_ID;
			}

			void CreateEntity::declare(network::Protocol &protocol)
			{
				protocol.declare(_entityTypeId);
				protocol.declare(_entityId);
				protocol.declare(_ownerId);
			}

			void CreateEntity::process()
			{
				gameplay::Manager::instance->createEntity(_entityTypeId, _entityId, _ownerId);
			}
		}
	}
}
