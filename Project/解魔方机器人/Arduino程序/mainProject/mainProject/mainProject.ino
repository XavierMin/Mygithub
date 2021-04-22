#include <TimerOne.h>
#include <AccelStepper.h>
#include <MultiStepper.h>

/*
 Name:		mainProject.ino
 Created:	2018/4/17 17:12:45
 Author:	Fish
*/
#include "pinMap.h"
//#include "AccelStepper.h"
//#include "TimerOne.h"
//#define DEBUG 1

#define STEPPER_MAX_SPEED 800.0              //7500    4000
#define STEPPER_CLAW_MAX_SPEED 1000.0         //6000   12000
#define STEPPER_CLAW_ACCELERATION 20000.0     //20000   20000
#define STEPPER_ACCELERATION 20000.0          //25000   20000
#define STEP_PER_TURN 200                     //200     200
#define STEP_LOOSE_CLAW 450                   //450     450
#define STEP_LOOSE 400                        //400     400
#define STEP_TIGHT -30                        //-30     -30

_Bool ledStatus = 0;


AccelStepper stepperX(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);//前面步进电机
AccelStepper stepperY(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN);//后面步进电机
AccelStepper stepperZ(AccelStepper::DRIVER, Z_STEP_PIN, Z_DIR_PIN);//前后松紧步进电机
AccelStepper stepperE(AccelStepper::DRIVER, E_STEP_PIN, E_DIR_PIN);//左面步进电机
AccelStepper stepperQ(AccelStepper::DRIVER, Q_STEP_PIN, Q_DIR_PIN);//右面步进电机
AccelStepper stepperR(AccelStepper::DRIVER, R_STEP_PIN, R_DIR_PIN);//左右松紧步进电机

struct cubeTurn
{
	_Bool nReady, isProcess, isNeedSyncMove, isSyncMoveFinish, cubeStatus=0;
	char face;
	int  degree, direction;
}cubeTurn;

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);

	pinMode(LED_PIN, OUTPUT);

	stepperX.setEnablePin(X_ENABLE_PIN);
	stepperX.setPinsInverted(0,0,1);	
	stepperX.setMaxSpeed(STEPPER_MAX_SPEED);
	stepperX.setAcceleration(STEPPER_ACCELERATION);
	stepperX.disableOutputs();

	stepperY.setEnablePin(Y_ENABLE_PIN);
	stepperY.setPinsInverted(0, 0, 1);	
	stepperY.setMaxSpeed(STEPPER_MAX_SPEED);
	stepperY.setAcceleration(STEPPER_ACCELERATION);
	stepperY.disableOutputs();

	stepperZ.setEnablePin(Z_ENABLE_PIN);                    
	stepperZ.setPinsInverted(0, 0, 1);	
	stepperZ.setMaxSpeed(STEPPER_CLAW_MAX_SPEED);
	stepperZ.setAcceleration(STEPPER_CLAW_ACCELERATION);
	stepperZ.disableOutputs();

	stepperR.setEnablePin(R_ENABLE_PIN);
	stepperR.setPinsInverted(0, 0, 1);
	stepperR.setMaxSpeed(STEPPER_CLAW_MAX_SPEED);
	stepperR.setAcceleration(STEPPER_CLAW_ACCELERATION);
	stepperR.disableOutputs();

	stepperE.setEnablePin(E_ENABLE_PIN);
	stepperE.setPinsInverted(0, 0, 1);	
	stepperE.setMaxSpeed(STEPPER_MAX_SPEED);
	stepperE.setAcceleration(STEPPER_ACCELERATION);
	stepperE.disableOutputs();

	stepperQ.setEnablePin(Q_ENABLE_PIN);
	stepperQ.setPinsInverted(0, 0, 1);	
	stepperQ.setMaxSpeed(STEPPER_MAX_SPEED);
	stepperQ.setAcceleration(STEPPER_ACCELERATION);
	stepperQ.disableOutputs();

	pinMode(R_POWER_PIN, OUTPUT);
	digitalWrite(R_POWER_PIN, LOW);

	Timer1.initialize(100000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
	Timer1.attachInterrupt(timerIsr); // attach the service routine here
}

// the loop function runs over and over again until power down or reset
void loop() {
	stepperX.run();
	stepperY.run();
	stepperZ.run();
	stepperE.run();
	stepperQ.run();
	if (cubeTurn.face=='F'&&stepperX.distanceToGo() == 0 && cubeTurn.isProcess == 1)
	{
		if (cubeTurn.degree % 2 != 0)
		{
			stepperX.runToNewPosition(cubeTurn.degree*STEP_PER_TURN);
			stepperR.enableOutputs();
			stepperR.runToNewPosition(STEP_LOOSE);
			stepperX.runToNewPosition(0);
			stepperR.runToNewPosition(STEP_TIGHT);
			stepperR.setCurrentPosition(0);
			stepperR.disableOutputs();
		}
		else
		{
			stepperX.runToNewPosition(cubeTurn.degree*STEP_PER_TURN);
			stepperX.setCurrentPosition(0);
		}

		stepperX.disableOutputs();
		Serial.println("OK");
		cubeTurn.isProcess = 0;
	}
	else if (cubeTurn.face == 'B'&&stepperY.distanceToGo() == 0 && cubeTurn.isProcess == 1)
	{
		if (cubeTurn.degree % 2 != 0)
		{
			stepperY.runToNewPosition(cubeTurn.degree*STEP_PER_TURN);
			stepperR.enableOutputs();
			stepperR.runToNewPosition(STEP_LOOSE);
			stepperY.runToNewPosition(0);
			stepperR.runToNewPosition(STEP_TIGHT);
			stepperR.setCurrentPosition(0);
			stepperR.disableOutputs();
		}
		else
		{
			stepperY.runToNewPosition(cubeTurn.degree*STEP_PER_TURN);			
			stepperY.setCurrentPosition(0);
		}

		stepperY.disableOutputs();
		Serial.println("OK");
		cubeTurn.isProcess = 0;
	}
	else if (cubeTurn.face == 'L'&&stepperE.distanceToGo() == 0 && cubeTurn.isProcess == 1)
	{
		if (cubeTurn.cubeStatus == 1)
		{
			if (cubeTurn.isNeedSyncMove == 0)
			{
				stepperZ.enableOutputs();
				stepperZ.runToNewPosition(STEP_LOOSE_CLAW);

				stepperX.moveTo(STEP_PER_TURN);
				stepperY.moveTo(-STEP_PER_TURN);
				cubeTurn.isNeedSyncMove = 1;
				cubeTurn.isSyncMoveFinish = 0;

			}
			else if (cubeTurn.isNeedSyncMove == 1 && cubeTurn.isSyncMoveFinish == 0)
			{

			}
			else if (cubeTurn.isNeedSyncMove == 1 && cubeTurn.isSyncMoveFinish == 1)
			{
				cubeTurn.isNeedSyncMove = 0;
				stepperZ.runToNewPosition(STEP_TIGHT);
				stepperZ.setCurrentPosition(0);
				stepperZ.disableOutputs();
				stepperR.enableOutputs();
				stepperR.runToNewPosition(STEP_LOOSE);
				stepperX.runToNewPosition(0);
				stepperY.runToNewPosition(0);
				stepperR.runToNewPosition(STEP_TIGHT);
				stepperR.setCurrentPosition(0);
				stepperR.disableOutputs();
#ifdef DEBUG
				Serial.println("Status change");
#endif // DEBUG

				
				cubeTurn.cubeStatus = 0;
				if (cubeTurn.degree % 2 != 0)
				{
					stepperE.runToNewPosition(cubeTurn.degree*STEP_PER_TURN);
					stepperZ.enableOutputs();
					stepperZ.runToNewPosition(STEP_LOOSE);
					stepperE.runToNewPosition(0);
					stepperZ.runToNewPosition(STEP_TIGHT);
					stepperZ.setCurrentPosition(0);
					stepperZ.disableOutputs();
				}
				else
				{
					stepperE.runToNewPosition(cubeTurn.degree*STEP_PER_TURN);
					stepperE.setCurrentPosition(0);
				}
				stepperX.disableOutputs();
				stepperY.disableOutputs();
				stepperZ.disableOutputs();
				stepperE.disableOutputs();
				stepperQ.disableOutputs();
				Serial.println("OK");
				cubeTurn.isProcess = 0;
			}
			else
			{

			}
		}
		else
		{
			if (cubeTurn.degree % 2 != 0)
			{
				stepperE.runToNewPosition(cubeTurn.degree*STEP_PER_TURN);
				stepperZ.enableOutputs();
				stepperZ.runToNewPosition(STEP_LOOSE);
				stepperE.runToNewPosition(0);
				stepperZ.runToNewPosition(STEP_TIGHT);
				stepperZ.setCurrentPosition(0);
				stepperZ.disableOutputs();
			}
			else
			{
				stepperE.runToNewPosition(cubeTurn.degree*STEP_PER_TURN);
				stepperE.setCurrentPosition(0);
			}
			stepperX.disableOutputs();
			stepperY.disableOutputs();
			stepperZ.disableOutputs();
			stepperE.disableOutputs();
			stepperQ.disableOutputs();
			Serial.println("OK");
			cubeTurn.isProcess = 0;
		}

	}
	else if (cubeTurn.face == 'R'&&stepperQ.distanceToGo() == 0 && cubeTurn.isProcess == 1)
	{
		if (cubeTurn.cubeStatus == 1)
		{
			if (cubeTurn.isNeedSyncMove == 0)
			{
				stepperZ.enableOutputs();
				stepperZ.runToNewPosition(STEP_LOOSE_CLAW);

				stepperX.moveTo(STEP_PER_TURN);
				stepperY.moveTo(-STEP_PER_TURN);
				cubeTurn.isNeedSyncMove = 1;
				cubeTurn.isSyncMoveFinish = 0;

			}
			else if (cubeTurn.isNeedSyncMove == 1 && cubeTurn.isSyncMoveFinish == 0)
			{

			}
			else if (cubeTurn.isNeedSyncMove == 1 && cubeTurn.isSyncMoveFinish == 1)
			{
				cubeTurn.isNeedSyncMove = 0;
				stepperZ.runToNewPosition(STEP_TIGHT);
				stepperZ.setCurrentPosition(0);
				stepperZ.disableOutputs();
				stepperR.enableOutputs();
				stepperR.runToNewPosition(STEP_LOOSE);
				stepperX.runToNewPosition(0);
				stepperY.runToNewPosition(0);
				stepperR.runToNewPosition(STEP_TIGHT);
				stepperR.setCurrentPosition(0);
				stepperR.disableOutputs();
#ifdef DEBUG
				Serial.println("Status change");
#endif // DEBUG
				cubeTurn.cubeStatus = 0;
				if (cubeTurn.degree % 2 != 0)
				{
					stepperQ.runToNewPosition(cubeTurn.degree*STEP_PER_TURN);
					stepperZ.enableOutputs();
					stepperZ.runToNewPosition(STEP_LOOSE);
					stepperQ.runToNewPosition(0);
					stepperZ.runToNewPosition(STEP_TIGHT);
					stepperZ.setCurrentPosition(0);
					stepperZ.disableOutputs();
				}
				else
				{
					stepperQ.runToNewPosition(cubeTurn.degree*STEP_PER_TURN);
					stepperQ.setCurrentPosition(0);
				}
				stepperX.disableOutputs();
				stepperY.disableOutputs();
				stepperZ.disableOutputs();
				stepperE.disableOutputs();
				stepperQ.disableOutputs();
				Serial.println("OK");
				cubeTurn.isProcess = 0;
			}
			else
			{

			}
		}
		else
		{
			if (cubeTurn.degree % 2 != 0)
			{
				stepperQ.runToNewPosition(cubeTurn.degree*STEP_PER_TURN);
				stepperZ.enableOutputs();
				stepperZ.runToNewPosition(STEP_LOOSE);
				stepperQ.runToNewPosition(0);
				stepperZ.runToNewPosition(STEP_TIGHT);
				stepperZ.setCurrentPosition(0);
				stepperZ.disableOutputs();
			}
			else
			{
				stepperQ.runToNewPosition(cubeTurn.degree*STEP_PER_TURN);
				stepperQ.setCurrentPosition(0);
			}
			stepperX.disableOutputs();
			stepperY.disableOutputs();
			stepperZ.disableOutputs();
			stepperE.disableOutputs();
			stepperQ.disableOutputs();
			Serial.println("OK");
			cubeTurn.isProcess = 0;
		}

	}
	else if (cubeTurn.face == 'U' && cubeTurn.isProcess == 1 )
	{
		if (cubeTurn.cubeStatus == 0)
		{
			if (cubeTurn.isNeedSyncMove == 0)
			{
				stepperZ.enableOutputs();
				stepperZ.runToNewPosition(STEP_LOOSE_CLAW);

				stepperX.moveTo(-STEP_PER_TURN);
				stepperY.moveTo(STEP_PER_TURN);
				cubeTurn.isNeedSyncMove = 1;
				cubeTurn.isSyncMoveFinish = 0;

			}
			else if (cubeTurn.isNeedSyncMove == 1 && cubeTurn.isSyncMoveFinish == 0)
			{

			}
			else if (cubeTurn.isNeedSyncMove == 1 && cubeTurn.isSyncMoveFinish == 1)
			{
				cubeTurn.isNeedSyncMove = 0;
				stepperZ.runToNewPosition(STEP_TIGHT);
				stepperZ.setCurrentPosition(0);
				stepperZ.disableOutputs();
				stepperR.enableOutputs();
				stepperR.runToNewPosition(STEP_LOOSE);
				stepperX.runToNewPosition(0);
				stepperY.runToNewPosition(0);
				stepperR.runToNewPosition(STEP_TIGHT);
				stepperR.setCurrentPosition(0);
				stepperR.disableOutputs();
#ifdef DEBUG
				Serial.println("Status change");
#endif // DEBUG
				cubeTurn.cubeStatus = 1;
				if (cubeTurn.degree % 2 != 0)
				{
					stepperE.runToNewPosition(cubeTurn.degree*STEP_PER_TURN);
					stepperZ.enableOutputs();
					stepperZ.runToNewPosition(STEP_LOOSE);
					stepperE.runToNewPosition(0);
					stepperZ.runToNewPosition(STEP_TIGHT);
					stepperZ.setCurrentPosition(0);
					stepperZ.disableOutputs();
				}
				else
				{
					stepperE.runToNewPosition(cubeTurn.degree*STEP_PER_TURN);
					stepperE.setCurrentPosition(0);
				}
				stepperX.disableOutputs();
				stepperY.disableOutputs();
				stepperZ.disableOutputs();
				stepperE.disableOutputs();
				stepperQ.disableOutputs();
				Serial.println("OK");
				cubeTurn.isProcess = 0;
			}
			else
			{

			}
		}
		else
		{
			if (cubeTurn.degree % 2 != 0)
			{
				stepperE.runToNewPosition(cubeTurn.degree*STEP_PER_TURN);
				stepperZ.enableOutputs();
				stepperZ.runToNewPosition(STEP_LOOSE);
				stepperE.runToNewPosition(0);
				stepperZ.runToNewPosition(STEP_TIGHT);
				stepperZ.setCurrentPosition(0);
				stepperZ.disableOutputs();
			}
			else
			{
				stepperE.runToNewPosition(cubeTurn.degree*STEP_PER_TURN);
				stepperE.setCurrentPosition(0);
			}
			stepperX.disableOutputs();
			stepperY.disableOutputs();
			stepperZ.disableOutputs();
			stepperE.disableOutputs();
			stepperQ.disableOutputs();
			Serial.println("OK");
			cubeTurn.isProcess = 0;
		}

	}
	else if (cubeTurn.face == 'D' && cubeTurn.isProcess == 1)
	{
		if (cubeTurn.cubeStatus == 0)
		{
			if (cubeTurn.isNeedSyncMove == 0)
			{
				stepperZ.enableOutputs();
				stepperZ.runToNewPosition(STEP_LOOSE_CLAW);

				stepperX.moveTo(-STEP_PER_TURN);
				stepperY.moveTo(STEP_PER_TURN);
				cubeTurn.isNeedSyncMove = 1;
				cubeTurn.isSyncMoveFinish = 0;

			}
			else if (cubeTurn.isNeedSyncMove == 1 && cubeTurn.isSyncMoveFinish == 0)
			{

			}
			else if (cubeTurn.isNeedSyncMove == 1 && cubeTurn.isSyncMoveFinish == 1)
			{
				cubeTurn.isNeedSyncMove = 0;
				stepperZ.runToNewPosition(STEP_TIGHT);
				stepperZ.setCurrentPosition(0);
				stepperZ.disableOutputs();
				stepperR.enableOutputs();
				stepperR.runToNewPosition(STEP_LOOSE);
				stepperX.runToNewPosition(0);
				stepperY.runToNewPosition(0);
				stepperR.runToNewPosition(STEP_TIGHT);
				stepperR.setCurrentPosition(0);
				stepperR.disableOutputs();
#ifdef DEBUG
				Serial.println("Status change");
#endif // DEBUG
				cubeTurn.cubeStatus = 1;
				if (cubeTurn.degree % 2 != 0)
				{
					stepperQ.runToNewPosition(cubeTurn.degree*STEP_PER_TURN);
					stepperZ.enableOutputs();
					stepperZ.runToNewPosition(STEP_LOOSE);
					stepperQ.runToNewPosition(0);
					stepperZ.runToNewPosition(STEP_TIGHT);
					stepperZ.setCurrentPosition(0);
					stepperZ.disableOutputs();
				}
				else
				{
					stepperQ.runToNewPosition(cubeTurn.degree*STEP_PER_TURN);
					stepperQ.setCurrentPosition(0);
				}
				stepperX.disableOutputs();
				stepperY.disableOutputs();
				stepperZ.disableOutputs();
				stepperE.disableOutputs();
				stepperQ.disableOutputs();
				Serial.println("OK");
				cubeTurn.isProcess = 0;
			}
			else
			{

			}
		}
		else
		{
			if (cubeTurn.degree % 2 != 0)
			{
				stepperQ.runToNewPosition(cubeTurn.degree*STEP_PER_TURN);
				stepperZ.enableOutputs();
				stepperZ.runToNewPosition(STEP_LOOSE);
				stepperQ.runToNewPosition(0);
				stepperZ.runToNewPosition(STEP_TIGHT);
				stepperZ.setCurrentPosition(0);
				stepperZ.disableOutputs();
			}
			else
			{
				stepperQ.runToNewPosition(cubeTurn.degree*STEP_PER_TURN);
				stepperQ.setCurrentPosition(0);
			}
			stepperX.disableOutputs();
			stepperY.disableOutputs();
			stepperZ.disableOutputs();
			stepperE.disableOutputs();
			stepperQ.disableOutputs();
			Serial.println("OK");
			cubeTurn.isProcess = 0;
		}

	}
}

/// --------------------------
/// Custom ISR Timer Routine
/// --------------------------
void timerIsr()
{

	if (cubeTurn.nReady == 1&& cubeTurn.isProcess == 0)
	{
		switch (cubeTurn.face)
		{
		case'F':stepperX.enableOutputs();cubeTurn.isProcess = 1;break;
		case'B':stepperY.enableOutputs();cubeTurn.isProcess = 1;break;
		case'L':stepperX.enableOutputs();stepperY.enableOutputs();stepperE.enableOutputs();stepperQ.enableOutputs();cubeTurn.isProcess = 1;break;
		case'R':stepperX.enableOutputs();stepperY.enableOutputs();stepperE.enableOutputs();stepperQ.enableOutputs();cubeTurn.isProcess = 1;break;
		case'U':stepperX.enableOutputs();stepperY.enableOutputs();stepperE.enableOutputs();stepperQ.enableOutputs();cubeTurn.isProcess = 1;break;
		case'D':stepperX.enableOutputs();stepperY.enableOutputs();stepperE.enableOutputs();stepperQ.enableOutputs();cubeTurn.isProcess = 1;break;
		default:
			break;
		}
		cubeTurn.nReady = 0;
	}
	else if (cubeTurn.isProcess == 1)
	{
		switch (cubeTurn.face)
		{
		case'F':break;
		case'B':break;
		case'L':
			if (cubeTurn.isNeedSyncMove == 1)
			{
				if (stepperX.distanceToGo() == 0&&stepperY.distanceToGo() == 0 )
				{
					cubeTurn.isSyncMoveFinish = 1;
				}
			}
			break;
		case'R':
			if (cubeTurn.isNeedSyncMove == 1)
			{
				if (stepperX.distanceToGo() == 0 && stepperY.distanceToGo() == 0)
				{
					cubeTurn.isSyncMoveFinish = 1;
				}
			}
			break;
		case'U':
			if (cubeTurn.isNeedSyncMove == 1)
			{
				if (stepperX.distanceToGo() == 0 && stepperY.distanceToGo() == 0)
				{
					cubeTurn.isSyncMoveFinish = 1;
				}
			}
			break;
		case'D':
			if (cubeTurn.isNeedSyncMove == 1)
			{
				if (stepperX.distanceToGo() == 0 && stepperY.distanceToGo() == 0)
				{
					cubeTurn.isSyncMoveFinish = 1;
				}
			}
			break;
		default:
			break;
		}
	}
}

/*
SerialEvent occurs whenever a new data comes in the hardware serial RX. This
routine is run between each time loop() runs, so using delay inside loop can
delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
	digitalWrite(LED_PIN, ledStatus ^= 1);
	String serialBuff = "";
	while (Serial.available() > 0)
	{
		serialBuff += char(Serial.read());
		delay(3);
#ifdef DEBUG
		Serial.print(" " + serialBuff + " ");
#endif // DEBUG
	}

		if (serialBuff[0] == 'F' || serialBuff[0] == 'B' || serialBuff[0] == 'L' || serialBuff[0] == 'R' || serialBuff[0] == 'U' || serialBuff[0] == 'D')
		{
#ifdef DEBUG
			Serial.print("GET ");
#endif // DEBUG
			cubeTurn.nReady = 1;
			cubeTurn.face = serialBuff[0];
			if (serialBuff.indexOf('\'')!=-1)
			{
				cubeTurn.direction = -1;
			}
			else
			{
				cubeTurn.direction = 1;
			}
			if (serialBuff[1]=='2')
			{
				cubeTurn.direction = 1;
				cubeTurn.degree = 2 * cubeTurn.direction;
			}
			else if (serialBuff[1] == '3')
			{
				cubeTurn.direction = -1;
				cubeTurn.degree = 1 * cubeTurn.direction;
			}
			else
			{
				cubeTurn.direction = 1;
				cubeTurn.degree = 1* cubeTurn.direction;
			}
#ifdef DEBUG
			Serial.print(cubeTurn.face);
			Serial.println(cubeTurn.degree);
#endif // DEBUG


		}
	
	digitalWrite(LED_PIN, ledStatus ^= 1);
}
