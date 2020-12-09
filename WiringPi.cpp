#include "Divers.h"

/**
*Constructeur de la classe WiringPi.
*\param[in] btn1 Bouton servant a faire defiler les plantxas lors de la selection
*\param[in] btn2 Bouton servant a valider le choix de la plantxa lors de la selection
*/
WiringPi::WiringPi(const int btn1, const int btn2) {

	//Appel de la fonction pour initialiser WiringPi 
	wiringPiSetup();

	//Déclaration des bouttons comme entrées
	pinMode(BUTTON_BLACK, INPUT);
	pinMode(BUTTON_RED, INPUT);

	flow.open("/dev/ttyACM0");
	flow << "\xFE\x58";
	flow << "Hello there :)";
	flow.close();
}

/**
*Nettoyer l'afficheur LCD.
*/
void WiringPi::clearLCD() {
	flow.open("/dev/ttyACM0");
	flow << "\xFE\x58";
	flow.close();
}

/**
*Detecter le choix du client lors de la selection de la plantxa.
*\param[in] nCarte Le numero de la carte
*/
void WiringPi::detectButtons(int nCarte) {

	flow.open("/dev/ttyACM0");
	flow << "\xFE\x58";
	flow << "Plantxa: 1";
	flow.close();

	choixPlantxa = 1;
	finished = false;

	do {
		if (digitalRead(BUTTON_BLACK)) {
			if (choixPlantxa < maxPlantxaNb) {
				choixPlantxa++;
				flow.open("/dev/ttyACM0");
				flow << "\xFE\x58";
				flow << "Plantxa: ";
				flow << choixPlantxa;
				flow.close();
				delay(500);
			}
			else {
				choixPlantxa = 0;
			}
			finished = false;
			delay(500);

		}

		if (digitalRead(BUTTON_RED)) {

			Plantxa plantxa;

			std::string plantxaState = plantxa.getPlantxaState(choixPlantxa);

			if (plantxaState == "False") {

				std::cout << "Plantxa disponible" << endl;
				flow.open("/dev/ttyACM0");
				flow << "\xFE\x58";
				flow << "Vous avez choisi la plantxa: ";
				flow << choixPlantxa;
				flow.close();

				plantxa.setPlantxaUsed(choixPlantxa, "True");
				plantxa.setCookingKeys(choixPlantxa, nCarte);
				finished = true;
				delay(500);

			}
			else if (plantxaState == "True") {
				std::cout << "Plantxa indisponible" << endl;
				flow.open("/dev/ttyACM0");
				flow << "\xFE\x58";
				flow << "Plantxa indisponible";
				flow.close();
				delay(500);
			}
		}
	} while (finished == false);
}

/**
*Afficher un message specifique sur l'afficheur LCD pendant un certain temps.
*\param[in] message Le message a afficher
*\param[in] timeOnScreen Le temps durant lequel le message sera affiche sur l'afficheur LCD
*/
void WiringPi::displayMessage(const std::string message, unsigned int timeOnScreen) {

	flow.open("/dev/ttyACM0");
	flow << "\xFE\x58";
	flow << message;
	flow.close();

	std::this_thread::sleep_for(std::chrono::seconds(timeOnScreen));
	flow.open("/dev/ttyACM0");
	flow << "\xFE\x58";
	flow.close();
}
