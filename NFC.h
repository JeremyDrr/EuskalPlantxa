#pragma once

#define MAX_APDU_SIZE 255

class NFC {
private:
	LONG         lRet = 0;
	SCARDCONTEXT hContext = 0;
	SCARDHANDLE  hCard = 0;
	DWORD        dwAP = 0;
	BYTE         pbSend[MAX_APDU_SIZE];
	BYTE         pbRecv[MAX_APDU_SIZE];
	DWORD        cbSend = 0;
	DWORD        cbRecv = 0;
	LPSTR mszReaders;
	DWORD dwReaders;
public:
	NFC();
	LONG etablissement();
	LONG lecture();
	LONG connection(bool &isConnected);
	LONG datas();
	LONG load_default();
	LONG authentification(const char* auth);
	LONG update_binary(std::string x, const char* write);
	LONG read_binary(const char* read, std::string &str);
	LONG disconnect();
};
