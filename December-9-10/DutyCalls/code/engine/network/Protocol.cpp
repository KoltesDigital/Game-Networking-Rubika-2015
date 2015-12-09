#pragma once

#include "engine/network/Protocol.h"
#include "engine/network/data/Value.h"

namespace engine
{
	namespace network
	{
		void Protocol::declare(data::Value &value)
		{
			_values.push_back(&value);
		}

		void Protocol::serialize(sf::Packet &packet) const
		{
			for (data::Value *value : _values)
			{
				value->serialize(packet);
			}
		}

		void Protocol::deserialize(sf::Packet &packet) const
		{
			for (data::Value *value : _values)
			{
				value->deserialize(packet);
			}
		}
	}
}
