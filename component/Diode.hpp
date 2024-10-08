//Diode.h - class declaration for a diode
//
//The voltage drop across a diode is: V(I) = ηVtln[(I/Is)+1] 
//where:
//I is the applied current
//V(I) is the voltage as a function of current,
//Is is the reverse saturation current,
//Vt = T / 11,586 is the volt equivalence of temperature, and
//η is the emission coefficient, which is 1 for germanium devices and 2 for silicon devices.

#ifndef _DIODE_H_
#define _DIODE_H_

#include <math.h>
#include "Component.h"

class Diode : public Component {
private:
	double Voltage;
	double reverseSaturationCurrent;
	double thermalVoltage;
	double emissionCoefficient;
public:
	Diode(double Is, double Vt, double n) {
		reverseSaturationCurrent = Is;
		thermalVoltage = Vt;
		emissionCoefficient = n;
		Voltage = 0;
	}
	double GetVoltage(double I) {
		//log() returns the natural log
		Voltage = emissionCoefficient * thermalVoltage * log(I / reverseSaturationCurrent + 1);
		return Voltage;
	}
	void Describe() {
		std::cout << "This diode has a voltage drop of " << Voltage << "V." << std::endl;
	}
};

#endif// _DIODE_H_