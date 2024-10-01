Descrição do Projeto
Este projeto consiste em um sistema de monitoramento baseado em IoT (Internet das Coisas) que utiliza um ESP32 e sensores de temperatura e velocidade para coletar e transmitir dados. Os dados são enviados para um broker MQTT e podem ser visualizados em um dashboard, permitindo um acompanhamento em tempo real.

Video Explicativo:
https://youtu.be/MsjYNk-5NhY

Integrantes da Equipe
Vinicius Henrique Oliveira (RM 556908)
Gabriel Miranda (RM 559102)
Gustavo Pierre Santos (RM 558928)
Eric Megherdijan Darakjian (RM 557082)
Kheyla Thais (RM 558434)


Arquitetura Proposta
A arquitetura da solução é composta por três principais componentes:

Dispositivos IoT: Inclui ESP32 e sensores (DHT22 para temperatura e sensores de velocidade).
Back-end: Um broker MQTT para a troca de mensagens entre dispositivos e uma aplicação que processa e armazena os dados.
Front-end: Um dashboard para visualização dos dados em tempo real.
Diagrama da Arquitetura
+-------------------+         +-------------------+         +-------------------+
|   Dispositivos    |         |       Back-end    |         |      Front-end     |
|      IoT          | ----->  |     (Broker MQTT) | ----->  |    (Dashboard)     |
| (ESP32 + Sensores)|         |  (Processamento)  |         |   (Visualização)   |
+-------------------+         +-------------------+         +-------------------+



Recursos Necessários
Dispositivos IoT
ESP32: Microcontrolador com suporte Wi-Fi e Bluetooth.
DHT22: Sensor de temperatura e umidade.
Sensor de Velocidade: Sensor para medir a velocidade de rotação.

Back-end
Broker MQTT: Para a comunicação entre dispositivos.
Servidor: Um PC ou servidor que hospeda o broker MQTT e uma aplicação que pode processar dados (por exemplo, Node.js).

Front-end
Framework: React, Angular ou qualquer outro framework de visualização para o dashboard.

Bibliotecas: Bibliotecas para gráficos, como Chart.js ou D3.js.

Instruções de Uso

Instale as bibliotecas necessárias (DHT, PubSubClient) na IDE do Arduino.
Configure o seu broker MQTT (por exemplo, Mosquitto) no PC ou servidor.
Substitua as variáveis de configuração no código do ESP32 (SSID, senha, IP do broker, etc.).
Carregue o código no ESP32.
Execute o broker MQTT no PC/servidor.
Execute o front-end (dashboard) para visualizar os dados.

Requisitos
Hardware

1 x ESP32
1 x DHT22
1 x Sensor de Velocidade
Conexão à Internet
Software

IDE do Arduino
Broker MQTT (ex: Mosquitto)
Ferramentas para desenvolvimento front-end (Node.js, React, etc.)
Dependências
WiFi.h
PubSubClient
DHT sensor library




