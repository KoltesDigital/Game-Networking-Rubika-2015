#pragma once

#include "engine/util/StringId.h"
#include "engine/network/data/Value.h"
#include "engine/network/Protocol.h"

namespace engine
{
	namespace network
	{
		class Protocol;

		namespace data
		{
			class Container : public Value
			{
			public:
				virtual util::StringId getContainerId() const = 0;

				virtual void serialize(sf::Packet &packet) override;
				virtual void deserialize(sf::Packet &packet) override;

				virtual void declare(Protocol &protocol) = 0;
			};
		}
	}
}
