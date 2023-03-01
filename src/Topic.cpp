/*
 Name:		PuzzleClient.cpp
 Created:	2/25/2023 1:56:36 PM
 Author:	Zackery Cassar
 Editor:	http://www.visualmicro.com
*/

#include "Topic.h"

Topic::Topic() {}
Topic::Topic(const char* baseTopic) {
	this->_topic = (char*) baseTopic;
}

Topic& Topic::append(const char* subTopic) {
	// Allocate enough memory for the topic in the heap
	int size = strlen(this->_topic) + 1 + strlen(subTopic) + 1;	// +1 for null-terminator and topic dividers
	char* result = new char[size];

	// Append the subtopic to the main topic
	strcpy(result, this->_topic);
	strcat(result, "/");
	strcat(result, subTopic);
	this->_topic = result;

	return *this;
}

const char* Topic::c_str() {
	return this->_topic;
}
