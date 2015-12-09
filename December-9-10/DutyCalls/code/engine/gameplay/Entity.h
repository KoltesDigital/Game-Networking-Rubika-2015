#pragma once

#include "engine/util/Guid.h"
#include "engine/util/StringId.h"
#include "engine/network/data/Container.h"

namespace engine
{
	namespace gameplay
	{
		class Entity
		{
		public:
			util::Guid id;
			util::Guid ownerId;
			
			virtual util::StringId getEntityTypeId() const = 0;

			virtual void readContainer(const network::data::Container &container) = 0;
			virtual void writeContainer(network::data::Container &container) const = 0;

			virtual void update() = 0;
		};
	}
}
