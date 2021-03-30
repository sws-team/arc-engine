#include "lobby.h"

#include <iostream>
#include <steam/steam_api.h>
#include <steam/isteammatchmaking.h>
#include <steam/isteamuser.h>

class Lobby::Private
{
public:
	Private()
		:callbackMessageReceived(this, &Private::onMessageReceived)
	{

	}

	bool host = false;
	CSteamID lobbyID = CSteamID(-1, k_EUniverseInvalid, k_EAccountTypeInvalid);

	std::vector<LobbyData> lobbies;
	bool refreshing = false;
	std::string lobbyName;

	CCallResult<Lobby::Private, LobbyMatchList_t> refreshCallResult;
	CCallResult<Lobby::Private, LobbyCreated_t> createCallResult;
	CCallResult<Lobby::Private, LobbyEnter_t> joinCallResult;

	std::function<void (bool)> joinedCallback = nullptr;
	std::function<void (const std::string &)> messageReceivedCallback = nullptr;
	std::function<void (const std::vector<Lobby::LobbyData> &)> lobbiesChangedCallback = nullptr;

	void resetID();

	void updateLobbyList(LobbyMatchList_t *pLobbyMatchList, bool bIOFailure);
	void lobbyCreated(LobbyCreated_t *pLobby, bool bIOFailure);
	void lobbyJoined(LobbyEnter_t *pLobby, bool bIOFailure);

private:
	STEAM_CALLBACK(Private, onMessageReceived, LobbyChatMsg_t, callbackMessageReceived);
};

void Lobby::Private::onMessageReceived(LobbyChatMsg_t *pCallback)
{
	static constexpr int buf_size = 4 * 1024;
	char msg[buf_size];
	SteamMatchmaking()->GetLobbyChatEntry(pCallback->m_ulSteamIDLobby, pCallback->m_iChatID,
										  nullptr, &msg, buf_size, nullptr);
	if (messageReceivedCallback != nullptr)
		messageReceivedCallback(std::string(msg));
}

Lobby::Lobby()
	: d(new Private())
{

}

Lobby::~Lobby()
{
	delete d;
}

void Lobby::setJoinedCallback(const std::function<void (bool)> &callback)
{
	d->joinedCallback = callback;
}

void Lobby::setMessageReceivedCallback(const std::function<void (const std::string &)> &callback)
{
	d->messageReceivedCallback = callback;
}

void Lobby::setLobbiesChangedCallback(const std::function<void (const std::vector<Lobby::LobbyData> &)> &callback)
{
	d->lobbiesChangedCallback = callback;
}

void Lobby::refresh()
{
	if (d->refreshing)
		return;
	d->refreshing = true;
	SteamAPICall_t request = SteamMatchmaking()->RequestLobbyList();
	d->refreshCallResult.Set(request, d, &Lobby::Private::updateLobbyList);
}

void Lobby::create(const std::string &name, bool isPrivate, int playersCount)
{
	d->lobbyName = name;
	const ELobbyType eLobbyType = isPrivate ? k_ELobbyTypePrivate : k_ELobbyTypePublic;
	SteamAPICall_t request = SteamMatchmaking()->CreateLobby(eLobbyType, playersCount);
	d->createCallResult.Set(request, d, &Lobby::Private::lobbyCreated);
}

void Lobby::join(int n)
{
	CSteamID ID = SteamMatchmaking()->GetLobbyByIndex(n);
	SteamAPICall_t request = SteamMatchmaking()->JoinLobby(ID);
	d->joinCallResult.Set(request, d, &Lobby::Private::lobbyJoined);
}

void Lobby::leave()
{
	SteamMatchmaking()->LeaveLobby(d->lobbyID);
}

void Lobby::invite()
{
	SteamFriends()->ActivateGameOverlayInviteDialog(d->lobbyID);
}

bool Lobby::host() const
{
	return d->host;
}

void Lobby::reset()
{
	d->host = false;
}

void Lobby::sendMessage(const std::string &message)
{
	if (!d->lobbyID.IsLobby() || !d->lobbyID.IsValid())
		return;
	SteamMatchmaking()->SendLobbyChatMsg(d->lobbyID, message.c_str(), message.length() + 1);
}

std::vector<CSteamID> Lobby::players() const
{
	const CSteamID myID = SteamUser()->GetSteamID();
	std::vector<CSteamID> result;
	const int count = SteamMatchmaking()->GetNumLobbyMembers(d->lobbyID);
	for (int i = 0; i < count; ++i)
	{
		const CSteamID id = SteamMatchmaking()->GetLobbyMemberByIndex(d->lobbyID, i);
		if (id == myID)
			continue;
		result.push_back(id);
	}
	return result;
}

int Lobby::myID() const
{
	const CSteamID id = SteamUser()->GetSteamID();
	const int count = SteamMatchmaking()->GetNumLobbyMembers(d->lobbyID);
	for (int i = 0; i < count; ++i)
	{
		if (SteamMatchmaking()->GetLobbyMemberByIndex(d->lobbyID, i) == id)
			return i;
	}
	return -1;
}


void Lobby::Private::resetID()
{
	lobbyID = CSteamID(-1, k_EUniverseInvalid, k_EAccountTypeInvalid);
}

void Lobby::Private::updateLobbyList(LobbyMatchList_t *pLobbyMatchList, bool bIOFailure)
{
	lobbies.clear();
	refreshing = false;
	if (bIOFailure || pLobbyMatchList->m_nLobbiesMatching == 0)
	{
		if (lobbiesChangedCallback != nullptr)
			lobbiesChangedCallback(lobbies);
		return;
	}
	for (uint32 i = 0; i < pLobbyMatchList->m_nLobbiesMatching; ++i)
	{
		const CSteamID ID = SteamMatchmaking()->GetLobbyByIndex(i);
		if (!ID.IsValid())
			continue;
		if (!ID.IsLobby())
			continue;
		const uint64 steamID = ID.ConvertToUint64();
		const std::string name = SteamMatchmaking()->GetLobbyData(steamID, "name");
		const int maxCount = SteamMatchmaking()->GetLobbyMemberLimit(steamID);
		const int currentCount = SteamMatchmaking()->GetNumLobbyMembers(steamID);

		LobbyData lobby;
		lobby.name = name;
		lobby.maxCount = maxCount;
		lobby.currentCount = currentCount;
		lobbies.push_back(lobby);
	}
	if (lobbiesChangedCallback != nullptr)
		lobbiesChangedCallback(lobbies);
}

void Lobby::Private::lobbyCreated(LobbyCreated_t *pLobby, bool bIOFailure)
{
	if (bIOFailure || pLobby->m_eResult != k_EResultOK)
		return;

	lobbyID = pLobby->m_ulSteamIDLobby;
	host = true;
	SteamMatchmaking()->SetLobbyData(lobbyID, "name", lobbyName.c_str());
	if (joinedCallback != nullptr)
		joinedCallback(true);
}

void Lobby::Private::lobbyJoined(LobbyEnter_t *pLobby, bool bIOFailure)
{
	if (bIOFailure)
		return;
	lobbyID = pLobby->m_ulSteamIDLobby;
	if (joinedCallback != nullptr)
		joinedCallback(false);
}
