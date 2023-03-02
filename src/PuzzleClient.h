/*
 Name:		PuzzleClient.h
 Created:	2/26/2023 5:21:09 PM
 Author:	Zackery Cassar
 Editor:	http://www.visualmicro.com
*/

#ifndef _PuzzleClient_h
#define _PuzzleClient_h

#include <Arduino.h>
#include "Topic.h"
#include <Client.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <LinkedList.h>

#define RECONNECT_INTERVAL	5000

enum PuzzleState {
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
		PuzzleState _state;

		unsigned long _lastReconnectAttempt;

	public:
		LinkedList<Topic> _topics;
		PubSubClient client;	// Temporarily shown in case needed for any debugging etc.

		PuzzleClient(const char* clientId, Topic roomTopic);
		PuzzleClient(const char* clientId, const char* roomTopic);
		PuzzleClient(const char* clientId, Topic roomTopic, Topic puzzleTopic);
		PuzzleClient(const char* clientId, const char* roomTopic, const char* puzzleTopic);

		void setServer(IPAddress ip, uint16_t port = 1883);
		void setServer(uint8_t * ip, uint16_t port = 1883);
		void setCallback(MQTT_CALLBACK_SIGNATURE);
		void setClient(Client& netClient);

		void setState(PuzzleState state);
		PuzzleState getState();
		
		bool connect();
		bool connected();
		bool reconnect();
		void disconnect();
		void loop();


		void subscribe(Topic topic);
		void subscribe(const char* topic);
		void publish(Topic topic, JsonDocument& doc, const size_t size, bool retain = false, bool sender = false);
		void publish(const char* topic, JsonDocument& doc, const size_t size, bool retain = false, bool sender = false);
		void unsubscribe(Topic topic);
		void unsubscribe(const char* topic);

		void log(const char* message);
		void log(int count, ...);

		const char* stateToString(PuzzleState puzzleState);
};

#endif // _PuzzleClient_h