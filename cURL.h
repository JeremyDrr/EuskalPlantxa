#pragma once

class cURL {
	void *curl;		// pointeur sur objet curl

public:
	cURL();
	~cURL();

	static std::size_t callback(	// fonction callback qui reçoit la réponse
		const char* in,
		std::size_t size,
		std::size_t num,
		std::string* out);

	int rGET(const std::string& url, std::string& buffer);
	int rPOSTPlantxa(const std::string& url, int &nPlantxa, std::string &etat);
	int rPOSTCuisson(const std::string& url, int &nPlantxa, int &nCarte);
	int rDELCuisson(const std::string& url, int &nCarte);
};
