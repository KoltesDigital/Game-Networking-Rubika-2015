#include "engine/input/Manager.h"
#include "engine/graphics/Manager.h"

namespace engine
{
	namespace input
	{
		Manager *Manager::instance = nullptr;

		bool Manager::isKeyPressed(sf::Keyboard::Key key)
		{
			if (!graphics::Manager::instance->hasFocus())
				return false;

			return sf::Keyboard::isKeyPressed(key);
		}
	}
}
