#ifndef NETWORK_H
#define NETWORK_H

#include <string>
#include <functional>

class Network
{
public:
	Network();
	~Network();

	void setMessageReceivedCallback(const std::function<void(const std::string&)>& callback);

	void process();

	void startSession();
	void sendMessage(const std::string &message);

private:
	class Private;
	Private *d;
};

#endif //NETWORK_H
