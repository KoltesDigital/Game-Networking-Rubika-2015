#pragma once

#include "engine/util/StringId.h"
#include "engine/network/data/Container.h"
#include "engine/network/data/Plain.h"
#include "engine/network/data/Value.h"

namespace engine
{
	namespace network
	{
		namespace data
		{
			class ContainerWrapper : public Value
			{
			public:
				ContainerWrapper();
				ContainerWrapper(Container &container);

				operator Container &();

				virtual void serialize(sf::Packet &packet) override;
				virtual void deserialize(sf::Packet &packet) override;

			private:
				StringId _containerId;
				Container *_container;
			};
		}
	}
}
