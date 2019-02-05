#ifndef DisplayManager_h
#define DisplayManager_h

// Screen
#include <Wire.h>
#include "SSD1306.h"

class DisplayManager {
public:
	DisplayManager();

	void connectingWiFi(SSD1306 display);

	void wifiConnected(SSD1306 display);

	void data(SSD1306 display, String name, String label, String value);

private:
	void clearDisplay(SSD1306 display);

	void enableDisplay(SSD1306 display);
};

#endif