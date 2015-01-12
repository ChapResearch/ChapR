.DATA

.WEAK	"%eax"
.WEAK	"%ebx"
.WEAK	"%ecx"
.WEAK	"%r0"
.WEAK	"%r1"
.WEAK	"%r2"
.WEAK	"%r3"
FILE	.DB	67	?
.GLOBAL	  DO_NOT_EXPORT "FILE"
fileState	.DB	1	?
.GLOBAL	  DO_NOT_EXPORT "fileState"
Str@0	.ASCIIZ	" DIR"
Str@1	.ASCIIZ	"."
Str@2	.ASCIIZ	".."
Str@3	.ASCIIZ	"\rUSB VID = $"
Str@4	.ASCIIZ	"\rUSB PID = $"
Str@5	.ASCIIZ	"\rVendor Id = "
Str@6	.ASCIIZ	"\rProduct Id = "
Str@7	.ASCIIZ	"\rRevision Level = "
Str@8	.ASCIIZ	"\rI/F = SCSI"
Str@9	.ASCIIZ	"\rFAT32"
Str@10	.ASCIIZ	"\rFAT16"
Str@11	.ASCIIZ	"\rBytes/Sector = $"
Str@12	.ASCIIZ	"\rBytes/Cluster = $"
Str@13	.ASCIIZ	"\rCapacity = $"
Str@14	.ASCIIZ	"\rFree Space = $"
Str@15	.ASCIIZ	" Bytes"
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

.WEAK	"fat_dirIsRoot"

.WEAK	"diskResetDisk"

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

.WEAK	"monSlaveDisabled"

.WEAK	"cmd_qd"

.WEAK	"cmd_cr"

.WEAK	"cmd_sc"

.WEAK	"cmd_sd"

.WEAK	"cmd_vb"

.WEAK	"vos_gpio_wait_on_all_ints"

.WEAK	"fat_dirEntryName"

.WEAK	"cmd_sf"

.WEAK	"monDeviceRemoved"

.WEAK	"cmd_vf"

.WEAK	"cmd_vp"

.WEAK	"fat_dirEntryTime"

.WEAK	"cmd_sw"

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

.WEAK	"monRead"

.WEAK	"cmd_fmc"

.WEAK	"cmd_cpf"

.WEAK	"cmd_sbd"

.WEAK	"cmd_fgm"

.WEAK	"cmd_drd"

.WEAK	"cmd_ipa"

.WEAK	"cmd_fsb"

.WEAK	"cmd_dsd"

.WEAK	"cmd_ecs"

.WEAK	"cmd_imf"

.WEAK	"cmd_fsd"

.WEAK	"cmd_ini"

.WEAK	"cmd_iph"

.WEAK	"vos_unlock_mutex"

.WEAK	"cmd_fsl"

.WEAK	"cmd_cps"

.WEAK	"cmd_wkd"

.WEAK	"cmd_ims"

.WEAK	"cmd_vra"

.WEAK	"cmd_scs"

.WEAK	"cmd_pgs"

.WEAK	"cmd_ior"

.WEAK	"monValidateNumber"

.WEAK	"cmd_vsb"

.WEAK	"monInit"

.WEAK	"cmd_vrd"

.WEAK	"cmd_vpf"

.WEAK	"cmd_sud"

.WEAK	"cmd_vsd"

.WEAK	"cmd_vrf"

.WEAK	"cmd_vsf"

.WEAK	"cmd_iow"

.WEAK	"cmd_fwu"

.WEAK	"cmd_fwv"

.WEAK	"cmd_psr"

.WEAK	"cmd_sum"

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

.WEAK	"fat_getVolumeLabel"

.WEAK	"cmd_echo"

.WEAK	"monFilenameInvalid"

.WEAK	"vos_stop_profiler"

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

.WEAK	"boms_init"

.WEAK	"fat_bytesPerCluster"

.WEAK	"monPrompt"

.WEAK	"vos_gpio_enable_int"

.WEAK	"monReadCmd"

.WEAK	"vos_signal_cond_var"

.WEAK	"fat_dirTableFindNext"

.WEAK	"monLedsOff"

.WEAK	"monLedsRed"

diskInit:	
.GLOBAL	 DO_NOT_EXPORT  "diskInit"

.FUNCTION	"diskInit"	
LD8	fileState	$0
RTS	
.FUNC_END	"diskInit"

cmd_dir:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_dir"

.FUNCTION	"cmd_dir"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_DEC	$94
LD8	%ecx	$13
SP_WR8	%ecx	$0
LD8	%ecx	$32
SP_WR8	%ecx	$1
LD32	%r0	$Str@0
CPY16	%r0	%r0
CMP8	cmdTerminator	$32
JNZ	@IC2
@IC3:	
PUSH8	$1
SP_DEC	$1
CALL	monReadFileName
POP8	%eax
SP_WR8	%eax	$3
SP_INC	$1
CALL	monReadCr
SP_STORE	%r1
PUSH16	$1
PUSH16	%r1
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$7
SP_INC	$4
CMP8	fileState	$2
JNZ	@IC6
@IC7:	
LD8	%eax	$7
SP_WR8	%eax	$113
SP_INC	$94
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
@IC6:	
PUSH8	$1
SP_DEC	$1
CALL	monValidateFileName
POP8	%eax
SP_WR8	%eax	$5
SP_INC	$1
SP_RD8	%r3	$4
CMP8	%r3	$0
JNZ	@IC10
@IC11:	
SP_STORE	%r1
INC16	%r1	$5
LD16	%r2	$param1Data
CPY16	%r2	%r2
PUSH16	%r2
PUSH16	%r1
PUSH16	fatContext
SP_DEC	$1
CALL	fat_dirTableFind
POP8	%eax
SP_WR8	%eax	$78
SP_INC	$6
SP_RD8	%ecx	$72
CMP8	%ecx	$0
JNZ	@IC15
@IC16:	
SP_STORE	%r1
INC16	%r1	$5
PUSH16	%r1
SP_DEC	$4
CALL	fat_dirEntrySize
POP32	%eax
SP_WR32	%eax	$75
SP_INC	$2
SP_RD32	%ecx	$73
SP_WR32	%ecx	$77
CPY16	%r1	%r1
PUSH16	%r1
SP_DEC	$1
CALL	monWriteFileName
POP8	%eax
SP_WR8	%eax	$83
SP_INC	$2
SP_STORE	%r1
INC16	%r1	$1
PUSH16	$1
PUSH16	%r1
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$86
SP_INC	$4
SP_STORE	%r1
INC16	%r1	$77
CPY16	%r1	%r1
PUSH8	$4
PUSH16	%r1
CALL	monAddNumberToConsole
SP_INC	$3
SP_STORE	%r1
PUSH16	$1
PUSH16	%r1
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$87
SP_INC	$4
JUMP	@IC14
@IC15:	
LD8	%r3	$3
@IC14:	
@IC10:	
JUMP	@IC1
@IC2:	
CALL	monReadCr
SP_STORE	%r1
PUSH16	$1
PUSH16	%r1
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$88
SP_INC	$4
CMP8	fileState	$2
JNZ	@IC19
@IC20:	
LD8	%eax	$7
SP_WR8	%eax	$113
SP_INC	$94
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
@IC19:	
PUSH16	fatContext
SP_DEC	$1
CALL	fat_dirIsRoot
POP8	%eax
SP_WR8	%eax	$87
SP_INC	$2
SP_RD8	%ecx	$85
CMP8	%ecx	$0
JNZ	@IC23
@IC24:	
LD32	%r1	$Str@1
PUSH16	%r1
CALL	monWriteString
SP_INC	$2
PUSH16	%r0
CALL	monWriteString
SP_INC	$2
SP_STORE	%r1
PUSH16	$1
PUSH16	%r1
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$90
SP_INC	$4
LD32	%r2	$Str@2
PUSH16	%r2
CALL	monWriteString
SP_INC	$2
PUSH16	%r0
CALL	monWriteString
SP_INC	$2
PUSH16	$1
PUSH16	%r1
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$91
SP_INC	$4
@IC23:	
SP_STORE	%r1
INC16	%r1	$5
PUSH16	%r1
PUSH16	fatContext
SP_DEC	$1
CALL	fat_dirTableFindFirst
POP8	%eax
SP_WR8	%eax	$92
SP_INC	$4
SP_RD8	%ecx	$88
CMP8	%ecx	$0
JNZ	@IC25
@IC26:	
@IC29:	
SP_STORE	%r1
INC16	%r1	$5
PUSH16	%r1
SP_DEC	$1
CALL	fat_dirEntryIsValid
POP8	%eax
SP_WR8	%eax	$91
SP_INC	$2
SP_RD8	%ecx	$89
CMP8	%ecx	$0
JZ	@IC32
@IC33:	
SP_STORE	%r1
INC16	%r1	$5
CPY16	%r2	%r1
PUSH16	%r2
SP_DEC	$1
CALL	monWriteFileName
POP8	%eax
SP_WR8	%eax	$92
SP_INC	$2
PUSH16	%r1
SP_DEC	$1
CALL	fat_dirEntryIsDirectory
POP8	%eax
SP_WR8	%eax	$93
SP_INC	$2
SP_RD8	%ecx	$91
CMP8	%ecx	$0
JZ	@IC34
@IC35:	
PUSH16	%r0
CALL	monWriteString
SP_INC	$2
@IC34:	
SP_STORE	%r1
PUSH16	$1
PUSH16	%r1
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$96
SP_INC	$4
@IC32:	
@IC30:	
SP_STORE	%r1
INC16	%r1	$5
PUSH16	%r1
PUSH16	fatContext
SP_DEC	$1
CALL	fat_dirTableFindNext
POP8	%eax
SP_WR8	%eax	$97
SP_INC	$4
SP_RD8	%ecx	$93
CMP8	%ecx	$0
JZ	@IC29
JNZ	@IC31
@IC31:	
@IC25:	
LD8	%r3	$0
@IC1:	
SP_STORE	%eax
INC16	%eax	$113
CPY8	(%eax)	%r3
SP_INC	$94
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"cmd_dir"

cmd_cd:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_cd"

.FUNCTION	"cmd_cd"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
SP_DEC	$73
PUSH8	$1
SP_DEC	$1
CALL	monReadFileName
POP8	%eax
SP_WR8	%eax	$1
SP_INC	$1
CALL	monReadCr
CMP8	fileState	$0
JZ	@IC39
@IC40:	
LD8	%r0	$7
JUMP	@IC38
@IC39:	
PUSH8	$1
SP_DEC	$1
CALL	monValidateFileName
POP8	%eax
SP_WR8	%eax	$2
SP_INC	$1
SP_RD8	%r0	$1
CMP8	%r0	$0
JNZ	@IC43
@IC44:	
LD16	%r1	$param1Data
CPY8	%r1	(%r1)
CMP8	%r1	$47
JNZ	@IC48
@IC49:	
PUSH16	$0
PUSH16	fatContext
SP_DEC	$1
CALL	fat_dirChangeDir
POP8	%eax
SP_WR8	%eax	$6
SP_INC	$4
SP_RD8	%ecx	$2
CMP8	%ecx	$0
JZ	@IC52
@IC53:	
LD8	%r0	$3
@IC52:	
JUMP	@IC47
@IC48:	
LD16	%r1	$param1Data
PUSH16	%r1
PUSH16	fatContext
SP_DEC	$1
CALL	fat_dirChangeDir
POP8	%eax
SP_WR8	%eax	$7
SP_INC	$4
SP_RD8	%ecx	$3
CMP8	%ecx	$0
JZ	@IC56
@IC57:	
LD8	%r0	$3
SP_STORE	%r1
INC16	%r1	$4
LD16	%r2	$param1Data
PUSH8	$0
PUSH16	%r2
PUSH16	%r1
PUSH16	fatContext
SP_DEC	$1
CALL	fat_fileOpen
POP8	%eax
SP_WR8	%eax	$78
SP_INC	$7
SP_RD8	%ecx	$71
CMP8	%ecx	$0
JNZ	@IC60
@IC61:	
SP_STORE	%r1
INC16	%r1	$4
PUSH16	%r1
SP_DEC	$1
CALL	fat_dirEntryIsDirectory
POP8	%eax
SP_WR8	%eax	$74
SP_INC	$2
SP_RD8	%ecx	$72
CMP8	%ecx	$0
JNZ	@IC64
@IC65:	
LD8	%r0	$5
@IC64:	
@IC60:	
@IC56:	
@IC47:	
@IC43:	
@IC38:	
SP_STORE	%eax
INC16	%eax	$88
CPY8	(%eax)	%r0
SP_INC	$73
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"cmd_cd"

cmd_rd:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_rd"

.FUNCTION	"cmd_rd"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_DEC	$11
LD8	%r1	$3
PUSH8	$1
SP_DEC	$1
CALL	monReadFileName
POP8	%eax
SP_WR8	%eax	$1
SP_INC	$1
CALL	monReadCr
CMP8	fileState	$0
JZ	@IC67
@IC68:	
LD8	%r1	$7
JUMP	@IC66
@IC67:	
PUSH8	$1
SP_DEC	$1
CALL	monValidateFileName
POP8	%eax
SP_WR8	%eax	$2
SP_INC	$1
SP_RD8	%ecx	$1
CMP8	%ecx	$0
JNZ	@IC72
@IC73:	
LD16	%r2	$FILE
LD16	%r3	$param1Data
CPY16	%r3	%r3
PUSH16	%r3
PUSH16	%r2
PUSH16	fatContext
SP_DEC	$1
CALL	fat_dirTableFind
POP8	%eax
SP_WR8	%eax	$8
SP_INC	$6
SP_RD8	%ecx	$2
SP_WR8	%ecx	$3
SP_RD8	%ecx	$2
CMP8	%ecx	$0
JZ	@IC77
@IC78:	
LD8	%r1	$3
JUMP	@IC71
@IC77:	
LD16	%r2	$FILE
PUSH16	%r2
SP_DEC	$4
CALL	fat_dirEntrySize
POP32	%eax
SP_WR32	%eax	$6
SP_INC	$2
SP_RD32	%r0	$4
LD16	%r3	$param1Data
CPY16	%r3	%r3
PUSH8	$1
PUSH16	%r3
PUSH16	%r2
PUSH16	fatContext
SP_DEC	$1
CALL	fat_fileOpen
POP8	%eax
SP_WR8	%eax	$15
SP_INC	$7
SP_RD8	%ecx	$8
SP_WR8	%ecx	$3
SP_RD8	%ecx	$8
CMP8	%ecx	$6
JNZ	@IC82
@IC83:	
LD8	%r1	$3
JUMP	@IC81
@IC82:	
LD16	%r2	$FILE
PUSH16	$0
PUSH16	hMonitor
PUSH16	$0
PUSH32	%r0
PUSH16	%r2
SP_DEC	$1
CALL	fat_fileRead
POP8	%eax
SP_WR8	%eax	$21
SP_INC	$12
SP_RD8	%ecx	$9
SP_WR8	%ecx	$3
SP_RD8	%ecx	$9
CMP8	%ecx	$0
JNZ	@IC87
@IC88:	
LD8	%r1	$0
JUMP	@IC86
@IC87:	
LD8	%r1	$3
@IC86:	
@IC81:	
LD16	%r2	$FILE
PUSH16	%r2
SP_DEC	$1
CALL	fat_fileClose
POP8	%eax
SP_WR8	%eax	$12
SP_INC	$2
@IC76:	
JUMP	@IC71
@IC72:	
LD8	%r1	$9
@IC71:	
@IC66:	
SP_STORE	%eax
INC16	%eax	$30
CPY8	(%eax)	%r1
SP_INC	$11
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"cmd_rd"

cmd_dld:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_dld"

.FUNCTION	"cmd_dld"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
SP_DEC	$8
LD8	%r0	$3
PUSH8	$1
SP_DEC	$1
CALL	monReadFileName
POP8	%eax
SP_WR8	%eax	$1
SP_INC	$1
CALL	monReadCr
CMP8	fileState	$0
JZ	@IC92
@IC93:	
LD8	%r0	$7
JUMP	@IC91
@IC92:	
PUSH8	$1
SP_DEC	$1
CALL	monValidateFileName
POP8	%eax
SP_WR8	%eax	$2
SP_INC	$1
SP_RD8	%r1	$1
CMP8	%r1	$0
JZ	@IC97
@IC98:	
LD8	%r0	$9
JUMP	@IC96
@IC97:	
LD16	%r1	$FILE
LD16	%r2	$param1Data
CPY16	%r2	%r2
PUSH16	%r2
PUSH16	%r1
PUSH16	fatContext
SP_DEC	$1
CALL	fat_dirTableFind
POP8	%eax
SP_WR8	%eax	$8
SP_INC	$6
SP_RD8	%ecx	$2
CMP8	%ecx	$0
JNZ	@IC102
@IC103:	
LD16	%r1	$FILE
PUSH16	%r1
SP_DEC	$1
CALL	fat_dirEntryIsDirectory
POP8	%eax
SP_WR8	%eax	$5
SP_INC	$2
SP_RD8	%ecx	$3
CMP8	%ecx	$0
JZ	@IC107
@IC108:	
LD16	%r1	$FILE
PUSH16	%r1
SP_DEC	$1
CALL	fat_dirDirIsEmpty
POP8	%eax
SP_WR8	%eax	$6
SP_INC	$2
SP_RD8	%ecx	$4
CMP8	%ecx	$0
JZ	@IC110
@IC111:	
LD16	%r1	$FILE
LD16	%r2	$param1Data
CPY16	%r2	%r2
PUSH8	$0
PUSH16	%r2
PUSH16	%r1
PUSH16	fatContext
SP_DEC	$1
CALL	fat_fileOpen
POP8	%eax
SP_WR8	%eax	$12
SP_INC	$7
SP_RD8	%ecx	$5
CMP8	%ecx	$6
JNZ	@IC113
@IC114:	
LD8	%r0	$5
JUMP	@IC109
@IC113:	
LD16	%r1	$FILE
PUSH16	%r1
SP_DEC	$1
CALL	fat_fileDelete
POP8	%eax
SP_WR8	%eax	$8
SP_INC	$2
PUSH16	%r1
SP_DEC	$1
CALL	fat_fileClose
POP8	%eax
SP_WR8	%eax	$9
SP_INC	$2
LD8	%r0	$0
@IC112:	
JUMP	@IC106
@IC110:	
LD8	%r0	$8
@IC109:	
JUMP	@IC101
@IC107:	
LD8	%r0	$5
@IC106:	
JUMP	@IC101
@IC102:	
LD8	%r0	$3
@IC101:	
@IC96:	
@IC91:	
SP_STORE	%eax
INC16	%eax	$23
CPY8	(%eax)	%r0
SP_INC	$8
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"cmd_dld"

cmd_mkd:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_mkd"

.FUNCTION	"cmd_mkd"	
PUSH32	%r0
PUSH32	%r1
SP_DEC	$6
LD8	%r1	$3
PUSH8	$1
SP_DEC	$1
CALL	monReadFileName
POP8	%eax
SP_WR8	%eax	$1
SP_INC	$1
CMP8	cmdTerminator	$32
JNZ	@IC117
@IC118:	
PUSH8	$4
PUSH8	$2
SP_DEC	$1
CALL	monReadNumber
POP8	%eax
SP_WR8	%eax	$3
SP_INC	$2
PUSH8	$4
PUSH8	$2
SP_DEC	$1
CALL	monValidateNumber
POP8	%eax
SP_WR8	%eax	$4
SP_INC	$2
PUSH32	param2
SP_DEC	$1
CALL	fat_time
POP8	%eax
SP_WR8	%eax	$7
SP_INC	$4
@IC117:	
CALL	monReadCr
CMP8	fileState	$0
JZ	@IC122
@IC123:	
LD8	%r1	$7
JUMP	@IC121
@IC122:	
PUSH8	$1
SP_DEC	$1
CALL	monValidateFileName
POP8	%eax
SP_WR8	%eax	$5
SP_INC	$1
SP_RD8	%r0	$4
CMP8	%r0	$0
JZ	@IC127
@IC128:	
LD8	%r1	$9
JUMP	@IC126
@IC127:	
LD16	%r0	$param1Data
PUSH16	%r0
PUSH16	fatContext
SP_DEC	$1
CALL	fat_dirCreateDir
POP8	%eax
SP_WR8	%eax	$9
SP_INC	$4
SP_RD8	%r1	$5
SP_RD8	%ecx	$5
CMP8	%ecx	$7
JNZ	@IC132
@IC133:	
LD8	%r1	$3
JUMP	@IC131
@IC132:	
CMP8	%r1	$11
JNZ	@IC137
@IC138:	
LD8	%r1	$4
JUMP	@IC136
@IC137:	
LD8	%r1	$0
@IC136:	
@IC131:	
@IC126:	
@IC121:	
SP_STORE	%eax
INC16	%eax	$17
CPY8	(%eax)	%r1
SP_INC	$6
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"cmd_mkd"

cmd_dlf:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_dlf"

.FUNCTION	"cmd_dlf"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
SP_DEC	$8
LD8	%r0	$3
PUSH8	$1
SP_DEC	$1
CALL	monReadFileName
POP8	%eax
SP_WR8	%eax	$1
SP_INC	$1
CALL	monReadCr
CMP8	fileState	$0
JZ	@IC142
@IC143:	
LD8	%r0	$7
JUMP	@IC141
@IC142:	
PUSH8	$1
SP_DEC	$1
CALL	monValidateFileName
POP8	%eax
SP_WR8	%eax	$2
SP_INC	$1
SP_RD8	%r1	$1
CMP8	%r1	$0
JZ	@IC147
@IC148:	
LD8	%r0	$9
JUMP	@IC146
@IC147:	
LD16	%r1	$FILE
LD16	%r2	$param1Data
CPY16	%r2	%r2
PUSH16	%r2
PUSH16	%r1
PUSH16	fatContext
SP_DEC	$1
CALL	fat_dirTableFind
POP8	%eax
SP_WR8	%eax	$8
SP_INC	$6
SP_RD8	%ecx	$2
CMP8	%ecx	$0
JNZ	@IC152
@IC153:	
LD16	%r1	$FILE
PUSH16	%r1
SP_DEC	$1
CALL	fat_dirEntryIsFile
POP8	%eax
SP_WR8	%eax	$5
SP_INC	$2
SP_RD8	%ecx	$3
CMP8	%ecx	$0
JZ	@IC157
@IC158:	
LD16	%r1	$FILE
LD16	%r2	$param1Data
CPY16	%r2	%r2
PUSH8	$0
PUSH16	%r2
PUSH16	%r1
PUSH16	fatContext
SP_DEC	$1
CALL	fat_fileOpen
POP8	%eax
SP_WR8	%eax	$11
SP_INC	$7
SP_RD8	%ecx	$4
SP_WR8	%ecx	$5
SP_RD8	%ecx	$4
CMP8	%ecx	$6
JNZ	@IC160
@IC161:	
LD8	%r0	$5
JUMP	@IC156
@IC160:	
LD16	%r1	$FILE
PUSH16	%r1
SP_DEC	$1
CALL	fat_fileDelete
POP8	%eax
SP_WR8	%eax	$8
SP_INC	$2
PUSH16	%r1
SP_DEC	$1
CALL	fat_fileClose
POP8	%eax
SP_WR8	%eax	$9
SP_INC	$2
LD8	%r0	$0
@IC159:	
JUMP	@IC151
@IC157:	
LD8	%r0	$5
@IC156:	
JUMP	@IC151
@IC152:	
LD8	%r0	$3
@IC151:	
@IC146:	
@IC141:	
SP_STORE	%eax
INC16	%eax	$23
CPY8	(%eax)	%r0
SP_INC	$8
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"cmd_dlf"

cmd_wrf:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_wrf"

.FUNCTION	"cmd_wrf"	
PUSH32	%r0
PUSH32	%r1
SP_DEC	$3
LD8	%r1	$3
PUSH8	$4
PUSH8	$1
SP_DEC	$1
CALL	monReadNumber
POP8	%eax
SP_WR8	%eax	$2
SP_INC	$2
CALL	monReadCr
CMP8	fileState	$2
JZ	@IC165
@IC166:	
LD8	%r1	$5
JUMP	@IC164
@IC165:	
PUSH8	$4
PUSH8	$1
SP_DEC	$1
CALL	monValidateNumber
POP8	%eax
SP_WR8	%eax	$3
SP_INC	$2
LD16	%r0	$FILE
PUSH16	$0
PUSH16	hMonitor
PUSH16	$0
PUSH32	param1
PUSH16	%r0
SP_DEC	$1
CALL	fat_fileWrite
POP8	%eax
SP_WR8	%eax	$14
SP_INC	$12
SP_RD8	%r1	$2
SP_RD8	%ecx	$2
CMP8	%ecx	$9
JNZ	@IC170
@IC171:	
LD8	%r1	$5
JUMP	@IC169
@IC170:	
CMP8	%r1	$10
JNZ	@IC175
@IC176:	
LD8	%r1	$3
JUMP	@IC174
@IC175:	
CMP8	%r1	$12
JNZ	@IC180
@IC181:	
LD8	%r1	$4
JUMP	@IC179
@IC180:	
CMP8	%r1	$0
JZ	@IC185
@IC186:	
LD8	%r1	$3
JUMP	@IC184
@IC185:	
LD8	%r1	$0
@IC184:	
@IC179:	
@IC174:	
@IC169:	
@IC164:	
SP_STORE	%eax
INC16	%eax	$14
CPY8	(%eax)	%r1
SP_INC	$3
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"cmd_wrf"

cmd_opw:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_opw"

.FUNCTION	"cmd_opw"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
SP_DEC	$9
LD8	%r0	$3
PUSH8	$1
SP_DEC	$1
CALL	monReadFileName
POP8	%eax
SP_WR8	%eax	$1
SP_INC	$1
CMP8	cmdTerminator	$32
JNZ	@IC189
@IC190:	
PUSH8	$4
PUSH8	$2
SP_DEC	$1
CALL	monReadNumber
POP8	%eax
SP_WR8	%eax	$3
SP_INC	$2
PUSH8	$4
PUSH8	$2
SP_DEC	$1
CALL	monValidateNumber
POP8	%eax
SP_WR8	%eax	$4
SP_INC	$2
PUSH32	param2
SP_DEC	$1
CALL	fat_time
POP8	%eax
SP_WR8	%eax	$7
SP_INC	$4
@IC189:	
CALL	monReadCr
CMP8	fileState	$2
JNZ	@IC194
@IC195:	
LD8	%r0	$7
JUMP	@IC193
@IC194:	
PUSH8	$1
SP_DEC	$1
CALL	monValidateFileName
POP8	%eax
SP_WR8	%eax	$5
SP_INC	$1
SP_RD8	%r1	$4
CMP8	%r1	$0
JZ	@IC199
@IC200:	
LD8	%r0	$9
JUMP	@IC198
@IC199:	
LD16	%r1	$FILE
LD16	%r2	$param1Data
CPY16	%r2	%r2
PUSH8	$17
PUSH16	%r2
PUSH16	%r1
PUSH16	fatContext
SP_DEC	$1
CALL	fat_fileOpen
POP8	%eax
SP_WR8	%eax	$12
SP_INC	$7
SP_RD8	%r0	$5
SP_RD8	%ecx	$5
CMP8	%ecx	$1
JNZ	@IC203
@IC204:	
LD16	%r1	$FILE
LD16	%r2	$param1Data
CPY16	%r2	%r2
PUSH8	$18
PUSH16	%r2
PUSH16	%r1
PUSH16	fatContext
SP_DEC	$1
CALL	fat_fileOpen
POP8	%eax
SP_WR8	%eax	$13
SP_INC	$7
SP_RD8	%r0	$6
@IC203:	
CMP8	%r0	$2
JNZ	@IC208
@IC209:	
LD8	%r0	$6
JUMP	@IC207
@IC208:	
CMP8	%r0	$6
JNZ	@IC213
@IC214:	
LD8	%r0	$5
JUMP	@IC212
@IC213:	
CMP8	%r0	$11
JNZ	@IC218
@IC219:	
LD8	%r0	$4
JUMP	@IC217
@IC218:	
CMP8	%r0	$0
JZ	@IC223
@IC224:	
LD8	%r0	$5
JUMP	@IC222
@IC223:	
LD16	%r1	$FILE
PUSH8	$1
PUSH32	$0
PUSH16	%r1
SP_DEC	$1
CALL	fat_fileSeek
POP8	%eax
SP_WR8	%eax	$14
SP_INC	$7
SP_RD8	%ecx	$7
SP_WR8	%ecx	$8
SP_RD8	%ecx	$7
CMP8	%ecx	$0
JZ	@IC227
@IC228:	
LD8	%r0	$3
@IC227:	
LD8	fileState	$2
LD8	%r0	$0
@IC222:	
@IC217:	
@IC212:	
@IC207:	
@IC198:	
@IC193:	
SP_STORE	%eax
INC16	%eax	$24
CPY8	(%eax)	%r0
SP_INC	$9
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"cmd_opw"

cmd_clf:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_clf"

.FUNCTION	"cmd_clf"	
PUSH32	%r0
SP_DEC	$2
CMP8	cmdTerminator	$32
JNZ	@IC231
@IC232:	
PUSH8	$1
SP_DEC	$1
CALL	monReadFileName
POP8	%eax
SP_WR8	%eax	$1
SP_INC	$1
@IC231:	
CALL	monReadCr
CMP8	fileState	$0
JZ	@IC235
@IC236:	
LD16	%r0	$FILE
PUSH16	%r0
SP_DEC	$1
CALL	fat_fileClose
POP8	%eax
SP_WR8	%eax	$3
SP_INC	$2
@IC235:	
LD8	fileState	$0
LD8	%eax	$0
SP_WR8	%eax	$9
SP_INC	$2
POP32	%r0
RTS	
.FUNC_END	"cmd_clf"

cmd_rdf:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_rdf"

.FUNCTION	"cmd_rdf"	
PUSH32	%r0
PUSH32	%r1
SP_DEC	$3
LD8	%r1	$3
PUSH8	$4
PUSH8	$1
SP_DEC	$1
CALL	monReadNumber
POP8	%eax
SP_WR8	%eax	$2
SP_INC	$2
CALL	monReadCr
CMP8	fileState	$0
JNZ	@IC240
@IC241:	
LD8	%r1	$5
JUMP	@IC239
@IC240:	
PUSH8	$4
PUSH8	$1
SP_DEC	$1
CALL	monValidateNumber
POP8	%eax
SP_WR8	%eax	$3
SP_INC	$2
LD16	%r0	$FILE
PUSH16	$0
PUSH16	hMonitor
PUSH16	$0
PUSH32	param1
PUSH16	%r0
SP_DEC	$1
CALL	fat_fileRead
POP8	%eax
SP_WR8	%eax	$14
SP_INC	$12
SP_RD8	%r1	$2
SP_RD8	%ecx	$2
CMP8	%ecx	$9
JNZ	@IC245
@IC246:	
LD8	%r1	$5
JUMP	@IC244
@IC245:	
CMP8	%r1	$10
JNZ	@IC250
@IC251:	
LD8	%r1	$3
JUMP	@IC249
@IC250:	
CMP8	%r1	$0
JZ	@IC255
@IC256:	
LD8	%r1	$3
JUMP	@IC254
@IC255:	
LD8	%r1	$0
@IC254:	
@IC249:	
@IC244:	
@IC239:	
SP_STORE	%eax
INC16	%eax	$14
CPY8	(%eax)	%r1
SP_INC	$3
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"cmd_rdf"

cmd_ren:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_ren"

.FUNCTION	"cmd_ren"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
SP_DEC	$8
LD8	%r0	$3
PUSH8	$1
SP_DEC	$1
CALL	monReadFileName
POP8	%eax
SP_WR8	%eax	$1
SP_INC	$1
PUSH8	$2
SP_DEC	$1
CALL	monReadFileName
POP8	%eax
SP_WR8	%eax	$2
SP_INC	$1
CALL	monReadCr
CMP8	fileState	$0
JZ	@IC260
@IC261:	
LD8	%r0	$7
JUMP	@IC259
@IC260:	
PUSH8	$1
SP_DEC	$1
CALL	monValidateFileName
POP8	%eax
SP_WR8	%eax	$3
SP_INC	$1
SP_RD8	%r1	$2
CMP8	%r1	$0
JZ	@IC265
@IC266:	
LD8	%r0	$9
JUMP	@IC264
@IC265:	
PUSH8	$2
SP_DEC	$1
CALL	monValidateFileName
POP8	%eax
SP_WR8	%eax	$4
SP_INC	$1
SP_RD8	%r1	$3
CMP8	%r1	$0
JZ	@IC270
@IC271:	
LD8	%r0	$9
JUMP	@IC269
@IC270:	
LD16	%r1	$FILE
LD16	%r2	$param1Data
CPY16	%r2	%r2
PUSH8	$0
PUSH16	%r2
PUSH16	%r1
PUSH16	fatContext
SP_DEC	$1
CALL	fat_fileOpen
POP8	%eax
SP_WR8	%eax	$11
SP_INC	$7
SP_RD8	%ecx	$4
SP_WR8	%ecx	$5
SP_RD8	%ecx	$4
CMP8	%ecx	$0
JZ	@IC275
@IC276:	
LD8	%r0	$5
JUMP	@IC274
@IC275:	
LD16	%r1	$FILE
LD16	%r2	$param2Data
CPY16	%r2	%r2
PUSH16	%r2
PUSH16	%r1
SP_DEC	$1
CALL	fat_fileRename
POP8	%eax
SP_WR8	%eax	$10
SP_INC	$4
PUSH16	%r1
SP_DEC	$1
CALL	fat_fileClose
POP8	%eax
SP_WR8	%eax	$9
SP_INC	$2
LD8	%r0	$0
@IC274:	
@IC269:	
@IC264:	
@IC259:	
SP_STORE	%eax
INC16	%eax	$23
CPY8	(%eax)	%r0
SP_INC	$8
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"cmd_ren"

cmd_opr:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_opr"

.FUNCTION	"cmd_opr"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
SP_DEC	$7
LD8	%r0	$3
PUSH8	$1
SP_DEC	$1
CALL	monReadFileName
POP8	%eax
SP_WR8	%eax	$1
SP_INC	$1
CMP8	cmdTerminator	$32
JNZ	@IC279
@IC280:	
PUSH8	$2
PUSH8	$2
SP_DEC	$1
CALL	monReadNumber
POP8	%eax
SP_WR8	%eax	$3
SP_INC	$2
PUSH8	$2
PUSH8	$2
SP_DEC	$1
CALL	monValidateNumber
POP8	%eax
SP_WR8	%eax	$4
SP_INC	$2
LD32	%ebx	$16
SHL32	%r1	param2	%ebx
CPY32	param2	%r1
PUSH32	%r1
SP_DEC	$1
CALL	fat_time
POP8	%eax
SP_WR8	%eax	$7
SP_INC	$4
@IC279:	
CALL	monReadCr
CMP8	fileState	$2
JNZ	@IC284
@IC285:	
LD8	%r0	$7
JUMP	@IC283
@IC284:	
PUSH8	$1
SP_DEC	$1
CALL	monValidateFileName
POP8	%eax
SP_WR8	%eax	$5
SP_INC	$1
SP_RD8	%r1	$4
CMP8	%r1	$0
JZ	@IC289
@IC290:	
LD8	%r0	$9
JUMP	@IC288
@IC289:	
LD16	%r1	$FILE
LD16	%r2	$param1Data
CPY16	%r2	%r2
PUSH8	$1
PUSH16	%r2
PUSH16	%r1
PUSH16	fatContext
SP_DEC	$1
CALL	fat_fileOpen
POP8	%eax
SP_WR8	%eax	$12
SP_INC	$7
SP_RD8	%ecx	$5
SP_WR8	%ecx	$6
SP_RD8	%ecx	$5
CMP8	%ecx	$0
JZ	@IC294
@IC295:	
LD8	%r0	$5
JUMP	@IC293
@IC294:	
LD8	fileState	$1
LD8	%r0	$0
@IC293:	
@IC288:	
@IC283:	
SP_STORE	%eax
INC16	%eax	$22
CPY8	(%eax)	%r0
SP_INC	$7
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"cmd_opr"

cmd_sek:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_sek"

.FUNCTION	"cmd_sek"	
PUSH32	%r0
PUSH32	%r1
SP_DEC	$3
LD8	%r1	$3
PUSH8	$4
PUSH8	$1
SP_DEC	$1
CALL	monReadNumber
POP8	%eax
SP_WR8	%eax	$2
SP_INC	$2
CALL	monReadCr
CMP8	fileState	$0
JNZ	@IC299
@IC300:	
LD8	%r1	$5
JUMP	@IC298
@IC299:	
PUSH8	$4
PUSH8	$1
SP_DEC	$1
CALL	monValidateNumber
POP8	%eax
SP_WR8	%eax	$3
SP_INC	$2
LD16	%r0	$FILE
PUSH32	param1
PUSH16	%r0
SP_DEC	$1
CALL	fat_fileSetPos
POP8	%eax
SP_WR8	%eax	$8
SP_INC	$6
SP_RD8	%ecx	$2
CMP8	%ecx	$0
JZ	@IC304
@IC305:	
LD8	%r1	$5
JUMP	@IC303
@IC304:	
LD8	%r1	$0
@IC303:	
@IC298:	
SP_STORE	%eax
INC16	%eax	$14
CPY8	(%eax)	%r1
SP_INC	$3
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"cmd_sek"

cmd_fs_handler:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_fs_handler"

.FUNCTION	"cmd_fs_handler"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
SP_DEC	$11
SP_RD8	%r2	$27
LD8	%ecx	$13
SP_WR8	%ecx	$0
CALL	monReadCr
SP_STORE	%r0
INC16	%r0	$1
SP_STORE	%r1
INC16	%r1	$5
PUSH8	$1
PUSH16	%r1
PUSH16	%r0
PUSH16	fatContext
SP_DEC	$1
CALL	fat_freeSpace
POP8	%eax
SP_WR8	%eax	$16
SP_INC	$7
CMP8	%r2	$0
JNZ	@IC308
@IC309:	
SP_RD32	%ecx	$1
CMP32	%ecx	$0
JZ	@IC312
@IC313:	
LD32	%ecx	$-1
SP_WR32	%ecx	$5
@IC312:	
@IC308:	
SP_STORE	%r0
INC16	%r0	$5
CPY16	%r0	%r0
PUSH8	$4
PUSH16	%r0
CALL	monAddNumberToConsole
SP_INC	$3
CMP8	%r2	$0
JZ	@IC314
@IC315:	
SP_STORE	%r0
INC16	%r0	$1
CPY16	%r0	%r0
PUSH8	$2
PUSH16	%r0
CALL	monAddNumberToConsole
SP_INC	$3
@IC314:	
SP_STORE	%r0
PUSH16	$1
PUSH16	%r0
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$14
SP_INC	$4
LD8	%eax	$0
SP_WR8	%eax	$26
SP_INC	$11
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"cmd_fs_handler"

cmd_fs:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_fs"

.FUNCTION	"cmd_fs"	
SP_DEC	$1
PUSH8	$0
SP_DEC	$1
CALL	cmd_fs_handler
POP8	%eax
SP_WR8	%eax	$1
SP_INC	$1
SP_STORE	%eax
SP_STORE	%ecx
INC16	%ecx	$4
CPY8	(%ecx)	(%eax)
SP_INC	$1
RTS	
.FUNC_END	"cmd_fs"

cmd_fse:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_fse"

.FUNCTION	"cmd_fse"	
SP_DEC	$1
PUSH8	$1
SP_DEC	$1
CALL	cmd_fs_handler
POP8	%eax
SP_WR8	%eax	$1
SP_INC	$1
SP_RD8	%eax	$0
SP_WR8	%eax	$4
SP_INC	$1
RTS	
.FUNC_END	"cmd_fse"

cmd_idd_handler:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_idd_handler"

.FUNCTION	"cmd_idd_handler"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_DEC	$95
SP_RD8	%r3	$115
LD16	%ecx	$3341
SP_WR16	%ecx	$0
LD32	%r0	$Str@3
SP_WR16	%r0	$2
LD32	%r0	$Str@4
SP_WR16	%r0	$4
LD32	%r0	$Str@5
SP_WR16	%r0	$6
LD32	%r0	$Str@6
SP_WR16	%r0	$8
LD32	%r0	$Str@7
SP_WR16	%r0	$10
LD32	%r0	$Str@8
SP_WR16	%r0	$12
LD32	%r0	$Str@9
SP_WR16	%r0	$14
LD32	%r0	$Str@10
SP_WR16	%r0	$16
LD32	%r0	$Str@11
SP_WR16	%r0	$18
LD32	%r0	$Str@12
SP_WR16	%r0	$20
LD32	%r0	$Str@13
SP_WR16	%r0	$22
LD32	%r0	$Str@14
SP_WR16	%r0	$24
LD32	%r0	$Str@15
SP_WR16	%r0	$26
PUSH16	fatContext
SP_DEC	$2
CALL	fat_getDevHandle
POP16	%eax
SP_WR16	%eax	$30
SP_INC	$2
SP_STORE	%r0
INC16	%r0	$30
CPY16	%r1	%r0
LD8	(%r1)	$21
LD16	%r1	$1
ADD16	%r1	%r0
SP_STORE	%r2
INC16	%r2	$35
CPY16	(%r1)	%r2
PUSH16	%r0
SP_RD16	%eax	$30
PUSH16	%eax
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$71
SP_INC	$4
SP_RD16	%eax	$2
PUSH16	%eax
CALL	monWriteString
SP_INC	$2
LD16	%r0	$28
ADD16	%r0	%r2
CPY16	%r0	%r0
PUSH8	$2
PUSH16	%r0
CALL	monAddMultibyteNumberToConsole
SP_INC	$3
SP_RD16	%eax	$4
PUSH16	%eax
CALL	monWriteString
SP_INC	$2
LD16	%r0	$30
ADD16	%r0	%r2
CPY16	%r0	%r0
PUSH8	$2
PUSH16	%r0
CALL	monAddMultibyteNumberToConsole
SP_INC	$3
SP_RD16	%eax	$6
PUSH16	%eax
CALL	monWriteString
SP_INC	$2
CPY16	%r0	%r2
PUSH16	$8
PUSH16	%r0
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$72
SP_INC	$4
SP_RD16	%eax	$8
PUSH16	%eax
CALL	monWriteString
SP_INC	$2
LD16	%r0	$8
ADD16	%r0	%r2
PUSH16	$16
PUSH16	%r0
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$73
SP_INC	$4
SP_RD16	%eax	$10
PUSH16	%eax
CALL	monWriteString
SP_INC	$2
LD16	%r0	$24
ADD16	%r0	%r2
PUSH16	$4
PUSH16	%r0
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$74
SP_INC	$4
SP_RD16	%eax	$12
PUSH16	%eax
CALL	monWriteString
SP_INC	$2
JUMP	@IC318
@IC317:	
SP_RD16	%eax	$16
PUSH16	%eax
CALL	monWriteString
SP_INC	$2
JUMP	@IC316
@IC319:	
SP_RD16	%eax	$14
PUSH16	%eax
CALL	monWriteString
SP_INC	$2
JUMP	@IC316
@IC318:	
PUSH16	fatContext
SP_DEC	$1
CALL	fat_getFSType
POP8	%eax
SP_WR8	%eax	$73
SP_INC	$2
SP_RD8	%ecx	$71
CMP8	%ecx	$22
JZ	@IC317
@IC320:	
SP_RD8	%ecx	$71
CMP8	%ecx	$50
JZ	@IC319
@IC316:	
SP_STORE	%r0
INC16	%r0	$72
PUSH16	%r0
PUSH16	fatContext
SP_DEC	$1
CALL	fat_bytesPerSector
POP8	%eax
SP_WR8	%eax	$78
SP_INC	$4
SP_RD16	%eax	$18
PUSH16	%eax
CALL	monWriteString
SP_INC	$2
CPY16	%r0	%r0
PUSH8	$2
PUSH16	%r0
CALL	monAddMultibyteNumberToConsole
SP_INC	$3
SP_STORE	%r0
INC16	%r0	$75
PUSH16	%r0
PUSH16	fatContext
SP_DEC	$1
CALL	fat_bytesPerCluster
POP8	%eax
SP_WR8	%eax	$83
SP_INC	$4
SP_RD16	%eax	$20
PUSH16	%eax
CALL	monWriteString
SP_INC	$2
CPY16	%r1	%r0
PUSH8	$3
PUSH16	%r1
CALL	monAddMultibyteNumberToConsole
SP_INC	$3
SP_STORE	%r1
INC16	%r1	$80
PUSH16	%r1
PUSH16	%r0
PUSH16	fatContext
SP_DEC	$1
CALL	fat_capacity
POP8	%eax
SP_WR8	%eax	$90
SP_INC	$6
CMP8	%r3	$0
JNZ	@IC321
@IC322:	
SP_RD32	%ecx	$75
CMP32	%ecx	$0
JZ	@IC325
@IC326:	
LD32	%ecx	$-1
SP_WR32	%ecx	$80
@IC325:	
@IC321:	
SP_RD16	%eax	$22
PUSH16	%eax
CALL	monWriteString
SP_INC	$2
CMP8	%r3	$0
JZ	@IC328
@IC329:	
SP_STORE	%r0
INC16	%r0	$85
SP_STORE	%eax
INC16	%eax	$80
CPY32	(%r0)	(%eax)
LD16	%r1	$4
ADD16	%r1	%r0
SP_STORE	%eax
INC16	%eax	$75
CPY32	(%r1)	(%eax)
CPY16	%r0	%r0
PUSH8	$6
PUSH16	%r0
CALL	monAddMultibyteNumberToConsole
SP_INC	$3
JUMP	@IC327
@IC328:	
SP_STORE	%r0
INC16	%r0	$80
CPY16	%r0	%r0
PUSH8	$4
PUSH16	%r0
CALL	monAddMultibyteNumberToConsole
SP_INC	$3
@IC327:	
SP_RD16	%eax	$26
PUSH16	%eax
CALL	monWriteString
SP_INC	$2
SP_RD16	%eax	$24
PUSH16	%eax
CALL	monWriteString
SP_INC	$2
SP_STORE	%r0
INC16	%r0	$75
SP_STORE	%r1
INC16	%r1	$80
PUSH8	$0
PUSH16	%r1
PUSH16	%r0
PUSH16	fatContext
SP_DEC	$1
CALL	fat_freeSpace
POP8	%eax
SP_WR8	%eax	$100
SP_INC	$7
SP_RD8	%ecx	$93
CMP8	%ecx	$0
JNZ	@IC330
@IC331:	
CMP8	%r3	$0
JNZ	@IC334
@IC335:	
SP_RD32	%ecx	$75
CMP32	%ecx	$0
JZ	@IC338
@IC339:	
LD32	%ecx	$-1
SP_WR32	%ecx	$80
@IC338:	
@IC334:	
CMP8	%r3	$0
JZ	@IC341
@IC342:	
SP_STORE	%r0
INC16	%r0	$85
SP_STORE	%eax
INC16	%eax	$80
CPY32	(%r0)	(%eax)
LD16	%r1	$4
ADD16	%r1	%r0
SP_STORE	%eax
INC16	%eax	$75
CPY32	(%r1)	(%eax)
CPY16	%r0	%r0
PUSH8	$6
PUSH16	%r0
CALL	monAddMultibyteNumberToConsole
SP_INC	$3
JUMP	@IC340
@IC341:	
SP_STORE	%r0
INC16	%r0	$80
CPY16	%r0	%r0
PUSH8	$4
PUSH16	%r0
CALL	monAddMultibyteNumberToConsole
SP_INC	$3
@IC340:	
@IC330:	
SP_RD16	%eax	$26
PUSH16	%eax
CALL	monWriteString
SP_INC	$2
SP_STORE	%r0
PUSH16	$2
PUSH16	%r0
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$98
SP_INC	$4
LD8	%eax	$0
SP_WR8	%eax	$114
SP_INC	$95
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"cmd_idd_handler"

cmd_idd:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_idd"

.FUNCTION	"cmd_idd"	
SP_DEC	$1
PUSH8	$0
SP_DEC	$1
CALL	cmd_idd_handler
POP8	%eax
SP_WR8	%eax	$1
SP_INC	$1
SP_STORE	%eax
SP_STORE	%ecx
INC16	%ecx	$4
CPY8	(%ecx)	(%eax)
SP_INC	$1
RTS	
.FUNC_END	"cmd_idd"

cmd_idde:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_idde"

.FUNCTION	"cmd_idde"	
SP_DEC	$1
PUSH8	$1
SP_DEC	$1
CALL	cmd_idd_handler
POP8	%eax
SP_WR8	%eax	$1
SP_INC	$1
SP_RD8	%eax	$0
SP_WR8	%eax	$4
SP_INC	$1
RTS	
.FUNC_END	"cmd_idde"

cmd_dsn:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_dsn"

.FUNCTION	"cmd_dsn"	
PUSH32	%r0
SP_DEC	$7
LD8	%ecx	$13
SP_WR8	%ecx	$0
SP_STORE	%r0
INC16	%r0	$1
PUSH16	%r0
PUSH16	fatContext
SP_DEC	$1
CALL	fat_getVolumeID
POP8	%eax
SP_WR8	%eax	$9
SP_INC	$4
CPY16	%r0	%r0
PUSH8	$4
PUSH16	%r0
CALL	monAddNumberToConsole
SP_INC	$3
SP_STORE	%r0
PUSH16	$1
PUSH16	%r0
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$10
SP_INC	$4
LD8	%eax	$0
SP_WR8	%eax	$14
SP_INC	$7
POP32	%r0
RTS	
.FUNC_END	"cmd_dsn"

cmd_dvl:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_dvl"

.FUNCTION	"cmd_dvl"	
PUSH32	%r0
SP_DEC	$15
LD8	%ecx	$13
SP_WR8	%ecx	$0
SP_STORE	%r0
INC16	%r0	$1
PUSH16	%r0
PUSH16	fatContext
SP_DEC	$1
CALL	fat_getVolumeLabel
POP8	%eax
SP_WR8	%eax	$16
SP_INC	$4
PUSH16	$11
PUSH16	%r0
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$17
SP_INC	$4
SP_STORE	%r0
PUSH16	$1
PUSH16	%r0
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$18
SP_INC	$4
LD8	%eax	$0
SP_WR8	%eax	$22
SP_INC	$15
POP32	%r0
RTS	
.FUNC_END	"cmd_dvl"

cmd_dirt:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_dirt"

.FUNCTION	"cmd_dirt"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
SP_DEC	$87
LD8	%ecx	$13
SP_WR8	%ecx	$0
LD8	%ecx	$32
SP_WR8	%ecx	$1
LD8	%r2	$3
PUSH8	$1
SP_DEC	$1
CALL	monReadFileName
POP8	%eax
SP_WR8	%eax	$3
SP_INC	$1
CALL	monReadCr
PUSH8	$1
SP_DEC	$1
CALL	monValidateFileName
POP8	%eax
SP_WR8	%eax	$4
SP_INC	$1
SP_RD8	%r2	$3
CMP8	%r2	$0
JNZ	@IC344
@IC345:	
SP_STORE	%r0
INC16	%r0	$4
LD16	%r1	$param1Data
CPY16	%r1	%r1
PUSH16	%r1
PUSH16	%r0
PUSH16	fatContext
SP_DEC	$1
CALL	fat_dirTableFind
POP8	%eax
SP_WR8	%eax	$77
SP_INC	$6
SP_RD8	%ecx	$71
CMP8	%ecx	$0
JNZ	@IC349
@IC350:	
SP_STORE	%r0
INC16	%r0	$4
CPY16	%r1	%r0
PUSH16	%r1
SP_DEC	$1
CALL	monWriteFileName
POP8	%eax
SP_WR8	%eax	$74
SP_INC	$2
SP_STORE	%r1
INC16	%r1	$1
PUSH16	$1
PUSH16	%r1
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$77
SP_INC	$4
PUSH8	$2
PUSH16	%r0
SP_DEC	$2
CALL	fat_dirEntryTime
POP16	%eax
SP_WR16	%eax	$77
SP_INC	$3
SP_RD16	%ecx	$74
SP_WR16	%ecx	$76
SP_STORE	%r1
INC16	%r1	$76
CPY16	%r1	%r1
PUSH8	$2
PUSH16	%r1
CALL	monAddNumberToConsole
SP_INC	$3
PUSH8	$1
PUSH16	%r0
SP_DEC	$2
CALL	fat_dirEntryTime
POP16	%eax
SP_WR16	%eax	$81
SP_INC	$3
SP_RD16	%ecx	$78
SP_WR16	%ecx	$76
SP_STORE	%r1
INC16	%r1	$76
CPY16	%r1	%r1
PUSH8	$2
PUSH16	%r1
CALL	monAddNumberToConsole
SP_INC	$3
PUSH8	$5
PUSH16	%r0
SP_DEC	$2
CALL	fat_dirEntryTime
POP16	%eax
SP_WR16	%eax	$83
SP_INC	$3
SP_RD16	%ecx	$80
SP_WR16	%ecx	$76
SP_STORE	%r1
INC16	%r1	$76
CPY16	%r1	%r1
PUSH8	$2
PUSH16	%r1
CALL	monAddNumberToConsole
SP_INC	$3
PUSH8	$4
PUSH16	%r0
SP_DEC	$2
CALL	fat_dirEntryTime
POP16	%eax
SP_WR16	%eax	$85
SP_INC	$3
SP_RD16	%ecx	$82
SP_WR16	%ecx	$76
SP_STORE	%r1
INC16	%r1	$76
CPY16	%r1	%r1
PUSH8	$2
PUSH16	%r1
CALL	monAddNumberToConsole
SP_INC	$3
PUSH8	$3
PUSH16	%r0
SP_DEC	$2
CALL	fat_dirEntryTime
POP16	%eax
SP_WR16	%eax	$87
SP_INC	$3
SP_RD16	%ecx	$84
SP_WR16	%ecx	$76
SP_STORE	%r0
INC16	%r0	$76
CPY16	%r0	%r0
PUSH8	$2
PUSH16	%r0
CALL	monAddNumberToConsole
SP_INC	$3
SP_STORE	%r0
PUSH16	$1
PUSH16	%r0
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$90
SP_INC	$4
JUMP	@IC343
@IC349:	
LD8	%r2	$3
@IC348:	
JUMP	@IC343
@IC344:	
LD8	%r2	$9
@IC343:	
SP_STORE	%eax
INC16	%eax	$102
CPY8	(%eax)	%r2
SP_INC	$87
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"cmd_dirt"

cmd_dri:	
.GLOBAL	 DO_NOT_EXPORT  "cmd_dri"

.FUNCTION	"cmd_dri"	
SP_DEC	$1
CALL	monReadCr
SP_DEC	$1
CALL	diskResetDisk
POP8	%eax
SP_WR8	%eax	$0
LD8	%eax	$0
SP_WR8	%eax	$4
SP_INC	$1
RTS	
.FUNC_END	"cmd_dri"

