#ifndef DisplayManager_h
#define DisplayManager_h

#include <Wire.h>
#include "SSD1306.h"

class DisplayManager {
public:
	DisplayManager();
	
	void connectingWiFi(SSD1306 display);
	void wiFiConnected(SSD1306 display);
	void data(SSD1306 display, String name, String label, String value);

private:
	void clear(SSD1306 display);
	void enable(SSD1306 display);
};

#endif