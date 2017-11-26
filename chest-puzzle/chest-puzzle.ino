#include <SoftwareSerial.h>

#define ButtonPin			2		// connect this pin to the reed contact
#define Mp3ModulePin		A2		// connect this pin over 1k Ohm to the RX pin of the MP3 module
#define Volume				30		// max: 30
#define DebounceTime		20		// in ms

SoftwareSerial Mp3Module(255, Mp3ModulePin);


void setup() {
	uint32_t previousMillis = 0;
	pinMode(ButtonPin, INPUT_PULLUP);
	Mp3Module.begin(9600);
	SetVolume(Volume);
	while (previousMillis == 0 || millis() - previousMillis < DebounceTime) {
		if (digitalRead(ButtonPin) == HIGH) {
			if (previousMillis == 0) {
				previousMillis = millis();
			}
		}
		else if (previousMillis != 0) {
			previousMillis = 0;
		}
	}
	PlayFile(1);
}


void loop() {
	// empty loop
}


void SendCommand(uint8_t cmd, uint8_t paraml=0x00, uint8_t paramh=0x00) {
	uint8_t buf[] = {0x7E, 0xFF, 0x06, cmd, 0x00, paramh, paraml, 0xEF};
	for (uint8_t i = 0; i < 8; i++) {
		Mp3Module.write(buf[i]);
	}
}


void SetVolume(byte volume) {
	SendCommand(0x10, volume, 0x01);
}


void PlayFile(uint8_t file){
	SendCommand(0x03, file, 0);
}

