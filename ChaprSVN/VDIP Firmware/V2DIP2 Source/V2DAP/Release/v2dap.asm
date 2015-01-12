.DATA

.WEAK	"%eax"
.WEAK	"%ebx"
.WEAK	"%ecx"
.WEAK	"%r0"
.WEAK	"%r1"
.WEAK	"%r2"
.WEAK	"%r3"
fatContext	.DW	1	?
.GLOBAL	  DO_NOT_EXPORT "fatContext"
tcbMonitor	.DW	1	?
.GLOBAL	  DO_NOT_EXPORT "tcbMonitor"
monInterface	.DB	1	?
.GLOBAL	  DO_NOT_EXPORT "monInterface"
hUsb1	.DW	1	?
.GLOBAL	  DO_NOT_EXPORT "hUsb1"
hUsb2	.DW	1	?
.GLOBAL	  DO_NOT_EXPORT "hUsb2"
hBoms	.DW	1	?
.GLOBAL	  DO_NOT_EXPORT "hBoms"
hMonitor	.DW	1	?
.GLOBAL	  DO_NOT_EXPORT "hMonitor"
Str@0	.ASCIIZ	"Application"
Str@2	.ASCIIZ	"\rVer V2DAP"
Str@3	.ASCIIZ	" On-Line:\r"
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

Array@1	.DB	7	68, 65, 84, 65, 73, 78, 0

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

.WEAK	"fat_dirTableFindFirst"

.WEAK	"monAddNumberToConsole"

.WEAK	"vos_signal_semaphore"

.WEAK	"fat_fileMod"

.WEAK	"monFileOpen"

.WEAK	"vos_gpio_wait_on_int"

.WEAK	"monReadOnly"

.WEAK	"monDiskFull"

.WEAK	"vos_dma_get_fifo_data"

.WEAK	"fatdrv_init"

.WEAK	"hostSuspend"

.WEAK	"monASCIItoDec"

.WEAK	"vos_iocell_get_config"

.WEAK	"vos_iomux_define_bidi"

.WEAK	"vos_gpio_set_all_mode"

.WEAK	"vos_iocell_set_config"

.WEAK	"fat_fileRead"

.WEAK	"diskCheckFwu"

.WEAK	"vos_gpio_set_pin_mode"

.WEAK	"fat_fileSeek"

.WEAK	"monLedsGreen"

.WEAK	"fat_dirEntryIsReadOnly"

.WEAK	"vos_get_chip_revision"

.WEAK	"fat_fileTell"

.WEAK	"vos_wait_semaphore_ex"

.WEAK	"fat_fileOpen"

.WEAK	"monNoUpgrade"

.WEAK	"fat_fileCopy"

.WEAK	"vos_enable_interrupts"

.WEAK	"fat_capacity"

.WEAK	"vos_dev_read"

.WEAK	"vos_dev_open"

.WEAK	"vos_halt_cpu"

.WEAK	"vos_dev_init"

.WEAK	"monBadCommand"

.WEAK	"vos_dma_get_fifo_count"

.WEAK	"diskCheckDisk"

.WEAK	"fat_getFSType"

.WEAK	"usbhost_init"

.WEAK	"monLedsOffNow"

.WEAK	"vos_reset_kernel_clock"

.WEAK	"fat_freeSpace"

.WEAK	"monReadNumber"

.WEAK	"fat_fileClose"

.WEAK	"abs"

.WEAK	"diskResetDisk"

.WEAK	"fat_dirIsRoot"

.WEAK	"vos_gpio_set_port_mode"

.WEAK	"fat_fileFlush"

.WEAK	"vos_iomux_define_input"

.WEAK	"fat_fileWrite"

.WEAK	"vos_disable_interrupts"

.WEAK	"hostConnected"

.WEAK	"fat_dirEntryIsDirectory"

.WEAK	"vos_get_idle_thread_tcb"

.WEAK	"vos_dma_reset"

.WEAK	"vos_dev_close"

.WEAK	"vos_wdt_clear"

.WEAK	"vos_heap_size"

.WEAK	"vos_dev_ioctl"

.WEAK	"spislave_init"

.WEAK	"vos_dev_write"

.WEAK	"fat_fileDelete"

.WEAK	"fat_fileRename"

.WEAK	"vos_get_clock_frequency"

.WEAK	"fat_fileSetPos"

.WEAK	"hostGetDevType"

.WEAK	"vos_set_clock_frequency"

.WEAK	"free"

.WEAK	"fat_fileRewind"

.WEAK	"diskRemoveDisk"

.WEAK	"monDirNotEmpty"

.WEAK	"hostRemoveFromDeviceList"

.WEAK	"itoa"

.WEAK	"atoi"

.WEAK	"vos_dma_enable"

.WEAK	"ltoa"

.WEAK	"atol"

.WEAK	"vos_reset_vnc2"

.WEAK	"vos_heap_space"

.WEAK	"vos_iomux_define_output"

.WEAK	"monReadFileName"

.WEAK	"monWriteString"

.WEAK	"vos_wdt_enable"

.WEAK	"fat_getVolumeID"

.WEAK	"monSlaveEnabled"

.WEAK	"vos_dma_wait_on_complete"

.WEAK	"vos_lock_mutex"

.WEAK	"vos_power_down"

.WEAK	"vos_init_mutex"

.WEAK	"fat_dirEntryIsVolumeLabel"

.WEAK	"vos_gpio_wait_on_any_int"

.WEAK	"vos_get_priority_ceiling"

.WEAK	"vos_dma_disable"

.WEAK	"vos_set_priority_ceiling"

.WEAK	"vos_dma_release"

.WEAK	"vos_iomux_disable_output"

.WEAK	"vos_dma_acquire"

.WEAK	"fat_dirChangeDir"

.WEAK	"vos_delay_msecs"

.WEAK	"vos_stack_usage"

.WEAK	"fat_dirTableFind"

.WEAK	"fat_getDevHandle"

.WEAK	"vos_get_profile"

.WEAK	"fat_dirCreateDir"

.WEAK	"monCommandFailed"

.WEAK	"monWriteFileName"

.WEAK	"monSlaveDisabled"

.WEAK	"vos_gpio_wait_on_all_ints"

.WEAK	"monDeviceRemoved"

.WEAK	"fat_dirEntryName"

.WEAK	"hostSelectDevice"

.WEAK	"calloc"

.WEAK	"malloc"

.WEAK	"fat_dirEntryTime"

.WEAK	"fat_fileTruncate"

.WEAK	"fat_dirEntrySize"

.WEAK	"vos_delay_cancel"

.WEAK	"strtol"

.WEAK	"monLedsGreenFlash"

.WEAK	"vos_dma_retained_configure"

.WEAK	"monDeviceDetected"

.WEAK	"hostSetDeviceFtdi"

.WEAK	"fat_dirDirIsEmpty"

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

.WEAK	"fat_dirEntryIsFile"

.WEAK	"monCheck"

.WEAK	"monFilenameInvalid"

.WEAK	"fat_getVolumeLabel"

.WEAK	"initHeap"

.WEAK	"diskInit"

.WEAK	"vos_stop_profiler"

.WEAK	"hostGetPortDevType"

.WEAK	"hostWake"

.WEAK	"fat_time"

.WEAK	"fat_open"

.WEAK	"fat_init"

.WEAK	"vos_trylock_mutex"

.WEAK	"monWrite"

.WEAK	"fat_bytesPerSector"

.WEAK	"vos_free"

.WEAK	"vos_init"

.WEAK	"monValidateFileName"

.WEAK	"hostAddToDeviceList"

.WEAK	"monReadCr"

.WEAK	"monValidateDateTime"

.WEAK	"vos_gpio_read_port"

.WEAK	"vos_gpio_write_all"

.WEAK	"vos_set_idle_thread_tcb_size"

.WEAK	"vos_init_semaphore"

.WEAK	"vos_wait_semaphore"

.WEAK	"monNoDisk"

.WEAK	"vos_gpio_write_pin"

.WEAK	"fat_dirEntryIsValid"

.WEAK	"vos_start_profiler"

.WEAK	"monOnline"

.WEAK	"fat_close"

.WEAK	"fifo_init"

.WEAK	"boms_init"

.WEAK	"fat_bytesPerCluster"

.WEAK	"monPrompt"

.WEAK	"uart_init"

.WEAK	"vos_gpio_enable_int"

.WEAK	"monReadCmd"

.WEAK	"vos_signal_cond_var"

.WEAK	"fat_dirTableFindNext"

.WEAK	"monLedsOff"

.WEAK	"monLedsRed"

main:	
.GLOBAL	 DO_NOT_EXPORT  "main"

.FUNCTION	"main"	
PUSH32	%r0
PUSH32	%r1
SP_DEC	$87
SP_STORE	%r0
CPY16	%r0	%r0
LD8	(%r0)	$64
SP_STORE	%r0
INC16	%r0	$1
CPY16	%r0	%r0
LD8	(%r0)	$64
SP_STORE	%r0
INC16	%r0	$2
CPY16	%r1	%r0
LD8	(%r1)	$0
INC16	%r0	$1
LD8	(%r0)	$64
PUSH8	$4
PUSH16	$1
PUSH8	$10
CALL	vos_init
SP_INC	$4
PUSH16	$256
CALL	vos_set_idle_thread_tcb_size
SP_INC	$2
PUSH8	$0
CALL	vos_set_clock_frequency
SP_INC	$1
SP_STORE	%r0
INC16	%r0	$4
CPY16	%r1	%r0
LD8	(%r1)	$16
LD16	%r1	$1
ADD16	%r1	%r0
LD8	(%r1)	$32
LD16	%r1	$2
ADD16	%r1	%r0
LD8	(%r1)	$2
LD16	%r1	$3
ADD16	%r1	%r0
LD8	(%r1)	$1
PUSH16	%r0
PUSH8	$2
PUSH8	$1
SP_DEC	$1
CALL	usbhost_init
POP8	%eax
SP_WR8	%eax	$12
SP_INC	$4
PUSH8	$3
SP_DEC	$1
CALL	boms_init
POP8	%eax
SP_WR8	%eax	$10
SP_INC	$1
SP_DEC	$1
CALL	vos_get_package_type
POP8	%eax
SP_WR8	%eax	$10
SP_STORE	%ecx
INC16	%ecx	$10
CMP8	(%ecx)	$1
JNZ	@IC2
@IC3:	
PUSH8	$53
PUSH8	$46
SP_DEC	$1
CALL	vos_iomux_define_input
POP8	%eax
SP_WR8	%eax	$13
SP_INC	$2
PUSH8	$54
PUSH8	$47
SP_DEC	$1
CALL	vos_iomux_define_input
POP8	%eax
SP_WR8	%eax	$14
SP_INC	$2
LD8	%ecx	$0
SP_WR8	%ecx	$13
SP_STORE	%r0
INC16	%r0	$13
PUSH16	%r0
PUSH8	$3
SP_DEC	$1
CALL	vos_gpio_read_port
POP8	%eax
SP_WR8	%eax	$17
SP_INC	$3
JUMP	@IC1
@IC2:	
LD8	%ecx	$64
SP_WR8	%ecx	$13
@IC1:	
SP_RD8	%eax	$13
AND32	%eax	$255
LD32	%r0	$96
AND32	%r0	%eax
CMP32	%r0	$96
JZ	@IC8
JNZ	@IC9
@IC9:	
SP_RD8	%eax	$13
AND32	%eax	$255
LD32	%r0	$96
AND32	%r0	%eax
CMP32	%r0	$0
JNZ	@IC7
@IC8:	
SP_STORE	%r0
INC16	%r0	$0
PUSH16	%r0
PUSH8	$0
SP_DEC	$1
CALL	uart_init
POP8	%eax
SP_WR8	%eax	$18
SP_INC	$3
LD8	monInterface	$0
JUMP	@IC6
@IC7:	
SP_RD8	%eax	$13
AND32	%eax	$255
LD32	%r0	$32
AND32	%r0	%eax
CMP32	%r0	$0
JZ	@IC15
@IC16:	
SP_STORE	%r0
INC16	%r0	$1
PUSH16	%r0
PUSH8	$0
SP_DEC	$1
CALL	fifo_init
POP8	%eax
SP_WR8	%eax	$19
SP_INC	$3
LD8	monInterface	$2
JUMP	@IC14
@IC15:	
SP_STORE	%r0
INC16	%r0	$2
PUSH16	%r0
PUSH8	$0
SP_DEC	$1
CALL	spislave_init
POP8	%eax
SP_WR8	%eax	$20
SP_INC	$3
LD8	monInterface	$1
@IC14:	
@IC6:	
SP_DEC	$1
CALL	vos_get_package_type
POP8	%eax
SP_WR8	%eax	$18
SP_STORE	%ecx
INC16	%ecx	$18
CMP8	(%ecx)	$1
JNZ	@IC17
@IC18:	
PUSH8	$96
PUSH8	$24
PUSH8	$11
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$22
SP_INC	$3
PUSH8	$97
PUSH8	$25
PUSH8	$12
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$23
SP_INC	$3
PUSH8	$98
PUSH8	$26
PUSH8	$13
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$24
SP_INC	$3
PUSH8	$99
PUSH8	$27
PUSH8	$14
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$25
SP_INC	$3
PUSH8	$100
PUSH8	$28
PUSH8	$15
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$26
SP_INC	$3
PUSH8	$101
PUSH8	$29
PUSH8	$16
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$27
SP_INC	$3
PUSH8	$102
PUSH8	$30
PUSH8	$18
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$28
SP_INC	$3
PUSH8	$103
PUSH8	$31
PUSH8	$19
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$29
SP_INC	$3
PUSH8	$104
PUSH8	$32
PUSH8	$20
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$30
SP_INC	$3
PUSH8	$105
PUSH8	$33
PUSH8	$21
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$31
SP_INC	$3
PUSH8	$106
PUSH8	$34
PUSH8	$22
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$32
SP_INC	$3
PUSH8	$107
PUSH8	$35
PUSH8	$23
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$33
SP_INC	$3
CMP8	monInterface	$0
JNZ	@IC21
@IC22:	
PUSH8	$116
PUSH8	$35
SP_DEC	$1
CALL	vos_iomux_define_output
POP8	%eax
SP_WR8	%eax	$33
SP_INC	$2
PUSH8	$45
PUSH8	$36
SP_DEC	$1
CALL	vos_iomux_define_input
POP8	%eax
SP_WR8	%eax	$34
SP_INC	$2
PUSH8	$121
PUSH8	$49
PUSH8	$42
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$36
SP_INC	$3
PUSH8	$122
PUSH8	$50
PUSH8	$43
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$37
SP_INC	$3
PUSH8	$123
PUSH8	$51
PUSH8	$44
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$38
SP_INC	$3
PUSH8	$124
PUSH8	$52
PUSH8	$45
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$39
SP_INC	$3
PUSH8	$125
PUSH8	$53
PUSH8	$46
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$40
SP_INC	$3
@IC21:	
CMP8	monInterface	$1
JNZ	@IC25
@IC26:	
PUSH8	$17
PUSH8	$31
SP_DEC	$1
CALL	vos_iomux_define_input
POP8	%eax
SP_WR8	%eax	$40
SP_INC	$2
PUSH8	$18
PUSH8	$32
SP_DEC	$1
CALL	vos_iomux_define_input
POP8	%eax
SP_WR8	%eax	$41
SP_INC	$2
PUSH8	$88
PUSH8	$33
SP_DEC	$1
CALL	vos_iomux_define_output
POP8	%eax
SP_WR8	%eax	$42
SP_INC	$2
PUSH8	$19
PUSH8	$34
SP_DEC	$1
CALL	vos_iomux_define_input
POP8	%eax
SP_WR8	%eax	$43
SP_INC	$2
PUSH8	$117
PUSH8	$45
PUSH8	$36
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$45
SP_INC	$3
PUSH8	$118
PUSH8	$46
PUSH8	$37
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$46
SP_INC	$3
PUSH8	$119
PUSH8	$47
PUSH8	$38
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$47
SP_INC	$3
PUSH8	$120
PUSH8	$48
PUSH8	$41
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$48
SP_INC	$3
PUSH8	$121
PUSH8	$49
PUSH8	$42
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$49
SP_INC	$3
PUSH8	$122
PUSH8	$50
PUSH8	$43
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$50
SP_INC	$3
PUSH8	$123
PUSH8	$51
PUSH8	$44
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$51
SP_INC	$3
PUSH8	$52
PUSH8	$45
SP_DEC	$1
CALL	vos_iomux_define_input
POP8	%eax
SP_WR8	%eax	$51
SP_INC	$2
PUSH8	$125
PUSH8	$46
SP_DEC	$1
CALL	vos_iomux_define_output
POP8	%eax
SP_WR8	%eax	$52
SP_INC	$2
@IC25:	
CMP8	monInterface	$2
JNZ	@IC29
@IC30:	
PUSH8	$69
PUSH8	$6
PUSH8	$31
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$54
SP_INC	$3
PUSH8	$70
PUSH8	$7
PUSH8	$32
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$55
SP_INC	$3
PUSH8	$71
PUSH8	$8
PUSH8	$33
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$56
SP_INC	$3
PUSH8	$72
PUSH8	$9
PUSH8	$34
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$57
SP_INC	$3
PUSH8	$73
PUSH8	$10
PUSH8	$35
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$58
SP_INC	$3
PUSH8	$74
PUSH8	$11
PUSH8	$36
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$59
SP_INC	$3
PUSH8	$75
PUSH8	$12
PUSH8	$37
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$60
SP_INC	$3
PUSH8	$76
PUSH8	$13
PUSH8	$38
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$61
SP_INC	$3
PUSH8	$77
PUSH8	$41
SP_DEC	$1
CALL	vos_iomux_define_output
POP8	%eax
SP_WR8	%eax	$61
SP_INC	$2
PUSH8	$78
PUSH8	$42
SP_DEC	$1
CALL	vos_iomux_define_output
POP8	%eax
SP_WR8	%eax	$62
SP_INC	$2
PUSH8	$15
PUSH8	$43
SP_DEC	$1
CALL	vos_iomux_define_input
POP8	%eax
SP_WR8	%eax	$63
SP_INC	$2
PUSH8	$16
PUSH8	$44
SP_DEC	$1
CALL	vos_iomux_define_input
POP8	%eax
SP_WR8	%eax	$64
SP_INC	$2
PUSH8	$52
PUSH8	$45
SP_DEC	$1
CALL	vos_iomux_define_input
POP8	%eax
SP_WR8	%eax	$65
SP_INC	$2
PUSH8	$125
PUSH8	$46
SP_DEC	$1
CALL	vos_iomux_define_output
POP8	%eax
SP_WR8	%eax	$66
SP_INC	$2
@IC29:	
PUSH8	$126
PUSH8	$54
PUSH8	$47
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$68
SP_INC	$3
PUSH8	$127
PUSH8	$55
PUSH8	$48
SP_DEC	$1
CALL	vos_iomux_define_bidi
POP8	%eax
SP_WR8	%eax	$69
SP_INC	$3
@IC17:	
SP_DEC	$1
CALL	vos_get_package_type
POP8	%eax
SP_WR8	%eax	$67
SP_STORE	%ecx
INC16	%ecx	$67
CMP8	(%ecx)	$2
JNZ	@IC33
@IC34:	
PUSH8	$97
PUSH8	$12
SP_DEC	$1
CALL	vos_iomux_define_output
POP8	%eax
SP_WR8	%eax	$70
SP_INC	$2
PUSH8	$98
PUSH8	$13
SP_DEC	$1
CALL	vos_iomux_define_output
POP8	%eax
SP_WR8	%eax	$71
SP_INC	$2
PUSH8	$101
PUSH8	$29
SP_DEC	$1
CALL	vos_iomux_define_output
POP8	%eax
SP_WR8	%eax	$72
SP_INC	$2
PUSH8	$102
PUSH8	$31
SP_DEC	$1
CALL	vos_iomux_define_output
POP8	%eax
SP_WR8	%eax	$73
SP_INC	$2
PUSH8	$65
PUSH8	$39
SP_DEC	$1
CALL	vos_iomux_define_output
POP8	%eax
SP_WR8	%eax	$74
SP_INC	$2
PUSH8	$1
PUSH8	$40
SP_DEC	$1
CALL	vos_iomux_define_input
POP8	%eax
SP_WR8	%eax	$75
SP_INC	$2
PUSH8	$66
PUSH8	$41
SP_DEC	$1
CALL	vos_iomux_define_output
POP8	%eax
SP_WR8	%eax	$76
SP_INC	$2
PUSH8	$2
PUSH8	$42
SP_DEC	$1
CALL	vos_iomux_define_input
POP8	%eax
SP_WR8	%eax	$77
SP_INC	$2
PUSH8	$116
PUSH8	$43
SP_DEC	$1
CALL	vos_iomux_define_output
POP8	%eax
SP_WR8	%eax	$78
SP_INC	$2
PUSH8	$45
PUSH8	$44
SP_DEC	$1
CALL	vos_iomux_define_input
POP8	%eax
SP_WR8	%eax	$79
SP_INC	$2
@IC33:	
SP_DEC	$1
CALL	vos_get_package_type
POP8	%eax
SP_WR8	%eax	$78
SP_STORE	%ecx
INC16	%ecx	$78
CMP8	(%ecx)	$0
JNZ	@IC37
@IC38:	
PUSH8	$65
PUSH8	$23
SP_DEC	$1
CALL	vos_iomux_define_output
POP8	%eax
SP_WR8	%eax	$81
SP_INC	$2
PUSH8	$1
PUSH8	$24
SP_DEC	$1
CALL	vos_iomux_define_input
POP8	%eax
SP_WR8	%eax	$82
SP_INC	$2
PUSH8	$66
PUSH8	$25
SP_DEC	$1
CALL	vos_iomux_define_output
POP8	%eax
SP_WR8	%eax	$83
SP_INC	$2
PUSH8	$2
PUSH8	$26
SP_DEC	$1
CALL	vos_iomux_define_input
POP8	%eax
SP_WR8	%eax	$84
SP_INC	$2
PUSH8	$104
PUSH8	$29
SP_DEC	$1
CALL	vos_iomux_define_output
POP8	%eax
SP_WR8	%eax	$85
SP_INC	$2
PUSH8	$33
PUSH8	$30
SP_DEC	$1
CALL	vos_iomux_define_input
POP8	%eax
SP_WR8	%eax	$86
SP_INC	$2
@IC37:	
LD32	%r0	$firmware
LD32	%r1	$Str@0
PUSH16	$0
PUSH16	%r1
PUSH32	%r0
PUSH16	$4608
PUSH8	$20
SP_DEC	$2
CALL	vos_create_thread_ex
POP16	%eax
SP_WR16	%eax	$96
SP_INC	$11
SP_RD16	tcbMonitor	$85
CALL	vos_start_scheduler
@fl1main_loop:	
JUMP	@fl1main_loop
SP_INC	$87
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"main"

firmware:	
.GLOBAL	 DO_NOT_EXPORT  "firmware"

.FUNCTION	"firmware"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_DEC	$105
LD8	%ecx	$0
SP_WR8	%ecx	$0
LD8	%ecx	$0
SP_WR8	%ecx	$1
LD32	%ecx	$0
SP_WR32	%ecx	$2
LD32	%ecx	$0
SP_WR32	%ecx	$6
PUSH8	$0
CALL	monInit
SP_INC	$1
PUSH8	$255
PUSH8	$0
SP_DEC	$1
CALL	vos_gpio_set_port_mode
POP8	%eax
SP_WR8	%eax	$12
SP_INC	$2
PUSH8	$0
PUSH8	$1
SP_DEC	$1
CALL	vos_gpio_set_port_mode
POP8	%eax
SP_WR8	%eax	$13
SP_INC	$2
CMP8	monInterface	$0
JNZ	@IC42
@IC43:	
PUSH8	$16
PUSH8	$2
SP_DEC	$1
CALL	vos_gpio_set_port_mode
POP8	%eax
SP_WR8	%eax	$14
SP_INC	$2
JUMP	@IC41
@IC42:	
PUSH8	$0
PUSH8	$2
SP_DEC	$1
CALL	vos_gpio_set_port_mode
POP8	%eax
SP_WR8	%eax	$15
SP_INC	$2
@IC41:	
CMP8	monInterface	$0
JNZ	@IC47
@IC48:	
PUSH8	$0
PUSH8	$3
SP_DEC	$1
CALL	vos_gpio_set_port_mode
POP8	%eax
SP_WR8	%eax	$16
SP_INC	$2
JUMP	@IC46
@IC47:	
PUSH8	$32
PUSH8	$3
SP_DEC	$1
CALL	vos_gpio_set_port_mode
POP8	%eax
SP_WR8	%eax	$17
SP_INC	$2
@IC46:	
CALL	ackCmdMode
CALL	monLedsOff
PUSH8	$1
SP_DEC	$2
CALL	vos_dev_open
POP16	%eax
SP_WR16	%eax	$17
SP_INC	$1
SP_RD16	hUsb1	$16
PUSH8	$2
SP_DEC	$2
CALL	vos_dev_open
POP16	%eax
SP_WR16	%eax	$19
SP_INC	$1
SP_RD16	hUsb2	$18
PUSH8	$0
SP_DEC	$2
CALL	vos_dev_open
POP16	%eax
SP_WR16	%eax	$21
SP_INC	$1
SP_RD16	hMonitor	$20
SP_STORE	%r0
INC16	%r0	$22
CPY16	%r1	%r0
LD8	(%r1)	$34
LD16	%r1	$1
ADD16	%r1	%r0
CPY16	%r2	%r1
CPY16	%r3	%r2
LD32	(%r3)	$9600
PUSH16	%r0
SP_RD16	%eax	$22
PUSH16	%eax
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$35
SP_INC	$4
CPY16	%r3	%r0
LD8	(%r3)	$35
CPY16	%r3	%r2
LD8	(%r3)	$1
PUSH16	%r0
SP_RD16	%eax	$22
PUSH16	%eax
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$36
SP_INC	$4
CPY16	%r3	%r0
LD8	(%r3)	$36
CPY16	%r3	%r2
LD8	(%r3)	$1
PUSH16	%r0
SP_RD16	%eax	$22
PUSH16	%eax
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$37
SP_INC	$4
CPY16	%r3	%r0
LD8	(%r3)	$37
CPY16	%r2	%r2
LD8	(%r2)	$0
PUSH16	%r0
SP_RD16	%eax	$22
PUSH16	%eax
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$38
SP_INC	$4
CPY16	%r2	%r0
LD8	(%r2)	$38
CPY16	%r2	%r1
LD8	(%r2)	$0
PUSH16	%r0
SP_RD16	%eax	$22
PUSH16	%eax
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$39
SP_INC	$4
CPY16	%r2	%r0
LD8	(%r2)	$129
CPY16	%r2	%r1
LD8	(%r2)	$0
PUSH16	%r0
SP_RD16	%eax	$22
PUSH16	%eax
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$40
SP_INC	$4
CPY16	%r2	%r0
LD8	(%r2)	$69
CPY16	%r2	%r1
LD8	(%r2)	$4
PUSH16	%r0
SP_RD16	%eax	$22
PUSH16	%eax
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$41
SP_INC	$4
CPY16	%r2	%r0
LD8	(%r2)	$4
CPY16	%r1	%r1
LD8	(%r1)	$0
PUSH16	%r0
SP_RD16	%eax	$22
PUSH16	%eax
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$42
SP_INC	$4
@IC51:	
CALL	monLedsGreen
PUSH16	$250
SP_DEC	$1
CALL	vos_delay_msecs
POP8	%eax
SP_WR8	%eax	$41
SP_INC	$2
CALL	monLedsRed
PUSH16	$250
SP_DEC	$1
CALL	vos_delay_msecs
POP8	%eax
SP_WR8	%eax	$42
SP_INC	$2
CALL	monLedsGreen
PUSH16	$250
SP_DEC	$1
CALL	vos_delay_msecs
POP8	%eax
SP_WR8	%eax	$43
SP_INC	$2
CALL	monLedsRed
PUSH16	$250
SP_DEC	$1
CALL	vos_delay_msecs
POP8	%eax
SP_WR8	%eax	$44
SP_INC	$2
CALL	monLedsGreen
PUSH16	$250
SP_DEC	$1
CALL	vos_delay_msecs
POP8	%eax
SP_WR8	%eax	$45
SP_INC	$2
CALL	monLedsRed
PUSH16	$250
SP_DEC	$1
CALL	vos_delay_msecs
POP8	%eax
SP_WR8	%eax	$46
SP_INC	$2
CALL	monLedsOff
PUSH16	$250
SP_DEC	$1
CALL	vos_delay_msecs
POP8	%eax
SP_WR8	%eax	$47
SP_INC	$2
PUSH16	$250
SP_DEC	$1
CALL	vos_delay_msecs
POP8	%eax
SP_WR8	%eax	$48
SP_INC	$2
CMP8	monInterface	$0
JNZ	@IC55
@IC56:	
SP_STORE	%r0
INC16	%r0	$22
CPY16	%r1	%r0
LD8	(%r1)	$32
LD16	%r1	$5
ADD16	%r1	%r0
CPY16	%r2	%r1
LD8	(%r2)	$0
PUSH16	%r0
PUSH16	hMonitor
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$51
SP_INC	$4
CPY16	%r0	%r1
CPY8	%r2	(%r0)
AND8	%r2	$3
CPY8	(%r0)	%r2
CPY8	%r0	(%r1)
CMP8	%r0	$3
JZ	@IC59
@IC60:	
JUMP	@IC53
@IC59:	
JUMP	@IC54
@IC55:	
JUMP	@IC53
@IC54:	
@IC52:	
LD8	%ecx	$1
CMP8	%ecx	$0
JNZ	@IC51
@IC53:	
CALL	msgOnline
PUSH8	$0
SP_DEC	$1
CALL	hostSelectDevice
POP8	%eax
SP_WR8	%eax	$49
SP_INC	$1
@IC63:	
LD8	%ecx	$1
CMP8	%ecx	$0
JZ	@IC64
@IC65:	
SP_RD32	%ecx	$2
CMP32	%ecx	$0
JNZ	@IC66
@IC67:	
PUSH16	hUsb1
SP_DEC	$1
CALL	hostConnected
POP8	%eax
SP_WR8	%eax	$51
SP_INC	$2
SP_STORE	%ecx
INC16	%ecx	$49
CMP8	(%ecx)	$17
JNZ	@IC71
@IC72:	
PUSH8	$0
CALL	monDeviceDetected
SP_INC	$1
PUSH16	hUsb1
SP_DEC	$1
CALL	hostAddToDeviceList
POP8	%eax
SP_WR8	%eax	$52
SP_INC	$2
LD32	%ecx	$1
SP_WR32	%ecx	$6
JUMP	@IC70
@IC71:	
PUSH16	hUsb2
SP_DEC	$1
CALL	hostConnected
POP8	%eax
SP_WR8	%eax	$53
SP_INC	$2
SP_STORE	%ecx
INC16	%ecx	$51
CMP8	(%ecx)	$17
JNZ	@IC75
@IC76:	
PUSH8	$1
CALL	monDeviceDetected
SP_INC	$1
PUSH16	hUsb2
SP_DEC	$1
CALL	hostAddToDeviceList
POP8	%eax
SP_WR8	%eax	$54
SP_INC	$2
LD32	%ecx	$2
SP_WR32	%ecx	$6
@IC75:	
@IC70:	
@IC66:	
SP_RD32	%ecx	$2
CMP32	%ecx	$1
JNZ	@IC79
@IC80:	
PUSH16	hUsb1
SP_DEC	$1
CALL	hostConnected
POP8	%eax
SP_WR8	%eax	$55
SP_INC	$2
SP_STORE	%ecx
INC16	%ecx	$53
CMP8	(%ecx)	$0
JNZ	@IC84
@IC85:	
PUSH8	$0
CALL	monDeviceRemoved
SP_INC	$1
PUSH16	hUsb1
SP_DEC	$1
CALL	hostRemoveFromDeviceList
POP8	%eax
SP_WR8	%eax	$56
SP_INC	$2
CALL	monLedsOff
LD8	%ecx	$0
SP_WR8	%ecx	$0
LD32	%ecx	$0
SP_WR32	%ecx	$6
JUMP	@IC83
@IC84:	
PUSH16	hUsb1
SP_DEC	$1
CALL	hostConnected
POP8	%eax
SP_WR8	%eax	$57
SP_INC	$2
SP_STORE	%ecx
INC16	%ecx	$55
CMP8	(%ecx)	$17
JNZ	@IC88
@IC89:	
SP_STORE	%ecx
CMP8	(%ecx)	$0
JNZ	@IC92
@IC93:	
PUSH16	hUsb1
SP_DEC	$1
CALL	hostGetPortDevType
POP8	%eax
SP_WR8	%eax	$58
SP_INC	$2
SP_RD8	%ecx	$56
SP_WR8	%ecx	$0
@IC92:	
@IC88:	
@IC83:	
SP_RD32	%ecx	$2
SP_RD32	%eax	$6
CMP32	%ecx	%eax
JNZ	@IC96
@IC97:	
PUSH16	hUsb2
SP_DEC	$1
CALL	hostConnected
POP8	%eax
SP_WR8	%eax	$59
SP_INC	$2
SP_STORE	%ecx
INC16	%ecx	$57
CMP8	(%ecx)	$17
JNZ	@IC101
@IC102:	
PUSH8	$1
CALL	monDeviceDetected
SP_INC	$1
PUSH16	hUsb2
SP_DEC	$1
CALL	hostAddToDeviceList
POP8	%eax
SP_WR8	%eax	$60
SP_INC	$2
LD32	%ecx	$4
SP_WR32	%ecx	$6
JUMP	@IC100
@IC101:	
SP_DEC	$1
CALL	checkDataMode
POP8	%eax
SP_WR8	%eax	$59
SP_STORE	%ecx
INC16	%ecx	$59
CMP8	(%ecx)	$0
JZ	@IC105
@IC106:	
CALL	ackDataMode
CALL	monDataMode
CALL	ackCmdMode
@IC105:	
@IC100:	
@IC96:	
@IC79:	
SP_RD32	%ecx	$2
CMP32	%ecx	$2
JNZ	@IC107
@IC108:	
PUSH16	hUsb2
SP_DEC	$1
CALL	hostConnected
POP8	%eax
SP_WR8	%eax	$62
SP_INC	$2
SP_STORE	%ecx
INC16	%ecx	$60
CMP8	(%ecx)	$0
JNZ	@IC112
@IC113:	
PUSH8	$1
CALL	monDeviceRemoved
SP_INC	$1
PUSH16	hUsb2
SP_DEC	$1
CALL	hostRemoveFromDeviceList
POP8	%eax
SP_WR8	%eax	$63
SP_INC	$2
CALL	monLedsOff
LD8	%ecx	$0
SP_WR8	%ecx	$1
LD32	%ecx	$0
SP_WR32	%ecx	$6
JUMP	@IC111
@IC112:	
PUSH16	hUsb2
SP_DEC	$1
CALL	hostConnected
POP8	%eax
SP_WR8	%eax	$64
SP_INC	$2
SP_STORE	%ecx
INC16	%ecx	$62
CMP8	(%ecx)	$17
JNZ	@IC116
@IC117:	
SP_STORE	%ecx
INC16	%ecx	$1
CMP8	(%ecx)	$0
JNZ	@IC120
@IC121:	
PUSH16	hUsb2
SP_DEC	$1
CALL	hostGetPortDevType
POP8	%eax
SP_WR8	%eax	$65
SP_INC	$2
SP_RD8	%ecx	$63
SP_WR8	%ecx	$1
SP_RD8	%eax	$63
AND32	%eax	$255
LD32	%r0	$32
AND32	%r0	%eax
CMP32	%r0	$0
JZ	@IC124
@IC125:	
CALL	monLedsRed
PUSH8	$3
SP_DEC	$2
CALL	vos_dev_open
POP16	%eax
SP_WR16	%eax	$65
SP_INC	$1
SP_RD16	hBoms	$64
PUSH16	hUsb2
SP_DEC	$1
CALL	diskCheckDisk
POP8	%eax
SP_WR8	%eax	$68
SP_INC	$2
SP_STORE	%ecx
INC16	%ecx	$66
CMP8	(%ecx)	$0
JNZ	@IC127
@IC128:	
CALL	monLedsGreen
LD32	%ecx	$3
SP_WR32	%ecx	$6
CALL	monPrompt
JUMP	@IC126
@IC127:	
CALL	monLedsOff
PUSH16	hBoms
CALL	vos_dev_close
SP_INC	$2
@IC126:	
@IC124:	
@IC120:	
@IC116:	
@IC111:	
SP_RD32	%ecx	$2
SP_RD32	%eax	$6
CMP32	%ecx	%eax
JNZ	@IC131
@IC132:	
PUSH16	hUsb1
SP_DEC	$1
CALL	hostConnected
POP8	%eax
SP_WR8	%eax	$69
SP_INC	$2
SP_STORE	%ecx
INC16	%ecx	$67
CMP8	(%ecx)	$17
JNZ	@IC136
@IC137:	
PUSH8	$0
CALL	monDeviceDetected
SP_INC	$1
PUSH16	hUsb1
SP_DEC	$1
CALL	hostAddToDeviceList
POP8	%eax
SP_WR8	%eax	$70
SP_INC	$2
LD32	%ecx	$4
SP_WR32	%ecx	$6
JUMP	@IC135
@IC136:	
SP_DEC	$1
CALL	checkDataMode
POP8	%eax
SP_WR8	%eax	$69
SP_STORE	%ecx
INC16	%ecx	$69
CMP8	(%ecx)	$0
JZ	@IC140
@IC141:	
CALL	ackDataMode
CALL	monDataMode
CALL	ackCmdMode
@IC140:	
@IC135:	
@IC131:	
@IC107:	
SP_RD32	%ecx	$2
CMP32	%ecx	$3
JNZ	@IC142
@IC143:	
PUSH16	hUsb2
SP_DEC	$1
CALL	hostConnected
POP8	%eax
SP_WR8	%eax	$72
SP_INC	$2
SP_STORE	%ecx
INC16	%ecx	$70
CMP8	(%ecx)	$0
JNZ	@IC146
@IC147:	
SP_DEC	$1
CALL	diskRemoveDisk
POP8	%eax
SP_WR8	%eax	$71
PUSH8	$1
CALL	monDeviceRemoved
SP_INC	$1
PUSH16	hUsb2
SP_DEC	$1
CALL	hostRemoveFromDeviceList
POP8	%eax
SP_WR8	%eax	$74
SP_INC	$2
CALL	monLedsOff
LD8	%ecx	$0
SP_WR8	%ecx	$1
LD32	%ecx	$0
SP_WR32	%ecx	$6
PUSH16	hBoms
CALL	vos_dev_close
SP_INC	$2
@IC146:	
SP_RD32	%ecx	$2
SP_RD32	%eax	$6
CMP32	%ecx	%eax
JNZ	@IC150
@IC151:	
PUSH16	hUsb1
SP_DEC	$1
CALL	hostConnected
POP8	%eax
SP_WR8	%eax	$75
SP_INC	$2
SP_STORE	%ecx
INC16	%ecx	$73
CMP8	(%ecx)	$17
JNZ	@IC154
@IC155:	
PUSH8	$0
CALL	monDeviceDetected
SP_INC	$1
PUSH16	hUsb1
SP_DEC	$1
CALL	hostAddToDeviceList
POP8	%eax
SP_WR8	%eax	$76
SP_INC	$2
LD32	%ecx	$5
SP_WR32	%ecx	$6
@IC154:	
@IC150:	
@IC142:	
SP_RD32	%ecx	$2
CMP32	%ecx	$4
JNZ	@IC158
@IC159:	
PUSH16	hUsb1
SP_DEC	$1
CALL	hostConnected
POP8	%eax
SP_WR8	%eax	$77
SP_INC	$2
SP_STORE	%ecx
INC16	%ecx	$75
CMP8	(%ecx)	$0
JNZ	@IC163
@IC164:	
PUSH8	$0
CALL	monDeviceRemoved
SP_INC	$1
PUSH16	hUsb1
SP_DEC	$1
CALL	hostRemoveFromDeviceList
POP8	%eax
SP_WR8	%eax	$78
SP_INC	$2
LD8	%ecx	$0
SP_WR8	%ecx	$0
LD32	%ecx	$2
SP_WR32	%ecx	$6
JUMP	@IC162
@IC163:	
PUSH16	hUsb1
SP_DEC	$1
CALL	hostConnected
POP8	%eax
SP_WR8	%eax	$79
SP_INC	$2
SP_STORE	%ecx
INC16	%ecx	$77
CMP8	(%ecx)	$17
JNZ	@IC167
@IC168:	
SP_STORE	%ecx
CMP8	(%ecx)	$0
JNZ	@IC171
@IC172:	
PUSH16	hUsb1
SP_DEC	$1
CALL	hostGetPortDevType
POP8	%eax
SP_WR8	%eax	$80
SP_INC	$2
SP_RD8	%ecx	$78
SP_WR8	%ecx	$0
@IC171:	
@IC167:	
@IC162:	
SP_RD32	%ecx	$2
SP_RD32	%eax	$6
CMP32	%ecx	%eax
JNZ	@IC175
@IC176:	
PUSH16	hUsb2
SP_DEC	$1
CALL	hostConnected
POP8	%eax
SP_WR8	%eax	$81
SP_INC	$2
SP_STORE	%ecx
INC16	%ecx	$79
CMP8	(%ecx)	$0
JNZ	@IC180
@IC181:	
PUSH8	$1
CALL	monDeviceRemoved
SP_INC	$1
PUSH16	hUsb2
SP_DEC	$1
CALL	hostRemoveFromDeviceList
POP8	%eax
SP_WR8	%eax	$82
SP_INC	$2
CALL	monLedsOff
LD8	%ecx	$0
SP_WR8	%ecx	$1
LD32	%ecx	$1
SP_WR32	%ecx	$6
JUMP	@IC179
@IC180:	
PUSH16	hUsb2
SP_DEC	$1
CALL	hostConnected
POP8	%eax
SP_WR8	%eax	$83
SP_INC	$2
SP_STORE	%ecx
INC16	%ecx	$81
CMP8	(%ecx)	$17
JNZ	@IC184
@IC185:	
SP_STORE	%ecx
INC16	%ecx	$1
CMP8	(%ecx)	$0
JNZ	@IC188
@IC189:	
PUSH16	hUsb2
SP_DEC	$1
CALL	hostGetPortDevType
POP8	%eax
SP_WR8	%eax	$84
SP_INC	$2
SP_RD8	%ecx	$82
SP_WR8	%ecx	$1
SP_RD8	%eax	$82
AND32	%eax	$255
LD32	%r0	$32
AND32	%r0	%eax
CMP32	%r0	$0
JZ	@IC192
@IC193:	
CALL	monLedsRed
PUSH8	$3
SP_DEC	$2
CALL	vos_dev_open
POP16	%eax
SP_WR16	%eax	$84
SP_INC	$1
SP_RD16	hBoms	$83
PUSH16	hUsb2
SP_DEC	$1
CALL	diskCheckDisk
POP8	%eax
SP_WR8	%eax	$87
SP_INC	$2
SP_STORE	%ecx
INC16	%ecx	$85
CMP8	(%ecx)	$0
JNZ	@IC195
@IC196:	
CALL	monLedsGreen
LD32	%ecx	$5
SP_WR32	%ecx	$6
CALL	monPrompt
JUMP	@IC194
@IC195:	
CALL	monLedsOff
PUSH16	hBoms
CALL	vos_dev_close
SP_INC	$2
@IC194:	
@IC192:	
@IC188:	
@IC184:	
@IC179:	
@IC175:	
SP_RD32	%ecx	$2
SP_RD32	%eax	$6
CMP32	%ecx	%eax
JNZ	@IC199
@IC200:	
SP_DEC	$1
CALL	checkDataMode
POP8	%eax
SP_WR8	%eax	$86
SP_STORE	%ecx
INC16	%ecx	$86
CMP8	(%ecx)	$0
JZ	@IC203
@IC204:	
CALL	ackDataMode
CALL	monDataMode
CALL	ackCmdMode
@IC203:	
@IC199:	
@IC158:	
SP_RD32	%ecx	$2
CMP32	%ecx	$5
JNZ	@IC205
@IC206:	
PUSH16	hUsb1
SP_DEC	$1
CALL	hostConnected
POP8	%eax
SP_WR8	%eax	$89
SP_INC	$2
SP_STORE	%ecx
INC16	%ecx	$87
CMP8	(%ecx)	$0
JNZ	@IC210
@IC211:	
PUSH8	$0
CALL	monDeviceRemoved
SP_INC	$1
PUSH16	hUsb1
SP_DEC	$1
CALL	hostRemoveFromDeviceList
POP8	%eax
SP_WR8	%eax	$90
SP_INC	$2
LD8	%ecx	$0
SP_WR8	%ecx	$0
LD32	%ecx	$3
SP_WR32	%ecx	$6
JUMP	@IC209
@IC210:	
PUSH16	hUsb2
SP_DEC	$1
CALL	hostConnected
POP8	%eax
SP_WR8	%eax	$91
SP_INC	$2
SP_STORE	%ecx
INC16	%ecx	$89
CMP8	(%ecx)	$0
JNZ	@IC214
@IC215:	
SP_DEC	$1
CALL	diskRemoveDisk
POP8	%eax
SP_WR8	%eax	$90
PUSH8	$1
CALL	monDeviceRemoved
SP_INC	$1
PUSH16	hUsb2
SP_DEC	$1
CALL	hostRemoveFromDeviceList
POP8	%eax
SP_WR8	%eax	$93
SP_INC	$2
CALL	monLedsOff
LD8	%ecx	$0
SP_WR8	%ecx	$1
LD32	%ecx	$1
SP_WR32	%ecx	$6
PUSH16	hBoms
CALL	vos_dev_close
SP_INC	$2
@IC214:	
@IC209:	
SP_RD32	%ecx	$2
SP_RD32	%eax	$6
CMP32	%ecx	%eax
JNZ	@IC218
@IC219:	
SP_DEC	$1
CALL	checkDataMode
POP8	%eax
SP_WR8	%eax	$92
SP_STORE	%ecx
INC16	%ecx	$92
CMP8	(%ecx)	$0
JZ	@IC222
@IC223:	
CPY8	%r0	curDevice
AND32	%r0	$255
MUL32	%r0	$7
LD16	%r1	$deviceList
ADD16	%r0	%r1	%r0
INC16	%r0	$4
CPY16	%r0	(%r0)
CMP16	%r0	hUsb1
JNZ	@IC224
@IC225:	
CALL	ackDataMode
CALL	monDataMode
CALL	ackCmdMode
@IC224:	
@IC222:	
@IC218:	
@IC205:	
SP_RD32	%ecx	$2
SP_RD32	%eax	$6
CMP32	%ecx	%eax
JNZ	@IC228
@IC229:	
SP_RD32	%ecx	$2
CMP32	%ecx	$5
JZ	@IC232
LD32	%ecx	$0
SP_WR32	%ecx	$93
JUMP	@IC233
@IC232:	
LD32	%ecx	$1
SP_WR32	%ecx	$93
@IC233:	
SP_RD32	%ecx	$93
CMP32	%ecx	$0
JNZ	@IC235
SP_RD32	%ecx	$2
CMP32	%ecx	$3
JZ	@IC237
LD32	%ecx	$0
SP_WR32	%ecx	$97
JUMP	@IC238
@IC237:	
LD32	%ecx	$1
SP_WR32	%ecx	$97
@IC238:	
SP_RD32	%ecx	$97
CMP32	%ecx	$0
JZ	@IC234
@IC235:	
LD32	%ecx	$1
SP_WR32	%ecx	$101
JUMP	@IC236
@IC234:	
LD32	%ecx	$0
SP_WR32	%ecx	$101
@IC236:	
SP_RD16	%eax	$101
PUSH8	%eax
CALL	monCheck
SP_INC	$1
@IC228:	
SP_RD32	%ecx	$6
SP_WR32	%ecx	$2
JUMP	@IC63
@IC64:	
SP_INC	$105
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"firmware"

checkDataMode:	
.GLOBAL	 DO_NOT_EXPORT  "checkDataMode"

.FUNCTION	"checkDataMode"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
SP_DEC	$3
LD8	%r0	$0
CPY8	%r1	curDevice
AND32	%r1	$255
MUL32	%r1	$7
LD16	%r2	$deviceList
ADD16	%r1	%r2
INC16	%r1	$6
CPY8	%r1	(%r1)
CPY8	%eax	%r1
AND32	%eax	$255
LD32	%r1	$128
AND32	%r1	%eax
CMP32	%r1	$0
JNZ	@IC239
@IC240:	
CMP8	monInterface	$0
JNZ	@IC244
@IC245:	
SP_STORE	%r1
INC16	%r1	$0
PUSH16	%r1
PUSH8	$2
SP_DEC	$1
CALL	vos_gpio_read_port
POP8	%eax
SP_WR8	%eax	$4
SP_INC	$3
SP_STORE	%eax
CPY8	%eax	(%eax)
AND32	%eax	$255
LD32	%r1	$32
AND32	%r1	%eax
CMP32	%r1	$0
JNZ	@IC248
@IC249:	
LD8	%r0	$1
@IC248:	
JUMP	@IC243
@IC244:	
SP_STORE	%r1
PUSH16	%r1
PUSH8	$3
SP_DEC	$1
CALL	vos_gpio_read_port
POP8	%eax
SP_WR8	%eax	$5
SP_INC	$3
SP_STORE	%eax
CPY8	%eax	(%eax)
AND32	%eax	$255
LD32	%r1	$16
AND32	%r1	%eax
CMP32	%r1	$0
JNZ	@IC252
@IC253:	
LD8	%r0	$1
@IC252:	
@IC243:	
@IC239:	
SP_STORE	%eax
INC16	%eax	$18
CPY8	(%eax)	%r0
SP_INC	$3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"checkDataMode"

ackDataMode:	
.GLOBAL	 DO_NOT_EXPORT  "ackDataMode"

.FUNCTION	"ackDataMode"	
PUSH32	%r0
SP_DEC	$5
CMP8	monInterface	$0
JNZ	@IC257
@IC258:	
SP_STORE	%r0
INC16	%r0	$0
PUSH16	%r0
PUSH8	$2
SP_DEC	$1
CALL	vos_gpio_read_port
POP8	%eax
SP_WR8	%eax	$4
SP_INC	$3
SP_STORE	%eax
CPY8	%eax	(%eax)
AND32	%eax	$255
LD32	%r0	$239
AND32	%r0	%eax
SP_STORE	%ecx
CPY8	(%ecx)	%r0
SP_RD16	%eax	$0
PUSH8	%eax
PUSH8	$2
SP_DEC	$1
CALL	vos_gpio_write_port
POP8	%eax
SP_WR8	%eax	$4
SP_INC	$2
JUMP	@IC256
@IC257:	
SP_STORE	%r0
PUSH16	%r0
PUSH8	$3
SP_DEC	$1
CALL	vos_gpio_read_port
POP8	%eax
SP_WR8	%eax	$6
SP_INC	$3
SP_STORE	%eax
CPY8	%eax	(%eax)
AND32	%eax	$255
LD32	%r0	$223
AND32	%r0	%eax
CPY8	%r0	%r0
PUSH8	%r0
PUSH8	$3
SP_DEC	$1
CALL	vos_gpio_write_port
POP8	%eax
SP_WR8	%eax	$6
SP_INC	$2
@IC256:	
SP_INC	$5
POP32	%r0
RTS	
.FUNC_END	"ackDataMode"

ackCmdMode:	
.GLOBAL	 DO_NOT_EXPORT  "ackCmdMode"

.FUNCTION	"ackCmdMode"	
PUSH32	%r0
SP_DEC	$5
CMP8	monInterface	$0
JNZ	@IC262
@IC263:	
SP_STORE	%r0
PUSH16	%r0
PUSH8	$2
SP_DEC	$1
CALL	vos_gpio_read_port
POP8	%eax
SP_WR8	%eax	$4
SP_INC	$3
SP_STORE	%eax
CPY8	%eax	(%eax)
AND32	%eax	$255
LD32	%r0	$16
OR32	%r0	%eax
SP_STORE	%ecx
CPY8	(%ecx)	%r0
SP_RD16	%eax	$0
PUSH8	%eax
PUSH8	$2
SP_DEC	$1
CALL	vos_gpio_write_port
POP8	%eax
SP_WR8	%eax	$4
SP_INC	$2
JUMP	@IC261
@IC262:	
SP_STORE	%r0
PUSH16	%r0
PUSH8	$3
SP_DEC	$1
CALL	vos_gpio_read_port
POP8	%eax
SP_WR8	%eax	$6
SP_INC	$3
SP_STORE	%eax
CPY8	%eax	(%eax)
AND32	%eax	$255
LD32	%r0	$32
OR32	%r0	%eax
CPY8	%r0	%r0
PUSH8	%r0
PUSH8	$3
SP_DEC	$1
CALL	vos_gpio_write_port
POP8	%eax
SP_WR8	%eax	$6
SP_INC	$2
@IC261:	
SP_INC	$5
POP32	%r0
RTS	
.FUNC_END	"ackCmdMode"

monDataMode:	
.GLOBAL	 DO_NOT_EXPORT  "monDataMode"

.FUNCTION	"monDataMode"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_DEC	$150
LD16	%ecx	$0
SP_WR16	%ecx	$0
SP_STORE	%ecx
INC16	%ecx	$2
LD32	%eax	$Array@1
CPYROM	(%ecx)	%eax	$3
INC16	%ecx	$6
INC32	%eax	$3
CPYROM	%ebx	%eax	$1
CPY8	(%ecx)	%ebx
LD8	%ecx	$2
SP_WR8	%ecx	$9
LD8	%ecx	$2
SP_WR8	%ecx	$10
CPY8	%r0	curDevice
AND32	%r0	$255
MUL32	%r0	$7
LD16	%r1	$deviceList
ADD16	%r0	%r1
SP_STORE	%ecx
INC16	%ecx	$11
CPY16	(%ecx)	%r0
SP_RD16	%r0	$11
INC16	%r0	$6
CPY8	%r0	(%r0)
CMP8	%r0	$16
JNZ	@IC266
@IC267:	
SP_DEC	$2
CALL	cdcSetup
POP16	%eax
SP_WR16	%eax	$13
SP_RD16	%ecx	$13
SP_WR16	%ecx	$11
@IC266:	
SP_STORE	%r0
INC16	%r0	$11
CPY16	%r1	(%r0)
CPY32	%r1	(%r1)
SP_STORE	%ecx
INC16	%ecx	$15
CPY32	(%ecx)	%r1
LD32	%ecx	$0
SP_WR32	%ecx	$19
LD32	%ecx	$0
SP_WR32	%ecx	$23
SP_STORE	%r2
INC16	%r2	$27
CPY16	%r3	%r2
LD8	(%r3)	$49
LD16	%r3	$2
ADD16	%r3	%r2
CPY16	%r3	%r3
CPY32	(%r3)	%r1
LD16	%r1	$6
ADD16	%r1	%r2
SP_STORE	%r3
INC16	%r3	$19
CPY16	(%r1)	%r3
CPY16	%r0	(%r0)
INC16	%r0	$4
CPY16	%r0	(%r0)
PUSH16	%r2
PUSH16	%r0
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$41
SP_INC	$4
SP_RD32	%ecx	$19
CMP32	%ecx	$0
JNZ	@IC270
@IC271:	
SP_STORE	%r0
INC16	%r0	$27
CPY16	%r1	%r0
LD8	(%r1)	$51
SP_RD16	%r1	$11
INC16	%r1	$4
CPY16	%r1	(%r1)
PUSH16	%r0
PUSH16	%r1
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$42
SP_INC	$4
LD8	%ecx	$0
SP_WR8	%ecx	$9
@IC270:	
SP_RD32	%ecx	$19
CMP32	%ecx	$0
JZ	@IC272
@IC273:	
SP_STORE	%r0
INC16	%r0	$27
CPY16	%r1	%r0
LD8	(%r1)	$56
LD16	%r1	$2
ADD16	%r1	%r0
CPY16	%r1	%r1
SP_STORE	%eax
INC16	%eax	$19
CPY32	(%r1)	(%eax)
LD16	%r1	$6
ADD16	%r1	%r0
SP_STORE	%r2
INC16	%r2	$39
CPY16	(%r1)	%r2
SP_RD16	%r1	$11
INC16	%r1	$4
CPY16	%r1	(%r1)
PUSH16	%r0
PUSH16	%r1
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$47
SP_INC	$4
@IC272:	
SP_STORE	%r0
INC16	%r0	$27
CPY16	%r1	%r0
LD8	(%r1)	$50
LD16	%r1	$2
ADD16	%r1	%r0
CPY16	%r1	%r1
SP_STORE	%eax
INC16	%eax	$15
CPY32	(%r1)	(%eax)
LD16	%r1	$6
ADD16	%r1	%r0
SP_STORE	%r2
INC16	%r2	$23
CPY16	(%r1)	%r2
SP_RD16	%r1	$11
INC16	%r1	$4
CPY16	%r1	(%r1)
PUSH16	%r0
PUSH16	%r1
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$48
SP_INC	$4
SP_RD32	%ecx	$23
CMP32	%ecx	$0
JNZ	@IC274
@IC275:	
SP_STORE	%r0
INC16	%r0	$27
CPY16	%r1	%r0
LD8	(%r1)	$52
SP_RD16	%r1	$11
INC16	%r1	$4
CPY16	%r1	(%r1)
PUSH16	%r0
PUSH16	%r1
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$49
SP_INC	$4
LD8	%ecx	$0
SP_WR8	%ecx	$10
@IC274:	
SP_STORE	%r0
INC16	%r0	$46
PUSH16	$0
PUSH16	%r0
CALL	vos_init_semaphore
SP_INC	$4
SP_RD32	%ecx	$19
CMP32	%ecx	$0
JZ	@IC276
@IC277:	
LD32	%r0	$monDataModeIn
SP_STORE	%r1
INC16	%r1	$2
SP_STORE	%r2
INC16	%r2	$39
LD16	%r3	$1
ADD16	%r3	%r2
CPY16	%r3	(%r3)
PUSH16	%r3
SP_RD16	%eax	$11
PUSH8	%eax
SP_RD16	%eax	$14
PUSH16	%eax
SP_RD32	%eax	$24
PUSH32	%eax
PUSH16	$9
PUSH16	%r1
PUSH32	%r0
PUSH16	$1200
PUSH8	$24
SP_DEC	$2
CALL	vos_create_thread_ex
POP16	%eax
SP_WR16	%eax	$71
SP_INC	$20
SP_RD16	%ecx	$51
SP_WR16	%ecx	$0
SP_STORE	%r0
INC16	%r0	$27
CPY16	%r1	%r0
LD8	(%r1)	$56
LD16	%r1	$2
ADD16	%r1	%r0
CPY16	%r1	%r1
SP_STORE	%eax
INC16	%eax	$19
CPY32	(%r1)	(%eax)
INC16	%r0	$6
CPY16	(%r0)	%r2
@IC276:	
SP_RD32	%ecx	$19
CMP32	%ecx	$0
JNZ	@IC278
@IC280:	
SP_RD32	%ecx	$23
CMP32	%ecx	$0
JNZ	@IC278
@IC279:	
SP_INC	$150
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
@IC278:	
@IC285:	
SP_DEC	$1
CALL	checkDataMode
POP8	%eax
SP_WR8	%eax	$53
SP_STORE	%ecx
INC16	%ecx	$53
CMP8	(%ecx)	$0
JZ	@IC286
@IC287:	
SP_RD32	%ecx	$23
CMP32	%ecx	$0
JZ	@IC288
@IC289:	
SP_STORE	%r0
INC16	%r0	$54
CPY16	%r1	%r0
LD8	(%r1)	$2
PUSH16	%r0
PUSH16	hMonitor
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$67
SP_INC	$4
INC16	%r0	$5
CPY16	%r0	(%r0)
SP_STORE	%ecx
INC16	%ecx	$64
CPY16	(%ecx)	%r0
SP_RD16	%ecx	$64
CMP16	%ecx	$0
JZ	@IC290
@IC291:	
SP_RD16	%ecx	$64
CMP16	%ecx	$64
JLE	@IC292
@IC293:	
LD16	%ecx	$64
SP_WR16	%ecx	$64
@IC292:	
SP_STORE	%r0
INC16	%r0	$66
SP_STORE	%r1
INC16	%r1	$130
PUSH16	%r1
SP_RD16	%eax	$66
PUSH16	%eax
PUSH16	%r0
PUSH16	hMonitor
SP_DEC	$1
CALL	vos_dev_read
POP8	%eax
SP_WR8	%eax	$140
SP_INC	$8
SP_STORE	%r1
INC16	%r1	$133
LD16	%r2	$7
ADD16	%r2	%r1
CPY16	(%r2)	%r0
LD16	%r0	$9
ADD16	%r0	%r1
SP_STORE	%eax
INC16	%eax	$64
CPY16	(%r0)	(%eax)
CPY16	%r0	%r1
SP_STORE	%eax
INC16	%eax	$23
CPY32	(%r0)	(%eax)
LD16	%r0	$4
ADD16	%r0	%r1
SP_STORE	%r2
INC16	%r2	$46
CPY16	(%r0)	%r2
LD16	%r0	$6
ADD16	%r0	%r1
LD8	(%r0)	$15
LD16	%r0	$11
ADD16	%r0	%r1
SP_STORE	%eax
INC16	%eax	$10
CPY8	(%r0)	(%eax)
SP_RD16	%r0	$11
INC16	%r0	$4
CPY16	%r0	(%r0)
CPY16	%r1	%r1
PUSH16	$0
PUSH16	$14
PUSH16	%r1
PUSH16	%r0
SP_DEC	$1
CALL	vos_dev_write
POP8	%eax
SP_WR8	%eax	$155
SP_INC	$8
SP_STORE	%ecx
INC16	%ecx	$147
CMP8	(%ecx)	$1
JNZ	@IC296
@IC297:	
JUMP	@IC286
@IC296:	
@IC290:	
@IC288:	
SP_RD32	%ecx	$19
CMP32	%ecx	$0
JZ	@IC285
@IC302:	
SP_STORE	%ecx
CMP16	(%ecx)	$0
JZ	@IC285
@IC301:	
SP_RD16	%r0	$11
INC16	%r0	$4
CPY16	%r0	(%r0)
SP_STORE	%r1
INC16	%r1	$27
PUSH16	%r1
PUSH16	%r0
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$152
SP_INC	$4
SP_STORE	%ecx
INC16	%ecx	$148
CMP8	(%ecx)	$1
JNZ	@IC303
@IC304:	
JUMP	@IC286
@IC303:	
@IC300:	
JUMP	@IC285
@IC286:	
SP_STORE	%ecx
CMP16	(%ecx)	$0
JZ	@IC307
@IC308:	
SP_STORE	%r0
INC16	%r0	$27
CPY16	%r1	%r0
LD8	(%r1)	$72
LD16	%r1	$2
ADD16	%r1	%r0
CPY16	%r1	%r1
SP_STORE	%eax
INC16	%eax	$19
CPY32	(%r1)	(%eax)
SP_RD16	%r1	$11
INC16	%r1	$4
CPY16	%r1	(%r1)
PUSH16	%r0
PUSH16	%r1
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$153
SP_INC	$4
@IC307:	
SP_INC	$150
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monDataMode"

monDataModeIn:	
.GLOBAL	 DO_NOT_EXPORT  "monDataModeIn"

.FUNCTION	"monDataModeIn"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_DEC	$26
SP_RD8	%ecx	$51
SP_WR8	%ecx	$51
SP_RD32	%ecx	$45
SP_WR32	%ecx	$45
SP_RD16	%ecx	$52
SP_STORE	%r1
INC16	%r1	$0
PUSH16	$0
PUSH16	%r1
CALL	vos_init_semaphore
SP_INC	$4
SP_RD16	%eax	$52
PUSH16	%eax
SP_DEC	$2
CALL	vos_malloc
POP16	%eax
SP_WR16	%eax	$7
SP_INC	$2
SP_RD16	%ecx	$5
SP_WR16	%ecx	$7
SP_RD16	%r0	$7
SP_RD16	%r1	$49
INC16	%r1	$6
CPY8	%r1	(%r1)
CMP8	%r1	$1
JNZ	@IC311
@IC312:	
SP_RD16	%r1	$7
INC16	%r1	$2
CPY16	%r0	%r1
@IC311:	
@IC315:	
SP_DEC	$1
CALL	checkDataMode
POP8	%eax
SP_WR8	%eax	$9
SP_STORE	%ecx
INC16	%ecx	$9
CMP8	(%ecx)	$0
JZ	@IC316
@IC317:	
SP_STORE	%r1
INC16	%r1	$10
LD16	%r2	$7
ADD16	%r2	%r1
SP_STORE	%eax
INC16	%eax	$7
CPY16	(%r2)	(%eax)
LD16	%r2	$9
ADD16	%r2	%r1
SP_STORE	%eax
INC16	%eax	$52
CPY16	(%r2)	(%eax)
CPY16	%r2	%r1
SP_STORE	%eax
INC16	%eax	$45
CPY32	(%r2)	(%eax)
LD16	%r2	$4
ADD16	%r2	%r1
SP_STORE	%r3
INC16	%r3	$0
CPY16	(%r2)	%r3
LD16	%r2	$6
ADD16	%r2	%r1
LD8	(%r2)	$15
LD16	%r2	$11
ADD16	%r2	%r1
SP_RD8	%eax	$51
AND32	%eax	$255
LD32	%r3	$64
OR32	%r3	%eax
CPY8	(%r2)	%r3
SP_RD16	%r2	$49
INC16	%r2	$4
CPY16	%r2	(%r2)
CPY16	%r1	%r1
PUSH16	$0
PUSH16	$14
PUSH16	%r1
PUSH16	%r2
SP_DEC	$1
CALL	vos_dev_read
POP8	%eax
SP_WR8	%eax	$32
SP_INC	$8
SP_RD8	%r3	$24
SP_STORE	%ecx
INC16	%ecx	$24
CMP8	(%ecx)	$0
JNZ	@IC319
@IC320:	
SP_STORE	%r1
INC16	%r1	$10
INC16	%r1	$6
CPY8	%r1	(%r1)
CMP8	%r1	$0
JNZ	@IC324
@IC325:	
SP_STORE	%r1
INC16	%r1	$10
INC16	%r1	$9
CPY16	%r1	(%r1)
CMP16	%r1	$0
JZ	@IC323
@IC329:	
SP_RD16	%r1	$49
INC16	%r1	$6
CPY8	%r1	(%r1)
CMP8	%r1	$1
JNZ	@IC330
@IC331:	
SP_STORE	%r1
INC16	%r1	$10
INC16	%r1	$9
CPY16	%r2	(%r1)
SUB16	%r2	$2
CPY16	(%r1)	%r2
@IC330:	
CPY16	%r1	%r0
SP_STORE	%r2
INC16	%r2	$10
INC16	%r2	$9
CPY16	%r2	(%r2)
PUSH16	%r2
PUSH16	%r1
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$29
SP_INC	$4
@IC328:	
JUMP	@IC318
@IC324:	
JUMP	@IC316
@IC323:	
JUMP	@IC315
@IC319:	
CMP8	%r3	$1
JNZ	@IC334
@IC335:	
JUMP	@IC316
@IC334:	
@IC318:	
JUMP	@IC315
@IC316:	
SP_RD16	%eax	$7
PUSH16	%eax
CALL	vos_free
SP_INC	$2
SP_INC	$26
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monDataModeIn"

cdcSetup:	
.GLOBAL	 DO_NOT_EXPORT  "cdcSetup"

.FUNCTION	"cdcSetup"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_DEC	$96
CPY8	%r1	curDevice
AND32	%r1	$255
MUL32	%r1	$7
LD16	%r0	$deviceList
ADD16	%r1	%r0	%r1
SP_STORE	%ecx
INC16	%ecx	$0
CPY16	(%ecx)	%r1
CPY8	%eax	curDevice
AND32	%eax	$255
LD32	%r1	$1
ADD32	%r1	%eax
LD32	%ebx	$7
MUL32	%r1	%ebx
ADD16	%r0	%r1
SP_STORE	%ecx
INC16	%ecx	$2
CPY16	(%ecx)	%r0
SP_STORE	%r0
INC16	%r0	$2
CPY16	%r1	(%r0)
CPY32	%r1	(%r1)
SP_STORE	%ecx
INC16	%ecx	$4
CPY32	(%ecx)	%r1
SP_RD16	%r2	$0
CPY32	%r2	(%r2)
SP_STORE	%ecx
INC16	%ecx	$8
CPY32	(%ecx)	%r2
SP_STORE	%r2
INC16	%r2	$12
CPY16	%r3	%r2
LD8	(%r3)	$37
LD16	%r3	$2
ADD16	%r3	%r2
CPY16	%r3	%r3
CPY32	(%r3)	%r1
LD16	%r1	$6
ADD16	%r1	%r2
SP_STORE	%r3
INC16	%r3	$22
CPY16	(%r1)	%r3
CPY16	%r0	(%r0)
INC16	%r0	$4
CPY16	%r0	(%r0)
PUSH16	%r2
PUSH16	%r0
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$29
SP_INC	$4
CPY8	%r0	(%r3)
CMP8	%r0	$10
JNZ	@IC338
@IC339:	
SP_STORE	%r0
INC16	%r0	$12
CPY16	%r1	%r0
LD8	(%r1)	$38
LD16	%r1	$6
ADD16	%r1	%r0
SP_STORE	%r2
INC16	%r2	$26
CPY16	(%r1)	%r2
SP_STORE	%ecx
INC16	%ecx	$33
SP_STORE	%eax
INC16	%eax	$2
CPY16	(%ecx)	%eax
SP_RD16	%eax	$33
CPY16	%r3	(%eax)
INC16	%r3	$4
CPY16	%r3	(%r3)
PUSH16	%r0
PUSH16	%r3
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$39
SP_INC	$4
INC16	%r2	$2
CPY8	%r2	(%r2)
SP_STORE	%ecx
INC16	%ecx	$36
CPY8	%eax	%r2
AND16	%eax	$255
CPY16	(%ecx)	%eax
LD32	%ecx	$0
SP_WR32	%ecx	$38
LD32	%ecx	$0
SP_WR32	%ecx	$42
CPY16	%r2	%r0
LD8	(%r2)	$51
LD16	%r2	$2
ADD16	%r2	%r0
CPY16	%r3	%r2
SP_STORE	%eax
INC16	%eax	$8
CPY32	(%r3)	(%eax)
SP_STORE	%r3
INC16	%r3	$38
CPY16	(%r1)	%r3
SP_RD16	%r3	$0
INC16	%r3	$4
CPY16	%r3	(%r3)
PUSH16	%r0
PUSH16	%r3
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$50
SP_INC	$4
CPY16	%r3	%r0
LD8	(%r3)	$48
CPY16	%r2	%r2
SP_STORE	%eax
INC16	%eax	$4
CPY32	(%r2)	(%eax)
SP_STORE	%r2
INC16	%r2	$42
CPY16	(%r1)	%r2
SP_RD16	%eax	$33
CPY16	%r1	(%eax)
INC16	%r1	$4
CPY16	%r1	(%r1)
PUSH16	%r0
PUSH16	%r1
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$51
SP_INC	$4
SP_RD32	%ecx	$38
CMP32	%ecx	$0
JZ	@IC342
@IC344:	
SP_RD32	%ecx	$42
CMP32	%ecx	$0
JZ	@IC342
@IC343:	
SP_STORE	%r0
INC16	%r0	$48
CPY16	%r1	%r0
LD8	(%r1)	$161
LD16	%r1	$1
ADD16	%r1	%r0
LD8	(%r1)	$33
LD16	%r2	$2
ADD16	%r2	%r0
LD16	(%r2)	$0
LD16	%r3	$4
ADD16	%r3	%r0
SP_STORE	%eax
INC16	%eax	$36
CPY16	(%r3)	(%eax)
SP_STORE	%ecx
INC16	%ecx	$56
LD16	%ebx	$6
ADD16	(%ecx)	%r0	%ebx
SP_RD16	%ecx	$56
LD16	(%ecx)	$7
SP_STORE	%ecx
INC16	%ecx	$58
SP_STORE	%eax
INC16	%eax	$12
CPY16	(%ecx)	%eax
SP_RD16	%r3	$58
LD8	(%r3)	$80
SP_RD16	%r3	$58
INC16	%r3	$2
CPY16	%r3	%r3
SP_STORE	%eax
INC16	%eax	$42
CPY32	(%r3)	(%eax)
SP_RD16	%r3	$58
INC16	%r3	$8
CPY16	(%r3)	%r0
SP_STORE	%ecx
INC16	%ecx	$60
SP_STORE	%eax
INC16	%eax	$58
LD16	%ebx	$6
ADD16	(%ecx)	(%eax)	%ebx
SP_STORE	%ecx
INC16	%ecx	$69
SP_STORE	%eax
INC16	%eax	$62
CPY16	(%ecx)	%eax
SP_RD16	%ecx	$60
SP_STORE	%eax
INC16	%eax	$69
CPY16	(%ecx)	(%eax)
SP_STORE	%ecx
INC16	%ecx	$71
SP_STORE	%eax
INC16	%eax	$2
CPY16	(%ecx)	%eax
SP_RD16	%eax	$71
CPY16	%r3	(%eax)
INC16	%r3	$4
CPY16	%r3	(%r3)
SP_RD16	%eax	$58
PUSH16	%eax
PUSH16	%r3
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$77
SP_INC	$4
CPY16	%r0	%r0
LD8	(%r0)	$33
LD8	(%r1)	$34
SP_RD16	%ecx	$56
LD16	(%ecx)	$0
SP_RD16	%eax	$71
CPY16	%r0	(%eax)
INC16	%r0	$4
CPY16	%r0	(%r0)
SP_RD16	%eax	$58
PUSH16	%eax
PUSH16	%r0
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$78
SP_INC	$4
SP_RD16	%eax	$71
CPY16	%r0	(%eax)
INC16	%r0	$4
CPY16	%r0	(%r0)
SP_STORE	%r3
INC16	%r3	$75
PUSH16	%r3
SP_RD32	%eax	$44
PUSH32	%eax
PUSH16	%r0
SP_DEC	$1
CALL	cdcGetSerialState
POP8	%eax
SP_WR8	%eax	$99
SP_INC	$8
LD8	(%r1)	$34
LD16	(%r2)	$3
SP_RD16	%eax	$71
CPY16	%r0	(%eax)
INC16	%r0	$4
CPY16	%r0	(%r0)
SP_RD16	%eax	$58
PUSH16	%eax
PUSH16	%r0
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$96
SP_INC	$4
SP_RD16	%eax	$71
CPY16	%r0	(%eax)
INC16	%r0	$4
CPY16	%r0	(%r0)
PUSH16	%r3
SP_RD32	%eax	$44
PUSH32	%eax
PUSH16	%r0
SP_DEC	$1
CALL	cdcGetSerialState
POP8	%eax
SP_WR8	%eax	$101
SP_INC	$8
LD8	(%r1)	$34
LD16	(%r2)	$3
SP_RD16	%ecx	$56
LD16	(%ecx)	$7
SP_RD16	%ecx	$60
SP_STORE	%eax
INC16	%eax	$69
CPY16	(%ecx)	(%eax)
SP_RD16	%eax	$71
CPY16	%r0	(%eax)
INC16	%r0	$4
CPY16	%r0	(%r0)
SP_RD16	%eax	$58
PUSH16	%eax
PUSH16	%r0
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$98
SP_INC	$4
SP_RD16	%eax	$71
CPY16	%r0	(%eax)
INC16	%r0	$4
CPY16	%r0	(%r0)
PUSH16	%r3
SP_RD32	%eax	$44
PUSH32	%eax
PUSH16	%r0
SP_DEC	$1
CALL	cdcGetSerialState
POP8	%eax
SP_WR8	%eax	$103
SP_INC	$8
SP_STORE	%eax
INC16	%eax	$2
SP_STORE	%ecx
INC16	%ecx	$115
CPY16	(%ecx)	(%eax)
SP_INC	$96
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
@IC342:	
@IC338:	
SP_STORE	%eax
SP_STORE	%ecx
INC16	%ecx	$115
CPY16	(%ecx)	(%eax)
SP_INC	$96
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"cdcSetup"

cdcGetSerialState:	
.GLOBAL	 DO_NOT_EXPORT  "cdcGetSerialState"

.FUNCTION	"cdcGetSerialState"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_DEC	$32
SP_RD16	%ecx	$52
SP_RD32	%ecx	$54
SP_WR32	%ecx	$54
SP_RD16	%r3	$58
SP_STORE	%r0
INC16	%r0	$0
PUSH16	$0
PUSH16	%r0
CALL	vos_init_semaphore
SP_INC	$4
SP_STORE	%r1
INC16	%r1	$5
LD16	%r2	$7
ADD16	%r2	%r1
CPY16	(%r2)	%r3
LD16	%r2	$9
ADD16	%r2	%r1
LD16	(%r2)	$16
CPY16	%r2	%r1
SP_STORE	%eax
INC16	%eax	$54
CPY32	(%r2)	(%eax)
LD16	%r2	$4
ADD16	%r2	%r1
CPY16	(%r2)	%r0
LD16	%r0	$6
ADD16	%r0	%r1
LD8	(%r0)	$15
LD16	%r2	$11
ADD16	%r2	%r1
LD8	(%r2)	$96
CPY16	%r1	%r1
PUSH16	$0
PUSH16	$14
PUSH16	%r1
SP_RD16	%eax	$58
PUSH16	%eax
SP_DEC	$1
CALL	vos_dev_read
POP8	%eax
SP_WR8	%eax	$27
SP_INC	$8
SP_STORE	%r1
INC16	%r1	$20
CPY16	%r2	%r1
LD8	(%r2)	$72
INC16	%r1	$2
CPY16	%r1	%r1
SP_STORE	%eax
INC16	%eax	$54
CPY32	(%r1)	(%eax)
PUSH16	$100
SP_DEC	$1
CALL	vos_delay_msecs
POP8	%eax
SP_WR8	%eax	$32
SP_INC	$2
CPY8	%r0	(%r0)
CMP8	%r0	$15
JNZ	@IC345
@IC346:	
SP_STORE	%r0
INC16	%r0	$20
PUSH16	%r0
SP_RD16	%eax	$54
PUSH16	%eax
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$35
SP_INC	$4
SP_STORE	%r0
INC16	%r0	$5
INC16	%r0	$9
LD16	(%r0)	$0
@IC345:	
SP_STORE	%r0
INC16	%r0	$5
INC16	%r0	$9
CPY16	%r0	(%r0)
SP_STORE	%eax
INC16	%eax	$51
CPY8	(%eax)	%r0
SP_INC	$32
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"cdcGetSerialState"

msgOnline:	
.GLOBAL	 DO_NOT_EXPORT  "msgOnline"

.FUNCTION	"msgOnline"	
PUSH32	%r0
PUSH32	%r1
LD32	%r0	$Str@2
CPY16	%r0	%r0
LD32	%r1	$Str@3
CPY16	%r1	%r1
PUSH16	%r0
CALL	monWriteString
SP_INC	$2
CALL	monVersion
PUSH16	%r1
CALL	monWriteString
SP_INC	$2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"msgOnline"

