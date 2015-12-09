#include "engine/gameplay/EntityFactory.h"

#include <cassert>

#include "engine/gameplay/entities/Player.h"

namespace engine
{
	namespace gameplay
	{
		EntityFactory::EntityFactory()
		{
#define DECLARE(TYPE) \
			_typeConstructors.insert(std::pair<util::StringId, TypeConstructor>(entities::TYPE::ENTITY_TYPE_ID, []() { \
				return new entities::TYPE(); \
			}));

			DECLARE(Player);

#undef DECLARE
		}

		Entity *EntityFactory::createEntity(util::StringId entityTypeId) const
		{
			const TypeConstructor &typeConstructor = _typeConstructors.at(entityTypeId);
			assert(typeConstructor);
			Entity *entity = typeConstructor();
			return entity;
		}
	}
}
