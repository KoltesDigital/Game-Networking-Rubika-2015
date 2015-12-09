#pragma once

#include <list>
#include <string>

#include "engine/network/async/PromiseHTTPTask.h"

namespace engine
{
	namespace network
	{
		namespace services
		{
			class Leaderboard
			{
			public:
				struct Entry
				{
					std::string userName;
					float score;
				};
				typedef std::list<Entry> Entries;

				async::Promise<> &sendScore(const std::string &userName, float score) const;

				async::Promise<const Entries &> &requestScores() const;

			private:
				class SendScoreTask : public async::PromiseHTTPTask<>
				{
				public:
					SendScoreTask(const std::string &userName, float score);

					virtual void buildRequest(sf::Http::Request &request) override;
					virtual void processResponse(sf::Http::Response &response) override;

				private:
					std::string _userName;
					float _score;
				};

				class RequestScoresTask : public async::PromiseHTTPTask<const Entries &>
				{
				public:
					virtual void buildRequest(sf::Http::Request &request) override;
					virtual void processResponse(sf::Http::Response &response) override;
				};
			};
		}
	}
}