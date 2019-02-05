#include "DisplayManager.h"

DisplayManager::DisplayManager() {}

void DisplayManager::connectingWiFi(SSD1306 display) {
	this->clear(display);

	display.setFont(ArialMT_Plain_16);
	display.setTextAlignment(TEXT_ALIGN_CENTER);

	display.drawString(64, 20, "Connecting WiFi...");

	this->enable(display);
}

void DisplayManager::wiFiConnected(SSD1306 display) {
	this->clear(display);

	display.setFont(ArialMT_Plain_16);
	display.setTextAlignment(TEXT_ALIGN_CENTER);

	display.drawString(64, 20, "WiFi connected");

	this->enable(display);
}

void DisplayManager::data(SSD1306 display, String name, String label, String value) {
	this->clear(display);

	display.setTextAlignment(TEXT_ALIGN_CENTER);

	display.setFont(ArialMT_Plain_16);
	display.drawString(64, 1, "@" + name);

	display.setFont(ArialMT_Plain_10);
	display.drawString(64, 20, label);

	display.setFont(ArialMT_Plain_24);
	display.drawString(64, 30, value);

	this->enable(display);
}

void DisplayManager::clear(SSD1306 display) {
	display.clear();
	display.flipScreenVertically();
}

void DisplayManager::enable(SSD1306 display) {
	display.display();
}