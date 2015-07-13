#include <RedBot.h>
#include <RedBotSoftwareSerial.h>

#define BEEPER_PIN 9
#define BUTTON_PIN 12
#define LED_PIN 13

RedBotMotors motors = RedBotMotors();
RedBotAccel accel = RedBotAccel();
RedBotSensor leftLine = RedBotSensor(17);
RedBotSensor midLine = RedBotSensor(20);
RedBotSensor rightLine = RedBotSensor(21);
RedBotBumper leftBumper = RedBotBumper(3);
RedBotBumper rightBumper = RedBotBumper(11);

void (*func)(); // Function Pointer. Called inside loop(). 
				// Effectively allows us to point loop() at different function as needed

void setup() {
	Serial.begin(9600);
	Serial.println();
	Serial.println("BEGIN");
	pinMode(BUTTON_PIN, INPUT_PULLUP);
	pinMode(LED_PIN, OUTPUT);
	Serial.println("Waiting For Button");
	func = &waitForButton; // Initialize func to wait for the button.
}

void loop() {
	func();
}

void waitForButton(){
	if (digitalRead(BUTTON_PIN) == LOW){
		Serial.println("Begin Main Program Loop");
		func = &mainProg;
	}
}

void mainProg(){
	blinker();
	delay(1000);
}

void blinker(){
	static boolean on = false;
	if (on){
		digitalWrite(LED_PIN, 1);
	} else {
		digitalWrite(LED_PIN, 0);
	} 
	on = !on;
}