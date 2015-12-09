#pragma once

#include <cstring>
#include <string>

namespace sf
{
	class Packet;
}

namespace engine
{
	namespace util
	{
		class StringId
		{
		public:
			StringId();
			StringId(const std::string &str);

			bool operator ==(const StringId &other) const;
			bool operator <(const StringId &other) const;

#if !defined(DEBUG)
			operator size_t() const;
#endif

		private:
			size_t _hash;

			static std::hash<std::string> hasher;

			friend sf::Packet &operator<<(sf::Packet &packet, const StringId &stringId);
			friend sf::Packet &operator>>(sf::Packet &packet, StringId &stringId);

#ifdef DEBUG
		public:
			operator const std::string &() const;

		private:
			std::string _str;
#endif
		};
	}
}
