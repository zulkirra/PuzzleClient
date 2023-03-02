/*
 Name:		PuzzleClient.cpp
 Created:	2/25/2023 1:56:36 PM
 Author:	Zackery Cassar
 Editor:	http://www.visualmicro.com
*/
#include "PuzzleClient.h"

PuzzleClient::PuzzleClient(const char* clientId, Topic roomTopic) {
	this->_clientId = clientId;
	this->_roomTopic = roomTopic;
	this->_stateChanged = true;

	// Subscribe to all required topics for a puzzle
	Topic topicRoomState = Topic(this->_roomTopic.get()).append("state");
	subscribe(topicRoomState.get());
}
PuzzleClient::PuzzleClient(const char* clientId, const char* roomTopic) {
	this->_clientId = clientId;
	this->_roomTopic = Topic(roomTopic);
	this->_stateChanged = true;

	// Subscribe to all required topics for a puzzle
	Topic topicRoomState = Topic(this->_roomTopic.get()).append("state");
	subscribe(topicRoomState.get());
}
PuzzleClient::PuzzleClient(const char* clientId, Topic roomTopic, Topic puzzleTopic) {
	this->_clientId = clientId;
	this->_roomTopic = roomTopic;
	this->_puzzleTopic = puzzleTopic;
	this->_stateChanged = true;

	// Subscribe to all required topics for a puzzle
	Topic topicState = Topic(this->_puzzleTopic.get()).append("state");
	Topic topicSet = Topic(this->_puzzleTopic.get()).append("set");
	Topic topicCommand = Topic(this->_puzzleTopic.get()).append("command");
	Topic topicConnected = Topic(this->_puzzleTopic.get()).append("connected");
	subscribe(topicState.get());
	subscribe(topicSet.get());
	subscribe(topicCommand.get());
	subscribe(topicConnected.get());
}
PuzzleClient::PuzzleClient(const char* clientId, const char* roomTopic, const char* puzzleTopic) {
	this->_clientId = clientId;
	this->_roomTopic = Topic(roomTopic);
	this->_puzzleTopic = Topic(roomTopic).append(puzzleTopic);
	this->_stateChanged = true;

	// Subscribe to all required topics for a puzzle
	Topic topicState		= Topic(this->_puzzleTopic.get()).append("state");
	Topic topicSet			= Topic(this->_puzzleTopic.get()).append("set");
	Topic topicCommand		= Topic(this->_puzzleTopic.get()).append("command");
	Topic topicConnected	= Topic(this->_puzzleTopic.get()).append("connected");
	subscribe(topicState.get());
	subscribe(topicSet.get());
	subscribe(topicCommand.get());
	subscribe(topicConnected.get());
}

void PuzzleClient::setServer(IPAddress ip, uint16_t port) {
	client.setServer(ip, port);
}
void PuzzleClient::setServer(uint8_t* ip, uint16_t port) {
	client.setServer(ip, port);
}
void PuzzleClient::setCallback(MQTT_CALLBACK_SIGNATURE) {
	client.setCallback(callback);
}
void PuzzleClient::setReadySetup(READY_SETUP) {
	this->readySetup = readySetup;
}
void PuzzleClient::setReadyLoop(READY_LOOP) {
	this->readyLoop = readyLoop;
}
void PuzzleClient::setActiveSetup(ACTIVE_SETUP) {
	this->activeSetup = activeSetup;
}
void PuzzleClient::setActiveLoop(ACTIVE_LOOP) {
	this->activeLoop = activeLoop;
}
void PuzzleClient::setPausedSetup(PAUSED_SETUP) {
	this->pausedSetup = pausedSetup;
}
void PuzzleClient::setPausedLoop(PAUSED_LOOP) {
	this->pausedLoop = pausedLoop;
}
void PuzzleClient::setFinishedSetup(FINISHED_SETUP) {
	this->finishedSetup = finishedSetup;
}
void PuzzleClient::setFinishedLoop(FINISHED_LOOP) {
	this->finishedLoop = finishedLoop;
}
void PuzzleClient::setResettingSetup(RESETTING_SETUP) {
	this->resettingSetup = resettingSetup;
}
void PuzzleClient::setResettingLoop(RESETTING_LOOP) {
	this->resettingLoop = resettingLoop;
}
void PuzzleClient::setClient(Client& netClient) {
	client.setClient(netClient);
}

void PuzzleClient::setState(PuzzleState state, bool publishState, bool triggerStateSetup) {
	// Publish the state change
	if (publishState) {
		Topic topicState = Topic(this->_puzzleTopic.get()).append("state");
		StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
		doc["state"] = state;
		publish(topicState, doc, MQTT_MAX_PACKET_SIZE);
	}

	// Update the state
	this->_state = state;
	if (triggerStateSetup) this->_stateChanged = true;
	log(3, "State set to '", stateToString(state), "'");
}
PuzzleState PuzzleClient::getState() {
	return this->_state;
}

bool PuzzleClient::connect() {

	// The connected topic is different for a room and a puzzle
	Topic topicConnected = this->_puzzleTopic.isEmpty()
		? Topic(this->_roomTopic.get()).append("connected")
		: Topic(this->_puzzleTopic.get()).append("connected");

	// Connect the the server with a disconnect will message
	if (client.connect(this->_clientId, topicConnected.get(), 0, true, "{\"connected\":false}")) {
		// Publish a connected message
		client.publish(topicConnected.get(), "{\"connected\":true}", true);

		log("Connected");

		// Subscribe to all necessary topics
		for (int i = 0; i < this->_topics.size(); i++) {
			subscribe(this->_topics.get(i));
		}

	}

	return client.connected();
}
bool PuzzleClient::connected() {
	return client.connected();
}
bool PuzzleClient::reconnect() {
	long now = millis();

	if (now - this->_lastReconnectAttempt > RECONNECT_INTERVAL) {
		this->_lastReconnectAttempt = now;

		if (connect()) this->_lastReconnectAttempt = 0;
	}

	return connected();
}
void PuzzleClient::disconnect() {
	log("Disconnecting from server");
	client.disconnect();
}

void PuzzleClient::loop() {
	if (connected()) client.loop();
	else reconnect();

	// Check if the state has changed this loop
	if (this->_stateChanged) {
		this->_stateChanged = false;

		// Run the new states setup function
		if (this->_state == Ready && readySetup) readySetup();
		else if (this->_state == Active && activeSetup) activeSetup();
		else if (this->_state == Paused && pausedSetup) pausedSetup();
		else if (this->_state == Finished && finishedSetup) finishedSetup();
		else if (this->_state == Resetting && resettingSetup) resettingSetup();
	}
	// Run the loop function for the current state
	if (this->_state == Ready && readyLoop) readyLoop();
	else if (this->_state == Active && activeLoop) activeLoop();
	else if (this->_state == Paused && pausedLoop) pausedLoop();
	else if (this->_state == Finished && finishedLoop) finishedLoop();
	else if (this->_state == Resetting && resettingLoop) resettingLoop();
}
void PuzzleClient::callback(char* topic, uint8_t* payload, unsigned int length) {
	StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
	deserializeJson(doc, payload);

	// Make sure the puzzle is connected at all times
	Topic topicConnected = Topic(this->_puzzleTopic.get()).append("connected");
	if (topicConnected.compare(topic) == 0) {
		if (doc.containsKey("connected") && doc["connected" == false]) {
			client.publish(topicConnected.get(), "{\"connected\":true}", true);
		}
	}

	// Update the state
	Topic topicState = this->_puzzleTopic.isEmpty()
		? Topic(this->_roomTopic.get()).append("state")
		: Topic(this->_puzzleTopic.get()).append("state");
	if (topicState.compare(topic) == 0) {
		if (doc.containsKey("state")) {
			PuzzleState newState = doc["state"].as<PuzzleState>();

			// Make sure this puzzle doesn't trigger the state setup when connecting again
			if (doc.containsKey("clientId") && strcmp(doc["clientId"].as<const char*>(), this->_clientId) == 0) {
				setState(newState, false, false);
			} else {
				setState(newState, false, true);
			}
		}
	}


}

void PuzzleClient::subscribe(Topic topic) {
	subscribe(topic.get());
}
void PuzzleClient::subscribe(const char* topic) {
	// If the client is connect then subscribe to the new topic
	if (connected()) {
		client.subscribe(topic);
		log(3, "Subscribed to topic '", topic, "'");
	}

	// Check if the topic has already been added to the list
	for (int i = 0; i < this->_topics.size(); i++) {
		if (strcmp(this->_topics.get(i).get(), topic) == 0) return;
	}

	// Add the topic if it doesn't exist in the list
	this->_topics.add(Topic(topic));
}
void PuzzleClient::publish(Topic topic, JsonDocument& doc, const size_t size, bool sender) {
	publish(topic.get(), doc, size);
}
void PuzzleClient::publish(const char* topic, JsonDocument& doc, const size_t size, bool sender) {
	if (!connected())
		return;

	// Add the sender if required
	if(sender) doc["clientId"] = this->_clientId;

	// Create the payload
	char payload[size];
	serializeJson(doc, payload, size);

	// Send the message
	log(5, "Sending '", payload, "' to topic '", topic, "'");
	client.publish(topic, payload);
}
void PuzzleClient::unsubscribe(Topic topic) {
	unsubscribe(topic.get());
}
void PuzzleClient::unsubscribe(const char* topic) {
	// If the client is connected then unsubscribe to the topic
	if (connected()) {
		client.unsubscribe(topic);
		log(3, "Unsubscribed from topic '", topic, "'");
	}

	// Remove any instances of the topic from the list
	for (int i = 0; i < this->_topics.size(); i++) {
		if (strcmp(this->_topics.get(i).get(), topic) == 0)
			this->_topics.remove(i);
	}
}

void PuzzleClient::log(const char* message) {
	if (Serial) Serial.println(message);
	if (!connected())
		return;

	// The logs topic is different for a room and a puzzle
	Topic topicLogs = this->_puzzleTopic.isEmpty()
		? Topic(this->_roomTopic.get()).append("logs")
		: Topic(this->_puzzleTopic.get()).append("logs");


	const char* state = stateToString(this->_state);

	// Create the message to send
	int size = strlen(this->_clientId) + strlen(state) + strlen(message) + 8;	// Includes extra characters
	char* payload = new char[size];
	
	strcpy(payload, "[");
	strcat(payload, this->_clientId);
	strcat(payload, "] [");
	strcat(payload, state);
	strcat(payload, "]: ");
	strcat(payload, message);

	// Send the message
	client.publish(topicLogs.get(), payload);

	// Free the memory
	delete payload;
}
void PuzzleClient::log(int count, ...) {
	LinkedList<const char*> messageParts;
	int size = 1;

	// Add all messages parts to a list and get the total memory size
	va_list args;
	va_start(args, count);
	for (int i = 0; i < count; i++) {
		const char* messagePart = va_arg(args, const char*);
		messageParts.add(messagePart);

		size += strlen(messagePart);
	}
	va_end(args);

	// Create the message
	char* message = new char[size];
	
	for (int i = 0; i < messageParts.size(); i++) {
		if (i == 0) strcpy(message, messageParts.get(i));
		else strcat(message, messageParts.get(i));
	}

	// Send the message
	log(message);

	// Free the memory
	delete message;
}

const char* PuzzleClient::stateToString(PuzzleState puzzleState) {
	if (puzzleState == Ready) return "READY";
	else if (puzzleState == Active && this->_puzzleTopic.isEmpty()) return "RUNNING";
	else if (puzzleState == Active && !this->_puzzleTopic.isEmpty()) return "ACTIVE";
	else if (puzzleState == Paused) return "PAUSED";
	else if (puzzleState == Finished) return "FINISHED";
	else if (puzzleState == Resetting) return "RESETTING";

	return "UNKNOWN";
}