#pragma once

#include "engine/network/Message.h"
#include "engine/network/data/Plain.h"

namespace engine
{
	namespace gameplay
	{
		class Entity;

		namespace messages
		{
			class DestroyEntity : public network::Message
			{
			public:
				DestroyEntity();
				DestroyEntity(const gameplay::Entity *entity);

				virtual util::StringId getContainerId() const override;

				virtual void declare(network::Protocol &protocol) override;

				virtual void process() override;

				static const util::StringId CONTAINER_ID;

			private:
				network::data::Guid _entityId;
			};
		}
	}
}
