const express = require('express');
const mongoose = require('mongoose');
const bodyParser = require('body-parser');
const mqtt = require('mqtt');

const app = express();
app.use(bodyParser.json());

// Conectar ao MongoDB
mongoose.connect('mongodb://localhost:27017/IoTData', { useNewUrlParser: true, useUnifiedTopology: true });

// Definir modelo de dados
const DataSchema = new mongoose.Schema({
    temperature: Number,
    humidity: Number,
    speed: Number,
    timestamp: { type: Date, default: Date.now }
});

const Data = mongoose.model('Data', DataSchema);

// Configurar o cliente MQTT
const client = mqtt.connect('mqtt://IP_Servidor_IoT');

client.on('connect', () => {
    console.log('Conectado ao Broker MQTT');
    client.subscribe('/TEF/device022/attrs/temp');
});

// Receber dados do MQTT
client.on('message', (topic, message) => {
    const data = JSON.parse(message.toString());
    const newData = new Data(data);
    newData.save();
});

// Iniciar o servidor
app.listen(3000, () => {
    console.log('Servidor rodando na porta 3000');
});
