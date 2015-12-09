#pragma once

#include <functional>
#include <map>
#include "engine/util/StringId.h"

namespace engine
{
	namespace network
	{
		namespace data
		{
			class Container;
		}

		class ContainerFactory
		{
		public:
			ContainerFactory();
			data::Container *createContainer(util::StringId containerId) const;

		private:
			typedef std::function<data::Container*()> TypeConstructor;
			std::map<util::StringId, TypeConstructor> _typeConstructors;
		};
	}
}
