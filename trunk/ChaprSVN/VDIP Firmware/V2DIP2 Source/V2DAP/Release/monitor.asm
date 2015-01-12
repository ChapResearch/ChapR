.DATA

.WEAK	"%eax"
.WEAK	"%ebx"
.WEAK	"%ecx"
.WEAK	"%r0"
.WEAK	"%r1"
.WEAK	"%r2"
.WEAK	"%r3"
monFlashLED	.DB	1	0
.GLOBAL	  DO_NOT_EXPORT "monFlashLED"
monLEDstate	.DB	1	96
.GLOBAL	  DO_NOT_EXPORT "monLEDstate"
cmdDataIndex	.DB	1	?
.GLOBAL	  DO_NOT_EXPORT "cmdDataIndex"
cmdTerminator	.DB	1	?
.GLOBAL	  DO_NOT_EXPORT "cmdTerminator"
param1	.DD	1	?
.GLOBAL	  DO_NOT_EXPORT "param1"
param2	.DD	1	?
.GLOBAL	  DO_NOT_EXPORT "param2"
cmdData	.DB	4	?
.GLOBAL	  DO_NOT_EXPORT "cmdData"
Mon_Flags	.DB	1	?
.GLOBAL	  DO_NOT_EXPORT "Mon_Flags"
param1Data	.DB	11	?
.GLOBAL	  DO_NOT_EXPORT "param1Data"
param2Data	.DB	11	?
.GLOBAL	  DO_NOT_EXPORT "param2Data"
Str@0	.ASCIIZ	"LED flash"
Str@1	.ASCIIZ	"D:\\>\r"
Str@2	.ASCIIZ	">\r"
Str@3	.ASCIIZ	"No Disk\r"
Str@4	.ASCIIZ	"ND\r"
Str@5	.ASCIIZ	"No Upgrade\r"
Str@6	.ASCIIZ	"NU\r"
Str@7	.ASCIIZ	"Device Detected P"
Str@8	.ASCIIZ	"DD"
Str@9	.ASCIIZ	"Device Removed P"
Str@10	.ASCIIZ	"DR"
Str@11	.ASCIIZ	"Slave Enabled\r"
Str@12	.ASCIIZ	"SDA\r"
Str@13	.ASCIIZ	"Slave Disabled\r"
Str@14	.ASCIIZ	"SDD\r"
Str@15	.ASCIIZ	"Bad Command\r"
Str@16	.ASCIIZ	"BC\r"
Str@17	.ASCIIZ	"Command Failed\r"
Str@18	.ASCIIZ	"CF\r"
Str@19	.ASCIIZ	"Disk Full\r"
Str@20	.ASCIIZ	"DF\r"
Str@21	.ASCIIZ	"Invalid\r"
Str@22	.ASCIIZ	"FI\r"
Str@23	.ASCIIZ	"Read Only\r"
Str@24	.ASCIIZ	"RO\r"
Str@25	.ASCIIZ	"File Open\r"
Str@26	.ASCIIZ	"FO\r"
Str@27	.ASCIIZ	"Dir Not Empty\r"
Str@28	.ASCIIZ	"NE\r"
Str@29	.ASCIIZ	"Filename Invalid\r"
Str@30	.ASCIIZ	"FN\r"
.WEAK	"deviceList"
.WEAK	"cmdDataIndex"
.WEAK	"userDataArea"
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
.WEAK	"fileState"
.WEAK	"param1Data"
.WEAK	"param2Data"




.TEXT

maskCodePage	.DW	8	0, 0, -7169, 1023, -1, -14337, -1, -6145
commands	.DATA_DEF	810
commands	.DATA_INIT	0	0	32	32
commands	.DATA_INIT	256	32	16	16
commands	.DATA_INIT	cmd_cr	48	32	32
commands	.DATA_INIT	5456723	80	32	32
commands	.DATA_INIT	16	112	16	16
commands	.DATA_INIT	cmd_scs	128	32	32
commands	.DATA_INIT	16	160	32	32
commands	.DATA_INIT	16	192	16	16
commands	.DATA_INIT	cmd_scs	208	32	32
commands	.DATA_INIT	4539219	240	32	32
commands	.DATA_INIT	17	272	16	16
commands	.DATA_INIT	cmd_ecs	288	32	32
commands	.DATA_INIT	4804673	320	32	32
commands	.DATA_INIT	144	352	16	16
commands	.DATA_INIT	cmd_ipa	368	32	32
commands	.DATA_INIT	4804680	400	32	32
commands	.DATA_INIT	145	432	16	16
commands	.DATA_INIT	cmd_iph	448	32	32
commands	.DATA_INIT	5456452	480	32	32
commands	.DATA_INIT	20	512	16	16
commands	.DATA_INIT	cmd_sbd	528	32	32
commands	.DATA_INIT	4609878	560	32	32
commands	.DATA_INIT	19	592	16	16
commands	.DATA_INIT	cmd_fwv	608	32	32
commands	.DATA_INIT	69	640	32	32
commands	.DATA_INIT	69	672	16	16
commands	.DATA_INIT	cmd_echo	688	32	32
commands	.DATA_INIT	101	720	32	32
commands	.DATA_INIT	101	752	16	16
commands	.DATA_INIT	cmd_echo	768	32	32
commands	.DATA_INIT	4475218	800	32	32
commands	.DATA_INIT	257	832	16	16
commands	.DATA_INIT	cmd_dir	848	32	32
commands	.DATA_INIT	17220	880	32	32
commands	.DATA_INIT	258	912	16	16
commands	.DATA_INIT	cmd_cd	928	32	32
commands	.DATA_INIT	21060	960	32	32
commands	.DATA_INIT	260	992	16	16
commands	.DATA_INIT	cmd_rd	1008	32	32
commands	.DATA_INIT	4475972	1040	32	32
commands	.DATA_INIT	261	1072	16	16
commands	.DATA_INIT	cmd_dld	1088	32	32
commands	.DATA_INIT	5065540	1120	32	32
commands	.DATA_INIT	262	1152	16	16
commands	.DATA_INIT	cmd_mkd	1168	32	32
commands	.DATA_INIT	4475974	1200	32	32
commands	.DATA_INIT	263	1232	16	16
commands	.DATA_INIT	cmd_dlf	1248	32	32
commands	.DATA_INIT	5722694	1280	32	32
commands	.DATA_INIT	264	1312	16	16
commands	.DATA_INIT	cmd_wrf	1328	32	32
commands	.DATA_INIT	5197911	1360	32	32
commands	.DATA_INIT	265	1392	16	16
commands	.DATA_INIT	cmd_opw	1408	32	32
commands	.DATA_INIT	4410438	1440	32	32
commands	.DATA_INIT	266	1472	16	16
commands	.DATA_INIT	cmd_clf	1488	32	32
commands	.DATA_INIT	5391430	1520	32	32
commands	.DATA_INIT	267	1552	16	16
commands	.DATA_INIT	cmd_rdf	1568	32	32
commands	.DATA_INIT	5391694	1600	32	32
commands	.DATA_INIT	268	1632	16	16
commands	.DATA_INIT	cmd_ren	1648	32	32
commands	.DATA_INIT	5197906	1680	32	32
commands	.DATA_INIT	270	1712	16	16
commands	.DATA_INIT	cmd_opr	1728	32	32
commands	.DATA_INIT	5457227	1760	32	32
commands	.DATA_INIT	296	1792	16	16
commands	.DATA_INIT	cmd_sek	1808	32	32
commands	.DATA_INIT	18003	1840	32	32
commands	.DATA_INIT	274	1872	16	16
commands	.DATA_INIT	cmd_fs	1888	32	32
commands	.DATA_INIT	4608837	1920	32	32
commands	.DATA_INIT	403	1952	16	16
commands	.DATA_INIT	cmd_fse	1968	32	32
commands	.DATA_INIT	4801604	2000	32	32
commands	.DATA_INIT	271	2032	16	16
commands	.DATA_INIT	cmd_idd	2048	32	32
commands	.DATA_INIT	1229210693	2080	32	32
commands	.DATA_INIT	404	2112	16	16
commands	.DATA_INIT	cmd_idde	2128	32	32
commands	.DATA_INIT	4477774	2160	32	32
commands	.DATA_INIT	301	2192	16	16
commands	.DATA_INIT	cmd_dsn	2208	32	32
commands	.DATA_INIT	4478540	2240	32	32
commands	.DATA_INIT	302	2272	16	16
commands	.DATA_INIT	cmd_dvl	2288	32	32
commands	.DATA_INIT	1145655892	2320	32	32
commands	.DATA_INIT	303	2352	16	16
commands	.DATA_INIT	cmd_dirt	2368	32	32
commands	.DATA_INIT	5461316	2400	32	32
commands	.DATA_INIT	277	2432	16	16
commands	.DATA_INIT	cmd_sud	2448	32	32
commands	.DATA_INIT	5720900	2480	32	32
commands	.DATA_INIT	278	2512	16	16
commands	.DATA_INIT	cmd_wkd	2528	32	32
commands	.DATA_INIT	5461325	2560	32	32
commands	.DATA_INIT	23	2592	16	16
commands	.DATA_INIT	cmd_sum	2608	32	32
commands	.DATA_INIT	4804434	2640	32	32
commands	.DATA_INIT	41	2672	16	16
commands	.DATA_INIT	cmd_ior	2688	32	32
commands	.DATA_INIT	4804439	2720	32	32
commands	.DATA_INIT	42	2752	16	16
commands	.DATA_INIT	cmd_iow	2768	32	32
commands	.DATA_INIT	5261139	2800	32	32
commands	.DATA_INIT	129	2832	16	16
commands	.DATA_INIT	cmd_pgs	2848	32	32
commands	.DATA_INIT	5264210	2880	32	32
commands	.DATA_INIT	130	2912	16	16
commands	.DATA_INIT	cmd_psr	2928	32	32
commands	.DATA_INIT	5328945	2960	32	32
commands	.DATA_INIT	43	2992	16	16
commands	.DATA_INIT	cmd_qp1	3008	32	32
commands	.DATA_INIT	5328946	3040	32	32
commands	.DATA_INIT	44	3072	16	16
commands	.DATA_INIT	cmd_qp2	3088	32	32
commands	.DATA_INIT	20804	3120	32	32
commands	.DATA_INIT	133	3152	16	16
commands	.DATA_INIT	cmd_qd	3168	32	32
commands	.DATA_INIT	21315	3200	32	32
commands	.DATA_INIT	134	3232	16	16
commands	.DATA_INIT	cmd_sc	3248	32	32
commands	.DATA_INIT	4477764	3280	32	32
commands	.DATA_INIT	131	3312	16	16
commands	.DATA_INIT	cmd_dsd	3328	32	32
commands	.DATA_INIT	4477508	3360	32	32
commands	.DATA_INIT	132	3392	16	16
commands	.DATA_INIT	cmd_drd	3408	32	32
commands	.DATA_INIT	5460821	3440	32	32
commands	.DATA_INIT	154	3472	16	16
commands	.DATA_INIT	cmd_ssu	3488	32	32
commands	.DATA_INIT	21318	3520	32	32
commands	.DATA_INIT	135	3552	16	16
commands	.DATA_INIT	cmd_sf	3568	32	32
commands	.DATA_INIT	5329747	3600	32	32
commands	.DATA_INIT	152	3632	16	16
commands	.DATA_INIT	cmd_qss	3648	32	32
commands	.DATA_INIT	4604484	3680	32	32
commands	.DATA_INIT	24	3712	16	16
commands	.DATA_INIT	cmd_fbd	3728	32	32
commands	.DATA_INIT	4607299	3760	32	32
commands	.DATA_INIT	25	3792	16	16
commands	.DATA_INIT	cmd_fmc	3808	32	32
commands	.DATA_INIT	4608836	3840	32	32
commands	.DATA_INIT	26	3872	16	16
commands	.DATA_INIT	cmd_fsd	3888	32	32
commands	.DATA_INIT	4605507	3920	32	32
commands	.DATA_INIT	27	3952	16	16
commands	.DATA_INIT	cmd_ffc	3968	32	32
commands	.DATA_INIT	4605773	4000	32	32
commands	.DATA_INIT	28	4032	16	16
commands	.DATA_INIT	cmd_fgm	4048	32	32
commands	.DATA_INIT	4608844	4080	32	32
commands	.DATA_INIT	34	4112	16	16
commands	.DATA_INIT	cmd_fsl	4128	32	32
commands	.DATA_INIT	4608834	4160	32	32
commands	.DATA_INIT	35	4192	16	16
commands	.DATA_INIT	cmd_fsb	4208	32	32
commands	.DATA_INIT	4605762	4240	32	32
commands	.DATA_INIT	36	4272	16	16
commands	.DATA_INIT	cmd_fgb	4288	32	32
commands	.DATA_INIT	5656646	4320	32	32
commands	.DATA_INIT	285	4352	16	16
commands	.DATA_INIT	cmd_vpf	4368	32	32
commands	.DATA_INIT	5657158	4400	32	32
commands	.DATA_INIT	393	4432	16	16
commands	.DATA_INIT	cmd_vrf	4448	32	32
commands	.DATA_INIT	5657428	4480	32	32
commands	.DATA_INIT	288	4512	16	16
commands	.DATA_INIT	cmd_vst	4528	32	32
commands	.DATA_INIT	5649217	4560	32	32
commands	.DATA_INIT	289	4592	16	16
commands	.DATA_INIT	cmd_v3a	4608	32	32
commands	.DATA_INIT	5657153	4640	32	32
commands	.DATA_INIT	394	4672	16	16
commands	.DATA_INIT	cmd_vra	4688	32	32
commands	.DATA_INIT	5657170	4720	32	32
commands	.DATA_INIT	399	4752	16	16
commands	.DATA_INIT	cmd_vrr	4768	32	32
commands	.DATA_INIT	5657414	4800	32	32
commands	.DATA_INIT	293	4832	16	16
commands	.DATA_INIT	cmd_vsf	4848	32	32
commands	.DATA_INIT	5657410	4880	32	32
commands	.DATA_INIT	294	4912	16	16
commands	.DATA_INIT	cmd_vsb	4928	32	32
commands	.DATA_INIT	5657412	4960	32	32
commands	.DATA_INIT	398	4992	16	16
commands	.DATA_INIT	cmd_vsd	5008	32	32
commands	.DATA_INIT	22096	5040	32	32
commands	.DATA_INIT	395	5072	16	16
commands	.DATA_INIT	cmd_vp	5088	32	32
commands	.DATA_INIT	22086	5120	32	32
commands	.DATA_INIT	396	5152	16	16
commands	.DATA_INIT	cmd_vf	5168	32	32
commands	.DATA_INIT	22082	5200	32	32
commands	.DATA_INIT	397	5232	16	16
commands	.DATA_INIT	cmd_vb	5248	32	32
commands	.DATA_INIT	5657156	5280	32	32
commands	.DATA_INIT	287	5312	16	16
commands	.DATA_INIT	cmd_vrd	5328	32	32
commands	.DATA_INIT	5658450	5360	32	32
commands	.DATA_INIT	286	5392	16	16
commands	.DATA_INIT	cmd_vwr	5408	32	32
commands	.DATA_INIT	5657430	5440	32	32
commands	.DATA_INIT	392	5472	16	16
commands	.DATA_INIT	cmd_vsv	5488	32	32
commands	.DATA_INIT	16698	5520	32	32
commands	.DATA_INIT	321	5552	16	16
commands	.DATA_INIT	cmd_a	5568	32	32
commands	.DATA_INIT	16954	5600	32	32
commands	.DATA_INIT	322	5632	16	16
commands	.DATA_INIT	cmd_b	5648	32	32
commands	.DATA_INIT	4803923	5680	32	32
commands	.DATA_INIT	294	5712	16	16
commands	.DATA_INIT	cmd_ims	5728	32	32
commands	.DATA_INIT	4803910	5760	32	32
commands	.DATA_INIT	293	5792	16	16
commands	.DATA_INIT	cmd_imf	5808	32	32
commands	.DATA_INIT	4411475	5840	32	32
commands	.DATA_INIT	289	5872	16	16
commands	.DATA_INIT	cmd_cps	5888	32	32
commands	.DATA_INIT	4411462	5920	32	32
commands	.DATA_INIT	288	5952	16	16
commands	.DATA_INIT	cmd_cpf	5968	32	32
commands	.DATA_INIT	21316	6000	32	32
commands	.DATA_INIT	259	6032	16	16
commands	.DATA_INIT	cmd_sd	6048	32	32
commands	.DATA_INIT	21335	6080	32	32
commands	.DATA_INIT	402	6112	16	16
commands	.DATA_INIT	cmd_sw	6128	32	32
commands	.DATA_INIT	4609877	6160	32	32
commands	.DATA_INIT	149	6192	16	16
commands	.DATA_INIT	cmd_fwu	6208	32	32
commands	.DATA_INIT	4477513	6240	32	32
commands	.DATA_INIT	409	6272	16	16
commands	.DATA_INIT	cmd_dri	6288	32	32
commands	.DATA_INIT	4804169	6320	32	32
commands	.DATA_INIT	255	6352	16	16
commands	.DATA_INIT	cmd_ini	6368	32	32
commands	.DATA_INIT	65535	6400	32	32
commands	.DATA_INIT	255	6432	16	16
commands	.DATA_INIT	0	6448	8	8
commands	.DATA_DEF_END

.WEAK	"vos_dma_get_fifo_flow_control"

.WEAK	"vos_start_scheduler"

.WEAK	"vos_gpio_write_port"

.WEAK	"vos_signal_semaphore_from_isr"

.WEAK	"vos_malloc"

.WEAK	"vos_create_thread_ex"

.WEAK	"vos_memcpy"

.WEAK	"vos_memset"

.WEAK	"vos_get_kernel_clock"

.WEAK	"vos_gpio_disable_int"

.WEAK	"vos_get_package_type"

.WEAK	"vos_dma_get_fifo_data_register"

.WEAK	"monDataMode"

.WEAK	"vos_signal_semaphore"

.WEAK	"vos_gpio_wait_on_int"

.WEAK	"vos_dma_get_fifo_data"

.WEAK	"hostSuspend"

.WEAK	"vos_iocell_get_config"

.WEAK	"vos_iomux_define_bidi"

.WEAK	"vos_gpio_set_all_mode"

.WEAK	"vos_iocell_set_config"

.WEAK	"diskCheckFwu"

.WEAK	"vos_gpio_set_pin_mode"

.WEAK	"vos_get_chip_revision"

.WEAK	"vos_wait_semaphore_ex"

.WEAK	"vos_enable_interrupts"

.WEAK	"vos_dev_read"

.WEAK	"checkDataMode"

.WEAK	"vos_dev_open"

.WEAK	"vos_halt_cpu"

.WEAK	"vos_dev_init"

.WEAK	"vos_dma_get_fifo_count"

.WEAK	"diskCheckDisk"

.WEAK	"usbhost_init"

.WEAK	"vos_reset_kernel_clock"

.WEAK	"diskResetDisk"

.WEAK	"vos_gpio_set_port_mode"

.WEAK	"vos_iomux_define_input"

.WEAK	"vos_disable_interrupts"

.WEAK	"hostConnected"

.WEAK	"vos_get_idle_thread_tcb"

.WEAK	"vos_dma_reset"

.WEAK	"vos_dev_close"

.WEAK	"vos_wdt_clear"

.WEAK	"vos_heap_size"

.WEAK	"vos_dev_ioctl"

.WEAK	"vos_dev_write"

.WEAK	"vos_get_clock_frequency"

.WEAK	"hostGetDevType"

.WEAK	"vos_set_clock_frequency"

.WEAK	"diskRemoveDisk"

.WEAK	"hostRemoveFromDeviceList"

.WEAK	"vos_dma_enable"

.WEAK	"vos_reset_vnc2"

.WEAK	"vos_heap_space"

.WEAK	"vos_iomux_define_output"

.WEAK	"vos_wdt_enable"

.WEAK	"vos_dma_wait_on_complete"

.WEAK	"vos_lock_mutex"

.WEAK	"cmd_a"

.WEAK	"cmd_b"

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

.WEAK	"cmd_cd"

.WEAK	"cmd_qd"

.WEAK	"cmd_cr"

.WEAK	"cmd_sc"

.WEAK	"cmd_rd"

.WEAK	"cmd_sd"

.WEAK	"cmd_vb"

.WEAK	"vos_gpio_wait_on_all_ints"

.WEAK	"cmd_sf"

.WEAK	"cmd_fs"

.WEAK	"hostSelectDevice"

.WEAK	"cmd_vf"

.WEAK	"cmd_vp"

.WEAK	"cmd_sw"

.WEAK	"vos_delay_cancel"

.WEAK	"cmd_v3a"

.WEAK	"cmd_qp1"

.WEAK	"cmd_qp2"

.WEAK	"vos_dma_retained_configure"

.WEAK	"cmd_fbd"

.WEAK	"hostSetDeviceFtdi"

.WEAK	"cmd_fgb"

.WEAK	"cmd_ffc"

.WEAK	"cmd_idd"

.WEAK	"cmd_dld"

.WEAK	"cmd_clf"

.WEAK	"cmd_fmc"

.WEAK	"cmd_dlf"

.WEAK	"cmd_cpf"

.WEAK	"cmd_sbd"

.WEAK	"cmd_fgm"

.WEAK	"cmd_drd"

.WEAK	"cmd_ipa"

.WEAK	"cmd_fsb"

.WEAK	"cmd_dsd"

.WEAK	"cmd_ecs"

.WEAK	"cmd_imf"

.WEAK	"cmd_rdf"

.WEAK	"cmd_mkd"

.WEAK	"cmd_fsd"

.WEAK	"cmd_fse"

.WEAK	"cmd_dir"

.WEAK	"cmd_dri"

.WEAK	"cmd_ini"

.WEAK	"cmd_iph"

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

.WEAK	"cmd_scs"

.WEAK	"cmd_pgs"

.WEAK	"cmd_ior"

.WEAK	"cmd_vsb"

.WEAK	"cmd_vrd"

.WEAK	"cmd_vpf"

.WEAK	"cmd_sud"

.WEAK	"cmd_vsd"

.WEAK	"cmd_vrf"

.WEAK	"cmd_vsf"

.WEAK	"cmd_wrf"

.WEAK	"cmd_iow"

.WEAK	"cmd_opr"

.WEAK	"cmd_fwu"

.WEAK	"cmd_fwv"

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

.WEAK	"cmd_idde"

.WEAK	"cmd_echo"

.WEAK	"diskInit"

.WEAK	"vos_stop_profiler"

.WEAK	"hostGetPortDevType"

.WEAK	"hostWake"

.WEAK	"cmd_dirt"

.WEAK	"vos_trylock_mutex"

.WEAK	"vos_free"

.WEAK	"vos_init"

.WEAK	"hostAddToDeviceList"

.WEAK	"vos_gpio_read_port"

.WEAK	"vos_gpio_write_all"

.WEAK	"vos_set_idle_thread_tcb_size"

.WEAK	"vos_init_semaphore"

.WEAK	"vos_wait_semaphore"

.WEAK	"vos_gpio_write_pin"

.WEAK	"vos_start_profiler"

.WEAK	"monOnline"

.WEAK	"uart_init"

.WEAK	"vos_gpio_enable_int"

.WEAK	"vos_signal_cond_var"

.GLOBAL	  DO_NOT_EXPORT "maskCodePage"
.GLOBAL	  DO_NOT_EXPORT "commands"
monInit:	
.GLOBAL	 DO_NOT_EXPORT  "monInit"

.FUNCTION	"monInit"	
PUSH32	%r0
PUSH32	%r1
SP_DEC	$4
SP_RD8	%r0	$15
CPY8	Mon_Flags	%r0
LD16	%r0	$deviceList
PUSH16	$112
PUSH32	$0
PUSH16	%r0
SP_DEC	$2
CALL	vos_memset
POP16	%eax
SP_WR16	%eax	$8
SP_INC	$8
CALL	diskInit
LD32	%r0	$monFlashLedThread
LD32	%r1	$Str@0
PUSH16	$0
PUSH16	%r1
PUSH32	%r0
PUSH16	$120
PUSH8	$20
SP_DEC	$2
CALL	vos_create_thread_ex
POP16	%eax
SP_WR16	%eax	$13
SP_INC	$11
SP_INC	$4
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monInit"

monWriteString:	
.GLOBAL	 DO_NOT_EXPORT  "monWriteString"

.FUNCTION	"monWriteString"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_DEC	$3
SP_RD16	%r2	$22
LD16	%r0	$0
@IC1:	
CPY16	%eax	%r0
AND32	%eax	$65535
CPY32	%r1	%eax
ADD16	%r1	%r2
CPY8	%r1	(%r1)
AND32	%r1	$255
CMP32	%r1	$0
JZ	@IC2
@IC3:	
INC16	%r0	$1
JUMP	@IC1
@IC2:	
CPY16	%r1	%r2
SP_STORE	%r3
INC16	%r3	$0
PUSH16	%r3
PUSH16	%r0
PUSH16	%r1
PUSH16	hMonitor
SP_DEC	$1
CALL	vos_dev_write
POP8	%eax
SP_WR8	%eax	$10
SP_INC	$8
SP_INC	$3
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monWriteString"

monVersion:	
.GLOBAL	 DO_NOT_EXPORT  "monVersion"

.FUNCTION	"monVersion"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
SP_DEC	$2
LD8	%r0	$0
@IC4:	
CMP8	%r0	$16
JGES	@IC5
@IC6:	
CPY8	%eax	%r0
SHL32	%eax	$24
SAR32	%eax	$24
LD32	%ebx	$1
SAR32	%r1	%eax	%ebx
CPYF	F1	C
LD32	%r2	$userDataArea
ADD32	%r1	%r2
CPYROM	%eax	%r1	$1
JNF1	cgLabel@0
SHR16	%eax	$8
cgLabel@0:	
CPY8	%r1	%eax
SP_STORE	%ecx
CPY8	(%ecx)	%r1
CMP8	%r1	$0
JNZ	@IC10
@IC11:	
JUMP	@IC5
@IC10:	
SP_STORE	%r1
PUSH16	$1
PUSH16	%r1
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$5
SP_INC	$4
@IC7:	
INC8	%r0	$1
JUMP	@IC4
@IC5:	
SP_INC	$2
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monVersion"

monPrompt:	
.GLOBAL	 DO_NOT_EXPORT  "monPrompt"

.FUNCTION	"monPrompt"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
LD32	%r0	$Str@1
CPY16	%r0	%r0
LD32	%r1	$Str@2
CPY16	%r1	%r1
CPY8	%eax	Mon_Flags
AND32	%eax	$255
LD32	%r2	$1
AND32	%r2	%eax
CMP32	%r2	$0
JZ	@IC15
@IC16:	
PUSH16	%r1
CALL	monWriteString
SP_INC	$2
JUMP	@IC14
@IC15:	
PUSH16	%r0
CALL	monWriteString
SP_INC	$2
@IC14:	
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monPrompt"

monNoDisk:	
.GLOBAL	 DO_NOT_EXPORT  "monNoDisk"

.FUNCTION	"monNoDisk"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
LD32	%r0	$Str@3
CPY16	%r0	%r0
LD32	%r1	$Str@4
CPY16	%r1	%r1
CPY8	%eax	Mon_Flags
AND32	%eax	$255
LD32	%r2	$1
AND32	%r2	%eax
CMP32	%r2	$0
JZ	@IC18
@IC19:	
PUSH16	%r1
CALL	monWriteString
SP_INC	$2
JUMP	@IC17
@IC18:	
PUSH16	%r0
CALL	monWriteString
SP_INC	$2
@IC17:	
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monNoDisk"

monNoUpgrade:	
.GLOBAL	 DO_NOT_EXPORT  "monNoUpgrade"

.FUNCTION	"monNoUpgrade"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
LD32	%r0	$Str@5
CPY16	%r0	%r0
LD32	%r1	$Str@6
CPY16	%r1	%r1
CPY8	%eax	Mon_Flags
AND32	%eax	$255
LD32	%r2	$1
AND32	%r2	%eax
CMP32	%r2	$0
JZ	@IC21
@IC22:	
PUSH16	%r1
CALL	monWriteString
SP_INC	$2
JUMP	@IC20
@IC21:	
PUSH16	%r0
CALL	monWriteString
SP_INC	$2
@IC20:	
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monNoUpgrade"

monDeviceDetected:	
.GLOBAL	 DO_NOT_EXPORT  "monDeviceDetected"

.FUNCTION	"monDeviceDetected"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_DEC	$3
SP_RD8	%r2	$22
LD32	%r0	$Str@7
CPY16	%r0	%r0
LD32	%r1	$Str@8
CPY16	%r1	%r1
CPY8	%eax	Mon_Flags
AND32	%eax	$255
LD32	%r3	$1
AND32	%r3	%eax
CMP32	%r3	$0
JZ	@IC24
@IC25:	
PUSH16	%r1
CALL	monWriteString
SP_INC	$2
JUMP	@IC23
@IC24:	
PUSH16	%r0
CALL	monWriteString
SP_INC	$2
@IC23:	
CPY8	%eax	%r2
AND32	%eax	$255
LD32	%r3	$49
ADD32	%r3	%eax
SP_STORE	%ecx
INC16	%ecx	$0
CPY8	(%ecx)	%r3
SP_STORE	%r3
PUSH16	$1
PUSH16	%r3
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$5
SP_INC	$4
LD8	%ecx	$13
SP_WR8	%ecx	$0
SP_STORE	%r3
PUSH16	$1
PUSH16	%r3
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$6
SP_INC	$4
SP_INC	$3
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monDeviceDetected"

monDeviceRemoved:	
.GLOBAL	 DO_NOT_EXPORT  "monDeviceRemoved"

.FUNCTION	"monDeviceRemoved"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_DEC	$3
SP_RD8	%r2	$22
LD32	%r0	$Str@9
CPY16	%r0	%r0
LD32	%r1	$Str@10
CPY16	%r1	%r1
CPY8	%eax	Mon_Flags
AND32	%eax	$255
LD32	%r3	$1
AND32	%r3	%eax
CMP32	%r3	$0
JZ	@IC27
@IC28:	
PUSH16	%r1
CALL	monWriteString
SP_INC	$2
JUMP	@IC26
@IC27:	
PUSH16	%r0
CALL	monWriteString
SP_INC	$2
@IC26:	
CPY8	%eax	%r2
AND32	%eax	$255
LD32	%r3	$49
ADD32	%r3	%eax
SP_STORE	%ecx
INC16	%ecx	$0
CPY8	(%ecx)	%r3
SP_STORE	%r3
PUSH16	$1
PUSH16	%r3
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$5
SP_INC	$4
LD8	%ecx	$13
SP_WR8	%ecx	$0
SP_STORE	%r3
PUSH16	$1
PUSH16	%r3
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$6
SP_INC	$4
SP_INC	$3
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monDeviceRemoved"

monSlaveEnabled:	
.GLOBAL	 DO_NOT_EXPORT  "monSlaveEnabled"

.FUNCTION	"monSlaveEnabled"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
LD32	%r0	$Str@11
CPY16	%r0	%r0
LD32	%r1	$Str@12
CPY16	%r1	%r1
CPY8	%eax	Mon_Flags
AND32	%eax	$255
LD32	%r2	$1
AND32	%r2	%eax
CMP32	%r2	$0
JZ	@IC30
@IC31:	
PUSH16	%r1
CALL	monWriteString
SP_INC	$2
JUMP	@IC29
@IC30:	
PUSH16	%r0
CALL	monWriteString
SP_INC	$2
@IC29:	
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monSlaveEnabled"

monSlaveDisabled:	
.GLOBAL	 DO_NOT_EXPORT  "monSlaveDisabled"

.FUNCTION	"monSlaveDisabled"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
LD32	%r0	$Str@13
CPY16	%r0	%r0
LD32	%r1	$Str@14
CPY16	%r1	%r1
CPY8	%eax	Mon_Flags
AND32	%eax	$255
LD32	%r2	$1
AND32	%r2	%eax
CMP32	%r2	$0
JZ	@IC33
@IC34:	
PUSH16	%r1
CALL	monWriteString
SP_INC	$2
JUMP	@IC32
@IC33:	
PUSH16	%r0
CALL	monWriteString
SP_INC	$2
@IC32:	
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monSlaveDisabled"

monBadCommand:	
.GLOBAL	 DO_NOT_EXPORT  "monBadCommand"

.FUNCTION	"monBadCommand"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
LD32	%r0	$Str@15
CPY16	%r0	%r0
LD32	%r1	$Str@16
CPY16	%r1	%r1
CPY8	%eax	Mon_Flags
AND32	%eax	$255
LD32	%r2	$1
AND32	%r2	%eax
CMP32	%r2	$0
JZ	@IC36
@IC37:	
PUSH16	%r1
CALL	monWriteString
SP_INC	$2
JUMP	@IC35
@IC36:	
PUSH16	%r0
CALL	monWriteString
SP_INC	$2
@IC35:	
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monBadCommand"

monCommandFailed:	
.GLOBAL	 DO_NOT_EXPORT  "monCommandFailed"

.FUNCTION	"monCommandFailed"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
LD32	%r0	$Str@17
CPY16	%r0	%r0
LD32	%r1	$Str@18
CPY16	%r1	%r1
CPY8	%eax	Mon_Flags
AND32	%eax	$255
LD32	%r2	$1
AND32	%r2	%eax
CMP32	%r2	$0
JZ	@IC39
@IC40:	
PUSH16	%r1
CALL	monWriteString
SP_INC	$2
JUMP	@IC38
@IC39:	
PUSH16	%r0
CALL	monWriteString
SP_INC	$2
@IC38:	
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monCommandFailed"

monDiskFull:	
.GLOBAL	 DO_NOT_EXPORT  "monDiskFull"

.FUNCTION	"monDiskFull"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
LD32	%r0	$Str@19
CPY16	%r0	%r0
LD32	%r1	$Str@20
CPY16	%r1	%r1
CPY8	%eax	Mon_Flags
AND32	%eax	$255
LD32	%r2	$1
AND32	%r2	%eax
CMP32	%r2	$0
JZ	@IC42
@IC43:	
PUSH16	%r1
CALL	monWriteString
SP_INC	$2
JUMP	@IC41
@IC42:	
PUSH16	%r0
CALL	monWriteString
SP_INC	$2
@IC41:	
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monDiskFull"

monInvalid:	
.GLOBAL	 DO_NOT_EXPORT  "monInvalid"

.FUNCTION	"monInvalid"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
LD32	%r0	$Str@21
CPY16	%r0	%r0
LD32	%r1	$Str@22
CPY16	%r1	%r1
CPY8	%eax	Mon_Flags
AND32	%eax	$255
LD32	%r2	$1
AND32	%r2	%eax
CMP32	%r2	$0
JZ	@IC45
@IC46:	
PUSH16	%r1
CALL	monWriteString
SP_INC	$2
JUMP	@IC44
@IC45:	
PUSH16	%r0
CALL	monWriteString
SP_INC	$2
@IC44:	
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monInvalid"

monReadOnly:	
.GLOBAL	 DO_NOT_EXPORT  "monReadOnly"

.FUNCTION	"monReadOnly"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
LD32	%r0	$Str@23
CPY16	%r0	%r0
LD32	%r1	$Str@24
CPY16	%r1	%r1
CPY8	%eax	Mon_Flags
AND32	%eax	$255
LD32	%r2	$1
AND32	%r2	%eax
CMP32	%r2	$0
JZ	@IC48
@IC49:	
PUSH16	%r1
CALL	monWriteString
SP_INC	$2
JUMP	@IC47
@IC48:	
PUSH16	%r0
CALL	monWriteString
SP_INC	$2
@IC47:	
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monReadOnly"

monFileOpen:	
.GLOBAL	 DO_NOT_EXPORT  "monFileOpen"

.FUNCTION	"monFileOpen"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
LD32	%r0	$Str@25
CPY16	%r0	%r0
LD32	%r1	$Str@26
CPY16	%r1	%r1
CPY8	%eax	Mon_Flags
AND32	%eax	$255
LD32	%r2	$1
AND32	%r2	%eax
CMP32	%r2	$0
JZ	@IC51
@IC52:	
PUSH16	%r1
CALL	monWriteString
SP_INC	$2
JUMP	@IC50
@IC51:	
PUSH16	%r0
CALL	monWriteString
SP_INC	$2
@IC50:	
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monFileOpen"

monDirNotEmpty:	
.GLOBAL	 DO_NOT_EXPORT  "monDirNotEmpty"

.FUNCTION	"monDirNotEmpty"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
LD32	%r0	$Str@27
CPY16	%r0	%r0
LD32	%r1	$Str@28
CPY16	%r1	%r1
CPY8	%eax	Mon_Flags
AND32	%eax	$255
LD32	%r2	$1
AND32	%r2	%eax
CMP32	%r2	$0
JZ	@IC54
@IC55:	
PUSH16	%r1
CALL	monWriteString
SP_INC	$2
JUMP	@IC53
@IC54:	
PUSH16	%r0
CALL	monWriteString
SP_INC	$2
@IC53:	
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monDirNotEmpty"

monFilenameInvalid:	
.GLOBAL	 DO_NOT_EXPORT  "monFilenameInvalid"

.FUNCTION	"monFilenameInvalid"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
LD32	%r0	$Str@29
CPY16	%r0	%r0
LD32	%r1	$Str@30
CPY16	%r1	%r1
CPY8	%eax	Mon_Flags
AND32	%eax	$255
LD32	%r2	$1
AND32	%r2	%eax
CMP32	%r2	$0
JZ	@IC57
@IC58:	
PUSH16	%r1
CALL	monWriteString
SP_INC	$2
JUMP	@IC56
@IC57:	
PUSH16	%r0
CALL	monWriteString
SP_INC	$2
@IC56:	
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monFilenameInvalid"

monReadCmd:	
.GLOBAL	 DO_NOT_EXPORT  "monReadCmd"

.FUNCTION	"monReadCmd"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
SP_DEC	$16
LD8	%ecx	$0
SP_WR8	%ecx	$0
LD32	%ecx	$0
SP_WR32	%ecx	$1
LD32	%r0	$1
SP_STORE	%r1
INC16	%r1	$5
CPY16	%r2	%r1
LD8	(%r2)	$2
PUSH16	%r1
PUSH16	hMonitor
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$18
SP_INC	$4
INC16	%r1	$5
CPY16	%r1	(%r1)
CPY16	%eax	%r1
AND32	%eax	$65535
CPY32	%r1	%eax
CMP32	%r1	$0
JZ	@IC59
@IC60:	
CALL	monLedsGreenFlash
LD8	cmdDataIndex	$0
@IC61:	
CMP8	cmdDataIndex	$4
JGE	@IC62
@IC63:	
CPY8	%eax	cmdDataIndex
AND32	%eax	$255
CPY32	%r1	%eax
LD16	%r2	$cmdData
ADD16	%r1	%r2
LD8	(%r1)	$0
@IC64:	
INC8	cmdDataIndex	$1
JUMP	@IC61
@IC62:	
LD8	cmdDataIndex	$0
LD32	param1	$0
LD32	param2	$0
@IC67:	
SP_STORE	%r1
PUSH16	$1
PUSH16	%r1
SP_DEC	$1
CALL	monRead
POP8	%eax
SP_WR8	%eax	$19
SP_INC	$4
SP_STORE	%ecx
CMP8	(%ecx)	$13
JZ	@IC71
JNZ	@IC72
@IC72:	
SP_STORE	%ecx
CMP8	(%ecx)	$32
JNZ	@IC70
@IC71:	
JUMP	@IC69
@IC70:	
CMP8	cmdDataIndex	$4
JGE	@IC77
@IC78:	
CPY8	%eax	cmdDataIndex
AND32	%eax	$255
CPY32	%r1	%eax
LD16	%r2	$cmdData
ADD16	%r1	%r2
SP_STORE	%eax
CPY8	(%r1)	(%eax)
INC8	cmdDataIndex	$1
@IC77:	
@IC68:	
LD8	%ecx	$1
CMP8	%ecx	$0
JNZ	@IC67
@IC69:	
SP_STORE	%eax
CPY8	cmdTerminator	(%eax)
LD32	%r0	$0
@IC59:	
SP_STORE	%eax
INC16	%eax	$31
CPY8	(%eax)	%r0
SP_INC	$16
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monReadCmd"

monReadFileName:	
.GLOBAL	 DO_NOT_EXPORT  "monReadFileName"

.FUNCTION	"monReadFileName"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_DEC	$3
SP_RD8	%r2	$23
LD8	%ecx	$0
SP_WR8	%ecx	$0
LD8	%r3	$46
LD8	%r1	$0
CMP8	%r2	$1
JNZ	@IC82
@IC83:	
LD16	%r2	$param1Data
CPY16	%r0	%r2
JUMP	@IC81
@IC82:	
LD16	%r2	$param2Data
CPY16	%r0	%r2
@IC81:	
LD8	%ecx	$0
SP_WR8	%ecx	$1
@IC86:	
SP_STORE	%ecx
INC16	%ecx	$1
CMP8	(%ecx)	$11
JGE	@IC87
@IC88:	
SP_RD8	%eax	$1
AND32	%eax	$255
CPY32	%r2	%eax
ADD16	%r2	%r0
LD8	(%r2)	$32
@IC89:	
SP_STORE	%eax
INC16	%eax	$1
INC8	(%eax)	$1
JUMP	@IC86
@IC87:	
@IC92:	
SP_STORE	%r2
INC16	%r2	$0
PUSH16	$1
PUSH16	%r2
SP_DEC	$1
CALL	monRead
POP8	%eax
SP_WR8	%eax	$6
SP_INC	$4
CMP8	%r1	$0
JNZ	@IC95
@IC97:	
SP_STORE	%ecx
CMP8	(%ecx)	$32
JNZ	@IC95
@IC96:	
JUMP	@IC93
@IC95:	
SP_STORE	%ecx
CMP8	(%ecx)	$13
JZ	@IC103
JNZ	@IC104
@IC104:	
SP_STORE	%ecx
CMP8	(%ecx)	$32
JNZ	@IC102
@IC103:	
JUMP	@IC94
@IC102:	
SP_STORE	%ecx
CMP8	(%ecx)	$46
JNZ	@IC110
@IC112:	
CMP8	%r3	$46
JZ	@IC110
@IC111:	
CMP8	%r1	$8
JGE	@IC109
@IC118:	
CPY8	%eax	(%r0)
AND32	%eax	$255
CPY32	%r2	%eax
CMP32	%r2	$46
JZ	@IC121
@IC122:	
LD8	%r1	$8
@IC121:	
@IC117:	
JUMP	@IC109
@IC110:	
CMP8	%r1	$11
JGE	@IC125
@IC126:	
CPY8	%eax	%r1
AND32	%eax	$255
CPY32	%r2	%eax
ADD16	%r2	%r0
SP_STORE	%eax
CPY8	(%r2)	(%eax)
INC8	%r1	$1
@IC125:	
@IC109:	
SP_STORE	%eax
CPY8	%r3	(%eax)
@IC93:	
LD8	%ecx	$1
CMP8	%ecx	$0
JNZ	@IC92
@IC94:	
SP_STORE	%eax
CPY8	cmdTerminator	(%eax)
LD8	%eax	$0
SP_WR8	%eax	$22
SP_INC	$3
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monReadFileName"

monReadNumber:	
.GLOBAL	 DO_NOT_EXPORT  "monReadNumber"

.FUNCTION	"monReadNumber"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_DEC	$33
SP_RD8	%ecx	$54
SP_WR8	%ecx	$54
SP_RD8	%r2	$53
LD8	%ecx	$0
SP_WR8	%ecx	$0
LD8	%ecx	$0
SP_WR8	%ecx	$1
LD32	%ecx	$0
SP_WR32	%ecx	$2
LD8	%r0	$0
LD8	%r1	$0
CMP8	%r2	$1
JNZ	@IC130
@IC131:	
LD16	%r2	$param1Data
SP_STORE	%ecx
INC16	%ecx	$6
CPY16	(%ecx)	%r2
JUMP	@IC129
@IC130:	
LD16	%r2	$param2Data
SP_STORE	%ecx
INC16	%ecx	$6
CPY16	(%ecx)	%r2
@IC129:	
CPY8	%eax	Mon_Flags
AND32	%eax	$255
LD32	%r2	$2
AND32	%r2	%eax
CMP32	%r2	$0
JZ	@IC135
@IC136:	
LD8	%ecx	$0
SP_WR8	%ecx	$8
@IC137:	
SP_STORE	%ecx
INC16	%ecx	$8
CMP8	(%ecx)	$11
JGE	@IC138
@IC139:	
SP_RD8	%eax	$8
AND32	%eax	$255
CPY32	%r2	%eax
SP_RD16	%eax	$6
ADD16	%r2	%eax
LD8	(%r2)	$0
@IC140:	
SP_STORE	%eax
INC16	%eax	$8
INC8	(%eax)	$1
JUMP	@IC137
@IC138:	
@IC143:	
SP_STORE	%r2
INC16	%r2	$0
PUSH16	$1
PUSH16	%r2
SP_DEC	$1
CALL	monRead
POP8	%eax
SP_WR8	%eax	$13
SP_INC	$4
SP_STORE	%ecx
CMP8	(%ecx)	$32
JNZ	@IC146
@IC147:	
LD8	%ecx	$48
SP_WR8	%ecx	$0
@IC146:	
@IC144:	
SP_STORE	%ecx
CMP8	(%ecx)	$48
JZ	@IC143
JNZ	@IC145
@IC145:	
SP_STORE	%ecx
CMP8	(%ecx)	$36
JZ	@IC153
JNZ	@IC154
@IC154:	
SP_STORE	%ecx
CMP8	(%ecx)	$120
JNZ	@IC152
@IC153:	
LD8	%ecx	$1
SP_WR8	%ecx	$1
SP_STORE	%r2
PUSH16	$1
PUSH16	%r2
SP_DEC	$1
CALL	monRead
POP8	%eax
SP_WR8	%eax	$14
SP_INC	$4
@IC152:	
SP_STORE	%ecx
CMP8	(%ecx)	$115
JZ	@IC161
JNZ	@IC162
@IC162:	
SP_STORE	%ecx
CMP8	(%ecx)	$83
JNZ	@IC160
@IC161:	
SP_RD16	%ecx	$6
SP_STORE	%eax
CPY8	(%ecx)	(%eax)
@IC167:	
SP_STORE	%r2
PUSH16	$1
PUSH16	%r2
SP_DEC	$1
CALL	monRead
POP8	%eax
SP_WR8	%eax	$15
SP_INC	$4
@IC168:	
SP_STORE	%ecx
CMP8	(%ecx)	$13
JNZ	@IC167
JZ	@IC169
@IC169:	
SP_STORE	%eax
CPY8	cmdTerminator	(%eax)
JUMP	@IC134
@IC160:	
@IC172:	
SP_STORE	%ecx
CMP8	(%ecx)	$13
JZ	@IC173
@IC175:	
SP_STORE	%ecx
CMP8	(%ecx)	$32
JZ	@IC173
@IC174:	
CMP8	%r1	$16
JGE	@IC180
@IC181:	
CPY8	%eax	%r1
AND32	%eax	$255
CPY32	%r2	%eax
SP_STORE	%r3
INC16	%r3	$12
ADD16	%r2	%r3
SP_RD16	%eax	$0
PUSH8	%eax
SP_DEC	$1
CALL	monASCIItoDec
POP8	%eax
SP_WR8	%eax	$29
SP_INC	$1
SP_STORE	%eax
INC16	%eax	$28
CPY8	(%r2)	(%eax)
INC8	%r1	$1
@IC180:	
SP_STORE	%r2
PUSH16	$1
PUSH16	%r2
SP_DEC	$1
CALL	monRead
POP8	%eax
SP_WR8	%eax	$33
SP_INC	$4
JUMP	@IC172
@IC173:	
SP_STORE	%eax
CPY8	cmdTerminator	(%eax)
SP_STORE	%ecx
INC16	%ecx	$1
CMP8	(%ecx)	$0
JZ	@IC185
@IC186:	
@IC187:	
CMP8	%r1	$0
JLE	@IC184
@IC189:	
DEC8	%r1	$1
CPY8	%eax	%r1
AND32	%eax	$255
CPY32	%r2	%eax
SP_STORE	%r3
INC16	%r3	$12
ADD16	%r2	%r3
CPY8	%r2	(%r2)
SP_STORE	%ecx
CPY8	(%ecx)	%r2
CMP8	%r1	$0
JLE	@IC192
@IC193:	
DEC8	%r1	$1
CPY8	%eax	%r1
AND32	%eax	$255
CPY32	%r2	%eax
SP_STORE	%r3
INC16	%r3	$12
ADD16	%r2	%r3
CPY8	%r2	(%r2)
CPY8	%eax	%r2
AND32	%eax	$255
LD32	%ebx	$4
SHL32	%r2	%eax	%ebx
SP_RD8	%eax	$0
SHL32	%eax	$24
SAR32	%eax	$24
ADD32	%r2	%eax	%r2
SP_STORE	%ecx
CPY8	(%ecx)	%r2
@IC192:	
CPY8	%eax	%r0
AND32	%eax	$255
CPY32	%r2	%eax
SP_RD16	%eax	$6
ADD16	%r2	%eax
SP_STORE	%eax
CPY8	(%r2)	(%eax)
INC8	%r0	$1
JUMP	@IC187
@IC188:	
JUMP	@IC184
@IC185:	
LD32	%ecx	$0
SP_WR32	%ecx	$2
LD8	%r0	$0
@IC196:	
CMP8	%r1	$0
JLE	@IC197
@IC198:	
DEC8	%r1	$1
CPY8	%eax	%r0
AND32	%eax	$255
CPY32	%r2	%eax
SP_STORE	%r3
INC16	%r3	$12
ADD16	%r2	%r3
CPY8	%r2	(%r2)
SP_STORE	%eax
INC16	%eax	$2
CPY8	%ebx	%r2
AND32	%ebx	$255
ADD32	%r2	(%eax)	%ebx
SP_STORE	%ecx
INC16	%ecx	$2
CPY32	(%ecx)	%r2
INC8	%r0	$1
CMP8	%r1	$0
JLE	@IC196
@IC202:	
SP_RD32	%r2	$2
MUL32	%r2	$10
SP_STORE	%ecx
INC16	%ecx	$2
CPY32	(%ecx)	%r2
@IC201:	
JUMP	@IC196
@IC197:	
SP_STORE	%r2
INC16	%r2	$2
PUSH16	$4
PUSH16	%r2
SP_RD16	%eax	$10
PUSH16	%eax
SP_DEC	$2
CALL	vos_memcpy
POP16	%eax
SP_WR16	%eax	$36
SP_INC	$6
@IC184:	
@IC159:	
JUMP	@IC134
@IC135:	
SP_STORE	%ecx
INC16	%ecx	$54
CMP8	(%ecx)	$0
JZ	@IC205
@IC206:	
@IC207:	
SP_STORE	%eax
INC16	%eax	$54
DEC8	(%eax)	$1
SP_STORE	%r2
INC16	%r2	$0
PUSH16	$1
PUSH16	%r2
SP_DEC	$1
CALL	monRead
POP8	%eax
SP_WR8	%eax	$36
SP_INC	$4
SP_RD8	%eax	$54
AND32	%eax	$255
CPY32	%r2	%eax
SP_RD16	%eax	$6
ADD16	%r2	%eax
SP_STORE	%eax
CPY8	(%r2)	(%eax)
@IC208:	
SP_STORE	%ecx
INC16	%ecx	$54
CMP8	(%ecx)	$0
JNZ	@IC207
JZ	@IC209
@IC209:	
@IC205:	
SP_RD8	cmdTerminator	$0
@IC134:	
LD8	%eax	$0
SP_WR8	%eax	$52
SP_INC	$33
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monReadNumber"

monReadCr:	
.GLOBAL	 DO_NOT_EXPORT  "monReadCr"

.FUNCTION	"monReadCr"	
PUSH32	%r0
SP_DEC	$2
CMP8	cmdTerminator	$13
JZ	@IC212
@IC213:	
@IC216:	
SP_STORE	%r0
PUSH16	$1
PUSH16	%r0
SP_DEC	$1
CALL	monRead
POP8	%eax
SP_WR8	%eax	$5
SP_INC	$4
@IC217:	
SP_STORE	%ecx
CMP8	(%ecx)	$13
JNZ	@IC216
JZ	@IC218
@IC218:	
@IC212:	
SP_INC	$2
POP32	%r0
RTS	
.FUNC_END	"monReadCr"

monWrite:	
.GLOBAL	 DO_NOT_EXPORT  "monWrite"

.FUNCTION	"monWrite"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
SP_DEC	$3
SP_RD16	%r2	$21
SP_RD16	%r0	$19
CPY16	%r0	%r0
SP_STORE	%r1
PUSH16	%r1
PUSH16	%r2
PUSH16	%r0
PUSH16	hMonitor
SP_DEC	$1
CALL	vos_dev_write
POP8	%eax
SP_WR8	%eax	$10
SP_INC	$8
SP_STORE	%ecx
CMP16	(%ecx)	%r2
JNZ	@IC222
@IC223:	
LD8	%eax	$0
SP_WR8	%eax	$18
SP_INC	$3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
JUMP	@IC221
@IC222:	
LD8	%eax	$3
SP_WR8	%eax	$18
SP_INC	$3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
@IC221:	
SP_INC	$3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monWrite"

monWriteFileName:	
.GLOBAL	 DO_NOT_EXPORT  "monWriteFileName"

.FUNCTION	"monWriteFileName"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_DEC	$6
SP_RD16	%r0	$26
LD8	%ecx	$46
SP_WR8	%ecx	$0
LD8	%r1	$0
@IC226:	
CMP8	%r1	$11
JGE	@IC227
@IC228:	
CPY8	%eax	%r1
AND32	%eax	$255
CPY32	%r2	%eax
ADD16	%r2	%r0
CPY8	%r2	(%r2)
AND32	%r2	$255
CMP32	%r2	$32
JNZ	@IC233
@IC234:	
CMP8	%r1	$8
JGE	@IC227
@IC239:	
CPY16	%r2	%r0
CPY8	%r2	(%r2)
AND32	%r2	$255
CMP32	%r2	$46
JNZ	@IC242
@IC243:	
JUMP	@IC227
@IC242:	
LD16	%r2	$8
ADD16	%r2	%r0
CPY8	%r2	(%r2)
AND32	%r2	$255
CMP32	%r2	$32
JNZ	@IC246
@IC247:	
JUMP	@IC227
@IC246:	
LD8	%r1	$8
SP_STORE	%r2
SP_STORE	%r3
INC16	%r3	$1
PUSH16	%r3
PUSH16	$1
PUSH16	%r2
PUSH16	hMonitor
SP_DEC	$1
CALL	vos_dev_write
POP8	%eax
SP_WR8	%eax	$11
SP_INC	$8
JUMP	@IC232
@IC238:	
JUMP	@IC227
@IC237:	
JUMP	@IC232
@IC233:	
CMP8	%r1	$8
JNZ	@IC250
@IC251:	
SP_STORE	%r2
INC16	%r2	$0
SP_STORE	%r3
INC16	%r3	$1
PUSH16	%r3
PUSH16	$1
PUSH16	%r2
PUSH16	hMonitor
SP_DEC	$1
CALL	vos_dev_write
POP8	%eax
SP_WR8	%eax	$12
SP_INC	$8
@IC250:	
@IC232:	
CPY8	%eax	%r1
AND32	%eax	$255
CPY32	%r2	%eax
ADD16	%r2	%r0
SP_STORE	%r3
INC16	%r3	$1
PUSH16	%r3
PUSH16	$1
PUSH16	%r2
PUSH16	hMonitor
SP_DEC	$1
CALL	vos_dev_write
POP8	%eax
SP_WR8	%eax	$13
SP_INC	$8
@IC229:	
INC8	%r1	$1
JUMP	@IC226
@IC227:	
LD8	%eax	$0
SP_WR8	%eax	$25
SP_INC	$6
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monWriteFileName"

monRead:	
.GLOBAL	 DO_NOT_EXPORT  "monRead"

.FUNCTION	"monRead"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_DEC	$13
SP_RD16	%r1	$33
SP_RD16	%r0	$35
SP_STORE	%r2
INC16	%r2	$0
CPY16	%r3	%r2
LD8	(%r3)	$2
INC16	%r2	$5
LD16	(%r2)	$0
@IC254:	
SP_STORE	%r2
PUSH16	%r2
PUSH16	hMonitor
SP_DEC	$1
CALL	vos_dev_ioctl
POP8	%eax
SP_WR8	%eax	$13
SP_INC	$4
@IC255:	
SP_STORE	%r2
INC16	%r2	$5
CPY16	%r2	(%r2)
CMP16	%r2	%r0
JLT	@IC254
@IC256:	
CPY16	%r2	%r1
SP_STORE	%r3
INC16	%r3	$10
PUSH16	%r3
PUSH16	%r0
PUSH16	%r2
PUSH16	hMonitor
SP_DEC	$1
CALL	vos_dev_read
POP8	%eax
SP_WR8	%eax	$20
SP_INC	$8
SP_RD32	%ecx	$10
CMP16	%ecx	%r0
JNZ	@IC260
@IC261:	
LD8	%eax	$0
SP_WR8	%eax	$32
SP_INC	$13
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
JUMP	@IC259
@IC260:	
LD8	%eax	$3
SP_WR8	%eax	$32
SP_INC	$13
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
@IC259:	
SP_INC	$13
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monRead"

monAddNumberToConsole:	
.GLOBAL	 DO_NOT_EXPORT  "monAddNumberToConsole"

.FUNCTION	"monAddNumberToConsole"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_DEC	$8
SP_RD16	%r3	$27
SP_RD8	%r2	$29
CPY8	%eax	Mon_Flags
AND32	%eax	$255
LD32	%r0	$2
AND32	%r0	%eax
CMP32	%r0	$0
JZ	@IC265
@IC266:	
SP_STORE	%r0
INC16	%r0	$0
LD16	%r1	$0
ADD16	%r1	%r0
LD8	(%r1)	$36
INC16	%r0	$3
LD8	(%r0)	$32
@IC267:	
CMP8	%r2	$0
JZ	@IC264
@IC269:	
DEC8	%r2	$1
CPY16	%r0	%r3
INC16	%r3	$1
CPY8	%r0	(%r0)
SP_STORE	%ecx
INC16	%ecx	$4
CPY8	(%ecx)	%r0
CPY8	%eax	%r0
AND32	%eax	$255
LD32	%ebx	$4
SHR32	%r0	%eax	%ebx
SP_STORE	%ecx
INC16	%ecx	$5
CPY8	(%ecx)	%r0
SP_STORE	%ecx
INC16	%ecx	$5
CMP8	(%ecx)	$9
JLE	@IC271
@IC272:	
SP_STORE	%r0
INC16	%r0	$0
INC16	%r0	$1
SP_RD8	%ebx	$5
AND32	%ebx	$255
LD32	%eax	$55
ADD32	%r1	%eax	%ebx
CPY8	(%r0)	%r1
JUMP	@IC270
@IC271:	
SP_STORE	%r0
INC16	%r0	$0
INC16	%r0	$1
SP_RD8	%ebx	$5
AND32	%ebx	$255
LD32	%eax	$48
ADD32	%r1	%eax	%ebx
CPY8	(%r0)	%r1
@IC270:	
SP_RD8	%eax	$4
AND32	%eax	$255
LD32	%r0	$15
AND32	%r0	%eax
SP_STORE	%ecx
INC16	%ecx	$5
CPY8	(%ecx)	%r0
SP_STORE	%ecx
INC16	%ecx	$5
CMP8	(%ecx)	$9
JLE	@IC276
@IC277:	
SP_STORE	%r0
INC16	%r0	$0
INC16	%r0	$2
SP_RD8	%ebx	$5
AND32	%ebx	$255
LD32	%eax	$55
ADD32	%r1	%eax	%ebx
CPY8	(%r0)	%r1
JUMP	@IC275
@IC276:	
SP_STORE	%r0
INC16	%r0	$0
INC16	%r0	$2
SP_RD8	%ebx	$5
AND32	%ebx	$255
LD32	%eax	$48
ADD32	%r1	%eax	%ebx
CPY8	(%r0)	%r1
@IC275:	
SP_STORE	%r0
INC16	%r0	$0
PUSH16	$4
PUSH16	%r0
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$10
SP_INC	$4
JUMP	@IC267
@IC268:	
JUMP	@IC264
@IC265:	
CPY16	%r0	%r3
CPY8	%r1	%r2
AND16	%r1	$255
PUSH16	%r1
PUSH16	%r0
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$11
SP_INC	$4
@IC264:	
SP_INC	$8
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monAddNumberToConsole"

monAddMultibyteNumberToConsole:	
.GLOBAL	 DO_NOT_EXPORT  "monAddMultibyteNumberToConsole"

.FUNCTION	"monAddMultibyteNumberToConsole"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_DEC	$5
SP_RD16	%r1	$24
SP_RD8	%r0	$26
@IC280:	
CMP8	%r0	$0
JZ	@IC281
@IC282:	
DEC8	%r0	$1
CPY8	%eax	%r0
AND32	%eax	$255
CPY32	%r2	%eax
ADD16	%r2	%r1	%r2
CPY8	%r2	(%r2)
AND32	%r2	$255
SP_STORE	%ecx
INC16	%ecx	$0
CPY8	(%ecx)	%r2
SP_STORE	%eax
CPY8	%eax	(%eax)
AND32	%eax	$255
LD32	%ebx	$4
SHR32	%r2	%eax	%ebx
SP_STORE	%ecx
INC16	%ecx	$1
CPY8	(%ecx)	%r2
SP_STORE	%ecx
INC16	%ecx	$1
CMP8	(%ecx)	$9
JLE	@IC284
@IC285:	
SP_STORE	%r2
INC16	%r2	$2
INC16	%r2	$0
SP_RD8	%ebx	$1
AND32	%ebx	$255
LD32	%eax	$55
ADD32	%r3	%eax	%ebx
CPY8	(%r2)	%r3
JUMP	@IC283
@IC284:	
SP_STORE	%r2
INC16	%r2	$2
SP_RD8	%ebx	$1
AND32	%ebx	$255
LD32	%eax	$48
ADD32	%r3	%eax	%ebx
CPY8	(%r2)	%r3
@IC283:	
SP_RD8	%eax	$0
AND32	%eax	$255
LD32	%r2	$15
AND32	%r2	%eax
SP_STORE	%ecx
INC16	%ecx	$1
CPY8	(%ecx)	%r2
SP_STORE	%ecx
INC16	%ecx	$1
CMP8	(%ecx)	$9
JLE	@IC289
@IC290:	
SP_STORE	%r2
INC16	%r2	$2
INC16	%r2	$1
SP_RD8	%ebx	$1
AND32	%ebx	$255
LD32	%eax	$55
ADD32	%r3	%eax	%ebx
CPY8	(%r2)	%r3
JUMP	@IC288
@IC289:	
SP_STORE	%r2
INC16	%r2	$2
INC16	%r2	$1
SP_RD8	%ebx	$1
AND32	%ebx	$255
LD32	%eax	$48
ADD32	%r3	%eax	%ebx
CPY8	(%r2)	%r3
@IC288:	
SP_STORE	%r2
INC16	%r2	$2
PUSH16	$2
PUSH16	%r2
SP_DEC	$1
CALL	monWrite
POP8	%eax
SP_WR8	%eax	$8
SP_INC	$4
JUMP	@IC280
@IC281:	
SP_INC	$5
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monAddMultibyteNumberToConsole"

monValidateFileName:	
.GLOBAL	 DO_NOT_EXPORT  "monValidateFileName"

.FUNCTION	"monValidateFileName"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_DEC	$2
SP_RD8	%r2	$22
LD8	%ecx	$0
SP_WR8	%ecx	$0
CMP8	%r2	$2
JNZ	@IC294
@IC295:	
LD16	%r2	$param2Data
CPY16	%r0	%r2
JUMP	@IC293
@IC294:	
LD16	%r2	$param1Data
CPY16	%r0	%r2
@IC293:	
CPY8	%r2	(%r0)
CMP8	%r2	$32
JNZ	@IC298
@IC299:	
LD8	%ecx	$9
SP_WR8	%ecx	$0
@IC298:	
LD8	%ecx	$0
SP_WR8	%ecx	$1
@IC302:	
SP_STORE	%ecx
INC16	%ecx	$1
CMP8	(%ecx)	$11
JGE	@IC303
@IC304:	
CPY8	%r2	(%r0)
CPY8	%r1	%r2
CMP8	%r1	$229
JNZ	@IC308
@IC309:	
LD8	%r1	$5
@IC308:	
CMP8	%r1	$128
JGE	@IC312
@IC313:	
CPY8	%eax	%r1
AND32	%eax	$255
LD32	%ebx	$4
SHR32	%r2	%eax	%ebx
LD32	%r3	$maskCodePage
ADD32	%r2	%r3	%r2
CPYROM	%eax	%r2	$1
AND32	%eax	$65535
CPY32	%r2	%eax
CPY8	%eax	%r1
AND32	%eax	$255
LD32	%r3	$15
AND32	%r3	%eax
SHR32	%r2	%r3
AND32	%r2	$1
CMP32	%r2	$0
JNZ	@IC316
@IC317:	
LD8	%ecx	$9
SP_WR8	%ecx	$0
JUMP	@IC303
@IC316:	
CMP8	%r1	$97
JLT	@IC320
@IC322:	
CMP8	%r1	$122
JGT	@IC320
@IC321:	
CPY8	%r2	(%r0)
LD8	%ebx	$32
SUB8	%r2	%ebx
CPY8	(%r0)	%r2
@IC320:	
@IC312:	
INC16	%r0	$1
@IC305:	
SP_STORE	%eax
INC16	%eax	$1
INC8	(%eax)	$1
JUMP	@IC302
@IC303:	
SP_STORE	%eax
SP_STORE	%ecx
INC16	%ecx	$21
CPY8	(%ecx)	(%eax)
SP_INC	$2
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monValidateFileName"

monValidateNumber:	
.GLOBAL	 DO_NOT_EXPORT  "monValidateNumber"

.FUNCTION	"monValidateNumber"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_DEC	$4
SP_RD8	%r0	$25
SP_RD8	%r1	$24
CMP8	%r1	$2
JNZ	@IC328
@IC329:	
LD16	%r1	$param2
LD16	%r2	$param2Data
INC16	%r2	$0
CPY8	%r3	%r0
AND16	%r3	$255
PUSH16	%r3
PUSH16	%r2
PUSH16	%r1
SP_DEC	$2
CALL	vos_memcpy
POP16	%eax
SP_WR16	%eax	$6
SP_INC	$6
JUMP	@IC327
@IC328:	
LD16	%r1	$param1
LD16	%r2	$param1Data
CPY8	%r3	%r0
AND16	%r3	$255
PUSH16	%r3
PUSH16	%r2
PUSH16	%r1
SP_DEC	$2
CALL	vos_memcpy
POP16	%eax
SP_WR16	%eax	$8
SP_INC	$6
@IC327:	
LD8	%eax	$0
SP_WR8	%eax	$23
SP_INC	$4
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monValidateNumber"

monValidateDateTime:	
.GLOBAL	 DO_NOT_EXPORT  "monValidateDateTime"

.FUNCTION	"monValidateDateTime"	
PUSH32	%r0
SP_DEC	$1
SP_RD8	%r0	$9
PUSH8	$4
PUSH8	%r0
SP_DEC	$1
CALL	monValidateNumber
POP8	%eax
SP_WR8	%eax	$2
SP_INC	$2
SP_STORE	%eax
SP_STORE	%ecx
INC16	%ecx	$8
CPY8	(%ecx)	(%eax)
SP_INC	$1
POP32	%r0
RTS	
.FUNC_END	"monValidateDateTime"

monASCIItoDec:	
.GLOBAL	 DO_NOT_EXPORT  "monASCIItoDec"

.FUNCTION	"monASCIItoDec"	
PUSH32	%r0
SP_RD8	%r0	$8
JUMP	@IC334
@IC333:	
LD8	%eax	$0
SP_WR8	%eax	$7
POP32	%r0
RTS	
@IC335:	
LD8	%eax	$1
SP_WR8	%eax	$7
POP32	%r0
RTS	
@IC337:	
LD8	%eax	$2
SP_WR8	%eax	$7
POP32	%r0
RTS	
@IC339:	
LD8	%eax	$3
SP_WR8	%eax	$7
POP32	%r0
RTS	
@IC341:	
LD8	%eax	$4
SP_WR8	%eax	$7
POP32	%r0
RTS	
@IC343:	
LD8	%eax	$5
SP_WR8	%eax	$7
POP32	%r0
RTS	
@IC345:	
LD8	%eax	$6
SP_WR8	%eax	$7
POP32	%r0
RTS	
@IC347:	
LD8	%eax	$7
SP_WR8	%eax	$7
POP32	%r0
RTS	
@IC349:	
LD8	%eax	$8
SP_WR8	%eax	$7
POP32	%r0
RTS	
@IC351:	
LD8	%eax	$9
SP_WR8	%eax	$7
POP32	%r0
RTS	
@IC353:	
LD8	%eax	$10
SP_WR8	%eax	$7
POP32	%r0
RTS	
@IC355:	
LD8	%eax	$10
SP_WR8	%eax	$7
POP32	%r0
RTS	
@IC357:	
LD8	%eax	$11
SP_WR8	%eax	$7
POP32	%r0
RTS	
@IC359:	
LD8	%eax	$11
SP_WR8	%eax	$7
POP32	%r0
RTS	
@IC361:	
LD8	%eax	$12
SP_WR8	%eax	$7
POP32	%r0
RTS	
@IC363:	
LD8	%eax	$12
SP_WR8	%eax	$7
POP32	%r0
RTS	
@IC365:	
LD8	%eax	$13
SP_WR8	%eax	$7
POP32	%r0
RTS	
@IC367:	
LD8	%eax	$13
SP_WR8	%eax	$7
POP32	%r0
RTS	
@IC369:	
LD8	%eax	$14
SP_WR8	%eax	$7
POP32	%r0
RTS	
@IC371:	
LD8	%eax	$14
SP_WR8	%eax	$7
POP32	%r0
RTS	
@IC373:	
LD8	%eax	$15
SP_WR8	%eax	$7
POP32	%r0
RTS	
@IC375:	
LD8	%eax	$15
SP_WR8	%eax	$7
POP32	%r0
RTS	
@IC377:	
LD8	%eax	$0
SP_WR8	%eax	$7
POP32	%r0
RTS	
JUMP	@IC332
@IC334:	
CMP8	%r0	$48
JZ	@IC333
@IC336:	
CMP8	%r0	$49
JZ	@IC335
@IC338:	
CMP8	%r0	$50
JZ	@IC337
@IC340:	
CMP8	%r0	$51
JZ	@IC339
@IC342:	
CMP8	%r0	$52
JZ	@IC341
@IC344:	
CMP8	%r0	$53
JZ	@IC343
@IC346:	
CMP8	%r0	$54
JZ	@IC345
@IC348:	
CMP8	%r0	$55
JZ	@IC347
@IC350:	
CMP8	%r0	$56
JZ	@IC349
@IC352:	
CMP8	%r0	$57
JZ	@IC351
@IC354:	
CMP8	%r0	$65
JZ	@IC353
@IC356:	
CMP8	%r0	$97
JZ	@IC355
@IC358:	
CMP8	%r0	$66
JZ	@IC357
@IC360:	
CMP8	%r0	$98
JZ	@IC359
@IC362:	
CMP8	%r0	$67
JZ	@IC361
@IC364:	
CMP8	%r0	$99
JZ	@IC363
@IC366:	
CMP8	%r0	$68
JZ	@IC365
@IC368:	
CMP8	%r0	$100
JZ	@IC367
@IC370:	
CMP8	%r0	$69
JZ	@IC369
@IC372:	
CMP8	%r0	$101
JZ	@IC371
@IC374:	
CMP8	%r0	$70
JZ	@IC373
@IC376:	
CMP8	%r0	$102
JZ	@IC375
@IC378:	
JUMP	@IC377
@IC332:	
POP32	%r0
RTS	
.FUNC_END	"monASCIItoDec"

monCmdProcess:	
.GLOBAL	 DO_NOT_EXPORT  "monCmdProcess"

.FUNCTION	"monCmdProcess"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_DEC	$4
SP_RD32	%r1	$26
SP_RD8	%r0	$30
SP_RD16	%r3	$24
LD8	%ecx	$3
SP_WR8	%ecx	$0
CPY8	%eax	Mon_Flags
AND32	%eax	$255
LD32	%r2	$4
AND32	%r2	%eax
CMP32	%r2	$0
JZ	@IC379
@IC380:	
PUSH16	hUsb2
SP_DEC	$1
CALL	hostWake
POP8	%eax
SP_WR8	%eax	$3
SP_INC	$2
@IC379:	
CPY16	%eax	%r3
AND32	%eax	$65535
LD32	%ebx	$8
SHR32	%r2	%eax	%ebx
CMP32	%r2	$0
JLES	@IC381
@IC382:	
CMP8	%r0	$0
JNZ	@IC385
@IC386:	
LD8	%ecx	$10
SP_WR8	%ecx	$0
@IC385:	
@IC381:	
SP_STORE	%ecx
INC16	%ecx	$0
CMP8	(%ecx)	$10
JZ	@IC389
@IC390:	
CMP32	%r1	$0
JZ	@IC393
@IC394:	
SP_DEC	$1
SP_RD32	%ecx	$27
CALL	(%ecx)
POP8	%eax
SP_WR8	%eax	$2
SP_RD8	%ecx	$2
SP_WR8	%ecx	$0
@IC393:	
@IC389:	
CPY8	%eax	Mon_Flags
AND32	%eax	$255
LD32	%r2	$4
AND32	%r2	%eax
CMP32	%r2	$0
JZ	@IC395
@IC396:	
PUSH16	hUsb2
SP_DEC	$1
CALL	hostSuspend
POP8	%eax
SP_WR8	%eax	$5
SP_INC	$2
@IC395:	
SP_STORE	%eax
SP_STORE	%ecx
INC16	%ecx	$23
CPY8	(%ecx)	(%eax)
SP_INC	$4
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monCmdProcess"

monChkCmd:	
.GLOBAL	 DO_NOT_EXPORT  "monChkCmd"

.FUNCTION	"monChkCmd"	
PUSH32	%r0
PUSH32	%r1
PUSH32	%r2
PUSH32	%r3
SP_DEC	$12
SP_RD8	%ecx	$32
SP_WR8	%ecx	$32
LD32	%ecx	$0
SP_WR32	%ecx	$0
LD32	%ecx	$0
SP_WR32	%ecx	$4
LD8	%ecx	$0
SP_WR8	%ecx	$8
LD8	%ecx	$2
SP_WR8	%ecx	$9
CPY8	%eax	Mon_Flags
AND32	%eax	$255
LD32	%r1	$1
AND32	%r1	%eax
CMP32	%r1	$0
JNZ	@IC397
@IC398:	
@ICO0:	
LD16	%r0	$cmdData
@IC401:	
SP_STORE	%ebx
INC16	%ebx	$4
ADD16	%r1	%r0	(%ebx)
CPY8	%r1	(%r1)
CMP8	%r1	$0
JZ	@IC402
@IC403:	
LD16	%r1	$cmdData
SP_STORE	%ebx
INC16	%ebx	$4
ADD16	%r1	(%ebx)
CPY8	%r2	(%r1)
SP_STORE	%ecx
INC16	%ecx	$8
CPY8	(%ecx)	%r2
CPY8	%r1	(%r1)
CMP8	%r1	$96
JLE	@IC406
@IC408:	
LD16	%r1	$cmdData
SP_STORE	%ebx
INC16	%ebx	$4
ADD16	%r1	(%ebx)
CPY8	%r1	(%r1)
CMP8	%r1	$123
JGE	@IC406
@IC407:	
LD16	%r1	$cmdData
SP_STORE	%ebx
INC16	%ebx	$4
ADD16	%r1	(%ebx)
CPY8	%r1	(%r1)
CPY8	%eax	%r1
AND32	%eax	$255
LD32	%ebx	$32
SUB32	%r1	%eax	%ebx
SP_STORE	%ecx
INC16	%ecx	$8
CPY8	(%ecx)	%r1
@IC406:	
SP_RD32	%r1	$0
SHL32	%r1	$8
SP_RD8	%ebx	$8
SHL32	%ebx	$24
SAR32	%ebx	$24
OR32	%r1	%ebx
SP_STORE	%ecx
INC16	%ecx	$0
CPY32	(%ecx)	%r1
SP_STORE	%eax
INC16	%eax	$4
INC32	(%eax)	$1
SP_RD32	%ecx	$4
CMP32	%ecx	$4
JNZ	@IC413
@IC414:	
JUMP	@IC402
@IC413:	
JUMP	@IC401
@IC402:	
LD32	%ecx	$0
SP_WR32	%ecx	$4
@IC417:	
SP_RD32	%r1	$4
MUL32	%r1	$5
LD32	%r2	$commands
ADD32	%r1	%r2
LD32	%ebx	$0
ADD32	%r1	%ebx
CPYROM	%r1	%r1	$2
CMP32	%r1	$65535
JZ	@IC418
@IC419:	
SP_RD32	%r1	$4
MUL32	%r1	$5
LD32	%r2	$commands
ADD32	%r1	%r2	%r1
LD32	%ebx	$0
ADD32	%r1	%ebx
CPYROM	%r1	%r1	$2
SP_STORE	%eax
CMP32	%r1	(%eax)
JNZ	@IC422
@IC423:	
SP_RD32	%r1	$4
MUL32	%r1	$5
LD32	%r2	$commands
ADD32	%r3	%r2	%r1
LD32	%ebx	$2
ADD32	%r3	%ebx
CPYROM	%r3	%r3	$1
CPY32	%r2	%r2
ADD32	%r1	%r2	%r1
LD32	%ebx	$3
ADD32	%r1	%ebx
CPYROM	%r1	%r1	$2
SP_RD16	%eax	$32
PUSH8	%eax
PUSH32	%r1
PUSH16	%r3
SP_DEC	$1
CALL	monCmdProcess
POP8	%eax
SP_WR8	%eax	$17
SP_INC	$7
SP_RD8	%ecx	$10
SP_WR8	%ecx	$9
JUMP	@IC418
@IC422:	
SP_STORE	%eax
INC16	%eax	$4
INC32	(%eax)	$1
JUMP	@IC417
@IC418:	
@IC397:	
CPY8	%eax	Mon_Flags
AND32	%eax	$255
LD32	%r1	$1
AND32	%r1	%eax
CMP32	%r1	$0
JZ	@IC426
@IC427:	
LD16	%r1	$cmdData
CPY8	%r1	(%r1)
SP_STORE	%ecx
INC16	%ecx	$0
CPY8	%eax	%r1
AND32	%eax	$255
CPY32	(%ecx)	%eax
LD32	%ecx	$0
SP_WR32	%ecx	$4
@IC428:	
SP_RD32	%r1	$4
MUL32	%r1	$5
LD32	%r2	$commands
ADD32	%r1	%r2	%r1
LD32	%ebx	$0
ADD32	%r1	%ebx
CPYROM	%r1	%r1	$2
CMP32	%r1	$65535
JZ	@IC429
@IC430:	
SP_RD32	%r1	$4
MUL32	%r1	$5
LD32	%r2	$commands
ADD32	%r1	%r2	%r1
LD32	%ebx	$2
ADD32	%r1	%ebx
CPYROM	%r1	%r1	$1
CPY16	%eax	%r1
AND32	%eax	$65535
LD32	%r1	$255
AND32	%r1	%eax
SP_STORE	%eax
CMP32	%r1	(%eax)
JNZ	@IC433
@IC434:	
SP_RD32	%r1	$4
MUL32	%r1	$5
LD32	%r2	$commands
ADD32	%r3	%r2	%r1
LD32	%ebx	$2
ADD32	%r3	%ebx
CPYROM	%r3	%r3	$1
CPY32	%r2	%r2
ADD32	%r1	%r2	%r1
LD32	%ebx	$3
ADD32	%r1	%ebx
CPYROM	%r1	%r1	$2
SP_RD16	%eax	$32
PUSH8	%eax
PUSH32	%r1
PUSH16	%r3
SP_DEC	$1
CALL	monCmdProcess
POP8	%eax
SP_WR8	%eax	$18
SP_INC	$7
SP_RD8	%ecx	$11
SP_WR8	%ecx	$9
JUMP	@IC429
@IC433:	
SP_STORE	%eax
INC16	%eax	$4
INC32	(%eax)	$1
JUMP	@IC428
@IC429:	
@IC426:	
CMP8	cmdTerminator	$13
JZ	@IC437
@IC438:	
SP_STORE	%ecx
INC16	%ecx	$9
CMP8	(%ecx)	$2
JNZ	@IC441
@IC442:	
CALL	monReadCr
@IC441:	
@IC437:	
SP_RD8	%eax	$9
SP_WR8	%eax	$31
SP_INC	$12
POP32	%r3
POP32	%r2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monChkCmd"

monCheck:	
.GLOBAL	 DO_NOT_EXPORT  "monCheck"

.FUNCTION	"monCheck"	
PUSH32	%r0
PUSH32	%r1
SP_DEC	$2
SP_RD8	%r0	$13
SP_DEC	$1
CALL	monReadCmd
POP8	%eax
SP_WR8	%eax	$0
SP_STORE	%eax
CPY8	%r1	(%eax)
SP_STORE	%ecx
CMP8	(%ecx)	$0
JNZ	@IC445
@IC446:	
PUSH8	%r0
SP_DEC	$1
CALL	monChkCmd
POP8	%eax
SP_WR8	%eax	$2
SP_INC	$1
SP_RD8	%r1	$1
CMP8	%r0	$0
JZ	@IC450
@IC451:	
CALL	monLedsGreen
JUMP	@IC449
@IC450:	
CALL	monLedsOff
@IC449:	
@IC445:	
CMP8	%r1	$1
JZ	@IC452
@IC453:	
CMP8	%r1	$2
JNZ	@IC457
@IC458:	
CALL	monBadCommand
JUMP	@IC456
@IC457:	
CMP8	%r1	$3
JNZ	@IC462
@IC463:	
CALL	monCommandFailed
JUMP	@IC461
@IC462:	
CMP8	%r1	$4
JNZ	@IC467
@IC468:	
CALL	monDiskFull
JUMP	@IC466
@IC467:	
CMP8	%r1	$5
JNZ	@IC472
@IC473:	
CALL	monInvalid
JUMP	@IC471
@IC472:	
CMP8	%r1	$6
JNZ	@IC477
@IC478:	
CALL	monReadOnly
JUMP	@IC476
@IC477:	
CMP8	%r1	$7
JNZ	@IC482
@IC483:	
CALL	monFileOpen
JUMP	@IC481
@IC482:	
CMP8	%r1	$8
JNZ	@IC487
@IC488:	
CALL	monDirNotEmpty
JUMP	@IC486
@IC487:	
CMP8	%r1	$9
JNZ	@IC492
@IC493:	
CALL	monFilenameInvalid
JUMP	@IC491
@IC492:	
CMP8	%r1	$10
JNZ	@IC497
@IC498:	
CALL	monNoDisk
JUMP	@IC496
@IC497:	
CALL	monPrompt
@IC496:	
@IC491:	
@IC486:	
@IC481:	
@IC476:	
@IC471:	
@IC466:	
@IC461:	
@IC456:	
@IC452:	
SP_INC	$2
POP32	%r1
POP32	%r0
RTS	
.FUNC_END	"monCheck"

monPowerLedOn:	
.GLOBAL	 DO_NOT_EXPORT  "monPowerLedOn"

.FUNCTION	"monPowerLedOn"	
PUSH32	%r0
LD8	monFlashLED	$0
CPY8	%eax	monLEDstate
SHL32	%eax	$24
SAR32	%eax	$24
LD32	%r0	$-33
AND32	%r0	%eax
CPY8	monLEDstate	%r0
POP32	%r0
RTS	
.FUNC_END	"monPowerLedOn"

monPowerLedOff:	
.GLOBAL	 DO_NOT_EXPORT  "monPowerLedOff"

.FUNCTION	"monPowerLedOff"	
PUSH32	%r0
LD8	monFlashLED	$0
CPY8	%eax	monLEDstate
SHL32	%eax	$24
SAR32	%eax	$24
LD32	%r0	$32
OR32	%r0	%eax
CPY8	monLEDstate	%r0
POP32	%r0
RTS	
.FUNC_END	"monPowerLedOff"

monActivityLedOn:	
.GLOBAL	 DO_NOT_EXPORT  "monActivityLedOn"

.FUNCTION	"monActivityLedOn"	
PUSH32	%r0
LD8	monFlashLED	$0
CPY8	%eax	monLEDstate
SHL32	%eax	$24
SAR32	%eax	$24
LD32	%r0	$-65
AND32	%r0	%eax
CPY8	monLEDstate	%r0
POP32	%r0
RTS	
.FUNC_END	"monActivityLedOn"

monActivityLedOff:	
.GLOBAL	 DO_NOT_EXPORT  "monActivityLedOff"

.FUNCTION	"monActivityLedOff"	
PUSH32	%r0
LD8	monFlashLED	$0
CPY8	%eax	monLEDstate
SHL32	%eax	$24
SAR32	%eax	$24
LD32	%r0	$64
OR32	%r0	%eax
CPY8	monLEDstate	%r0
POP32	%r0
RTS	
.FUNC_END	"monActivityLedOff"

monLedsGreen:	
.GLOBAL	 DO_NOT_EXPORT  "monLedsGreen"

.FUNCTION	"monLedsGreen"	
PUSH32	%r0
LD8	monFlashLED	$0
CPY8	%eax	monLEDstate
SHL32	%eax	$24
SAR32	%eax	$24
LD32	%r0	$-97
AND32	%r0	%eax
CPY8	monLEDstate	%r0
CPY8	%eax	monLEDstate
SHL32	%eax	$24
SAR32	%eax	$24
LD32	%r0	$32
OR32	%r0	%eax
CPY8	monLEDstate	%r0
POP32	%r0
RTS	
.FUNC_END	"monLedsGreen"

monLedsRed:	
.GLOBAL	 DO_NOT_EXPORT  "monLedsRed"

.FUNCTION	"monLedsRed"	
PUSH32	%r0
LD8	monFlashLED	$0
CPY8	%eax	monLEDstate
SHL32	%eax	$24
SAR32	%eax	$24
LD32	%r0	$-97
AND32	%r0	%eax
CPY8	monLEDstate	%r0
CPY8	%eax	monLEDstate
SHL32	%eax	$24
SAR32	%eax	$24
LD32	%r0	$64
OR32	%r0	%eax
CPY8	monLEDstate	%r0
POP32	%r0
RTS	
.FUNC_END	"monLedsRed"

monLedsOff:	
.GLOBAL	 DO_NOT_EXPORT  "monLedsOff"

.FUNCTION	"monLedsOff"	
LD8	monFlashLED	$0
LD8	monLEDstate	$96
RTS	
.FUNC_END	"monLedsOff"

monLedsOffNow:	
.GLOBAL	 DO_NOT_EXPORT  "monLedsOffNow"

.FUNCTION	"monLedsOffNow"	
PUSH32	%r0
SP_DEC	$3
CALL	monLedsOff
SP_STORE	%r0
INC16	%r0	$0
PUSH16	%r0
PUSH8	$0
SP_DEC	$1
CALL	vos_gpio_read_port
POP8	%eax
SP_WR8	%eax	$4
SP_INC	$3
SP_STORE	%eax
CPY8	%eax	(%eax)
SHL32	%eax	$24
SAR32	%eax	$24
LD32	%r0	$96
OR32	%r0	%eax
CPY8	%r0	%r0
PUSH8	%r0
PUSH8	$0
SP_DEC	$1
CALL	vos_gpio_write_port
POP8	%eax
SP_WR8	%eax	$4
SP_INC	$2
SP_INC	$3
POP32	%r0
RTS	
.FUNC_END	"monLedsOffNow"

monLedsGreenFlash:	
.GLOBAL	 DO_NOT_EXPORT  "monLedsGreenFlash"

.FUNCTION	"monLedsGreenFlash"	
LD8	monFlashLED	$64
RTS	
.FUNC_END	"monLedsGreenFlash"

monFlashLedThread:	
.GLOBAL	 DO_NOT_EXPORT  "monFlashLedThread"

.FUNCTION	"monFlashLedThread"	
PUSH32	%r0
SP_DEC	$6
@IC501:	
LD8	%ecx	$1
CMP8	%ecx	$0
JZ	@IC502
@IC503:	
PUSH16	$125
SP_DEC	$1
CALL	vos_delay_msecs
POP8	%eax
SP_WR8	%eax	$2
SP_INC	$2
CMP8	monFlashLED	$0
JZ	@IC505
@IC506:	
SP_STORE	%r0
INC16	%r0	$1
PUSH16	%r0
PUSH8	$0
SP_DEC	$1
CALL	vos_gpio_read_port
POP8	%eax
SP_WR8	%eax	$5
SP_INC	$3
SP_RD8	%eax	$1
CPY8	%ebx	monFlashLED
AND32	%eax	$255
SHL32	%ebx	$24
SAR32	%ebx	$24
XOR32	%r0	%eax	%ebx
SP_STORE	%ecx
INC16	%ecx	$1
CPY8	(%ecx)	%r0
SP_RD16	%eax	$1
PUSH8	%eax
PUSH8	$0
SP_DEC	$1
CALL	vos_gpio_write_port
POP8	%eax
SP_WR8	%eax	$5
SP_INC	$2
JUMP	@IC501
@IC505:	
SP_STORE	%r0
INC16	%r0	$1
PUSH16	%r0
PUSH8	$0
SP_DEC	$1
CALL	vos_gpio_read_port
POP8	%eax
SP_WR8	%eax	$7
SP_INC	$3
SP_RD8	%eax	$1
AND32	%eax	$255
LD32	%r0	$-97
AND32	%r0	%eax
SP_STORE	%ecx
INC16	%ecx	$1
CPY8	(%ecx)	%r0
SP_RD8	%eax	$1
CPY8	%ebx	monLEDstate
AND32	%eax	$255
SHL32	%ebx	$24
SAR32	%ebx	$24
OR32	%r0	%eax	%ebx
SP_STORE	%ecx
INC16	%ecx	$1
CPY8	(%ecx)	%r0
SP_RD16	%eax	$1
PUSH8	%eax
PUSH8	$0
SP_DEC	$1
CALL	vos_gpio_write_port
POP8	%eax
SP_WR8	%eax	$7
SP_INC	$2
@IC504:	
JUMP	@IC501
@IC502:	
SP_INC	$6
POP32	%r0
RTS	
.FUNC_END	"monFlashLedThread"

