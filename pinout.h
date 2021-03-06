/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */


// a/o v95:
// This file was pieced together, long ago, from various sources.
//
// It's somewhat impossible to make an all-inclusive pinout file, at this
// point, since there are so many different options.
//
// Pseudo-FPD-Link-compatible AVRs:
// * ATtiny861		** implemented **
// * AT90PWM161	** implemented ** 
// (* AT90PWMxxx	Not difficult to implement using AT90PWM161 as a
// 					 starting-point)
// (* ATxmega(?)	Might work with *low-speed* LVDS/FPD-Link, as the
// 					 dead-time generators don't run at PLL speeds)
// 
// Parallel-LCD-compatible AVRs (6-bit, implemented):
// * ATtiny861		** implemented **
// (* ANY!			Not difficult to port to another chip)
//
// Sensors:
// * Piezo hit-detector   \  OPTIONALLY
// * Bump-switch           > Used in FB_Question for "hit-detection"
// * Momentary-pushbutton /
// * Potentiometer        -  Used in some games (e.g. SEG_RACER)
//                          (Also "BLUE_ADC" which is like an oscilloscope)

// Pseudo-FPD-Link:	(see _interfaces/lvds(161).c/h for more info)
//   (aka Pulse-Width-Banged FPD-Link/LVDS):
//
//   The AVR's outputs are single-ended (either high or low), but LVDS
//   requires differential voltages.
//   So, we need a Buffer and an Inverter tied to the AVR's outputs.
//
//   It's probably best to use two XORs from the same chip for each
//    LVDS channel, since different TTL chips may have slightly different
//    timing/voltage-output characteristics. 
//
//   The entire circuit, thus, requires TWO 74LS86's 
//    (four XORs apiece, two per LVDS channel, 8-total)
//
//                                                              __________
//              VCC3V3   VCC3V3                                | LCD
//                |        |                                   |
//                +---\ \¯¯¯-_       100ohm OPTIONAL See below |
//                     | |    ¯-                               |
//                     | | XOR   >---/\/\/\-------> RXinN/clk- |---+
//   AVR               | |    _-                               |   |
//   OC1x >----+------/ /___-¯                                 |   |
//   output    |                                               |   \ 100ohm
//             |                                               |   / built-
//             |                                               |   \ in to 
//             `------\ \¯¯¯-_       100ohm OPTIONAL See below |   / LCD
//                     | |    ¯-                               |   |
//                     | | XOR   >---/\/\/\-------> RXinN/clk+ |---+
//                     | |    _-                               |
//                +---/ /___-¯                                 |
//                |        |
//               GND      GND
//
// In my experiments:
// The XOR outputs are wired directly to the LCDs' LVDS inputs
// It's explained elsewhere, but these particular LS chips underdriven at
//  3.3V and overloaded with 100ohms (in the LCD) causes damned-near 
//  perfect voltage-levels for LVDS.
//  IN OTHER WORDS: As-Is, in my setup, with LS's from 1980, the 100ohm
//   resistors are *not* used. And I have not tested use of them.
//
// IT IS WISE TO TEST THE VOLTAGE OUTPUT LEVELS OF YOUR XOR CHIPS
//  BEFORE CONNECTING THE LCD by connecting a 100ohm resistor
//  between the + and - XOR outputs.
//  Newer chips probably have higher drive ability, so this is wise even
//  with 74LS chips newer than 1980 (the ones I used)
//  I suppose if the voltage is too high, then you could use a resistor on
//  each output (in series with the LCD inputs, as shown)... 
//  Ideally the voltage across that resistor should be no more than +/- 1V
//  probably 100ohms on each output pin?
//  No promises, here...

//###############################
//#
//# ATtiny861 + FPD-Link/LVDS
//#
//# OC1x outputs:
//#  These might be listed differently in various places... I went through
//#  many iterations, trying to find the most useful/versatile. Latest is:
//#   (This has been pretty-well verified, e.g. lvds_timerInit())
//#   PB3   OC1B -> LVDS-CLK           (RXclk)
//#   PB2  /OC1B -> LVDS-"Green"       (RXin1) (OUTPUT IS INVERTED)
//#   PB5   OC1D -> LVDS-"Red"         (RXin0)
//#   PB1   OC1A -> LVDS-"D/V/H/Blue"  (RXin2)
//#
//# GREEN needs to be inverted... 
//# Ihis is easily done by swapping the LCD's RXin1+ and RXin1- inputs 
//# to the opposite outputs of the respective XORs
//###############################


//###############################
//#
//#  ATtiny861 Programming Header (+FPD-Link/LVDS)
//#
//# I use a 6-pin single-row .1in connector for programming-headers...
//# (yours may vary)
//#
//#          Typical Other Use       Pin   FPD-Link Use
//#  -------------------------------------------------
//# 1  GND
//# 2  V+         
//# 3  SCK   (polled_uar: Rx0)       PB2   FPD-Link Green (inverted)
//# 4  MOSI  (polled_uat: Tx0)       PB0   "Button" which...?
//# 5  /RST  
//# 6  MISO  (Heart LED/pushbutton)  PB1   FPD-Link DVH/Blue
//#
//# Because the In-System-Programming pins (MISO/MOSI/SCK) are ALSO the
//#  PWM outputs (OC1x), which are necessary for FPD-Link, the
//#  programming-header pins can not be used for their "typical other use"
//#
//###############################


// A/O v80: THIS IS NO LONGER ALWAYS THE RELEVENT PINOUT
//          e.g. if using the parallel Sony LCD, check
//          _interfaces/6bitParallel.c for the pinout.


//###############################
//#
//#    ATtiny861 + FPD-Link/LVDS   Full Pinout:
//#
// Also, why are there two "Button" inputs?
// As far as I'm aware the only button that's implemented is PA6
//
//  TO LCD Buffers
//   |                       ATtiny861
//   V                       ____________________
//                         |         |_|        |
//       "Button" / MOSI --|  1 PB0      PA0 20 |-- 
// DVHBlue (OC1A) / MISO --|  2 PB1      PA1 19 |-- 
// -Green (/OC1B) / SCK  --|  3 PB2      PA2 18 |-- 
// Clock   (OC1B)  --------|  4 PB3      PA3 17 |-- 
//                   VCC --|  5 VCC     AGND 16 |-- GND
//                   GND --|  6 GND     AVCC 15 |-- VCC
//                       --|  7 PB4      PA4 14 |--      
//   Red   (OC1D)  ----- --|  8 PB5      PA5 13 |--        
//                       --|  9 PB6      PA6 12 |-- ---- Heartbeat/Button
//                /Reset --| 10 PB7      PA7 11 |-- <--.
//                         |____________________|      |
//                                                     |
//     PA7 is wired differently depending on the purpose:
//                    PA7 = ADC6/AIN1... used for the BUMP_SENSOR in 
//                                       FB_QUESTION...
//                                       Also PIEZO_HIT_DETECTOR.
//                                       Also Potentiometer for SEG_RACER.
//                                                     |
//                                                     |  VCC
//                                                     |   |
//                                                     |   \  Potentiometer
//                                                     '-->/  5k
//                                                         \  for "Racer"
//                                                         |  control
//                                                        GND
//
// (It can be left completely unwired in intial-testing)
//
// HEARTBEAT is ALSO used as a momentary-pushbutton "hit-detector"
// for FB_QUESTION. 

//#######################################
//#
//# Heartbeat LED + Momentary Pushbutton:
//#
//# (Typically connected to the same pin as the programming-header's MISO
//#  so a pushbutton can be easily-added to any project... But that's 
//#  not always possible.)
//#
//#  AVR            LED           
//#  Pin ><-----+---|<|---/\/\/\----< +V
//#             |
//#             O |
//#               |-
//#             O |
//#             |
//#            GND
//#
//#######################################


//This bit has been added to _commonCode.../piezoHitDetector...
// FOR FB_QUESTION:
//   Piezo element
//   (contact-mic/
//    "hit-sensor")    3.3V 3.3V
//                      |    |
//                      \   _|__
//    ______            /    /\
//   /  __  \           \   /__\
//  |  /  \  |    ||    |    |
//  | |   o-------||----+----+-------> PA7
//  |  \__/  |    ||    |    |
//   \o_____/           \   _|__
//    |                 /    /\
//   GND                \   /__\
//                      |    |
//                     GND  GND
//
// AGAIN: This whole hit-sensor is completely hokey... the analog threshold
// values in main.c certainly would need to be changed according to your
// circuit. And, even after fine-tuning, mine acts differently in different
// temperatures, etc. It's *very* sensitive to electrical interference
// (ended up shielding *everything*). And, even viewing it on the 'scope,
// the actual "hit" is barely discernable from just regular noise in the
// circuit. Certainly the code could stand to be improved. You might be
// best-off replacing it with a button.
//

// Heartbeat's momentary pushbutton input is used in "SEG_QUESTION" mode. 
// Shorting the pin to ground momentarily causes the image to change.



/* mehPL:
 *    I would love to believe in a world where licensing shouldn't be
 *    necessary; where people would respect others' work and wishes, 
 *    and give credit where it's due. 
 *    A world where those who find people's work useful would at least 
 *    send positive vibes--if not an email.
 *    A world where we wouldn't have to think about the potential
 *    legal-loopholes that others may take advantage of.
 *
 *    Until that world exists:
 *
 *    This software and associated hardware design is free to use,
 *    modify, and even redistribute, etc. with only a few exceptions
 *    I've thought-up as-yet (this list may be appended-to, hopefully it
 *    doesn't have to be):
 * 
 *    1) Please do not change/remove this licensing info.
 *    2) Please do not change/remove others' credit/licensing/copyright 
 *         info, where noted. 
 *    3) If you find yourself profiting from my work, please send me a
 *         beer, a trinket, or cash is always handy as well.
 *         (Please be considerate. E.G. if you've reposted my work on a
 *          revenue-making (ad-based) website, please think of the
 *          years and years of hard work that went into this!)
 *    4) If you *intend* to profit from my work, you must get my
 *         permission, first. 
 *    5) No permission is given for my work to be used in Military, NSA,
 *         or other creepy-ass purposes. No exceptions. And if there's 
 *         any question in your mind as to whether your project qualifies
 *         under this category, you must get my explicit permission.
 *
 *    The open-sourced project this originated from is ~98% the work of
 *    the original author, except where otherwise noted.
 *    That includes the "commonCode" and makefiles.
 *    Thanks, of course, should be given to those who worked on the tools
 *    I've used: avr-dude, avr-gcc, gnu-make, vim, usb-tiny, and 
 *    I'm certain many others. 
 *    And, as well, to the countless coders who've taken time to post
 *    solutions to issues I couldn't solve, all over the internets.
 *
 *
 *    I'd love to hear of how this is being used, suggestions for
 *    improvements, etc!
 *         
 *    The creator of the original code and original hardware can be
 *    contacted at:
 *
 *        EricWazHung At Gmail Dotcom
 *
 *    This code's origin (and latest versions) can be found at:
 *
 *        https://code.google.com/u/ericwazhung/
 *
 *    The site associated with the original open-sourced project is at:
 *
 *        https://sites.google.com/site/geekattempts/
 *
 *    If any of that ever changes, I will be sure to note it here, 
 *    and add a link at the pages above.
 *
 * This license added to the original file located at:
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/pinout.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */
