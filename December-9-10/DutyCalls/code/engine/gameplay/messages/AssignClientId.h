#pragma once

#include "engine/util/Guid.h"
#include "engine/network/Message.h"
#include "engine/network/data/Plain.h"

namespace engine
{
	namespace gameplay
	{
		class Entity;

		namespace messages
		{
			class AssignClientId : public network::Message
			{
			public:
				AssignClientId();
				AssignClientId(util::Guid clientId);

				virtual util::StringId getContainerId() const override;

				virtual void declare(network::Protocol &protocol) override;
				
				virtual void process() override;

				static const util::StringId CONTAINER_ID;

			private:
				network::data::Guid _clientId;
			};
		}
	}
}
