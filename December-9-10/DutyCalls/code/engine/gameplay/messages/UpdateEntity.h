#pragma once

#include "engine/util/Guid.h"
#include "engine/network/Message.h"
#include "engine/network/data/Container.h"
#include "engine/network/data/ContainerWrapper.h"
#include "engine/network/data/Plain.h"

namespace engine
{
	namespace gameplay
	{
		class Entity;

		namespace messages
		{
			class UpdateEntity : public network::Message
			{
			public:
				UpdateEntity();
				UpdateEntity(const gameplay::Entity *entity, network::data::Container &container);

				virtual util::StringId getContainerId() const override;

				virtual void declare(network::Protocol &protocol) override;

				virtual void process() override;

				static const util::StringId CONTAINER_ID;

			private:
				network::data::Guid _entityId;
				network::data::ContainerWrapper _container;
			};
		}
	}
}
