#ifndef _REACTIVEMODEL_H
#define _REACTIVEMODEL_H

#include "Makros.h"
#include "Model.h"
#include "InputPort.h"
#include "MotorPort.h"
#include "TxControllerSupervision.h"
#include "Timing.h"

/*! Die Reactive-Model-Klasse realisiert ein Fischertechnik-Modell, welches eine
 *  ereignisorientierte Ablaufsteuerung besitzt. Die Klasse ist die Basisklasse oder
 *  Oberklasse f�r ein reales Fischertechnik-Modell. Die Methoden on_XYZ_Changed
 *  werden automatisch aufgerufen, sobald bestimmte Sensorwerte sich �ndern. Durch
 *  �berladen der Methoden kann dadurch eine Reaktion implementiert werden (daher 
 *  kommt auch der Klassenname - ReactiveModel). */

class DLL_DECLSPEC ReactiveModel : public Model {
public:

	/*! Ein Bitfield ist ein Bitmuster der L�nge 8. */
	typedef unsigned char Bitfield;

	/*! Der Standardkonstruktor. */
	ReactiveModel();

	/*! Diese Methode liefert das zum Modell geh�rige TX-Controller-Objekt zur�ck. */
	TxControllerSupervision* controller() const;

	/*! Die folgenden Methoden werden automatisch aufgerufen, wenn sich die
	 *  Werte an den Sensoren �ndern. �ber das Bitfield-Argument kann festgestellt
	 *  werden, welche Eing�nge (k�nnen mehrere ein) sich ge�ndert haben. Jedem
	 *  Eingang ist ein Bit zugeordnet. */
	
	/*! Die Methode onCounterChanged wird aufgerufen, wenn sich ein Z�hlereingang
	 *  �ndert.
	 *  \param bfield gibt den Z�hlereingang C1,...,C4 an. C1 ist das L(east)S(significant)B(it). */  
	virtual void onCounterChanged(Bitfield bfield);

	/*! Die Methode onCounterConfChanged wird aufgerufen, wenn ein Z�hler neukonfiguriert
	 *  wird. Ein Z�hler wird bei der Motorsteuerung im intelligenten Modus konfiguriert.
	 *  \param bfield gibt den Z�hlereingang C1,...,C4 an. */
	virtual void onCounterConfChanged(Bitfield bfield);

	/*! Die Methode onInputChanged wird aufgerufen, wenn an einem der Universaleing�nge
	 *  ein neuer Wert anliegt. 
	 *  \param bfield gibt den Universaleingang I1,...,I8 an. I1 ist das LSB. */
	virtual void onInputChanged(Bitfield bfield);

	/*! Diese Methode onMotorStopped wird aufgerufen, wenn (i) der Motor im intelligenten
	 *  Modus seine endg�ltige Position erreicht hat (wird durch die definierten Drehimpulse
	 *  festgelegt) oder (ii) der Motor explizit gestoppt wird.
	 *  \param bfield gibt den Motor M1,...,M4 an. M1 ist das LSB. */
	virtual void onMotorStopped(Bitfield bfield);

	/*! Diese Methode wird in der aktuellen Version nicht unterst�tzt. */
	virtual void onMotorAborted(Bitfield bfield);
	
	/*! Diese Methode onTimerChanged wird aufgerufen, wenn ein oder mehrere Zeitgeber ausgel�st
	 *  sind. Die Zeitgeber m�ssen zuvor mit resetTimer() konfiguriert und mit startTimer() gestartet
	 *  worden sein.
	 *  \param bfield gibt den Zeitgeber TIMER1,...,TIMER8 an. */
	virtual void onTimerChanged(Bitfield bfield);

	/*! Die Methode onStart wird aufgerufen, wenn das reaktive Modell gestartet wird. Durch
	 *  �berladen der Methode k�nnen vorbereitende Arbeiten, beispielsweise das Einschalten
	 *  von Lampen, durchgef�hrt werden. */
	virtual void onStart();
	
	/*! Die Methode onStop wird aufgerufen, wenn das reaktive Modell beendet wird.
	 *  Durch �berladen der Methode k�nnen bestimmte Aufr�umarbeiten am Modell (z.B.
	 *  Lampen ausschalten) vorgenommen werden. */
	virtual void onStop();

	/*! Diese Methode sollte aufgerufen werden, wenn das Modell in einer Fehlersituation
	 *  sch befindet. Beispielsweise kann in abgeleiteten Klassen eine Lampe etc. 
	 *  die Fehlersituation signalisieren. */
	virtual void failure();

	/*! Diese Methode sollte aufgerufen werden, wenn das Modell aus einer Fehlersituation
	 *  in einen Normalzustand zur�ckkehrt. Diese Methode ist in Kombination zur failure()-Methode
	 *  zu impelementieren. */
	virtual void recover();		
	
	/*! Die Methode setIsFinished kann aus einem Modell heraus aufgerufen werden, um 
	 *  die Ausf�hrung des reaktiven Modells zu stoppen. Dieses kann beispielsweise in
	 *  Fehlerf�llen notwendig sein. 
	 *  \param val Gibt an, ob das Signal gesetzt (true) oder gel�scht (false) werden soll. */
	void setIsFinished(bool val =true);

	/*! Mit dieser Methode kann das Signal zum Beenden des reaktiven Modells abgefragt werden. */
	virtual bool isFinished();
	
	/*! Die Methode onError wird aufgerufen, wenn Fehlersitautionen erkannt werden. Die
	 *  Methode kann �berladen werden, ob die Ausgabe in eine Datei o.�. zu realisieren.
	 *  Standardm��ifg wird in den cerr-Fehlerstrom geschrieben.
	 *  \param err Gibt den Fehlertext an. */
	virtual void onError(const char* err);

	/*! Die folgenden Methoden erm�glichen es, die Bitmasken f�r die Signalisierung von
	 *  �nderungen an den Eing�ngen, Z�hlern und Motoren abzufragen. */
	Bitfield counterMask() const;
	Bitfield counterConfMask() const;
	Bitfield inputMask() const;
	Bitfield motorMask() const;

	/*! Diese Methode wird in der aktuellen Version nicht unterst�tzt. */
	Bitfield abortMask() const;

	/*! Die Methode erm�glicht es, einen Zeitgeber zu konfigurieren. Der
	 *  Zeitgeber wird aber erst durch den Aufruf der Methode startTimer() tats�chlich
	 *  aktiv. Ein bereits laufender Zeitgeber wird durch den Aufruf der resetTimer()-Methode
	 *  inaktiv und muss ggfs. neu gestartet werden. Es k�nnen maximal 8 Zeitgeber zeitgleich
	 *  aktiv sein.
	 *  \param tid Gibt den zu konfigurierenden Zeitgeber an. 
	 *  \param period Gibt die Periode in [ms] f�r den Zeitgeber an.
	 *  \param continuation Gibt die maximale Dauer (ggfs. unendlich) f�r den Zeitgeber an. */
	void resetTimer(TimerIdentifier tid, TimingInformation period =TIMING_1S, TimingInformation continuation =TIMING_FOREVER);

	/*! Die Methode startet einen zuvor mit resetTimer() konfigurierten Zeitgeber. Es ist m�glich,
	 *  einen zeitlichen Verzug des erstmaligen Ausl�sens zu definieren. Ist der
	 *  Zeitgeber bereits aktiv, so wird false ansonsten true zur�ckgegeben. 
	 *  \param tid Gibt den zu konfigurierenden Zeitgeber an.
	 *  \param offset Gibt den zeitlichen Verzug des Startens in [ms] an. */
	bool startTimer(TimerIdentifier tid, TimingInformation offset =TIMING_NOW);

	/*! Diese Methode stoppt einen zuvor gestarteten Zeitgeber. Die Methode liefert
	 *  false zur�ck, falls der Zeitgeber inaktiv ist. 
	 *  \param tid Gibt den zu konfigurierenden Zeitgeber an. */
	bool stopTimer(TimerIdentifier tid);

	/*! Diese Methode liefert die aktuelle Uhrzeit in [ms] seit Aktivierung des Modells zur�ck. */
	TimingInformation current() const;

	/*! Interne Methode zum Setzen der aktuellen Uhrzeit [Handle with Care!]. */
	void current(TimingInformation time);

	/*! Interne Methode zum Abfragen/Ver�ndern eines Zeitgebers [Handle with Care!]. */
	TimerModel& getTimer(TimerIdentifier tid);
		
protected:

	/*! Die folgenden Bitfelder geben an, f�r welche Eing�nge, Z�hler und Motoren
	 *  ein Signalisierung (Aufruf der entsprechenden on_XYZ_Changed-Methode) ausgef�hrt
	 *  werden soll. Standardm��ig wird jede �nderung ber�cksichtigt (die Masken
	 *  besitzen den Wert 0xff). Soll beispielsweise nur auf �nderungen am Universaleingang I1
	 *  reagiert werden, dann muss die Maske auf 0x01f gesetzt werden. */
	Bitfield	_counterMask;
	Bitfield	_counterConfMask;
	Bitfield	_inputMask;
	Bitfield	_motorMask;
	Bitfield    _abortMask;

private:

	/*! Bit zur Beendigung des reaktiven Modells. */
	bool _isFinished;

	/*! Datenstruktur f�r Timer-Funktionalit�t. */
	TimerModel _timer[NTIMER];
	
	/*! Aktuelle Zeit seit Start des Modells in [ms]. */
	TimingInformation _current;

};


#endif
