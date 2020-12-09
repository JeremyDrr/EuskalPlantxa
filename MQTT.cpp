#include "Divers.h"

std::string *MQTT::transfertMessageRecu1{ nullptr };
std::string *MQTT::transfertMessageRecu2{ nullptr };

/**
*Constructeur de la classe MQTT.
*/
MQTT::MQTT(const char* serverURI, const char* clientId, std::string *transfMessRecu1, std::string *transfMessRecu2) {

	MQTT::transfertMessageRecu1 = transfMessRecu1; //Pointeur pour onMsgArrvd
	MQTT::transfertMessageRecu2 = transfMessRecu2; //Pointeur pour onMsgArrvd

	// Connection MQTT Async, qui va lancer le subcribe
	MQTTAsync_create(&client, serverURI, clientId, MQTTCLIENT_PERSISTENCE_NONE, NULL);

	MQTTAsync_setCallbacks(client, client, &MQTT::onConnlost, &MQTT::onMsgarrvd, NULL);

	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	conn_opts.onSuccess = &MQTT::onConnect;		// va lancer subscribe
	conn_opts.onFailure = &MQTT::onConnectFailure;
	conn_opts.context = client;
	//conn_opts.username = "";
	//conn_opts.password = "";
	int rc;
	if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS) {
		printf("Failed to start connect, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}
}

MQTT::~MQTT() {

	MQTTAsync_destroy(&client);
}

bool MQTT::get_connected() {

	return MQTTAsync_isConnected(client);
}

/**
*Souscrire à un topic afin \b d'envoyer et \b recevoir des message sur ce-dernier.
*\param[in] leTopic Topic sur lequel les message seront envoyes
*\param[in] leQos Qualite de service avec lequel les messages seront envoyes
*/
bool MQTT::souscrire(const char* leTopic, const int leQos) {

	std::cout << "Subscribing to topic " << leTopic << " for client " << client << " using QoS " + to_string(leQos) << endl;
	//printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n", leTopic, client, leQos);
	resp_opts.onSuccess = &MQTT::onSubscribe;
	resp_opts.onFailure = &MQTT::onSubscribeFailure;
	resp_opts.context = client;

	int rc;
	if ((rc = MQTTAsync_subscribe(client, leTopic, leQos, &resp_opts)) != MQTTASYNC_SUCCESS) {
		printf("Failed to start subscribe, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}
}

/**
*\b Envoyer un message sur un topic specifique.
*\param[in] message Message qui sera envoye
*\param[in] leQos Qualite de service avec lequel le message sera envoye
*\param[in] leTopic Topic sur lequel le message sera envoye
*/
bool MQTT::envoieMessage(std::string message, const int leQos, const char* leTopic) {

	resp_opts.onSuccess = onSend;
	resp_opts.context = client;
	MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
	pubmsg.payload = (void*)(message.c_str());
	pubmsg.payloadlen = (int)message.size();
	pubmsg.qos = leQos;
	pubmsg.retained = 0;

	int rc;
	if ((rc = MQTTAsync_sendMessage(client, leTopic, &pubmsg, &resp_opts)) != MQTTASYNC_SUCCESS) {

		printf("Failed to start sendMessage, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}
}

void MQTT::onConnectFailure(void* context, MQTTAsync_failureData* response) {

	printf("Connect failed, rc %d\n", response ? response->code : 0);
	MQTT::onConnlost(context, "Erreur connection");
}

void MQTT::onSubscribe(void* context, MQTTAsync_successData* response) {

	printf("Subscribe succeeded\n");
}

void MQTT::onSubscribeFailure(void* context, MQTTAsync_failureData* response) {

	printf("Subscribe failed, rc %d\n", response ? response->code : 0);
}

void MQTT::onSend(void* context, MQTTAsync_successData* response) {

	printf("Delivery confirmed, Token: %d,   ", response->token);
	printf("Destination: %s,  Message: ", response->alt.pub.destinationName);
	char *payloadptr = (char*)(char*)response->alt.pub.message.payload;
	for (int i = 0; i < response->alt.pub.message.payloadlen; i++) {
		putchar(*payloadptr++);
	}
	putchar('\n');
}

void MQTT::onConnect(void* context, MQTTAsync_successData* response) {

	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_responseOptions resp_opts = MQTTAsync_responseOptions_initializer;

	printf("Successful connection\n");
}

void MQTT::onConnlost(void *context, char *cause) {

	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;

	printf("\nConnection lost\n");
	printf("     cause: %s\n", cause);
	printf("Reconnecting\n");

	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	conn_opts.onSuccess = &MQTT::onConnect;		// va lancer subscribe
	conn_opts.onFailure = &MQTT::onConnectFailure;
	conn_opts.context = client;

	int rc;
	if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start connect, return code %d\n", rc);
	}
}

int MQTT::onMsgarrvd(void *context, char *topicName, int topicLen, MQTTAsync_message *message) {

	int i;
	char* payloadptr = (char*)message->payload;

	printf("Message arrived  topic: %s   ", topicName);

	if (strcmp(topicName, "EuskalPlantxa/MQTT/LED") == 0)
	{
		MQTT::transfertMessageRecu1->erase();
		for (i = 0; i < message->payloadlen; i++)
		{
			*MQTT::transfertMessageRecu1 += *payloadptr;
			payloadptr++;
		}
	}

	if (strcmp(topicName, "EuskalPlantxa/MQTT/Datas") == 0)
	{
		MQTT::transfertMessageRecu2->erase();
		for (i = 0; i < message->payloadlen; i++)
		{
			*MQTT::transfertMessageRecu2 += *payloadptr;
			payloadptr++;
		}
	}

	MQTTAsync_freeMessage(&message);
	MQTTAsync_free(topicName);
	return 1;
}

void MQTT::onDisconnect(void* context, MQTTAsync_successData* response) {

	printf("Successful disconnection\n");
}

bool MQTT::deconnexion() {

	disc_opts.onSuccess = onDisconnect;

	int rc;
	if ((rc = MQTTAsync_disconnect(client, &disc_opts)) != MQTTASYNC_SUCCESS) {

		printf("Failed to start disconnect, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}
	while (get_connected())

		usleep(10000L);
	return true;
}
