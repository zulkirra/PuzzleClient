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

	// Subscribe to all required topics for a puzzle
	Topic topicRoomState = Topic(this->_roomTopic.get()).append("state");
	subscribe(topicRoomState.get());
}
PuzzleClient::PuzzleClient(const char* clientId, const char* roomTopic) {
	this->_clientId = clientId;
	this->_roomTopic = Topic(roomTopic);

	// Subscribe to all required topics for a puzzle
	Topic topicRoomState = Topic(this->_roomTopic.get()).append("state");
	subscribe(topicRoomState.get());
}
PuzzleClient::PuzzleClient(const char* clientId, Topic roomTopic, Topic puzzleTopic) {
	this->_clientId = clientId;
	this->_roomTopic = roomTopic;
	this->_puzzleTopic = puzzleTopic;
	
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
void PuzzleClient::setClient(Client& netClient) {
	client.setClient(netClient);
}

void PuzzleClient::setState(PuzzleState state) {
	this->_state = state;
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

void PuzzleClient::loop() {
	if (connected()) client.loop();
	else reconnect();
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
void PuzzleClient::publish(Topic topic, JsonDocument& doc, const size_t size, bool retain, bool sender) {
	publish(topic.get(), doc, size, retain, sender);
}
void PuzzleClient::publish(const char* topic, JsonDocument& doc, const size_t size, bool retain, bool sender) {

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
	if (!connected())
		return;

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