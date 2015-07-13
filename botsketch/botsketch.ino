#include <RedBot.h>
#include <RedBotSoftwareSerial.h>
#include <Servo.h>

#define BEEPER_PIN 9
#define BUTTON_PIN 12
#define LED_PIN 13
#define SPEED 100

RedBotMotors motors = RedBotMotors();
RedBotAccel accel = RedBotAccel();
RedBotSensor leftLine = RedBotSensor(17);
RedBotSensor midLine = RedBotSensor(20);
RedBotSensor rightLine = RedBotSensor(21);
RedBotBumper leftBumper = RedBotBumper(3, &bumpHandler);
RedBotBumper rightBumper = RedBotBumper(11, &bumpHandler);
RedBotEncoder encoder = RedBotEncoder(A2, A0);

void (*func)(); // Function Pointer. Called inside loop(). 
				// Effectively allows us to point loop() at different function as needed
boolean bumped;
int colorDiff;


// Called once at the start by Arduino
void setup() { 
	Serial.begin(9600);
	Serial.println();
	Serial.println("BEGIN");
	pinMode(BUTTON_PIN, INPUT_PULLUP);
	pinMode(LED_PIN, OUTPUT);
	bumped = false;
	encoder.clearEnc(BOTH);
	waitForButton();
	takeReadings();
	waitForButton();
	func = &mainProg;
}

// Called repeatedly by Arduino
void loop() {
	func();
}

void halt(){
	motors.brake();
	while (true){
		// Do nothing
	}
}

// Waits for button press and release. Blinks LED at half-second intervals as visual prompt. 
void waitForButton(){
	Serial.println("Waiting For Button");
	unsigned long i = millis();
	while (digitalRead(BUTTON_PIN) == HIGH){
		if ((millis() - i) > 500){
			blinker();
			i = millis();
		}
	}
	while (digitalRead(BUTTON_PIN) == LOW){
		if ((millis() - i) > 500){
			blinker();
			i = millis();
		}
	}
	Serial.println("Button Press Detected");
	digitalWrite(LED_PIN, 0);
}



void takeReadings(){
	Serial.println("=== Taking line sensor readings ===");
	waitForButton();
	leftLine.setBGLevel();
	midLine.setBGLevel();
	rightLine.setBGLevel();
	waitForButton();
	leftLine.setDetectLevel();
	midLine.setDetectLevel();
	rightLine.setDetectLevel();
	Serial.println("=== Finished line sensor readings ===");
}

void mainProg(){
	encoder.clearEnc(BOTH);
	while (!bumped && !lineCheck()){
		if (stuck() > 4000){
			reverse();
			turn();
		} else {
			motors.drive(SPEED);
		}
	} 
	reverse();
	bumped = false;
	turn();
}

void reverse(){
	encoder.clearEnc(BOTH);
	while (encoder.getTicks(LEFT) > -100 && encoder.getTicks(RIGHT) > -100){
		if (stuck() > 4000){
			break;
		}
		motors.drive(-SPEED);
	}
}

void turn(){
	encoder.clearEnc(BOTH);
	while (encoder.getTicks(RIGHT) < 100){
		if (stuck() > 4000){
			break;
		}
		motors.rightDrive(SPEED);
	}
}

// Returns the time in milliseconds since wheels have turned freely
// This can be inserted into any loop, so long as...
//		1. You clear the encoders before the loop
// 		2. The loop does not clear the encoders internally
//		3. Both wheels are supposed to be turning inside the loop.
unsigned long stuck(){
	static unsigned long time = millis();
	static int left = encoder.getTicks(LEFT);
	static int right = encoder.getTicks(RIGHT);
	int t = millis();
	int l = encoder.getTicks(LEFT);
	int r = encoder.getTicks(RIGHT);
	if (left != encoder.getTicks(LEFT) && right != encoder.getTicks(RIGHT)){
		time = t;
		left = l;
		right = r;
		return 0;
	} else {
		return t - time;
	}
}

// Toggles the LED each call. 
void blinker(){
	static boolean on = true;
	if (on){
		digitalWrite(LED_PIN, 1);
	} else {
		digitalWrite(LED_PIN, 0);
	} 
	on = !on;
}

void bumpHandler(){
	bumped = true;
	motors.brake();
	Serial.println("Bump Detected!");
}

boolean lineCheck(){
	return leftLine.check() || midLine.check() || rightLine.check();
}
