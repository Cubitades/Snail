///////////////////////////////////////////////////////////
//  TrafficLight.h
//  Implementation of the Class TrafficLight
//  Created on:      21-Mai-2010 11:53:29
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_7E07ADAB_5F75_472e_A235_83E205217957__INCLUDED_)
#define EA_7E07ADAB_5F75_472e_A235_83E205217957__INCLUDED_

#include "Makros.h"
#include "Lamp.h"
#include "Model.h"

/*! Diese Methode implementiert eine Stra�enampel im CrossRoads Modell. Diese 
    besteht aus drei LEDs (gr�n, geld, rot). */

class DLL_DECLSPEC TrafficLight : public Model
{

public:
	/*! Der Standardkonstruktor mit den 3 Ausg�ngen als Parameter. */
	TrafficLight(OutputPort pred=O1,OutputPort pyellow=O2,OutputPort pgreen=O3);
	/*! Der Destruktor. */
	virtual ~TrafficLight();
	/*! Diese Methode schaltet die rote LED ein. */
	void redOn();
	/*! Diese Methode schaltet die rote LED aus. */
	void redOff();
	/*! Diese Methode schaltet die gelbe LED ein. */
	void yellowOn();
	/*! Diese Methode schaltet die gelbe LED aus. */
	void yellowOff();
	/*! Diese Methode schaltet die gr�ne LED ein. */
	void greenOn();
	/*! Diese Methode schaltet die gr�ne LED aus. */
	void greenOff();
	/*! Diese Methode schaltet alle LEDs ein (zu Testzwecken). */
	void allOn();
	/*! Diese Methode schaltet alle LEDs aus (zu Testzwecken). */
	void allOff();
	/*! Diese Methode setzt die Ausg�nge f�r die 3 LEDs. */
	void port(OutputPort pred, OutputPort pyellow, OutputPort pgreen);
	/*! Diese Methode verbindet das Objekt mit dem Fischertechnik Controller. */
	virtual bool construct(Controller* controller);
	/*! Diese Methode liefert die gr�ne LED als Objekt zur�ck. */
	Lamp& green();
	/*! Diese Methode liefert die gelbe LED als Objekt zur�ck. */
	Lamp& yellow();
	/*! Diese Methode liefert die rote LED als Objekt zur�ck. */
	Lamp& red();

private:
	Lamp redlight;
	Lamp yellowlight;
	Lamp greenlight;

};
#endif // !defined(EA_7E07ADAB_5F75_472e_A235_83E205217957__INCLUDED_)
