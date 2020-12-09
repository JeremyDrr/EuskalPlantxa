#pragma once
class Plantxa {
public:
	Plantxa();

	std::string getTimeLeft(std::string name);

	std::string getPlantxaState(int nPlantxa);
	void setPlantxaUsed(int plantxaNb, std::string plantxaState);

	int getCardID(std::string name);
	int getPlantxaID(int cardID);

	void setCookingKeys(int plantxaID, int cardID);
	void deleteCookingKeys(int cardID);
};
