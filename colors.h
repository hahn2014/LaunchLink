#ifndef COLORS_H
#define COLORS_H

static const char Empty[]       = {0};
static const char Reset[]       = { 0x1b, '[', '0', ';', '3', '9', 'm', 0 };
static const char Black[]       = { 0x1b, '[', '0', ';', '3', '0', 'm', 0 };
static const char Red[]	        = { 0x1b, '[', '0', ';', '3', '1', 'm', 0 };
static const char Green[]       = { 0x1b, '[', '0', ';', '3', '2', 'm', 0 };
static const char Yellow[]	    = { 0x1b, '[', '0', ';', '3', '3', 'm', 0 };
static const char Blue[]	    = { 0x1b, '[', '0', ';', '3', '4', 'm', 0 };
static const char Magenta[]	    = { 0x1b, '[', '0', ';', '3', '5', 'm', 0 };
static const char Cyan[]   	    = { 0x1b, '[', '0', ';', '3', '6', 'm', 0 };
static const char White[]	    = { 0x1b, '[', '0', ';', '3', '7', 'm', 0 };
static const char BoldBlack[]   = { 0x1b, '[', '1', ';', '3', '0', 'm', 0 };
static const char BoldRed[]     = { 0x1b, '[', '1', ';', '3', '1', 'm', 0 };
static const char BoldGreen[]   = { 0x1b, '[', '1', ';', '3', '2', 'm', 0 };
static const char BoldYellow[]  = { 0x1b, '[', '1', ';', '3', '3', 'm', 0 };
static const char BoldBlue[]    = { 0x1b, '[', '1', ';', '3', '4', 'm', 0 };
static const char BoldMagenta[] = { 0x1b, '[', '1', ';', '3', '5', 'm', 0 };
static const char BoldCyan[]    = { 0x1b, '[', '1', ';', '3', '6', 'm', 0 };
static const char BoldWhite[]   = { 0x1b, '[', '1', ';', '3', '7', 'm', 0 };

#endif
