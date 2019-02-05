#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <ArduinoJson.h>

#include "DisplayManager.h"

const char* WIFI_SSID = "Turuturay";
const char* WIFI_PASS = "stog@TicFa#";

const int LOOP_DELAY = 500;
const int SCREEN_DELAY = 2000;
const int capacity = JSON_OBJECT_SIZE(10);
const int SDA_SCREEN = D3;
const int SCL_SCREEN = D4;

const String URL = "http://sergiocasero.es/instagram.php?username=queicopostres";


SSD1306 display(0x3c, SDA_SCREEN, SCL_SCREEN);

/* The display */

DisplayManager displayManager = DisplayManager();

struct instagram {
	int followers;
	int following;
	int posts;
};

void setup() {
	display.init();

	Serial.begin(115200);
	WiFi.begin(WIFI_SSID, WIFI_PASS);

	Serial.println();

	displayManager.connectingWiFi(display);

	while(WiFi.status() != WL_CONNECTED) {
	delay(LOOP_DELAY);
		Serial.print(".");
	}

	displayManager.wifiConnected(display);
}

void loop() {
	instagram userData = getInstagramData();
	printUserData(userData);
}

instagram getInstagramData() {
	HTTPClient client;
	client.begin(URL);

	int responseCode = client.GET();

	StaticJsonBuffer<capacity> jsonBuffer;
	JsonObject& instagramData = jsonBuffer.parseObject(client.getString());

	instagram userData;

	if(instagramData.success()) {
		userData.followers = instagramData["followers"];
		userData.following = instagramData["following"];
		userData.posts = instagramData["posts"];
	} else {
		Serial.println("error");
	}

	return userData;
}

void printUserData(instagram userData) {
	Serial.println("Followers: " + String(userData.followers));
	Serial.println("Following: " + String(userData.following));
	Serial.println("Posts: " + String(userData.posts));

	displayManager.data(display, "@queicopostres", "Followers", String(userData.followers));
	delay(SCREEN_DELAY);
	displayManager.data(display, "@queicopostres", "Following", String(userData.following));
	delay(SCREEN_DELAY);
	displayManager.data(display, "@queicopostres", "Posts", String(userData.posts));
	delay(SCREEN_DELAY);
}
