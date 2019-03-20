#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <PxMatrix.h>

#define P_LAT 16
#define P_A 5
#define P_B 4
#define P_C 15
#define P_D 12
#define P_E 0
#define P_OE 2

// Pins for LED MATRIX

#define matrix_width 64
#define matrix_height 32

Ticker display_ticker;

const char* WIFI_SSID = "WIFI_SSID";
const char* WIFI_PASS = "WIFI_PASS";

const String USERNAME = "3siete3_cafe";
const String ENDPOINT = "http://sergiocasero.es/instagram.php?only_followers=true&username=";

unsigned long api_mtbs = 2000; //mean time between api requests
unsigned long api_lasttime = 0;   //last time api request has been done

int latestFollowers = 0;

// PxMATRIX display(32,16,P_LAT, P_OE,P_A,P_B,P_C);
// PxMATRIX display(64,32,P_LAT, P_OE,P_A,P_B,P_C,P_D);
PxMATRIX display(64, 32, P_LAT, P_OE, P_A, P_B, P_C, P_D, P_E);

// Some standard colors
uint16_t myRED = display.color565(255, 0, 0);
uint16_t myGREEN = display.color565(0, 255, 0);
uint16_t myBLUE = display.color565(0, 0, 255);
uint16_t myWHITE = display.color565(255, 255, 255);
uint16_t myYELLOW = display.color565(255, 255, 0);
uint16_t myCYAN = display.color565(0, 255, 255);
uint16_t myMAGENTA = display.color565(255, 0, 255);
uint16_t myBLACK = display.color565(0, 0, 0);
uint16_t instagramRedViolet = display.color565(255, 0, 255);

uint16 myCOLORS[8] = {myRED, myGREEN, myBLUE, myWHITE, myYELLOW, myCYAN, myMAGENTA, myBLACK};

void setup() {
	Serial.begin(9600);
	// Define your display layout here, e.g. 1/8 step
	display.begin(16);
	display.clearDisplay();
	display.setTextColor(instagramRedViolet);
	display.setTextSize(1.5);
	display.setCursor(2, 0);
	display.print("Conectando");

	display.setCursor(2, 16);
	display.print("WiFi...");

	// Set WiFi to station mode and disconnect from an AP if it was Previously
	// connected
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	delay(100);

	// Attempt to connect to Wifi network:
	Serial.print("Connecting Wifi: ");
	Serial.println(WIFI_SSID);
	WiFi.begin(WIFI_SSID, WIFI_PASS);
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		delay(500);
	}
	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	IPAddress ip = WiFi.localIP();
	Serial.println(ip);

	Serial.print("Pixel draw latency in us: ");
	unsigned long start_timer = micros();
	display.drawPixel(1, 1, 0);
	unsigned long delta_timer = micros() - start_timer;
	Serial.println(delta_timer);

	Serial.print("Display update latency in us: ");
	start_timer = micros();
	display.display(0);
	delta_timer = micros() - start_timer;
	Serial.println(delta_timer);

	getInstagramData();

	display_ticker.attach(0.02, display_updater);
	yield();
	delay(1000);
	display.clearDisplay();
 	updateScreen(latestFollowers);
}

void display_updater() {
	display.display(20);
}

void displayText(String text, int yPos, float textSize) {
	int16_t  x1, y1;
	uint16_t w, h;
	display.setTextSize(textSize);
	char charBuf[text.length() + 1];
	text.toCharArray(charBuf, text.length() + 1);
	display.getTextBounds(charBuf, 0, yPos, &x1, &y1, &w, &h);
	int startingX = 33 - (w / 2);
	if (startingX < 0) {
		display.getTextBounds(charBuf, 0, yPos, &x1, &y1, &w, &h);
		startingX = 33 - (w / 2);
	}
	display.setCursor(startingX, yPos);
	display.print(text);
}	

void updateScreen(long subCount) {
	display.clearDisplay();
	displayText("3siete3", 1, 0.5);
	displayText("FOLLOWERS", 1, 0.5);
	displayText(String(subCount), 18, 2.5);
}

void loop() {
	if (millis() - api_lasttime > api_mtbs)  {
		getInstagramData();
		api_lasttime = millis();
	}
}

void getInstagramData() {
	WiFiClient client;
    HTTPClient http;
    String url = ENDPOINT + USERNAME;

    http.setTimeout(60000);
	if (http.begin(client, url)) {
		int httpCode = http.GET();
		if (httpCode > 0) {
			if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
				String payload = http.getString();
				int newFollowers = payload.toInt();

				if (latestFollowers != newFollowers) {
					latestFollowers = newFollowers;
 					updateScreen(latestFollowers);
				}
			}
		}

		http.end();
	}

}
