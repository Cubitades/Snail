///////////////////////////////////////////////////////////
//  EventField.h
//  Implementation of the Class EventField
//  Created on:      21-Mai-2010 11:53:24
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_DAF600BA_5870_4053_AA41_31F4CD21E5C9__INCLUDED_)
#define EA_DAF600BA_5870_4053_AA41_31F4CD21E5C9__INCLUDED_

#include "Makros.h"

/*! Diese Klasse definiert ein Feld von maximal 64 Ereignissen. Ein Ereignis
 *  kann gesetzt (true) oder nicht gesetzt sein (false). Ein Ereignisfeld bietet
 *  sich dann an, wenn mehrere, unterschiedliche Ereignisse verwaltet werden
 *  m�ssen.
 *
 *  Hinweis: Um die Handhabung einfach zu gestalten, sollte f�r jedes m�gliche
 *  Ereignis eine Konstante definiert werden, beispielsweise durch 
 *  \code
 *  #define EREIGNIS_ALARM_IST_AUSGELOEST    0 
 *  #define EREIGNIS_TASTER_IST_GEDRUECKT    1 
 *  #define EREIGNIS_FEHLER                 63 
 *  \endcode
 *  Als Wertebereich stehen die Zahlen 0 bis 63 zur Verf�gung. Es ist darauf zu
 *  achten, dass den symbolischen Namen tats�chlich auch unterschiedliche Werte
 *  zugeordnet werden, weil ansonsten Ereignisse nicht unterschieden werden k�nnen. */

class DLL_DECLSPEC EventField
{

public:
	/*! Der Standardkonstruktor. */
	EventField();
	/*! Der Destruktor. */
	virtual ~EventField();
	/*! Diese Methode setzt ein Ereignis. 
	 *  \param ev Ereignis [0..63] */
	bool set(int ev);
	/*! Diese Methode l�scht ein Ereignis. 
	 *  \param ev Ereignis [0..63] */
	bool clear(int ev);
	/*! Diese Methode liefert true zur�ck, wenn das Ereignis gesetzt ist. 
	 *  \param ev Ereignis [0..63] */
	bool isSet(int ev);
	/*! Diese Methode liefert true zur�ck, wenn das Ereignis gesetzt ist. Zus�tzlich
	 *  wird das Ereignis zur�ckgesetzt. Ein erneuter Aufruf von isSet oder isSetAndClear
	 *  liefert dann false zur�ck.
	 *  \param ev Ereignis [0..63] */
	bool isSetAndClear(int ev);
	/*! Dieser Operator liefert true zur�ck, wenn mindestens ein Ereignis gesetzt ist. */
	operator bool();
	/*! Diese Methode l�scht alle gesetzten Ereignisse. */
	void reset();
	/*! Diese Methode setzt die Ereignisse entsprechend einem Bitmuster. */
	void bits(unsigned long long pattern);
	/*! Diese Methode liefert die Ereignisse als Bitmuster zur�ck. */
	unsigned long long bits();
	/*! Diese Methode liefert die maximale Anzahl der Ereignisse zur�ck (zzt. 64). */
	int length();

private:
	unsigned long long flags;

};
#endif // !defined(EA_DAF600BA_5870_4053_AA41_31F4CD21E5C9__INCLUDED_)
