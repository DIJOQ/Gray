#include <WiFi.h>
#include <HTTPClient.h>
#include <TinyGPS++.h>

const char* ssid = "YOUR_SSID"; // Remplace par ton SSID
const char* password = "YOUR_PASSWORD"; // Remplace par ton mot de passe Wi-Fi
const char* serverUrl = "https://your-server-url.com"; // URL de ton serveur

TinyGPSPlus gps; // Instance de TinyGPS++
HardwareSerial gpsSerial(1); // Utilisation d'un port série pour le GPS

void setup() {
    Serial.begin(115200); // Initialiser la communication série
    gpsSerial.begin(9600, SERIAL_8N1, 16, 17); // Initialiser le port série pour le GPS
    WiFi.begin(ssid, password); // Connexion au Wi-Fi

    while (WiFi.status() != WL_CONNECTED) { // Attendre la connexion
        delay(500); // Attendre 500 ms
        Serial.print("."); // Afficher un point pour indiquer la tentative de connexion
    }
    Serial.println("Connected to WiFi"); // Indiquer que la connexion est établie
}

void loop() {
    while (gpsSerial.available() > 0) { // Vérifier si des données GPS sont disponibles
        gps.encode(gpsSerial.read()); // Lire et encoder les données GPS
        if (gps.location.isUpdated()) { // Vérifier si la localisation a été mise à jour
            double latitude = gps.location.lat(); // Obtenir la latitude
            double longitude = gps.location.lng(); // Obtenir la longitude
            Serial.print("Latitude: ");
            Serial.print(latitude, 6);
            Serial.print(" Longitude: ");
            Serial.println(longitude, 6);
            sendDataToServer(latitude, longitude); // Envoyer les données au serveur
        }
    }
}

void sendDataToServer(double lat, double lng) {
    if (WiFi.status() == WL_CONNECTED) { // Vérifier la connexion Wi-Fi
        HTTPClient http; // Créer une instance de HTTPClient
        String url = String(serverUrl) + "?lat=" + String(lat) + "&lng=" + String(lng); // Créer l'URL avec les données GPS
        http.begin(url); // Initialiser la requête HTTP
        int httpResponseCode = http.GET(); // Envoyer la requête GET
        if (httpResponseCode > 0) {
            Serial.println("Data sent successfully");
        } else {
            Serial.print("Error sending data: ");
            Serial.println(httpResponseCode);
        }
        http.end(); // Terminer la requête
    } else {
        Serial.println("WiFi not connected");
    }
}

