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

#define READY_SETUP		void(*readySetup)()
#define READY_LOOP		void(*readyLoop)()
#define ACTIVE_SETUP	void(*activeSetup)()
#define ACTIVE_LOOP		void(*activeLoop)()
#define PAUSED_SETUP	void(*pausedSetup)()
#define PAUSED_LOOP		void(*pausedLoop)()
#define FINISHED_SETUP	void(*finishedSetup)()
#define FINISHED_LOOP	void(*finishedLoop)()
#define RESETTING_SETUP	void(*resettingSetup)()
#define RESETTING_LOOP	void(*resettingLoop)()

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
		bool _stateChanged;     

		READY_SETUP;
		READY_LOOP;
		ACTIVE_SETUP;
		ACTIVE_LOOP;
		PAUSED_SETUP;
		PAUSED_LOOP;
		FINISHED_SETUP;
		FINISHED_LOOP;
		RESETTING_SETUP;
		RESETTING_LOOP;

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
		void setReadySetup(READY_SETUP);
		void setReadyLoop(READY_LOOP);
		void setActiveSetup(ACTIVE_SETUP);
		void setActiveLoop(ACTIVE_LOOP);
		void setPausedSetup(PAUSED_SETUP);
		void setPausedLoop(PAUSED_LOOP);
		void setFinishedSetup(FINISHED_SETUP);
		void setFinishedLoop(FINISHED_LOOP);
		void setResettingSetup(RESETTING_SETUP);
		void setResettingLoop(RESETTING_LOOP);
		void setClient(Client& netClient);

		void setState(PuzzleState state, bool publishState = true, bool triggerStateSetup = true);
		PuzzleState getState();
		
		bool connect();
		bool connected();
		bool reconnect();
		void disconnect();

		void loop();
		void callback(char* topic, uint8_t* payload, unsigned int length);

		void subscribe(Topic topic);
		void subscribe(const char* topic);
		void publish(Topic topic, JsonDocument& doc, const size_t size, bool sender = true);
		void publish(const char* topic, JsonDocument& doc, const size_t size, bool sender = true);
		void unsubscribe(Topic topic);
		void unsubscribe(const char* topic);

		void log(const char* message);
		void log(int count, ...);

		const char* stateToString(PuzzleState puzzleState);
};

#endif // _PuzzleClient_h