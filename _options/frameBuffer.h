/* mehPL:
 *    This is Open Source, but NOT GPL. I call it mehPL.
 *    I'm not too fond of long licenses at the top of the file.
 *    Please see the bottom.
 *    Enjoy!
 */





#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__


// 30 < MAIN_MS < 300
#if (MAIN_MS < 30)
#error "WTF?"
#endif




//Please see writeColor.c


#if(defined(FB_REFRESH_ON_CHANGE) && FB_REFRESH_ON_CHANGE)
extern volatile uint8_t updateFrame;

void restartFrameUpdate(void);
#endif

//drawPix from program memory
//a/0 v60: Again, unused for its original purpose, but tightly intertwined
// in old code that's still being (mis)used...
#ifndef FB_WIDTH
 #define FB_WIDTH 16
 #error "This error a/o v80, just noting that FB_WIDTH wasn't previously defined elsewhere..."
#endif
#ifndef FB_HEIGHT
 #define FB_HEIGHT 16
#endif

//FB_WIDTH+1 is a hack for displaying the last pixel equal-length...
// see writeColor() notes in nonRSB_drawPix()
extern uint8_t frameBuffer[FB_HEIGHT][FB_WIDTH+1];
/*	This does NOT init just these elements, but ALL elements
	Which takes up a TON of program-space.
	Moved to frameBufferInit();
	= 
{ 
	[0 ... FB_HEIGHT-1][FB_WIDTH]=_W
};
*/
/*
{ 
	[0][FB_WIDTH]=_W,
	[1][FB_WIDTH]=_W,
	[2][FB_WIDTH]=_W,
	[3][FB_WIDTH]=_W,
	[4][FB_WIDTH]=_W,
	[5][FB_WIDTH]=_W,
	[6][FB_WIDTH]=_W,
	[7][FB_WIDTH]=_W,
	[8][FB_WIDTH]=_W,
	[9][FB_WIDTH]=_W,
	[10][FB_WIDTH]=_W,
	[11][FB_WIDTH]=_W,
	[12][FB_WIDTH]=_W,
	[13][FB_WIDTH]=_W,
	[14][FB_WIDTH]=_W,
	[15][FB_WIDTH]=_W
}
*/


//AHHH it wasn't IMAGE_BUFFER that's entangled, but IMAGE_CHANGE
// which is even weirder.

//This was #if'd into existence with IMAGE_BUFFER, but since it's only been
// used with frameBuffer, and since IMAGE_BUFFER is somewhat entangled with
// newer stuff, I'm putting it here and NOT including this file, (yet)
void setColor(uint8_t red, uint8_t green, uint8_t blue, 
               uint8_t row, uint8_t col);


//Called as: pgm_readImageByte(pgm_image1, row, col)
#define pgm_readImageByte(image, row, col)   \
      pgm_read_byte((uint8_t *)(&((image)[(row)*FB_WIDTH+(col)])))


//a/o v86: I can't recall why I chose white to display at the end of the
//row... it may have had to do with the lvds displays and the timing stuff,
//so I could actually see where the DE data was ending...(?)
// Either way, I'd prefer it to be black in this particular instance, so
// FB_END_OF_ROW_COLOR is now predefinable...
#ifndef FB_END_OF_ROW_COLOR
#define FB_END_OF_ROW_COLOR	_W
#endif

// This stuff was located in main() before the while loop...
// #if !COLOR_BAR_SCROLL... (which is *nearly every case* including 
//  RowSegBuffer, etc)
// it's long-since been commented-out...
void frameBufferInit(void);


//#include "_options/smiley.c"


static __inline__
void frameBufferChange(uint8_t *bufferChanged, 
							uint8_t row, uint8_t col, uint8_t color)
	__attribute__((__always_inline__));


//This isn't especially necessary to write to the frameBuffer
// ...since it's global, it could be done directly
// but this allows for testing whether a change occurs
// in case it's desired to only refresh when that happens.
//Since it's always-inline, it should optimize-out a few things, e.g. the
//NULL-test, and even the write to bufferChanged, if it is null.
// so it should be pretty quick.
// It does NOT have a return-value, instead it only modifies bufferChanged
// IF there is a change, so e.g. it could be run in a for-loop
// and the end-value of bufferChanged will either be FALSE if no change, or
// TRUE if there was a change in any of the loops.

#define frameBufferSet(row, col, color) \
				frameBufferChange(NULL, (row), (col), (color))



// #if(defined(FB_REFRESH_ON_CHANGE) && FB_REFRESH_ON_CHANGE)
//This is sorta handled in heartbeat... and maybe it'd make sense to use
//its timer-stuff instead, in a general sense... something to consider
#if(defined(_HEART_DMS_) && _HEART_DMS_)
 #define fb_timer_t			dms4day_t
 #define fb_getTime()		dmsGetTime()
 #define fb_isItTime(a,b)	dmsIsItTimeV2((a),(b),FALSE)
#elif(defined(_HEART_TCNTER_) && _HEART_TCNTER_)
 #define fb_timer_t			myTcnter_t
 #define fb_getTime()		tcnter_get()
 #define fb_isItTime(a,b)	tcnter_isItTimeV2((a),(b),FALSE)
#else
#error "Need timer-functions for REFRESH_ON_CHANGE"
#endif
//#endif



// This code was in main...
// This isn't generalized enough to justify this function-name
// just not ready to delete it completely
// Returns TRUE if there's a change to the frameBuffer image
// (so a refresh can be scheduled, if so desired)
// a/o v71: This is called in the main loop
// 	w/ REFRESH_ON_CHANGE:
//        * The fb_updater() function (e.g. tet_update) is only called
//          after the frame-refresh is complete
//          (if there are several refreshes, it waits until they're all
//          done)
//          It *also* waits for a delay *after the refreshes* before
//          fb_updater() is called... (this may be a little redundant?)
//    w/o:
//        * The fb_updater() function is called at the beginning of a new
//          frame (once) to attempt to only update the framebuffer at a
//          time when it won't refresh the screen with only half of the
//          framebuffer itself updated.
void frameBufferUpdate(void);


#endif //__FRAMEBUFFER_H__




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
 * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/_options/frameBuffer.h
 *
 *    (Wow, that's a lot longer than I'd hoped).
 *
 *    Enjoy!
 */
