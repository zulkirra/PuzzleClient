/*
 Name:		PuzzleClient.h
 Created:	2/26/2023 5:21:09 PM
 Author:	Zackery Cassar
 Editor:	http://www.visualmicro.com
*/

#ifndef _Insight_h
#define _Insight_h

#include <Arduino.h>


template<class T>
class Insight {
	private:
		T _var;

	public:
		Insight(T var);

		void operator=(T var);


};

#endif // _Insight_h