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


	public:
		Topic();
		Topic(const char* baseTopic);

		Topic& append(const char* subTopic);
		const char* c_str();
};


#endif // _Topic_h