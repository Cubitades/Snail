///////////////////////////////////////////////////////////
//  CrossWalk.h
//  Implementation of the Class CrossWalk
//  Created on:      21-Mai-2010 11:53:23
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_09611158_F115_4625_A99B_BC864A6311D6__INCLUDED_)
#define EA_09611158_F115_4625_A99B_BC864A6311D6__INCLUDED_

#include "TrafficLight.h"
#include "PedestrianLight.h"
#include "Contact.h"
#include "Model.h"

/*! Die Template-Klasse GenericCrossWalk beschreibt einen Fu�g�nger�berweg in
 *  dem CrossRoads-Modell. Ein Fu�g�nger�berweg besteht aus einer Stra�enampel
 *  und 2 Fu�g�ngerampeln, die links und rechts der Stra�e angeordnet sind.
 *  Zus�tzlich befindet sich vor der Haltelinie ein Kontaktsensor.
 *  Hinweis: F�r die Verwendung einer konkreten Klasse ist die Klasse CrossWalk
 *  definiert, die die Klassen TrafficLight und PedestrianLight verwendet.
 */

template<class TRAFFIC, class PEDESTRIAN> class GenericCrossWalk : public Model
{

public:

	/*! Der Standardkonstruktor. */
	GenericCrossWalk();
	/*! Der Destruktor. */
	virtual ~GenericCrossWalk();

	/*! Mit dieser Methode wird die Verbindung zwischen dem Fu�g�nger�berweg und dem 
	 * Controller hergestellt. */
	virtual bool construct(Controller* controller);
	
	/*! Diese Methode liefert die Stra�enampel zur�ck. */
	TRAFFIC& main();
	/*! Diese Methode liefert die linke (in Fahrtrichtung) Fu�g�ngerampel zur�ck. */
	PEDESTRIAN& left();
	/*! Diese Methode liefert die rechte (in Fahrtrichtung) Fu�g�ngerampel zur�ck. */
	PEDESTRIAN& right();
	/*! Diese Methode liefert den Kontaktsensor vor der Haltelinie zur�ck. */
	Contact& stop();
	
private:
	TRAFFIC traffic;
	PEDESTRIAN pedestrianright;
	PEDESTRIAN pedestrianleft;
	Contact stopcontact;

};

/*! Diese Klasse definiert einen Fu�g�nger�berweg mit den beiden konkreten
 *  Implementierungen f�r die Stra�enampel und den beiden Fu�g�nger�berwegen.
 */
typedef GenericCrossWalk<TrafficLight,PedestrianLight> CrossWalk;


template<class TRAFFIC,class PEDESTRIAN> 
inline GenericCrossWalk<TRAFFIC,PEDESTRIAN>::GenericCrossWalk(){}

template<class TRAFFIC,class PEDESTRIAN> 
inline GenericCrossWalk<TRAFFIC,PEDESTRIAN>::~GenericCrossWalk(){}

template<class TRAFFIC,class PEDESTRIAN> 
inline TRAFFIC& GenericCrossWalk<TRAFFIC,PEDESTRIAN>::main(){
	return traffic;
}

template<class TRAFFIC,class PEDESTRIAN> 
inline PEDESTRIAN& GenericCrossWalk<TRAFFIC,PEDESTRIAN>::left(){
	return pedestrianleft;
}

template<class TRAFFIC,class PEDESTRIAN> 
inline PEDESTRIAN& GenericCrossWalk<TRAFFIC,PEDESTRIAN>::right(){
	return pedestrianright;
}

template<class TRAFFIC,class PEDESTRIAN> 
inline Contact& GenericCrossWalk<TRAFFIC,PEDESTRIAN>::stop(){
	return stopcontact;
}

template<class TRAFFIC,class PEDESTRIAN> 
inline bool GenericCrossWalk<TRAFFIC,PEDESTRIAN>::construct(Controller* controller){
	bool retvalue=Model::construct(controller);
	if (retvalue) {
		main().construct(controller);
		left().construct(controller);
		right().construct(controller);
		stop().Controlled::connect(controller);
	}
	return retvalue;
}

#endif // !defined(EA_09611158_F115_4625_A99B_BC864A6311D6__INCLUDED_)
