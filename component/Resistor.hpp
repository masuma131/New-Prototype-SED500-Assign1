//Resistor.h - class declaration for a resistor

#ifndef _RESISTOR_H_
#define _RESISTOR_H_

#include "Component.h"

class Resistor : public Component {
private:
	double Resistance;
	double Voltage;
public:
	Resistor(double R) {
		Resistance = R;
		Voltage = 0.0;
	}
	double GetVoltage(double I) {
		Voltage = I * Resistance;
		return Voltage;
	}
	void Describe() {
		std::cout << "This component has a resistance of " << Resistance << "ohms with a voltage drop of " << Voltage << "V." << std::endl;
	}
};

#endif// _RESISTOR_H_