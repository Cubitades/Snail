#ifndef _SUPERVISION_H
#define _SUPERVISION_H

#include "Makros.h"
#include "ReactiveModel.h"

/*! Dieser Aufz�hlungstyp definiert, wie das Fischertechnik-Modell ausgef�hrt werden soll. 
 *  Durch Auswahl NON_REACTIVE werden die on_XYZ_Changed-Methoden nicht automatisch aufgerufen.
 *  Es k�nnen aber alle Sensorwerte abgefragt und auch die Motoren angesteuert werden. Durch
 *  Auswahl von REACTIVE wird die onStart-Methode des Modells ausgef�hrt und im folgenden bei
 *  �nderungen an den Eing�ngen/Motoren die zugeh�rigen on_XYZ_Changed-Methoden. Wichtiger
 *  Hinweis: Ist das Modell im reaktiven Modus, dann sollte die Steuerung des Modells nur durch
 *  die on_XYZ_Changed-Methoden erfolgen! */
enum ReactiveModelMode {
	NON_REACTIVE,	/*! Einfacher Modus. */ 
	REACTIVE		/*! Reaktiver Modus. */
};

/*! Dieser Aufz�hlungstyp wird zzt. nicht unterst�tzt. */
enum ReactiveModelPlayback {NO_PLAYBACK, PLAYBACK_ONCE, PLAYBACK_LOOP} ;

/*! Diese Funktion startet die Ausf�hrung eines Fischertechnikmodells. Erst nach Aufruf dieser
 *  Funktion wird das Modell aktiv. Alle �nderungen der Sensorwerte und Motorenzust�nde werden
 *  mit einer hohen zeitlichen Aufl�sung in einer Datei geschrieben. Der Inhalt dieser Datei kann
 *  zur Fehlersuche eingesetzt werden. Die Datei wird mit jedem Aufruf der StartSupervision-Funktion
 *  �berschrieben. 
 *	\param model Gibt das Fischertechnikmodell an.
 *	\param mode Gibt den Modus der Ausf�hrung an.
 *	\param ofile Gibt den Dateinamen zur Ausgabe von Sensorwerten etc. zu Debugzwecken an
 *	\param pback Wird zzt. nicht unterst�tzt.
 *	\param ifile Wird zzt. nicht unterst�tzt. */
DLL_DECLSPEC void StartSupervision(ReactiveModel* model, ReactiveModelMode mode = REACTIVE, const char* ofile = "txcontroller.txt",
		ReactiveModelPlayback pback =NO_PLAYBACK, const char* ifile =0);

/*! Mit dieser Funktion kann die Ausf�hrung eines Fischertechnik-Modells beendet werden. */
DLL_DECLSPEC void StopSupervision();

/*! Interne Funktionen. */
DLL_DECLSPEC int GetUniversalInput(InputPort port);
DLL_DECLSPEC int GetCounter(InputPort port);
DLL_DECLSPEC int GetCounterConfiguration(InputPort port);
DLL_DECLSPEC void SetAnnotationSupervision(const std::string& note);

#endif
