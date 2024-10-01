#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Configurações - variáveis editáveis
const char* default_SSID = "Yzs_Pierre"; // Nome da rede Wi-Fi
const char* default_PASSWORD = "Pierrecelta22"; // Senha da rede Wi-Fi
const char* default_BROKER_MQTT = "192.168.15.105"; // IP do Broker MQTT
const int default_BROKER_PORT = 1883; // Porta do Broker MQTT
const char* default_TOPICO_SUBSCRIBE = "/TEF/device022/cmd"; // Tópico MQTT de escuta
const char* default_TOPICO_PUBLISH_1 = "/TEF/device022/attrs"; // Tópico MQTT de envio de informações para Broker (LED)
const char* default_TOPICO_PUBLISH_2 = "/TEF/device022/attrs/p"; // Tópico MQTT de envio de informações para Broker (Luminosidade)
const char* default_TOPICO_PUBLISH_TEMP = "/TEF/device022/attrs/temp"; // Tópico MQTT para temperatura
const char* default_TOPICO_PUBLISH_SPEED = "/TEF/device022/attrs/speed"; // Tópico MQTT para velocidade
const char* default_ID_MQTT = "fiware_022"; // ID MQTT
const int default_D4 = 2; // Pino do LED onboard

// Configurações dos Sensores
#define DHTPIN 4          // Pino conectado ao DHT22
#define DHTTYPE DHT22     // Tipo do sensor DHT
#define SPEED_SENSOR_PIN 5 // Pino conectado ao sensor de velocidade (interrupt)

// Declaração da variável para o prefixo do tópico
const char* topicPrefix = "device022";

// Variáveis para configurações editáveis
char* SSID = const_cast<char*>(default_SSID);
char* PASSWORD = const_cast<char*>(default_PASSWORD);
char* BROKER_MQTT = const_cast<char*>(default_BROKER_MQTT);
int BROKER_PORT = default_BROKER_PORT;
char* TOPICO_SUBSCRIBE = const_cast<char*>(default_TOPICO_SUBSCRIBE);
char* TOPICO_PUBLISH_1 = const_cast<char*>(default_TOPICO_PUBLISH_1);
char* TOPICO_PUBLISH_2 = const_cast<char*>(default_TOPICO_PUBLISH_2);
char* TOPICO_PUBLISH_TEMP = const_cast<char*>(default_TOPICO_PUBLISH_TEMP);
char* TOPICO_PUBLISH_SPEED = const_cast<char*>(default_TOPICO_PUBLISH_SPEED);
char* ID_MQTT = const_cast<char*>(default_ID_MQTT);
int D4 = default_D4;

// Instancia dos Objetos
WiFiClient espClient;
PubSubClient MQTT(espClient);
DHT dht(DHTPIN, DHTTYPE);

// Variáveis Globais
char EstadoSaida = '0';
volatile unsigned int rotationCount = 0;
unsigned long lastSpeedCheck = 0;
unsigned long speedInterval = 10000; // 10 segundos
float currentSpeed = 0.0;

// Funções de Prototipagem
void initSerial();
void initWiFi();
void initMQTT();
void reconnectMQTT();
void reconectWiFi();
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void VerificaConexoesWiFIEMQTT();
void EnviaEstadoOutputMQTT();
void InitOutput();
void handleLuminosity();
void handleTemperature();
void handleSpeed();

// Função de Interrupção para o Sensor de Velocidade
void IRAM_ATTR onSpeedPulse() {
    rotationCount++;
}

void setup() {
    InitOutput();
    initSerial();
    initWiFi();
    initMQTT();
    delay(5000);
    MQTT.publish(TOPICO_PUBLISH_1, "s|on");

    // Inicializar Sensor DHT
    dht.begin();

    // Configurar pino do sensor de velocidade como entrada com pull-up
    pinMode(SPEED_SENSOR_PIN, INPUT_PULLUP);
    // Anexar interrupção para cada pulso do sensor de velocidade
    attachInterrupt(digitalPinToInterrupt(SPEED_SENSOR_PIN), onSpeedPulse, FALLING);
}

void loop() {
    VerificaConexoesWiFIEMQTT();
    EnviaEstadoOutputMQTT();
    handleLuminosity();
    handleTemperature();
    handleSpeed();
    MQTT.loop();
}

// Inicialização da Comunicação Serial
void initSerial() {
    Serial.begin(115200);
}

// Inicialização da Conexão Wi-Fi
void initWiFi() {
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
    reconectWiFi();
}

// Inicialização do Cliente MQTT
void initMQTT() {
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);
    MQTT.setCallback(mqtt_callback);
}

// Callback para Mensagens MQTT Recebidas
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    String msg;
    for (int i = 0; i < length; i++) {
        char c = (char)payload[i];
        msg += c;
    }
    Serial.print("- Mensagem recebida: ");
    Serial.println(msg);

    // Forma o padrão de tópico para comparação
    String onTopic = String(topicPrefix) + "@on|";
    String offTopic = String(topicPrefix) + "@off|";

    // Compara com o tópico recebido
    if (msg.equals(onTopic)) {
        digitalWrite(D4, HIGH);
        EstadoSaida = '1';
    }

    if (msg.equals(offTopic)) {
        digitalWrite(D4, LOW);
        EstadoSaida = '0';
    }
}

// Verificação e Reconexão de Conexões Wi-Fi e MQTT
void VerificaConexoesWiFIEMQTT() {
    if (!MQTT.connected())
        reconnectMQTT();
    reconectWiFi();
}

// Reconexão ao Broker MQTT
void reconnectMQTT() {
    while (!MQTT.connected()) {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE);
        } else {
            Serial.print("Falha ao reconectar no broker, rc=");
            Serial.print(MQTT.state());
            Serial.println(" Tentando novamente em 2s");
            delay(2000);
        }
    }
}

// Publicação do Estado do LED via MQTT
void EnviaEstadoOutputMQTT() {
    if (EstadoSaida == '1') {
        MQTT.publish(TOPICO_PUBLISH_1, "s|on");
        Serial.println("- LED Ligado");
    }

    if (EstadoSaida == '0') {
        MQTT.publish(TOPICO_PUBLISH_1, "s|off");
        Serial.println("- LED Desligado");
    }
    Serial.println("- Estado do LED onboard enviado ao broker!");
    delay(1000);
}

// Inicialização do LED Onboard
void InitOutput() {
    pinMode(D4, OUTPUT);
    digitalWrite(D4, HIGH);
    boolean toggle = false;

    for (int i = 0; i <= 10; i++) {
        toggle = !toggle;
        digitalWrite(D4, toggle);
        delay(200);
    }
}

// Manipulação da Luminosidade
void handleLuminosity() {
    const int potPin = 34; // Pino analógico para luminosidade
    int sensorValue = analogRead(potPin);
    int luminosity = map(sensorValue, 0, 4095, 0, 100);
    String mensagem = String(luminosity);
    Serial.print("Valor da luminosidade: ");
    Serial.println(mensagem.c_str());
    MQTT.publish(TOPICO_PUBLISH_2, mensagem.c_str());
}

// Manipulação da Temperatura e Umidade
void handleTemperature() {
    float temperatura = dht.readTemperature(); // Leitura em Celsius
    float umidade = dht.readHumidity();

    // Verificar se as leituras falharam
    if (isnan(temperatura) || isnan(umidade)) {
        Serial.println("Falha na leitura do sensor DHT!");
        return;
    }

    // Formatar mensagem JSON
    String payload = "{\"temperatura\":" + String(temperatura, 2) + ",\"umidade\":" + String(umidade, 2) + "}";
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.print("°C, Umidade: ");
    Serial.print(umidade);
    Serial.println("%");

    MQTT.publish(TOPICO_PUBLISH_TEMP, payload.c_str());
}

// Manipulação da Velocidade
void handleSpeed() {
    unsigned long currentMillis = millis();
    if (currentMillis - lastSpeedCheck >= speedInterval) {
        noInterrupts();
        unsigned int count = rotationCount;
        rotationCount = 0;
        interrupts();

        // Calcular velocidade (rotações por intervalo)
        // Exemplo: Se cada rotação corresponde a uma rotação completa, velocidade pode ser rotações por minuto
        float rpm = (count / (speedInterval / 60000.0)); // Rotações por minuto
        currentSpeed = rpm; // Ajuste conforme necessidade

        // Formatar mensagem
        String speedMsg = String(currentSpeed, 2);
        Serial.print("Velocidade: ");
        Serial.print(speedMsg);
        Serial.println(" RPM");

        MQTT.publish(TOPICO_PUBLISH_SPEED, speedMsg.c_str());

        lastSpeedCheck = currentMillis;
    }
}

}
