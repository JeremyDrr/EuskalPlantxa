#include "Divers.h"

/**
*Constructeur de la classe Plantxa.
*/
Plantxa::Plantxa() {
}

/**
*Recuperer le temps de cuisson restant de la base de donnees.
*\param[in] name Nom du client de la carte a comparer a celui de la base de donnees
*\return Renvoit le temps de cuisson restant sous forme de string
*/
std::string Plantxa::getTimeLeft(std::string name) {

	cURL cURLObj;
	std::string url{ "http://10.0.0.18/PHP/php_rest/temps_restant.php" };
	std::string reponseDatabase{ "" };
	int reponseHTTP{};
	url += "?nom=" + name;

	reponseHTTP = cURLObj.rGET(url, reponseDatabase);

	if (reponseHTTP == 200) {
		std::cout << "\nDecodage du temps restant en cours..\n";
		RapidJSON decJson(reponseDatabase);
		std::string temps = decJson.decodeTime();

		return temps;
	}
	else {
		std::cout << "\nEchec du GET depuis " << url << " avec le code HTTP " << reponseHTTP << endl;
	}

}

/**
*Recuperer l'etat de la plantxa, pour savoir si elle est actuellement utilisee ou non.
*\param[in] nPlantxa Le numero de la plantxa a laquelle verifier l'etat
*\return Renvoit l'etat de la plantxa sous forme de string
*/
std::string Plantxa::getPlantxaState(int nPlantxa) {

	cURL cURLObj;
	std::string url{ "http://10.0.0.18/PHP/php_rest/rest_plantxautil_get.php" };
	std::string reponseDatabase{ "" };
	int reponseHTTP{};
	url += "?n_plantxa=" + to_string(nPlantxa);

	reponseHTTP = cURLObj.rGET(url, reponseDatabase);

	if (reponseHTTP == 200) {
		std::cout << "\nDecodage etat de la plantxa en cours..\n";
		RapidJSON decJson(reponseDatabase);
		std::string plantxaState = decJson.decodePlantxa();

		return plantxaState;
	}
	else {
		std::cout << "\nEchec du GET depuis " << url << " avec le code HTTP " << reponseHTTP << endl;
	}
}

/**
*Mettre a jour l'etat de la plantxa.
*\param[in] plantxaNb Le numero de la plantxa a laquelle mettre a jour l'etat
*\param[in] plantxaState Etat de la plantxa (\b True ou \b False )
*/
void Plantxa::setPlantxaUsed(int plantxaNb, std::string plantxaState) {

	cURL cURLObj;

	std::string url{ "http://10.0.0.18/PHP/php_rest/rest_plantxautil_post.php" };
	cURLObj.rPOSTPlantxa(url, plantxaNb, plantxaState);
}

/**
*Recuperer le numero de la carte.
*\param[in] name Le nom du client ecrit sur la carte NFC
*\return Renvoit le numero de la carte
*/
int Plantxa::getCardID(std::string name) {

	cURL cURLObj;
	std::string url{ "http://10.0.0.18/PHP/php_rest/rest_carteid_get.php" };
	std::string reponseDatabase{ "" };
	int reponseHTTP{};
	url += "?nom=" + name;

	reponseHTTP = cURLObj.rGET(url, reponseDatabase);

	if (reponseHTTP == 200) {
		std::cout << "\nDecodage du numero de carte en cours..\n";
		RapidJSON decJson(reponseDatabase);
		int cardID = decJson.decodeCardID();

		return cardID;
	}
	else {
		std::cout << "\nEchec du GET depuis " << url << " avec le code HTTP " << reponseHTTP << endl;
	}
}

/**
*Recuperer le numero de la plantxa selectionnee par le client.
*\param[in] cardID Le numero de la carte du client
*\return Renvoit le numero de la plantxa choisie par le client
*/
int Plantxa::getPlantxaID(int cardID) {

	cURL cURLObj;
	std::string url{ "http://10.0.0.18/PHP/php_rest/rest_cuisson_get.php" };
	std::string reponseDatabase{ "" };
	int reponseHTTP{};
	url += "?carte=" + to_string(cardID);

	reponseHTTP = cURLObj.rGET(url, reponseDatabase);

	if (reponseHTTP == 200) {
		std::cout << "\nDecodage du numero de plantxa en cours..\n";
		RapidJSON decJson(reponseDatabase);
		int plantxaID = decJson.decodePlantxaID();

		return plantxaID;
	}
	else {
		std::cout << "\nEchec du GET depuis " << url << " avec le code HTTP " << reponseHTTP << endl;
	}
}

/**
*Mettre a jour la liaison entre le numero de plantxa et le numero de carte.
*\param[in] plantxaID Le numero de la plantxa choisie par le client
*\param[in] cardID Le numero de la carte du client
*/
void Plantxa::setCookingKeys(int plantxaID, int cardID) {

	cURL cURLObj;
	std::string url{ "http://10.0.0.18/PHP/php_rest/rest_cuisson_post.php" };
	cURLObj.rPOSTCuisson(url, plantxaID, cardID);
}

/**
*Supprimer la liaison entre le numero de plantxa et le numero de carte.
*\param[in] cardID Le numero de la carte du client
*/
void Plantxa::deleteCookingKeys(int cardID) {

	cURL cURLObj;
	std::string url{ "http://10.0.0.18/PHP/php_rest/rest_cuisson_delete.php" };
	cURLObj.rDELCuisson(url, cardID);
}
