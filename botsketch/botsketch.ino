#include <RedBot.h>
#include <RedBotSoftwareSerial.h>

RedBotMotors motors = RedBotMotors();
RedBotAccel accel = RedBotAccel();
RedBotSensor leftLine = RedBotSensor(17);
RedBotSensor midLine = RedBotSensor(20);
RedBotSensor rightLine = RedBotSensor(21);
RedBotBumper leftBumper = RedBotBumper(3);
RedBotBumper rightBumper = RedBotBumper(11);

void setup() {
	Serial.begin(9600);
	Serial.println("BEGIN");
}

void loop() {
}
