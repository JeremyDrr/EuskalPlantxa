#pragma once
class WiringPi {
private:

	int choixPlantxa = 1;
	int maxPlantxaNb = 5;
	bool finished{ false };

	fstream flow;

public:

	static const int BUTTON_BLACK = 28;
	static const int BUTTON_RED = 25;


	WiringPi(const int btn1, const int btn2);
	void clearLCD();
	void detectButtons(int nCarte);
	void displayMessage(const std::string message, unsigned int timeOnScreen);

};
