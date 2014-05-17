//
// nxtg.h
//
//	This file (and nxtg.cpp) support communicating with an nxt that is running NXT-G.
//

#ifndef NXTG_H
#define NXTG_H

int nxtGBool(byte *, bool);
int nxtGInt(byte *, int);
int nxtGText(byte *, char *, int);

#endif NXTG_H
