/// ============================================================================
/*
Copyright (C) 2005 Robert Beckebans <trebor_7@users.sourceforge.net>
Please see the file "AUTHORS" for a list of contributors

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
/// ============================================================================

uniform sampler2D	u_ColorMap;
uniform vec2		u_FBufScale;
uniform vec2		u_NPotScale;
uniform float		u_BlurMagnitude;

varying vec2		var_TexGlow;

void	main()
{
//	vec2 st00 = var_TexGlow;
	
	vec2 st00 = gl_FragCoord.st;

	// calculate the screen texcoord in the 0.0 to 1.0 range
	st00 *= u_FBufScale;
	
	// scale by the screen non-power-of-two-adjust
	st00 *= u_NPotScale;
	
	// set so a magnitude of 1 is approximately 1 pixel with 640x480
	vec2 deform = vec2(u_BlurMagnitude * 0.0016, u_BlurMagnitude * 0.00213333);
	
	// fragment offsets for blur samples
	vec2 offset01 = vec2( 0.0, -1.0);
	vec2 offset02 = vec2(-1.0,  0.0);
	vec2 offset03 = vec2( 1.0,  0.0);
	vec2 offset04 = vec2( 0.0,  1.0);
	vec2 offset05 = vec2(-2.0, -2.0);
	vec2 offset06 = vec2( 2.0, -2.0);
	vec2 offset07 = vec2(-2.0,  2.0);
	vec2 offset08 = vec2( 2.0,  2.0);
	
	// calculate our offset texture coordinates
	vec2 st01 = st00 + offset01 * deform;
	vec2 st02 = st00 + offset02 * deform;
	vec2 st03 = st00 + offset03 * deform;
	vec2 st04 = st00 + offset04 * deform;
	vec2 st05 = st00 + offset05 * deform;
	vec2 st06 = st00 + offset06 * deform;
	vec2 st07 = st00 + offset07 * deform;
	vec2 st08 = st00 + offset08 * deform;
	
	// base color
	vec4 c00 = texture2D(u_ColorMap, st00);

	// sample the current render for each coordinate
	vec4 c01 = texture2D(u_ColorMap, st01);
	vec4 c02 = texture2D(u_ColorMap, st02);
	vec4 c03 = texture2D(u_ColorMap, st03);
	vec4 c04 = texture2D(u_ColorMap, st04);
	vec4 c05 = texture2D(u_ColorMap, st05);
	vec4 c06 = texture2D(u_ColorMap, st06);
	vec4 c07 = texture2D(u_ColorMap, st07);
	vec4 c08 = texture2D(u_ColorMap, st08);
	
	vec4 sum = c01 + c02 + c03 + c04 + c05 + c06 + c07 + c08;
	sum *= 1.0 / 8.0;
	
	gl_FragColor = c00 + sum;
}
