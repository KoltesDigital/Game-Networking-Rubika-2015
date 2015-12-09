#pragma once

#include "engine/gameplay/Entity.h"
#include "engine/network/data/Container.h"
#include "engine/network/data/Number.h"

namespace sf
{
	class Shape;
}

namespace engine
{
	namespace gameplay
	{
		namespace entities
		{
			class Player : public Entity
			{
			public:
				class PositionContainer : public network::data::Container
				{
				public:
					virtual util::StringId getContainerId() const override;

					virtual void declare(network::Protocol &protocol) override;

					network::data::Number<float> x;
					network::data::Number<float> y;

					static const util::StringId CONTAINER_ID;
				};

				Player();
				~Player();

				virtual util::StringId getEntityTypeId() const override;

				virtual void readContainer(const network::data::Container &container) override;
				virtual void writeContainer(network::data::Container &container) const override;

				virtual void update() override;

				static const util::StringId ENTITY_TYPE_ID;

			private:
				sf::Shape *_shape;
				float _x;
				float _y;
				float _realX;
				float _realY;
			};
		}
	}
}
