#include <Arduino.h>

//
// Definitions of the Arduino pins from the test fixture
//
#define AUTOCONNECT_PIN	12	// Green from fixture
#define BAUD9600_PIN	10	// Blue
#define RESET_PIN	11	// Brown/White
#define CONNECTED_PIN	13	// Orange/White

//
// Debug pin for debugging this program itself
//
#define DEBUG_PIN	3

//
// RN42 GPIOs for each of these signals
//	Note that RESET isn't a GPIO and that it is inverted
//
#define AUTOCONNECT_GPIO	6	// INPUT to RN42 - puts RN42 into autoconnect mode
#define BAUD9600_GPIO		7	// INPUT to RN42 - puts RN42 into 9600 baud mode
#define CONNECTED_GPIO		2	// OUTPUT from RN42 - indicates connection status

//
// The Arduino control pin that will flip the serial port as needed
//
#define SERIAL_CONTROL_PIN	2	// HIGH selects computer, LOW RN42 - there is an external pull-up on this

//
// Baud rates for talking to the computer, and to the RN42
//
#define COMPUTER_BAUD_RATE	38400
#define RN42_INITIAL_BAUD_RATE	115200
#define RN42_CONTROL_BAUD_RATE	9600
#define RN42_OP_BAUD_RATE	38400

void pinInitialization()
{
     // debug pin

     pinMode(DEBUG_PIN,OUTPUT);
     digitalWrite(DEBUG_PIN,LOW);

     // set-up the input pins - these will be wiggled by specifically modifying GPIO values
     // note that they are floating at first...no pull-ups

     pinMode(AUTOCONNECT_PIN,INPUT);
     pinMode(CONNECTED_PIN,INPUT);

     // ensure that we are NOT in 9600 baud mode intially

     pinMode(BAUD9600_PIN,OUTPUT);
     digitalWrite(BAUD9600_PIN,LOW);

     // set the RESET pin to OUTPUT, and keep put the RN42 out of RESET (high)

     pinMode(RESET_PIN,OUTPUT);
     digitalWrite(RESET_PIN,HIGH);

     // note that we assume that the TX/RX pins are handled appropriately already
     // but start driving the TX/RX switch to high - even though it has a discreet pull-up

     pinMode(SERIAL_CONTROL_PIN,OUTPUT);
     digitalWrite(SERIAL_CONTROL_PIN,HIGH);
}

void serialFlush()
{
     while(Serial.available()) {
	  Serial.read();
     }
}

void serialComputer()
{
     digitalWrite(SERIAL_CONTROL_PIN,HIGH);
     Serial.begin(COMPUTER_BAUD_RATE);
     delay(100);
     serialFlush();
}

void serialRN42(unsigned long baud)
{
     digitalWrite(SERIAL_CONTROL_PIN,LOW);
     Serial.begin(baud);
     delay(100);
     serialFlush();
}

void waitForReturn()
{
     int	c;

     while(1) {
	  if(Serial.available()) {
	       c = Serial.read();
	       if(c == '\r' || c == '\n') {
		    break;
	       }
	  }
     }

     serialFlush();
}

void pressDownBoardMessage()
{
     Serial.println("");
     Serial.println("*** Please press down on the circuit board ***");
     Serial.println("***      You have 5 seconds to comply      ***");
     Serial.println("");
}

void countDown()
{
     Serial.print("5...");
     delay(1000);
     Serial.print("4...");
     delay(1000);
     Serial.print("3...");
     delay(1000);
     Serial.print("2...");
     delay(1000);
     Serial.print("1...");
     delay(1000);
     Serial.println("GO!");
}

void welcomeMessage()
{
     Serial.println(F(",----------------------------------------------,"));
     Serial.println(F("|                 RN42 TESTER                  |"));
     Serial.println(F("|                                              |"));
     Serial.println(F("| This program will test a circuit board with  |"));
     Serial.println(F("|     an RN42 (only) soldered down to it.      |"));
     Serial.println(F("|                                              |"));
     Serial.println(F("|    *** USE ONLY 3.3V PROGRAMMER !!! ***      |"));
     Serial.println(F("|                                              |"));
     Serial.println(F("|   Please insert a circuit board, and press   |"));
     Serial.println(F("|          ENTER/RETURN to get going.          |"));
     Serial.println(F("'----------------------------------------------'"));
}

void endMessage()
{
     Serial.println(F(""));
     Serial.println(F(",----------------------------------------------,"));
     Serial.println(F("|        RN42 TESTER - test concluded          |"));
     Serial.println(F(",----------------------------------------------,"));
}

void goodTestMessage()
{
     Serial.println(F(""));
     Serial.println(F("**** ALL TESTS PASSED ****"));
}

void badTestMessage()
{
     Serial.println(F(""));
     Serial.println(F("**** ONE OR MORE TESTS FAILED ****"));
}

void initialTestFailureMessage()
{
     Serial.println(F("FAILURE"));
     Serial.println(F(""));
     Serial.println(F("    The initial connection test failed. This"));
     Serial.println(F("    means that one of the following is wrong:"));
     Serial.println(F("    1. You forgot to put an RN42 board in the fixture!"));
     Serial.println(F("    2. RN42 isn't receiving power (pins 11/12)"));
     Serial.println(F("    3. RN42 TX/RX not connected or shorted (pins 13/14)"));
     Serial.println(F("    4. The RESET line is stuck low"));
     Serial.println(F("    5. Wrong version of RN42 firmware"));
     Serial.println(F(""));
     Serial.println(F("TESTING HALTED"));
     Serial.println(F(""));
}

void resetTestFailureMessage()
{
     // this error message assumes that the initialTest() passed, so those
     // things may not be wrong

     Serial.println(F("FAILURE"));
     Serial.println(F(""));
     Serial.println(F("    The RN42 would not reset."));
     Serial.println(F("    One of the following is wrong:"));
     Serial.println(F("    1. RN42 RESET pin (5) isn't connected or shorted to VCC"));
     Serial.println(F("    2. Wrong version of RN42 firmware"));
     Serial.println(F(""));
     Serial.println(F("TESTING HALTED"));
     Serial.println(F(""));
}
void baudSwitchTestFailureMessage()
{
     // this error message assumes that the initialTest() passed, so those
     // things may not be wrong

     Serial.println(F("FAILURE"));
     Serial.println(F(""));
     Serial.println(F("    The RN42 would not switch into 9600-baud mode."));
     Serial.println(F("    One of the following is wrong:"));
     Serial.println(F("    1. RN42 9600 Baud pin (4) isn't connected or shorted to GND"));
     Serial.println(F("    2. Wrong version of RN42 firmware"));
     Serial.println(F(""));
     Serial.println(F("TESTING HALTED"));
     Serial.println(F(""));
}

void getVersionTestFailureMessage()
{
     // this error message assumes that the initialTest() passed, so those
     // things may not be wrong

     Serial.println(F("FAILURE"));
     Serial.println(F(""));
     Serial.println(F("    I really don't know why this would fail since everything"));
     Serial.println(F("    seemed to work OK up to this point.  My only theory:"));
     Serial.println(F("    1. Wrong version of RN42 firmware"));
     Serial.println(F(""));
     Serial.println(F("TESTING HALTED"));
     Serial.println(F(""));
}

void autoConnectPinTestFailureMessage()
{
     // this error message assumes that all other tests have passed, so we're
     // really only looking at this pin - which means we are NOT wrong firmware
     
     Serial.println(F("FAILURE"));
     Serial.println(F(""));
     Serial.println(F("    The RN42 AutoConnect pin isn't responding."));
     Serial.println(F("    The following is probably wrong:"));
     Serial.println(F("    1. RN42 AutoConnect pin (3) isn't connected or is shorted"));
     Serial.println(F(""));
     Serial.println(F("Testing continues..."));
     Serial.println(F(""));
}

void connectedPinTestFailureMessage()
{
     // this error message assumes that all other tests have passed, so we're
     // really only looking at this pin - which means we are NOT wrong firmware

     Serial.println(F("FAILURE"));
     Serial.println(F(""));
     Serial.println(F("    The RN42 Connected pin isn't responding."));
     Serial.println(F("    The following is probably wrong:"));
     Serial.println(F("    1. RN42 Connected pin (19) isn't connected or is shorted"));
     Serial.println(F(""));
}

//
// baudSwitchTest() - can we switch into 9600 baud?  Note that
//			we leave the RN42 in 9600 mode after a
//			successful test.
//
int baudSwitchTest()
{
     int	results = false;

     serialRN42(9600);

     digitalWrite(BAUD9600_PIN,HIGH);

     RN42Reset();

     Serial.write("$$$");
     delay(100);
     Serial.write("+\r");
     delay(100);
     serialFlush();
     delay(100);
     Serial.write("+");
     delay(100);

     if(Serial.available() && Serial.read() == '+') {
	  results = true;
     }

// Leave it in 9600 baud mode - so don't do:
//     digitalWrite(BAUD9600_PIN,LOW);

     serialComputer();

     return(results);
}


//
// initialTest() - do the initial "are you there" test.
//
int initialTest()
{
     int	results = false;

     serialRN42(115200);

     Serial.write("$$$");
     delay(100);
     Serial.write("+\r");
     delay(100);
     serialFlush();
     delay(100);
     Serial.write("+");
     delay(100);

     if(Serial.available() && Serial.read() == '+') {
	  results = true;
     }

     serialComputer();

     return(results);
}

//
// gpioPinTest() - check to see if we have a behaving GPIO pin.
// 		   This is done by wiggling the given GPIO line and
//		   listening for the results.
//		   NOTE that we assume that we're in command mode.
//		   Doesn't really matter what baud, because we simply
//		   jam out the commands.
//		   NOTE that the GPIO pin is left in a funky state
//		   after this test, but who cares?
//
int gpioPinTest(int gpioPin, int arduinoPin)
{
     int	GPIOPos;
     char	*GPIOHex;
     char	GPIO_Output_Command[9];
     char	GPIO_High_Command[9];
     char	GPIO_Low_Command[9];

     GPIOPos = 1 << gpioPin;

     sprintf(GPIO_Output_Command,"S@,%02x%02x\r",GPIOPos,GPIOPos);// compose the command to set the GPIO pin as an output pin
     sprintf(GPIO_High_Command,"S&,%02x%02x\r",GPIOPos,GPIOPos);	// compose the command to drive the pin high
     sprintf(GPIO_Low_Command,"S&,%02x00\r",GPIOPos);		// compose the command to drive the pin low

     Serial.write(GPIO_Output_Command);	// get ready to wiggle the lines
     delay(100);

     Serial.write(GPIO_Low_Command);		// put the GPIO pin in low mode
     delay(100);
     if(digitalRead(arduinoPin) == LOW) {
	  Serial.write(GPIO_High_Command);	// put the GPIO pin in high mode
	  delay(100);
	  if(digitalRead(arduinoPin) == HIGH) {
	       Serial.write(GPIO_Low_Command);	// back to low
	       delay(100);
	       if(digitalRead(arduinoPin) == LOW) {
		    return(true);
	       }
	  }
     }

     return(false);
     
}

//
// autoConnectPinTest() - check to see if we have a behaving AutoConnect/Connected pin.
// connectedPinTest()	  the gpioPinTest() routine does this for us.  See that for
//			  important information!
//
int autoConnectPinTest()
{
     int	results = false;

     serialRN42(9600);		// assumes we're still in 9600 baud command mode
     results = gpioPinTest(AUTOCONNECT_GPIO,AUTOCONNECT_PIN);
     serialComputer();
     return(results);
}

 int connectedPinTest()
{
     int	results = false;

     serialRN42(9600);		// assumes we're still in 9600 baud command mode
     results = gpioPinTest(CONNECTED_GPIO,CONNECTED_PIN);
     serialComputer();

     return(results);
}

//
// getRN42Version() - get the version string from the RN42.  Note that this
//			only works when in 9600 baud mode (can't reliably
//			keep up with 115200 unfortunately).  We also assume
//			that we're in command mode.
//
int getRN42Version(char *buffer,int size, unsigned long timeout)
{
     unsigned long	target;
     int		results = false;

     target = millis() + timeout;

     serialRN42(9600);

     // first we need to turn off echo
     Serial.write("+\r");
     delay(100);
     serialFlush();

     // then try for the version

     Serial.write("ver\r");

     while(millis() < target && size) {
	  if(Serial.available()) {
	       *buffer = Serial.read();
	       if(*buffer == '\r' || *buffer == '\n') {
		    *buffer = '\0';
		    results = true;
		    break;
	       } else {
		    size--;
		    buffer++;
	       }
	  }
     }

     serialComputer();

     return(results);
}

//
// RN42Reset() - need some special timing for doing an RN42 reset
//
void RN42Reset()
{
     digitalWrite(RESET_PIN,LOW);
     delay(100);
     digitalWrite(RESET_PIN,HIGH);
     delay(1000);
}

//
// resetTest() - do a reset, then try something.
//		 This test assumes that we are still in command mode
//		 from the initialTest().  So we do something that
//		 shouldn't work UNLESS WE RESET.  In this case,
//		 it is a simple repeat of the initial test.
//
int resetTest()
{
     RN42Reset();
     return(initialTest());
}

void testTitleOutput(char *title)
{
     Serial.print("TEST:  ");
     Serial.print(title);
     Serial.print("...");
     delay(100);
}

void testPassedOutput()
{
     Serial.println("PASSED.");
     delay(100);
}

void testSkippedOutput()
{
     Serial.println("SKIPPED.");
     delay(100);
}

void setup()
{
     int	results = true;
     int	gpioResults = true;
     char	buffer[100];

     pinInitialization();

     serialComputer();	// flip to computer interface
     welcomeMessage();
     waitForReturn();
     pressDownBoardMessage();
     countDown();

     testTitleOutput("Initial Connection");

     if(initialTest()) {
	  testPassedOutput();
     } else {
	  initialTestFailureMessage();
	  results = false;
     }

     // note that the RN42 is left in command mode from the initialTest()
     //  ... so long as it works anyway

     // So try the resetTest()

     testTitleOutput("Reset");

     if(!results) {
	  testSkippedOutput();
     } else {
	  if(resetTest()) {
	       testPassedOutput();
	  } else {
	       resetTestFailureMessage();
	       results = false;
	  }
     }

     testTitleOutput("RN42 9600 Baud");

     if(!results) {
	  testSkippedOutput();
     } else {
	  if(baudSwitchTest()) {
	       testPassedOutput();
	  } else {
	       baudSwitchTestFailureMessage();
	       results = false;
	  }
     }

     testTitleOutput("Retrieve RN42 Version");

     if(!results) {
	  testSkippedOutput();
     } else {
	  if(getRN42Version(buffer,sizeof(buffer),1000)) {
	       testPassedOutput();
	       Serial.print("    Version: \"");
	       Serial.print(buffer);
	       Serial.println("\"");
	  } else {
	       getVersionTestFailureMessage();
	       results = false;
	  }
     }

     // we're still in command mode on the RN42, so let's try testing other pins

     testTitleOutput("\"Connected\" interface pin");

     if(!results) {
	  testSkippedOutput();
     } else {
	  if(connectedPinTest()) {
	       testPassedOutput();
	  } else {
	       connectedPinTestFailureMessage();
	       gpioResults = false;
	  }
     }

     testTitleOutput("\"AutoConnect\" interface pin");

     if(!results) {
	  testSkippedOutput();
     } else {
	  if(autoConnectPinTest()) {
	       testPassedOutput();
	  } else {
	       autoConnectPinTestFailureMessage();
	       gpioResults = false;
	  }
     }

     results = results && gpioResults;

     if(results) {
	  goodTestMessage();
     } else {
	  badTestMessage();
     }

     endMessage();

     delay(200);

     // restart! (cheap and easy way...)
     asm volatile ("  jmp 0");  
}

void loop()
{
     // nothing in here for this program, it all happens in setup
}
