#include "Divers.h"

//Variables NFC
const char auth01[] = "\xFF\x86\x00\x00\x05\x01\x00\x08\x61\x00";
const char read_binary01[] = "\xFF\xB0\x00\x08\x10";
const char write01[] = "\xFF\xD6\x00\x08\x10";

const char auth03[] = "\xFF\x86\x00\x00\x05\x01\x00\x0A\x61\x00";
const char read_binary03[] = "\xFF\xB0\x00\x0A\x10";
const char write03[] = "\xFF\xD6\x00\x0A\x10";

const char auth04[] = "\xFF\x86\x00\x00\x05\x01\x00\x0C\x61\x00";
const char read_binary04[] = "\xFF\xB0\x00\x0C\x10";
const char write04[] = "\xFF\xD6\x00\x0C\x10";

//Variables MQTT
const char* serverURI1 = "tcp://10.0.0.18:1883";
const char* clientId1 = "RPI";
const int QOS = 2;
const char* topicEnv = "EuskalPlantxa/MQTT";
const char* topicSubs1 = "EuskalPlantxa/MQTT/LED";
std::string messageRecuTopic1{ "" };
std::string messageRecuTopic2{ "" };


//Déclaration des fonctions
void InitialisationCarteDebug(NFC &myNFC);
void IsCardOnReader(NFC &myNFC);
int main() {

	bool isCardReady{ true };

	//Instanciation des objets
	NFC myNFC;
	MQTT MQTT(serverURI1, clientId1, &messageRecuTopic1, &messageRecuTopic2);
	WiringPi wiringPi(WiringPi::BUTTON_BLACK, WiringPi::BUTTON_RED);
	cURL cURL;
	Plantxa plantxa;

	//Souscription aux topics
	MQTT.souscrire(topicEnv, QOS);
	MQTT.souscrire(topicSubs1, QOS);

	while (1) {

		isCardReady = true;

		//Partie établissement NFC
		myNFC.connection(isCardReady);
		myNFC.etablissement();
		myNFC.lecture();
		myNFC.datas();
		myNFC.load_default();

		if (isCardReady == true) {

			//Appel fonction création du profil sur la carte NFC.
			//InitialisationCarteDebug(myNFC);

			//Récupérer l'ID de la carte pour faire la liaison avec l'ID de la plantxa
			std::string name{ "" };
			myNFC.authentification(auth01);
			myNFC.read_binary(read_binary01, name);
			name.erase(remove(name.begin(), name.end(), '\n'), name.end());	//Enlève le '\n' à la fin du nom
			int cardID = plantxa.getCardID(name);

			//Récupération du bloc 4 pour savoir si le client a déjà badgé
			std::string userAlreadyChecked{ "" };
			myNFC.authentification(auth04);
			myNFC.read_binary(read_binary04, userAlreadyChecked);
			userAlreadyChecked.erase(remove(userAlreadyChecked.begin(), userAlreadyChecked.end(), '\n'), userAlreadyChecked.end());	//Enlève le '\n' à la fin du mot

			//Récupération du bloc 3 pour obtenir le temps restant sur la carte
			std::string x{ "" };
			myNFC.authentification(auth03);
			myNFC.read_binary(read_binary03, x);
			x.erase(remove(x.begin(), x.end(), '\n'), x.end());	//Enlève le '\n' à la fin du mot
			int timeLeft = stoi(x);

			if ((timeLeft > 0) && (userAlreadyChecked == "False")) {

				//Envoie du message MQTT du temps restant
				MQTT.envoieMessage(x, QOS, topicSubs1);
				usleep(500);

				//Mise à zéro du temps sur la carte
				myNFC.update_binary("0", write03);
				wiringPi.displayMessage("Le credit a ete mis a 0...", 2);
				std::cout << "\nLe credit de la carte a ete mis a 0..." << endl;
				wiringPi.displayMessage("Selection de la plantxa..", 1);
				std::cout << "Selection de la plantxa.." << endl;

				//Sélection de la plantxa
				wiringPi.detectButtons(cardID);
				myNFC.authentification(auth04);
				myNFC.update_binary("True", write04);

			}
			else if ((timeLeft == 0) && (userAlreadyChecked == "False")) {
				std::cout << "\nTemps de cuisson epuise, veuillez vous rendre a l'acceuil." << endl;
				wiringPi.displayMessage("Temps de cuisson epuise", 3);

			}
			else if ((timeLeft == 0) && (userAlreadyChecked == "True")) {
				std::cout << "\nLe temps est egal a 0: recreditation..." << endl;
				wiringPi.displayMessage("Recreditation en cours..", 3);

				//Récupération du temps à re-créditer à l'aide d'une requête GET interrogeant la BDD
				std::string temps = plantxa.getTimeLeft(name);

				myNFC.authentification(auth03);
				myNFC.update_binary(temps, write03);
				myNFC.read_binary(read_binary03, temps);
				myNFC.authentification(auth04);
				myNFC.update_binary("False", write04);

				int plantxaID = plantxa.getPlantxaID(cardID);
				plantxa.setPlantxaUsed(plantxaID, "False");
				plantxa.deleteCookingKeys(cardID);
			}
			else {
				std::cout << "\n\nErreur: Aucun cas de compatibilite trouve!" << endl;
				wiringPi.displayMessage("Erreur: Aucun cas similaire!", 3);
			}

			std::cout << "\nVeuillez retirer la carte" << endl;
			IsCardOnReader(myNFC);
		}
		else {
			std::cout << "Carte non posee!" << endl;
			wiringPi.displayMessage("Veuillez poser votre carte", 1);
			sleep(1);
		}
	}

	BUFFER;
	std::cout << "Press any key to continue..";
	BUFFER;
}

//Définition de la fonction création de profil sur la carte NFC
void InitialisationCarteDebug(NFC &myNFC) {

	std::string tmpVar1, tmpVar3, tmpVar4;

	std::cout << endl << endl << "DEBUG NFC CARD" << endl << endl << endl;

	//Nom de l'utilisateur
	myNFC.authentification(auth01);
	myNFC.update_binary("DebugUser", write01);

	//Temps de cuisson restant
	myNFC.authentification(auth03);
	myNFC.update_binary("64", write03);

	//Utilisateur déjà badgé
	myNFC.authentification(auth04);
	myNFC.update_binary("False", write04);

	std::cout << endl << endl << endl;
}

//Définition de la fonction détection carte
void IsCardOnReader(NFC &myNFC) {

	bool isCardReady{ true };
	WiringPi wiringPi(WiringPi::BUTTON_BLACK, WiringPi::BUTTON_RED);

	do {

		myNFC.connection(isCardReady);
		std::cout << "Carte toujours sur le lecteur" << endl;
		wiringPi.displayMessage("Veuillez retirer votre carte", 1);
		sleep(1);

	} while (isCardReady == true);
}
