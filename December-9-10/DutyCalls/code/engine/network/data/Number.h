#pragma once

#include "engine/network/data/Value.h"

namespace engine
{
	namespace network
	{
		namespace data
		{
			template<typename T>
			class Number : public Value
			{
			public:
				Number()
				{
					// empty
				}

				Number(const T &value)
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
		}
	}
}
