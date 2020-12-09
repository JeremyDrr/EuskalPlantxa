#include "Divers.h"

/**
*Constructeur de la classe NFC.
*/
NFC::NFC() {

	lRet = 0;
	hContext = 0;
	hCard = 0;
	dwAP = 0;
	pbSend[MAX_APDU_SIZE];
	pbRecv[MAX_APDU_SIZE];
	cbSend = 0;
	cbRecv = 0;

}

/**
*Etablissement du contexte.
*/
LONG NFC::etablissement() {
	if ((lRet = SCardEstablishContext(SCARD_SCOPE_USER, 0, 0, &hContext)) != SCARD_S_SUCCESS)
		return lRet;
}

/**
*Lecture du nom du lecteur raccorde en USB.
*/
LONG NFC::lecture() {
	lRet = SCardListReaders(hContext, NULL, NULL, &dwReaders);
	mszReaders = (LPSTR)malloc(sizeof(char)*dwReaders);
	if ((lRet = SCardListReaders(hContext, NULL, mszReaders, &dwReaders)) != SCARD_S_SUCCESS)
		return lRet;
}

/**
*Connexion au lecteur RFID ACR122U.
*\param[in} isCardReady Booleen mis a jour si la carte n'est pas posee sur le lecteur
*/
LONG NFC::connection(bool &isCardReady) {
	if ((lRet = SCardConnect(hContext, mszReaders, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T1, &hCard, &dwAP)) != SCARD_S_SUCCESS)
	{
		isCardReady = false;
		//std::cout << "Error connecting to the card " << std::hex << lRet << "\n";
		return lRet;
	}
}

/**
*Recuperer les donnees.
*/
LONG NFC::datas() {
	memcpy(pbSend, "\xFF\xCA\x00\x00\x00", 5);
	cbSend = 5;
	cbRecv = MAX_APDU_SIZE;
	SCARD_IO_REQUEST ioRequest;
	ioRequest.dwProtocol = dwAP;
	ioRequest.cbPciLength = sizeof(ioRequest);
	if ((lRet = SCardTransmit(hCard, &ioRequest, pbSend, cbSend, NULL, pbRecv, &cbRecv)) != SCARD_S_SUCCESS)
		return lRet;
	printf("Card UID: ");
	for (DWORD i = 0; i < cbRecv - 2; i++) {
		printf("%02X", pbRecv[i]);
		if (i == cbRecv - 3)
			printf("\n");
	}
}

//Load default Mifare FF Key, P1=0x20: non volatile reader memory, P2-key number
/**
*Charger la cle par defaut de Mifare.
*/
LONG NFC::load_default() {
	memcpy(pbSend, "\xFF\x82\x00\x00\x06\xFF\xFF\xFF\xFF\xFF\xFF", 11);
	cbSend = 11;
	cbRecv = MAX_APDU_SIZE;
	SCARD_IO_REQUEST ioRequest;
	ioRequest.dwProtocol = dwAP;
	ioRequest.cbPciLength = sizeof(ioRequest);
	if ((lRet = SCardTransmit(hCard, &ioRequest, pbSend, cbSend, NULL, pbRecv, &cbRecv)) != SCARD_S_SUCCESS)
		return lRet;
}

//Authentification pour bloc 1, key type A, key number 0 (3 derniers octets)
/**
*Permet de s'authentifier a un bloc pour y lire et affecter des valeurs.
*\param[in] auth Bloc auquel s'authentifier
*/
LONG NFC::authentification(const char* auth) {
	//memcpy(pbSend, "\xFF\x86\x00\x00\x05\x01\x00\x01\x60\x00", 10);
	memcpy(pbSend, auth, 10);
	cbSend = 10;
	cbRecv = MAX_APDU_SIZE;
	SCARD_IO_REQUEST ioRequest;
	ioRequest.dwProtocol = dwAP;
	ioRequest.cbPciLength = sizeof(ioRequest);
	if ((lRet = SCardTransmit(hCard, &ioRequest, pbSend, cbSend, NULL, pbRecv, &cbRecv)) != SCARD_S_SUCCESS)
		return lRet;
}

//Update Binary		Bloc 1, 16(0x10) mots
/**
*Ecrire dans un bloc apres s'etre authentifie sur ce-dernier.
*\param[in] x Valeur a ecrire dans le bloc sous forme de string
*\param[in] write Cle du bloc dans lequel ecrire
*/
LONG NFC::update_binary(std::string x, const char* write) {

	std::string finalString = x + '\n';

	//memcpy(pbSend, "\xFF\xD6\x00\x01\x10", 5);
	memcpy(pbSend, write, 5);
	memcpy(&pbSend[5], finalString.c_str(), finalString.size());

	cbSend = 5 + finalString.size();
	cbRecv = MAX_APDU_SIZE;
	SCARD_IO_REQUEST ioRequest;
	ioRequest.dwProtocol = dwAP;
	ioRequest.cbPciLength = sizeof(ioRequest);
	if ((lRet = SCardTransmit(hCard, &ioRequest, pbSend, cbSend, NULL, pbRecv, &cbRecv)) != SCARD_S_SUCCESS)
		return lRet;
}

//Read Binary		Bloc 1, 16(0x10) mots
/**
*Lire une valeur dans un bloc specifique.
*\param[in] read Cle du bloc auquel lire la valeur
*\param[out] str Variable dans laquelle sera affectee la valeur apres la lecture sous forme de string
*/
LONG NFC::read_binary(const char* read, std::string &str) {
	//memcpy(pbSend, "\xFF\xB0\x00\x01\x10", 5);
	memcpy(pbSend, read, 5);
	cbSend = 5;
	cbRecv = MAX_APDU_SIZE;
	SCARD_IO_REQUEST ioRequest;
	ioRequest.dwProtocol = dwAP;
	ioRequest.cbPciLength = sizeof(ioRequest);

	if ((lRet = SCardTransmit(hCard, &ioRequest, pbSend, cbSend, NULL, pbRecv, &cbRecv)) != SCARD_S_SUCCESS)
		return lRet;

	printf("Block 01: ");

	for (DWORD i = 0; i < cbRecv - 2; i++) {
		std::cout << pbRecv[i];
		str += pbRecv[i];

		if (pbRecv[i] == '\n')
			break;
	}

}

/**
*Permet de se deconnecter.
*/
LONG NFC::disconnect() {
	lRet = SCardDisconnect(hCard, SCARD_LEAVE_CARD);
	lRet = SCardReleaseContext(hContext);
	return lRet;
}
