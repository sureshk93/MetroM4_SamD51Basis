/*
*  Boot.cpp
*
*  Created:  9/26/2019  6:07:22  PM
*  Author:  juneh
*/


#include  "clocks.h"
#include "../../common/named_phctrl.h"
#include "../../common/named_gens.h"

void  Clocks::boot()
{
  Clocks::boot_osc32k();
  Clocks::boot_dfll();
  Clocks::boot_gens_initial();
  Clocks::boot_dpll();
  Clocks::boot_gens_post();
  Clocks::boot_mclk();
  Clocks::boot_post();
}

//  Enable  and  configure  32KHz  Crystal
void  Clocks::boot_osc32k()
{
  //  Enable  RTC  Clock  and  have  it  use  32K  crystal
  OSC32KCTRL->RTCCTRL.reg  =  OSC32KCTRL_RTCCTRL_RTCSEL(4);

  //  Setup  fail-safe  clock  that  auto  switches  back  and  forth
  OSC32KCTRL->CFDCTRL.reg  =  OSC32KCTRL_CFDCTRL_CFDEN  |  OSC32KCTRL_CFDCTRL_SWBACK;

  //  Power  on  crystal  (2  pin  mode)  and  pipe  32K & 1K  output  out,  also  always  run  even  in  standby
  //  Give  a  rough  ~125ms  startup  time  in  high  gain  mode
  // Run in standby mode (This is a critical clock)
  OSC32KCTRL->XOSC32K.reg  =  OSC32KCTRL_XOSC32K_ENABLE  |  OSC32KCTRL_XOSC32K_XTALEN  |  OSC32KCTRL_XOSC32K_EN32K  | OSC32KCTRL_XOSC32K_EN1K |
  OSC32KCTRL_XOSC32K_STARTUP(1)  |  OSC32KCTRL_XOSC32K_CGM(2);

  // Enable 32K & 1K Internal
  OSC32KCTRL->OSCULP32K.bit.EN32K = true;
  OSC32KCTRL->OSCULP32K.bit.EN1K = true;

  while(OSC32KCTRL->STATUS.bit.XOSC32KRDY  ==  0);
}

//  Enable  and  configure  48MHz
void  Clocks::boot_dfll()
{
  //  Configure  DFLL  to  be  additionally  stable,  use  USB  as  clock  recovery  if  avail  and  ensure  it  doesn't
  //  actually  start  until  it's  already  been  finely  tuned
  OSCCTRL->DFLLCTRLB.reg  =  OSCCTRL_DFLLCTRLB_STABLE  |  OSCCTRL_DFLLCTRLB_USBCRM  |  OSCCTRL_DFLLCTRLB_WAITLOCK;

  //  Enable & Run in standby mode (This is a critical clock)
  OSCCTRL->DFLLCTRLA.reg  =  OSCCTRL_DFLLCTRLA_ENABLE;
  while(OSCCTRL->DFLLSYNC.bit.ENABLE);

  //  Wait  to  be  ready
  while(OSCCTRL->STATUS.bit.DFLLRDY  ==  0);
}

//  Initial  setup  of  GCLK  Generators
void  Clocks::boot_gens_initial()
{
  //  Enable  Generator  2  to  be  DFLL  48MHz  (48MHz) Max 200 MHz
  Clocks::_enable_gen(GEN_48MHz,  GCLK_GENCTRL_SRC_DFLL);

  //  Enable  Generator  3  to  be  DFLL  48MHz  /  48  (1MHz) Max 200 MHz
  Clocks::_enable_gen(GEN_1MHz,  GCLK_GENCTRL_SRC_DFLL,  48);

  //  Enable  Generator  4  to  be  OSC32K  (32.768KHz) Max 200 MHz
  Clocks::_enable_gen(GEN_32_768KHz,  GCLK_GENCTRL_SRC_XOSC32K);

  //  Enable  Generator  5  to  be  OSC32K  /  32  (1.024KHz) Max 200 MHz
  Clocks::_enable_gen(GEN_1_024KHz,  GCLK_GENCTRL_SRC_XOSC32K,  32);

  //  Enable  Generator  6  to  be  DFLL  48MHz  /  4  (12MHz) Max 200 MHz
  Clocks::_enable_gen(GEN_12MHz,  GCLK_GENCTRL_SRC_DFLL,  4);

  //  Enable  Generator  7  to  be  OSCULP32  (32.768KHz) Max 200 MHz
  Clocks::_enable_gen(GEN_32_768KHz_ULP,  GCLK_GENCTRL_SRC_OSCULP32K);

  //  Setup PCH Clocks

  // FDPLL
  Clocks::_enable_pch(PHCTRL_OSCCTRL_FDPLL0,  GEN_1MHz); // Max 3.2MHz
  Clocks::_enable_pch(PHCTRL_OSCCTRL_FDPLL1,  GEN_1MHz); // Max 3.2MHz
  Clocks::_enable_pch(PHCTRL_OSCCTRL_FDPLL0_32K,  GEN_32_768KHz); // Max 100KHz

  // TC0 (1ms timer)
  Clocks::_enable_pch(PHCTRL_TC0, GEN_1MHz); // Max 100MHz

  // Ensure there's no on-going sync amongst the entirety of GCLK
  while(GCLK->SYNCBUSY.reg);
}

//  Setup  DPLL0  to  output  120MHz  &  DPLL1  to  output  100MHz
void  Clocks::boot_dpll()
{
  Clocks::_enable_dpll(0,  119);
  Clocks::_enable_dpll(1,  99);
}

//  Pull  DPLL  0  &  1  into  generators  0  &  1
void  Clocks::boot_gens_post()
{
  //  Enable  Generator  1  to  be  DPLL  1  (100MHz)
  Clocks::_enable_gen(GEN_100MHz,  GCLK_GENCTRL_SRC_DPLL1); // Max 200MHz

  //  Enable  Generator  0  to  be  DPLL  0  (120MHz)
  Clocks::_enable_gen(GEN_120MHz,  GCLK_GENCTRL_SRC_DPLL0); // Max 200MHz Input (Output cannot be higher than 120 MHz)

  // Enable PCHCTRL

  // EIC
  Clocks::_enable_pch(PHCTRL_EIC, GEN_100MHz); // Max 100MHz

  // EVSYS Synchronous Channels
  Clocks::_enable_pch(PHCTRL_EVSYS_0, GEN_100MHz); // Max 100MHz

  // Ensure there's no on-going sync amongst the entirety of GCLK
  while(GCLK->SYNCBUSY.reg);
}

//  Select  Individual  Component  Main  Clock  Power
void  Clocks::boot_mclk()
{
  MCLK->AHBMASK.reg  =  MCLK_AHBMASK_HPB0  |  MCLK_AHBMASK_HPB1  |  MCLK_AHBMASK_HPB2  |  MCLK_AHBMASK_HPB3  |
  MCLK_AHBMASK_DSU  |		MCLK_AHBMASK_NVMCTRL  |
  (1  <<  12)/*PAC*/  |		MCLK_AHBMASK_CMCC  |	MCLK_AHBMASK_DMAC  |
  MCLK_AHBMASK_NVMCTRL_CACHE  |  MCLK_AHBMASK_NVMCTRL_SMEEPROM  | /*MCLK_AHBMASK_QSPI |*/

  //  Reserved  Bits (Have to be written to as 1)
  (1  <<  5)  |  (1  <<  7)  |  (1  <<  11);

  MCLK->APBAMASK.reg  =  MCLK_APBAMASK_GCLK  |		MCLK_APBAMASK_PAC  |			MCLK_APBAMASK_PM  |		MCLK_APBAMASK_MCLK  |
  MCLK_APBAMASK_RSTC  |		MCLK_APBAMASK_OSCCTRL  |		MCLK_APBAMASK_OSC32KCTRL  |	MCLK_APBAMASK_SUPC  |
  MCLK_APBAMASK_EIC  |		MCLK_APBAMASK_RTC | MCLK_APBAMASK_TC0 | MCLK_APBAMASK_RTC;

  MCLK->APBBMASK.reg  =  MCLK_APBBMASK_DSU  |	MCLK_APBBMASK_EVSYS  |	MCLK_APBBMASK_NVMCTRL  |	MCLK_APBBMASK_PORT |
  MCLK_APBBMASK_RAMECC;

  MCLK->APBCMASK.reg  =  0 /*MCLK_APBCMASK_QSPI*/;

  MCLK->APBDMASK.reg  = 0;
}

// Setup power-saving configuration for the clocks
void Clocks::boot_post()
{
  // Mark critical clocks to run always even in standby
  OSCCTRL->DFLLCTRLA.bit.RUNSTDBY = true;
  OSCCTRL->Dpll[0].DPLLCTRLA.bit.RUNSTDBY = true;
  OSCCTRL->Dpll[1].DPLLCTRLA.bit.RUNSTDBY = true;
  OSC32KCTRL->XOSC32K.bit.RUNSTDBY = true;

  // Mark generators to run in standby
  GCLK->GENCTRL[GEN_120MHz].bit.RUNSTDBY = true; // Main Generator (System Clock)
  GCLK->GENCTRL[GEN_100MHz].bit.RUNSTDBY = true; // Secondary Generator
  GCLK->GENCTRL[GEN_48MHz].bit.RUNSTDBY = true; // DFLL Generator (Basic Clock)
  GCLK->GENCTRL[GEN_1MHz].bit.RUNSTDBY = true; // 1MHz Generator (Feeds to DPLL)
  GCLK->GENCTRL[GEN_32_768KHz].bit.RUNSTDBY = true; // OSC32K Generator (Basic Clock)
  GCLK->GENCTRL[GEN_1_024KHz].bit.RUNSTDBY = true; // 1KHz Generator (Basic Frequency)
  GCLK->GENCTRL[GEN_32_768KHz_ULP].bit.RUNSTDBY = true; // OSCULP32K Generator (Basic Clock)
}

//  Enables  and  configures  a  generator
void  Clocks::_enable_gen(var8  ind,  var8  src,  var16  div  /*=  1*/)
{
  GCLK->GENCTRL[ind].reg  =  GCLK_GENCTRL_IDC  |  GCLK_GENCTRL_GENEN  |  src  |  GCLK_GENCTRL_DIV(div);
  while(GCLK->GENCTRL[ind].bit.SRC  !=  src  ||  GCLK->GENCTRL[ind].bit.GENEN  ==  false);
}

void  Clocks::_enable_pch(var8  ind,  var8  src)
{
  GCLK->PCHCTRL[ind].reg  =  GCLK_PCHCTRL_GEN(src)  |  GCLK_PCHCTRL_CHEN;
  while(GCLK->PCHCTRL[ind].bit.GEN  !=  src  ||  GCLK->PCHCTRL[ind].bit.CHEN  ==  false);
}

void  Clocks::_enable_dpll(var8  ind,  var32  ldr)
{
  //  DPLL  n  uses  GCLK  Input  and  bypasses  fine  lock  stage
  OSCCTRL->Dpll[ind].DPLLCTRLB.reg  =  OSCCTRL_DPLLCTRLB_REFCLK_GCLK  |  OSCCTRL_DPLLCTRLB_LBYPASS;

  //  Multiply
  OSCCTRL->Dpll[ind].DPLLRATIO.reg  =  OSCCTRL_DPLLRATIO_LDR(ldr);
  while(OSCCTRL->Dpll[ind].DPLLSYNCBUSY.bit.DPLLRATIO);

  //  Enable and allow to run in standby mode on demand only
  OSCCTRL->Dpll[ind].DPLLCTRLA.reg  =  OSCCTRL_DPLLCTRLA_ENABLE;
  while(OSCCTRL->Dpll[ind].DPLLSYNCBUSY.bit.ENABLE);

  //  Wait  for  coarse  lock  (fine  lock  disabled)
  while(OSCCTRL->Dpll[ind].DPLLSTATUS.bit.CLKRDY  ==  0  ||  OSCCTRL->Dpll[ind].DPLLSTATUS.bit.LOCK  ==  0);
}
