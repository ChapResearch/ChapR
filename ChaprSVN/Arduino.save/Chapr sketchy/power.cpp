#include <arduino.h>
#include "power.h"

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


