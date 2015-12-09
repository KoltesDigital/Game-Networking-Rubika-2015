#include "engine/network/data/ContainerWrapper.h"

#include <cassert>
#include <SFML/Network/Packet.hpp>
#include "engine/gameplay/Manager.h"

namespace engine
{
	namespace network
	{
		namespace data
		{
			ContainerWrapper::ContainerWrapper()
				: _container(nullptr)
				{
					// empty
				}

			ContainerWrapper::ContainerWrapper(Container &container)
				: _containerId(container.getContainerId())
				, _container(&container)
			{
				// empty
			}

			ContainerWrapper::operator Container&()
			{
				return *_container;
			}

			void ContainerWrapper::serialize(sf::Packet &packet)
			{
				assert(_container);

				packet << _containerId;

				Protocol protocol;
				_container->declare(protocol);
				protocol.serialize(packet);
			}

			void ContainerWrapper::deserialize(sf::Packet &packet)
			{
				util::StringId containerId;
				packet >> containerId;
				_containerId = containerId;

				_container = gameplay::Manager::instance->createContainer(containerId);
				Protocol protocol;
				_container->declare(protocol);
				protocol.deserialize(packet);
			}
		}
	}
}
