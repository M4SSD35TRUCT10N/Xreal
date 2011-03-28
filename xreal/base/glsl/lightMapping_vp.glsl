/*
===========================================================================
Copyright (C) 2006-2011 Robert Beckebans <trebor_7@users.sourceforge.net>

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

/* lightMapping_vp.glsl */

attribute vec4		attr_Position;
attribute vec4		attr_TexCoord0;
attribute vec4		attr_TexCoord1;
attribute vec3		attr_Tangent;
attribute vec3		attr_Binormal;
attribute vec3		attr_Normal;

uniform mat4		u_DiffuseTextureMatrix;
uniform mat4		u_NormalTextureMatrix;
uniform mat4		u_SpecularTextureMatrix;
uniform mat4		u_ModelMatrix;
uniform mat4		u_ModelViewProjectionMatrix;

uniform int			u_DeformGen;
uniform vec4		u_DeformWave;	// [base amplitude phase freq]
uniform vec3		u_DeformBulge;	// [width height speed]
uniform float		u_DeformSpread;
uniform float		u_Time;

varying vec3		var_Position;
varying vec2		var_TexDiffuse;
varying vec2		var_TexNormal;
varying vec2		var_TexSpecular;
varying vec2		var_TexLight;
varying vec3		var_Tangent;
varying vec3		var_Binormal;
varying vec3		var_Normal;



void	main()
{
	vec4 position = attr_Position;
	
#if defined(USE_DEFORM_VERTEXES)
	position = DeformPosition(	u_DeformGen,
								u_DeformWave,	// [base amplitude phase freq]
								u_DeformBulge,	// [width height speed]
								u_DeformSpread,
								u_Time,
								position,
								attr_Normal,
								attr_TexCoord0.st);
#endif

	// transform vertex position into homogenous clip-space
#if 1
	gl_Position = u_ModelViewProjectionMatrix * position;
#else
	gl_Position.xy = attr_TexCoord1 * 2.0 - 1.0;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;
#endif


	// transform diffusemap texcoords
	var_TexDiffuse = (u_DiffuseTextureMatrix * attr_TexCoord0).st;
	var_TexLight = attr_TexCoord1.st;

#if defined(USE_NORMAL_MAPPING) || defined(USE_PARALLAX_MAPPING)

#if 0
	// transform position into world space
	var_Position = (u_ModelMatrix * position).xyz;
	
	var_Tangent.xyz = (u_ModelMatrix * vec4(attr_Tangent, 0.0)).xyz;
	var_Binormal.xyz = (u_ModelMatrix * vec4(attr_Binormal, 0.0)).xyz;
	var_Normal.xyz = (u_ModelMatrix * vec4(attr_Normal, 0.0)).xyz;
#else
	var_Position = position.xyz;
	
	var_Tangent = attr_Tangent.xyz;
	var_Binormal = attr_Binormal.xyz;
	var_Normal = attr_Normal.xyz;
#endif
	
	// transform normalmap texcoords
	var_TexNormal = (u_NormalTextureMatrix * attr_TexCoord0).st;
	
	// transform specularmap texcoords
	var_TexSpecular = (u_SpecularTextureMatrix * attr_TexCoord0).st;
#endif
}
