#include "pinMap.h"
#include "AccelStepper.h"

AccelStepper stepperX(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper stepperY(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN);
AccelStepper stepperZ(AccelStepper::DRIVER, Z_STEP_PIN, Z_DIR_PIN);
AccelStepper stepperE(AccelStepper::DRIVER, E_STEP_PIN, E_DIR_PIN);
AccelStepper stepperQ(AccelStepper::DRIVER, Q_STEP_PIN, Q_DIR_PIN);
AccelStepper stepperF(AccelStepper::DRIVER, 47, 32);

void setup() {
	pinMode(LED_PIN, OUTPUT);

	pinMode(X_STEP_PIN, OUTPUT);
	pinMode(X_DIR_PIN, OUTPUT);
	pinMode(X_ENABLE_PIN, OUTPUT);

	pinMode(Y_STEP_PIN, OUTPUT);
	pinMode(Y_DIR_PIN, OUTPUT);
	pinMode(Y_ENABLE_PIN, OUTPUT);

	pinMode(Z_STEP_PIN, OUTPUT);
	pinMode(Z_DIR_PIN, OUTPUT);
	pinMode(Z_ENABLE_PIN, OUTPUT);

	pinMode(E_STEP_PIN, OUTPUT);
	pinMode(E_DIR_PIN, OUTPUT);
	pinMode(10, OUTPUT);

	pinMode(Q_STEP_PIN, OUTPUT);
	pinMode(Q_DIR_PIN, OUTPUT);
	pinMode(35, OUTPUT);

	digitalWrite(X_ENABLE_PIN, LOW);
	digitalWrite(Y_ENABLE_PIN, LOW);
	digitalWrite(Z_ENABLE_PIN, LOW);
	digitalWrite(10, LOW);
	digitalWrite(35, LOW);


	stepperE.setMaxSpeed(1000000.0);
	stepperE.setAcceleration(2000000.0);

	stepperQ.setMaxSpeed(10000.0);
	stepperQ.setAcceleration(10000.0);

	stepperF.setMaxSpeed(10000.0);
	stepperF.setAcceleration(10000.0);

	//stepperE.setSpeed(5000.0);


}

void loop() {
	if (millis() % 1000 <500)
		digitalWrite(LED_PIN, HIGH);
	else
		digitalWrite(LED_PIN, LOW);


	stepperX.run();
	stepperY.run();
	stepperZ.run();
	stepperE.run();
	stepperQ.run();

	stepperF.runToNewPosition(3200);
	//stepperE.runToNewPosition(0);
	//stepperE.runToNewPosition(0); // Cause an overshoot then back to 0
	//stepperE.runSpeedToPosition();
	// Now stopped after quickstop
}
