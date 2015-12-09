#include "engine/gameplay/entities/Player.h"

#include <cassert>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include "engine/Engine.h"
#include "engine/graphics/Manager.h"
#include "engine/input/Manager.h"
#include "engine/network/Manager.h"
#include "engine/network/Protocol.h"

namespace engine
{
	namespace gameplay
	{
		namespace entities
		{
			const util::StringId Player::PositionContainer::CONTAINER_ID("PlayerPositionContainer");

			util::StringId Player::PositionContainer::getContainerId() const
			{
				return CONTAINER_ID;
			}

			void Player::PositionContainer::declare(network::Protocol &protocol)
			{
				protocol.declare(x);
				protocol.declare(y);
			}

			const util::StringId Player::ENTITY_TYPE_ID("PlayerEntity");

			Player::Player()
				: _x(0.f)
				, _y(0.f)
				, _realX(0.f)
				, _realY(0.f)
			{
				assert(graphics::Manager::instance);
				_shape = graphics::Manager::instance->createShape(graphics::Manager::ShapeType::CIRCLE);
			
				reinterpret_cast<sf::CircleShape*>(_shape)->setRadius(10.f);
				_shape->setOrigin(sf::Vector2f(10.f, 10.f));
				_shape->setFillColor(sf::Color(150, 50, 250));
				_shape->setOutlineThickness(2.f);
				_shape->setOutlineColor(sf::Color(250, 150, 100));
			}

			Player::~Player()
			{
				assert(graphics::Manager::instance);
				graphics::Manager::instance->destroyShape(_shape);
			}

			util::StringId Player::getEntityTypeId() const
			{
				return ENTITY_TYPE_ID;
			}

			void Player::readContainer(const network::data::Container &abstractContainer)
			{
				if (abstractContainer.getContainerId() == PositionContainer::CONTAINER_ID)
				{
					const PositionContainer &container = static_cast<const PositionContainer&>(abstractContainer);
					_realX = container.x;
					_realY = container.y;
				}
			}

			void Player::writeContainer(network::data::Container &abstractContainer) const
			{
				if (abstractContainer.getContainerId() == PositionContainer::CONTAINER_ID)
				{
					PositionContainer &container = static_cast<PositionContainer&>(abstractContainer);
					container.x = _x;
					container.y = _y;
				}
			}

			void Player::update()
			{
				int dx = 0;
				int dy = 0;

				float deltaTime = Engine::instance->getDeltaTime();
				float deltaMove = deltaTime * 200; // px/s

				if (network::Manager::instance->localId == ownerId)
				{
					if (input::Manager::instance->isKeyPressed(sf::Keyboard::Left))
						dx -= 1;
					if (input::Manager::instance->isKeyPressed(sf::Keyboard::Right))
						dx += 1;

					if (input::Manager::instance->isKeyPressed(sf::Keyboard::Up))
						dy -= 1;
					if (input::Manager::instance->isKeyPressed(sf::Keyboard::Down))
						dy += 1;

					_x += dx * deltaMove;
					_y += dy * deltaMove;

					_realX = _x;
					_realY = _y;

					if (input::Manager::instance->isKeyPressed(sf::Keyboard::Tab))
					{
						auto &promise = network::Manager::instance->leaderboardService.requestScores();
						promise.onSuccess([](const network::services::Leaderboard::Entries &entries)
						{
							std::cout << "=== Leaderboards (" << entries.size() << " players) ===" << std::endl;
							for (auto &entry : entries)
							{
								std::cout << entry.userName << " (" << entry.score << ")" << std::endl;
							}
						});
						promise.onError([](network::async::Error error)
						{
							std::cout << network::async::getErrorMessage(error) << std::endl;
						});
					}
				}
				else
				{
					if (_realX < _x - deltaMove)
						dx -= 1;
					else if (_realX > _x + deltaMove)
						dx += 1;
					else
						_x = _realX;

					if (_realY < _y - deltaMove)
						dy -= 1;
					else if (_realY > _y + deltaMove)
						dy += 1;
					else
						_y = _realY;

					if (dx)
						_x += dx * deltaMove;
					if (dy)
						_y += dy * deltaMove;


				}

				_shape->setPosition(_x, _y);
			}
		}
	}
}
