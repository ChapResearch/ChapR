#include <Arduino.h>
#include "config.h"
#include "power.h"
#include "blinky.h"
#include "sound.h"

//
// lowPowerOperation() - configures the arduino to go into the lowest power mode available
//			given the ChapR requirements
//
void lowPowerOperation()
{
     power_all_enable();	// turn everything on at first

     ADCSRA &= ~(1<<ADEN);	//Disable ADC  
     ACSR = (1<<ACD);		//Disable the analog comparator

     power_adc_disable();	// ADC disabled
     power_twi_disable();	// IC2/TWI disabled
     power_spi_disable();	// SPI disabled
//     power_timer0_disable();	// affects delay/milli
     power_timer1_disable();	// affects servos
     power_timer2_disable();	// affects tones (this is turned on/off in sound routines too)
     power_usart0_disable();	// disables communication via onboard USART (turned on in pairing mode)

  // at this point, we've left the USART on as well as timer 0 (for delay/millis)
}

void tonePowerOn()
{
     power_timer2_enable();
}

void tonePowerOff()
{
     power_timer2_disable();
}

//
// powerDown() - power down the ChapR.  Note that beeps first!  Note, too, that this routine
//		 never returns.  BTW - the reason that it turns off the LEDs is that when
//		 programming/debugging the ChapR the programmer supplies power, so lowering
//		 the POWER_ON_HOLD line doesn't actually turn off the ChapR.
//
//	BIG NOTE - this routine never returns!
//
void powerDown()
{
     extern blinky powerLED;
     extern blinky indicateLED;
     extern sound beeper;

     powerLED.off();
     indicateLED.off();
     beeper.yawn();
     digitalWrite(POWER_ON_HOLD,LOW);
     exit(0);
}


