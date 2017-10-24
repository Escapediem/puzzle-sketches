#include <SoftwareSerial.h>

#define useSameSound		0		// change this to 1 to use the same sound for all candles
#define ButtonPin			A1		// connect this pin to the button
#define Mp3ModulePin		A2		// connect this pin over 1k Ohm to the RX pin of the MP3 module
#define Volume				30		// max: 30
#define DelayTime			500		// in ms

SoftwareSerial Mp3Module(255, Mp3ModulePin);

const uint8_t RelayPins[]  = {2, 3, 4, 5, 6, 7, 8, 9};
bool lastState = LOW;


void setup() {
	pinMode(ButtonPin, INPUT_PULLUP);
	for (uint8_t i = 0; i < sizeof(RelayPins); i++) {
		pinMode(RelayPins[i], OUTPUT);
		digitalWrite(RelayPins[i], HIGH);
	}
	Mp3Module.begin(9600);
	SetVolume(Volume);
}


void loop() {
	if (digitalRead == LOW) {
		if (lastState == HIGH) {
			for (uint8_t i = 0; i < sizeof(RelayPins); i++) {
				digitalWrite(RelayPins[i], LOW);
				#if useSameSound
				PlayFile(1);
				#else
				PlayFile(i+1);
				#endif
				delay(DelayTime);
			}
			lastState == LOW;
		}
	}
	else if (lastState == LOW) {
		for (uint8_t i = 0; i < sizeof(RelayPins); i++) {
			digitalWrite(RelayPins[i], HIGH);
		}
		lastState == HIGH;
	}
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
