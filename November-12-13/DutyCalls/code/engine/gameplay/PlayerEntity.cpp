#include "engine/gameplay/PlayerEntity.h"

#include <cassert>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "engine/Engine.h"
#include "engine/graphics/Manager.h"

namespace engine
{
	namespace gameplay
	{
		PlayerEntity::PlayerEntity()
			: _x(0.f)
			, _y(0.f)
		{
			assert(graphics::Manager::instance);
			_shape = graphics::Manager::instance->createShape(graphics::Manager::ShapeType::CIRCLE);

			reinterpret_cast<sf::CircleShape*>(_shape)->setRadius(10.f);
			_shape->setOrigin(sf::Vector2f(10.f, 10.f));
			_shape->setFillColor(sf::Color(150, 50, 250));
			_shape->setOutlineThickness(2.f);
			_shape->setOutlineColor(sf::Color(250, 150, 100));
		}

		PlayerEntity::~PlayerEntity()
		{
			assert(graphics::Manager::instance);
			graphics::Manager::instance->destroyShape(_shape);
		}

		void PlayerEntity::update()
		{
			if (graphics::Manager::instance->hasFocus())
			{
				int dx = 0;
				int dy = 0;

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
					dx -= 1;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
					dx += 1;

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
					dy -= 1;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
					dy += 1;

				assert(Engine::instance);
				float deltaTime = Engine::instance->getDeltaTime();
				float deltaMove = deltaTime * 200; // px/s
				_x += dx * deltaMove;
				_y += dy * deltaMove;

				_shape->setPosition(_x, _y);
			}
		}
	}
}
