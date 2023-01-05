/****************************************************************************************************************************
  Ethernet_Generic_Debug.h
  
  Ethernet_Generic is a library for the W5x00 Ethernet shields trying to merge the good features of
  previous Ethernet libraries
  
  Based on and modified from 
  
  1) Ethernet Library         https://github.com/arduino-libraries/Ethernet
  2) EthernetLarge Library    https://github.com/OPEnSLab-OSU/EthernetLarge
  3) Ethernet2 Library        https://github.com/adafruit/Ethernet2
  4) Ethernet3 Library        https://github.com/sstaub/Ethernet3
    
  Built by Khoi Hoang https://github.com/khoih-prog/EthernetWebServer
  
  Version: 2.3.0
    
  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  2.0.0   K Hoang      31/03/2022 Initial porting and coding to support SPI2, debug, h-only library
  2.0.1   K Hoang      08/04/2022 Add support to SPI1 for RP2040 using arduino-pico core
  2.1.0   K Hoang      22/04/2022 Add support to WIZNet W5100S
  2.2.0   K Hoang      02/05/2022 Add support to custom SPI for any board, such as STM32
  2.3.0   K Hoang      03/05/2022 Add support to custom SPI for RP2040, Portenta_H7, etc. using Arduino-mbed core
 *****************************************************************************************************************************/

#pragma once

#include <stdio.h>

#ifdef DEBUG_ETHERNET_GENERIC_PORT
#define ETG_DEBUG_OUTPUT 			DEBUG_ETHERNET_GENERIC_PORT
#else
#define ETG_DEBUG_OUTPUT 			Serial
#endif

// Change _ETG_LOGLEVEL_ to set tracing and logging verbosity
// 0: DISABLED: no logging
// 1: ERROR: errors
// 2: WARN: errors and warnings
// 3: INFO: errors, warnings and informational (default)
// 4: DEBUG: errors, warnings, informational and debug

#ifndef _ETG_LOGLEVEL_
	#define _ETG_LOGLEVEL_       1
#endif

///////////////////////////////////////

const char ETG_MARK[]  = "[ETG] ";
const char ETG_SPACE[] = " ";
const char ETG_LINE[]  = "========================================\n";

#define ETG_PRINT_MARK   ETG_PRINT(ETG_MARK)
#define ETG_PRINT_SP     ETG_PRINT(ETG_SPACE)
#define ETG_PRINT_LINE   ETG_PRINT(ETG_LINE)

#define ETG_PRINT        ETG_DEBUG_OUTPUT.print
#define ETG_PRINTLN      ETG_DEBUG_OUTPUT.println

///////////////////////////////////////

#define ETG_LOGERROR(x)         if(_ETG_LOGLEVEL_>0) { ETG_PRINT_MARK; ETG_PRINTLN(x); }
#define ETG_LOGERROR_LINE(x)    if(_ETG_LOGLEVEL_>0) { ETG_PRINT_MARK; ETG_PRINTLN(x); ETG_PRINT_LINE; }
#define ETG_LOGERROR0(x)        if(_ETG_LOGLEVEL_>0) { ETG_PRINT(x); }
#define ETG_LOGERROR1(x,y)      if(_ETG_LOGLEVEL_>0) { ETG_PRINT_MARK; ETG_PRINT(x); ETG_PRINT_SP; ETG_PRINTLN(y); }
#define ETG_LOGERROR2(x,y,z)    if(_ETG_LOGLEVEL_>0) { ETG_PRINT_MARK; ETG_PRINT(x); ETG_PRINT_SP; ETG_PRINT(y); ETG_PRINT_SP; ETG_PRINTLN(z); }
#define ETG_LOGERROR3(x,y,z,w)  if(_ETG_LOGLEVEL_>0) { ETG_PRINT_MARK; ETG_PRINT(x); ETG_PRINT_SP; ETG_PRINT(y); ETG_PRINT_SP; ETG_PRINT(z); ETG_PRINT_SP; ETG_PRINTLN(w); }
#define ETG_LOGERROR5(x,y,z,w, xx, yy)  if(_ETG_LOGLEVEL_>0) { ETG_PRINT_MARK; ETG_PRINT(x); ETG_PRINT_SP; ETG_PRINT(y); ETG_PRINT_SP; ETG_PRINT(z); ETG_PRINT_SP; ETG_PRINT(w); ETG_PRINT_SP; ETG_PRINT(xx); ETG_PRINT_SP; ETG_PRINTLN(yy);}

///////////////////////////////////////

#define ETG_LOGWARN(x)          if(_ETG_LOGLEVEL_>1) { ETG_PRINT_MARK; ETG_PRINTLN(x); }
#define ETG_LOGWARN_LINE(x)     if(_ETG_LOGLEVEL_>1) { ETG_PRINT_MARK; ETG_PRINTLN(x); ETG_PRINT_LINE; }
#define ETG_LOGWARN0(x)         if(_ETG_LOGLEVEL_>1) { ETG_PRINT(x); }
#define ETG_LOGWARN1(x,y)       if(_ETG_LOGLEVEL_>1) { ETG_PRINT_MARK; ETG_PRINT(x); ETG_PRINT_SP; ETG_PRINTLN(y); }
#define ETG_LOGWARN2(x,y,z)     if(_ETG_LOGLEVEL_>1) { ETG_PRINT_MARK; ETG_PRINT(x); ETG_PRINT_SP; ETG_PRINT(y); ETG_PRINT_SP; ETG_PRINTLN(z); }
#define ETG_LOGWARN3(x,y,z,w)   if(_ETG_LOGLEVEL_>1) { ETG_PRINT_MARK; ETG_PRINT(x); ETG_PRINT_SP; ETG_PRINT(y); ETG_PRINT_SP; ETG_PRINT(z); ETG_PRINT_SP; ETG_PRINTLN(w); }
#define ETG_LOGWARN5(x,y,z,w, xx, yy)  if(_ETG_LOGLEVEL_>1) { ETG_PRINT_MARK; ETG_PRINT(x); ETG_PRINT_SP; ETG_PRINT(y); ETG_PRINT_SP; ETG_PRINT(z); ETG_PRINT_SP; ETG_PRINT(w); ETG_PRINT_SP; ETG_PRINT(xx); ETG_PRINT_SP; ETG_PRINTLN(yy);}

///////////////////////////////////////

#define ETG_LOGINFO(x)          if(_ETG_LOGLEVEL_>2) { ETG_PRINT_MARK; ETG_PRINTLN(x); }
#define ETG_LOGINFO_LINE(x)     if(_ETG_LOGLEVEL_>2) { ETG_PRINT_MARK; ETG_PRINTLN(x); ETG_PRINT_LINE; }
#define ETG_LOGINFO0(x)         if(_ETG_LOGLEVEL_>2) { ETG_PRINT(x); }
#define ETG_LOGINFO1(x,y)       if(_ETG_LOGLEVEL_>2) { ETG_PRINT_MARK; ETG_PRINT(x); ETG_PRINT_SP; ETG_PRINTLN(y); }
#define ETG_LOGINFO2(x,y,z)     if(_ETG_LOGLEVEL_>2) { ETG_PRINT_MARK; ETG_PRINT(x); ETG_PRINT_SP; ETG_PRINT(y); ETG_PRINT_SP; ETG_PRINTLN(z); }
#define ETG_LOGINFO3(x,y,z,w)   if(_ETG_LOGLEVEL_>2) { ETG_PRINT_MARK; ETG_PRINT(x); ETG_PRINT_SP; ETG_PRINT(y); ETG_PRINT_SP; ETG_PRINT(z); ETG_PRINT_SP; ETG_PRINTLN(w); }
#define ETG_LOGINFO5(x,y,z,w, xx, yy)  if(_ETG_LOGLEVEL_>2) { ETG_PRINT_MARK; ETG_PRINT(x); ETG_PRINT_SP; ETG_PRINT(y); ETG_PRINT_SP; ETG_PRINT(z); ETG_PRINT_SP; ETG_PRINT(w); ETG_PRINT_SP; ETG_PRINT(xx); ETG_PRINT_SP; ETG_PRINTLN(yy);}

///////////////////////////////////////

#define ETG_LOGDEBUG(x)         if(_ETG_LOGLEVEL_>3) { ETG_PRINT_MARK; ETG_PRINTLN(x); }
#define ETG_LOGDEBUG_LINE(x)    if(_ETG_LOGLEVEL_>3) { ETG_PRINT_MARK; ETG_PRINTLN(x); ETG_PRINT_LINE; }
#define ETG_LOGDEBUG0(x)        if(_ETG_LOGLEVEL_>3) { ETG_PRINT(x); }
#define ETG_LOGDEBUG1(x,y)      if(_ETG_LOGLEVEL_>3) { ETG_PRINT_MARK; ETG_PRINT(x); ETG_PRINT_SP; ETG_PRINTLN(y); }
#define ETG_LOGHEXDEBUG1(x,y)   if(_ETG_LOGLEVEL_>3) { ETG_PRINT_MARK; ETG_PRINT(x); ETG_PRINT_SP; ETG_PRINTLN(y, HEX); }
#define ETG_LOGDEBUG2(x,y,z)    if(_ETG_LOGLEVEL_>3) { ETG_PRINT_MARK; ETG_PRINT(x); ETG_PRINT_SP; ETG_PRINT(y); ETG_PRINT_SP; ETG_PRINTLN(z); }
#define ETG_LOGDEBUG3(x,y,z,w)  if(_ETG_LOGLEVEL_>3) { ETG_PRINT_MARK; ETG_PRINT(x); ETG_PRINT_SP; ETG_PRINT(y); ETG_PRINT_SP; ETG_PRINT(z); ETG_PRINT_SP; ETG_PRINTLN(w); }
#define ETG_LOGDEBUG5(x,y,z,w, xx, yy)  if(_ETG_LOGLEVEL_>3) { ETG_PRINT_MARK; ETG_PRINT(x); ETG_PRINT_SP; ETG_PRINT(y); ETG_PRINT_SP; ETG_PRINT(z); ETG_PRINT_SP; ETG_PRINT(w); ETG_PRINT_SP; ETG_PRINT(xx); ETG_PRINT_SP; ETG_PRINTLN(yy);}

