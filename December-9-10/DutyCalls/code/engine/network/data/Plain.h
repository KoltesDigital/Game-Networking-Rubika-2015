#pragma once

#include "engine/util/Guid.h"
#include "engine/util/StringId.h"
#include "engine/network/data/Value.h"

namespace engine
{
	namespace network
	{
		namespace data
		{
			template<typename T>
			class Plain : public Value
			{
			public:
				Plain()
				{
					// empty
				}

				Plain(const T &value)
					: _value(value)
				{
					// empty
				}

				operator T() const
				{
					return _value;
				}

				void operator =(const T &value)
				{
					_value = value;
				}

				virtual void serialize(sf::Packet &packet) override
				{
					packet << _value;
				}

				virtual void deserialize(sf::Packet &packet) override
				{
					packet >> _value;
				}

			private:
				T _value;
			};

			typedef Plain<util::Guid> Guid;
			typedef Plain<util::StringId> StringId;
		}
	}
}
