/*
 Name:		PuzzleClient.h
 Created:	2/26/2023 5:21:09 PM
 Author:	Zackery Cassar
 Editor:	http://www.visualmicro.com
*/

#ifndef _Topic_h
#define _Topic_h

#include <Arduino.h>

class Topic {

	private:
		char* _topic;
		bool _isEmpty;


	public:
		Topic();
		Topic(const char* baseTopic);

		Topic& append(const char* subTopic);

		const char* get();
		int compare(Topic topic);
		int compare(const char* topic);

		bool isEmpty();
};


#endif // _Topic_h