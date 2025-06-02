#include <WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// 📡 Configuração da Rede Wi-Fi
const char* SSID = "Wokwi-GUEST";
const char* PASSWORD = "";

// 🔗 Configuração do Broker MQTT (HiveMQ)
const char* BROKER_MQTT = "broker.hivemq.com";
const int BROKER_PORT = 1883;
const char* ID_MQTT = "esp32_mqtt"; // Identificador único do ESP32 no broker MQTT

// 🔥 Tópicos MQTT
const char* TOPIC_SUBSCRIBE_LED       = "meuProjeto/led";
const char* TOPIC_PUBLISH_TEMPERATURE = "meuProjeto/temperatura";
const char* TOPIC_PUBLISH_HUMIDITY    = "meuProjeto/umidade";
const char* TOPIC_PUBLISH_TEMP_FRIA   = "meuProjeto/tempFria";

// 🕒 Tempo entre publicações
#define PUBLISH_DELAY 5000

// 🎯 Configuração dos sensores
#define PIN_DHT 15
DHTesp dht;

#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// 💡 Configuração do LED
#define PIN_LED 2

// 📡 Objetos para conexão
WiFiClient espClient;
PubSubClient MQTT(espClient);

// 🔥 Variáveis globais
unsigned long publishUpdate;
static char strTemperature[10] = {0};
static char strHumidity[10] = {0};
static char strTempFria[10] = {0};

// 📌 Conectar ao Wi-Fi
void initWiFi() {
    Serial.println("------Conectando ao Wi-Fi------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    WiFi.begin(SSID, PASSWORD);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\n✅ Wi-Fi conectado! IP: " + WiFi.localIP().toString());
}

// 📌 Conectar ao MQTT
void initMQTT() {
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);
    MQTT.setCallback(callbackMQTT);
}

// 📌 Callback para receber mensagens MQTT
void callbackMQTT(char *topic, byte *payload, unsigned int length) {
    String msg = "";
    for (int i = 0; i < length; i++) {
        msg += (char)payload[i];
    }
    
    Serial.printf("📩 Mensagem recebida no tópico %s: %s\n", topic, msg.c_str());

    // 💡 Comando para ligar/desligar o LED
    if (msg.equals("1")) {
        digitalWrite(PIN_LED, HIGH);
        Serial.println("🔆 LED ligado via MQTT");
    } else if (msg.equals("0")) {
        digitalWrite(PIN_LED, LOW);
        Serial.println("🌑 LED desligado via MQTT");
    }
}

// 📌 Reconectar ao broker MQTT
void reconnectMQTT() {
    while (!MQTT.connected()) {
        Serial.print("🔄 Tentando conectar ao MQTT...");
        if (MQTT.connect(ID_MQTT)) {
            Serial.println("✅ Conectado ao broker MQTT!");
            MQTT.subscribe(TOPIC_SUBSCRIBE_LED);
        } else {
            Serial.println("❌ Falha na conexão. Tentando novamente...");
            delay(2000);
        }
    }
}

// 📌 Verificação das conexões WiFi e MQTT
void checkWiFIAndMQTT() {
    if (!MQTT.connected()) {
        reconnectMQTT();
    }
}

// 📌 Setup: Inicializa sensores, Wi-Fi e MQTT
void setup() {
    Serial.begin(115200);
    delay(3000); // ✅ Adicionando 3 segundos de delay antes de imprimir mensagens no Serial Monitor
    Serial.println("🚀 ESP32 iniciado!");

    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);

    dht.setup(PIN_DHT, DHTesp::DHT22);
    sensors.begin();

    initWiFi();
    initMQTT();
}

// 📌 Loop principal
void loop() {
    checkWiFIAndMQTT();
    MQTT.loop();

    unsigned long now = millis();
    if (now - publishUpdate >= PUBLISH_DELAY) {
        publishUpdate = now;

        // 🔍 Leitura dos sensores
        float temperature = dht.getTemperature();
        float humidity = dht.getHumidity();
        sensors.requestTemperatures();
        float tempFria = sensors.getTempCByIndex(0);

        if (!isnan(temperature) && !isnan(humidity) && tempFria != DEVICE_DISCONNECTED_C) {
            sprintf(strTemperature, "%.2f", temperature);
            sprintf(strHumidity, "%.2f", humidity);
            sprintf(strTempFria, "%.2f", tempFria);

            // 📡 Publicação dos dados via MQTT
            MQTT.publish(TOPIC_PUBLISH_TEMPERATURE, strTemperature);
            MQTT.publish(TOPIC_PUBLISH_HUMIDITY, strHumidity);
            MQTT.publish(TOPIC_PUBLISH_TEMP_FRIA, strTempFria);

            Serial.printf("📤 Dados enviados - Temp: %s°C, Umid: %s%%, Temp Fria: %s°C\n", strTemperature, strHumidity, strTempFria);
        } else {
            Serial.println("⚠ Erro na leitura dos sensores!");
        }
    }
}