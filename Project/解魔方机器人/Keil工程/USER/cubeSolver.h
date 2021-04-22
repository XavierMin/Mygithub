#ifndef __CUBESOLVER_H
#define __CUBESOLVER_H	 
#include "sys.h"
#include "lcd.h"

#define white 1
#define red 2
#define blue 3
#define green 4
#define orange 5
#define yellow 6


void DrawCubeColor(void);
_Bool ScanTPInput(void);
_Bool CheckInput(void);


void TurnCube(char * in);
void Yiceng(void);
void Erceng(void);
void Shangmian(void);
void Wancheng(void);
#endif


