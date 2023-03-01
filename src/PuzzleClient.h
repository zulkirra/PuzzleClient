/*
 Name:		PuzzleClient.h
 Created:	2/26/2023 5:21:09 PM
 Author:	Zackery Cassar
 Editor:	http://www.visualmicro.com
*/

#ifndef _PuzzleClient_h
#define _PuzzleClient_h

#include "Topic.h"
#include <Client.h>
#include <PubSubClient.h>
#include <LinkedList.h>

enum PuzzleState {
	Unknown = -1,
	Ready,
	Active,
	Paused,
	Finished,
	Resetting
};

class PuzzleClient {

	private:
		const char* _clientId;
		Topic _roomTopic;
		Topic _puzzleTopic;

		Client* _netClient;
		PuzzleState _state;

	public:
		LinkedList<Topic> _topics;
		PubSubClient client;	// Temporarily shown in case needed for any debugging etc.

		PuzzleClient(const char* clientId, Topic roomTopic, Topic puzzleTopic);
		PuzzleClient(const char* clientId, const char* roomTopic, const char* puzzleTopic);

		void setServer(IPAddress ip, uint16_t port = 1883);
		void setServer(uint8_t * ip, uint16_t port = 1883);
		void setCallback(MQTT_CALLBACK_SIGNATURE);

		void setClient(Client& netClient);
		Client& getClient();

		void setState(PuzzleState state);
		PuzzleState getState();


		bool connect();
		bool reconnect();
		void disconnect();

		bool subscribe(Topic topic);
		bool subscribe(const char* topic);
		bool unsubscribe(Topic topic);
		bool unsubscribe(const char* topic);

};

#endif // _PuzzleClient_h