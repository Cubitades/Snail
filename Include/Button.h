///////////////////////////////////////////////////////////
//  Button.h
//  Implementation of the Class Button
//  Created on:      21-Mai-2010 11:53:20
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_70572C16_296A_4f36_9F0C_55C81E78DC74__INCLUDED_)
#define EA_70572C16_296A_4f36_9F0C_55C81E78DC74__INCLUDED_

#include "Makros.h"
#include "Sensor.h"

/*! Die Klasse Button realisiert einen Taster, beispielsweise den rot-schwarzen 
 *  Fischertechnik-Baustein oder auch den Ber�hrungstaster im CrossRoads-Modell. */

class DLL_DECLSPEC Button : public Sensor
{

public:
	/*! Der Standardkonstruktor. */
	Button(std::string text = "Button");
	/*! Der Destruktor. */
	virtual ~Button();
	/*! Diese Methode liefert true zur�ck, wenn der Taster seit der letzten Abfrage
	 *  gedr�ckt worden ist. Wichtig: diese Methode reagiert nur auf Ver�nderungen des 
	 *  Zustands! Um festzustellen, ob der Taster gedr�ckt ist, unabh�ngig von Zustands�nderungen,
	 *  so kann dieses �ber den Aufruf der Methode value erfolgen. */
	bool isPressed();
	/*! Diese Methode liefert true zur�ck, wenn der Taster seit der letzten Abfrage
	 *  losgelassen worden ist. Wichtig: Es werden nur Zustands�nderungen signalisiert (siehe
	 *  isPressed-Methode. */
	bool isReleased();
};
#endif // !defined(EA_70572C16_296A_4f36_9F0C_55C81E78DC74__INCLUDED_)
