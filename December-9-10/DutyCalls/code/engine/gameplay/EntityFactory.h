#pragma once

#include <functional>
#include <map>
#include "engine/util/StringId.h"

namespace engine
{
	namespace gameplay
	{
		class Entity;

		class EntityFactory
		{
		public:
			EntityFactory();
			Entity *createEntity(util::StringId entityTypeId) const;

		private:
			typedef std::function<Entity*()> TypeConstructor;
			std::map<util::StringId, TypeConstructor> _typeConstructors;
		};
	}
}
