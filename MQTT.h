#pragma once
class MQTT {
	MQTTAsync client;
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	MQTTAsync_responseOptions resp_opts = MQTTAsync_responseOptions_initializer;
	MQTTAsync_disconnectOptions disc_opts = MQTTAsync_disconnectOptions_initializer;
	static std::string *transfertMessageRecu1;
	static std::string *transfertMessageRecu2;

public:
	MQTT(const char* serverURI, const char* clientId, std::string *transfMessRecu1, std::string *transMessRecu2);
	virtual ~MQTT();

	bool get_connected();
	bool souscrire(const char* leTopic, const int leQos);
	bool envoieMessage(std::string message, const int leQos, const char* leTopic);
	bool deconnexion();

	static void onConnlost(void *context, char *cause);
	static void onConnectFailure(void* context, MQTTAsync_failureData* response);
	static void onSubscribe(void* context, MQTTAsync_successData* response);
	static void onSubscribeFailure(void* context, MQTTAsync_failureData* response);
	static void onSend(void* context, MQTTAsync_successData* response);
	static void onConnect(void* context, MQTTAsync_successData* response);
	static int onMsgarrvd(void *context, char *topicName, int topicLen, MQTTAsync_message *message);
	static void onDisconnect(void* context, MQTTAsync_successData* response);

};
