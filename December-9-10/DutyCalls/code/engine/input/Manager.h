#pragma once

#include <SFML/Window/Keyboard.hpp>

namespace engine
{
	namespace input
	{
		class Manager
		{
		public:
			bool isKeyPressed(sf::Keyboard::Key key);

			static Manager *instance;
		};
	}
}
