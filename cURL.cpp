#include "Divers.h"

/**
*Constructeur de la classe cURL.
*/
cURL::cURL() {

	curl_global_init(CURL_GLOBAL_DEFAULT);	// init
	curl = curl_easy_init();				// init
}

/**
*Destructeur de la classe cURL.
*/
cURL::~cURL()
{
	curl_easy_cleanup(curl);
}

// fonction appelée automatiquement et qui reçoit la réponse
std::size_t cURL::callback(
	const char* in,
	std::size_t size,
	std::size_t num,
	std::string* out)
{
	const std::size_t totalBytes(size * num);
	out->append(in, totalBytes);	// réponse dans la chaine
	return totalBytes;
}

/**
*Fonction servant a effectuer une requete GET pour recuperer des donnees.
*\param[in] url URL du fichier PHP sur lequel executer la requete
*\param[in] buffer Variable dans laquelle sera affectee la valeur
*\return Renvoit le code HTTP de la requete
*/
int cURL::rGET(const std::string& url, std::string& buffer) {	// GET

	if (curl != nullptr) {
		// Set verbe HTTP.
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");

		// Set URI.
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

		// Pas d'attente infinie, time out de 5 secondes.
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);

		// Suit redirections HTTP si nécessaire.
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, "cache-control: no-cache");
		headers = curl_slist_append(headers, "Content-Type: application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		// Configure le nom de la fonction appelée pour la réception.
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &cURL::callback);

		// Configure le nom de la structure de données remplie à la réception.
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

		// Login et Mot de passe
		curl_easy_setopt(curl, CURLOPT_USERPWD, "rest:lmdpderest");

		// Lance la commande HTTP GET, capture la réponse et le code.
		// utilise callback, attend la fin (bloquant)
		CURLcode res = curl_easy_perform(curl);
		// Check for errors 
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
			return 0;
		}
		else {
			int httpCode;				// code retour du serveur
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
			return httpCode;
		}
	}
}

/**
*Fonction servant a effectuer une requete POST pour mettre a jour des donnees relatives a la plantxa.
*\param[in] url URL du fichier PHP sur lequel executer la requete
*\param[in] nPlantxa Numero de la plantxa a mettre a jour
*\param[in] etat Etat de la plantxa apres la mise a jour (\b True ou \b False )
*\return Renvoit le code HTTP de la requete
*/
int cURL::rPOSTPlantxa(const std::string& url, int &nPlantxa, std::string &etat) {	// POST

	if (curl != nullptr) {

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		/* Now specify the POST data */

		std::string request = "{\"n_plantxa\":" + to_string(nPlantxa) + ",\"plantxa_util\":\"" + etat + "\"}";
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.c_str());

		/* Perform the request, res will get the return code */
		CURLcode res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		return 0;
	}
	else {
		int httpCode;				// code retour du serveur
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
		return httpCode;
	}
}

/**
*Fonction servant a effectuer une requete POST pour faire le lien entre l'ID du client et le numero de plantxa.
*\param[in] url URL du fichier PHP sur lequel executer la requete
*\param[in] nPlantxa Numero de la plantxa choisie par le client
*\param[in] nCarte Numero de la carte du client
*\return Renvoit le code HTTP de la requete
*/
int cURL::rPOSTCuisson(const std::string& url, int &nPlantxa, int &nCarte) {	// POST

	if (curl != nullptr) {

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		/* Now specify the POST data */

		std::string request = "{\"n_plantxa\":" + to_string(nPlantxa) + ",\"n_carte\":" + to_string(nCarte) + "}";
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.c_str());

		/* Perform the request, res will get the return code */
		CURLcode res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		return 0;
	}
	else {
		int httpCode;				// code retour du serveur
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
		return httpCode;
	}
}

/**
*Fonction servant a effectuer une requete DELETE pour supprimer la relation entre l'ID du client et le numero de plantxa.
*\param[in] url URL du fichier PHP sur lequel executer la requete
*\param[in] nCarte Numero de la carte du client
*\return Renvoit le code HTTP de la requete
*/
int cURL::rDELCuisson(const std::string& url, int &nCarte) {

	if (curl != nullptr) {

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
		/* Now specify the POST data */

		std::string request = "{\"carte\":" + to_string(nCarte) + "}";
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.c_str());

		/* Perform the request, res will get the return code */
		CURLcode res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		return 0;
	}
	else {
		int httpCode;				// code retour du serveur
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
		return httpCode;
	}
}
