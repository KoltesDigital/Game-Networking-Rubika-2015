#pragma once

struct _ENetPeer;
typedef struct _ENetPeer ENetPeer;

#include "engine/util/Guid.h"

namespace engine
{
	namespace network
	{
		class Client
		{
		public:
			util::Guid id;

			Client(ENetPeer *peer);

			ENetPeer *getPeer() const;

		private:
			ENetPeer *_peer;
		};
	}
}
