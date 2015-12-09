#include "engine/network/services/Leaderboard.h"

#include <json.hpp>
#include <SFML/Network/Http.hpp>
#include "engine/network/Manager.h"

namespace engine
{
	namespace network
	{
		namespace services
		{
			async::Promise<> &Leaderboard::sendScore(const std::string &userName, float score) const
			{
				SendScoreTask *task = new SendScoreTask(userName, score);
				Manager::instance->runTask(task);
				return task->promise;
			}

			async::Promise<const Leaderboard::Entries &> &Leaderboard::requestScores() const
			{
				RequestScoresTask *task = new RequestScoresTask();
				Manager::instance->runTask(task);
				return task->promise;
			}


			Leaderboard::SendScoreTask::SendScoreTask(const std::string &userName, float score)
				: _userName(userName)
				, _score(score)
			{
				// empty
			}

			void Leaderboard::SendScoreTask::buildRequest(sf::Http::Request &request)
			{
				request.setMethod(sf::Http::Request::Method::Post);
				request.setUri("/leaderboard/v1/");

				json body;
				body["userName"] = _userName;
				body["score"] = _score;
				request.setBody(body);
			}

			void Leaderboard::SendScoreTask::processResponse(sf::Http::Response &response)
			{
				if (checkErrors(response)) return;

				if (response.getStatus() == sf::Http::Response::Status::Created)
					promise.resolveSuccess();
				else
					promise.resolveError(async::Error::INVALID_RESPONSE);
			}


			void Leaderboard::RequestScoresTask::buildRequest(sf::Http::Request &request)
			{
				request.setMethod(sf::Http::Request::Method::Get);
				request.setUri("/leaderboard/v1/");
			}

			void Leaderboard::RequestScoresTask::processResponse(sf::Http::Response &response)
			{
				if (checkErrors(response)) return;

				if (response.getStatus() == sf::Http::Response::Status::Ok)
				{
					Entries entries;
					json body = json::parse(response.getBody());
					try
					{
						for (auto item : body)
						{
							Entry entry;
							entry.userName = item["userName"].get<std::string>();
							entry.score = item["score"].get<float>();
							entries.push_back(entry);
						}
						promise.resolveSuccess(entries);
						return;
					}
					catch (const std::domain_error &)
					{
						// empty
					}
				}
				
				promise.resolveError(async::Error::INVALID_RESPONSE);
			}
		}
	}
}