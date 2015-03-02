//
// power.h
//	Routines and defines that help control power consumption.
//

// the PRR register is used to turn off/on modules in the Arduino
// these defines implement:
//
//		Bit 7 - PRTWI: Power Reduction TWI
//		Bit 6 - PRTIM2: Power Reduction Timer/Counter2 (used by tone())
//		Bit 5 - PRTIM0: Power Reduction Timer/Counter0 (used by delay() and millis()
//		Bit 4 - Res: Reserved bit
//		Bit 3 - PRTIM1: Power Reduction Timer/Counter1 (used by servo library)
//		Bit 2 - PRSPI: Power Reduction Serial Peripheral Interface
//		Bit 1 - PRUSART0: Power Reduction USART0
//		Bit 0 - PRADC: Power Reduction ADC
//

#define power_adc_enable()      (PRR &= (uint8_t)~(1 << PRADC))
#define power_adc_disable()     (PRR |= (uint8_t)(1 << PRADC))
 
#define power_spi_enable()      (PRR &= (uint8_t)~(1 << PRSPI))
#define power_spi_disable()     (PRR |= (uint8_t)(1 << PRSPI))
 
#define power_usart0_enable()   (PRR &= (uint8_t)~(1 << PRUSART0))
#define power_usart0_disable()  (PRR |= (uint8_t)(1 << PRUSART0))
 
#define power_timer0_enable()   (PRR &= (uint8_t)~(1 << PRTIM0))
#define power_timer0_disable()  (PRR |= (uint8_t)(1 << PRTIM0))
 
#define power_timer1_enable()   (PRR &= (uint8_t)~(1 << PRTIM1))
#define power_timer1_disable()  (PRR |= (uint8_t)(1 << PRTIM1))
 
#define power_timer2_enable()   (PRR &= (uint8_t)~(1 << PRTIM2))
#define power_timer2_disable()  (PRR |= (uint8_t)(1 << PRTIM2))
 
#define power_twi_enable()      (PRR &= (uint8_t)~(1 << PRTWI))
#define power_twi_disable()     (PRR |= (uint8_t)(1 << PRTWI))
 
#define power_all_enable()      (PRR &= (uint8_t)~((1<<PRADC)|(1<<PRSPI)|(1<<PRUSART0)|(1<<PRTIM0)|(1<<PRTIM1)|(1<<PRTIM2)|(1<<PRTWI)))
#define power_all_disable()     (PRR |= (uint8_t)((1<<PRADC)|(1<<PRSPI)|(1<<PRUSART0)|(1<<PRTIM0)|(1<<PRTIM1)|(1<<PRTIM2)|(1<<PRTWI)))

extern void lowPowerOperation();
extern void tonePowerOn();
extern void tonePowerOff();
extern void powerDown();
