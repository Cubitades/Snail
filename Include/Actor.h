///////////////////////////////////////////////////////////
//  Actor.h
//  Implementation of the Class Actor
//  Created on:      21-Mai-2010 11:53:19
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_5B8C10A5_57EF_44d7_BBFC_E6C21ECBD867__INCLUDED_)
#define EA_5B8C10A5_57EF_44d7_BBFC_E6C21ECBD867__INCLUDED_

#include "Makros.h"
#include "OutputPort.h"
#include "Controlled.h"

/*! Die Actor-Klasse realisiert eine Oberklasse f�r Fischertechnik-Aktoren. Zzt. werden
 *  nur LED (Lamp-Klasse) unterst�tzt. F�r Motoren existieren eigene Oberklassen. F�r den
 *  ROBO IF Controller ist das die Klasse Motor; der TX Controller unterst�tzt auch die
 *  EncoderMotor-Klasse. */

class DLL_DECLSPEC Actor : public Controlled
{

public:
	/*! Der Konstruktor.
	 * \param po Gibt den Ausgang (Port) an. */
	Actor(OutputPort po=O1);
	/*! Der Destruktor. */
	virtual ~Actor();
	/*! Diese Methode verbindet den Aktor mit einem Ausgang am Controller.
	 * \param port Gibt den Ausgang (Port) an.
	 * \param controller Gibt den Controller an, mit dem der Aktor verbunden ist. */
	bool connect(OutputPort port, Controller* controller);
	/*! Mit dieser Methode l�sst sich der Ausgang �ndern. */
	void port(OutputPort po);
	/*! Diese Methode setzt den Wert am Ausgang. */
	bool value(int val);
	/*! Diese Methode liefert den zuletzt gesetzten Wert zur�ck. */
	int last();

private:
	OutputPort oport;
	int lvalue;

};
#endif // !defined(EA_5B8C10A5_57EF_44d7_BBFC_E6C21ECBD867__INCLUDED_)
