#include "engine/util/StringId.h"

#include <SFML/Config.hpp>
#include <SFML/Network/Packet.hpp>

namespace engine
{
	namespace util
	{
		std::hash<std::string> StringId::hasher;

		StringId::StringId()
			: _hash(0)
		{
			// empty
		}

		StringId::StringId(const std::string &str)
			: _hash(hasher(str))
#ifdef DEBUG
			, _str(str)
#endif
		{
			// empty
		}

		bool StringId::operator ==(const StringId &other) const
		{
			return _hash == other._hash;
		}

		bool StringId::operator <(const StringId &other) const
		{
			return _hash < other._hash;
		}

#ifdef DEBUG
		StringId::operator const std::string &() const
		{
			return _str;
		}
#else
		StringId::operator size_t() const
		{
			return _hash;
		}
#endif

		sf::Packet &operator<<(sf::Packet &packet, const StringId &stringId)
		{
#ifdef DEBUG
			return packet << stringId._str;
#else
			return packet << stringId._hash;
#endif
		}

		sf::Packet &operator>>(sf::Packet &packet, StringId &stringId)
		{
#ifdef DEBUG
			packet >> stringId._str;
			stringId._hash = StringId::hasher(stringId._str);
			return packet;
#else
			return packet >> stringId._hash;
#endif
		}
	}
}
