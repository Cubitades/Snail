///////////////////////////////////////////////////////////
//  PedestrianLight.h
//  Implementation of the Class PedestrianLight
//  Created on:      21-Mai-2010 11:53:27
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_6A54BBB4_1A4E_4c41_9871_E710603DE926__INCLUDED_)
#define EA_6A54BBB4_1A4E_4c41_9871_E710603DE926__INCLUDED_

#include "Makros.h"
#include "Lamp.h"
#include "Button.h"
#include "Model.h"

/*! Diese Klasse realisiert eine Fu�g�ngerampel f�r das CrossRoads-Modell. 
	Eine Fu�g�ngerampel besteht aus zwei, einer roten und einer gr�nen, LED. 
	Zus�tzlich hat die Fu�g�ngerampel einen Taster. */

class DLL_DECLSPEC PedestrianLight : public Model
{

public:
	/*! Der Standardkonstruktor mit Angabe der Schnittstelle der roten und
	    gr�nen LED. */
	PedestrianLight(OutputPort pred=O1,OutputPort pgreen=O2);
	/*! Der Destruktor. */
	virtual ~PedestrianLight();
	/*! Diese Methode verbindet das Objekt mit dem CrossRoads-Modell. */
	virtual bool construct(Controller* controller);
	/*! Diese Methode schaltet die rote LED an. */
	void redOn();
	/*! Diese Methode schaltet die rote LED aus. */
	void redOff();
	/*! Diese Methode schaltet die gr�ne LED an. */
	void greenOn();
	/*! Diese Methode schaltet die gr�ne LED aus. */
	void greenOff();
	/*! Diese Methode liefert liefert die gr�ne LED als Objekt zur�ck. */
	Lamp& green();
	/*! Diese Methode liefert liefert die gr�ne LED als Objekt zur�ck. */
	Lamp& red();
	/*! Diese Methode liefert den Taster als Objekt zur�ck. */
	Button& button();
	/*! Mit dieser Methode lassen sich die Schnittstellen der beiden LED setzen. */
	void port(OutputPort pred, OutputPort pgreen);
	
private:
	Lamp redlight;
	Lamp greenlight;
	Button pbutton;
};
#endif // !defined(EA_6A54BBB4_1A4E_4c41_9871_E710603DE926__INCLUDED_)
