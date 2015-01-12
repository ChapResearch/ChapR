/*
** cmdmon.h
**
** Copyright © 2010-2011 Future Technology Devices International Limited
**
** C Header file for Vinculum II V2DAP Commands
**
** Author: FTDI
** Project: Vinculum II
** Module: Command Monitor
** Requires: VOS
** Comments: Refer to VNC1L Firmware User Manual for detailed command information
**
** History:
**  1 – Initial version
**
*/
#ifndef __cmdmon_h__
#define __cmdmon_h__

/* Monitor Functions : cmdconfig.c */
unsigned char cmd_cr();
unsigned char cmd_fwv();
unsigned char cmd_echo();
unsigned char cmd_scs();
unsigned char cmd_ecs();
unsigned char cmd_ipa();
unsigned char cmd_iph();
unsigned char cmd_sbd();
unsigned char cmd_ior();
unsigned char cmd_iow();
unsigned char cmd_sd();
unsigned char cmd_sw();
unsigned char cmd_fwu();
unsigned char cmd_dri();
unsigned char cmd_ini();
/* Disk Operations : cmddisk.c */
unsigned char cmd_dir();
unsigned char cmd_cd();
unsigned char cmd_rd();
unsigned char cmd_dld();
unsigned char cmd_mkd();
unsigned char cmd_dlf();
unsigned char cmd_wrf();
unsigned char cmd_opw();
unsigned char cmd_clf();
unsigned char cmd_rdf();
unsigned char cmd_ren();
unsigned char cmd_opr();
unsigned char cmd_sek();
unsigned char cmd_fs();
unsigned char cmd_fse();
unsigned char cmd_idd();
unsigned char cmd_idde();
unsigned char cmd_dsn();
unsigned char cmd_dvl();
unsigned char cmd_dirt();
/* Power Management Commands :cmdpwr.c */
unsigned char cmd_sud();
unsigned char cmd_wkd();
unsigned char cmd_sum();
/* Printer Commands : cmdprinter.c */
unsigned char cmd_pgs();
unsigned char cmd_psr();
/* USB Device Commands : cmdusb.c */
unsigned char cmd_qp1();
unsigned char cmd_qp2();
unsigned char cmd_qd();
unsigned char cmd_sc();
unsigned char cmd_dsd();
unsigned char cmd_drd();
unsigned char cmd_ssu();
unsigned char cmd_sf();
unsigned char cmd_qss();
/* Commands to FTDI Device : cmdft.c */
unsigned char cmd_fbd();
unsigned char cmd_fmc();
unsigned char cmd_fsd();
unsigned char cmd_ffc();
unsigned char cmd_fgm();
unsigned char cmd_fsl();
unsigned char cmd_fsb();
unsigned char cmd_fgb();
/* VMusic Commands : cmdvmusic.c */
unsigned char cmd_vpf();
unsigned char cmd_vrf();
unsigned char cmd_vst();
unsigned char cmd_v3a();
unsigned char cmd_vra();
unsigned char cmd_vrr();
unsigned char cmd_vsf();
unsigned char cmd_vsb();
unsigned char cmd_vsd();
unsigned char cmd_vp();
unsigned char cmd_vf();
unsigned char cmd_vb();
unsigned char cmd_vrd();
unsigned char cmd_vwr();
unsigned char cmd_vsv();
/* VDFC Commands : cmdvdfc.c */
unsigned char cmd_a();
unsigned char cmd_b();
unsigned char cmd_ims();
unsigned char cmd_imf();
unsigned char cmd_cps();
unsigned char cmd_cpf();

#endif                                 // __cmdmon_h__

