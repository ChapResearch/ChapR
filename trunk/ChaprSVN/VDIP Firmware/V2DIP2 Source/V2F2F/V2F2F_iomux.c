/*
** V2F2F_iomux.c
**
** C Source File
**
** Part of solution FileCopier in project V2F2F
*/
#include "vos.h";

void iomux_setup(void)
{
	unsigned char packageType;

	packageType = vos_get_package_type();

	if (packageType == VINCULUM_II_32_PIN)
	{
		// Debugger to pin 11 as Bi-Directional.
		vos_iomux_define_bidi(199, IOMUX_IN_DEBUGGER, IOMUX_OUT_DEBUGGER);
		// UART_TXD to pin 23 as Output.
		vos_iomux_define_output(23, IOMUX_OUT_UART_TXD);
		// UART_RXD to pin 24 as Input.
		vos_iomux_define_input(24, IOMUX_IN_UART_RXD);
		// UART_RTS_N to pin 25 as Output.
		vos_iomux_define_output(25, IOMUX_OUT_UART_RTS_N);
		// UART_CTS_N to pin 26 as Input.
		vos_iomux_define_input(26, IOMUX_IN_UART_CTS_N);
	}

	if (packageType == VINCULUM_II_48_PIN)
	{
		// Debugger to pin 11 as Bi-Directional.
		vos_iomux_define_bidi(199, IOMUX_IN_DEBUGGER, IOMUX_OUT_DEBUGGER);
		// BD0 -> Pin 11
		vos_iomux_define_bidi(11, IOMUX_IN_GPIO_PORT_A_0, IOMUX_OUT_GPIO_PORT_A_0);
		// BD1 -> Pin 12
		vos_iomux_define_bidi(12, IOMUX_IN_GPIO_PORT_A_1, IOMUX_OUT_GPIO_PORT_A_1);
		// BD2 -> Pin 13
		vos_iomux_define_bidi(13, IOMUX_IN_GPIO_PORT_A_2, IOMUX_OUT_GPIO_PORT_A_2);
		// BD3 -> Pin 14
		vos_iomux_define_bidi(14, IOMUX_IN_GPIO_PORT_A_3, IOMUX_OUT_GPIO_PORT_A_3);
		// BD4 -> Pin 15
		vos_iomux_define_bidi(15, IOMUX_IN_GPIO_PORT_A_4, IOMUX_OUT_GPIO_PORT_A_4);
		// BD5 -> Pin 16
		vos_iomux_define_bidi(16, IOMUX_IN_GPIO_PORT_A_5, IOMUX_OUT_GPIO_PORT_A_5);
		// BD6 -> Pin 18
		vos_iomux_define_bidi(18, IOMUX_IN_GPIO_PORT_A_6, IOMUX_OUT_GPIO_PORT_A_6);
		// BD7 -> Pin 19
		vos_iomux_define_bidi(19, IOMUX_IN_GPIO_PORT_A_7, IOMUX_OUT_GPIO_PORT_A_7);
		// UART_TXD to pin 31 as Output.
		vos_iomux_define_output(31, IOMUX_OUT_UART_TXD);
		// UART_RXD to pin 32 as Input.
		vos_iomux_define_input(32, IOMUX_IN_UART_RXD);
		// UART_RTS_N to pin 33 as Output.
		vos_iomux_define_output(33, IOMUX_OUT_UART_RTS_N);
		// UART_CTS_N to pin 34 as Input.
		vos_iomux_define_input(34, IOMUX_IN_UART_CTS_N);
		// UART_DTR_N to pin 35 as Output.
		vos_iomux_define_output(35, IOMUX_OUT_UART_DTR_N);
		// UART_DSR_N to pin 36 as Input.
		vos_iomux_define_input(36, IOMUX_IN_UART_DSR_N);
		// UART_DCD to pin 37 as Input.
		vos_iomux_define_input(37, IOMUX_IN_UART_DCD);
		// UART_RI to pin 38 as Input.
		vos_iomux_define_input(38, IOMUX_IN_UART_RI);
		// UART_TX_Active to pin 41 as Output.
		vos_iomux_define_output(41, IOMUX_OUT_UART_TX_ACTIVE);
		// GPIO_Port_A_5 to pin 42 as Input.
		vos_iomux_define_input(42, IOMUX_IN_GPIO_PORT_A_5);
		// GPIO_Port_A_6 to pin 43 as Input.
		vos_iomux_define_input(43, IOMUX_IN_GPIO_PORT_A_6);
		// UART_CTS_N to pin 44 as Input.
		vos_iomux_define_input(44, IOMUX_IN_UART_CTS_N);
	}

	if (packageType == VINCULUM_II_64_PIN)
	{
		// Debugger to pin 11 as Bi-Directional.
		vos_iomux_define_bidi(199, IOMUX_IN_DEBUGGER, IOMUX_OUT_DEBUGGER);
		// BD1 -> Pin 12 -> V2EVAL Board LED 3
		vos_iomux_define_output(12, IOMUX_OUT_GPIO_PORT_A_1);
		// BD2 -> Pin 13 -> V2EVAL Board LED 4
		vos_iomux_define_output(13, IOMUX_OUT_GPIO_PORT_A_2);
		// BD5 -> Pin 29 -> V2EVAL Board LED 5
		vos_iomux_define_output(29, IOMUX_OUT_GPIO_PORT_A_5);
		// BD6 -> Pin 31 -> V2EVAL Board LED 6
		vos_iomux_define_output(31, IOMUX_OUT_GPIO_PORT_A_6);
		// UART_DSR_N to pin 29 as Input.
		vos_iomux_define_input(29, IOMUX_IN_UART_DSR_N);
		// UART_DCD to pin 31 as Input.
		vos_iomux_define_input(31, IOMUX_IN_UART_DCD);
		// UART_RI to pin 32 as Input.
		vos_iomux_define_input(32, IOMUX_IN_UART_RI);
		// UART_TXD to pin 39 as Output.
		vos_iomux_define_output(39, IOMUX_OUT_UART_TXD);
		// UART_RXD to pin 40 as Input.
		vos_iomux_define_input(40, IOMUX_IN_UART_RXD);
		// UART_RTS_N to pin 41 as Output.
		vos_iomux_define_output(41, IOMUX_OUT_UART_RTS_N);
		// UART_CTS_N to pin 42 as Input.
		vos_iomux_define_input(42, IOMUX_IN_UART_CTS_N);
		// UART_DTR_N to pin 43 as Output.
		vos_iomux_define_output(43, IOMUX_OUT_UART_DTR_N);
		// UART_DSR_N to pin 44 as Input.
		vos_iomux_define_input(44, IOMUX_IN_UART_DSR_N);
		// UART_DCD to pin 45 as Input.
		vos_iomux_define_input(45, IOMUX_IN_UART_DCD);
		// UART_RI to pin 46 as Input.
		vos_iomux_define_input(46, IOMUX_IN_UART_RI);
		// UART_TX_Active to pin 47 as Output.
		vos_iomux_define_output(47, IOMUX_OUT_UART_TX_ACTIVE);
	}
}

