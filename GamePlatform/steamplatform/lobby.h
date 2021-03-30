#ifndef LOBBY_H
#define LOBBY_H

#include <string>
#include <vector>
#include <functional>

class CSteamID;

class Lobby
{
public:
	Lobby();
	~Lobby();

	struct LobbyData
	{
		std::string name;
		int maxCount;
		int currentCount;
	};

	void setJoinedCallback(const std::function<void(bool)>& callback);
	void setMessageReceivedCallback(const std::function<void(const std::string&)>& callback);
	void setLobbiesChangedCallback(const std::function<void(const std::vector<LobbyData>&)>& callback);

	void refresh();
	void create(const std::string &name, bool isPrivate, int playersCount);
	void join(int n);
	void leave();
	void invite();
	bool host() const;
	void reset();

	void sendMessage(const std::string& message);

	std::vector<CSteamID> players() const;
	int myID() const;

private:
	class Private;
	Private *d;
};

#endif //LOBBY_H
