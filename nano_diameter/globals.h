#ifndef GLOBALS
#define GLOBALS


bool b_CLK_IS     = 0;
bool b_CLK_WAS    = 0;
bool b_DATA_IS    = 0;

#define  	INDEX_MAX  		23
#define  	INDEX_DIGITS  	15
#define		MAX_READ_OUT  	25832

bool Buffer[INDEX_MAX];

byte  Index_Now   = 0;
int  Znak  = 0;
#define  WAIT_TIME  10


unsigned long Timer     = 0;
unsigned long CurrentMillis = 0;

long  ReadOut = 0;
int dialClkPin = 0;
int dialDataPin = 0;


#endif
