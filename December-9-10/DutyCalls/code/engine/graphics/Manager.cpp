#include "engine/graphics/Manager.h"

#include <cassert>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>
#include "engine/graphics/EventListener.h"

namespace engine
{
	namespace graphics
	{
		Manager *Manager::instance = nullptr;

		Manager::Manager(EventListener *eventListener)
			: _eventListener(eventListener)
		{
			assert(eventListener);
		}

		bool Manager::initialize()
		{
			_window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Engine");

			_window.setVerticalSyncEnabled(true);

			sf::View view(sf::Vector2f(0.f, 0.f), sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
			_window.setView(view);

			return true;
		}

		void Manager::deinitialize()
		{
			_window.close();
		}

		void Manager::update()
		{
			sf::Event event;
			while (_window.pollEvent(event))
			{
				switch (event.type)
				{
				case sf::Event::Closed:
					_eventListener->onWindowClosed();
					break;
				}
			}

			_window.clear(sf::Color::Black);

			for (const sf::Shape *shape : _shapes)
			{
				_window.draw(*shape);
			}

			_window.display();
		}

		void Manager::setWindowTitle(const std::string &title)
		{
			_window.setTitle(title);
		}

		bool Manager::hasFocus() const
		{
			return _window.hasFocus();
		}

		sf::Shape *Manager::createShape(ShapeType type)
		{
			sf::Shape *shape;

			switch (type)
			{
			case ShapeType::CIRCLE:
				shape = new sf::CircleShape();
				break;

			case ShapeType::RECTANGLE:
				shape = new sf::RectangleShape();
				break;
			}

			assert(shape);
			_shapes.insert(shape);
			return shape;
		}

		void Manager::destroyShape(sf::Shape *shape)
		{
			assert(shape);
			size_t result = _shapes.erase(shape);
			assert(result == 1);
		}

	}
}
