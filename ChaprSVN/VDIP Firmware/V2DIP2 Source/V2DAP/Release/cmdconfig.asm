.DATA

.WEAK	"%eax"
.WEAK	"%ebx"
.WEAK	"%ecx"
.WEAK	"%r0"
.WEAK	"%r1"
.WEAK	"%r2"
.WEAK	"%r3"
Str@0	.ASCIIZ	"\rMAIN "
Str@1	.ASCIIZ	"\rRPRG 1.00R\r"
.WEAK	"fatContext"
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

Array@2	.DB	8	0, 4, 2, 1, 3, 5, 6, 7

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

.WEAK	"diskResetDisk"

.WEAK	"fat_dirIsRoot"

.WEAK	"vos_gpio_set_port_mode"

.WEAK	"fat_fileFlush"

.WEAK	"vos_iomux_define_input"

.WEAK	"fat_fileWrite"

.WEAK	"vos_disable_interrupts"

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

.WEAK	"vos_set_clock_frequency"

.WEAK	"fat_fileRewind"

.WEAK	"diskRemoveDisk"

.WEAK	"monDirNotEmpty"

.WEAK	"vos_dma_enable"

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

.WEAK	"cmd_a"

.WEAK	"cmd_b"

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

.WEAK	"cmd_cd"

.WEAK	"monSlaveDisabled"

.WEAK	"cmd_qd"

.WEAK	"cmd_sc"

.WEAK	"cmd_rd"

.WEAK	"cmd_vb"

.WEAK	"vos_gpio_wait_on_all_ints"

.WEAK	"cmd_sf"

.WEAK	"cmd_fs"

.WEAK	"monDeviceRemoved"

.WEAK	"fat_dirEntryName"

.WEAK	"cmd_vf"

.WEAK	"cmd_vp"

.WEAK	"fat_dirEntryTime"

.WEAK	"fat_fileTruncate"

.WEAK	"fat_dirEntrySize"

.WEAK	"vos_delay_cancel"

.WEAK	"cmd_v3a"

.WEAK	"cmd_qp1"

.WEAK	"cmd_qp2"

.WEAK	"monLedsGreenFlash"

.WEAK	"vos_dma_retained_configure"

.WEAK	"monDeviceDetected"

.WEAK	"cmd_fbd"

.WEAK	"cmd_fgb"

.WEAK	"cmd_ffc"

.WEAK	"fat_dirDirIsEmpty"

.WEAK	"cmd_idd"

.WEAK	"monRead"

.WEAK	"cmd_dld"

.WEAK	"cmd_clf"

.WEAK	"cmd_fmc"

.WEAK	"cmd_dlf"

.WEAK	"cmd_cpf"

.WEAK	"cmd_fgm"

.WEAK	"cmd_drd"

.WEAK	"cmd_fsb"

.WEAK	"cmd_dsd"

.WEAK	"cmd_imf"

.WEAK	"cmd_rdf"

.WEAK	"cmd_mkd"

.WEAK	"cmd_fsd"

.WEAK	"cmd_fse"

.WEAK	"cmd_dir"

.WEAK	"cmd_dri"

.WEAK	"vos_unlock_mutex"

.WEAK	"cmd_sek"

.WEAK	"cmd_fsl"

.WEAK	"cmd_dsn"

.WEAK	"cmd_ren"

.WEAK	"cmd_cps"

.WEAK	"cmd_wkd"

.WEAK	"cmd_dvl"

.WEAK	"cmd_ims"

.WEAK	"cmd_vra"

.WEAK	"cmd_pgs"

.WEAK	"monValidateNumber"

.WEAK	"cmd_vsb"

.WEAK	"monInit"

.WEAK	"cmd_vrd"

.WEAK	"cmd_vpf"

.WEAK	"cmd_sud"

.WEAK	"cmd_vsd"

.WEAK	"cmd_vrf"

.WEAK	"cmd_vsf"

.WEAK	"cmd_wrf"

.WEAK	"cmd_opr"

.WEAK	"cmd_psr"

.WEAK	"cmd_sum"

.WEAK	"cmd_opw"

.WEAK	"cmd_qss"

.WEAK	"cmd_vrr"

.WEAK	"cmd_ssu"

.WEAK	"cmd_vst"

.WEAK	"cmd_vsv"

.WEAK	"cmd_vwr"

.WEAK	"vos_gpio_read_all"

.WEAK	"vos_create_thread"

.WEAK	"vos_gpio_read_pin"

.WEAK	"vos_dma_configure"

.WEAK	"vos_init_cond_var"

.WEAK	"vos_wait_cond_var"

.WEAK	"fat_dirEntryIsFile"

.WEAK	"monCheck"

.WEAK	"cmd_idde"

.WEAK	"monFilenameInvalid"

.WEAK	"fat_getVolumeLabel"

.WEAK	"diskInit"

.WEAK	"vos_stop_profiler"

.WEAK	"cmd_dirt"

.WEAK	"fat_time"

.WEAK	"fat_open"

.WEAK	"fat_init"

.WEAK	"vos_trylock_mutex"

.WEAK	"monWrite"

.WEAK	"fat_bytesPerSector"

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

cmd_cr:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_cr"

.FUNCTION	"cmd_cr"	
LD8	%eax	$0
SP_WR8	%eax	$3
RTS	
.FUNC_END	"cmd_cr"

cmd_fwv:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_fwv"

.FUNCTION	"cmd_fwv"	
PUSH32	%r0
PUSH32	%r1
SP_DEC	$2
LD32	%r0	$Str@0
CPY16	%r0	%r0
LD32	%r1	$Str@1
CPY16	%r1	%r1
CALL	monReadCr
PUSH16	$6
PUSH16	%r0
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$4
SP_INC	$4
CALL	monVersion
PUSH16	$12
PUSH16	%r1
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$5
SP_INC	$4
LD8	%eax	$0
SP_WR8	%eax	$13
SP_INC	$2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"cmd_fwv"

cmd_echo:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_echo"

.FUNCTION	"cmd_echo"	
PUSH32	%r0
SP_DEC	$3
LD8	%ecx	$13
SP_WR8	%ecx	$0
CALL	monReadCr
LD16	%r0	$cmdData
CPY16	%r0	%r0
PUSH16	$1
PUSH16	%r0
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$5
SP_INC	$4
SP_STORE	%r0
PUSH16	$1
PUSH16	%r0
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$6
SP_INC	$4
LD8	%eax	$1
SP_WR8	%eax	$10
SP_INC	$3
POP32	%r0
RTS	
.FUNC_END	"cmd_echo"

cmd_scs:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_scs"

.FUNCTION	"cmd_scs"	
PUSH32	%r0
CALL	monReadCr
CPY8	%eax	Mon_Flags
AND32	%eax	$255
LD32	%r0	$1
OR32	%r0	%eax
CPY8	Mon_Flags	%r0
LD8	%eax	$0
SP_WR8	%eax	$7
POP32	%r0
RTS	
.FUNC_END	"cmd_scs"

cmd_ecs:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_ecs"

.FUNCTION	"cmd_ecs"	
PUSH32	%r0
CALL	monReadCr
CPY8	%eax	Mon_Flags
AND32	%eax	$255
LD32	%r0	$-2
AND32	%r0	%eax
CPY8	Mon_Flags	%r0
LD8	%eax	$0
SP_WR8	%eax	$7
POP32	%r0
RTS	
.FUNC_END	"cmd_ecs"

cmd_ipa:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_ipa"

.FUNCTION	"cmd_ipa"	
PUSH32	%r0
CALL	monReadCr
CPY8	%eax	Mon_Flags
AND32	%eax	$255
LD32	%r0	$2
OR32	%r0	%eax
CPY8	Mon_Flags	%r0
LD8	%eax	$0
SP_WR8	%eax	$7
POP32	%r0
RTS	
.FUNC_END	"cmd_ipa"

cmd_iph:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_iph"

.FUNCTION	"cmd_iph"	
PUSH32	%r0
CALL	monReadCr
CPY8	%eax	Mon_Flags
AND32	%eax	$255
LD32	%r0	$-3
AND32	%r0	%eax
CPY8	Mon_Flags	%r0
LD8	%eax	$0
SP_WR8	%eax	$7
POP32	%r0
RTS	
.FUNC_END	"cmd_iph"

cmd_sbd:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_sbd"

.FUNCTION	"cmd_sbd"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_DEC	$26
SP_STORE	%ecx
LD32	%eax	$Array@2
CPYROM	(%ecx)	%eax	$4
PUSH8	$3
PUSH8	$1
SP_DEC	$1
CALL	monReadNumber
POP8	%eax
SP_WR8	%eax	$10
SP_INC	$2
CALL	monReadCr
PUSH8	$3
PUSH8	$1
SP_DEC	$1
CALL	monValidateNumber
POP8	%eax
SP_WR8	%eax	$11
SP_INC	$2
CALL	monPrompt
LD32	%ebx	$16
SHR32	%r0	param1	%ebx
AND32	%r0	$255
CPY32	%r0	%r0
LD32	%r1	$16128
AND32	%r1	param1
OR32	%r1	%r0	%r1
CPY32	%r0	%r1
CMP32	%r0	$0
JNZ	@IC2
@IC3:	
LD32	%r3	$3000000
JUMP	@IC1
@IC2:	
CMP32	%r0	$1
JNZ	@IC7
@IC8:	
LD32	%r3	$2000000
JUMP	@IC6
@IC7:	
CMP32	%r0	$2
JNZ	@IC12
@IC13:	
LD32	%r3	$1500000
JUMP	@IC11
@IC12:	
LD32	%ebx	$3
SHL32	%r1	%r0	%ebx
SP_STORE	%ecx
INC16	%ecx	$10
CPY32	(%ecx)	%r1
LD32	%ebx	$14
SHR32	%r1	param1	%ebx
AND32	%r1	$3
SP_STORE	%r2
INC16	%r2	$0
ADD16	%r1	%r2
CPY8	%r1	(%r1)
SP_STORE	%eax
INC16	%eax	$10
CPY8	%ebx	%r1
AND32	%ebx	$255
ADD32	%r1	(%eax)	%ebx
SP_STORE	%ecx
INC16	%ecx	$10
CPY32	(%ecx)	%r1
LD32	%eax	$24000000
DIV32	%r1	%eax	%r1
CPY32	%r3	%r1
@IC11:	
@IC6:	
@IC1:	
PUSH16	$100
SP_DEC	$1
CALL	vos_delay_msecs
POP8	%eax
SP_WR8	%eax	$16
SP_INC	$2
SP_STORE	%r1
INC16	%r1	$15
CPY16	%r2	%r1
LD8	(%r2)	$34
LD16	%r2	$1
ADD16	%r2	%r1
CPY16	%r2	%r2
CPY32	(%r2)	%r3
PUSH16	%r1
PUSH16	hMonitor
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$28
SP_INC	$4
PUSH16	$100
SP_DEC	$1
CALL	vos_delay_msecs
POP8	%eax
SP_WR8	%eax	$27
SP_INC	$2
LD8	%eax	$0
SP_WR8	%eax	$45
SP_INC	$26
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"cmd_sbd"

cmd_sd:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_sd"

.FUNCTION	"cmd_sd"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_DEC	$255
SP_DEC	$255
SP_DEC	$42
LD16	%ecx	$0
SP_WR16	%ecx	$0
LD8	%ecx	$3
SP_WR8	%ecx	$2
PUSH8	$4
PUSH8	$1
SP_DEC	$1
CALL	monReadNumber
POP8	%eax
SP_WR8	%eax	$5
SP_INC	$2
CALL	monReadCr
PUSH8	$4
PUSH8	$1
SP_DEC	$1
CALL	monValidateNumber
POP8	%eax
SP_WR8	%eax	$6
SP_INC	$2
SP_RD8	%ecx	$4
SP_WR8	%ecx	$2
SP_RD8	%ecx	$2
CMP8	%ecx	$0
JNZ	@IC16
@IC17:	
SP_STORE	%r0
INC16	%r0	$5
PUSH16	$0
PUSH16	%r0
CALL	vos_init_semaphore
SP_INC	$4
SP_STORE	%r1
INC16	%r1	$10
LD16	%r2	$6
ADD16	%r2	%r1
SP_STORE	%r3
INC16	%r3	$38
CPY16	(%r2)	%r3
LD16	%r2	$8
ADD16	%r2	%r1
LD16	(%r2)	$512
LD16	%r2	$10
ADD16	%r2	%r1
LD16	(%r2)	$512
LD16	%r2	$4
ADD16	%r2	%r1
CPY16	(%r2)	%r0
LD16	%r0	$12
ADD16	%r0	%r1
LD8	(%r0)	$7
CPY16	%r0	%r1
CPY32	(%r0)	param1
LD16	%r0	$13
ADD16	%r0	%r1
LD8	(%r0)	$5
CPY16	%r0	%r1
PUSH16	$0
PUSH16	$28
PUSH16	%r0
PUSH16	hBoms
SP_DEC	$1
CALL	vos_dev_read
SP_STORE	%eax
ADD16	%eax	$559
POP8	(%eax)
SP_INC	$8
SP_STORE	%ecx
ADD16	%ecx	$550
CMP8	(%ecx)	$0
JZ	@IC20
@IC21:	
LD8	%ecx	$3
SP_WR8	%ecx	$2
@IC20:	
@IC16:	
SP_STORE	%r0
INC16	%r0	$38
SP_STORE	%r1
PUSH16	%r1
PUSH16	$512
PUSH16	%r0
PUSH16	hMonitor
SP_DEC	$1
CALL	vos_dev_write
SP_STORE	%eax
ADD16	%eax	$560
POP8	(%eax)
SP_INC	$8
SP_STORE	%eax
INC16	%eax	$2
SP_STORE	%ecx
ADD16	%ecx	$571
CPY8	(%ecx)	(%eax)
SP_INC	$255
SP_INC	$255
SP_INC	$42
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"cmd_sd"

cmd_sw:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_sw"

.FUNCTION	"cmd_sw"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_DEC	$255
SP_DEC	$255
SP_DEC	$42
LD16	%ecx	$0
SP_WR16	%ecx	$0
LD8	%ecx	$3
SP_WR8	%ecx	$2
PUSH8	$4
PUSH8	$1
SP_DEC	$1
CALL	monReadNumber
POP8	%eax
SP_WR8	%eax	$5
SP_INC	$2
CALL	monReadCr
PUSH8	$4
PUSH8	$1
SP_DEC	$1
CALL	monValidateNumber
POP8	%eax
SP_WR8	%eax	$6
SP_INC	$2
SP_RD8	%r0	$4
SP_STORE	%r1
ADD16	%r1	$5
SP_STORE	%r2
PUSH16	%r2
PUSH16	$512
PUSH16	%r1
PUSH16	hMonitor
SP_DEC	$1
CALL	vos_dev_read
SP_STORE	%eax
ADD16	%eax	$526
POP8	(%eax)
SP_INC	$8
CMP8	%r0	$0
JNZ	@IC24
@IC25:	
SP_STORE	%r0
ADD16	%r0	$518
PUSH16	$0
PUSH16	%r0
CALL	vos_init_semaphore
SP_INC	$4
SP_STORE	%r1
ADD16	%r1	$523
LD16	%r2	$6
ADD16	%r2	%r1
SP_STORE	%r3
INC16	%r3	$5
CPY16	(%r2)	%r3
LD16	%r2	$8
ADD16	%r2	%r1
LD16	(%r2)	$512
LD16	%r2	$10
ADD16	%r2	%r1
LD16	(%r2)	$512
LD16	%r2	$4
ADD16	%r2	%r1
CPY16	(%r2)	%r0
LD16	%r0	$12
ADD16	%r0	%r1
LD8	(%r0)	$7
CPY16	%r0	%r1
CPY32	(%r0)	param1
LD16	%r0	$13
ADD16	%r0	%r1
LD8	(%r0)	$5
CPY16	%r0	%r1
PUSH16	$0
PUSH16	$28
PUSH16	%r0
PUSH16	hBoms
SP_DEC	$1
CALL	vos_dev_write
SP_STORE	%eax
ADD16	%eax	$560
POP8	(%eax)
SP_INC	$8
SP_STORE	%ecx
ADD16	%ecx	$551
CMP8	(%ecx)	$0
JZ	@IC28
@IC29:	
LD8	%ecx	$3
SP_WR8	%ecx	$2
@IC28:	
@IC24:	
SP_STORE	%eax
ADD16	%eax	$571
LD8	(%eax)	$0
SP_INC	$255
SP_INC	$255
SP_INC	$42
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"cmd_sw"

cmd_fwu:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_fwu"

.FUNCTION	"cmd_fwu"	
PUSH32	%r0
SP_DEC	$2
PUSH8	$1
SP_DEC	$1
CALL	monReadFileName
POP8	%eax
SP_WR8	%eax	$1
SP_INC	$1
CALL	monReadCr
PUSH8	$1
SP_DEC	$1
CALL	monValidateFileName
POP8	%eax
SP_WR8	%eax	$2
SP_INC	$1
SP_RD8	%r0	$1
CMP8	%r0	$0
JNZ	@IC32
@IC33:	
CALL	diskCheckFwu
@IC32:	
SP_STORE	%eax
INC16	%eax	$9
CPY8	(%eax)	%r0
SP_INC	$2
POP32	%r0
RTS	
.FUNC_END	"cmd_fwu"

cmd_ior:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_ior"

.FUNCTION	"cmd_ior"	
PUSH32	%r0
SP_DEC	$9
LD8	%ecx	$13
SP_WR8	%ecx	$0
PUSH8	$1
PUSH8	$1
SP_DEC	$1
CALL	monReadNumber
POP8	%eax
SP_WR8	%eax	$3
SP_INC	$2
CALL	monReadCr
PUSH8	$1
PUSH8	$1
SP_DEC	$1
CALL	monValidateNumber
POP8	%eax
SP_WR8	%eax	$4
SP_INC	$2
JUMP	@IC38
@IC37:	
SP_STORE	%r0
INC16	%r0	$3
PUSH16	%r0
PUSH8	$2
SP_DEC	$1
CALL	vos_gpio_read_port
POP8	%eax
SP_WR8	%eax	$7
SP_INC	$3
JUMP	@IC36
@IC39:	
SP_STORE	%r0
INC16	%r0	$3
PUSH16	%r0
PUSH8	$3
SP_DEC	$1
CALL	vos_gpio_read_port
POP8	%eax
SP_WR8	%eax	$8
SP_INC	$3
JUMP	@IC36
@IC41:	
SP_STORE	%r0
INC16	%r0	$3
PUSH16	%r0
PUSH8	$0
SP_DEC	$1
CALL	vos_gpio_read_port
POP8	%eax
SP_WR8	%eax	$9
SP_INC	$3
JUMP	@IC36
@IC43:	
SP_STORE	%r0
INC16	%r0	$3
PUSH16	%r0
PUSH8	$1
SP_DEC	$1
CALL	vos_gpio_read_port
POP8	%eax
SP_WR8	%eax	$10
SP_INC	$3
JUMP	@IC36
@IC38:	
CMP32	param1	$0
JZ	@IC37
@IC40:	
CMP32	param1	$1
JZ	@IC39
@IC42:	
CMP32	param1	$2
JZ	@IC41
@IC44:	
JUMP	@IC43
@IC36:	
SP_STORE	%r0
INC16	%r0	$3
PUSH8	$1
PUSH16	%r0
CALL	monAddNumberToConsole
SP_INC	$3
SP_STORE	%r0
PUSH16	$1
PUSH16	%r0
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$12
SP_INC	$4
LD8	%eax	$0
SP_WR8	%eax	$16
SP_INC	$9
POP32	%r0
RTS	
.FUNC_END	"cmd_ior"

cmd_iow:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_iow"

.FUNCTION	"cmd_iow"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
SP_DEC	$10
PUSH8	$3
PUSH8	$1
SP_DEC	$1
CALL	monReadNumber
POP8	%eax
SP_WR8	%eax	$2
SP_INC	$2
CALL	monReadCr
PUSH8	$3
PUSH8	$1
SP_DEC	$1
CALL	monValidateNumber
POP8	%eax
SP_WR8	%eax	$3
SP_INC	$2
LD32	%r0	$16711680
AND32	%r0	param1
LD32	%ebx	$16
SHR32	%r0	%ebx
CPY8	%r0	%r0
LD32	%r1	$65280
AND32	%r1	param1
LD32	%ebx	$8
SHR32	%r1	%ebx
CPY8	%r1	%r1
LD32	%r2	$255
AND32	%r2	param1
CPY8	%r2	%r2
JUMP	@IC47
@IC46:	
PUSH8	%r1
PUSH8	$2
SP_DEC	$1
CALL	vos_gpio_set_port_mode
POP8	%eax
SP_WR8	%eax	$4
SP_INC	$2
PUSH8	%r2
PUSH8	$2
SP_DEC	$1
CALL	vos_gpio_write_port
POP8	%eax
SP_WR8	%eax	$5
SP_INC	$2
JUMP	@IC45
@IC48:	
PUSH8	%r1
PUSH8	$3
SP_DEC	$1
CALL	vos_gpio_set_port_mode
POP8	%eax
SP_WR8	%eax	$6
SP_INC	$2
PUSH8	%r2
PUSH8	$3
SP_DEC	$1
CALL	vos_gpio_write_port
POP8	%eax
SP_WR8	%eax	$7
SP_INC	$2
JUMP	@IC45
@IC50:	
PUSH8	%r1
PUSH8	$0
SP_DEC	$1
CALL	vos_gpio_set_port_mode
POP8	%eax
SP_WR8	%eax	$8
SP_INC	$2
PUSH8	%r2
PUSH8	$0
SP_DEC	$1
CALL	vos_gpio_write_port
POP8	%eax
SP_WR8	%eax	$9
SP_INC	$2
JUMP	@IC45
@IC52:	
PUSH8	%r1
PUSH8	$1
SP_DEC	$1
CALL	vos_gpio_set_port_mode
POP8	%eax
SP_WR8	%eax	$10
SP_INC	$2
PUSH8	%r2
PUSH8	$1
SP_DEC	$1
CALL	vos_gpio_write_port
POP8	%eax
SP_WR8	%eax	$11
SP_INC	$2
JUMP	@IC45
@IC47:	
CMP8	%r0	$0
JZ	@IC46
@IC49:	
CMP8	%r0	$1
JZ	@IC48
@IC51:	
CMP8	%r0	$2
JZ	@IC50
@IC53:	
JUMP	@IC52
@IC45:	
LD8	%eax	$0
SP_WR8	%eax	$25
SP_INC	$10
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"cmd_iow"

cmd_ini:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_ini"

.FUNCTION	"cmd_ini"	
SP_DEC	$4
LD32	%ecx	$960
SP_WR32	%ecx	$0
SP_RD32	%ecx	$0
CALL	(%ecx)
LD8	%eax	$0
SP_WR8	%eax	$7
SP_INC	$4
RTS	
.FUNC_END	"cmd_ini"

