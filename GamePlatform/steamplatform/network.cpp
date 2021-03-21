#include "network.h"
#include "lobby.h"

#include <iostream>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <random>
#include <chrono>

#include <steam/isteamnetworkingmessages.h>
#include <steam/isteamnetworkingutils.h>

class Network::Private
{
public:
	Private(Network *parent)
		: q(parent)
	{

	}
	bool isActive = false;
	std::vector<SteamNetworkingIdentity> players;

	static constexpr int MSG_VEC_SIZE = 10;
	std::function<void (const std::string &)> messageReceivedCallback = nullptr;

	void checkMessages();

private:
	Network *q = nullptr;
};

Network::Network()
	:d(new Private(this))
{

}

Network::~Network()
{
	delete d;
}

void Network::setMessageReceivedCallback(const std::function<void (const std::string &)> &callback)
{

}

void Network::process()
{
	if (!d->isActive)
		return;
	d->checkMessages();
}

void Network::startSession()
{
	d->players.clear();
	/*
	const std::vector<CSteamID> players = Lobby::Instance()->players();
	for(const CSteamID& id : players)
	{
		SteamNetworkingIdentity identity;
		identity.Clear();
		identity.m_eType = k_ESteamNetworkingIdentityType_SteamID;
		identity.SetSteamID(id);
		if (identity.IsInvalid())
			continue;
		d->players.push_back(identity);
	}*/
	d->isActive = true;
}

void Network::sendMessage(const std::string &message)
{
	for(const SteamNetworkingIdentity& identity : d->players)
	{
		const EResult result = SteamNetworkingMessages()->SendMessageToUser(identity,
													 (void*)message.c_str(),
													 message.length(),
													 k_nSteamNetworkingSend_Reliable, 0);
//		if (result == k_EResultOK)
//			Godot::print("SENDED");
//		else
//			Godot::print("NOT SENDED");

	}
}

void Network::Private::checkMessages()
{
	SteamNetworkingMessage_t *msgs[Private::MSG_VEC_SIZE];
	for (int i = 0; i < Private::MSG_VEC_SIZE; ++i)
		msgs[i] = nullptr;

	const int count = SteamNetworkingMessages()->ReceiveMessagesOnChannel(0, msgs, Private::MSG_VEC_SIZE);
	if (count == 0)
		return;

	for (int i = 0; i < Private::MSG_VEC_SIZE; ++i)
	{
		if (msgs[i] == nullptr)
			break;

		const int size = msgs[i]->GetSize();
		if (size == 0)
			continue;

		char *buf = new char[size];
		memcpy(buf, msgs[i]->GetData(), size);
		const std::string text = std::string(buf);
		delete[] buf;
		msgs[i]->Release();

		if (messageReceivedCallback != nullptr)
			messageReceivedCallback(text);
	}
}

