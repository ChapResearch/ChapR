.DATA

.WEAK	"%eax"
.WEAK	"%ebx"
.WEAK	"%ecx"
.WEAK	"%r0"
.WEAK	"%r1"
.WEAK	"%r2"
.WEAK	"%r3"
deviceList	.DB	112	?
.GLOBAL	  DO_NOT_EXPORT "deviceList"
curDevice	.DB	1	?
.GLOBAL	  DO_NOT_EXPORT "curDevice"
.WEAK	"deviceList"
.WEAK	"cmdDataIndex"
.WEAK	"cmdTerminator"
.WEAK	"hFT232"
.WEAK	"hUsb1"
.WEAK	"hUsb2"
.WEAK	"hBoms"
.WEAK	"param1"
.WEAK	"param2"
.WEAK	"cmdData"
.WEAK	"hMonitor"
.WEAK	"Mon_Flags"
.WEAK	"curDevice"
.WEAK	"param1Data"
.WEAK	"param2Data"




.TEXT


.WEAK	"vos_dma_get_fifo_flow_control"

.WEAK	"vos_start_scheduler"

.WEAK	"monInvalid"

.WEAK	"monAddMultibyteNumberToConsole"

.WEAK	"vos_gpio_write_port"

.WEAK	"vos_signal_semaphore_from_isr"

.WEAK	"vos_malloc"

.WEAK	"monVersion"

.WEAK	"vos_create_thread_ex"

.WEAK	"vos_memcpy"

.WEAK	"vos_memset"

.WEAK	"vos_get_kernel_clock"

.WEAK	"vos_gpio_disable_int"

.WEAK	"vos_get_package_type"

.WEAK	"vos_dma_get_fifo_data_register"

.WEAK	"monDataMode"

.WEAK	"monAddNumberToConsole"

.WEAK	"vos_signal_semaphore"

.WEAK	"monFileOpen"

.WEAK	"vos_gpio_wait_on_int"

.WEAK	"monReadOnly"

.WEAK	"monDiskFull"

.WEAK	"vos_dma_get_fifo_data"

.WEAK	"monASCIItoDec"

.WEAK	"vos_iocell_get_config"

.WEAK	"vos_iomux_define_bidi"

.WEAK	"vos_gpio_set_all_mode"

.WEAK	"vos_iocell_set_config"

.WEAK	"vos_gpio_set_pin_mode"

.WEAK	"monLedsGreen"

.WEAK	"vos_get_chip_revision"

.WEAK	"vos_wait_semaphore_ex"

.WEAK	"monNoUpgrade"

.WEAK	"vos_enable_interrupts"

.WEAK	"vos_dev_read"

.WEAK	"vos_dev_open"

.WEAK	"vos_halt_cpu"

.WEAK	"vos_dev_init"

.WEAK	"monBadCommand"

.WEAK	"vos_dma_get_fifo_count"

.WEAK	"usbhost_init"

.WEAK	"monLedsOffNow"

.WEAK	"vos_reset_kernel_clock"

.WEAK	"monReadNumber"

.WEAK	"vos_gpio_set_port_mode"

.WEAK	"vos_iomux_define_input"

.WEAK	"vos_disable_interrupts"

.WEAK	"vos_get_idle_thread_tcb"

.WEAK	"vos_dma_reset"

.WEAK	"vos_dev_close"

.WEAK	"vos_wdt_clear"

.WEAK	"vos_heap_size"

.WEAK	"vos_dev_ioctl"

.WEAK	"vos_dev_write"

.WEAK	"vos_get_clock_frequency"

.WEAK	"vos_set_clock_frequency"

.WEAK	"monDirNotEmpty"

.WEAK	"vos_dma_enable"

.WEAK	"vos_reset_vnc2"

.WEAK	"vos_heap_space"

.WEAK	"vos_iomux_define_output"

.WEAK	"monReadFileName"

.WEAK	"monWriteString"

.WEAK	"vos_wdt_enable"

.WEAK	"monSlaveEnabled"

.WEAK	"vos_dma_wait_on_complete"

.WEAK	"vos_lock_mutex"

.WEAK	"vos_power_down"

.WEAK	"vos_init_mutex"

.WEAK	"vos_gpio_wait_on_any_int"

.WEAK	"vos_get_priority_ceiling"

.WEAK	"vos_dma_disable"

.WEAK	"vos_set_priority_ceiling"

.WEAK	"vos_dma_release"

.WEAK	"vos_iomux_disable_output"

.WEAK	"vos_dma_acquire"

.WEAK	"vos_delay_msecs"

.WEAK	"vos_stack_usage"

.WEAK	"vos_get_profile"

.WEAK	"monCommandFailed"

.WEAK	"monWriteFileName"

.WEAK	"monSlaveDisabled"

.WEAK	"vos_gpio_wait_on_all_ints"

.WEAK	"monDeviceRemoved"

.WEAK	"vos_delay_cancel"

.WEAK	"monLedsGreenFlash"

.WEAK	"vos_dma_retained_configure"

.WEAK	"monDeviceDetected"

.WEAK	"monRead"

.WEAK	"vos_unlock_mutex"

.WEAK	"monValidateNumber"

.WEAK	"monInit"

.WEAK	"vos_gpio_read_all"

.WEAK	"vos_create_thread"

.WEAK	"vos_gpio_read_pin"

.WEAK	"vos_dma_configure"

.WEAK	"vos_init_cond_var"

.WEAK	"vos_wait_cond_var"

.WEAK	"monCheck"

.WEAK	"monFilenameInvalid"

.WEAK	"vos_stop_profiler"

.WEAK	"vos_trylock_mutex"

.WEAK	"monWrite"

.WEAK	"vos_free"

.WEAK	"vos_init"

.WEAK	"monValidateFileName"

.WEAK	"monReadCr"

.WEAK	"monValidateDateTime"

.WEAK	"vos_gpio_read_port"

.WEAK	"vos_gpio_write_all"

.WEAK	"vos_set_idle_thread_tcb_size"

.WEAK	"vos_init_semaphore"

.WEAK	"vos_wait_semaphore"

.WEAK	"monNoDisk"

.WEAK	"vos_gpio_write_pin"

.WEAK	"vos_start_profiler"

.WEAK	"monOnline"

.WEAK	"monPrompt"

.WEAK	"uart_init"

.WEAK	"vos_gpio_enable_int"

.WEAK	"monReadCmd"

.WEAK	"vos_signal_cond_var"

.WEAK	"monLedsOff"

.WEAK	"monLedsRed"

hostConnected:	
.GLOBAL	 DO_NOT_EXPORT  "hostConnected"

.FUNCTION	"hostConnected"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_DEC	$12
SP_RD16	%r0	$32
SP_STORE	%r1
CPY16	%r2	%r1
LD8	(%r2)	$16
LD16	%r2	$6
ADD16	%r2	%r1
SP_STORE	%r3
INC16	%r3	$10
CPY16	(%r2)	%r3
PUSH16	%r1
PUSH16	%r0
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$15
SP_INC	$4
SP_RD8	%eax	$10
SP_WR8	%eax	$31
SP_INC	$12
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"hostConnected"

hostAddToDeviceList:	
.GLOBAL	 DO_NOT_EXPORT  "hostAddToDeviceList"

.FUNCTION	"hostAddToDeviceList"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_DEC	$16
SP_RD16	%ecx	$36
CMP8	curDevice	$83
JNZ	@IC1
@IC2:	
@IC1:	
LD32	%ecx	$0
SP_WR32	%ecx	$0
SP_STORE	%r1
INC16	%r1	$4
CPY16	%r2	%r1
LD8	(%r2)	$33
INC16	%r1	$6
SP_STORE	%r2
CPY16	(%r1)	%r2
@IC5:	
SP_STORE	%r1
INC16	%r1	$4
LD16	%r2	$2
ADD16	%r2	%r1
CPY16	%r2	%r2
SP_STORE	%eax
INC16	%eax	$0
CPY32	(%r2)	(%eax)
PUSH16	%r1
SP_RD16	%eax	$38
PUSH16	%eax
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$18
SP_INC	$4
SP_RD8	%ecx	$14
CMP8	%ecx	$0
JZ	@IC8
@IC9:	
JUMP	@IC7
@IC8:	
SP_STORE	%ecx
CMP32	(%ecx)	$0
JZ	@IC12
@IC13:	
LD8	%r0	$0
@IC14:	
CMP8	%r0	$16
JGE	@IC15
@IC16:	
CPY8	%r1	%r0
AND32	%r1	$255
MUL32	%r1	$7
LD16	%r2	$deviceList
ADD16	%r1	%r2	%r1
CPY32	%r1	(%r1)
CMP32	%r1	$0
JNZ	@IC20
@IC21:	
CPY8	%r1	%r0
AND32	%r1	$255
MUL32	%r1	$7
LD16	%r2	$deviceList
ADD16	%r3	%r2	%r1
CPY16	%r3	%r3
SP_STORE	%eax
CPY32	(%r3)	(%eax)
CPY16	%r3	%r2
ADD16	%r3	%r1
INC16	%r3	$4
SP_STORE	%eax
INC16	%eax	$36
CPY16	(%r3)	(%eax)
CPY16	%r2	%r2
ADD16	%r1	%r2	%r1
INC16	%r1	$6
SP_RD32	%eax	$0
PUSH32	%eax
SP_RD16	%eax	$40
PUSH16	%eax
SP_DEC	$1
CALL	hostGetDevType
POP8	%eax
SP_WR8	%eax	$21
SP_INC	$6
SP_STORE	%eax
INC16	%eax	$15
CPY8	(%r1)	(%eax)
JUMP	@IC15
@IC20:	
@IC17:	
INC8	%r0	$1
JUMP	@IC14
@IC15:	
@IC12:	
@IC6:	
SP_RD32	%ecx	$0
CMP32	%ecx	$0
JNZ	@IC5
JZ	@IC7
@IC7:	
SP_STORE	%eax
INC16	%eax	$35
CPY8	(%eax)	curDevice
SP_INC	$16
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"hostAddToDeviceList"

hostRemoveFromDeviceList:	
.GLOBAL	 DO_NOT_EXPORT  "hostRemoveFromDeviceList"

.FUNCTION	"hostRemoveFromDeviceList"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_RD16	%ecx	$20
LD8	%r0	$0
@IC26:	
CMP8	%r0	$16
JGE	@IC27
@IC28:	
CPY8	%r1	%r0
AND32	%r1	$255
MUL32	%r1	$7
LD16	%r2	$deviceList
ADD16	%r1	%r2
CPY32	%r1	(%r1)
CMP32	%r1	$0
JZ	@IC32
@IC33:	
CPY8	%r1	%r0
AND32	%r1	$255
MUL32	%r1	$7
LD16	%r2	$deviceList
ADD16	%r1	%r2
INC16	%r1	$4
CPY16	%r1	(%r1)
SP_RD32	%ecx	$20
CMP16	%ecx	%r1
JNZ	@IC36
@IC37:	
CPY8	%r1	%r0
AND32	%r1	$255
MUL32	%r1	$7
LD16	%r2	$deviceList
ADD16	%r3	%r2	%r1
CPY16	%r3	%r3
LD32	(%r3)	$0
CPY16	%r3	%r2
ADD16	%r3	%r1
INC16	%r3	$4
LD16	(%r3)	$0
CPY16	%r2	%r2
ADD16	%r1	%r2	%r1
INC16	%r1	$6
LD8	(%r1)	$0
@IC36:	
@IC32:	
@IC29:	
INC8	%r0	$1
JUMP	@IC26
@IC27:	
LD8	%eax	$0
SP_WR8	%eax	$19
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"hostRemoveFromDeviceList"

hostSuspend:	
.GLOBAL	 DO_NOT_EXPORT  "hostSuspend"

.FUNCTION	"hostSuspend"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
SP_DEC	$11
SP_RD16	%r0	$27
SP_STORE	%r1
CPY16	%r2	%r1
LD8	(%r2)	$6
LD16	%r2	$1
ADD16	%r2	%r1
LD8	(%r2)	$0
LD16	%r2	$2
ADD16	%r2	%r1
CPY16	%r2	%r2
LD32	(%r2)	$0
PUSH16	%r1
PUSH16	%r0
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$14
SP_INC	$4
SP_RD8	%ecx	$10
CMP8	%ecx	$0
JNZ	@IC40
@IC41:	
LD8	%eax	$0
SP_WR8	%eax	$26
SP_INC	$11
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
@IC40:	
LD8	%eax	$1
SP_WR8	%eax	$26
SP_INC	$11
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"hostSuspend"

hostWake:	
.GLOBAL	 DO_NOT_EXPORT  "hostWake"

.FUNCTION	"hostWake"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
SP_DEC	$11
SP_RD16	%r0	$27
SP_STORE	%r1
CPY16	%r2	%r1
LD8	(%r2)	$7
LD16	%r2	$1
ADD16	%r2	%r1
LD8	(%r2)	$0
LD16	%r2	$2
ADD16	%r2	%r1
CPY16	%r2	%r2
LD32	(%r2)	$0
PUSH16	%r1
PUSH16	%r0
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$14
SP_INC	$4
SP_RD8	%ecx	$10
CMP8	%ecx	$0
JNZ	@IC44
@IC45:	
LD8	%eax	$0
SP_WR8	%eax	$26
SP_INC	$11
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
@IC44:	
LD8	%eax	$1
SP_WR8	%eax	$26
SP_INC	$11
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"hostWake"

hostGetDevType:	
.GLOBAL	 DO_NOT_EXPORT  "hostGetDevType"

.FUNCTION	"hostGetDevType"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_DEC	$20
SP_RD16	%ecx	$40
SP_RD32	%r0	$42
LD8	%ecx	$0
SP_WR8	%ecx	$0
SP_STORE	%r2
INC16	%r2	$1
LD16	%r3	$2
ADD16	%r3	%r2
CPY16	%r3	%r3
CPY32	(%r3)	%r0
CPY16	%r0	%r2
LD8	(%r0)	$36
LD16	%r0	$6
ADD16	%r0	%r2
SP_STORE	%r3
INC16	%r3	$11
CPY16	(%r0)	%r3
PUSH16	%r2
SP_RD16	%eax	$42
PUSH16	%eax
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$19
SP_INC	$4
CPY16	%r0	(%r3)
CMP16	%r0	$1027
JNZ	@IC49
@IC50:	
LD8	%ecx	$1
SP_WR8	%ecx	$0
JUMP	@IC48
@IC49:	
SP_STORE	%r0
INC16	%r0	$1
CPY16	%r2	%r0
LD8	(%r2)	$37
LD16	%r2	$6
ADD16	%r2	%r0
SP_STORE	%r3
INC16	%r3	$16
CPY16	(%r2)	%r3
PUSH16	%r0
SP_RD16	%eax	$42
PUSH16	%eax
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$23
SP_INC	$4
JUMP	@IC55
@IC54:	
LD8	%ecx	$16
SP_WR8	%ecx	$0
JUMP	@IC53
@IC56:	
LD8	%ecx	$4
SP_WR8	%ecx	$0
JUMP	@IC53
@IC58:	
LD8	%ecx	$8
SP_WR8	%ecx	$0
JUMP	@IC53
@IC60:	
LD8	%ecx	$128
SP_WR8	%ecx	$0
JUMP	@IC53
@IC62:	
LD8	%ecx	$32
SP_WR8	%ecx	$0
JUMP	@IC53
@IC64:	
LD8	%ecx	$64
SP_WR8	%ecx	$0
JUMP	@IC53
@IC55:	
SP_STORE	%r0
INC16	%r0	$16
CPY16	%r0	%r0
CPY8	%r1	(%r0)
CMP8	%r1	$2
JZ	@IC54
@IC57:	
CMP8	%r1	$7
JZ	@IC56
@IC59:	
CMP8	%r1	$3
JZ	@IC58
@IC61:	
CMP8	%r1	$9
JZ	@IC60
@IC63:	
CMP8	%r1	$8
JZ	@IC62
@IC65:	
JUMP	@IC64
@IC53:	
@IC48:	
SP_RD8	%eax	$0
SP_WR8	%eax	$39
SP_INC	$20
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"hostGetDevType"

hostGetPortDevType:	
.GLOBAL	 DO_NOT_EXPORT  "hostGetPortDevType"

.FUNCTION	"hostGetPortDevType"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_RD16	%ecx	$20
LD8	%r0	$0
LD8	%r1	$0
@IC66:	
CMP8	%r1	$16
JGE	@IC67
@IC68:	
CPY8	%r2	%r1
AND32	%r2	$255
MUL32	%r2	$7
LD16	%r3	$deviceList
ADD16	%r2	%r3
CPY32	%r2	(%r2)
CMP32	%r2	$0
JZ	@IC72
@IC73:	
CPY8	%r2	%r1
AND32	%r2	$255
MUL32	%r2	$7
LD16	%r3	$deviceList
ADD16	%r2	%r3
INC16	%r2	$4
CPY16	%r2	(%r2)
SP_RD32	%ecx	$20
CMP16	%ecx	%r2
JNZ	@IC76
@IC77:	
CPY8	%r2	%r1
AND32	%r2	$255
MUL32	%r2	$7
LD16	%r3	$deviceList
ADD16	%r2	%r3
INC16	%r2	$6
CPY8	%r2	(%r2)
CPY8	%eax	%r0
CPY8	%ebx	%r2
AND32	%eax	$255
AND32	%ebx	$255
OR32	%r2	%eax	%ebx
CPY8	%r0	%r2
@IC76:	
@IC72:	
@IC69:	
INC8	%r1	$1
JUMP	@IC66
@IC67:	
SP_STORE	%eax
INC16	%eax	$19
CPY8	(%eax)	%r0
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"hostGetPortDevType"

hostSetDeviceFtdi:	
.GLOBAL	 DO_NOT_EXPORT  "hostSetDeviceFtdi"

.FUNCTION	"hostSetDeviceFtdi"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
SP_RD8	%r0	$16
CMP8	%r0	$16
JLT	@IC81
@IC82:	
CMP8	%r0	$83
JNZ	@IC80
@IC81:	
CPY8	%r1	%r0
AND32	%r1	$255
MUL32	%r1	$7
LD16	%r2	$deviceList
ADD16	%r1	%r2
CPY32	%r1	(%r1)
CMP32	%r1	$0
JZ	@IC87
@IC88:	
CPY8	%r1	%r0
AND32	%r1	$255
MUL32	%r1	$7
LD16	%r2	$deviceList
ADD16	%r1	%r2
INC16	%r1	$6
LD8	(%r1)	$1
@IC87:	
LD8	%eax	$0
SP_WR8	%eax	$15
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
@IC80:	
LD8	%eax	$3
SP_WR8	%eax	$15
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"hostSetDeviceFtdi"

hostSelectDevice:	
.GLOBAL	 DO_NOT_EXPORT  "hostSelectDevice"

.FUNCTION	"hostSelectDevice"	
PUSH32	%r0
SP_RD8	%r0	$8
CMP8	%r0	$16
JLT	@IC92
@IC93:	
CMP8	%r0	$83
JNZ	@IC91
@IC92:	
CPY8	curDevice	%r0
LD8	%eax	$0
SP_WR8	%eax	$7
POP32	%r0
RTS	
@IC91:	
LD8	%eax	$3
SP_WR8	%eax	$7
POP32	%r0
RTS	
.FUNC_END	"hostSelectDevice"

