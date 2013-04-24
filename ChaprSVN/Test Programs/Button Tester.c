#pragma config(Hubs,  S1, HTMotor,  HTServo,  none,     none)
#pragma config(Motor,  mtr_S1_C1_1,     ,              tmotorNone, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     ,              tmotorNone, openLoop)

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.


void initializeRobot()
{
  StopTask(displayDiagnostics);
  eraseDisplay();
  return;
}

void checkButtons1(TJoystick joystick)
{
	static char buffer1[20];
	char *ptr;

	ptr = &(buffer1[0]);
	*ptr = '-';
	ptr++;

	for (int i = 1; i <= 6; i++){
		if (joy1Btn(i)){
			sprintf(ptr,"%2d", i);
	  }else {
			sprintf(ptr,"  ");
	  }
		ptr += 2;
	}
		nxtDisplayTextLine(2, buffer1);

	ptr = &(buffer1[0]);
	*ptr = '-';
	ptr++;

	for (int i = 7; i <= 12; i++){
		if (joy1Btn(i)){
			sprintf(ptr,"%2d", i);
	  }else {
			sprintf(ptr,"  ");
	  }
		ptr += 2;
	}
		nxtDisplayTextLine(3, buffer1);
		sprintf(buffer1, "%d", joystick.joy1_TopHat);
		nxtDisplayTextLine(4, buffer1);
}

void checkButtons2 (TJoystick joystick){
	static char buffer1[20];
	char *ptr;

	ptr = &(buffer1[0]);
	*ptr = '-';
	ptr++;

	for (int i = 1; i <= 6; i++){
		if (joy2Btn(i)){
			sprintf(ptr,"%2d", i);
	  }else {
			sprintf(ptr,"  ");
	  }
		ptr += 2;
	}
		nxtDisplayTextLine(5, buffer1);

	ptr = &(buffer1[0]);
	*ptr = '-';
	ptr++;

	for (int i = 7; i <= 12; i++){
		if (joy2Btn(i)){
			sprintf(ptr,"%2d", i);
	  }else {
			sprintf(ptr,"  ");
	  }
		ptr += 2;
	}
		nxtDisplayTextLine(6, buffer1);

		sprintf(buffer1, "%d", joystick.joy2_TopHat);
		nxtDisplayTextLine(7, buffer1);
}

task main()
{
  initializeRobot();
  waitForStart();
  nxtDisplayTextLine(1, "Checking buttons...");
  while (true)
  {
	 getJoystickSettings(joystick);
	 checkButtons1(joystick);
	 checkButtons2(joystick);
  }
}
