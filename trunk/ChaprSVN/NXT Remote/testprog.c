#pragma config(Hubs,  S1, HTMotor,  HTServo,  none,     none)
#pragma config(Motor,  mtr_S1_C1_1,     ,              tmotorNone, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     ,              tmotorNone, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.

task main()
{
  //initializeRobot();

  waitForStart();   // wait for start of tele-op phase

  while (true)
  {
    getJoystickSettings(joystick);

    if( joy1Btn(5) ) {
      nxtDisplayBigStringAt(0,32,"1 ");
    } else {
      nxtDisplayBigStringAt(0,32,"no");
    }

    wait1Msec(100);
  }
}
