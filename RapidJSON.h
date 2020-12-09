#pragma once

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

class RapidJSON {
private:
	rapidjson::Document doc;

public:
	RapidJSON(const std::string reponseJSON);
	~RapidJSON();
	std::string decodeTime();
	std::string decodePlantxa();
	int decodeCardID();
	int decodePlantxaID();
};
