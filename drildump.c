/*************************************************************************

    DRILDUMP - Display contents of DRILLER for the Spectrum

    Copyright (C) 2001  John Elliott <jce@seasip.demon.co.uk>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef EXIT_SUCCESS
# define EXIT_SUCCESS 0
#endif
#ifndef EXIT_FAILURE
# define EXIT_FAILURE 1
#endif

typedef unsigned char byte;
typedef unsigned short word;

typedef enum fsgame
{
	GA_DRILLER,
	GA_DARKSIDE,
	GA_ECLIPSE
} FSGAME;

FSGAME gl_game = GA_DRILLER;

int gl_pov  = 0;
int gl_room = 0;
int gl_roomr, gl_roomg, gl_roomb;
word gl_snabase = 0x3FE5;
word gl_snalen  = 0xC01B;
word gl_dbase = 0xCF3E;

byte gl_sna[51945];


byte *sna(unsigned short addr)
{
	return &gl_sna[addr - gl_snabase];
}





word peek(word a)
{
	return *sna(a) + 256 * (*sna(a+1));
}


void show_code(byte *obj, int len)
{
	int bcpc = 0;
	int opc = 0;
	int running = 1;

	if (!len) return;
	while (running)
	{
		byte op, opsize;

		if (len > 0 && bcpc >= len) break;
		if (len < 0 && opc  >= (-len)) break;

		op = (obj[bcpc]);
		opsize = 1;
		++bcpc;
		++opc;

		if (op & 0x80) printf("IFSHOT "); 
		else           printf("IFHIT  ");

		switch(op & 0x7F)
		{
			case  1: printf("+SCORE  %03d %03d %03d ", 
				        obj[bcpc], obj[1 + bcpc],
				        obj[3+bcpc]);
					opsize = 3; break;
			case  2: printf("+ENERGY %03d", obj[bcpc]); break;
			case  3: printf("TOGVIS  %03d", obj[bcpc]); break;
			case  4: printf("VIS     %03d", obj[bcpc]); break;
			case  5: printf("INVIS   %03d", obj[bcpc]); break;
			case  6: printf("RTOGVIS %03d %03d", obj[bcpc], 
                                        obj[1 + bcpc]); opsize = 2; break;
			case  7: printf("RVIS    %03d %03d", obj[bcpc], 
                                        obj[1 + bcpc]); opsize = 2; break;
			case  8: printf("RINVIS  %03d %03d", obj[bcpc], 
                                        obj[1 + bcpc]); opsize = 2; break;
			case  9: printf("INCR    %03d", obj[bcpc]); break;
			case 10: printf("DECR    %03d", obj[bcpc]); break;
			case 11: printf("IFEQ    %03d %03d", obj[bcpc],
                                        obj[1 + bcpc]); opsize = 2; break;
			case 12: printf("SETBIT  %03d", obj[bcpc]); break;
			case 13: printf("CLRBIT  %03d", obj[bcpc]); break;
			case 14: printf("IFBIT   %03d %03d", obj[bcpc],
                                        obj[1 + bcpc]); opsize = 2; break;
			case 15: printf("SOUND   %03d", obj[bcpc]); break;
			case 16: printf("DESTROY %03d", obj[bcpc]); break;
			case 17: printf("RDESTR  %03d %03d", obj[bcpc],
                                        obj[1 + bcpc]); opsize = 2; break;
			case 18: printf("GOTO    %03d %03d", obj[bcpc], 
					obj[1 + bcpc]); opsize = 2; break;
			case 19: printf("+SHIELD %03d", obj[bcpc]); break;
			case 20: printf("SETV    %03d %03d", obj[bcpc],
                                        obj[1 + bcpc]); opsize = 2; break;
			case 21: printf("SWAPJET"); opsize--; break;
			case 25: printf("SPFX    %02xh", obj[bcpc]); break;
			case 26: printf("REDRAW"); opsize--; break;
			case 27: printf("DELAY   %03d", obj[bcpc]); break;
			case 28: printf("SYNCSND %03d", obj[bcpc]); break;
			case 29: printf("TOGBIT  %03d", obj[bcpc]); break;
			case 30: printf("IFVIS   %03d", obj[bcpc]); break;
			case 31: printf("IFINVIS %03d", obj[bcpc]); break;
			case 32: printf("RIFVIS  %03d %03d", obj[bcpc],
                                        obj[1 + bcpc]); opsize = 2; break;
			case 33: printf("RIFINV  %03d %03d", obj[bcpc],
                                        obj[1 + bcpc]); opsize = 2; break;
			case 34: printf("MESSAGE %03d", obj[bcpc]); break;
			default: printf("0x%02x ", op); opsize = 0; break;
		}
		(bcpc) += opsize;
		printf("\n                      ");
	}
}


char *te_room(int nc)
{
	switch(nc)
	{
		case 0: return "* SAHARA";
		case 1: return "HORAKHTY";
		case 2: return "NEPHTHYS";
		case 3: return "KHEPRESH";
		case 4: return " RAMESES";
		case 5: return "PHARAOHS";
		case 6: return " SHABAKA";
		case 7: return "ILLUSION";
		default: return "????????";
	}
}

void pov_fstexture(int texture)
{
	double r, g, b;
	byte *p;

	//assert(texture & 0x0F);

	switch(texture & 0x0F)
	{
		case 0:  b = 0; r = 1; g = 0; break;	// Should not be drawn
		default: p = sna(gl_dbase + 0x06 + 4 * (texture & 0x0F));
			 r = g = b = (p[0] + p[1] + p[2] + p[3]) / 1020.0;
			 r *= gl_roomr;
			 g *= gl_roomg;
			 b *= gl_roomb;
			break;
	}
	printf("   pigment { color red %f green %f blue %f }\n", r,g,b);

	// This bit is currently the same for all textures
	printf("   finish { specular 0.5 roughness 0.15 }\n");
	printf("}\n");	// End object
}


int prline(double x1, double y1, double z1,
	    double x2, double y2, double z2)
{
//	fprintf(stderr, "(%f,%f,%f)(%f,%f,%f)(%f,%f,%f)(%f,%f,%f)\n",
//		x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4);
	if (x1 == x2 && y1 == y2 && z1 == z2) return 0; // Degenerate

	printf( "object { cylinder {\n"
		"  <%f, %f, %f>, <%f, %f, %f>, "
		"  0.1 }\n",
		x1, y1, z1,
		x2, y2, z2);
	return 1;
}


int prtri(double x1, double y1, double z1,
	    double x2, double y2, double z2,
            double x3, double y3, double z3)
{
//	fprintf(stderr, "(%f,%f,%f)(%f,%f,%f)(%f,%f,%f)(%f,%f,%f)\n",
//		x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4);
	if (x1 == x2 && y1 == y2 && z1 == z2 &&
            x1 == x3 && y1 == y3 && z1 == z3) return 0; // Degenerate

	// XXX To do: Check if it degenerates to a line	

	printf( "object { polygon {\n"
		"  4, <%f, %f, %f>, <%f, %f, %f>, "
		"     <%f, %f, %f>, <%f, %f, %f> }\n",
		x1, y1, z1,
		x2, y2, z2,
		x3, y3, z3,
		x1, y1, z1 );
	return 1;
}

int prquad(double x1, double y1, double z1,
	    double x2, double y2, double z2,
            double x3, double y3, double z3,
            double x4, double y4, double z4)
{
//	fprintf(stderr, "(%f,%f,%f)(%f,%f,%f)(%f,%f,%f)(%f,%f,%f)\n",
//		x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4);
	if (x1 == x2 && y1 == y2 && z1 == z2 &&
            x1 == x3 && y1 == y3 && z1 == z3 &&
            x1 == x4 && y1 == y4 && z1 == z4) return 0; // Degenerate
	
	// XXX Check if it degenerates to a line

	// Degenerates to a triangle?
	if (x1 == x2 && y1 == y2 && z1 == z2)
		return prtri(x2,y2,z2,x3,y3,z3,x4,y4,z4);
	if (x2 == x3 && y2 == y3 && z2 == z3)
		return prtri(x1,y1,z1,x3,y3,z3,x4,y4,z4);
	if (x3 == x4 && y3 == y4 && z3 == z4)
		return prtri(x1,y1,z1,x2,y2,z2,x3,y3,z3);
	if (x4 == x1 && y4 == y1 && z4 == z1)
		return prtri(x1,y1,z1,x2,y2,z2,x3,y3,z3);


	printf( "object { polygon {\n"
		"  5, <%f, %f, %f>, <%f, %f, %f>, "
		"     <%f, %f, %f>, <%f, %f, %f>, "
                "     <%f, %f, %f> }\n",
		x1, y1, z1,
		x2, y2, z2,
		x3, y3, z3,
		x4, y4, z4,
		x1, y1, z1 );
	return 1;
}


int prpent(double x1, double y1, double z1,
	    double x2, double y2, double z2,
            double x3, double y3, double z3,
            double x4, double y4, double z4,
	    double x5, double y5, double z5)
{
//
// No checks for degeneration, because these shapes are not generated
// internally by drildump
//
	printf( "object { polygon {\n"
		"  6, <%f, %f, %f>, <%f, %f, %f>, "
		"     <%f, %f, %f>, <%f, %f, %f>, "
                "     <%f, %f, %f>, <%f, %f, %f> }\n",
		x1, y1, z1,
		x2, y2, z2,
		x3, y3, z3,
		x4, y4, z4,
		x5, y5, z5,
		x1, y1, z1 );
	return 1;
}


int prhex(double x1, double y1, double z1,
	    double x2, double y2, double z2,
            double x3, double y3, double z3,
            double x4, double y4, double z4,
	    double x5, double y5, double z5,
	    double x6, double y6, double z6)
{
//
// No checks for degeneration, because these shapes are not generated
// internally by drildump
//
	printf( "object { polygon {\n"
		"  7, <%f, %f, %f>, <%f, %f, %f>, "
		"     <%f, %f, %f>, <%f, %f, %f>, "
                "     <%f, %f, %f>, <%f, %f, %f>, "
                "     <%f, %f, %f>  }\n",
		x1, y1, z1,
		x2, y2, z2,
		x3, y3, z3,
		x4, y4, z4,
		x5, y5, z5,
		x6, y6, z6,
		x1, y1, z1 );
	return 1;
}




void pov_pyramid(byte *obj)
{
	int xshade[2], yshade[2], zshade[2];
	int n;
	int type;
	int xbase[4], ybase[4], zbase[4];
	int xtop[4],  ytop[4],  ztop[4];

	type = obj[0] & 0x0F;
	xbase[0] = xbase[3] = obj[1];
	xbase[1] = xbase[2] = obj[1] + obj[4];
	zbase[0] = zbase[1] = obj[3];
	zbase[2] = zbase[3] = obj[3] + obj[6];
	for (n = 0; n < 4; n++) ybase[n] = obj[2];

	memcpy(xtop, xbase, sizeof xtop);
	memcpy(ztop, zbase, sizeof ztop);
	for (n = 0; n < 4; n++) ytop[n] = ybase[n] + obj[5];

	switch(type)
	{
		case 1: // CUBE
			xshade[1] = obj[9 ] & 0x0F; xshade[0] = obj[9 ] >> 4; 
			yshade[0] = obj[10] & 0x0F; yshade[1] = obj[10] >> 4;
			zshade[0] = obj[11] & 0x0F; zshade[1] = obj[11] >> 4;
			break;
		case 4: // EPYRAMID
		yshade[0] = obj[10] >> 4;   yshade[1] = obj[ 9] >> 4; 
		xshade[0] = obj[11] & 0x0F; xshade[1] = obj[11] >> 4;
		zshade[0] = obj[9 ] & 0x0F; zshade[1] = obj[10] & 0x0F;
		zbase[1] = obj[3] + obj[13]; zbase[2] = obj[3] + obj[15];
		ztop [1] = obj[3] + obj[13]; ztop [2] = obj[3] + obj[15];
		ybase[1] = obj[2] + obj[12]; ybase[2] = obj[2] + obj[12];
		ytop [1] = obj[2] + obj[14]; ytop [2] = obj[2] + obj[14];	
		break;
		case 5: // WPYRAMID
		yshade[0] = obj[9 ] >> 4;   yshade[1] = obj[10] >> 4; 
		xshade[0] = obj[11] >> 4;   xshade[1] = obj[11] >> 4; 
		zshade[0] = obj[9 ] & 0x0F; zshade[1] = obj[10] & 0x0F;
		zbase[3] = obj[3] + obj[15]; zbase[0] = obj[3] + obj[13];
		ztop [3] = obj[3] + obj[15]; ztop [0] = obj[3] + obj[13];
		ybase[3] = obj[2] + obj[12]; ybase[0] = obj[2] + obj[12];
		ytop [3] = obj[2] + obj[14]; ytop [0] = obj[2] + obj[14];	
		break;
		case 6:	// UPYRAMID
		xshade[0] = obj[9 ] >> 4;   xshade[1] = obj[10] >> 4; 
		yshade[0] = obj[11] & 0x0F; yshade[1] = obj[11] >> 4;
		zshade[0] = obj[9 ] & 0x0F; zshade[1] = obj[10] & 0x0F;
		xtop[0] = obj[1] + obj[12]; xtop[1] = obj[1] + obj[14];
		xtop[2] = obj[1] + obj[14]; xtop[3] = obj[1] + obj[12];
		ztop[0] = obj[3] + obj[13]; ztop[1] = obj[3] + obj[13];
		ztop[2] = obj[3] + obj[15]; ztop[3] = obj[3] + obj[15];
		break;
		case 7:	// DPYRAMID
		xshade[0] = obj[9 ] >> 4;   xshade[1] = obj[10] >> 4; 
		yshade[0] = obj[11] & 0x0F; yshade[1] = obj[11] >> 4;
		zshade[0] = obj[9 ] & 0x0F; zshade[1] = obj[10] & 0x0F;
		xbase[0] = obj[1] + obj[12]; xbase[1] = obj[1] + obj[14];
		xbase[2] = obj[1] + obj[14]; xbase[3] = obj[1] + obj[12];
		zbase[0] = obj[3] + obj[13]; zbase[1] = obj[3] + obj[13];
		zbase[2] = obj[3] + obj[15]; zbase[3] = obj[3] + obj[15];
		break;
		case 8: // NPYRAMID
		xshade[0] = obj[9 ] >> 4;   xshade[1] = obj[10] >> 4; 
		yshade[0] = obj[11] & 0x0F; yshade[1] = obj[11] >> 4;
		zshade[0] = obj[9 ] & 0x0F; zshade[1] = obj[10] & 0x0F;
		xbase[2] = obj[1] + obj[12]; xbase[3] = obj[1] + obj[12];
		xtop [2] = obj[1] + obj[14]; xtop [3] = obj[1] + obj[14];
		ybase[2] = obj[2] + obj[13]; ybase[3] = obj[2] + obj[13];
		ytop [2] = obj[2] + obj[15]; ytop [3] = obj[2] + obj[15];	
		break;
		case 9: // SPYRAMID
		xshade[0] = obj[9 ] >> 4;   xshade[1] = obj[10] >> 4; 
		yshade[0] = obj[11] & 0x0F; yshade[1] = obj[11] >> 4;
		zshade[0] = obj[9 ] & 0x0F; zshade[1] = obj[10] & 0x0F;
		xbase[0] = obj[1] + obj[12]; xbase[1] = obj[1] + obj[12];
		xtop [0] = obj[1] + obj[14]; xtop [1] = obj[1] + obj[14];
		ybase[0] = obj[2] + obj[13]; ybase[1] = obj[2] + obj[13];
		ytop [0] = obj[2] + obj[15]; ytop [1] = obj[2] + obj[15];	
		break;
	}

	printf("// obj=%d xshade=%x %x yshade=%x %x zshade=%x %x\n",
		obj[7], xshade[0], xshade[1], yshade[0], yshade[1], 
			zshade[0], zshade[1]);

	if (yshade[0])
	{
		if (prquad(xbase[0], ybase[0], zbase[0], 	// The base
       			xbase[1], ybase[1], zbase[1],
			xbase[2], ybase[2], zbase[2],
			xbase[3], ybase[3], zbase[3]))
			pov_fstexture(yshade[0]);			
	}
	if (yshade[1])
	{
		if (prquad(xtop[0], ytop[0], ztop[0], 	// The top
		       xtop[1], ytop[1], ztop[1],
		       xtop[2], ytop[2], ztop[2],
		       xtop[3], ytop[3], ztop[3]))
			pov_fstexture(yshade[1]);
	}
	if (zshade[0])
	{
		if (prquad(xbase[0], ybase[0], zbase[0],	// South face
		       xtop[0],  ytop[0],  ztop[0],
		       xtop[1],  ytop[1],  ztop[1],
		       xbase[1], ybase[1], zbase[1]))
			pov_fstexture(zshade[0]); 	// XXX 0 or 1?
	}
	if (xshade[0])
	{
		if (prquad(xbase[1], ybase[1], zbase[1],
		       xtop[1],  ytop[1],  ztop[1],
		       xtop[2],  ytop[2],  ztop[2],
		       xbase[2], ybase[2], zbase[2])) 
			pov_fstexture(xshade[0]);
	}
	if (zshade[1])
	{
		if (prquad(xbase[2], ybase[2], zbase[2],	// North face
		       xtop[2],  ytop[2],  ztop[2],
		       xtop[3],  ytop[3],  ztop[3],
		       xbase[3], ybase[3], zbase[3])) 
			pov_fstexture(zshade[1]);
	}
	if (xshade[1])
	{
		if (prquad(xbase[3], ybase[3], zbase[3],
		       xtop[3],  ytop[3],  ztop[3],
		       xtop[0],  ytop[0],  ztop[0],
		       xbase[0], ybase[0], zbase[0])) 
			pov_fstexture(xshade[1]);
	}
}

void pov_line(byte *obj)
{
	if (prline(obj[10], obj[11], obj[12], obj[13], obj[14], obj[15]))
		pov_fstexture((obj[9] >> 4) | obj[9]);
}


void pov_tri(byte *obj)
{
	double x1, y1, z1, x2, y2, z2;
	
	x1 = obj[1];
	y1 = obj[2];
	z1 = obj[3];
	x2 = obj[1] + obj[4];
	y2 = obj[2] + obj[5];
	z2 = obj[3] + obj[6];

	if (y1 == y2) 
	{
		y2 = y1 + 0.01;
		y1 = y1 - 0.01;
		if (obj[9] & 0x0F) 
		{
			if (prtri(obj[10], y1, obj[12],
				  obj[13], y1, obj[15],
				  obj[16], y1, obj[18]))
				pov_fstexture(obj[9]);
		}
		if (obj[9] & 0xF0)
		{
			if (prtri(obj[10], y2, obj[12],
				  obj[13], y2, obj[15],
				  obj[16], y2, obj[18]))
				pov_fstexture(obj[9] >> 4);
		}
	}
	else if (z1 == z2) 
	{
		z2 = z1 + 0.01;
		z1 = z1 - 0.01;
		if (obj[9] & 0x0F) 
		{
			if (prtri(obj[10], obj[11], z1,
				  obj[13], obj[14], z1,
				  obj[16], obj[17], z1))
				pov_fstexture(obj[9]);
		}
		if (obj[9] & 0xF0)
		{
			if (prtri(obj[10], obj[11], z2,
				  obj[13], obj[14], z2,
				  obj[16], obj[17], z2))
				pov_fstexture(obj[9] >> 4);
		}
	}
	else if (x1 == x2) 
	{
		x2 = x1 + 0.01;
		x1 = x1 - 0.01;
		if (obj[9] & 0x0F) 
		{
			if (prtri(x1, obj[11], obj[12],
				  x1, obj[14], obj[15],
				  x1, obj[17], obj[18]))
				pov_fstexture(obj[9]);
		}
		if (obj[9] & 0xF0)
		{
			if (prtri(x2, obj[11], obj[12],
				  x2, obj[14], obj[15],
				  x2, obj[17], obj[18]))
				pov_fstexture(obj[9] >> 4);
		}
	}
	else if (prtri(obj[10], obj[11], obj[12],
		       obj[13], obj[14], obj[15],
		       obj[16], obj[17], obj[18])) 
		pov_fstexture((obj[9] >> 4) | obj[9]);

}

void pov_quad(byte *obj)
{
	double x1, y1, z1, x2, y2, z2;
	
	x1 = obj[1];
	y1 = obj[2];
	z1 = obj[3];
	x2 = obj[1] + obj[4];
	y2 = obj[2] + obj[5];
	z2 = obj[3] + obj[6];

	if (y1 == y2) 
	{
		y2 = y1 + 0.01;
		y1 = y1 - 0.01;
		if (obj[9] & 0x0F) 
		{
			if (prquad(obj[10], y1, obj[12],
 				   obj[13], y1, obj[15],
				   obj[16], y1, obj[18],
				   obj[19], y1, obj[21]))
				pov_fstexture(obj[9]);
		}
		if (obj[9] & 0xF0)
		{
			if (prquad(obj[10], y2, obj[12],
				   obj[13], y2, obj[15],
				   obj[16], y2, obj[18],
				   obj[19], y2, obj[21]))
				pov_fstexture(obj[9] >> 4);
		}
	}
	else if (z1 == z2) 
	{
		z2 = z1 + 0.01;
		z1 = z1 - 0.01;
		if (obj[9] & 0x0F) 
		{
			if (prquad(obj[10], obj[11], z1,
				   obj[13], obj[14], z1,
				   obj[16], obj[17], z1,
				   obj[19], obj[20], z1))
				pov_fstexture(obj[9]);
		}
		if (obj[9] & 0xF0)
		{
			if (prquad(obj[10], obj[11], z2,
				   obj[13], obj[14], z2,
				   obj[16], obj[17], z2,
				   obj[19], obj[20], z2))
				pov_fstexture(obj[9] >> 4);
		}
	}
	else if (x1 == x2) 
	{
		x2 = x1 + 0.01;
		x1 = x1 - 0.01;
		if (obj[9] & 0x0F) 
		{
			if (prquad(x1, obj[11], obj[12],
				   x1, obj[14], obj[15],
				   x1, obj[17], obj[18],
				   x1, obj[20], obj[21]))
				pov_fstexture(obj[9]);
		}
		if (obj[9] & 0xF0)
		{
			if (prquad(x2, obj[11], obj[12],
				   x2, obj[14], obj[15],
				   x2, obj[17], obj[18],
				   x2, obj[20], obj[21]))
				pov_fstexture(obj[9] >> 4);
		}
	}
	else if (prquad(obj[10], obj[11], obj[12],
		        obj[13], obj[14], obj[15],
		        obj[16], obj[17], obj[18], 
		        obj[19], obj[20], obj[21])) 
		pov_fstexture((obj[9] >> 4) | obj[9]);
}




void pov_pent(byte *obj)
{
	double x1, y1, z1, x2, y2, z2;
	
	x1 = obj[1];
	y1 = obj[2];
	z1 = obj[3];
	x2 = obj[1] + obj[4];
	y2 = obj[2] + obj[5];
	z2 = obj[3] + obj[6];

	if (y1 == y2) 
	{
		y2 = y1 + 0.01;
		y1 = y1 - 0.01;
		if (obj[9] & 0x0F) 
		{
			if (prpent(obj[10], y1, obj[12],
 				   obj[13], y1, obj[15],
				   obj[16], y1, obj[18],
				   obj[19], y1, obj[21],
				   obj[22], y1, obj[24]))
				pov_fstexture(obj[9]);
		}
		if (obj[9] & 0xF0)
		{
			if (prpent(obj[10], y2, obj[12],
				   obj[13], y2, obj[15],
				   obj[16], y2, obj[18],
				   obj[19], y2, obj[21],
				   obj[22], y2, obj[24]))
				pov_fstexture(obj[9] >> 4);
		}
	}
	else if (z1 == z2) 
	{
		z2 = z1 + 0.01;
		z1 = z1 - 0.01;
		if (obj[9] & 0x0F) 
		{
			if (prpent(obj[10], obj[11], z1,
				   obj[13], obj[14], z1,
				   obj[16], obj[17], z1,
				   obj[19], obj[20], z1,
				   obj[22], obj[23], z1))
				pov_fstexture(obj[9]);
		}
		if (obj[9] & 0xF0)
		{
			if (prpent(obj[10], obj[11], z2,
				   obj[13], obj[14], z2,
				   obj[16], obj[17], z2,
				   obj[19], obj[20], z2,
				   obj[22], obj[23], z2))
				pov_fstexture(obj[9] >> 4);
		}
	}
	else if (x1 == x2) 
	{
		x2 = x1 + 0.01;
		x1 = x1 - 0.01;
		if (obj[9] & 0x0F) 
		{
			if (prpent(x1, obj[11], obj[12],
				   x1, obj[14], obj[15],
				   x1, obj[17], obj[18],
				   x1, obj[20], obj[21],
				   x1, obj[23], obj[24]))
				pov_fstexture(obj[9]);
		}
		if (obj[9] & 0xF0)
		{
			if (prpent(x2, obj[11], obj[12],
				   x2, obj[14], obj[15],
				   x2, obj[17], obj[18],
				   x2, obj[20], obj[21],
				   x2, obj[23], obj[24]))
				pov_fstexture(obj[9] >> 4);
		}
	}
	else if (prpent(obj[10], obj[11], obj[12],
		        obj[13], obj[14], obj[15],
		        obj[16], obj[17], obj[18], 
		        obj[19], obj[20], obj[21],
		        obj[22], obj[23], obj[24])) 
		pov_fstexture((obj[9] >> 4) | obj[9]);

}

void pov_hex(byte *obj)
{
	double x1, y1, z1, x2, y2, z2;
	
	x1 = obj[1];
	y1 = obj[2];
	z1 = obj[3];
	x2 = obj[1] + obj[4];
	y2 = obj[2] + obj[5];
	z2 = obj[3] + obj[6];

	if (y1 == y2) 
	{
		y2 = y1 + 0.01;
		y1 = y1 - 0.01;
		if (obj[9] & 0x0F) 
		{
			if (prhex(obj[10], y1, obj[12],
 				  obj[13], y1, obj[15],
				  obj[16], y1, obj[18],
				  obj[19], y1, obj[21],
				  obj[22], y1, obj[24],
				  obj[25], y1, obj[27]))
				pov_fstexture(obj[9]);
		}
		if (obj[9] & 0xF0)
		{
			if (prhex(obj[10], y2, obj[12],
				  obj[13], y2, obj[15],
				  obj[16], y2, obj[18],
				  obj[19], y2, obj[21],
				  obj[22], y2, obj[24],
				  obj[25], y2, obj[27]))
				pov_fstexture(obj[9] >> 4);
		}
	}
	else if (z1 == z2) 
	{
		z2 = z1 + 0.01;
		z1 = z1 - 0.01;
		if (obj[9] & 0x0F) 
		{
			if (prhex(obj[10], obj[11], z1,
				  obj[13], obj[14], z1,
				  obj[16], obj[17], z1,
				  obj[19], obj[20], z1,
				  obj[22], obj[23], z1,
				  obj[25], obj[26], z1))
				pov_fstexture(obj[9]);
		}
		if (obj[9] & 0xF0)
		{
			if (prhex(obj[10], obj[11], z2,
				  obj[13], obj[14], z2,
				  obj[16], obj[17], z2,
				  obj[19], obj[20], z2,
				  obj[22], obj[23], z2,
				  obj[25], obj[26], z2))
				pov_fstexture(obj[9] >> 4);
		}
	}
	else if (x1 == x2) 
	{
		x2 = x1 + 0.01;
		x1 = x1 - 0.01;
		if (obj[9] & 0x0F) 
		{
			if (prhex(x1, obj[11], obj[12],
				  x1, obj[14], obj[15],
				  x1, obj[17], obj[18],
				  x1, obj[20], obj[21],
				  x1, obj[23], obj[24],
				  x1, obj[26], obj[27]))
				pov_fstexture(obj[9]);
		}
		if (obj[9] & 0xF0)
		{
			if (prhex(x2, obj[11], obj[12],
				  x2, obj[14], obj[15],
				  x2, obj[17], obj[18],
				  x2, obj[20], obj[21],
				  x2, obj[23], obj[24],
				  x2, obj[26], obj[27]))
				pov_fstexture(obj[9] >> 4);
		}
	}
}


void pov_rect(byte *obj)
{
	double x1, y1, z1, x2, y2, z2;
	
	x1 = obj[1];
	y1 = obj[2];
	z1 = obj[3];
	x2 = obj[1] + obj[4];
	y2 = obj[2] + obj[5];
	z2 = obj[3] + obj[6];

	if (y1 == y2) 
	{
		y2 = y1 + 0.01;
		y1 = y1 - 0.01;
		if (obj[9] & 0x0F) 
		{
			if (prquad(x1, y1, z1, x1, y1, z2, x2, y1, z2, x2, y1, z1))
				pov_fstexture(obj[9]);
		}
		if (obj[9] & 0xF0)
		{
			if (prquad(x1, y2, z1, x1, y2, z2, x2, y2, z2, x2, y2, z1))
				pov_fstexture(obj[9] >> 4);
		}
	}
	else if (x1 == x2)
	{
		x2 = x1 + 0.01;
		x1 = x1 - 0.01;
		if (obj[9] & 0x0F) 
		{
 			if (prquad(x1, y1, z1, x1, y1, z2, x1, y2, z2, x1, y2, z1))
				pov_fstexture(obj[9]);
		}
		if (obj[9] & 0xF0)
		{
 			if (prquad(x2, y1, z1, x2, y1, z2, x2, y2, z2, x2, y2, z1))
				pov_fstexture(obj[9] >> 4);
		}
	}
	else 
	{
		z2 = z1 + 0.01;
		z1 = z1 - 0.01;
		if (obj[9] & 0x0F) 
		{
			if (prquad(x1, y1, z1, x2, y1, z1, x2, y2, z1, x1, y2, z1))
				pov_fstexture(obj[9]);
		}
		if (obj[9] & 0xF0)
		{
			if (prquad(x1, y1, z2, x2, y1, z2, x2, y2, z2, x1, y2, z2))
				pov_fstexture(obj[9] >> 4);
		}
	}
}

void showobj_pov(byte *obj)
{
	byte type = obj[0] & 0x1F;

	printf("// Object Type = %d ID = %d\n", type, obj[7]);
	switch(type)
	{
		case 2: // XXX sensor!
			break;
		case 3:  pov_rect(obj); break;
		case 1:
		case 4:
		case 5:
		case 6:
		case 7:  
		case 8:
		case 9: pov_pyramid(obj); break;
		case 10: pov_line(obj); break;
		case 11: pov_tri(obj); break;
		case 12: pov_quad(obj); break;
		case 13: pov_pent(obj); break;
		case 14: pov_hex(obj); break;
	}	
}


void showobj(byte *obj)
{
	byte type = obj[0] & 0x1F;
	word dsize = 9;
	int n;

	switch(type)
	{
		case 0: 	printf("ENTRANCE  "); break;
		case 1: 	dsize = 12; printf("CUBE      "); break;
		case 2: 	dsize = 14; printf("SENSOR    "); break;
		case 3: 	dsize = 10; printf("RECTANGLE "); break;
		case 4: 	dsize = 16; printf("EPYRAMID  "); break;
		case 5:		dsize = 16; printf("WPYRAMID  "); break;
		case 6: 	dsize = 16; printf("UPYRAMID  "); break;
		case 7: 	dsize = 16; printf("DPYRAMID  "); break;
		case 8:		dsize = 16; printf("NPYRAMID  "); break;
		case 9:		dsize = 16; printf("SPYRAMID  "); break;
		case 10:	dsize = 16; printf("LINE      "); break;
		case 11:	dsize = 19; printf("TRIANGLE  "); break;
		case 12:	dsize = 22; printf("QUAD.     "); break;
		case 13:	dsize = 25; printf("PENTAGON  "); break;
		case 14:	dsize = 28; printf("HEXAGON   "); break;
		default:	printf("TYPE %02d   ", type);
				break;
	}	
	if (obj[0] & 0x80) printf("[invisible] ");
	else		   printf("            ");

	printf("Pos=(%03d,%03d,%03d) ", obj[1], obj[2], obj[3]);	
	printf("Size=(%03d,%03d,%03d) ", obj[4], obj[5], obj[6]);	
	printf("ID=%03d\n                      ", obj[7]);
	switch(type)
	{
		case 1: printf("Textures: %02xh %02xh %02xh\n"
				"                      ", 
				obj[9], obj[10], obj[11]);
			break;

		case 3: printf("Texture:  %02xh\n"
				"                      ", obj[9]);
			break;

		case 4: case 5: case 6: case 7: case 8: case 9:
			printf("Textures: %02xh %02xh %02xh\n"
				"                      ", 
				obj[9], obj[10], obj[11]);
			printf("Slopes=(%03d, %03d, %03d, %03d) ", 
				obj[12], obj[13], obj[14], obj[15]);
			printf("\n                      ");
			break;

		case 10: printf("Texture:  %02xh\n"
				"                      ", obj[9]);
			printf("Points=(%03d,%03d,%03d) ", obj[10], obj[11], obj[12]);	
			printf(" (%03d,%03d,%03d) ", obj[13], obj[14], obj[15]);	
			printf("\n                      ");
			break;
		case 11: printf("Texture:  %02xh\n"
				"                      ", obj[9]);
			printf("Points=(%03d,%03d,%03d) ", obj[10], obj[11], obj[12]);	
			printf(" (%03d,%03d,%03d) ", obj[13], obj[14], obj[15]);	
			printf("\n                            ");
			printf(" (%03d,%03d,%03d) ", obj[16], obj[17], obj[18]);	
			printf("\n                      ");
			break;
		case 12: printf("Texture:  %02xh\n"
				"                      ", obj[9]);
			printf("Points=(%03d,%03d,%03d) ", obj[10], obj[11], obj[12]);	
			printf(" (%03d,%03d,%03d) ", obj[13], obj[14], obj[15]);	
			printf("\n                            ");
			printf(" (%03d,%03d,%03d) ", obj[16], obj[17], obj[18]);	
			printf(" (%03d,%03d,%03d) ", obj[19], obj[20], obj[21]);	
			printf("\n                      ");
			break;
		case 13: printf("Texture:  %02xh\n"
				"                      ", obj[9]);
			printf("Points=(%03d,%03d,%03d) ", obj[10], obj[11], obj[12]);	
			printf(" (%03d,%03d,%03d) ", obj[13], obj[14], obj[15]);	
			printf("\n                            ");
			printf(" (%03d,%03d,%03d) ", obj[16], obj[17], obj[18]);	
			printf(" (%03d,%03d,%03d) ", obj[19], obj[20], obj[21]);	
			printf("\n                            ");
			printf(" (%03d,%03d,%03d) ", obj[22], obj[23], obj[24]);	
			printf("\n                      ");
			break;
		case 14: printf("Texture:  %02xh\n"
				"                      ", obj[9]);
			printf("Points=(%03d,%03d,%03d) ", obj[10], obj[11], obj[12]);	
			printf(" (%03d,%03d,%03d) ", obj[13], obj[14], obj[15]);	
			printf("\n                            ");
			printf(" (%03d,%03d,%03d) ", obj[16], obj[17], obj[18]);	
			printf(" (%03d,%03d,%03d) ", obj[19], obj[20], obj[21]);	
			printf("\n                            ");
			printf(" (%03d,%03d,%03d) ", obj[22], obj[23], obj[24]);	
			printf(" (%03d,%03d,%03d) ", obj[25], obj[26], obj[27]);	
			printf("\n                      ");
			break;

		default: for (n = 9; n < dsize; n++)
			{
				printf("0x%02x ", obj[n]);
			}
			printf("\n                      ");
			break;
	}
	show_code(obj + dsize, obj[8] - dsize);
	printf("\n");
}


void dump_driller(void)
{
	byte room, obj;
	byte roomcount = *sna(gl_dbase);
	word rmbase;
	byte *rmbuf, *objbuf;
	word dsize, mcs;

	for (room = gl_room; room < roomcount; room++)
	{
		rmbase = peek(gl_dbase + 0xC8 + 2*room);
		if (!rmbase) continue;
		rmbuf = sna(gl_dbase + rmbase);

		// Room colour
		switch(rmbuf[9])
		{
			case 0: gl_roomr = 0; gl_roomg = 0; gl_roomb = 0; break;
			case 1: gl_roomr = 0; gl_roomg = 0; gl_roomb = 1; break;
			case 2: gl_roomr = 1; gl_roomg = 0; gl_roomb = 0; break;
			case 3: gl_roomr = 1; gl_roomg = 0; gl_roomb = 1; break;
			case 4: gl_roomr = 0; gl_roomg = 1; gl_roomb = 0; break;
			case 5: gl_roomr = 0; gl_roomg = 1; gl_roomb = 1; break;
			case 6: gl_roomr = 1; gl_roomg = 1; gl_roomb = 0; break;
			case 7: gl_roomr = 1; gl_roomg = 1; gl_roomb = 1; break;
		}
		
		if (!gl_pov) 
		{
			printf("Room %03d: ", rmbuf[2]);
			switch (gl_game)
			{
				case GA_DARKSIDE:
					printf("%-15.15s\n", rmbuf + 10);
					break;
				case GA_DRILLER:
					printf("%-12.12s\n", rmbuf + 13);
					break;
				case GA_ECLIPSE:
					printf("%s-%c %-3.3s\n", te_room(rmbuf[10]), rmbuf[11], rmbuf + 12); 
					break;
			}
			printf("Byte 0: 0x%02x\n", rmbuf[0]);
			printf("Scale: %d\n", rmbuf[5]);
			printf("Colours: %d %d %d %d\n", 
				rmbuf[6], rmbuf[7], rmbuf[8], rmbuf[9]);
			switch (gl_game)
			{
				case GA_DRILLER: printf("Gas pocket: (%d, %d) radius %d\n", 
							rmbuf[10], rmbuf[11], rmbuf[12]);
				break;
				default:
				break;
			}
		}
		switch (gl_game)
		{
			case GA_DARKSIDE:
			case GA_DRILLER:
				objbuf = rmbuf + 25;
				break;
			case GA_ECLIPSE:
				objbuf = rmbuf + 15;
				break;
		}
		for (obj = 0; obj < rmbuf[1]; obj++)
		{
			if (gl_pov) showobj_pov(objbuf);
			else showobj(objbuf);
			objbuf += objbuf[8];	
		}
		if (gl_pov) return;	// XXX Only does Amethyst
		objbuf = rmbuf + (rmbuf[3] + 256 * rmbuf[4]);
		mcs = *objbuf++;
		while (mcs)
		{
			dsize = *objbuf++;
			printf("Room subroutine [%d]: \n"
				"                      ", dsize);
			show_code(objbuf, dsize);
			printf("\n");
			objbuf += dsize;
			--mcs;
		}
	}
	objbuf = sna(gl_dbase + peek(gl_dbase + 0x48));
	mcs = *objbuf++;
	while (mcs)
	{
		dsize = *objbuf++;
		printf("Global routine [%d]: \n"
		       "                      ", dsize);
		show_code(objbuf, dsize);
		printf("\n");
		objbuf += dsize;
		--mcs;
	}
}

int main(int argc, char **argv)
{
	char *fname = "driller.sna";
	FILE *fp;
	int n;

	if (argc > 1) for (n = 1; n < argc; n++)
	{
		if (argv[n][0] == '-')
		{
			if (!strcmp(argv[n], "-pc")  || !strcmp(argv[n], "-PC"))
			{
				gl_dbase   = 0x9B40;
				gl_snalen  = 51944;
				gl_snabase = 0;
			}
			if (!strcmp(argv[n], "-cpc")  || !strcmp(argv[n], "-CPC"))
			{
				gl_dbase   = 0x79AD;
				gl_snalen  = 0xC000;
				gl_snabase = 0;
			}
			if (!strcmp(argv[n], "-dark") || !strcmp(argv[n], "-DARK")) 
			{
				gl_game = GA_DARKSIDE;
				gl_dbase = 0xC9CE;
			}
			if (!strcmp(argv[n], "-eclipse") || !strcmp(argv[n], "-ECLIPSE")) 
			{
				gl_game = GA_ECLIPSE;
				gl_dbase = 0xCDB7;
			}
			if (!strcmp(argv[n], "-pov") || !strcmp(argv[n], "-POV")) 
				gl_pov = 1;
			if (!strcmp(argv[n], "-room") || !strcmp(argv[n], "-ROOM"))
				gl_room = atoi(argv[++n]);
		}
		else fname = argv[1];
	}
	fp = fopen(fname, "rb");

	if (!fp) 
	{
		perror(fname);
		return EXIT_FAILURE;
	}	
	fread(sna(gl_snabase), 1, gl_snalen, fp);
	fclose(fp);

	dump_driller();
	return EXIT_SUCCESS;
}

