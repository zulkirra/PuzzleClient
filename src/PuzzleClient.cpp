/*
 Name:		PuzzleClient.cpp
 Created:	2/25/2023 1:56:36 PM
 Author:	Zackery Cassar
 Editor:	http://www.visualmicro.com
*/
#include "PuzzleClient.h"

PuzzleClient::PuzzleClient(const char* clientId, Topic roomTopic, Topic puzzleTopic) {
	this->_clientId = clientId;
	this->_roomTopic = roomTopic;
	this->_puzzleTopic = puzzleTopic;
}
PuzzleClient::PuzzleClient(const char* clientId, const char* roomTopic, const char* puzzleTopic) {
	this->_clientId = clientId;
	this->_roomTopic = Topic(roomTopic);
	this->_puzzleTopic = Topic(roomTopic).append(puzzleTopic);
}

bool PuzzleClient::subscribe(Topic topic) {
	_topics.add(topic);

	return true;
}

bool PuzzleClient::subscribe(const char* topic) {
	_topics.add(Topic(topic));

	return true;
}

bool PuzzleClient::unsubscribe(const char*) {
	_topics.remove(0);
}

