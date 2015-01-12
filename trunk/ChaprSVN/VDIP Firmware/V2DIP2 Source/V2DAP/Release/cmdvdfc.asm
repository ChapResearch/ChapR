.DATA

.WEAK	"%eax"
.WEAK	"%ebx"
.WEAK	"%ecx"
.WEAK	"%r0"
.WEAK	"%r1"
.WEAK	"%r2"
.WEAK	"%r3"
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

.WEAK	"vos_gpio_enable_int"

.WEAK	"monReadCmd"

.WEAK	"vos_signal_cond_var"

.WEAK	"monLedsOff"

.WEAK	"monLedsRed"

cmd_a:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_a"

.FUNCTION	"cmd_a"	
CALL	monReadCr
LD8	%eax	$0
SP_WR8	%eax	$3
RTS	
.FUNC_END	"cmd_a"

cmd_b:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_b"

.FUNCTION	"cmd_b"	
CALL	monReadCr
LD8	%eax	$0
SP_WR8	%eax	$3
RTS	
.FUNC_END	"cmd_b"

cmd_ims:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_ims"

.FUNCTION	"cmd_ims"	
CALL	monReadCr
LD8	%eax	$0
SP_WR8	%eax	$3
RTS	
.FUNC_END	"cmd_ims"

cmd_imf:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_imf"

.FUNCTION	"cmd_imf"	
CALL	monReadCr
LD8	%eax	$0
SP_WR8	%eax	$3
RTS	
.FUNC_END	"cmd_imf"

cmd_cps:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_cps"

.FUNCTION	"cmd_cps"	
CALL	monReadCr
LD8	%eax	$0
SP_WR8	%eax	$3
RTS	
.FUNC_END	"cmd_cps"

cmd_cpf:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_cpf"

.FUNCTION	"cmd_cpf"	
CALL	monReadCr
LD8	%eax	$0
SP_WR8	%eax	$3
RTS	
.FUNC_END	"cmd_cpf"

