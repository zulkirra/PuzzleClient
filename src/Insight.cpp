/*
	Name:       Playground.ino
	Created:	28/02/2023 6:46:14 PM
	Author:     LAPTOP\zacke
*/
#include "Insight.h"

template<class T>
Insight<T>::Insight(T var) {
	this->_var = var;
}

template<class T>
void Insight<T>::operator=(T var) {

}