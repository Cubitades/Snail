#include "SnailRunner.h"


/*! Anschl�sse des Roboters an den TX Controller.
 *  Eing�nge:
 *  I1 : Farbsensor hinten
 *  I4 : Farbsensor unten
 *  I6 : Ultraschallsensor
 *	I7 : Kontaktsensor
 *  I8 : Akkuspannung
 *  C1 : Impulsz�hler rechter Motor
 *  C2 : Impulsz�hler linker Motor
 *
 *  Ausg�nge:
 *  M1 : Rechter Motor
 *  M2 : Linker Motor
 *  O5 : Rechte Lampe
 *  O6 : Linke Lampe
 *  O7 : Front Lampen
 *  O8 : Controller Lampe
 */


#define	INPUT_COLOUR_BACK				I1
#define INPUT_COLOUR_DOWN				I4
#define	INPUT_ULTRASONIC				I6
#define INPUT_PUSH_BUTTON				I7
#define INPUT_ACCU_LEVEL				I8

#define INPUT_ULTRASONIC_SIDE			I7 

#define COUNTER_RIGHT_MOTOR				C1
#define COUNTER_LEFT_MOTOR				C2

#define OUTPUT_RIGHT_MOTOR				M1
#define OUTPUT_LEFT_MOTOR				M2
#define OUTPUT_LAMP_RIGHT				O5
#define OUTPUT_LAMP_LEFT				O6
#define OUTPUT_LAMPS_FRONT				O7
#define OUTPUT_LAMP_CONTROLLER			O8

 /* --Diese Informationen sind f�r die Log-Datei zur Information. */
#define INFO_MOTOR_LEFT		"MOTOR COUNTER|LEFT"
#define INFO_MOTOR_RIGHT	"MOTOR COUNTER|RIGHT"
#define INFO_COLOUR_BACK	"COLOUR|BACK"
#define INFO_COLOUR_DOWN	"COLOUR|FRONT, DOWN"
#define INFO_DISTANCE		"DISTANCE|AHEAD"
#define INFO_DISTANCE_SIDE	"DISTANCE|SIDE"
#define INFO_ACCU_LEVEL		"ANALOG|POWER SUPPLY, ACCU LEVEL"
#define INFO_BUTTON			"BUTTON|PUSH BUTTON, RIGHT"
#define WINDOW_HEIGHT 500
#define WINDOW_WIDTH 1000

SnailRunner::SnailRunner() : leftmotor(INFO_MOTOR_LEFT), rightmotor(INFO_MOTOR_RIGHT), speed(350),
colourSensorback(INFO_COLOUR_BACK),
colourSensordown(INFO_COLOUR_DOWN),
distance(INFO_DISTANCE),
distance_side(INFO_DISTANCE_SIDE),
accuLevel(ANALOG_10KV, I1, INFO_ACCU_LEVEL),
pushButton(INFO_BUTTON),
ex_state(0), ob_state(0), se_state(0), fw_state(0), st_state(0), sl_state(0), mission(EXPLORE_MISSION), 
runner(500, 250), map(WINDOW_HEIGHT, WINDOW_WIDTH), window(sf::VideoMode(WINDOW_WIDTH+200/**/, WINDOW_HEIGHT/**/), "DUCS LIVE MAP") {}


SnailRunner::~SnailRunner(void) {
	delete ob_state;
	delete ex_state;
	delete se_state;
	delete fw_state;
	delete st_state;
	delete sl_state;
	
}

bool SnailRunner::construct(TxControllerSupervision* controller) {
	bool retvalue = Model::construct(controller);

	/* --Sensoren konfigurieren. */
	if (retvalue == true && distance.connect(INPUT_ULTRASONIC, controller) == false)
		retvalue = false;

	if (retvalue == true && colourSensorback.connect(INPUT_COLOUR_BACK, controller) == false)
		retvalue = false;

	if (retvalue == true && colourSensordown.connect(INPUT_COLOUR_DOWN, controller) == false)
		retvalue = false;

	if (retvalue == true && accuLevel.connect(INPUT_ACCU_LEVEL, controller) == false)
		retvalue = false;

	if (retvalue == true && pushButton.connect(INPUT_PUSH_BUTTON, controller) == false)
		retvalue = false;
	
	if (retvalue == true && distance_side.connect(INPUT_ULTRASONIC_SIDE, controller) == false)
		retvalue = false;

	/* --Aktoren konfigurieren. */
	/* --Motoren. */
	if (retvalue == true &&
		(leftmotor.connect(OUTPUT_LEFT_MOTOR, COUNTER_LEFT_MOTOR, controller) == false ||
			rightmotor.connect(OUTPUT_RIGHT_MOTOR, COUNTER_RIGHT_MOTOR, controller) == false))
		retvalue = false;
	else
		/* --Synchronisation der Motoren. */
		leftmotor.synchronize(&rightmotor);

	/* --Lampen. */
	if (retvalue == true &&
		(lamp_control.connect(OUTPUT_LAMP_CONTROLLER, controller) == false ||
			lamp_front.connect(OUTPUT_LAMPS_FRONT, controller) == false ||
			lamp_left.connect(OUTPUT_LAMP_LEFT, controller) == false ||
			lamp_right.connect(OUTPUT_LAMP_RIGHT, controller) == false))
		retvalue = false;

	/* --Zustandsmaschine erzeugen. */
	ex_state = new ExploreStateMachine(this);
	ob_state = new ObstacleStateMachine(this);
	fw_state = new ForwardStateMachine(this);
	se_state = new SearchStateMachine(this);
	st_state = new StartStateMachine(this);
	sl_state = new StartLauferMachine(this);

	/* --Interne Attribute setzen. */
	speed = 350;
	last_colour_down = colourdown().value();
	last_colour_back = colourback().value();
	last_dis = ahead().value();


	return retvalue;
}

bool SnailRunner::forward(double val, RunUnit unit) { 
	return leftmotor.turnOn(val, unit, speed, LEFT, LEFT);
}

bool SnailRunner::backward(double val, RunUnit unit) {
	return leftmotor.turnOn(val, unit, speed, RIGHT, RIGHT);
}

bool SnailRunner::turnleft(double val, RunUnit unit) {
	return leftmotor.turnOn(val, unit, speed, LEFT, RIGHT);
}

bool SnailRunner::turnright(double val, RunUnit unit) {
	return leftmotor.turnOn(val, unit, speed, RIGHT, LEFT);
}

bool SnailRunner::stop() {
	return leftmotor.turnOff();
}


void SnailRunner::turn(double grad) {
	double val = 0;
	RunUnit unit = IMPULSE;
	grad = grad * direction;
	if (grad > 0)
	{
		val = grad * grad_in_impulse * uebersetzungsverhaeltnis_getriebe * radUmdrehungproGrad;
		leftmotor.turnOn(val, unit, speed, RIGHT, LEFT); // wenn Drehwinkel >0 dann dreht links
	}
	else if (grad < 0)
	{
		double grad_neu = abs(grad);
		val = grad_neu * grad_in_impulse * uebersetzungsverhaeltnis_getriebe * radUmdrehungproGrad;
		leftmotor.turnOn(val, unit, speed, LEFT, RIGHT); // wenn Drehwinkel <0 dann dreht rechts
		//cout << grad_neu << endl;
	}
	else
	{
		leftmotor.turnOff(); // wenn Drehwinkel gleich Null dann stoppen
	}
}
/*! Diese Methode wird aufgerufen, wenn ein oder mehrere Motoren
 *  am Roboter gestoppt sind. Dieses kann passieren, wenn (i) der
 *  vorgegebene Wert vomn Drehimpulsen erreicht ist, oder (ii) der
 *  Motor explizit gestoppt wurde.
 *
 *  Die Methode �berpr�ft, ob Motor 1 und/oder Motor2 angehalten hat
 *  und erzeugt dass Ereignis "IS_STOPPED", welches dann in der
 *  Zustandsmaschine zu einer Transition (Zustandswechsel) f�hrt.
 *  Das Verhalten des Roboters ist in den Dateien Mission.{cpp|h}
 *  beschrieben. */

void SnailRunner::onMotorStopped(Bitfield bfield) {
	// --�berpr�fe, ob die Motoren gestoppt sind.
	if (bfield&((1 << OUTPUT_LEFT_MOTOR) | (1 << OUTPUT_RIGHT_MOTOR)))
		// --�berpr�fe welche Mission.
		if (mission == OBSTACLE_MISSION)
			ob_state->handle(ObstacleStateMachine::Event::IS_STOPPED);
		else if (mission == EXPLORE_MISSION)
			ex_state->handle(ExploreStateMachine::Event::IS_STOPPED);
		else if (mission == SEARCH_MISSION)
			se_state->handle(SearchStateMachine::Event::IS_STOPPED);
		else if (mission == FORWARD_MISSION)
			fw_state->handle(ForwardStateMachine::Event::IS_STOPPED);
		else if (mission == START_MISSION && SnailRunner::vergleich_ecke(SnailRunner::corner_amount) == false)
			st_state->handle(StartStateMachine::Event::IS_STOPPED);
		else if (mission == START_LAUFER_MISSION)// && SnailRunner::vergleich_runde(SnailRunner::SollRunde) == false)
		{
			cout << "EVENT: IS_STOPPED" << endl;
			sl_state->handle(StartLauferMachine::Event::IS_STOPPED);		
		}
			
}

/*! Diese Methode �berpr�ft bestimmte Sensoreing�nge. Wenn Schwellenwerte
 *  �berschritten werden, dann werden Ereignisse erzeugt und diese
 *  einer Zustandsmaschine �bergeben. In der Zustandsmaschine wird dann
 *  eine Transition durchgef�hrt (in Abh�ngigkeit der Mission).
 *
 *  Mission 1 (Spurverfolgung)
 *  Wenn der Farbwert (THRESHOLD_COLOR) unterschritten wird, dann wird das
 *  Ereignis OFF_TRAIL ausgel�st. Liegt der aktuelle Farbwert wieder �ber
 *  dem Schwellenwert, dann wird das Ereignis ON_TRAIL ausgel�st.
 *
 *  Mission 2 (Braunsche Bewegung)
 *  Wenn der Distanzwert (THRESHOLD_DISTANCE_LOW) unterschritten wird, dann
 *  wird das Ereignis WALL_AHEAD ausgel�st. Liegt der aktuelle Distanzwert
 *  wieder oberhalb von THRESHOLD_DISTANCE_HIGH, dann wird das Ereignis
 *  CLEAR_VIEW ausgel�st.
 *
 *  Das eigentliche Verhalten des Roboters ist in den Zustandsmaschinen in
 *  den Dateien Mission.{cpp|h} beschrieben.
 */
	
void SnailRunner::onInputChanged(Bitfield bfield) {
	const int THRESHOLD_COLOR = 1400;
	const int THRESHOLD_DISTANCE_LOW = 30;
	const int THRESHOLD_DISTANCE_HIGH = 40;
	int THRESHOLD_DISTANCE = threshold_distance;
	int THRESHOLD_DISTANCE_SIDE = threshold_distance_no_side;
	const int THRESHOLD_COLOR_GRAU_WEISS = 1300;
	const int THRESHOLD_COLOR_GRAU_SCHWARZ = 1600;
	const int THRESHOLD_COLOR_BACK_MIN = 1000;
	const int THRESHOLD_GRADIENT = 275;
	/*
	window.clear();
	window.draw(map);
	window.draw(runner);
	window.display();
	*/
	//livemapstart();
	if (bfield&(1 << INPUT_COLOUR_DOWN)) {

		int col = colourdown().value();
		double secant = 2000;
		graydient.push(col);

		if (graydient.size() >= 4)
		{
			secant = pow((col - graydient.front()) / 4, 2);	//Quadrat der Steigung zwischen dem aktuellen und 4 letzten Wert
			graydient.pop();
		}

		// --�berpr�fe, ob Schwellenwert �berschritten.
		if (col > threshold_grey_high && last_colour_down < threshold_grey_high) //when the robots drives off course/on black... OFF_TRAIL
		{
			if (mission == EXPLORE_MISSION)
				ex_state->handle(ExploreStateMachine::Event::OFF_TRAIL);
			else if (mission == SEARCH_MISSION)
				se_state->handle(SearchStateMachine::Event::ON_TRAIL);
			else if (mission == OBSTACLE_MISSION)
				ob_state->handle(ObstacleStateMachine::Event::ON_TRAIL);
			else if (mission == START_MISSION) 
				st_state->handle(StartStateMachine::Event::OFF_TRAIL);
			else if (mission == START_LAUFER_MISSION)
			{
				{
					cout << "EVENT: OFF_TRAIL" << endl;
					sl_state->handle(StartLauferMachine::Event::OFF_TRAIL);
				}
			}
		}

		else if (col < threshold_grey_low && last_colour_down > threshold_grey_low) //when the robot is back on track/white... ON_TRAIL
		{
			if (mission == EXPLORE_MISSION)
				ex_state->handle(ExploreStateMachine::Event::ON_TRAIL);
			else if (mission == FORWARD_MISSION)
				fw_state->handle(ForwardStateMachine::Event::OFF_TRAIL);
			else if (mission == START_MISSION)
				st_state->handle(StartStateMachine::Event::ON_TRAIL);
			else if (mission == START_LAUFER_MISSION)
			{
				cout << "EVENT: ON_TRAIL" << endl;
				sl_state->handle(StartLauferMachine::Event::ON_TRAIL);
			}
		}
	
		else if (secant < 500 && last_Gradient >= 300 && col < threshold_grey_high && col > threshold_grey_low) //when the robot is in a gray area... ON_GREY
		{
			if (mission == START_MISSION)
				st_state->handle(StartStateMachine::Event::ON_GREY);
			else if (mission == START_LAUFER_MISSION)
			{					
				if (sl_state->state() == StartLauferMachine::State::ON_TRAIL)
					cout << "EVENT: ON_GREY" << endl;
					sl_state->handle(StartLauferMachine::Event::ON_GREY);
			}
		}

		last_Gradient = secant;
		last_colour_down = col;
	}

	if (bfield&(1 << INPUT_COLOUR_BACK)) {
		int col = colourback().value();
		if (mission == START_LAUFER_MISSION)
		{
			
				col = colourback().value();

				if (col > THRESHOLD_COLOR_BACK_MIN && last_colour_back < THRESHOLD_COLOR_BACK_MIN)
				{
					if (mission == START_LAUFER_MISSION)
					{
						if (sl_state->state() == StartLauferMachine::State::ENDE)
						{
							cout << "EVENT: LICHT_HINTEN" << endl;
							sl_state->handle(StartLauferMachine::Event::LICHT_HINTEN);
						}

					}
				}
				if (col < THRESHOLD_COLOR_BACK_MIN && last_colour_back > THRESHOLD_COLOR_BACK_MIN)
				{
					if (mission == START_LAUFER_MISSION)
					{
						if (sl_state->state() == StartLauferMachine::State::ENDE)
						{
							cout << "EVENT: NOT_LICHT_HINTEN" << endl;
							sl_state->handle(StartLauferMachine::Event::NOT_LICHT_HINTEN);
						}
						

					}
				}
				last_colour_back = col;
			
		}		
	}

	if (bfield&(1 << INPUT_ULTRASONIC)) {

		if (mission == OBSTACLE_MISSION)
		{
			// --�berpr�fe, ob Schwellwert �berschritten.
			int dis = ahead().value();

			if (dis >= THRESHOLD_DISTANCE && last_dis < THRESHOLD_DISTANCE) 
				ob_state->handle(ObstacleStateMachine::Event::CLEAR_VIEW);
			else if (dis < THRESHOLD_DISTANCE && last_dis >= THRESHOLD_DISTANCE) 
				ob_state->handle(ObstacleStateMachine::Event::WALL_AHEAD);

			last_dis = dis;
		}
		else if (mission == START_MISSION) {

			int dis = ahead().value();
			if (st_state->state() == StartStateMachine::State::START)
			{
				dis = ahead().value();

				if (dis >= THRESHOLD_DISTANCE && last_dis < THRESHOLD_DISTANCE)
					st_state->handle(StartStateMachine::Event::NOT_WALL_AHEAD);
				else if (dis < THRESHOLD_DISTANCE && last_dis >= THRESHOLD_DISTANCE)
					st_state->handle(StartStateMachine::Event::WALL_AHEAD);

			}
			last_dis = dis;
		}
		else if (mission == START_LAUFER_MISSION) {

			int dis = ahead().value();
			{
				dis = ahead().value();

				if (dis >= THRESHOLD_DISTANCE && last_dis < THRESHOLD_DISTANCE) {
					cout << "EVENT: NOT_WALL_AHEAD" << endl;
					sl_state->handle(StartLauferMachine::Event::NOT_WALL_AHEAD);
					
				}
				else if (dis < THRESHOLD_DISTANCE && last_dis >= THRESHOLD_DISTANCE) {
					cout << "WALL_AHEAD" << endl;
					sl_state->handle(StartLauferMachine::Event::WALL_AHEAD);
					
				}

			}
			last_dis = dis;
		}
	}
	if (bfield&(1 << INPUT_ULTRASONIC_SIDE)) {
		if (mission == OBSTACLE_MISSION) {
			// --�berpr�fe, ob Schwellwert �berschritten.
			int dis_side = side().value();

			if (dis_side >= THRESHOLD_DISTANCE_SIDE && last_dis_side <= THRESHOLD_DISTANCE_SIDE) {
				ob_state->handle(ObstacleStateMachine::Event::NO_SIDEWALL);
			}
			last_dis_side = dis_side;
		}
		if (mission == START_LAUFER_MISSION) {
			// --�berpr�fe, ob Schwellwert �berschritten.
			int dis_side = side().value();

			if (dis_side >= THRESHOLD_DISTANCE_SIDE && last_dis_side < THRESHOLD_DISTANCE_SIDE) {
				sl_state->handle(StartLauferMachine::Event::NO_SIDEWALL);
			}
			else if (dis_side <= threshold_distance_side_close && last_dis_side >= threshold_distance_side_close && last_dis_side <= THRESHOLD_DISTANCE_SIDE) {
				sl_state->handle(StartLauferMachine::Event::TOO_CLOSE);
			}
			else if ( dis_side >= threshold_distance_side_far && dis_side <= THRESHOLD_DISTANCE_SIDE &&  last_dis_side <= THRESHOLD_DISTANCE_SIDE) {
				sl_state->handle(StartLauferMachine::Event::TOO_FAR);
			}
			
			last_dis_side = dis_side;
		}
	}
}


/*! Diese Methode wird zum Beginn des reaktiven Modus des Roboters ausgef�hrt.
 *  Es wird die Controller-Lampe angeschaltet und dann in den ersten Zustand der
 *  jeweiligen Zustandsmaschine gewechselt. */

void SnailRunner::onStart() {
	lampcontroller().on();
	if (mission == OBSTACLE_MISSION)
		ob_state->start();
	else if (mission == EXPLORE_MISSION)
		ex_state->start();
	else if (mission == FORWARD_MISSION)
		fw_state->start();
	else if (mission == SEARCH_MISSION)
		se_state->start();
	else if (mission == START_MISSION)
		st_state->start();
	else if (mission == START_LAUFER_MISSION)
		sl_state->start();
}

/*! Diese Methode wird beim Beenden des reaktiven Modus des Roboters ausgef�hrt.
 *  Es wird die blaue Lampe wieder ausgeschaltet. */

void SnailRunner::onStop() {
	lampcontroller().off();
	stop();
}

void SnailRunner::onCounterChanged(Bitfield /*bfield*/) {}

/* -- Vergleich-Funktion -- */
bool SnailRunner::vergleich_ecke(int corner_amount) {
	if (corner_amount == st_state->ecke_cnt())
	{
		st_state->handle(StartStateMachine::Event::ECKEN_CNT);
		return true; 	
	}
	else
	{
		return false;
	}

}

/* -------------- SFML --------------------*/

/* --  Stand 31.07.2023: Livemapstart mit Aktualisierung mit Zeitabstand von 500ms hat gelungen, braucht nur die Position zu aktualisieren -- */
/* --  snail.setRotation (orientation) zeichnet wenn der Snail dreht -- */
/* --  snail.setPosition(x,y) zeichnet Position vom Snail -- */
/* -- Da die Map ist jede 500ms aktualisiert wird, soll die Strecke vor und nach 500ms gespeichert wird, um den Differenz zu berechnen und damit kann die Map anhand dieser Differenzstrecke aktualisieren -- */
/* --  Idee f�r die Livemap: - V
							 - wenn ecke = 0 : - vor ON_TRAIL : Strecke steigt X, y - Pixelwert nimmt X ab  
											   - nach ON_TRAIL : Strecke steigt X, x - Pixelwert nimmt X ab  
							 - wenn ecke = 1 : Strecke steigt X, y - Pixelwert nimmt X ab
							 - wenn ecke = 2 : Strecke steigt X, x - Pixelwert nimmt X zu 
							 - wenn ecke = 3 : Strecke steigt X, y - Pixelwert nimmt X zu
							 - wenn ecke = 4 : Strecke steigt X, x - Pixelwert nimmt X ab 

*/
void SnailRunner::livemapstart()
{
	double strecke = 0;
	double last_strecke = 0;
	double streckedifferenz = 0;
	double streckedifferenz_pixel = 0;
	double lapdistance_livemap_differenz = 0;
	double posX_Start = (WINDOW_WIDTH / 200.0) * 91.0;
	double posY_Start = (WINDOW_WIDTH / 200.0) * 90.0;	
	double posX_AUSRICHTEN3 = (WINDOW_WIDTH / 200.0) * 91.0;
	double posY_AUSRICHTEN3 = (WINDOW_WIDTH / 200.0) * 67.0;
	//while (window.isOpen())
	if (mission == START_LAUFER_MISSION)
	{
		while (sl_state ->state() != StartLauferMachine::State::FINAL)
		{
			//WaitUntilIsOver(200);

			if (sl_state->state() == StartLauferMachine::State::AUSRICHTEN_3)
			{
				orientation = 270.0;
				runner.setRotation(orientation);
				//cout << "Set Rotation 270" << endl;
				runner.setPosition(posX_AUSRICHTEN3, posY_AUSRICHTEN3);
				//cout << "Set Position 430 390" << endl;
				
			}
			
			else if (sl_state->state() == StartLauferMachine::State::SUCHEN || sl_state->state() == StartLauferMachine::State::START || sl_state->state() == StartLauferMachine::State::AUSRICHTEN || sl_state->state() == StartLauferMachine::State::AUSRICHTEN_2)
			{
				runner.setPosition(posX_Start, posY_Start);
				runner.setRotation(0);
				window.clear();
				//window.draw(map);
				window.draw(runner);
				window.display();
			}
			
			else
			{
				// aktualisiert den Impulswert vom Motor
				if (sl_state->count < 2)
				{
					lapdistance_livemap = left().encoder().value();
					//lapdistance_livemap_alt = lapdistance_livemap;
					//lapdistance_livemap_differenz = lapdistance_livemap - lapdistance_livemap_alt;
				}
				

				// Liefert der jetztigen Position (Strecke) zur�ck
				strecke = ImpulseToDistance(lapdistance_livemap);
			
				if ((strecke - last_strecke) > 0)
				{
					streckedifferenz = strecke - last_strecke;
				}
				else
				{
					streckedifferenz = 0;
				}
				
				streckedifferenz_pixel = convertToPixels(streckedifferenz, WINDOW_WIDTH);
				if (sl_state->obstacle == true)
				{
					if (current_corner == 0)
					{
						posX_AUSRICHTEN3 -= streckedifferenz_pixel;
						streckedifferenz_pixel = 0;
						streckedifferenz = 0;
						runner.setPosition(posX_AUSRICHTEN3, posY_AUSRICHTEN3);
						//runner.setRotation(270);
						
					}
					else if (current_corner == 1)
					{
						posY_AUSRICHTEN3 -= streckedifferenz_pixel;
						streckedifferenz_pixel = 0;
						streckedifferenz = 0;
						runner.setPosition(posX_AUSRICHTEN3, posY_AUSRICHTEN3);
						runner.setRotation(0);
						
					}
					else if (current_corner == 2)
					{
						posX_AUSRICHTEN3 += streckedifferenz_pixel;
						streckedifferenz_pixel = 0;
						streckedifferenz = 0;
						runner.setPosition(posX_AUSRICHTEN3, posY_AUSRICHTEN3);
						runner.setRotation(90);
						
					}
					else if (current_corner == 3)
					{
						posY_AUSRICHTEN3 += streckedifferenz_pixel;
						streckedifferenz_pixel = 0;
						streckedifferenz = 0;
						runner.setPosition(posX_AUSRICHTEN3, posY_AUSRICHTEN3);
						runner.setRotation(180);
						
					}
					else
					{
						posX_AUSRICHTEN3 -= streckedifferenz_pixel;
						streckedifferenz_pixel = 0;
						streckedifferenz = 0;
						runner.setPosition(posX_AUSRICHTEN3, posY_AUSRICHTEN3);
						runner.setRotation(270);
						
					}
				}

				else
				{
					//bewegt nach oben, Y-Wert nimmt ab
					if (current_corner_obstacle == 1)
					{
						posY_AUSRICHTEN3 -= streckedifferenz_pixel;
						streckedifferenz_pixel = 0;
						streckedifferenz = 0;
						runner.setPosition(posX_AUSRICHTEN3, posY_AUSRICHTEN3);
						runner.setRotation(0);

					}

					//bewegt nach rechts, X-Wert nimmt zu
					else if (current_corner_obstacle == 2)
					{
						posX_AUSRICHTEN3 += streckedifferenz_pixel;
						streckedifferenz_pixel = 0;
						streckedifferenz = 0;
						runner.setPosition(posX_AUSRICHTEN3, posY_AUSRICHTEN3);
						runner.setRotation(90);

					}

					//bewegt nach unten, Y-Wert nimmt zu
					else if (current_corner_obstacle == 3)
					{
						posY_AUSRICHTEN3 += streckedifferenz_pixel;
						streckedifferenz_pixel = 0;
						streckedifferenz = 0;
						runner.setPosition(posX_AUSRICHTEN3, posY_AUSRICHTEN3);
						runner.setRotation(180);

					}

					//bewegt nach links, X-Wert nimmt ab
					else if (current_corner_obstacle = 4)
					{
						posX_AUSRICHTEN3 += streckedifferenz_pixel;
						streckedifferenz_pixel = 0;
						streckedifferenz = 0;
						runner.setPosition(posX_AUSRICHTEN3, posY_AUSRICHTEN3);
						runner.setRotation(90);

					}

				}

				


				sf::Event event;
				while (window.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
						window.close();
				}

				SFMLwaypoint wp(runner.getPosition().x, runner.getPosition().y);
				waypoints.push_back(wp);

				window.clear();
				//window.draw(map);

				for (unsigned int i = 0; i < obstacles.size(); i++)
				{
					window.draw(obstacles.at(i));
				}

				for (unsigned int i = 0; i < waypoints.size(); i++)
				{
					window.draw(waypoints.at(i));
				}

				window.draw(runner);
				window.display();
			
				// Liefert der vorherigen Position (Strecke) (vor 500ms)
				last_strecke = strecke;
			
				//cout << "LIVE MAP IST GELUNGENNNNNNNNNNN" << endl;
				//cout << "Strecke: " << strecke << endl;
				//cout << "Last_strecke: " <<last_strecke << endl;
			}
			
		}

	}
	
}

double SnailRunner::convertToPixels(double distanceCm, int windowWidth)
{
	// Angenommen, das Fenster hat eine Bildh�he (windowHeight) und Bildbreite (windowWidth) in Pixeln
	// Berechne das Verh�ltnis zwischen der Fensterh�he und der tats�chlichen Plattenbreite (in cm)
	double pixelsPerCm = static_cast<double> (windowWidth / 200.0);

	// Multipliziere das Verh�ltnis mit der Strecke, um die Anzahl der Pixel zu berechnen
	double distancePixels = distanceCm * pixelsPerCm;

	// R�ckgabe der berechneten Pixelanzahl
	return distancePixels;
}

void SnailRunner::calculateOrientation(double value)
{
	// Add the value to the current orientation
	orientation += value;

	// Normalize the orientation to be within the range [0, 359]
	orientation = fmod(orientation, 360.0);
	if (orientation < 0.0)
	{
		orientation += 360.0;
	}
	cout << "Orientation ist: " << orientation << endl;
}

double SnailRunner:: ImpulseToDistance(double lapdistance_livemap)
{
	// distance in cm
	double distance;
	distance = ((lapdistance_livemap / 60.0) / 2.0)*M_PI*0.05 * 100; // multipliziere 100, um von m zu cm umzuwandeln
	return distance;
}

double SnailRunner::impulse_in_grad(double impulse)
{
	double ergebnis = 0;
	ergebnis = impulse / radUmdrehungproGrad / uebersetzungsverhaeltnis_getriebe / grad_in_impulse;
	return ergebnis;
}