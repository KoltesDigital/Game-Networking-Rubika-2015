#include "engine/network/ContainerFactory.h"

#include <cassert>
#include "engine/network/Protocol.h"
#include "engine/gameplay/entities/Player.h"

namespace engine
{
	namespace network
	{
		ContainerFactory::ContainerFactory()
		{
#define DECLARE(TYPE) \
			_typeConstructors.insert(std::pair<util::StringId, TypeConstructor>(gameplay::entities::TYPE::CONTAINER_ID, []() { \
				return new gameplay::entities::TYPE(); \
			}));

			DECLARE(Player::PositionContainer);

#undef DECLARE
		}

		data::Container *ContainerFactory::createContainer(util::StringId containerId) const
		{
			const TypeConstructor &typeConstructor = _typeConstructors.at(containerId);
			assert(typeConstructor);
			data::Container *container = typeConstructor();
			return container;
		}
	}
}
