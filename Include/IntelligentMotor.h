#ifndef _INTELLIGENT_MOTOR
#define _INTELLIGENT_MOTOR

#include "Makros.h"
#include "Motor.h"
#include "Direction.h"
#include "RunUnit.h"
#include "TxControllerSupervision.h"
#include "InputPort.h"
#include "Counter.h"

#include <string>

/*! Die IntelligentMotor-Klasse implementiert den sogenannten intelligenten Modus der
 *  Fischertechnik-Motoren. Die Motoren sind mit einem Encoder gekoppelt, der die Drehimpulse
 *  z�hlt. Der Motor wird durch Angabe einer Anzahl von Drehimpulsen gesteuert. Sind die
 *  vordefinierten Drehimpulse erreicht, so wird der Motor automatisch gestoppt. Hierdurch ist 
 *  eine genaue Positionierung der Motoren m�glich. Des Weiteren ist es m�glich, zwei Motoren 
 *  zu synchronisieren. Beide Motoren haben die gleiche Anzahl von Drehimpulsen und stoppen
 *  automatisch. Die Synchronisierung �ber die Encoder-Werte sorgt f�r einen Gleichlauf der
 *  Motoren. Es ist m�glich, die synchron laufenden Motoren in gegenl�ufiger Richtung zu
 *  betreiben. Der Roboter oder Fahrzeug dreht sich dann um die Hochachse. */

class DLL_DECLSPEC IntelligentMotor : public Motor
{

public:

	/*! Der Standardkonstruktor. */
	IntelligentMotor(std::string text="IntelligentMotor");
	/*! Der Destruktor. */
	virtual ~IntelligentMotor();

	/*! Diese Methoden schaltet einem oder zwei synchronisierte Motoren ein.
	 *  \param num Gibt zusammen mit der Einheit die Anzahl der Drehimpulse an.	
	 *  \param unit Einheit zur Berechung der Drehimpulse im intelligenten Modus.
	 *  \param speed Gibt die Geschwindigkeit {0,...,512} des Motors an. 
	 *  \param dir Gibt die Drehrichtung des Motors an.
	 *  \param dir2 Gibt die Drehrichtung des zweiten Motors an (Synchronlauf). */
	bool turnOn(double num, RunUnit unit =IMPULSE, int speed =300, Direction dir =LEFT, Direction dir2 =LEFT);
	/*! Diese Methode stoppt den Motor ggfs. auch den sychronisierten Motor. */
	bool turnOff();

	/*! Diese Methode schaltet zwei Motoren synchron. */
	bool synchronize(IntelligentMotor* slave);
	/*! Diese Methode verbindet den Motor mit dem Controller. */
	bool connect(MotorPort po, InputPort co, TxControllerSupervision* con);

	/*! Diese Methode definiert die Anzahl von Impulsen pro 360�-Umdrehung. Die
	 *  Fischertechnik-Motoren ben�tigen 75 Drehimpulse f�r eine Umdrehung. Seitens
	 *  der Hardware ist dieser Wert konstant und kann nicht ge�ndert werden.
	 *  Daher sollte diese Funktion nicht mit einem anderen Wert aufgerufen werden. */
	void rotation(unsigned int val);
	/*! Diese Methode liefert die Drehimpulse/360�-Umdrehung zur�ck. */	
	unsigned int rotation() const;

	/*! Diese Methode setzt den Durchmesser der am Motor befestigten Reifen. F�r die
	 *  schwarzen, grobstolligen Reifen betr�gt der Durchmesser 0.05m (5cm). Dieser
	 *  Wert sollte nur ge�ndert werden, wenn die Reifen durch andere ausgetauscht
	 *  werden. Ein fehlerhafte Angabe zu einer falschen Berechung von Drehimpulsen
	 *  bei Aufruf der turnOn-Methode. */
	double diameter() const;
	/*! Diese Methode liefert den Reifendurchmesser zur�ck. */
	void diameter(double val);

	/*! Diese Methode liefert das Sensor-Objekt (Counter) des Encoders des Motors zur�ck. */
	Counter& encoder();

private:
	unsigned int im_per_rot;
	double wheel_dia;

	Counter wheel_counter;
	IntelligentMotor* syncMotor;

	unsigned int impulse(double val, RunUnit unit = IMPULSE) const;

};

#endif
