/// ============================================================================
/*
Copyright (C) 1997-2001 Id Software, Inc.
Copyright (C) 2004 Robert Beckebans <trebor_7@users.sourceforge.net>
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
#ifndef MAP_LOCAL_H
#define MAP_LOCAL_H

/// includes ===================================================================
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
// system -------------------------------------------------------------------
#include <map>
// qrazor-fx ----------------------------------------------------------------
#include "files.h"
// xreal --------------------------------------------------------------------



class face_t;
class portal_t;


#define	MAX_POINTS_ON_WINDING	64

// you can define on_epsilon in the makefile as tighter
#ifndef	ON_EPSILON
#define	ON_EPSILON	0.1
#endif


class winding_t
{
public:
	int			numpoints;
	vec3_t			p[4];		// variable sized
};

class side_t
{
public:
	int			planenum;
	int			texinfo;
	winding_t*		winding;
	side_t*			original;		// bspbrush_t sides will reference the mapbrush_t sides
	
	int			contents;		// from miptex
	int			surf;			// from miptex
	
	bool			visible;		// choose visble planes first
	bool			tested;			// this plane allready checked as a split
	bool			bevel;			// don't ever use for bsp splitting
};

class mapbrush_t
{
public:
	int			entitynum;
	int			brushnum;

	int			contents;
	
	vec3_t			mins, maxs;

	int			sides_num;
	side_t*			sides;
};


class entity_t
{
public:
	vec3_c					origin;
	
	std::vector<mapbrush_t*>		brushes;
	
	std::map<std::string, std::string>	epairs;

	// only valid for func_areaportals
	int					areaportalnum;
	int					portalareas[2];
};


#define	PLANENUM_LEAF		-1

#define	MAXEDGES		20

class face_t
{
public:
	face_t*			next;		// on node

	// the chain of faces off of a node can be merged or split,
	// but each face_t along the way will remain in the chain
	// until the entire tree is freed
	face_t*			merged;	// if set, this face isn't valid anymore
	face_t*			split[2];	// if set, this face isn't valid anymore

	portal_t*		portal;
	int			texinfo;
	int			planenum;
	int			contents;	// faces in different contents can't merge
	int			outputnumber;
	winding_t*		w;
	int			numpoints;
	bool			badstartvert;	// tjunctions cannot be fixed without a midpoint vertex
	int			vertexnums[MAXEDGES];
};



class bspbrush_t
{
public:
	bspbrush_t*		next;
	
	vec3_t			mins, maxs;
	int			side, testside;		// side of node during construction
	mapbrush_t*		original;
	int			sides_num;
	side_t			sides[6];			// variably sized
};



#define	MAX_NODE_BRUSHES	8
class node_t
{
public:
	// both leafs and nodes
	int			planenum;	// -1 = leaf node
	node_t*			parent;
	vec3_t			mins, maxs;	// valid after portalization
	bspbrush_t*		volume;		// one for each leaf/node

	// nodes only
	bool			detail_seperator;	// a detail brush caused the split
	side_t*			side;			// the side that created the node
	node_t*			children[2];
	face_t*			faces;

	// leafs only
	bspbrush_t*		brushlist;	// fragments of all brushes in this leaf
	int			contents;	// OR of all brush contents
	int			occupied;	// 1 or greater can reach entity
	entity_t*		occupant;	// for leak file testing
	int			cluster;	// for portalfile writing
	int			area;		// for areaportals
	portal_t*		portals;	// also on nodes during construction
};

class portal_t
{
public:
	cplane_c		plane;
	node_t*			onnode;		// NULL = outside box
	node_t*			nodes[2];		// [0] = front side of plane
	portal_t*		next[2];
	winding_t*		winding;

	bool			sidefound;		// false if ->side hasn't been checked
	side_t*			side;			// NULL = non-visible
	face_t*			face[2];		// output face in bsp file
};

class tree_t
{
public:
	node_t*			headnode;
	node_t			outside_node;
	vec3_t			mins, maxs;
};



//
// map_bspfile.cxx
//
extern std::vector<bsp_dentdata_t>	dentdata;

extern std::vector<bsp_dshader_t>	dshaders;

extern std::vector<bsp_dplane_t>	dplanes;

extern std::vector<bsp_dnode_t>		dnodes;

extern std::vector<bsp_dleaf_t>		dleafs;

extern std::vector<bsp_dleafsurface_t>	dleafsurfaces;

extern std::vector<bsp_dleafbrush_t>	dleafbrushes;

extern std::vector<bsp_dmodel_t>	dmodels;

extern std::vector<bsp_dbrush_t>	dbrushes;

extern std::vector<bsp_dbrushside_t>	dbrushsides;

extern std::vector<bsp_dvertex_t>	dvertexes;

extern std::vector<bsp_dindex_t>	dindexes;

extern std::vector<bsp_dsurface_t>	dsurfaces;

extern std::vector<bsp_dvisdata_t>	dvisdata;

//extern	int		dareas_num;
//extern	bsp_darea_t	dareas[MAX_BSP_AREAS];

//extern int			dareaportals_num;
//extern bsp_dareaportal_t	dareaportals[MAX_BSP_AREAPORTALS];




//extern byte			dpop[256];

//void	DecompressVis(byte *in, byte *decompressed);
//int	CompressVis(byte *vis, byte *dest);


void		LoadBSPFile(const std::string &filename);
void		WriteBSPFile(const std::string &filename);
void		PrintBSPFileSizes();

//
// map_entity.cxx
//
extern std::vector<entity_t>	entities;

void		ParseEntities();
void		UnparseEntities();

void 		SetKeyValue(entity_t &ent, const std::string &key, const std::string &value);
const char*	ValueForKey(entity_t &ent, const std::string &key);
// will return "" if not present

vec_t		GetFloatForKey(entity_t &ent, const std::string &key);
void 		GetVector3ForKey(entity_t &ent, const std::string &key, vec3_c &v);

void		PrintEntity(entity_t &ent);


//
// map_map.cxx
//
void		LoadMapFile(const std::string &filename);


//
// map_winding.cxx
//
winding_t*	AllocWinding(int points);
vec_t		WindingArea(winding_t *w);
void		WindingCenter(winding_t *w, vec3_t center);
void		ClipWindingEpsilon(winding_t *in, vec3_t normal, vec_t dist, vec_t epsilon, winding_t **front, winding_t **back);
winding_t*	ChopWinding(winding_t *in, vec3_t normal, vec_t dist);
winding_t*	CopyWinding(winding_t *w);
winding_t*	ReverseWinding(winding_t *w);
winding_t*	BaseWindingForPlane(vec3_t normal, vec_t dist);
void		CheckWinding(winding_t *w);
void		WindingPlane(winding_t *w, vec3_c &normal, vec_t &dist);
void		RemoveColinearPoint(winding_t *w);
int		WindingOnPlaneSide(winding_t *w, vec3_t normal, vec_t dist);
void		FreeWinding(winding_t *w);
void		WindingBounds(winding_t *w, vec3_t mins, vec3_t maxs);

void		ChopWindingInPlace(winding_t **w, vec3_t normal, vec_t dist, vec_t epsilon);
// frees the original if clipped

void		pw(winding_t *w);





#endif // MAP_LOCAL_H
