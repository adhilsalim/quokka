#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const String gemini_url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-pro:generateContent";

String api_key = ""; // Don't store API key directly in code

WiFiClient client;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

bool connectWithApiKey(const String& key) {
  api_key = key;
  return true;
}

String sendPrompt(const String& prompt) {
  if (api_key.length() == 0) {
    Serial.println("Error: Missing API key");
    return "";
  }

  DynamicJsonDocument doc(4096);
  JsonArray contentArray = doc.createNestedArray("contents");
  JsonObject partsObject = contentArray.createNestedObject();
  JsonArray partsArray = partsObject.createNestedArray("parts");
  JsonObject textObject = partsArray.createNestedObject();
  textObject["text"] = prompt;

  String jsonBody;
  serializeJson(doc, jsonBody);

  HTTPClient http;
  String requestUrl = gemini_url + "?key=" + api_key;
  http.begin(client, requestUrl);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(jsonBody);

  String response = "";
  if (httpResponseCode > 0) {
    response = http.getString();
    http.end();
    Serial.println("Response:");
    Serial.println(response);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  return response;
}

void loop() {
  String prompt = "Write a story about a magic backpack";
  String response = sendPrompt(prompt);

  delay(5000);
}
