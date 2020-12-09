#pragma once

//Imporation des bibliothèques
#include <iostream>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <exception>
#include <fstream>
#include <memory>
#include <algorithm>
#include <thread>

#include "Plantxa.h"

//Using à la place de namespace std
using std::cout;
using std::cin;
using std::endl;
using std::to_string;
using std::exception;
using std::fstream;
using std::stoi;
using std::remove;
using std::thread;

#define BUFFER std::cin.ignore(32767, '\n')

/*******************************************
				NFC - IMPORTS
*******************************************/

#include "winscard.h"
#include "wintypes.h"
#include "NFC.h"

/*******************************************
				MQTT - IMPORTS
*******************************************/

#include "MQTTAsync.h"
#include "MQTT.h"
#include <unistd.h>

/*******************************************
			WIRINGPI - IMPORT
*******************************************/

#include <wiringPi.h>
#include "WiringPi.h"

/*******************************************
				CURL - IMPORT
*******************************************/

#include "cURL.h"
#include "curl/curl.h"
#include "RapidJSON.h"


#include "AppliRPI.h"
