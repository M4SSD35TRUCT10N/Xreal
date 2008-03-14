/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.
Copyright (C) 2001-2006 William Joseph
Copyright (C) 2006-2008 Robert Beckebans <trebor_7@users.sourceforge.net>

This file is part of XreaL source code.

XreaL source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

XreaL source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with XreaL source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

// q_math.c -- stateless support routines that are included in each code module
#include "q_shared.h"
#include <float.h>

int             nonansicast = 0;

// *INDENT-OFF*
vec3_t          vec3_origin = { 0, 0, 0 };

vec3_t  axisDefault[3] = {{ 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }};

matrix_t matrixIdentity = {	1, 0, 0, 0,
							0, 1, 0, 0,
							0, 0, 1, 0,
							0, 0, 0, 1};

quat_t          quatIdentity = { 0, 0, 0, 1 };

vec4_t          colorBlack      = {0, 0, 0, 1};
vec4_t          colorRed        = {1, 0, 0, 1};
vec4_t          colorGreen      = {0, 1, 0, 1};
vec4_t          colorBlue       = {0, 0, 1, 1};
vec4_t          colorYellow     = {1, 1, 0, 1};
vec4_t          colorMagenta	= {1, 0, 1, 1};
vec4_t          colorCyan       = {0, 1, 1, 1};
vec4_t          colorWhite      = {1, 1, 1, 1};
vec4_t          colorLtGrey     = {0.75, 0.75, 0.75, 1};
vec4_t          colorMdGrey     = {0.5, 0.5, 0.5, 1};
vec4_t          colorDkGrey     = {0.25, 0.25, 0.25, 1};

const vec4_t g_color_table[MAX_CCODES] = 
{
	{0.00000f, 0.00000f, 0.00000f, 1.00000f},	// 0 standard colors
	{1.00000f, 0.00000f, 0.00000f, 1.00000f},	// 1 |
	{0.00000f, 1.00000f, 0.00000f, 1.00000f},	// 2 v
	{1.00000f, 1.00000f, 0.00000f, 1.00000f},	// 3
	{0.00000f, 0.00000f, 1.00000f, 1.00000f},	// 4
	{0.00000f, 1.00000f, 1.00000f, 1.00000f},	// 5
	{1.00000f, 0.00000f, 1.00000f, 1.00000f},	// 6 ^
	{1.00000f, 1.00000f, 1.00000f, 1.00000f},	// 7 |
	{1.00000f, 0.50000f, 0.00000f, 1.00000f},	// 8
	{0.60000f, 0.60000f, 1.00000f, 1.00000f},	// 9

	{1.00000f, 0.00000f, 0.00000f, 1.00000f},	// a Rainbow
	{1.00000f, 0.13239f, 0.00000f, 1.00000f},	// A |
	{1.00000f, 0.26795f, 0.00000f, 1.00000f},	// b |
	{1.00000f, 0.37829f, 0.00000f, 1.00000f},	// B v
	{1.00000f, 0.50000f, 0.00000f, 1.00000f},	// c 
	{1.00000f, 0.60633f, 0.00000f, 1.00000f},	// C 
	{1.00000f, 0.73205f, 0.00000f, 1.00000f},	// d 
	{1.00000f, 0.84990f, 0.00000f, 1.00000f},	// D 
	{1.00000f, 1.00000f, 0.00000f, 1.00000f},	// e 
	{0.86761f, 1.00000f, 0.00000f, 1.00000f},	// E 
	{0.73205f, 1.00000f, 0.00000f, 1.00000f},	// f 
	{0.62171f, 1.00000f, 0.00000f, 1.00000f},	// F 
	{0.50000f, 1.00000f, 0.00000f, 1.00000f},	// g 
	{0.39367f, 1.00000f, 0.00000f, 1.00000f},	// G 
	{0.26795f, 1.00000f, 0.00000f, 1.00000f},	// h 
	{0.15010f, 1.00000f, 0.00000f, 1.00000f},	// H 
	{0.00000f, 1.00000f, 0.00000f, 1.00000f},	// i 
	{0.00000f, 1.00000f, 0.13239f, 1.00000f},	// I 
	{0.00000f, 1.00000f, 0.26795f, 1.00000f},	// j 
	{0.00000f, 1.00000f, 0.37829f, 1.00000f},	// J 
	{0.00000f, 1.00000f, 0.50000f, 1.00000f},	// k 
	{0.00000f, 1.00000f, 0.60633f, 1.00000f},	// K 
	{0.00000f, 1.00000f, 0.73205f, 1.00000f},	// l 
	{0.00000f, 1.00000f, 0.84990f, 1.00000f},	// L 
	{0.00000f, 1.00000f, 1.00000f, 1.00000f},	// m 
	{0.00000f, 0.86761f, 1.00000f, 1.00000f},	// M 
	{0.00000f, 0.73205f, 1.00000f, 1.00000f},	// n 
	{0.00000f, 0.62171f, 1.00000f, 1.00000f},	// N 
	{0.00000f, 0.50000f, 1.00000f, 1.00000f},	// o 
	{0.00000f, 0.39367f, 1.00000f, 1.00000f},	// O 
	{0.00000f, 0.26795f, 1.00000f, 1.00000f},	// p 
	{0.00000f, 0.15010f, 1.00000f, 1.00000f},	// P 
	{0.00000f, 0.00000f, 1.00000f, 1.00000f},	// q 
	{0.13239f, 0.00000f, 1.00000f, 1.00000f},	// Q 
	{0.26795f, 0.00000f, 1.00000f, 1.00000f},	// r 
	{0.37829f, 0.00000f, 1.00000f, 1.00000f},	// R 
	{0.50000f, 0.00000f, 1.00000f, 1.00000f},	// s 
	{0.60633f, 0.00000f, 1.00000f, 1.00000f},	// S 
	{0.73205f, 0.00000f, 1.00000f, 1.00000f},	// t 
	{0.84990f, 0.00000f, 1.00000f, 1.00000f},	// T 
	{1.00000f, 0.00000f, 1.00000f, 1.00000f},	// u 
	{1.00000f, 0.00000f, 0.86761f, 1.00000f},	// U 
	{1.00000f, 0.00000f, 0.73205f, 1.00000f},	// v 
	{1.00000f, 0.00000f, 0.62171f, 1.00000f},	// V 
	{1.00000f, 0.00000f, 0.50000f, 1.00000f},	// w ^
	{1.00000f, 0.00000f, 0.39367f, 1.00000f},	// W |
	{1.00000f, 0.00000f, 0.26795f, 1.00000f},	// x |
	{1.00000f, 0.00000f, 0.15010f, 1.00000f},	// X Rainbow

	{0.75000f, 0.75000f, 0.75000f, 1.00000f},	// y Grey 
	{0.50000f, 0.50000f, 0.50000f, 1.00000f},	// Y |
	{0.25000f, 0.25000f, 0.25000f, 1.00000f},	// z Grey
	{1.00000f, 0.50000f, 1.00000f, 1.00000f},	// Z 
};

vec3_t  bytedirs[NUMVERTEXNORMALS] =
{
	{-0.525731f, 0.000000f, 0.850651f}, {-0.442863f, 0.238856f, 0.864188f}, 
	{-0.295242f, 0.000000f, 0.955423f}, {-0.309017f, 0.500000f, 0.809017f}, 
	{-0.162460f, 0.262866f, 0.951056f}, {0.000000f, 0.000000f, 1.000000f}, 
	{0.000000f, 0.850651f, 0.525731f}, {-0.147621f, 0.716567f, 0.681718f}, 
	{0.147621f, 0.716567f, 0.681718f}, {0.000000f, 0.525731f, 0.850651f}, 
	{0.309017f, 0.500000f, 0.809017f}, {0.525731f, 0.000000f, 0.850651f}, 
	{0.295242f, 0.000000f, 0.955423f}, {0.442863f, 0.238856f, 0.864188f}, 
	{0.162460f, 0.262866f, 0.951056f}, {-0.681718f, 0.147621f, 0.716567f}, 
	{-0.809017f, 0.309017f, 0.500000f},{-0.587785f, 0.425325f, 0.688191f}, 
	{-0.850651f, 0.525731f, 0.000000f},{-0.864188f, 0.442863f, 0.238856f}, 
	{-0.716567f, 0.681718f, 0.147621f},{-0.688191f, 0.587785f, 0.425325f}, 
	{-0.500000f, 0.809017f, 0.309017f}, {-0.238856f, 0.864188f, 0.442863f}, 
	{-0.425325f, 0.688191f, 0.587785f}, {-0.716567f, 0.681718f, -0.147621f}, 
	{-0.500000f, 0.809017f, -0.309017f}, {-0.525731f, 0.850651f, 0.000000f}, 
	{0.000000f, 0.850651f, -0.525731f}, {-0.238856f, 0.864188f, -0.442863f}, 
	{0.000000f, 0.955423f, -0.295242f}, {-0.262866f, 0.951056f, -0.162460f}, 
	{0.000000f, 1.000000f, 0.000000f}, {0.000000f, 0.955423f, 0.295242f}, 
	{-0.262866f, 0.951056f, 0.162460f}, {0.238856f, 0.864188f, 0.442863f}, 
	{0.262866f, 0.951056f, 0.162460f}, {0.500000f, 0.809017f, 0.309017f}, 
	{0.238856f, 0.864188f, -0.442863f},{0.262866f, 0.951056f, -0.162460f}, 
	{0.500000f, 0.809017f, -0.309017f},{0.850651f, 0.525731f, 0.000000f}, 
	{0.716567f, 0.681718f, 0.147621f}, {0.716567f, 0.681718f, -0.147621f}, 
	{0.525731f, 0.850651f, 0.000000f}, {0.425325f, 0.688191f, 0.587785f}, 
	{0.864188f, 0.442863f, 0.238856f}, {0.688191f, 0.587785f, 0.425325f}, 
	{0.809017f, 0.309017f, 0.500000f}, {0.681718f, 0.147621f, 0.716567f}, 
	{0.587785f, 0.425325f, 0.688191f}, {0.955423f, 0.295242f, 0.000000f}, 
	{1.000000f, 0.000000f, 0.000000f}, {0.951056f, 0.162460f, 0.262866f}, 
	{0.850651f, -0.525731f, 0.000000f},{0.955423f, -0.295242f, 0.000000f}, 
	{0.864188f, -0.442863f, 0.238856f}, {0.951056f, -0.162460f, 0.262866f}, 
	{0.809017f, -0.309017f, 0.500000f}, {0.681718f, -0.147621f, 0.716567f}, 
	{0.850651f, 0.000000f, 0.525731f}, {0.864188f, 0.442863f, -0.238856f}, 
	{0.809017f, 0.309017f, -0.500000f}, {0.951056f, 0.162460f, -0.262866f}, 
	{0.525731f, 0.000000f, -0.850651f}, {0.681718f, 0.147621f, -0.716567f}, 
	{0.681718f, -0.147621f, -0.716567f},{0.850651f, 0.000000f, -0.525731f}, 
	{0.809017f, -0.309017f, -0.500000f}, {0.864188f, -0.442863f, -0.238856f}, 
	{0.951056f, -0.162460f, -0.262866f}, {0.147621f, 0.716567f, -0.681718f}, 
	{0.309017f, 0.500000f, -0.809017f}, {0.425325f, 0.688191f, -0.587785f}, 
	{0.442863f, 0.238856f, -0.864188f}, {0.587785f, 0.425325f, -0.688191f}, 
	{0.688191f, 0.587785f, -0.425325f}, {-0.147621f, 0.716567f, -0.681718f}, 
	{-0.309017f, 0.500000f, -0.809017f}, {0.000000f, 0.525731f, -0.850651f}, 
	{-0.525731f, 0.000000f, -0.850651f}, {-0.442863f, 0.238856f, -0.864188f}, 
	{-0.295242f, 0.000000f, -0.955423f}, {-0.162460f, 0.262866f, -0.951056f}, 
	{0.000000f, 0.000000f, -1.000000f}, {0.295242f, 0.000000f, -0.955423f}, 
	{0.162460f, 0.262866f, -0.951056f}, {-0.442863f, -0.238856f, -0.864188f}, 
	{-0.309017f, -0.500000f, -0.809017f}, {-0.162460f, -0.262866f, -0.951056f}, 
	{0.000000f, -0.850651f, -0.525731f}, {-0.147621f, -0.716567f, -0.681718f}, 
	{0.147621f, -0.716567f, -0.681718f}, {0.000000f, -0.525731f, -0.850651f}, 
	{0.309017f, -0.500000f, -0.809017f}, {0.442863f, -0.238856f, -0.864188f}, 
	{0.162460f, -0.262866f, -0.951056f}, {0.238856f, -0.864188f, -0.442863f}, 
	{0.500000f, -0.809017f, -0.309017f}, {0.425325f, -0.688191f, -0.587785f}, 
	{0.716567f, -0.681718f, -0.147621f}, {0.688191f, -0.587785f, -0.425325f}, 
	{0.587785f, -0.425325f, -0.688191f}, {0.000000f, -0.955423f, -0.295242f}, 
	{0.000000f, -1.000000f, 0.000000f}, {0.262866f, -0.951056f, -0.162460f}, 
	{0.000000f, -0.850651f, 0.525731f}, {0.000000f, -0.955423f, 0.295242f}, 
	{0.238856f, -0.864188f, 0.442863f}, {0.262866f, -0.951056f, 0.162460f}, 
	{0.500000f, -0.809017f, 0.309017f}, {0.716567f, -0.681718f, 0.147621f}, 
	{0.525731f, -0.850651f, 0.000000f}, {-0.238856f, -0.864188f, -0.442863f}, 
	{-0.500000f, -0.809017f, -0.309017f}, {-0.262866f, -0.951056f, -0.162460f}, 
	{-0.850651f, -0.525731f, 0.000000f}, {-0.716567f, -0.681718f, -0.147621f}, 
	{-0.716567f, -0.681718f, 0.147621f}, {-0.525731f, -0.850651f, 0.000000f}, 
	{-0.500000f, -0.809017f, 0.309017f}, {-0.238856f, -0.864188f, 0.442863f}, 
	{-0.262866f, -0.951056f, 0.162460f}, {-0.864188f, -0.442863f, 0.238856f}, 
	{-0.809017f, -0.309017f, 0.500000f}, {-0.688191f, -0.587785f, 0.425325f}, 
	{-0.681718f, -0.147621f, 0.716567f}, {-0.442863f, -0.238856f, 0.864188f}, 
	{-0.587785f, -0.425325f, 0.688191f}, {-0.309017f, -0.500000f, 0.809017f}, 
	{-0.147621f, -0.716567f, 0.681718f}, {-0.425325f, -0.688191f, 0.587785f}, 
	{-0.162460f, -0.262866f, 0.951056f}, {0.442863f, -0.238856f, 0.864188f}, 
	{0.162460f, -0.262866f, 0.951056f}, {0.309017f, -0.500000f, 0.809017f}, 
	{0.147621f, -0.716567f, 0.681718f}, {0.000000f, -0.525731f, 0.850651f}, 
	{0.425325f, -0.688191f, 0.587785f}, {0.587785f, -0.425325f, 0.688191f}, 
	{0.688191f, -0.587785f, 0.425325f}, {-0.955423f, 0.295242f, 0.000000f}, 
	{-0.951056f, 0.162460f, 0.262866f}, {-1.000000f, 0.000000f, 0.000000f}, 
	{-0.850651f, 0.000000f, 0.525731f}, {-0.955423f, -0.295242f, 0.000000f}, 
	{-0.951056f, -0.162460f, 0.262866f}, {-0.864188f, 0.442863f, -0.238856f}, 
	{-0.951056f, 0.162460f, -0.262866f}, {-0.809017f, 0.309017f, -0.500000f}, 
	{-0.864188f, -0.442863f, -0.238856f}, {-0.951056f, -0.162460f, -0.262866f}, 
	{-0.809017f, -0.309017f, -0.500000f}, {-0.681718f, 0.147621f, -0.716567f}, 
	{-0.681718f, -0.147621f, -0.716567f}, {-0.850651f, 0.000000f, -0.525731f}, 
	{-0.688191f, 0.587785f, -0.425325f}, {-0.587785f, 0.425325f, -0.688191f}, 
	{-0.425325f, 0.688191f, -0.587785f}, {-0.425325f, -0.688191f, -0.587785f}, 
	{-0.587785f, -0.425325f, -0.688191f}, {-0.688191f, -0.587785f, -0.425325f}
};
// *INDENT-ON*

//==============================================================

int Q_rand(int *seed)
{
	*seed = (69069 * *seed + 1);
	return *seed;
}

float Q_random(int *seed)
{
	return (Q_rand(seed) & 0xffff) / (float)0x10000;
}

float Q_crandom(int *seed)
{
	return 2.0 * (Q_random(seed) - 0.5);
}

//=======================================================

byte ClampByte(int i)
{
	if(i < 0)
	{
		return 0;
	}
	if(i > 255)
	{
		return 255;
	}
	return i;
}

signed char ClampChar(int i)
{
	if(i < -128)
	{
		return -128;
	}
	if(i > 127)
	{
		return 127;
	}
	return i;
}

signed short ClampShort(int i)
{
	if(i < -32768)
	{
		return -32768;
	}
	if(i > 0x7fff)
	{
		return 0x7fff;
	}
	return i;
}


// this isn't a real cheap function to call!
int DirToByte(vec3_t dir)
{
	int             i, best;
	float           d, bestd;

	if(!dir)
	{
		return 0;
	}

	bestd = 0;
	best = 0;
	for(i = 0; i < NUMVERTEXNORMALS; i++)
	{
		d = DotProduct(dir, bytedirs[i]);
		if(d > bestd)
		{
			bestd = d;
			best = i;
		}
	}

	return best;
}

void ByteToDir(int b, vec3_t dir)
{
	if(b < 0 || b >= NUMVERTEXNORMALS)
	{
		VectorCopy(vec3_origin, dir);
		return;
	}
	VectorCopy(bytedirs[b], dir);
}


unsigned ColorBytes3(float r, float g, float b)
{
	unsigned        i;

	((byte *) & i)[0] = r * 255;
	((byte *) & i)[1] = g * 255;
	((byte *) & i)[2] = b * 255;

	return i;
}

unsigned ColorBytes4(float r, float g, float b, float a)
{
	unsigned        i;

	((byte *) & i)[0] = r * 255;
	((byte *) & i)[1] = g * 255;
	((byte *) & i)[2] = b * 255;
	((byte *) & i)[3] = a * 255;

	return i;
}

float NormalizeColor(const vec3_t in, vec3_t out)
{
	float           max;

	max = in[0];
	if(in[1] > max)
	{
		max = in[1];
	}
	if(in[2] > max)
	{
		max = in[2];
	}

	if(!max)
	{
		VectorClear(out);
	}
	else
	{
		out[0] = in[0] / max;
		out[1] = in[1] / max;
		out[2] = in[2] / max;
	}
	return max;
}

void ClampColor(vec4_t color)
{
	int             i;

	for(i = 0; i < 4; i++)
	{
		if(color[i] < 0)
			color[i] = 0;

		if(color[i] > 1)
			color[i] = 1;
	}
}

// Rounds the argument to the next integer. Used by SnapVector.
/*
void init_tonextint()
{
	float           decimal = 0.9f;

	nonansicast = (int)decimal;
}

float tonextint(float x)
{
	int             casted;
	float           rest;

	if(nonansicast)
		return (int)x;

	casted = (int)x;
	rest = x - (float)casted;

	if(rest >= 0.5f)
		return casted + 1;
	else if(rest <= -0.5f)
		return casted - 1;
	else
		return casted;
}
*/

vec_t PlaneNormalize(vec4_t plane)
{
	vec_t           length, ilength;

	length = sqrt(plane[0] * plane[0] + plane[1] * plane[1] + plane[2] * plane[2]);
	if(length == 0)
	{
		VectorClear(plane);
		return 0;
	}

	ilength = 1.0 / length;
	plane[0] = plane[0] * ilength;
	plane[1] = plane[1] * ilength;
	plane[2] = plane[2] * ilength;
	plane[3] = plane[3] * ilength;

	return length;
}

/*
=====================
PlaneFromPoints

Returns false if the triangle is degenrate.
=====================
*/
qboolean PlaneFromPoints(vec4_t plane, const vec3_t a, const vec3_t b, const vec3_t c, qboolean cw)
{
	vec3_t          d1, d2;

	VectorSubtract(b, a, d1);
	VectorSubtract(c, a, d2);

	if(cw)
	{
		CrossProduct(d2, d1, plane);
	}
	else
	{
		CrossProduct(d1, d2, plane);
	}

	if(VectorNormalize(plane) == 0)
	{
		return qfalse;
	}

	plane[3] = DotProduct(a, plane);
	return qtrue;
}

/*
===============
RotatePointAroundVector
===============
*/
void RotatePointAroundVector(vec3_t dst, const vec3_t dir, const vec3_t point, float degrees)
{
	vec3_t          m[3];
	vec3_t          im[3];
	vec3_t          zrot[3];
	vec3_t          tmpmat[3];
	vec3_t          rot[3];

	vec3_t          vr, vup, vf;

	vf[0] = dir[0];
	vf[1] = dir[1];
	vf[2] = dir[2];

	PerpendicularVector(vr, dir);
	CrossProduct(vr, vf, vup);

	m[0][0] = vr[0];
	m[1][0] = vr[1];
	m[2][0] = vr[2];

	m[0][1] = vup[0];
	m[1][1] = vup[1];
	m[2][1] = vup[2];

	m[0][2] = vf[0];
	m[1][2] = vf[1];
	m[2][2] = vf[2];

	im[0][0] = m[0][0];
	im[0][1] = m[1][0];
	im[0][2] = m[2][0];

	im[1][0] = m[0][1];
	im[1][1] = m[1][1];
	im[1][2] = m[2][1];

	im[2][0] = m[0][2];
	im[2][1] = m[1][2];
	im[2][2] = m[2][2];

	zrot[0][0] = (float)cos(DEG2RAD(degrees));
	zrot[0][1] = (float)sin(DEG2RAD(degrees));
	zrot[0][2] = 0;

	zrot[1][0] = (float)-sin(DEG2RAD(degrees));
	zrot[1][1] = (float)cos(DEG2RAD(degrees));
	zrot[1][2] = 0;

	zrot[2][0] = 0.0f;
	zrot[2][1] = 0.0f;
	zrot[2][2] = 1.0f;

	AxisMultiply(m, zrot, tmpmat);
	AxisMultiply(tmpmat, im, rot);

	dst[0] = rot[0][0] * point[0] + rot[0][1] * point[1] + rot[0][2] * point[2];
	dst[1] = rot[1][0] * point[0] + rot[1][1] * point[1] + rot[1][2] * point[2];
	dst[2] = rot[2][0] * point[0] + rot[2][1] * point[1] + rot[2][2] * point[2];
}

/*
===============
RotateAroundDirection
===============
*/
void RotateAroundDirection(vec3_t axis[3], float yaw)
{

	// create an arbitrary axis[1] 
	PerpendicularVector(axis[1], axis[0]);

	// rotate it around axis[0] by yaw
	if(yaw)
	{
		vec3_t          temp;

		VectorCopy(axis[1], temp);
		RotatePointAroundVector(axis[1], axis[0], temp, yaw);
	}

	// cross to get axis[2]
	CrossProduct(axis[0], axis[1], axis[2]);
}

/*
=====================
Q_acos

the msvc acos doesn't always return a value between -PI and PI:

int i;
i = 1065353246;
acos(*(float*) &i) == -1.#IND0
=====================
*/
float Q_acos(float c)
{
	float           angle;

	angle = acos(c);

	if(angle > M_PI)
	{
		return (float)M_PI;
	}
	else if(angle < -M_PI)
	{
		return (float)M_PI;
	}
	return angle;
}

/*
================
Q_isnan

Don't pass doubles to this
================
*/
int Q_isnan(float x)
{
	union
	{
		float           f;
		unsigned int    i;
	} t;

	t.f = x;
	t.i &= 0x7FFFFFFF;
	t.i = 0x7F800000 - t.i;

	return (int)((unsigned int)t.i >> 31);
}

void vectoangles(const vec3_t value1, vec3_t angles)
{
	float           forward;
	float           yaw, pitch;

	if(value1[1] == 0 && value1[0] == 0)
	{
		yaw = 0;
		if(value1[2] > 0)
		{
			pitch = 90;
		}
		else
		{
			pitch = 270;
		}
	}
	else
	{
		if(value1[0])
		{
			yaw = (atan2(value1[1], value1[0]) * 180 / M_PI);
		}
		else if(value1[1] > 0)
		{
			yaw = 90;
		}
		else
		{
			yaw = 270;
		}
		if(yaw < 0)
		{
			yaw += 360;
		}

		forward = sqrt(value1[0] * value1[0] + value1[1] * value1[1]);
		pitch = (atan2(value1[2], forward) * 180 / M_PI);
		if(pitch < 0)
		{
			pitch += 360;
		}
	}

	angles[PITCH] = -pitch;
	angles[YAW] = yaw;
	angles[ROLL] = 0;
}


/*
=================
AnglesToAxis
=================
*/
void AnglesToAxis(const vec3_t angles, vec3_t axis[3])
{
	vec3_t          right;

	// angle vectors returns "right" instead of "y axis"
	AngleVectors(angles, axis[0], right, axis[2]);
	VectorSubtract(vec3_origin, right, axis[1]);
}

void AxisClear(vec3_t axis[3])
{
	axis[0][0] = 1;
	axis[0][1] = 0;
	axis[0][2] = 0;
	axis[1][0] = 0;
	axis[1][1] = 1;
	axis[1][2] = 0;
	axis[2][0] = 0;
	axis[2][1] = 0;
	axis[2][2] = 1;
}

void AxisCopy(vec3_t in[3], vec3_t out[3])
{
	VectorCopy(in[0], out[0]);
	VectorCopy(in[1], out[1]);
	VectorCopy(in[2], out[2]);
}

void ProjectPointOnPlane(vec3_t dst, const vec3_t p, const vec3_t normal)
{
	float           d;
	vec3_t          n;
	float           inv_denom;

	inv_denom = DotProduct(normal, normal);
#ifndef Q3_VM
	//assert(Q_fabs(inv_denom) != 0.0f);    // bk010122 - zero vectors get here
#endif
	inv_denom = 1.0f / inv_denom;

	d = DotProduct(normal, p) * inv_denom;

	n[0] = normal[0] * inv_denom;
	n[1] = normal[1] * inv_denom;
	n[2] = normal[2] * inv_denom;

	dst[0] = p[0] - d * n[0];
	dst[1] = p[1] - d * n[1];
	dst[2] = p[2] - d * n[2];
}

/*
================
MakeNormalVectors

Given a normalized forward vector, create two
other perpendicular vectors
================
*/
void MakeNormalVectors(const vec3_t forward, vec3_t right, vec3_t up)
{
	float           d;

	// this rotate and negate guarantees a vector
	// not colinear with the original
	right[1] = -forward[0];
	right[2] = forward[1];
	right[0] = forward[2];

	d = DotProduct(right, forward);
	VectorMA(right, -d, forward, right);
	VectorNormalize(right);
	CrossProduct(right, forward, up);
}


void VectorRotate(vec3_t in, vec3_t matrix[3], vec3_t out)
{
	out[0] = DotProduct(in, matrix[0]);
	out[1] = DotProduct(in, matrix[1]);
	out[2] = DotProduct(in, matrix[2]);
}


//============================================================

/*
===============
LerpAngle

===============
*/
float LerpAngle(float from, float to, float frac)
{
	float           a;

	if(to - from > 180)
	{
		to -= 360;
	}
	if(to - from < -180)
	{
		to += 360;
	}
	a = from + frac * (to - from);

	return a;
}


/*
=================
AngleSubtract

Always returns a value from -180 to 180
=================
*/
float AngleSubtract(float a1, float a2)
{
	float           a;

	a = a1 - a2;
	while(a > 180)
	{
		a -= 360;
	}
	while(a < -180)
	{
		a += 360;
	}
	return a;
}


void AnglesSubtract(vec3_t v1, vec3_t v2, vec3_t v3)
{
	v3[0] = AngleSubtract(v1[0], v2[0]);
	v3[1] = AngleSubtract(v1[1], v2[1]);
	v3[2] = AngleSubtract(v1[2], v2[2]);
}


float AngleMod(float a)
{
	a = (360.0 / 65536) * ((int)(a * (65536 / 360.0)) & 65535);
	return a;
}


/*
=================
AngleNormalize360

returns angle normalized to the range [0 <= angle < 360]
=================
*/
float AngleNormalize360(float angle)
{
	return (360.0 / 65536) * ((int)(angle * (65536 / 360.0)) & 65535);
}


/*
=================
AngleNormalize180

returns angle normalized to the range [-180 < angle <= 180]
=================
*/
float AngleNormalize180(float angle)
{
	angle = AngleNormalize360(angle);
	if(angle > 180.0)
	{
		angle -= 360.0;
	}
	return angle;
}


/*
=================
AngleDelta

returns the normalized delta from angle1 to angle2
=================
*/
float AngleDelta(float angle1, float angle2)
{
	return AngleNormalize180(angle1 - angle2);
}


/*
=================
AngleBetweenVectors

returns the angle between two vectors normalized to the range [0 <= angle <= 180]
=================
*/
float AngleBetweenVectors(const vec3_t a, const vec3_t b)
{
	vec_t           alen, blen;

	alen = VectorLength(a);
	blen = VectorLength(b);

	// complete dot product of two vectors a, b is |a| * |b| * cos(angle)
	// this results in:
	//
	// angle = acos( (a * b) / (|a| * |b|) )
	return RAD2DEG(Q_acos(DotProduct(a, b) / (alen * blen)));
}


//============================================================

/*
=================
SetPlaneSignbits
=================
*/
void SetPlaneSignbits(cplane_t * out)
{
	int             bits, j;

	// for fast box on planeside test
	bits = 0;
	for(j = 0; j < 3; j++)
	{
		if(out->normal[j] < 0)
		{
			bits |= 1 << j;
		}
	}
	out->signbits = bits;
}


/*
==================
BoxOnPlaneSide

Returns 1, 2, or 1 + 2

// this is the slow, general version
int BoxOnPlaneSide2(vec3_t mins, vec3_t maxs, vec4_t plane)
{
	int             i;
	float           dist1, dist2;
	int             sides;
	vec3_t          corners[2];

	for(i = 0; i < 3; i++)
	{
		if(plane[i] < 0)
		{
			corners[0][i] = mins[i];
			corners[1][i] = maxs[i];
		}
		else
		{
			corners[1][i] = mins[i];
			corners[0][i] = maxs[i];
		}
	}

	dist1 = DotProduct(plane, corners[0]) - plane[3];
	dist2 = DotProduct(plane, corners[1]) - plane[3];

	sides = 0;
	if(dist1 >= 0)
		sides = 1;
	if(dist2 < 0)
		sides |= 2;

	return sides;
}

==================
*/

#if !(defined(_MSC_VER) && id386)

int BoxOnPlaneSide(vec3_t emins, vec3_t emaxs, struct cplane_s *p)
{
	float           dist1, dist2;
	int             sides;

// fast axial cases
	if(p->type < 3)
	{
		if(p->dist <= emins[p->type])
			return 1;
		if(p->dist >= emaxs[p->type])
			return 2;
		return 3;
	}

// general case
	switch (p->signbits)
	{
		case 0:
			dist1 = p->normal[0] * emaxs[0] + p->normal[1] * emaxs[1] + p->normal[2] * emaxs[2];
			dist2 = p->normal[0] * emins[0] + p->normal[1] * emins[1] + p->normal[2] * emins[2];
			break;
		case 1:
			dist1 = p->normal[0] * emins[0] + p->normal[1] * emaxs[1] + p->normal[2] * emaxs[2];
			dist2 = p->normal[0] * emaxs[0] + p->normal[1] * emins[1] + p->normal[2] * emins[2];
			break;
		case 2:
			dist1 = p->normal[0] * emaxs[0] + p->normal[1] * emins[1] + p->normal[2] * emaxs[2];
			dist2 = p->normal[0] * emins[0] + p->normal[1] * emaxs[1] + p->normal[2] * emins[2];
			break;
		case 3:
			dist1 = p->normal[0] * emins[0] + p->normal[1] * emins[1] + p->normal[2] * emaxs[2];
			dist2 = p->normal[0] * emaxs[0] + p->normal[1] * emaxs[1] + p->normal[2] * emins[2];
			break;
		case 4:
			dist1 = p->normal[0] * emaxs[0] + p->normal[1] * emaxs[1] + p->normal[2] * emins[2];
			dist2 = p->normal[0] * emins[0] + p->normal[1] * emins[1] + p->normal[2] * emaxs[2];
			break;
		case 5:
			dist1 = p->normal[0] * emins[0] + p->normal[1] * emaxs[1] + p->normal[2] * emins[2];
			dist2 = p->normal[0] * emaxs[0] + p->normal[1] * emins[1] + p->normal[2] * emaxs[2];
			break;
		case 6:
			dist1 = p->normal[0] * emaxs[0] + p->normal[1] * emins[1] + p->normal[2] * emins[2];
			dist2 = p->normal[0] * emins[0] + p->normal[1] * emaxs[1] + p->normal[2] * emaxs[2];
			break;
		case 7:
			dist1 = p->normal[0] * emins[0] + p->normal[1] * emins[1] + p->normal[2] * emins[2];
			dist2 = p->normal[0] * emaxs[0] + p->normal[1] * emaxs[1] + p->normal[2] * emaxs[2];
			break;
		default:
			dist1 = dist2 = 0;	// shut up compiler
			break;
	}

	sides = 0;
	if(dist1 >= p->dist)
		sides = 1;
	if(dist2 < p->dist)
		sides |= 2;

	return sides;
}
#else
#pragma warning( disable: 4035 )

// *INDENT-OFF*
__declspec( naked ) int BoxOnPlaneSide (vec3_t emins, vec3_t emaxs, struct cplane_s *p)
{
	static int bops_initialized;
	static int Ljmptab[8];

	__asm {

		push ebx
			
		cmp bops_initialized, 1
		je  initialized
		mov bops_initialized, 1
		
		mov Ljmptab[0*4], offset Lcase0
		mov Ljmptab[1*4], offset Lcase1
		mov Ljmptab[2*4], offset Lcase2
		mov Ljmptab[3*4], offset Lcase3
		mov Ljmptab[4*4], offset Lcase4
		mov Ljmptab[5*4], offset Lcase5
		mov Ljmptab[6*4], offset Lcase6
		mov Ljmptab[7*4], offset Lcase7
			
initialized:

		mov edx,dword ptr[4+12+esp]
		mov ecx,dword ptr[4+4+esp]
		xor eax,eax
		mov ebx,dword ptr[4+8+esp]
		mov al,byte ptr[17+edx]
		cmp al,8
		jge Lerror
		fld dword ptr[0+edx]
		fld st(0)
		jmp dword ptr[Ljmptab+eax*4]
Lcase0:
		fmul dword ptr[ebx]
		fld dword ptr[0+4+edx]
		fxch st(2)
		fmul dword ptr[ecx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[4+ebx]
		fld dword ptr[0+8+edx]
		fxch st(2)
		fmul dword ptr[4+ecx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[8+ebx]
		fxch st(5)
		faddp st(3),st(0)
		fmul dword ptr[8+ecx]
		fxch st(1)
		faddp st(3),st(0)
		fxch st(3)
		faddp st(2),st(0)
		jmp LSetSides
Lcase1:
		fmul dword ptr[ecx]
		fld dword ptr[0+4+edx]
		fxch st(2)
		fmul dword ptr[ebx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[4+ebx]
		fld dword ptr[0+8+edx]
		fxch st(2)
		fmul dword ptr[4+ecx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[8+ebx]
		fxch st(5)
		faddp st(3),st(0)
		fmul dword ptr[8+ecx]
		fxch st(1)
		faddp st(3),st(0)
		fxch st(3)
		faddp st(2),st(0)
		jmp LSetSides
Lcase2:
		fmul dword ptr[ebx]
		fld dword ptr[0+4+edx]
		fxch st(2)
		fmul dword ptr[ecx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[4+ecx]
		fld dword ptr[0+8+edx]
		fxch st(2)
		fmul dword ptr[4+ebx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[8+ebx]
		fxch st(5)
		faddp st(3),st(0)
		fmul dword ptr[8+ecx]
		fxch st(1)
		faddp st(3),st(0)
		fxch st(3)
		faddp st(2),st(0)
		jmp LSetSides
Lcase3:
		fmul dword ptr[ecx]
		fld dword ptr[0+4+edx]
		fxch st(2)
		fmul dword ptr[ebx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[4+ecx]
		fld dword ptr[0+8+edx]
		fxch st(2)
		fmul dword ptr[4+ebx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[8+ebx]
		fxch st(5)
		faddp st(3),st(0)
		fmul dword ptr[8+ecx]
		fxch st(1)
		faddp st(3),st(0)
		fxch st(3)
		faddp st(2),st(0)
		jmp LSetSides
Lcase4:
		fmul dword ptr[ebx]
		fld dword ptr[0+4+edx]
		fxch st(2)
		fmul dword ptr[ecx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[4+ebx]
		fld dword ptr[0+8+edx]
		fxch st(2)
		fmul dword ptr[4+ecx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[8+ecx]
		fxch st(5)
		faddp st(3),st(0)
		fmul dword ptr[8+ebx]
		fxch st(1)
		faddp st(3),st(0)
		fxch st(3)
		faddp st(2),st(0)
		jmp LSetSides
Lcase5:
		fmul dword ptr[ecx]
		fld dword ptr[0+4+edx]
		fxch st(2)
		fmul dword ptr[ebx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[4+ebx]
		fld dword ptr[0+8+edx]
		fxch st(2)
		fmul dword ptr[4+ecx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[8+ecx]
		fxch st(5)
		faddp st(3),st(0)
		fmul dword ptr[8+ebx]
		fxch st(1)
		faddp st(3),st(0)
		fxch st(3)
		faddp st(2),st(0)
		jmp LSetSides
Lcase6:
		fmul dword ptr[ebx]
		fld dword ptr[0+4+edx]
		fxch st(2)
		fmul dword ptr[ecx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[4+ecx]
		fld dword ptr[0+8+edx]
		fxch st(2)
		fmul dword ptr[4+ebx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[8+ecx]
		fxch st(5)
		faddp st(3),st(0)
		fmul dword ptr[8+ebx]
		fxch st(1)
		faddp st(3),st(0)
		fxch st(3)
		faddp st(2),st(0)
		jmp LSetSides
Lcase7:
		fmul dword ptr[ecx]
		fld dword ptr[0+4+edx]
		fxch st(2)
		fmul dword ptr[ebx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[4+ecx]
		fld dword ptr[0+8+edx]
		fxch st(2)
		fmul dword ptr[4+ebx]
		fxch st(2)
		fld st(0)
		fmul dword ptr[8+ecx]
		fxch st(5)
		faddp st(3),st(0)
		fmul dword ptr[8+ebx]
		fxch st(1)
		faddp st(3),st(0)
		fxch st(3)
		faddp st(2),st(0)
LSetSides:
		faddp st(2),st(0)
		fcomp dword ptr[12+edx]
		xor ecx,ecx
		fnstsw ax
		fcomp dword ptr[12+edx]
		and ah,1
		xor ah,1
		add cl,ah
		fnstsw ax
		and ah,1
		add ah,ah
		add cl,ah
		pop ebx
		mov eax,ecx
		ret
Lerror:
		int 3
	}
}
// *INDENT-ON*
#pragma warning( default: 4035 )

#endif

/*
=================
RadiusFromBounds
=================
*/
float RadiusFromBounds(const vec3_t mins, const vec3_t maxs)
{
	int             i;
	vec3_t          corner;
	float           a, b;

	for(i = 0; i < 3; i++)
	{
		a = fabs(mins[i]);
		b = fabs(maxs[i]);
		corner[i] = a > b ? a : b;
	}

	return VectorLength(corner);
}


void ClearBounds(vec3_t mins, vec3_t maxs)
{
	mins[0] = mins[1] = mins[2] = 99999;
	maxs[0] = maxs[1] = maxs[2] = -99999;
}

void AddPointToBounds(const vec3_t v, vec3_t mins, vec3_t maxs)
{
	if(v[0] < mins[0])
	{
		mins[0] = v[0];
	}
	if(v[0] > maxs[0])
	{
		maxs[0] = v[0];
	}

	if(v[1] < mins[1])
	{
		mins[1] = v[1];
	}
	if(v[1] > maxs[1])
	{
		maxs[1] = v[1];
	}

	if(v[2] < mins[2])
	{
		mins[2] = v[2];
	}
	if(v[2] > maxs[2])
	{
		maxs[2] = v[2];
	}
}

qboolean BoundsIntersect(const vec3_t mins, const vec3_t maxs, const vec3_t mins2, const vec3_t maxs2)
{
	if(maxs[0] < mins2[0] ||
	   maxs[1] < mins2[1] || maxs[2] < mins2[2] || mins[0] > maxs2[0] || mins[1] > maxs2[1] || mins[2] > maxs2[2])
	{
		return qfalse;
	}

	return qtrue;
}

qboolean BoundsIntersectSphere(const vec3_t mins, const vec3_t maxs, const vec3_t origin, vec_t radius)
{
	if(origin[0] - radius > maxs[0] ||
	   origin[0] + radius < mins[0] ||
	   origin[1] - radius > maxs[1] ||
	   origin[1] + radius < mins[1] || origin[2] - radius > maxs[2] || origin[2] + radius < mins[2])
	{
		return qfalse;
	}

	return qtrue;
}

qboolean BoundsIntersectPoint(const vec3_t mins, const vec3_t maxs, const vec3_t origin)
{
	if(origin[0] > maxs[0] ||
	   origin[0] < mins[0] || origin[1] > maxs[1] || origin[1] < mins[1] || origin[2] > maxs[2] || origin[2] < mins[2])
	{
		return qfalse;
	}

	return qtrue;
}

/*
======================
SnapVectorTowards

Round a vector to integers for more efficient network
transmission, but make sure that it rounds towards a given point
rather than blindly truncating.  This prevents it from truncating 
into a wall.
======================
*/
void SnapVectorTowards(vec3_t v, vec3_t to)
{
	int             i;

	for(i = 0; i < 3; i++)
	{
		if(to[i] <= v[i])
			v[i] = (int)v[i];
		else
			v[i] = (int)v[i] + 1;
	}
}

// *INDENT-OFF*
vec_t VectorNormalize(vec3_t v)
{
#if id386_3dnow && defined __GNUC__ && 0
//#error VectorNormalize
	vec_t           length;

	femms();
	asm volatile
	(
													// lo                                		| hi
	"movq           (%%eax),        %%mm0\n"	// v[0]                                                                 | v[1]
	"movd           8(%%eax),       %%mm1\n"	// v[2]                                                                 | -
	// mm0[lo] = dot product(this)
	"pfmul          %%mm0,          %%mm0\n"	// v[0]*v[0]                                                    | v[1]*v[1]
	"pfmul          %%mm1,          %%mm1\n"	// v[2]*v[2]                                                    | -
	"pfacc          %%mm0,          %%mm0\n"	// v[0]*v[0]+v[1]*v[1]                                  | -
	"pfadd          %%mm1,          %%mm0\n"	// v[0]*v[0]+v[1]*v[1]+v[2]*v[2]                | -
	// mm0[lo] = sqrt(mm0[lo])
	"pfrsqrt        %%mm0,          %%mm1\n"	// 1/sqrt(dot)                                                  | 1/sqrt(dot)           (approx)
	"movq           %%mm1,          %%mm2\n"	// 1/sqrt(dot)                                                  | 1/sqrt(dot)           (approx)
	"pfmul          %%mm1,          %%mm1\n"	// (1/sqrt(dot))?                                               | (1/sqrt(dot))?        step 1
	"punpckldq      %%mm0,          %%mm0\n"	// dot                                                                  | dot                   (MMX instruction)
	"pfrsqit1       %%mm0,          %%mm1\n"	// intermediate                                                 | intermediate          step 2
	"pfrcpit2       %%mm2,          %%mm1\n"	// 1/sqrt(dot) (full 24-bit precision)  | 1/sqrt(dot)           step 3
	"pfmul          %%mm1,          %%mm0\n"	// sqrt(dot)                                                    | sqrt(dot)
	// len = mm0[lo]
	"movd           %%mm0,          (%%edx)\n"
	// load this into registers
	"movq           (%%eax),        %%mm2\n"	// v[0]                                                                 | v[1]
	"movd           8(%%eax),       %%mm3\n"	// v[2]                                                                 | -
	// scale this by the reciprocal square root
	"pfmul          %%mm1,          %%mm2\n"	// v[0]*1/sqrt(dot)                                             | v[1]*1/sqrt(dot)
	"pfmul          %%mm1,          %%mm3\n"	// v[2]*1/sqrt(dot)                                             | -
	// store scaled vector
	"movq           %%mm2,          (%%eax)\n"
	"movd           %%mm3,          8(%%eax)\n"
	:
	:"a" (v), "d"(&length)
	:"memory"
	);
	femms();
	return length;
#else
	// NOTE: TTimo - Apple G4 altivec source uses double?
	float           length, ilength;

	length = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	length = sqrt(length);

	if(length)
	{
		ilength = 1 / length;
		v[0] *= ilength;
		v[1] *= ilength;
		v[2] *= ilength;
	}

	return length;
#endif
}
// *INDENT-ON*

vec_t VectorNormalize2(const vec3_t v, vec3_t out)
{
	float           length, ilength;

	length = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	length = sqrt(length);

	if(length)
	{
#ifndef Q3_VM					// bk0101022 - FPE related
//    assert( ((Q_fabs(v[0])!=0.0f) || (Q_fabs(v[1])!=0.0f) || (Q_fabs(v[2])!=0.0f)) );
#endif
		ilength = 1 / length;
		out[0] = v[0] * ilength;
		out[1] = v[1] * ilength;
		out[2] = v[2] * ilength;
	}
	else
	{
#ifndef Q3_VM					// bk0101022 - FPE related
//    assert( ((Q_fabs(v[0])==0.0f) && (Q_fabs(v[1])==0.0f) && (Q_fabs(v[2])==0.0f)) );
#endif
		VectorClear(out);
	}

	return length;

}




int NearestPowerOfTwo(int val)
{
	int             answer;

	for(answer = 1; answer < val; answer <<= 1)
		;
	return answer;
}

int Q_log2(int val)
{
	int             answer;

	answer = 0;
	while((val >>= 1) != 0)
	{
		answer++;
	}
	return answer;
}



void AxisMultiply(axis_t in1, axis_t in2, axis_t out)
{
	out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] + in1[0][2] * in2[2][0];
	out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] + in1[0][2] * in2[2][1];
	out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] + in1[0][2] * in2[2][2];
	out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] + in1[1][2] * in2[2][0];
	out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] + in1[1][2] * in2[2][1];
	out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] + in1[1][2] * in2[2][2];
	out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] + in1[2][2] * in2[2][0];
	out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] + in1[2][2] * in2[2][1];
	out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] + in1[2][2] * in2[2][2];
}


void AngleVectors(const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up)
{
	float           angle;
	static float    sr, sp, sy, cr, cp, cy;

	// static to help MS compiler fp bugs

	angle = angles[YAW] * (M_PI * 2 / 360);
	sy = sin(angle);
	cy = cos(angle);
	angle = angles[PITCH] * (M_PI * 2 / 360);
	sp = sin(angle);
	cp = cos(angle);
	angle = angles[ROLL] * (M_PI * 2 / 360);
	sr = sin(angle);
	cr = cos(angle);

	if(forward)
	{
		forward[0] = cp * cy;
		forward[1] = cp * sy;
		forward[2] = -sp;
	}
	if(right)
	{
		right[0] = (-1 * sr * sp * cy + -1 * cr * -sy);
		right[1] = (-1 * sr * sp * sy + -1 * cr * cy);
		right[2] = -1 * sr * cp;
	}
	if(up)
	{
		up[0] = (cr * sp * cy + -sr * -sy);
		up[1] = (cr * sp * sy + -sr * cy);
		up[2] = cr * cp;
	}
}

/*
=================
PerpendicularVector

assumes "src" is normalized
=================
*/
void PerpendicularVector(vec3_t dst, const vec3_t src)
{
	int             pos;
	float           minelem;

	if(src[0])
	{
		dst[0] = 0;
		if(src[1])
		{
			dst[1] = 0;
			if(src[2])
			{
				dst[2] = 0;
				pos = 0;
				minelem = fabs(src[0]);
				if(Q_fabs(src[1]) < minelem)
				{
					pos = 1;
					minelem = fabs(src[1]);
				}

				if(Q_fabs(src[2]) < minelem)
					pos = 2;

				dst[pos] = 1;
				dst[0] -= src[pos] * src[0];
				dst[1] -= src[pos] * src[1];
				dst[2] -= src[pos] * src[2];

				VectorNormalize(dst);
			}
			else
			{
				dst[2] = 1;
			}
		}
		else
		{
			dst[1] = 1;
			dst[2] = 0;
		}
	}
	else
	{
		dst[0] = 1;
		dst[1] = 0;
		dst[2] = 0;
	}
}

// *INDENT-OFF*
void MatrixIdentity(matrix_t m)
{
    m[ 0] = 1;      m[ 4] = 0;      m[ 8] = 0;      m[12] = 0;
	m[ 1] = 0;      m[ 5] = 1;      m[ 9] = 0;      m[13] = 0;
	m[ 2] = 0;      m[ 6] = 0;      m[10] = 1;      m[14] = 0;
	m[ 3] = 0;      m[ 7] = 0;      m[11] = 0;      m[15] = 1;
}

void MatrixClear(matrix_t m)
{
    m[ 0] = 0;      m[ 4] = 0;      m[ 8] = 0;      m[12] = 0;
	m[ 1] = 0;      m[ 5] = 0;      m[ 9] = 0;      m[13] = 0;
	m[ 2] = 0;      m[ 6] = 0;      m[10] = 0;      m[14] = 0;
	m[ 3] = 0;      m[ 7] = 0;      m[11] = 0;      m[15] = 0;
}


void MatrixCopy(const matrix_t in, matrix_t out)
{
#if id386_sse && defined __GNUC__ && 0
	asm volatile
	(
	"movups         (%%edx),        %%xmm0\n"
	"movups         0x10(%%edx),    %%xmm1\n"
	"movups         0x20(%%edx),    %%xmm2\n"
	"movups         0x30(%%edx),    %%xmm3\n"

	"movups         %%xmm0,         (%%eax)\n"
	"movups         %%xmm1,         0x10(%%eax)\n"
	"movups         %%xmm2,         0x20(%%eax)\n"
	"movups         %%xmm3,         0x30(%%eax)\n"
	:
	: "a"( out ), "d"( in )
	: "memory"
		);
#elif id386_3dnow && defined __GNUC__
	asm volatile
	(
	"femms\n"
	"movq           (%%edx),        %%mm0\n"
	"movq           8(%%edx),       %%mm1\n"
	"movq           16(%%edx),      %%mm2\n"
	"movq           24(%%edx),      %%mm3\n"
	"movq           32(%%edx),      %%mm4\n"
	"movq           40(%%edx),      %%mm5\n"
	"movq           48(%%edx),      %%mm6\n"
	"movq           56(%%edx),      %%mm7\n"

	"movq           %%mm0,          (%%eax)\n"
	"movq           %%mm1,          8(%%eax)\n"
	"movq           %%mm2,          16(%%eax)\n"
	"movq           %%mm3,          24(%%eax)\n"
	"movq           %%mm4,          32(%%eax)\n"
	"movq           %%mm5,          40(%%eax)\n"
	"movq           %%mm6,          48(%%eax)\n"
	"movq           %%mm7,          56(%%eax)\n"
	"femms\n"
	:
	: "a"(out), "d"(in)
	: "memory"
	);
#else
        out[ 0] = in[ 0];       out[ 4] = in[ 4];       out[ 8] = in[ 8];       out[12] = in[12];
        out[ 1] = in[ 1];       out[ 5] = in[ 5];       out[ 9] = in[ 9];       out[13] = in[13];
		out[ 2] = in[ 2];       out[ 6] = in[ 6];       out[10] = in[10];       out[14] = in[14];
		out[ 3] = in[ 3];       out[ 7] = in[ 7];       out[11] = in[11];       out[15] = in[15];
#endif
}

void MatrixTransposeIntoXMM(const matrix_t m)
{
#if id386_sse && defined __GNUC__ && 0
        asm volatile
        (                                                                               // reg[0]                       | reg[1]                | reg[2]                | reg[3]
        // load transpose into XMM registers
        "movlps         (%%eax),        %%xmm4\n"               // m[0][0]                      | m[0][1]               | -                     | -
        "movhps         16(%%eax),      %%xmm4\n"               // m[0][0]                      | m[0][1]               | m[1][0]               | m[1][1]
        
        "movlps         32(%%eax),      %%xmm3\n"               // m[2][0]                      | m[2][1]               | -                     | -
        "movhps         48(%%eax),      %%xmm3\n"               // m[2][0]                      | m[2][1]               | m[3][0]               | m[3][1]
        
        "movups         %%xmm4,         %%xmm5\n"               // m[0][0]                      | m[0][1]               | m[1][0]               | m[1][1]
        
        // 0x88 = 10 00 | 10 00 <-> 00 10 | 00 10          xmm4[00]                       xmm4[10]                xmm3[00]                xmm3[10]
        "shufps         $0x88, %%xmm3,  %%xmm4\n"       // m[0][0]                      | m[1][0]               | m[2][0]               | m[3][0]
        
        // 0xDD = 11 01 | 11 01 <-> 01 11 | 01 11          xmm5[01]                       xmm5[11]                xmm3[01]                xmm3[11]
        "shufps         $0xDD, %%xmm3,  %%xmm5\n"       // m[0][1]                      | m[1][1]               | m[2][1]               | m[3][1]
        
        "movlps         8(%%eax),       %%xmm6\n"               // m[0][2]                      | m[0][3]               | -                     | -
        "movhps         24(%%eax),      %%xmm6\n"               // m[0][2]                      | m[0][3]               | m[1][2]               | m[1][3]
        
        "movlps         40(%%eax),      %%xmm3\n"               // m[2][2]                      | m[2][3]               | -                     | -
        "movhps         56(%%eax),      %%xmm3\n"               // m[2][2]                      | m[2][3]               | m[3][2]               | m[3][3]
        
        "movups         %%xmm6,         %%xmm7\n"               // m[0][2]                      | m[0][3]               | m[1][2]               | m[1][3]
        
        // 0x88 = 10 00 | 10 00 <-> 00 10 | 00 10          xmm6[00]                       xmm6[10]                xmm3[00]                xmm3[10]
        "shufps         $0x88, %%xmm3,  %%xmm6\n"       // m[0][2]                      | m[1][2]               | m[2][2]               | m[3][2]
        
        // 0xDD = 11 01 | 11 01 <-> 01 11 | 01 11          xmm7[01]                       xmm7[11]                xmm3[01]                xmm3[11]
        "shufps         $0xDD, %%xmm3,  %%xmm7\n"       // m[0][3]                      | m[1][3]               | m[2][3]               | m[3][3]
	:
	: "a"( m )
	: "memory"
		);
#endif
}

void MatrixTranspose(const matrix_t in, matrix_t out)
{
#if id386_sse && defined __GNUC__ && 0
        // transpose the matrix into the xmm4-7
        MatrixTransposeIntoXMM(in);
        
        asm volatile
				(
				"movups         %%xmm4,         (%%eax)\n"
				"movups         %%xmm5,         0x10(%%eax)\n"
				"movups         %%xmm6,         0x20(%%eax)\n"
				"movups         %%xmm7,         0x30(%%eax)\n"
	:
	: "a"( out )
	: "memory"
				);
#else
	out[ 0] = in[ 0];       out[ 1] = in[ 4];       out[ 2] = in[ 8];       out[ 3] = in[12];
	out[ 4] = in[ 1];       out[ 5] = in[ 5];       out[ 6] = in[ 9];       out[ 7] = in[13];
	out[ 8] = in[ 2];       out[ 9] = in[ 6];       out[10] = in[10];       out[11] = in[14];
	out[12] = in[ 3];       out[13] = in[ 7];       out[14] = in[11];       out[15] = in[15];
#endif
}


// helper functions for MatrixInverse from GtkRadiant C mathlib
static float m3_det( matrix3x3_t mat )
{
  float det;
  
  det = mat[0] * ( mat[4]*mat[8] - mat[7]*mat[5] )
    - mat[1] * ( mat[3]*mat[8] - mat[6]*mat[5] )
    + mat[2] * ( mat[3]*mat[7] - mat[6]*mat[4] );
  
  return( det );
}

static int m3_inverse( matrix3x3_t mr, matrix3x3_t ma )
{
  float det = m3_det( ma );
 
  if (det == 0 )
  {
    return 1;
  }

  
  mr[0] =    ma[4]*ma[8] - ma[5]*ma[7]   / det;
  mr[1] = -( ma[1]*ma[8] - ma[7]*ma[2] ) / det;
  mr[2] =    ma[1]*ma[5] - ma[4]*ma[2]   / det;
  
  mr[3] = -( ma[3]*ma[8] - ma[5]*ma[6] ) / det;
  mr[4] =    ma[0]*ma[8] - ma[6]*ma[2]   / det;
  mr[5] = -( ma[0]*ma[5] - ma[3]*ma[2] ) / det;
  
  mr[6] =    ma[3]*ma[7] - ma[6]*ma[4]   / det;
  mr[7] = -( ma[0]*ma[7] - ma[6]*ma[1] ) / det;
  mr[8] =    ma[0]*ma[4] - ma[1]*ma[3]   / det;

  return 0;
}

static void m4_submat( matrix_t mr, matrix3x3_t mb, int i, int j )
{
  int ti, tj, idst, jdst;
  
  for ( ti = 0; ti < 4; ti++ )
  {
    if ( ti < i )
      idst = ti;
    else
      if ( ti > i )
        idst = ti-1;
      
      for ( tj = 0; tj < 4; tj++ )
      {
        if ( tj < j )
          jdst = tj;
        else
          if ( tj > j )
            jdst = tj-1;
          
          if ( ti != i && tj != j )
            mb[idst*3 + jdst] = mr[ti*4 + tj ];
      }
  }
}

static float m4_det( matrix_t mr )
{
  float  det, result = 0, i = 1;
  matrix3x3_t msub3;
  int     n;
  
  for ( n = 0; n < 4; n++, i *= -1 )
  {
    m4_submat( mr, msub3, 0, n );
    
    det     = m3_det( msub3 );
    result += mr[n] * det * i;
  }
  
  return result;
}

qboolean MatrixInverse(matrix_t matrix)
{
  float  mdet = m4_det(matrix);
  matrix3x3_t mtemp;
  int     i, j, sign;
  matrix_t m4x4_temp;
  
#if 0
  if ( fabs( mdet ) < 0.0000000001 )
    return qtrue;
#endif

  MatrixCopy(matrix, m4x4_temp);
  
  for ( i = 0; i < 4; i++ )
    for ( j = 0; j < 4; j++ )
    {
      sign = 1 - ( (i +j) % 2 ) * 2;
      
      m4_submat( m4x4_temp, mtemp, i, j );
      
	  // FIXME: try using * inverse det and see if speed/accuracy are good enough
      matrix[i+j*4] = ( m3_det( mtemp ) * sign ) / mdet; 
    }
    
  return qfalse;
}

void MatrixSetupXRotation(matrix_t m, vec_t degrees)
{
	vec_t a = DEG2RAD(degrees);
	
	m[ 0] = 1;      m[ 4] = 0;              m[ 8] = 0;              m[12] = 0;
	m[ 1] = 0;      m[ 5] = cos(a);         m[ 9] =-sin(a);         m[13] = 0;
	m[ 2] = 0;      m[ 6] = sin(a);         m[10] = cos(a);         m[14] = 0;
	m[ 3] = 0;      m[ 7] = 0;              m[11] = 0;              m[15] = 1;
}

void MatrixSetupYRotation(matrix_t m, vec_t degrees)
{
	vec_t a = DEG2RAD(degrees);
	
	m[ 0] = cos(a);         m[ 4] = 0;      m[ 8] = sin(a);         m[12] = 0;
	m[ 1] = 0;              m[ 5] = 1;      m[ 9] = 0;              m[13] = 0;
	m[ 2] =-sin(a);         m[ 6] = 0;      m[10] = cos(a);         m[14] = 0;
	m[ 3] = 0;              m[ 7] = 0;      m[11] = 0;              m[15] = 1;
}

void MatrixSetupZRotation(matrix_t m, vec_t degrees)
{
	vec_t a = DEG2RAD(degrees);
	
	m[ 0] = cos(a);         m[ 4] =-sin(a);         m[ 8] = 0;      m[12] = 0;
	m[ 1] = sin(a);         m[ 5] = cos(a);         m[ 9] = 0;      m[13] = 0;
	m[ 2] = 0;              m[ 6] = 0;              m[10] = 1;      m[14] = 0;
	m[ 3] = 0;              m[ 7] = 0;              m[11] = 0;      m[15] = 1;
}

void MatrixSetupTranslation(matrix_t m, vec_t x, vec_t y, vec_t z)
{
	m[ 0] = 1;      m[ 4] = 0;      m[ 8] = 0;      m[12] = x;
	m[ 1] = 0;      m[ 5] = 1;      m[ 9] = 0;      m[13] = y;
	m[ 2] = 0;      m[ 6] = 0;      m[10] = 1;      m[14] = z;
	m[ 3] = 0;      m[ 7] = 0;      m[11] = 0;      m[15] = 1;
}

void MatrixSetupScale(matrix_t m, vec_t x, vec_t y, vec_t z)
{
	m[ 0] = x;      m[ 4] = 0;      m[ 8] = 0;      m[12] = 0;
	m[ 1] = 0;      m[ 5] = y;      m[ 9] = 0;      m[13] = 0;
	m[ 2] = 0;      m[ 6] = 0;      m[10] = z;      m[14] = 0;
	m[ 3] = 0;      m[ 7] = 0;      m[11] = 0;      m[15] = 1;
}

void MatrixSetupShear(matrix_t m, vec_t x, vec_t y)
{
	m[ 0] = 1;      m[ 4] = x;      m[ 8] = 0;      m[12] = 0;
	m[ 1] = y;      m[ 5] = 1;      m[ 9] = 0;      m[13] = 0;
	m[ 2] = 0;      m[ 6] = 0;      m[10] = 1;      m[14] = 0;
	m[ 3] = 0;      m[ 7] = 0;      m[11] = 0;      m[15] = 1;
}

void MatrixMultiply(const matrix_t a, const matrix_t b, matrix_t out)
{
#if id386_sse
//#error MatrixMultiply
	int				i;
	__m128			_t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7;

	_t4 = _mm_loadu_ps(&a[0]);
	_t5 = _mm_loadu_ps(&a[4]);
	_t6 = _mm_loadu_ps(&a[8]);
	_t7 = _mm_loadu_ps(&a[12]);

	for(i = 0; i < 4; i++)
	{
		_t0 = _mm_load1_ps(&b[i * 4 + 0]);
		_t0 = _mm_mul_ps(_t4, _t0);

		_t1 = _mm_load1_ps(&b[i * 4 + 1]);
		_t1 = _mm_mul_ps(_t5, _t1);
		
		_t2 = _mm_load1_ps(&b[i * 4 + 2]);
		_t2 = _mm_mul_ps(_t6, _t2);
		
		_t3 = _mm_load1_ps(&b[i * 4 + 3]);
		_t3 = _mm_mul_ps(_t7, _t3);
	
		_t1 = _mm_add_ps(_t0, _t1);
		_t2 = _mm_add_ps(_t1, _t2);
		_t3 = _mm_add_ps(_t2, _t3);

		_mm_storeu_ps(&out[i * 4], _t3);
	}
	
#else
        out[ 0] = b[ 0]*a[ 0] + b[ 1]*a[ 4] + b[ 2]*a[ 8] + b[ 3]*a[12];
        out[ 1] = b[ 0]*a[ 1] + b[ 1]*a[ 5] + b[ 2]*a[ 9] + b[ 3]*a[13];
		out[ 2] = b[ 0]*a[ 2] + b[ 1]*a[ 6] + b[ 2]*a[10] + b[ 3]*a[14];
		out[ 3] = b[ 0]*a[ 3] + b[ 1]*a[ 7] + b[ 2]*a[11] + b[ 3]*a[15];
        
		out[ 4] = b[ 4]*a[ 0] + b[ 5]*a[ 4] + b[ 6]*a[ 8] + b[ 7]*a[12];
		out[ 5] = b[ 4]*a[ 1] + b[ 5]*a[ 5] + b[ 6]*a[ 9] + b[ 7]*a[13];
		out[ 6] = b[ 4]*a[ 2] + b[ 5]*a[ 6] + b[ 6]*a[10] + b[ 7]*a[14];
		out[ 7] = b[ 4]*a[ 3] + b[ 5]*a[ 7] + b[ 6]*a[11] + b[ 7]*a[15];
        
		out[ 8] = b[ 8]*a[ 0] + b[ 9]*a[ 4] + b[10]*a[ 8] + b[11]*a[12];
		out[ 9] = b[ 8]*a[ 1] + b[ 9]*a[ 5] + b[10]*a[ 9] + b[11]*a[13];
		out[10] = b[ 8]*a[ 2] + b[ 9]*a[ 6] + b[10]*a[10] + b[11]*a[14];
		out[11] = b[ 8]*a[ 3] + b[ 9]*a[ 7] + b[10]*a[11] + b[11]*a[15];
        
		out[12] = b[12]*a[ 0] + b[13]*a[ 4] + b[14]*a[ 8] + b[15]*a[12];
		out[13] = b[12]*a[ 1] + b[13]*a[ 5] + b[14]*a[ 9] + b[15]*a[13];
		out[14] = b[12]*a[ 2] + b[13]*a[ 6] + b[14]*a[10] + b[15]*a[14];
		out[15] = b[12]*a[ 3] + b[13]*a[ 7] + b[14]*a[11] + b[15]*a[15];
#endif
}

void MatrixMultiply2(matrix_t m, const matrix_t m2)
{
	matrix_t        tmp;

	MatrixCopy(m, tmp);
	MatrixMultiply(tmp, m2, m);
}

void MatrixMultiplyRotation(matrix_t m, vec_t pitch, vec_t yaw, vec_t roll)
{
	matrix_t        tmp, rot;

	MatrixCopy(m, tmp);
	MatrixFromAngles(rot, pitch, yaw, roll);

	MatrixMultiply(tmp, rot, m);
}

void MatrixMultiplyZRotation(matrix_t m, vec_t degrees)
{
	matrix_t        tmp, rot;

	MatrixCopy(m, tmp);
	MatrixSetupZRotation(rot, degrees);

	MatrixMultiply(tmp, rot, m);
}

void MatrixMultiplyTranslation(matrix_t m, vec_t x, vec_t y, vec_t z)
{
#if 1
	matrix_t        tmp, trans;

	MatrixCopy(m, tmp);
	MatrixSetupTranslation(trans, x, y, z);
	MatrixMultiply(tmp, trans, m);
#else
	m[12] += m[ 0] * x + m[ 4] * y + m[ 8] * z;
	m[13] += m[ 1] * x + m[ 5] * y + m[ 9] * z;
	m[14] += m[ 2] * x + m[ 6] * y + m[10] * z;
	m[15] += m[ 3] * x + m[ 7] * y + m[11] * z;
#endif
}

void MatrixMultiplyScale(matrix_t m, vec_t x, vec_t y, vec_t z)
{
#if 0
	matrix_t        tmp, scale;

	MatrixCopy(m, tmp);
	MatrixSetupScale(scale, x, y, z);
	MatrixMultiply(tmp, scale, m);
#else
	m[ 0] *= x;     m[ 4] *= y;        m[ 8] *= z;
	m[ 1] *= x;     m[ 5] *= y;        m[ 9] *= z;
	m[ 2] *= x;     m[ 6] *= y;        m[10] *= z;
	m[ 3] *= x;     m[ 7] *= y;        m[11] *= z;
#endif
}

void MatrixMultiplyShear(matrix_t m, vec_t x, vec_t y)
{
	matrix_t        tmp, shear;

	MatrixCopy(m, tmp);
	MatrixSetupShear(shear, x, y);
	MatrixMultiply(tmp, shear, m);
}

void MatrixToAngles(const matrix_t m, vec3_t angles)
{
#if 1
	double          theta;
	double          cp;
	double          sp;

	sp = m[2];

	// cap off our sin value so that we don't get any NANs
	if(sp > 1.0)
	{
		sp = 1.0;
	}
	else if(sp < -1.0)
	{
		sp = -1.0;
	}

	theta = -asin(sp);
	cp = cos(theta);

	if(cp > 8192 * FLT_EPSILON)
	{
		angles[PITCH] = RAD2DEG(theta);
		angles[YAW] = RAD2DEG(atan2(m[1], m[0]));
		angles[ROLL] = RAD2DEG(atan2(m[6], m[10]));
	}
	else
	{
		angles[PITCH] = RAD2DEG(theta);
		angles[YAW] = RAD2DEG(-atan2(m[4], m[5]));
		angles[ROLL] = 0;
	}
#else
	double          a;
	double          ca;
	
	a = asin(-m[2]);
	ca = cos(a);

	if(fabs(ca) > 0.005)		// Gimbal lock?
	{
		angles[PITCH] = RAD2DEG(atan2(m[6] / ca, m[10] / ca));
		angles[YAW] = RAD2DEG(a);
		angles[ROLL] = RAD2DEG(atan2(m[1] / ca, m[0] / ca));
	}
	else
	{
		// Gimbal lock has occurred
		angles[PITCH] = RAD2DEG(atan2(-m[9], m[5]));
		angles[YAW] = RAD2DEG(a);
		angles[ROLL] = 0;
	}
#endif
}


void MatrixFromAngles(matrix_t m, vec_t pitch, vec_t yaw, vec_t roll)
{
	static float    sr, sp, sy, cr, cp, cy;

    // static to help MS compiler fp bugs
	sp = sin(DEG2RAD(pitch));
	cp = cos(DEG2RAD(pitch));

	sy = sin(DEG2RAD(yaw));
	cy = cos(DEG2RAD(yaw));

	sr = sin(DEG2RAD(roll));
	cr = cos(DEG2RAD(roll));
	
	m[ 0] = cp*cy;  m[ 4] = (sr*sp*cy+cr*-sy);      m[ 8] = (cr*sp*cy+-sr*-sy);     m[12] = 0;
	m[ 1] = cp*sy;  m[ 5] = (sr*sp*sy+cr*cy);       m[ 9] = (cr*sp*sy+-sr*cy);      m[13] = 0;
	m[ 2] = -sp;    m[ 6] = sr*cp;                  m[10] = cr*cp;                  m[14] = 0;
	m[ 3] = 0;      m[ 7] = 0;                      m[11] = 0;                      m[15] = 1;
}

void MatrixFromVectorsFLU(matrix_t m, const vec3_t forward, const vec3_t left, const vec3_t up)
{
    m[ 0] = forward[0];     m[ 4] = left[0];        m[ 8] = up[0];  m[12] = 0;
	m[ 1] = forward[1];     m[ 5] = left[1];        m[ 9] = up[1];  m[13] = 0;
	m[ 2] = forward[2];     m[ 6] = left[2];        m[10] = up[2];  m[14] = 0;
	m[ 3] = 0;              m[ 7] = 0;              m[11] = 0;      m[15] = 1;
}

void MatrixFromVectorsFRU(matrix_t m, const vec3_t forward, const vec3_t right, const vec3_t up)
{
    m[ 0] = forward[0];     m[ 4] =-right[0];       m[ 8] = up[0];  m[12] = 0;
	m[ 1] = forward[1];     m[ 5] =-right[1];       m[ 9] = up[1];  m[13] = 0;
	m[ 2] = forward[2];     m[ 6] =-right[2];       m[10] = up[2];  m[14] = 0;
	m[ 3] = 0;              m[ 7] = 0;              m[11] = 0;      m[15] = 1;
}

void MatrixFromQuat(matrix_t m, const quat_t q)
{	
#if 1
	/*
	From Quaternion to Matrix and Back
	February 27th 2005
	J.M.P. van Waveren
	
	http://www.intel.com/cd/ids/developer/asmo-na/eng/293748.htm
	*/
	float			x2, y2, z2, w2;
	float			yy2, xy2;
	float			xz2, yz2, zz2;
	float			wz2, wy2, wx2, xx2;
	
	x2 = q[0] + q[0];
	y2 = q[1] + q[1];
	z2 = q[2] + q[2];
	w2 = q[3] + q[3];
	
	yy2 = q[1] * y2;
	xy2 = q[0] * y2;
	
	xz2 = q[0] * z2;
	yz2 = q[1] * z2;
	zz2 = q[2] * z2;
	
	wz2 = q[3] * z2;
	wy2 = q[3] * y2;
	wx2 = q[3] * x2;
	xx2 = q[0] * x2;

	m[ 0] = - yy2 - zz2 + 1.0f;
	m[ 1] =   xy2 + wz2;
	m[ 2] =   xz2 - wy2;
	
	m[ 4] =   xy2 - wz2;
	m[ 5] = - xx2 - zz2 + 1.0f;
	m[ 6] =   yz2 + wx2;
	
	m[ 8] =   xz2 + wy2;
	m[ 9] =   yz2 - wx2;
	m[10] = - xx2 - yy2 + 1.0f;
	
	m[ 3] = m[ 7] = m[11] = m[12] = m[13] = m[14] = 0;
    m[15] = 1;

#else
	/*
	http://www.gamedev.net/reference/articles/article1691.asp#Q54
	Q54. How do I convert a quaternion to a rotation matrix?
	
	Assuming that a quaternion has been created in the form:

    Q = |X Y Z W|
    
    Then the quaternion can then be converted into a 4x4 rotation
    matrix using the following expression (Warning: you might have to
    transpose this matrix if you (do not) follow the OpenGL order!):

         ?        2     2                                      ?
         ? 1 - (2Y  + 2Z )   2XY - 2ZW         2XZ + 2YW       ?
         ?                                                     ?
         ?                          2     2                    ?
     M = ? 2XY + 2ZW         1 - (2X  + 2Z )   2YZ - 2XW       ?
         ?                                                     ?
         ?                                            2     2  ?
         ? 2XZ - 2YW         2YZ + 2XW         1 - (2X  + 2Y ) ?
         ?                                                     ?
	*/
	
	// http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm

	float			xx, xy, xz, xw, yy, yz, yw, zz, zw;

	xx = q[0] * q[0];
    xy = q[0] * q[1];
    xz = q[0] * q[2];
    xw = q[0] * q[3];
    yy = q[1] * q[1];
    yz = q[1] * q[2];
    yw = q[1] * q[3];
    zz = q[2] * q[2];
    zw = q[2] * q[3];
    
    m[ 0] = 1 - 2 * ( yy + zz );
    m[ 1] =     2 * ( xy + zw );
    m[ 2] =     2 * ( xz - yw );
    m[ 4] =     2 * ( xy - zw );
    m[ 5] = 1 - 2 * ( xx + zz );
    m[ 6] =     2 * ( yz + xw );
    m[ 8] =     2 * ( xz + yw );
    m[ 9] =     2 * ( yz - xw );
    m[10] = 1 - 2 * ( xx + yy );
   
    m[ 3] = m[ 7] = m[11] = m[12] = m[13] = m[14] = 0;
    m[15] = 1;
#endif
}

void MatrixFromPlanes(matrix_t m, const vec4_t left, const vec4_t right, const vec4_t bottom, const vec4_t top, const vec4_t front, const vec4_t back)
{
	m[ 0] = (right[0] - left[0]) / 2;
	m[ 1] = (top[0] - bottom[0]) / 2;
	m[ 2] = (back[0] - front[0]) / 2;
	m[ 3] = right[0] - (right[0] - left[0]) / 2;
	m[ 4] = (right[1] - left[1]) / 2;
	m[ 5] = (top[1] - bottom[1]) / 2;
	m[ 6] = (back[1] - front[1]) / 2;
	m[ 7] = right[1] - (right[1] - left[1]) / 2;
	m[ 8] = (right[1] - left[1]) / 2;
	m[ 9] = (top[1] - bottom[1]) / 2;
	m[10] = (back[1] - front[1]) / 2;
	m[11] = right[1] - (right[1] - left[1]) / 2;
	m[12] = (right[1] - left[1]) / 2;
	m[13] = (top[1] - bottom[1]) / 2;
	m[14] = (back[1] - front[1]) / 2;
	m[15] = right[1] - (right[1] - left[1]) / 2;
}

void MatrixToVectorsFLU(const matrix_t m, vec3_t forward, vec3_t left, vec3_t up)
{
	if(forward)
	{
		forward[0] = m[ 0];     // cp*cy;
		forward[1] = m[ 1];     // cp*sy;
		forward[2] = m[ 2];     //-sp;
	}
     
    if(left)
    {   
		left[0] = m[ 4];        // sr*sp*cy+cr*-sy;
		left[1] = m[ 5];        // sr*sp*sy+cr*cy;
		left[2] = m[ 6];        // sr*cp;
    }
	
	if(up)
	{
		up[0] = m[ 8];  // cr*sp*cy+-sr*-sy;
		up[1] = m[ 9];  // cr*sp*sy+-sr*cy;
		up[2] = m[10];  // cr*cp;
	}
}

void MatrixToVectorsFRU(const matrix_t m, vec3_t forward, vec3_t right, vec3_t up)
{
	if(forward)
	{
		forward[0] = m[ 0];
		forward[1] = m[ 1];
		forward[2] = m[ 2];
	}
	
	if(right)
	{   
		right[0] =-m[ 4];
		right[1] =-m[ 5];
		right[2] =-m[ 6];
	}

	if(up)
	{
		up[0] = m[ 8];
		up[1] = m[ 9];
		up[2] = m[10];
	}
}

void MatrixSetupTransform(matrix_t m, const vec3_t forward, const vec3_t left, const vec3_t up, const vec3_t origin)
{
	m[ 0] = forward[0];     m[ 4] = left[0];        m[ 8] = up[0];  m[12] = origin[0];
	m[ 1] = forward[1];     m[ 5] = left[1];        m[ 9] = up[1];  m[13] = origin[1];
	m[ 2] = forward[2];     m[ 6] = left[2];        m[10] = up[2];  m[14] = origin[2];
	m[ 3] = 0;              m[ 7] = 0;              m[11] = 0;      m[15] = 1;
}

void MatrixSetupTransformFromRotation(matrix_t m, const matrix_t rot, const vec3_t origin)
{
	m[ 0] = rot[ 0];     m[ 4] = rot[ 4];        m[ 8] = rot[ 8];  m[12] = origin[0];
	m[ 1] = rot[ 1];     m[ 5] = rot[ 5];        m[ 9] = rot[ 9];  m[13] = origin[1];
	m[ 2] = rot[ 2];     m[ 6] = rot[ 6];        m[10] = rot[10];  m[14] = origin[2];
	m[ 3] = 0;           m[ 7] = 0;              m[11] = 0;        m[15] = 1;
}

void MatrixSetupTransformFromQuat(matrix_t m, const quat_t quat, const vec3_t origin)
{
	matrix_t        rot;
	
	MatrixFromQuat(rot, quat);
	
	m[ 0] = rot[ 0];     m[ 4] = rot[ 4];        m[ 8] = rot[ 8];  m[12] = origin[0];
	m[ 1] = rot[ 1];     m[ 5] = rot[ 5];        m[ 9] = rot[ 9];  m[13] = origin[1];
	m[ 2] = rot[ 2];     m[ 6] = rot[ 6];        m[10] = rot[10];  m[14] = origin[2];
	m[ 3] = 0;           m[ 7] = 0;              m[11] = 0;        m[15] = 1;
}

void MatrixAffineInverse(const matrix_t in, matrix_t out)
{
#if 0 
		MatrixCopy(in, out);
		MatrixInverse(out);
#else
        // Tr3B - cleaned up
        out[ 0] = in[ 0];       out[ 4] = in[ 1];       out[ 8] = in[ 2];
        out[ 1] = in[ 4];       out[ 5] = in[ 5];       out[ 9] = in[ 6];
		out[ 2] = in[ 8];       out[ 6] = in[ 9];       out[10] = in[10];
		out[ 3] = 0;            out[ 7] = 0;            out[11] = 0;            out[15] = 1;
        
		out[12] = -( in[12] * out[ 0] + in[13] * out[ 4] + in[14] * out[ 8] );
		out[13] = -( in[12] * out[ 1] + in[13] * out[ 5] + in[14] * out[ 9] );
		out[14] = -( in[12] * out[ 2] + in[13] * out[ 6] + in[14] * out[10] );
#endif
}

void MatrixTransformNormal(const matrix_t m, const vec3_t in, vec3_t out)
{
	out[ 0] = m[ 0] * in[ 0] + m[ 4] * in[ 1] + m[ 8] * in[ 2];
	out[ 1] = m[ 1] * in[ 0] + m[ 5] * in[ 1] + m[ 9] * in[ 2];
	out[ 2] = m[ 2] * in[ 0] + m[ 6] * in[ 1] + m[10] * in[ 2];
}

void MatrixTransformPoint(const matrix_t m, const vec3_t in, vec3_t out)
{
	out[ 0] = m[ 0] * in[ 0] + m[ 4] * in[ 1] + m[ 8] * in[ 2] + m[12];
	out[ 1] = m[ 1] * in[ 0] + m[ 5] * in[ 1] + m[ 9] * in[ 2] + m[13];
	out[ 2] = m[ 2] * in[ 0] + m[ 6] * in[ 1] + m[10] * in[ 2] + m[14];
}

void MatrixTransform4(const matrix_t m, const vec4_t in, vec4_t out)
{
#if id386_sse
//#error MatrixTransform4

	__m128			_t0, _t1, _t2, _x, _y, _z, _w, _m0, _m1, _m2, _m3;

	_m0 = _mm_loadu_ps(&m[0]);
	_m1 = _mm_loadu_ps(&m[4]);
	_m2 = _mm_loadu_ps(&m[8]);
	_m3 = _mm_loadu_ps(&m[12]);

	_t0 = _mm_loadu_ps(in);
	_x = _mm_shuffle_ps(_t0, _t0, _MM_SHUFFLE(0, 0, 0, 0));
	_y = _mm_shuffle_ps(_t0, _t0, _MM_SHUFFLE(1, 1, 1, 1));
	_z = _mm_shuffle_ps(_t0, _t0, _MM_SHUFFLE(2, 2, 2, 2));
	_w = _mm_shuffle_ps(_t0, _t0, _MM_SHUFFLE(3, 3, 3, 3));

	_t0 = _mm_mul_ps(_m3, _w);
	_t1 = _mm_mul_ps(_m2, _z);
	_t0 = _mm_add_ps(_t0, _t1);

	_t1 = _mm_mul_ps(_m1, _y);
	_t2 = _mm_mul_ps(_m0, _x);
	_t1 = _mm_add_ps(_t1, _t2);

	_t0 = _mm_add_ps(_t0, _t1);

	_mm_storeu_ps(out, _t0);
#else
	out[ 0] = m[ 0] * in[ 0] + m[ 4] * in[ 1] + m[ 8] * in[ 2] + m[12] * in[ 3];
	out[ 1] = m[ 1] * in[ 0] + m[ 5] * in[ 1] + m[ 9] * in[ 2] + m[13] * in[ 3];
	out[ 2] = m[ 2] * in[ 0] + m[ 6] * in[ 1] + m[10] * in[ 2] + m[14] * in[ 3];
	out[ 3] = m[ 3] * in[ 0] + m[ 7] * in[ 1] + m[11] * in[ 2] + m[15] * in[ 3];
#endif
}

// *INDENT-ON*


vec_t QuatNormalize(quat_t q)
{
	float           length, ilength;

	length = q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];
	length = sqrt(length);

	if(length)
	{
		ilength = 1 / length;
		q[0] *= ilength;
		q[1] *= ilength;
		q[2] *= ilength;
		q[3] *= ilength;
	}

	return length;
}

void QuatFromAngles(quat_t q, vec_t pitch, vec_t yaw, vec_t roll)
{
#if 1
	matrix_t        tmp;

	MatrixFromAngles(tmp, pitch, yaw, roll);
	QuatFromMatrix(q, tmp);
#else
	static float    sr, sp, sy, cr, cp, cy;

	// static to help MS compiler fp bugs
	sp = sin(DEG2RAD(pitch));
	cp = cos(DEG2RAD(pitch));

	sy = sin(DEG2RAD(yaw));
	cy = cos(DEG2RAD(yaw));

	sr = sin(DEG2RAD(roll));
	cr = cos(DEG2RAD(roll));

	q[0] = sr * cp * cy - cr * sp * sy;	// x
	q[1] = cr * sp * cy + sr * cp * sy;	// y
	q[2] = cr * cp * sy - sr * sp * cy;	// z
	q[3] = cr * cp * cy + sr * sp * sy;	// w
#endif
}

void QuatFromMatrix(quat_t q, const matrix_t m)
{
#if 1
	/*
	   From Quaternion to Matrix and Back
	   February 27th 2005
	   J.M.P. van Waveren

	   http://www.intel.com/cd/ids/developer/asmo-na/eng/293748.htm
	 */
	float           t, s;

	if(m[0] + m[5] + m[10] > 0.0f)
	{
		t = m[0] + m[5] + m[10] + 1.0f;
		s = 0.5f / sqrt(t);

		q[3] = s * t;
		q[2] = (m[1] - m[4]) * s;
		q[1] = (m[8] - m[2]) * s;
		q[0] = (m[6] - m[9]) * s;
	}
	else if(m[0] > m[5] && m[0] > m[10])
	{
		t = m[0] - m[5] - m[10] + 1.0f;
		s = 0.5f / sqrt(t);

		q[0] = s * t;
		q[1] = (m[1] + m[4]) * s;
		q[2] = (m[8] + m[2]) * s;
		q[3] = (m[6] - m[9]) * s;
	}
	else if(m[5] > m[10])
	{
		t = -m[0] + m[5] - m[10] + 1.0f;
		s = 0.5f / sqrt(t);

		q[1] = s * t;
		q[0] = (m[1] + m[4]) * s;
		q[3] = (m[8] - m[2]) * s;
		q[2] = (m[6] + m[9]) * s;
	}
	else
	{
		t = -m[0] - m[5] + m[10] + 1.0f;
		s = 0.5f / sqrt(t);

		q[2] = s * t;
		q[3] = (m[1] - m[4]) * s;
		q[0] = (m[8] + m[2]) * s;
		q[1] = (m[6] + m[9]) * s;
	}

#else
	float           trace;

	// http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm

	trace = 1.0f + m[0] + m[5] + m[10];

	if(trace > 0.0f)
	{
		vec_t           s = 0.5f / sqrt(trace);

		q[0] = (m[6] - m[9]) * s;
		q[1] = (m[8] - m[2]) * s;
		q[2] = (m[1] - m[4]) * s;
		q[3] = 0.25f / s;
	}
	else
	{
		if(m[0] > m[5] && m[0] > m[10])
		{
			// column 0
			float           s = sqrt(1.0f + m[0] - m[5] - m[10]) * 2.0f;

			q[0] = 0.25f * s;
			q[1] = (m[4] + m[1]) / s;
			q[2] = (m[8] + m[2]) / s;
			q[3] = (m[9] - m[6]) / s;
		}
		else if(m[5] > m[10])
		{
			// column 1
			float           s = sqrt(1.0f + m[5] - m[0] - m[10]) * 2.0f;

			q[0] = (m[4] + m[1]) / s;
			q[1] = 0.25f * s;
			q[2] = (m[9] + m[6]) / s;
			q[3] = (m[8] - m[2]) / s;
		}
		else
		{
			// column 2
			float           s = sqrt(1.0f + m[10] - m[0] - m[5]) * 2.0f;

			q[0] = (m[8] + m[2]) / s;
			q[1] = (m[9] + m[6]) / s;
			q[2] = 0.25f * s;
			q[3] = (m[4] - m[1]) / s;
		}
	}

	QuatNormalize(q);
#endif
}

void QuatToVectorsFLU(const quat_t q, vec3_t forward, vec3_t left, vec3_t up)
{
	matrix_t        tmp;

	MatrixFromQuat(tmp, q);
	MatrixToVectorsFRU(tmp, forward, left, up);
}

void QuatToVectorsFRU(const quat_t q, vec3_t forward, vec3_t right, vec3_t up)
{
	matrix_t        tmp;

	MatrixFromQuat(tmp, q);
	MatrixToVectorsFRU(tmp, forward, right, up);
}

void QuatToAxis(const quat_t q, vec3_t axis[3])
{
	matrix_t        tmp;

	MatrixFromQuat(tmp, q);
	MatrixToVectorsFLU(tmp, axis[0], axis[1], axis[2]);
}

void QuatToAngles(const quat_t q, vec3_t angles)
{
	quat_t          q2;

	q2[0] = q[0] * q[0];
	q2[1] = q[1] * q[1];
	q2[2] = q[2] * q[2];
	q2[3] = q[3] * q[3];

	angles[PITCH] = RAD2DEG(asin(-2 * (q[2] * q[0] - q[3] * q[1])));
	angles[YAW] = RAD2DEG(atan2(2 * (q[2] * q[3] + q[0] * q[1]), (q2[2] - q2[3] - q2[0] + q2[1])));
	angles[ROLL] = RAD2DEG(atan2(2 * (q[3] * q[0] + q[2] * q[1]), (-q2[2] - q2[3] + q2[0] + q2[1])));
}


void QuatMultiply0(quat_t qa, const quat_t qb)
{
	quat_t          tmp;

	QuatCopy(qa, tmp);
	QuatMultiply1(tmp, qb, qa);
}

void QuatMultiply1(const quat_t qa, const quat_t qb, quat_t qc)
{
	/*
	   from matrix and quaternion faq
	   x = w1x2 + x1w2 + y1z2 - z1y2
	   y = w1y2 + y1w2 + z1x2 - x1z2
	   z = w1z2 + z1w2 + x1y2 - y1x2

	   w = w1w2 - x1x2 - y1y2 - z1z2
	 */

	qc[0] = qa[3] * qb[0] + qa[0] * qb[3] + qa[1] * qb[2] - qa[2] * qb[1];
	qc[1] = qa[3] * qb[1] + qa[1] * qb[3] + qa[2] * qb[0] - qa[0] * qb[2];
	qc[2] = qa[3] * qb[2] + qa[2] * qb[3] + qa[0] * qb[1] - qa[1] * qb[0];
	qc[3] = qa[3] * qb[3] - qa[0] * qb[0] - qa[1] * qb[1] - qa[2] * qb[2];
}

void QuatMultiply2(const quat_t qa, const quat_t qb, quat_t qc)
{
	qc[0] = qa[3] * qb[0] + qa[0] * qb[3] + qa[1] * qb[2] + qa[2] * qb[1];
	qc[1] = qa[3] * qb[1] - qa[1] * qb[3] - qa[2] * qb[0] + qa[0] * qb[2];
	qc[2] = qa[3] * qb[2] - qa[2] * qb[3] - qa[0] * qb[1] + qa[1] * qb[0];
	qc[3] = qa[3] * qb[3] - qa[0] * qb[0] - qa[1] * qb[1] + qa[2] * qb[2];
}

void QuatMultiply3(const quat_t qa, const quat_t qb, quat_t qc)
{
	qc[0] = qa[3] * qb[0] + qa[0] * qb[3] + qa[1] * qb[2] + qa[2] * qb[1];
	qc[1] = -qa[3] * qb[1] + qa[1] * qb[3] - qa[2] * qb[0] + qa[0] * qb[2];
	qc[2] = -qa[3] * qb[2] + qa[2] * qb[3] - qa[0] * qb[1] + qa[1] * qb[0];
	qc[3] = -qa[3] * qb[3] + qa[0] * qb[0] - qa[1] * qb[1] + qa[2] * qb[2];
}

void QuatMultiply4(const quat_t qa, const quat_t qb, quat_t qc)
{
	qc[0] = qa[3] * qb[0] - qa[0] * qb[3] - qa[1] * qb[2] - qa[2] * qb[1];
	qc[1] = -qa[3] * qb[1] - qa[1] * qb[3] + qa[2] * qb[0] - qa[0] * qb[2];
	qc[2] = -qa[3] * qb[2] - qa[2] * qb[3] + qa[0] * qb[1] - qa[1] * qb[0];
	qc[3] = -qa[3] * qb[3] - qa[0] * qb[0] + qa[1] * qb[1] - qa[2] * qb[2];
}

void QuatSlerp(const quat_t from, const quat_t to, float frac, quat_t out)
{
#if 0
	quat_t          to1;
	double          omega, cosom, sinom, scale0, scale1;

	cosom = from[0] * to[0] + from[1] * to[1] + from[2] * to[2] + from[3] * to[3];

	if(cosom < 0.0)
	{
		cosom = -cosom;

		QuatCopy(to, to1);
		QuatAntipodal(to1);
	}
	else
	{
		QuatCopy(to, to1);
	}

	if((1.0 - cosom) > 0)
	{
		omega = acos(cosom);
		sinom = sin(omega);
		scale0 = sin((1.0 - frac) * omega) / sinom;
		scale1 = sin(frac * omega) / sinom;
	}
	else
	{
		scale0 = 1.0 - frac;
		scale1 = frac;
	}

	out[0] = scale0 * from[0] + scale1 * to1[0];
	out[1] = scale0 * from[1] + scale1 * to1[1];
	out[2] = scale0 * from[2] + scale1 * to1[2];
	out[3] = scale0 * from[3] + scale1 * to1[3];
#else
	/*
	   Slerping Clock Cycles
	   February 27th 2005
	   J.M.P. van Waveren

	   http://www.intel.com/cd/ids/developer/asmo-na/eng/293747.htm
	 */
	float           cosom, absCosom, sinom, sinSqr, omega, scale0, scale1;

	if(frac <= 0.0f)
	{
		QuatCopy(from, out);
		return;
	}

	if(frac >= 1.0f)
	{
		QuatCopy(to, out);
		return;
	}

	if(QuatCompare(from, to))
	{
		QuatCopy(from, out);
		return;
	}

	cosom = from[0] * to[0] + from[1] * to[1] + from[2] * to[2] + from[3] * to[3];
	absCosom = fabs(cosom);

	if((1.0f - absCosom) > 1e-6f)
	{
		sinSqr = 1.0f - absCosom * absCosom;
		sinom = 1.0f / sqrt(sinSqr);
		omega = atan2(sinSqr * sinom, absCosom);

		scale0 = sin((1.0f - frac) * omega) * sinom;
		scale1 = sin(frac * omega) * sinom;
	}
	else
	{
		scale0 = 1.0f - frac;
		scale1 = frac;
	}

	scale1 = (cosom >= 0.0f) ? scale1 : -scale1;

	out[0] = scale0 * from[0] + scale1 * to[0];
	out[1] = scale0 * from[1] + scale1 * to[1];
	out[2] = scale0 * from[2] + scale1 * to[2];
	out[3] = scale0 * from[3] + scale1 * to[3];
#endif
}

void QuatTransformVector(const quat_t q, const vec3_t in, vec3_t out)
{
	matrix_t        m;

	MatrixFromQuat(m, q);
	MatrixTransformNormal(m, in, out);
}
