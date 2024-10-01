#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "SEU_SSID";
const char* password = "SEU_SENHA_WIFI";

const char* server = "api.whatsapp.com";

const char* phoneNumbers[] = {"NUMERO_DE_TELEFONE_1", "NUMERO_DE_TELEFONE_2", "NUMERO_DE_TELEFONE_3"};
const int numPhones = sizeof(phoneNumbers) / sizeof(phoneNumbers[0]);

int sensorPin = A0;
float humidity;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  humidity = analogRead(sensorPin) / 1023.0 * 100.0;
  Serial.println(humidity);

  if (humidity < 40) {
    for (int i = 0; i < numPhones; i++) {
      sendWhatsAppMessage(phoneNumbers[i], humidity);
    }
  }

  delay(1000);
}

void sendWhatsAppMessage(const char* phoneNumber, float humidity) {
  HTTPClient http;
  http.begin(server, 443, "/v2/messages/send");
  http.addHeader("Content-Type", "application/json");

  String data = "{\"phone_number\":\"" + phoneNumber + "\",\"message\":\"O nível de umidade do ar está em aproximadamente " + String(humidity) + "%, por favor se hidrate e cuide dos seus familiares.\"}";

  int statusCode = http.POST(data);
  if (statusCode == 200) {
    Serial.println("Mensagem enviada com sucesso!");
  } else {
    Serial.println("Falha ao enviar mensagem.");
  }

  http.end();
}