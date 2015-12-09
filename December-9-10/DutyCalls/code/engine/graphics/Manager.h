#pragma once

#include <set>
#include <SFML/Graphics/RenderWindow.hpp>

namespace sf
{
	class Shape;
}

namespace engine
{
	namespace graphics
	{
		class EventListener;

		class Manager
		{
		public:
			enum class ShapeType
			{
				CIRCLE,
				RECTANGLE,
			};

			Manager(EventListener *eventListener);

			bool initialize();
			void deinitialize();

			void update();
			
			void setWindowTitle(const std::string &title);
			bool hasFocus() const;

			sf::Shape *createShape(ShapeType type);
			void destroyShape(sf::Shape *shape);

			static Manager *instance;

			static const sf::Int16 WINDOW_WIDTH = 800;
			static const sf::Int16 WINDOW_HEIGHT = 600;

		private:
			EventListener *_eventListener;
			sf::RenderWindow _window;
			std::set<sf::Shape*> _shapes;
		};
	}
}
