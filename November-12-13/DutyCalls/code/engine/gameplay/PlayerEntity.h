#pragma once

namespace sf
{
	class Shape;
}

namespace engine
{
	namespace gameplay
	{
		class PlayerEntity
		{
		public:
			PlayerEntity();
			~PlayerEntity();

			void update();

		private:
			sf::Shape *_shape;
			float _x;
			float _y;
		};
	}
}
