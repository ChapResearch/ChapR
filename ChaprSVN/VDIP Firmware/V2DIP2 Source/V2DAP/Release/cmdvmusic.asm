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

cmd_vpf:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_vpf"

.FUNCTION	"cmd_vpf"	
CALL	monReadCr
LD8	%eax	$2
SP_WR8	%eax	$3
RTS	
.FUNC_END	"cmd_vpf"

cmd_vrf:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_vrf"

.FUNCTION	"cmd_vrf"	
CALL	monReadCr
LD8	%eax	$2
SP_WR8	%eax	$3
RTS	
.FUNC_END	"cmd_vrf"

cmd_vst:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_vst"

.FUNCTION	"cmd_vst"	
CALL	monReadCr
LD8	%eax	$2
SP_WR8	%eax	$3
RTS	
.FUNC_END	"cmd_vst"

cmd_vp:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_vp"

.FUNCTION	"cmd_vp"	
CALL	monReadCr
LD8	%eax	$2
SP_WR8	%eax	$3
RTS	
.FUNC_END	"cmd_vp"

cmd_v3a:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_v3a"

.FUNCTION	"cmd_v3a"	
CALL	monReadCr
LD8	%eax	$2
SP_WR8	%eax	$3
RTS	
.FUNC_END	"cmd_v3a"

cmd_vra:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_vra"

.FUNCTION	"cmd_vra"	
CALL	monReadCr
LD8	%eax	$2
SP_WR8	%eax	$3
RTS	
.FUNC_END	"cmd_vra"

cmd_vrr:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_vrr"

.FUNCTION	"cmd_vrr"	
CALL	monReadCr
LD8	%eax	$2
SP_WR8	%eax	$3
RTS	
.FUNC_END	"cmd_vrr"

cmd_vsf:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_vsf"

.FUNCTION	"cmd_vsf"	
CALL	monReadCr
LD8	%eax	$2
SP_WR8	%eax	$3
RTS	
.FUNC_END	"cmd_vsf"

cmd_vsb:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_vsb"

.FUNCTION	"cmd_vsb"	
CALL	monReadCr
LD8	%eax	$2
SP_WR8	%eax	$3
RTS	
.FUNC_END	"cmd_vsb"

cmd_vsd:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_vsd"

.FUNCTION	"cmd_vsd"	
CALL	monReadCr
LD8	%eax	$2
SP_WR8	%eax	$3
RTS	
.FUNC_END	"cmd_vsd"

cmd_vf:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_vf"

.FUNCTION	"cmd_vf"	
CALL	monReadCr
LD8	%eax	$2
SP_WR8	%eax	$3
RTS	
.FUNC_END	"cmd_vf"

cmd_vb:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_vb"

.FUNCTION	"cmd_vb"	
CALL	monReadCr
LD8	%eax	$2
SP_WR8	%eax	$3
RTS	
.FUNC_END	"cmd_vb"

cmd_vrd:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_vrd"

.FUNCTION	"cmd_vrd"	
CALL	monReadCr
LD8	%eax	$2
SP_WR8	%eax	$3
RTS	
.FUNC_END	"cmd_vrd"

cmd_vwr:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_vwr"

.FUNCTION	"cmd_vwr"	
CALL	monReadCr
LD8	%eax	$2
SP_WR8	%eax	$3
RTS	
.FUNC_END	"cmd_vwr"

cmd_vsv:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_vsv"

.FUNCTION	"cmd_vsv"	
CALL	monReadCr
LD8	%eax	$2
SP_WR8	%eax	$3
RTS	
.FUNC_END	"cmd_vsv"

