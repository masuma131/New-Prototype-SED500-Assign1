//Component.h - class interface for circuit components

#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include <iostream>

class Component {
public:
	virtual double GetVoltage(double I) = 0;
	virtual void Describe() = 0;
};

#endif// _COMPONENT_H_