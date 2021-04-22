#include "cubeSolverNew.h"
#include "usart.h"
#include "lcd.h"
#include "delay.h" 
#include "stdio.h"
#include "string.h"
#include "rt_heap.h"
#include "stdlib.h"

#define shang cubeColorU
#define xia cubeColorD
#define zuo cubeColorL
#define you cubeColorR
#define qian cubeColorF
#define hou cubeColorB

extern unsigned char cubeColorU[],cubeColorD[],cubeColorL[],cubeColorR[],cubeColorF[],cubeColorB[];
extern const u16 cubeColorDraw[];

char solvedMove[4][20][2],solvedMoveCount[4];

char
// RLFBUD is the face order used for input, so that a correctly oriented
// piece in the input has its 'highest value' facelet first. The rest of the
// program uses moves in FBRLUD order.
*faces = "RLFBUD",
// I use char arrays here cause they can be initialised with a string
// which is shorter than initialising other arrays.
// Internally cube uses slightly different ordering to the input so that
//  orbits of stage 4 are contiguous. Note also that the two corner orbits
//  are diametrically opposite each other.
//input:  UF UR UB UL  DF DR DB DL  FR FL BR BL  UFR URB UBL ULF   DRF DFL DLB DBR
//        A  B  C  D   E  F  G  H   I  J  K  L   M   N   O   P     Q   R   S   T   
//        A  E  C  G   B  F  D  H   I  J  K  L   M   S   N   T     R   O   Q   P
//intrnl: UF DF UB DB  UR DR UL DL  FR FL BR BL  UFR UBL DFL DBR   DLB DRF URB ULF 
*order = "AECGBFDHIJKLMSNTROQP",
//To quickly recognise the pieces, I construct an integer by setting a bit for each
// facelet. The unique result is then found on the list below to map it to the correct
// cubelet of the cube.
//intrnl: UF DF UB DB  UR DR UL DL  FR FL BR BL  UFR UBL DFL DBR   DLB DRF URB ULF 
//bithash:20,36,24,40, 17,33,18,34, 5, 6, 9, 10, 21, 26, 38, 41,   42, 37, 25, 22
*bithash = "TdXhQaRbEFIJUZfijeYV",
//Each move consists of two 4-cycles. This string contains these in FBRLUD order.
//intrnl: UF DF UB DB  UR DR UL DL  FR FL BR BL  UFR UBL DFL DBR   DLB DRF URB ULF 
//        A  B   C   D   E   F   G   H   I   J   K   L   M   N   O   P   Q   R   S   T   
*perm = "AIBJTMROCLDKSNQPEKFIMSPRGJHLNTOQAGCEMTNSBFDHORPQ",

// current cube position
pos[20], ori[20], val[20],
// temporary variable used in swap macro
TEMP,
// pruning tables, 2 for each phase
*tables[8];
// current phase solution
int move[20], moveamount[20],
// current phase being searched (0,2,4,6 for phases 1 to 4)
phase = 0,
// Length of pruning tables. (one dummy in phase 1);
tablesize[] = {1, 4096, 6561, 4096, 256, 1536, 13824, 576};

// Use very ugly and unsafe macro to swap items instead of classic routine with
//   pointers for the sole reason of brevity
#define SWAP(a,b) TEMP=a;a=b;b=TEMP;
// number 65='A' is often subtracted to convert char ABC... to number 0,1,2,...
#define CHAROFFSET 65

 


// Cycles 4 pieces in array p, the piece indices given by a[0..3].
void cycle(char*p, char*a) {
	SWAP(p[*a - CHAROFFSET], p[a[1] - CHAROFFSET]);
	SWAP(p[*a - CHAROFFSET], p[a[2] - CHAROFFSET]);
	SWAP(p[*a - CHAROFFSET], p[a[3] - CHAROFFSET]);
}

// twists i-th piece a+1 times.
void twist(int i, int a) {
	i -= CHAROFFSET;
	ori[i] = (ori[i] + a + 1) % val[i];
}


// set cube to solved position
void reset() 
{
	int i;
	for (i = 0; i<20; pos[i] = i, ori[i++] = 0);
}

// convert permutation of 4 chars to a number in range 0..23
int permtonum(char* p) {
	int a, b, n = 0;
	for (a = 0; a<4; a++) {
		n *= 4 - a;
		for (b = a; ++b<4; )
			if (p[b]<p[a]) n++;
	}
	return n;
}

// convert number in range 0..23 to permutation of 4 chars.
void numtoperm(char* p, int n, int o) 
{
	int a, b;
	p += o;
	p[3] = o;
	for (a = 3; a--;) {
		p[a] = n % (4 - a) + o;
		n /= 4 - a;
		for (b = a; ++b<4; )
			if (p[b] >= p[a]) p[b]++;
	}
}

// get index of cube position from table t
int getposition(int t) {
	int i = -1, n = 0;
	switch (t) {
		// case 0 does nothing so returns 0
	case 1://edgeflip
		   // 12 bits, set bit if edge is flipped
		for (;++i<12;) n += ori[i] << i;
		break;
	case 2://cornertwist
		   // get base 3 number of 8 digits - each digit is corner twist
		for (i = 20;--i>11;) n = n * 3 + ori[i];
		break;
	case 3://middle edge choice
		   // 12 bits, set bit if edge belongs in Um middle slice
		for (;++i<12;) n += (pos[i] & 8) ? (1 << i) : 0;
		break;
	case 4://ud slice choice
		   // 8 bits, set bit if UD edge belongs in Fm middle slice
		for (;++i<8;) n += (pos[i] & 4) ? (1 << i) : 0;
		break;
	case 5://tetrad choice, twist and parity
	{int corn[8], j, k, l, corn2[4];
		// 8 bits, set bit if corner belongs in second tetrad.
		// also separate pieces for twist/parity determination
		k = j = 0;
		for (;++i<8;)
			if ((l = pos[i + 12] - 12) & 4) {
				corn[l] = k++;
				n += 1 << i;
			}
			else corn[j++] = l;
			//Find permutation of second tetrad after solving first
			for (i = 0;i<4;i++) corn2[i] = corn[4 + corn[i]];
			//Solve one piece of second tetrad
			for (;--i;) corn2[i] ^= corn2[0];

			// encode parity/tetrad twist
			n = n * 6 + corn2[1] * 2 - 2;
			if (corn2[3]<corn2[2])n++;
			break;
		}
	case 6://two edge and one corner orbit, permutation
		n = permtonum(pos) * 576 + permtonum(pos + 4) * 24 + permtonum(pos + 12);
		break;
	case 7://one edge and one corner orbit, permutation
		n = permtonum(pos + 8) * 24 + permtonum(pos + 16);
		break;
	}
	return n;
}


// sets cube to any position which has index n in table t
void setposition(int t, int n) {
	int i = 0, j = 12, k = 0;
	char *corn = "QRSTQRTSQSRTQTRSQSTRQTSR";
	reset();
	switch (t) {
		// case 0 does nothing so leaves cube solved
	case 1://edgeflip
		for (;i<12;i++, n >>= 1) ori[i] = n & 1;
		break;
	case 2://cornertwist
		for (i = 12;i<20;i++, n /= 3) ori[i] = n % 3;
		break;
	case 3://middle edge choice
		for (;i<12;i++, n >>= 1) pos[i] = 8 * n & 8;
		break;
	case 4://ud slice choice
		for (;i<8;i++, n >>= 1) pos[i] = 4 * n & 4;
		break;
	case 5://tetrad choice,parity,twist
		corn += n % 6 * 4;
		n /= 6;
		for (;i<8;i++, n >>= 1)
			pos[i + 12] = n & 1 ? corn[k++] - CHAROFFSET : j++;
		break;
	case 6://slice permutations
		numtoperm(pos, n % 24, 12);n /= 24;
		numtoperm(pos, n % 24, 4); n /= 24;
		numtoperm(pos, n, 0);
		break;
	case 7://corner permutations
		numtoperm(pos, n / 24, 8);
		numtoperm(pos, n % 24, 16);
		break;
	}
}


//do a clockwise quarter turn cube move
void domove(int m) {
	char *p = perm + 8 * m, i = 8;
	//cycle the edges
	cycle(pos, p);
	cycle(ori, p);
	//cycle the corners
	cycle(pos, p + 4);
	cycle(ori, p + 4);
	//twist corners if RLFB
	if (m<4)
		for (;--i>3;) twist(p[i], i & 1);
	//flip edges if FB
	if (m<2)
		for (i = 4;i--;) twist(p[i], 0);
}



// calculate a pruning table
void filltable(int ti) 													//产生一个表格供寻找得到解决路径
{												
	int n = 1, l = 1, tl = tablesize[ti];					//规定表格大小
	int f, q, i, r;

	
	// alocate table memory
	char* tb = tables[ti] = malloc ( tl);
	//clear table
	memset(tb, 0, tl);
	//mark solved position as depth 1
	reset();											//回到复原状态
	tb[getposition(ti)] = 1;			//标记当前为第一层

	while (n) {
		n = 0;
		for (i=0; i<tl; i++) 		//遍历来找一层所有情况
		{
			if (tb[i] == l) 
			{
				//construct that cube position
				setposition(ti, i);
				
				// try each face any amount
				for (f=0; f<6; f++) 				//int f;
				{
					for (q=1; q<4; q++) 	//int q;
					{
						domove(f);
						// get resulting position，得到旋转后的序列
						r = getposition(ti);
						
						// if move as allowed in that phase, and position is a new one
						if ((q==2 || f>=(ti&6)) && !tb[r]) 
						{
							// mark that position as depth l+1
							tb[r] = l + 1;
							n++;
						}
					}
					domove(f);
				}
			}
		}
		l++;
	}
}


// Pruned tree search. recursive.
_Bool searchphase(int movesleft, int movesdone, int lastmove) 
{
	int i, j;
	// prune - position must still be solvable in the remaining moves available
	if (tables[phase][getposition(phase)] - 1 > movesleft ||
		tables[phase + 1][getposition(phase + 1)] - 1 > movesleft) return 0;

	// If no moves left to do, we have solved this phase
	if (!movesleft) return 1;

	// not solved. try each face move
	for (i = 6;i--;) 
	{
		// do not repeat same face, nor do opposite after DLB.
		if (i - lastmove && (i - lastmove + 1 || i | 1)) {
			move[movesdone] = i;
			// try 1,2,3 quarter turns of that face
			for (j = 0;++j<4;) {
				//do move and remember it
				domove(i);
				moveamount[movesdone] = j;
				//Check if phase only allows half moves of this face
				if ((j == 2 || i >= phase) &&
					//search on
					searchphase(movesleft - 1, movesdone + 1, i)) return 1;
			}
			// put face back to original position.
			domove(i);
		}
	}
	// no solution found
	return 0;
}





void turnU(void)
{
	char t;
	
	t=shang[2];
	shang[2]=shang[0];
	shang[0]=shang[5];
	shang[5]=shang[7];
	shang[7]=t;
	
	t=shang[1];
	shang[1]=shang[3];
	shang[3]=shang[6];
	shang[6]=shang[4];
	shang[4]=t;
	
	t=qian[0];
	qian[0]=you[0];
	you[0]=hou[2];
	hou[2]=zuo[2];
	zuo[2]=t;
	
	t=qian[1];
	qian[1]=you[1];
	you[1]=hou[1];
	hou[1]=zuo[1];
	zuo[1]=t;
	
	t=qian[2];
	qian[2]=you[2];
	you[2]=hou[0];
	hou[0]=zuo[0];
	zuo[0]=t;
}
void turnUR(void)
{
	char t;
	
	t=shang[7];
	shang[7]=shang[5];
	shang[5]=shang[0];
	shang[0]=shang[2];
	shang[2]=t;
	
	t=shang[4];
	shang[4]=shang[6];
	shang[6]=shang[3];
	shang[3]=shang[1];
	shang[1]=t;
	
	t=zuo[2];
	zuo[2]=hou[2];
	hou[2]=you[0];
	you[0]=qian[0];
	qian[0]=t;
	
	t=zuo[1];
	zuo[1]=hou[1];
	hou[1]=you[1];
	you[1]=qian[1];
	qian[1]=t;
	
	t=zuo[0];
	zuo[0]=hou[0];
	hou[0]=you[2];
	you[2]=qian[2];
	qian[2]=t;
}
void turnL(void)
{
	int t;
	
	t=zuo[7];
	zuo[7]=zuo[5];
	zuo[5]=zuo[0];
	zuo[0]=zuo[2];
	zuo[2]=t;

	t=zuo[4];
	zuo[4]=zuo[6];
	zuo[6]=zuo[3];
	zuo[3]=zuo[1];
	zuo[1]=t;

	t=shang[3];
	shang[3]=hou[3];
	hou[3]=xia[3];
	xia[3]=qian[3];
	qian[3]=t;
	
	t=shang[0];
	shang[0]=hou[5];
	hou[5]=xia[5];
	xia[5]=qian[0];
	qian[0]=t;
	
	t=shang[5];
	shang[5]=hou[0];
	hou[0]=xia[0];
	xia[0]=qian[5];
	qian[5]=t;
}
void turnLR(void)
{
	int t;
	
	t=zuo[2];
	zuo[2]=zuo[0];
	zuo[0]=zuo[5];
	zuo[5]=zuo[7];
	zuo[7]=t;

	t=zuo[1];
	zuo[1]=zuo[3];
	zuo[3]=zuo[6];
	zuo[6]=zuo[4];
	zuo[4]=t;

	t=qian[3];
	qian[3]=xia[3];
	xia[3]=hou[3];
	hou[3]=shang[3];
	shang[3]=t;
	
	t=qian[0];
	qian[0]=xia[5];
	xia[5]=hou[5];
	hou[5]=shang[0];
	shang[0]=t;
	
	t=qian[5];
	qian[5]=xia[0];
	xia[0]=hou[0];
	hou[0]=shang[5];
	shang[5]=t;
}
void turnR(void)
{
	int t;
	
	t=you[2];
	you[2]=you[0];
	you[0]=you[5];
	you[5]=you[7];
	you[7]=t;
	
	t=you[1];
	you[1]=you[3];
	you[3]=you[6];
	you[6]=you[4];
	you[4]=t;
	
	t=shang[4];
	shang[4]=qian[4];
	qian[4]=xia[4];
	xia[4]=hou[4];
	hou[4]=t;
	
	t=shang[2];
	shang[2]=qian[2];
	qian[2]=xia[7];
	xia[7]=hou[7];
	hou[7]=t;
	
	t=shang[7];
	shang[7]=qian[7];
	qian[7]=xia[2];
	xia[2]=hou[2];
	hou[2]=t;
}
void turnRR(void)
{
	int t;
	
	t=you[7];
	you[7]=you[5];
	you[5]=you[0];
	you[0]=you[2];
	you[2]=t;
	
	t=you[4];
	you[4]=you[6];
	you[6]=you[3];
	you[3]=you[1];
	you[1]=t;
	
	t=hou[4];
	hou[4]=xia[4];
	xia[4]=qian[4];
	qian[4]=shang[4];
	shang[4]=t;
	
	t=hou[7];
	hou[7]=xia[7];
	xia[7]=qian[2];
	qian[2]=shang[2];
	shang[2]=t;
	
	t=hou[2];
	hou[2]=xia[2];
	xia[2]=qian[7];
	qian[7]=shang[7];
	shang[7]=t;
}
void turnF(void)
{
	int t;
	
	t=qian[2];
	qian[2]=qian[0];
	qian[0]=qian[5];
	qian[5]=qian[7];
	qian[7]=t;
	
	t=qian[1];
	qian[1]=qian[3];
	qian[3]=qian[6];
	qian[6]=qian[4];
	qian[4]=t;
	
	t=shang[5];
	shang[5]=zuo[5];
	zuo[5]=xia[7];
	xia[7]=you[0];
	you[0]=t;
	
	t=shang[6];
	shang[6]=zuo[3];
	zuo[3]=xia[6];
	xia[6]=you[3];
	you[3]=t;
	
	t=shang[7];
	shang[7]=zuo[0];
	zuo[0]=xia[5];
	xia[5]=you[5];
	you[5]=t;
}
void turnFR(void)
{
	int t;
	
	t=qian[7];
	qian[7]=qian[5];
	qian[5]=qian[0];
	qian[0]=qian[2];
	qian[2]=t;
	
	t=qian[4];
	qian[4]=qian[6];
	qian[6]=qian[3];
	qian[3]=qian[1];
	qian[1]=t;
	
	t=you[0];
	you[0]=xia[7];
	xia[7]=zuo[5];
	zuo[5]=shang[5];
	shang[5]=t;
	
	t=you[3];
	you[3]=xia[6];
	xia[6]=zuo[3];
	zuo[3]=shang[6];
	shang[6]=t;
	
	t=you[5];
	you[5]=xia[5];
	xia[5]=zuo[0];
	zuo[0]=shang[7];
	shang[7]=t;
}
void turnB(void)
{
	int t;
	
	t=hou[7];
	hou[7]=hou[5];
	hou[5]=hou[0];
	hou[0]=hou[2];
	hou[2]=t;
	
	t=hou[4];
	hou[4]=hou[6];
	hou[6]=hou[3];
	hou[3]=hou[1];
	hou[1]=t;
	
	t=shang[0];
	shang[0]=you[2];
	you[2]=xia[2];
	xia[2]=zuo[7];
	zuo[7]=t;
	
	t=shang[1];
	shang[1]=you[4];
	you[4]=xia[1];
	xia[1]=zuo[4];
	zuo[4]=t;
	
	t=shang[2];
	shang[2]=you[7];
	you[7]=xia[0];
	xia[0]=zuo[2];
	zuo[2]=t;
}
void turnBR(void)
{
	int t;
	
	t=hou[2];
	hou[2]=hou[0];
	hou[0]=hou[5];
	hou[5]=hou[7];
	hou[7]=t;
	
	t=hou[1];
	hou[1]=hou[3];
	hou[3]=hou[6];
	hou[6]=hou[4];
	hou[4]=t;
	
	t=zuo[7];
	zuo[7]=xia[2];
	xia[2]=you[2];
	you[2]=shang[0];
	shang[0]=t;
	
	t=zuo[4];
	zuo[4]=xia[1];
	xia[1]=you[4];
	you[4]=shang[1];
	shang[1]=t;
	
	t=zuo[2];
	zuo[2]=xia[0];
	xia[0]=you[7];
	you[7]=shang[2];
	shang[2]=t;
}
void turnD(void)
{
	char t;
	
	t=xia[7];
	xia[7]=xia[5];
	xia[5]=xia[0];
	xia[0]=xia[2];
	xia[2]=t;
	
	t=xia[6];
	xia[6]=xia[3];
	xia[3]=xia[1];
	xia[1]=xia[4];
	xia[4]=t;
	
	t=qian[5];
	qian[5]=zuo[7];
	zuo[7]=hou[7];
	hou[7]=you[5];
	you[5]=t;
	
	t=qian[6];
	qian[6]=zuo[6];
	zuo[6]=hou[6];
	hou[6]=you[6];
	you[6]=t;
	
	t=qian[7];
	qian[7]=zuo[5];
	zuo[5]=hou[5];
	hou[5]=you[7];
	you[7]=t;
}
void turnDR(void)
{
	turnD();
	turnD();
	turnD();
}


void DrawCubeColorNew()
{
	LCD_Fill(50,50,100,100,cubeColorDraw[cubeColorU[0]-1]);
	LCD_Fill(100,50,150,100,cubeColorDraw[cubeColorU[1]-1]);
	LCD_Fill(150,50,200,100,cubeColorDraw[cubeColorU[2]-1]);		
	LCD_Fill(50,100,100,150,cubeColorDraw[cubeColorU[3]-1]);
	LCD_Fill(100,100,150,150,cubeColorDraw[yellow-1]);
	LCD_Fill(150,100,200,150,cubeColorDraw[cubeColorU[4]-1]);	
	LCD_Fill(50,150,100,200,cubeColorDraw[cubeColorU[5]-1]);
	LCD_Fill(100,150,150,200,cubeColorDraw[cubeColorU[6]-1]);
	LCD_Fill(150,150,200,200,cubeColorDraw[cubeColorU[7]-1]);
	LCD_DrawLine(50,50,200,50);
	LCD_DrawLine(50,100,200,100);
	LCD_DrawLine(50,150,200,150);
	LCD_DrawLine(50,200,200,200);
	LCD_DrawLine(50,50,50,200);
	LCD_DrawLine(100,50,100,200);
	LCD_DrawLine(150,50,150,200);
	LCD_DrawLine(200,50,200,200);
	LCD_ShowChar(120,112,'U',24,1);
	
	
	LCD_Fill(250,50,300,100,cubeColorDraw[cubeColorD[5]-1]);
	LCD_Fill(300,50,350,100,cubeColorDraw[cubeColorD[6]-1]);
	LCD_Fill(350,50,400,100,cubeColorDraw[cubeColorD[7]-1]);		
	LCD_Fill(250,100,300,150,cubeColorDraw[cubeColorD[3]-1]);
	LCD_Fill(300,100,350,150,cubeColorDraw[white-1]);
	LCD_Fill(350,100,400,150,cubeColorDraw[cubeColorD[4]-1]);	
	LCD_Fill(250,150,300,200,cubeColorDraw[cubeColorD[0]-1]);
	LCD_Fill(300,150,350,200,cubeColorDraw[cubeColorD[1]-1]);
	LCD_Fill(350,150,400,200,cubeColorDraw[cubeColorD[2]-1]);
	LCD_DrawLine(250,50,400,50);
	LCD_DrawLine(250,100,400,100);
	LCD_DrawLine(250,150,400,150);
	LCD_DrawLine(250,200,400,200);
	LCD_DrawLine(250,50,250,200);
	LCD_DrawLine(300,50,300,200);
	LCD_DrawLine(350,50,350,200);
	LCD_DrawLine(400,50,400,200);
	LCD_ShowChar(320,112,'D',24,1);
	
	
	LCD_Fill(50,250,100,300,cubeColorDraw[cubeColorL[2]-1]);
	LCD_Fill(100,250,150,300,cubeColorDraw[cubeColorL[1]-1]);
	LCD_Fill(150,250,200,300,cubeColorDraw[cubeColorL[0]-1]);		
	LCD_Fill(50,300,100,350,cubeColorDraw[cubeColorL[4]-1]);
	LCD_Fill(100,300,150,350,cubeColorDraw[blue-1]);
	LCD_Fill(150,300,200,350,cubeColorDraw[cubeColorL[3]-1]);	
	LCD_Fill(50,350,100,400,cubeColorDraw[cubeColorL[7]-1]);
	LCD_Fill(100,350,150,400,cubeColorDraw[cubeColorL[6]-1]);
	LCD_Fill(150,350,200,400,cubeColorDraw[cubeColorL[5]-1]);
	LCD_DrawLine(50,250,200,250);
	LCD_DrawLine(50,300,200,300);
	LCD_DrawLine(50,350,200,350);
	LCD_DrawLine(50,400,200,400);
	LCD_DrawLine(50,250,50,400);
	LCD_DrawLine(100,250,100,400);
	LCD_DrawLine(150,250,150,400);
	LCD_DrawLine(200,250,200,400);
	LCD_ShowChar(120,312,'L',24,1);
	

	LCD_Fill(250,250,300,300,cubeColorDraw[cubeColorR[0]-1]);
	LCD_Fill(300,250,350,300,cubeColorDraw[cubeColorR[1]-1]);
	LCD_Fill(350,250,400,300,cubeColorDraw[cubeColorR[2]-1]);		
	LCD_Fill(250,300,300,350,cubeColorDraw[cubeColorR[3]-1]);
	LCD_Fill(300,300,350,350,cubeColorDraw[green-1]);
	LCD_Fill(350,300,400,350,cubeColorDraw[cubeColorR[4]-1]);	
	LCD_Fill(250,350,300,400,cubeColorDraw[cubeColorR[5]-1]);
	LCD_Fill(300,350,350,400,cubeColorDraw[cubeColorR[6]-1]);
	LCD_Fill(350,350,400,400,cubeColorDraw[cubeColorR[7]-1]);
	LCD_DrawLine(250,250,400,250);
	LCD_DrawLine(250,300,400,300);
	LCD_DrawLine(250,350,400,350);
	LCD_DrawLine(250,400,400,400);
	LCD_DrawLine(250,250,250,400);
	LCD_DrawLine(300,250,300,400);
	LCD_DrawLine(350,250,350,400);
	LCD_DrawLine(400,250,400,400);
	LCD_ShowChar(320,312,'R',24,1);
	
	
	LCD_Fill(50,450,100,500,cubeColorDraw[cubeColorF[0]-1]);
	LCD_Fill(100,450,150,500,cubeColorDraw[cubeColorF[1]-1]);
	LCD_Fill(150,450,200,500,cubeColorDraw[cubeColorF[2]-1]);		
	LCD_Fill(50,500,100,550,cubeColorDraw[cubeColorF[3]-1]);
	LCD_Fill(100,500,150,550,cubeColorDraw[red-1]);
	LCD_Fill(150,500,200,550,cubeColorDraw[cubeColorF[4]-1]);	
	LCD_Fill(50,550,100,600,cubeColorDraw[cubeColorF[5]-1]);
	LCD_Fill(100,550,150,600,cubeColorDraw[cubeColorF[6]-1]);
	LCD_Fill(150,550,200,600,cubeColorDraw[cubeColorF[7]-1]);
	LCD_DrawLine(50,450,200,450);
	LCD_DrawLine(50,500,200,500);
	LCD_DrawLine(50,550,200,550);
	LCD_DrawLine(50,600,200,600);
	LCD_DrawLine(50,450,50,600);
	LCD_DrawLine(100,450,100,600);
	LCD_DrawLine(150,450,150,600);
	LCD_DrawLine(200,450,200,600);
	LCD_ShowChar(120,512,'F',24,1);
	
	
	LCD_Fill(250,450,300,500,cubeColorDraw[cubeColorB[2]-1]);
	LCD_Fill(300,450,350,500,cubeColorDraw[cubeColorB[1]-1]);
	LCD_Fill(350,450,400,500,cubeColorDraw[cubeColorB[0]-1]);		
	LCD_Fill(250,500,300,550,cubeColorDraw[cubeColorB[4]-1]);
	LCD_Fill(300,500,350,550,cubeColorDraw[orange-1]);
	LCD_Fill(350,500,400,550,cubeColorDraw[cubeColorB[3]-1]);	
	LCD_Fill(250,550,300,600,cubeColorDraw[cubeColorB[7]-1]);
	LCD_Fill(300,550,350,600,cubeColorDraw[cubeColorB[6]-1]);
	LCD_Fill(350,550,400,600,cubeColorDraw[cubeColorB[5]-1]);
	LCD_DrawLine(250,450,400,450);
	LCD_DrawLine(250,500,400,500);
	LCD_DrawLine(250,550,400,550);
	LCD_DrawLine(250,600,400,600);
	LCD_DrawLine(250,450,250,600);
	LCD_DrawLine(300,450,300,600);
	LCD_DrawLine(350,450,350,600);
	LCD_DrawLine(400,450,400,600);
	LCD_ShowChar(320,512,'B',24,1);
}

void TurnCubeNew(char * in)
{
	unsigned int i;
	u8 j;
	LCD_Fill(0,0,240,40,WHITE);
	LCD_ShowString(15,10,240,24,24,in);
	if(in[1]<'9'&&in[1]>'0')j=in[1]-'0';
	else j=1;
	for(i=0;i<j;i++)
	{
		switch (in[0])
		{
			case 'F':
				{
					if(in[1]=='\''||in[2]=='\'') turnFR();
					else turnF();
					break;
				}
			case 'B':
				{
					if(in[1]=='\''||in[2]=='\'') turnBR();
					else turnB();
					break;
				}
			case 'L':
				{
					if(in[1]=='\''||in[2]=='\'') turnLR();
					else turnL();
					break;
				}
			case 'R':
				{
					if(in[1]=='\''||in[2]=='\'') turnRR();
					else turnR();
					break;
				}
			case 'U':
				{
					if(in[1]=='\''||in[2]=='\'') turnUR();
					else turnU();
					break;
				}
			case 'D':
				{
					if(in[1]=='\''||in[2]=='\'') turnDR();
					else turnD();
					break;
				}
		}
	}

	for(i=0;in[i]!='\0';i++)
	{
		USART_ClearFlag(USART1, USART_FLAG_TC);
		USART_SendData(USART1, in[i]);         
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)!=SET);
		USART_ClearFlag(USART2, USART_FLAG_TC);
		USART_SendData(USART2, in[i]);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)!=SET);
	}
	
	DrawCubeColorNew();
	
	while(1)
	{
		if(USART2_RX_STA&0x8000)
		{					   
			USART2_RX_STA=0;			
			if(USART2_RX_BUF[0]=='O')
			{
				if(USART2_RX_BUF[1]=='K')
				{
					break;
				}
			}
		}
		else if(USART_RX_STA&0x8000)
		{					   
			USART_RX_STA=0;			
			if(USART_RX_BUF[0]=='O')
			{
				if(USART_RX_BUF[1]=='K')
				{
					break;
				}
			}
		}
	}
}

char numToChar(u8 in)
{
	switch( in){
		case 6: return 'U';
		case 1: return 'D';
		case 3: return 'L';
		case 4: return 'R';
		case 2: return 'F';
		case 5: return 'B';
	}
}

void faceTo(char in[][4])
{
	in[0][0]=numToChar(cubeColorU[6]);
	in[0][1]=numToChar(cubeColorF[1]);
	in[0][2]='\0';
	in[1][0]=numToChar(cubeColorU[4]);
	in[1][1]=numToChar(cubeColorR[1]);
	in[1][2]='\0';
	in[2][0]=numToChar(cubeColorU[1]);
	in[2][1]=numToChar(cubeColorB[1]);
	in[2][2]='\0';
	in[3][0]=numToChar(cubeColorU[3]);
	in[3][1]=numToChar(cubeColorL[1]);
	in[3][2]='\0';
	in[4][0]=numToChar(cubeColorD[6]);
	in[4][1]=numToChar(cubeColorF[6]);
	in[4][2]='\0';
	in[5][0]=numToChar(cubeColorD[4]);
	in[5][1]=numToChar(cubeColorR[6]);
	in[5][2]='\0';
	in[6][0]=numToChar(cubeColorD[1]);
	in[6][1]=numToChar(cubeColorB[6]);
	in[6][2]='\0';
	in[7][0]=numToChar(cubeColorD[3]);
	in[7][1]=numToChar(cubeColorL[6]);
	in[7][2]='\0';
	in[8][0]=numToChar(cubeColorF[4]);
	in[8][1]=numToChar(cubeColorR[3]);
	in[8][2]='\0';
	in[9][0]=numToChar(cubeColorF[3]);
	in[9][1]=numToChar(cubeColorL[3]);
	in[9][2]='\0';
	in[10][0]=numToChar(cubeColorB[4]);
	in[10][1]=numToChar(cubeColorR[4]);
	in[10][2]='\0';
	in[11][0]=numToChar(cubeColorB[3]);
	in[11][1]=numToChar(cubeColorL[4]);
	in[11][2]='\0';
	in[12][0]=numToChar(cubeColorU[7]);
	in[12][1]=numToChar(cubeColorF[2]);
	in[12][2]=numToChar(cubeColorR[0]);
	in[12][3]='\0';
	in[13][0]=numToChar(cubeColorU[2]);
	in[13][1]=numToChar(cubeColorR[2]);
	in[13][2]=numToChar(cubeColorB[2]);
	in[13][3]='\0';
	in[14][0]=numToChar(cubeColorU[0]);
	in[14][1]=numToChar(cubeColorB[0]);
	in[14][2]=numToChar(cubeColorL[2]);
	in[14][3]='\0';
	in[15][0]=numToChar(cubeColorU[5]);
	in[15][1]=numToChar(cubeColorL[0]);
	in[15][2]=numToChar(cubeColorF[0]);
	in[15][3]='\0';
	in[16][0]=numToChar(cubeColorD[7]);
	in[16][1]=numToChar(cubeColorR[5]);
	in[16][2]=numToChar(cubeColorF[7]);
	in[16][3]='\0';
	in[17][0]=numToChar(cubeColorD[5]);
	in[17][1]=numToChar(cubeColorF[5]);
	in[17][2]=numToChar(cubeColorL[5]);
	in[17][3]='\0';
	in[18][0]=numToChar(cubeColorD[0]);
	in[18][1]=numToChar(cubeColorL[7]);
	in[18][2]=numToChar(cubeColorB[5]);
	in[18][3]='\0';
	in[19][0]=numToChar(cubeColorD[2]);
	in[19][1]=numToChar(cubeColorB[7]);
	in[19][2]=numToChar(cubeColorR[7]);
	in[19][3]='\0';
}



char argv[20][4] = {"RF", "LD", "RD", "BL" ,"FU", "LU", "DB", "LF" ,"UR" ,"FD", "RB", "UB", "URB" ,"DRF" ,"DFL", "UFR", "LFU" ,"UBL","LBD" ,"BRD"};

// the setup function runs once when you press reset or power the board
	
void Solve(void) 
{
	int f, i=0, j=0, k=0, pc, mor;
	
	for( ; k<20; k++) 
		val[k] = k<12 ? 2 : 3;
	for( ; j<8; j++) 
		filltable(j);
	
	faceTo(argv);										//
	
	// read input, 20 pieces worth
	for(; i<20; i++) 
	{
		f = pc = k = mor = 0;
		for( ; f<val[i]; f++) 				//循环48次
		{
			// read input from stdin, or...
			//     do{cin>>c;}while(c==' ');
			//     j=strchr(faces,c)-faces;
			// ...from command line and get face number of facelet
			j = strchr(faces, argv[i+1][f]) - faces;									//这里把i改成了i+1
			// keep track of principal facelet for orientation
			if (j>k) { k = j; mor = f;}
			//construct bit hash code
			pc += 1<<j;

		}
		// find which cubelet it belongs, i.e. the label for this piece
		for(f=0; f<20; f++)
			if(pc == bithash[f] - 64) break;
		// store piece
		pos[order[i] - CHAROFFSET] = f;
		ori[order[i] - CHAROFFSET] = mor%val[i];
	}
	
	
	//solve the cube
	// four phases
	for(; phase<8; phase += 2) 
	{
//		USART_SendData(USART2, 0x59);
//		delay_ms(500);

		// try each depth till solved
		for(j=0; !searchphase(j, 0, 9); j++);		//循环156次
		
		//output result of this phase
		for(i=0; i<j; i++)					//循环7次
		{
			solvedMoveCount[phase/2]++;
			solvedMove[phase/2][i][0]="FBRLUD"[move[i]];
			solvedMove[phase/2][i][1]=moveamount[i]+'0';
		}
	}
	
	
	for(i=0;i<4;i++)
	{
		for(j=0;j<solvedMoveCount[i];j++)
		{
			char sendTemp[3]={ ' ' ,' ' ,'\0'};
			sendTemp[0]=solvedMove[i][j][0];
			sendTemp[1]=solvedMove[i][j][1];
			TurnCubeNew(sendTemp);	
		}
	
	}

}


