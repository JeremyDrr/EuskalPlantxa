#include "Divers.h"

using namespace rapidjson;

/**
*Constructeur de la classe RapidJSON.
*/
RapidJSON::RapidJSON(const std::string reponseJSON) {

	// Parse le string JSON dans le DOM.	
	doc.Parse(reponseJSON.c_str());
}

/**
*Destructeur de la classe RapidJSON.
*/
RapidJSON::~RapidJSON()
{
}

/**
*Decoder le JSON recue apres la requete GET pour extraire le temps restant.
*\return Renvoit le temps de cuisson restant sous forme de string
*/
std::string RapidJSON::decodeTime() {

	std::string timeLeft;

	// Lit le tableau depuis le DOM.
	for (SizeType i = 0; i < doc.Size(); i++) { // Uses SizeType instead of size_t

		Value& temps = doc[i]["T_Cuisson_Restant"];

		std::cout << "Il y a " << temps.GetString() << " minutes restantes." << endl;
		timeLeft = temps.GetString();
	}
	return timeLeft;
}

/**
*Decoder le JSON recue apres la requete GET pour extraire l'etat de la plantxa.
*\return Renvoit l'etat de la plantxa sous forme de string
*/
std::string RapidJSON::decodePlantxa() {

	std::string plantxa;

	// Lit le tableau depuis le DOM.
	for (SizeType i = 0; i < doc.Size(); i++) { // Uses SizeType instead of size_t

		Value& util = doc[i]["plantxa_util"];

		std::cout << "La plantxa est: " << util.GetString() << endl;
		plantxa = util.GetString();
	}
	return plantxa;
}

/**
*Decoder le JSON recue apres la requete GET pour extraire le numero de carte.
*\return Renvoit le numero de carte
*/
int RapidJSON::decodeCardID() {

	int cardID;

	// Lit le tableau depuis le DOM.
	for (SizeType i = 0; i < doc.Size(); i++) { // Uses SizeType instead of size_t

		Value& util = doc[i]["n_carte"];

		std::cout << "L'ID de la carte est: " << util.GetString() << endl;
		std::string sCardID = util.GetString();
		cardID = stoi(sCardID);
	}
	return cardID;
}

/**
*Decoder le JSON recue apres la requete GET pour extraire le numero de plantxa.
*\return Renvoit le numero de plantxa
*/
int RapidJSON::decodePlantxaID() {

	int plantxaID;

	// Lit le tableau depuis le DOM.
	for (SizeType i = 0; i < doc.Size(); i++) { // Uses SizeType instead of size_t

		Value& util = doc[i]["plantxa"];

		std::cout << "L'ID de la plantxa est: " << util.GetString() << endl;
		std::string sPlantxaID = util.GetString();
		plantxaID = stoi(sPlantxaID);
	}
	return plantxaID;
}
