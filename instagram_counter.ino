#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <ArduinoJson.h>

#include "DisplayManager.h"

const char* WIFI_SSID = "prebeta";
const char* WIFI_PASS = "prebeta23";

const String USERNAME = "jacobo.cl";
const String ENDPOINT = "http://sergiocasero.es/instagram.php?username=";

const int WIFI_DELAY = 500;
const int DISPLAY_DELAY = 2000;
const int CAPACITY = JSON_OBJECT_SIZE(10);

const int SDA_SCREEN = D3;
const int SCL_SCREEN = D4;

SSD1306 display(0x3c, SDA_SCREEN, SCL_SCREEN);

DisplayManager manager = DisplayManager();

struct instagram {
	int followers;
	int following;
	int posts;
};

void setup() {
	Serial.begin(115200);
	display.init();

	manager.connectingWiFi(display);

	WiFi.begin(WIFI_SSID, WIFI_PASS);
	while(WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		delay(WIFI_DELAY);
	}

	manager.wiFiConnected(display);
}

void loop() {
	instagram user = getInstagramData(USERNAME);
	print(user);
}

instagram getInstagramData(String username) {
	String url = ENDPOINT + username;
	HTTPClient client;
	client.begin(url);

	int responseCode = client.GET();

	StaticJsonBuffer<CAPACITY> buffer;
	JsonObject& json = buffer.parseObject(client.getString());

	instagram user;

	if (json.success()) {
		user.followers = json["followers"];
		user.following = json["following"];
		user.posts = json["posts"];
	} else {
		Serial.println("Error parsing");
	}

	return user;
}

void print(instagram user) {
	manager.data(display, USERNAME, "Followers", String(user.followers));
	delay(DISPLAY_DELAY);
	manager.data(display, USERNAME, "Following", String(user.following));
	delay(DISPLAY_DELAY);
	manager.data(display, USERNAME, "Posts", String(user.posts));
	delay(DISPLAY_DELAY);
}