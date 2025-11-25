#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

const char* ssid = "LAPTOP-FHJBJ2FC 2173";
const char* password = "0r725[6N";

const char* apiKey = "LGXH42GGXJ9SJW5I";
const char* server = "http://api.thingspeak.com/update";
const int channelID = 3155905;

#define DHTPIN 32
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("TP4 ThingSpeak - Patrice Allagbe");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connexion WiFi en cours...");
  }

  Serial.println("Connecté au WiFi");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());

  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Erreur de lecture du capteur DHT");
    delay(2000);
    return;
  }

  Serial.println("Mesures relevées");
  Serial.printf("Température : %.2f °C\n", t);
  Serial.printf("Humidité : %.2f %%\n", h);

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(server) + "?api_key=" + apiKey +
                 "&field1=" + String(t) +
                 "&field2=" + String(h);

    http.begin(url);
    int code = http.GET();

    if (code > 0) {
      Serial.printf("Données envoyées. Code HTTP : %d\n", code);
    } else {
      Serial.printf("Erreur lors de l'envoi. Code : %d\n", code);
    }
    http.end();
  } else {
    Serial.println("WiFi déconnecté");
  }

  Serial.println("=============================");
  delay(20000);
}
