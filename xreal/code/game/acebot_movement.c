/*
===========================================================================
Copyright (C) 1998 Steve Yeager
Copyright (C) 2008 Robert Beckebans <trebor_7@users.sourceforge.net>

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
//
//  acebot_movement.c - This file contains all of the 
//                      movement routines for the ACE bot



#include "g_local.h"
#include "acebot.h"

#if defined(ACEBOT)

///////////////////////////////////////////////////////////////////////
// Checks if bot can move (really just checking the ground)
// Also, this is not a real accurate check, but does a
// pretty good job and looks for lava/slime. 
///////////////////////////////////////////////////////////////////////
qboolean ACEMV_CanMove(gentity_t * self, int direction)
{
	vec3_t          forward, right;
	vec3_t          offset, start, end;
	vec3_t          angles;
	trace_t         tr;

	// Now check to see if move will move us off an edge
	VectorCopy(self->bs.viewAngles, angles);

	if(direction == MOVE_LEFT)
		angles[1] += 90;
	else if(direction == MOVE_RIGHT)
		angles[1] -= 90;
	else if(direction == MOVE_BACK)
		angles[1] -= 180;

	// Set up the vectors
	AngleVectors(angles, forward, right, NULL);

	VectorSet(offset, 36, 0, 24);
	G_ProjectSource(self->s.origin, offset, forward, right, start);

	VectorSet(offset, 36, 0, -400);
	G_ProjectSource(self->s.origin, offset, forward, right, end);
	
	trap_Trace(&tr, self->client->ps.origin, NULL, NULL, end, self->s.number, MASK_PLAYERSOLID);

	if((tr.fraction > 0.3 && tr.fraction != 1) || tr.contents & (CONTENTS_LAVA | CONTENTS_SLIME))
	{
		if(debug_mode)
			debug_printf("%s: move blocked\n", self->client->pers.netname);
		return qfalse;
	}

	return qtrue;				// yup, can move
}

///////////////////////////////////////////////////////////////////////
// Handle special cases of crouch/jump
//
// If the move is resolved here, this function returns
// true.
///////////////////////////////////////////////////////////////////////
qboolean ACEMV_SpecialMove(gentity_t * self, usercmd_t * ucmd)
{
#if 0
	vec3_t          dir, forward, right, start, end, offset;
	vec3_t          top;
	trace_t         tr;

	// Get current direction
	VectorCopy(self->client->ps.viewangles, dir);
	dir[YAW] = self->bs.viewAngles[YAW];
	AngleVectors(dir, forward, right, NULL);

	VectorSet(offset, 18, 0, 0);
	G_ProjectSource(self->s.origin, offset, forward, right, start);
	offset[0] += 18;
	G_ProjectSource(self->s.origin, offset, forward, right, end);

	// trace it
	start[2] += 18;				// so they are not jumping all the time
	end[2] += 18;
	tr = gi.trace(start, self->mins, self->maxs, end, self, MASK_MONSTERSOLID);

	if(tr.allsolid)
	{
		// Check for crouching
		start[2] -= 14;
		end[2] -= 14;

		// Set up for crouching check
		VectorCopy(self->maxs, top);
		top[2] = 0.0;			// crouching height
		tr = gi.trace(start, self->mins, top, end, self, MASK_PLAYERSOLID);

		// Crouch
		if(!tr.allsolid)
		{
			ucmd->forwardmove = 400;
			ucmd->upmove = -400;
			return true;
		}

		// Check for jump
		start[2] += 32;
		end[2] += 32;
		tr = gi.trace(start, self->mins, self->maxs, end, self, MASK_MONSTERSOLID);

		if(!tr.allsolid)
		{
			ucmd->forwardmove = 400;
			ucmd->upmove = 400;
			return true;
		}
	}

	return false;				// We did not resolve a move here
#else
	return qfalse;
#endif
}


///////////////////////////////////////////////////////////////////////
// Checks for obstructions in front of bot
//
// This is a function I created origianlly for ACE that
// tries to help steer the bot around obstructions.
//
// If the move is resolved here, this function returns true.
///////////////////////////////////////////////////////////////////////
qboolean ACEMV_CheckEyes(gentity_t * self, usercmd_t * ucmd)
{
	vec3_t          forward, right;
	vec3_t          leftstart, rightstart, focalpoint;
	vec3_t          upstart, upend;
	vec3_t          dir, offset;

	trace_t         traceRight, traceLeft, traceUp, traceFront;	// for eyesight
	gentity_t      *traceEntity;

	// Get current angle and set up "eyes"
	VectorCopy(self->bs.viewAngles, dir);
	AngleVectors(dir, forward, right, NULL);

	// Let them move to targets by walls
	if(!self->bs.movetarget)
		VectorSet(offset, 200, 0, 4);	// focalpoint 
	else
		VectorSet(offset, 36, 0, 4);	// focalpoint 

	G_ProjectSource(self->s.origin, offset, forward, right, focalpoint);

	// Check from self to focalpoint
	// Ladder code
	VectorSet(offset, 36, 0, 0);	// set as high as possible
	G_ProjectSource(self->s.origin, offset, forward, right, upend);
	
	//traceFront = gi.trace(self->s.origin, self->mins, self->maxs, upend, self, MASK_OPAQUE);
	trap_Trace(&traceFront, self->client->ps.origin, self->r.mins, self->r.maxs, upend, self->s.number, MASK_PLAYERSOLID);

	if(traceFront.contents & CONTENTS_DETAIL)	// using detail brush here cuz sometimes it does not pick up ladders...??
	{
		ucmd->upmove = 127;
		ucmd->forwardmove = 127;
		return qtrue;
	}

	// If this check fails we need to continue on with more detailed checks
	if(traceFront.fraction == 1)
	{
		ucmd->forwardmove = 127;
		return qtrue;
	}

	VectorSet(offset, 0, 18, 4);
	G_ProjectSource(self->s.origin, offset, forward, right, leftstart);

	offset[1] -= 36;			// want to make sure this is correct
	//VectorSet(offset, 0, -18, 4);
	G_ProjectSource(self->s.origin, offset, forward, right, rightstart);

	//traceRight = gi.trace(rightstart, NULL, NULL, focalpoint, self, MASK_OPAQUE);
	//traceLeft = gi.trace(leftstart, NULL, NULL, focalpoint, self, MASK_OPAQUE);
	
	trap_Trace(&traceRight, rightstart, NULL, NULL, focalpoint, self->s.number, MASK_PLAYERSOLID);
	trap_Trace(&traceLeft, leftstart, NULL, NULL, focalpoint, self->s.number, MASK_PLAYERSOLID);

	// Wall checking code, this will degenerate progressivly so the least cost 
	// check will be done first.

	// If open space move ok
	
	if(traceLeft.entityNum != ENTITYNUM_WORLD)
	{
		traceEntity = &g_entities[traceLeft.entityNum];	
	}
	else
	{
		traceEntity = NULL;	
	}
	
	if(traceRight.fraction != 1 || traceLeft.fraction != 1)// || (traceEntity && strcmp(traceEntity->classname, "func_door") != 0))
	{
		// Special uppoint logic to check for slopes/stairs/jumping etc.
		VectorSet(offset, 0, 18, 24);
		G_ProjectSource(self->s.origin, offset, forward, right, upstart);

		VectorSet(offset, 0, 0, 200);	// scan for height above head
		G_ProjectSource(self->s.origin, offset, forward, right, upend);
		//traceUp = gi.trace(upstart, NULL, NULL, upend, self, MASK_OPAQUE);
		trap_Trace(&traceUp, upstart, NULL, NULL, upend, self->s.number, MASK_PLAYERSOLID);

		VectorSet(offset, 200, 0, 200 * traceUp.fraction - 5);	// set as high as possible
		G_ProjectSource(self->s.origin, offset, forward, right, upend);
		//traceUp = gi.trace(upstart, NULL, NULL, upend, self, MASK_OPAQUE);
		trap_Trace(&traceUp, upstart, NULL, NULL, upend, self->s.number, MASK_PLAYERSOLID);

		// If the upper trace is not open, we need to turn.
		if(traceUp.fraction != 1)
		{
			if(traceRight.fraction > traceLeft.fraction)
				self->bs.viewAngles[YAW] += (1.0 - traceLeft.fraction) * 45.0;
			else
				self->bs.viewAngles[YAW] += -(1.0 - traceRight.fraction) * 45.0;

			ucmd->forwardmove = 127;
			return qtrue;
		}
	}

	return qfalse;
}

///////////////////////////////////////////////////////////////////////
// Make the change in angles a little more gradual, not so snappy
// Subtle, but noticeable.
// 
// Modified from the original id ChangeYaw code...
///////////////////////////////////////////////////////////////////////
void ACEMV_ChangeBotAngle(gentity_t * ent)
{
#if 0
	float           ideal_yaw;
	float           ideal_pitch;
	float           current_yaw;
	float           current_pitch;
	float           move;
	float           speed;
	vec3_t          ideal_angle;

	// Normalize the move angle first
	VectorNormalize(ent->bs.move_vector);

	current_yaw = anglemod(ent->s.angles[YAW]);
	current_pitch = anglemod(ent->s.angles[PITCH]);

	vectoangles(ent->bs.move_vector, ideal_angle);

	ideal_yaw = anglemod(ideal_angle[YAW]);
	ideal_pitch = anglemod(ideal_angle[PITCH]);

	// Yaw
	if(current_yaw != ideal_yaw)
	{
		move = ideal_yaw - current_yaw;
		speed = ent->yaw_speed;
		if(ideal_yaw > current_yaw)
		{
			if(move >= 180)
				move = move - 360;
		}
		else
		{
			if(move <= -180)
				move = move + 360;
		}
		if(move > 0)
		{
			if(move > speed)
				move = speed;
		}
		else
		{
			if(move < -speed)
				move = -speed;
		}
		ent->s.angles[YAW] = anglemod(current_yaw + move);
	}

	// Pitch
	if(current_pitch != ideal_pitch)
	{
		move = ideal_pitch - current_pitch;
		speed = ent->yaw_speed;
		if(ideal_pitch > current_pitch)
		{
			if(move >= 180)
				move = move - 360;
		}
		else
		{
			if(move <= -180)
				move = move + 360;
		}
		if(move > 0)
		{
			if(move > speed)
				move = speed;
		}
		else
		{
			if(move < -speed)
				move = -speed;
		}
		ent->s.angles[PITCH] = anglemod(current_pitch + move);
	}
#endif
}

///////////////////////////////////////////////////////////////////////
// Set bot to move to it's movetarget. (following node path)
///////////////////////////////////////////////////////////////////////
void ACEMV_MoveToGoal(gentity_t * self, usercmd_t * ucmd)
{
#if 1
	// If a rocket or grenade is around deal with it
	// Simple, but effective (could be rewritten to be more accurate)
	if(strcmp(self->bs.movetarget->classname, "rocket") == 0 || strcmp(self->bs.movetarget->classname, "grenade") == 0)
	{
		VectorSubtract(self->bs.movetarget->s.origin, self->s.origin, self->bs.move_vector);
		ACEMV_ChangeBotAngle(self);
		if(debug_mode)
			debug_printf("%s: Oh crap a rocket!\n", self->client->pers.netname);

		// strafe left/right
		if(rand() % 1 && ACEMV_CanMove(self, MOVE_LEFT))
		{
			ucmd->rightmove = -127;
		}
		else if(ACEMV_CanMove(self, MOVE_RIGHT))
		{
			ucmd->rightmove = 127;
		}
		return;

	}
	else
	{
		// Set bot's movement direction
		VectorSubtract(self->bs.movetarget->s.origin, self->s.origin, self->bs.move_vector);
		ACEMV_ChangeBotAngle(self);
		ucmd->forwardmove = 127;
		return;
	}
#endif
}

///////////////////////////////////////////////////////////////////////
// Main movement code. (following node path)
///////////////////////////////////////////////////////////////////////
void ACEMV_Move(gentity_t * self, usercmd_t * ucmd)
{
#if 1
	vec3_t          dist;
	int             current_node_type = -1;
	int             next_node_type = -1;
	int             i;

	// Get current and next node back from nav code.
	if(!ACEND_FollowPath(self))
	{
		self->bs.state = STATE_WANDER;
		self->bs.wander_timeout = level.time + 1000;
		return;
	}

	current_node_type = nodes[self->bs.current_node].type;
	next_node_type = nodes[self->bs.next_node].type;

	///////////////////////////
	// Move To Goal
	///////////////////////////
	if(self->bs.movetarget)
		ACEMV_MoveToGoal(self, ucmd);

	////////////////////////////////////////////////////////
	// Grapple
	///////////////////////////////////////////////////////
	/*
	if(next_node_type == NODE_GRAPPLE)
	{
		ACEMV_ChangeBotAngle(self);
		ACEIT_ChangeWeapon(self, FindItem("grapple"));
		ucmd->buttons = BUTTON_ATTACK;
		return;
	}
	// Reset the grapple if hangin on a graple node
	if(current_node_type == NODE_GRAPPLE)
	{
		CTFPlayerResetGrapple(self);
		return;
	}
	*/

	////////////////////////////////////////////////////////
	// Platforms
	///////////////////////////////////////////////////////
	if(current_node_type != NODE_PLATFORM && next_node_type == NODE_PLATFORM)
	{
		// check to see if lift is down?
		for(i = 0; i < num_items; i++)
			if(item_table[i].node == self->bs.next_node)
				if(item_table[i].ent->moverState != MOVER_POS1)
					return;		// Wait for elevator
	}
	
	if(current_node_type == NODE_PLATFORM && next_node_type == NODE_PLATFORM)
	{
		// Move to the center
		self->bs.move_vector[2] = 0;	// kill z movement    
		
		if(VectorLength(self->bs.move_vector) > 10)
			ucmd->forwardmove = 200;	// walk to center

		ACEMV_ChangeBotAngle(self);

		return;					// No move, riding elevator
	}

	////////////////////////////////////////////////////////
	// Jumpto Nodes
	///////////////////////////////////////////////////////
	if(next_node_type == NODE_JUMP ||
	   (current_node_type == NODE_JUMP && next_node_type != NODE_ITEM && nodes[self->bs.next_node].origin[2] > self->s.origin[2]))
	{
		// Set up a jump move
		ucmd->forwardmove = 127;
		ucmd->upmove = 127;

		ACEMV_ChangeBotAngle(self);

		VectorCopy(self->bs.move_vector, dist);
		VectorScale(dist, 127, self->client->ps.velocity);

		return;
	}

	////////////////////////////////////////////////////////
	// Ladder Nodes
	///////////////////////////////////////////////////////
	/*
	if(next_node_type == NODE_LADDER && nodes[self->next_node].origin[2] > self->s.origin[2])
	{
		// Otherwise move as fast as we can
		ucmd->forwardmove = 400;
		self->velocity[2] = 320;

		ACEMV_ChangeBotAngle(self);

		return;

	}
	// If getting off the ladder
	if(current_node_type == NODE_LADDER && next_node_type != NODE_LADDER && nodes[self->next_node].origin[2] > self->s.origin[2])
	{
		ucmd->forwardmove = 400;
		ucmd->upmove = 200;
		self->velocity[2] = 200;
		ACEMV_ChangeBotAngle(self);
		return;
	}
	*/

	////////////////////////////////////////////////////////
	// Water Nodes
	///////////////////////////////////////////////////////
	if(current_node_type == NODE_WATER)
	{
		// We need to be pointed up/down
		ACEMV_ChangeBotAngle(self);

		// If the next node is not in the water, then move up to get out.
		if(next_node_type != NODE_WATER && !(trap_PointContents(nodes[self->bs.next_node].origin, self->s.number) & MASK_WATER))	// Exit water
			ucmd->upmove = 127;

		ucmd->forwardmove = 100;
		return;

	}

	// Falling off ledge?
	if(self->s.groundEntityNum == ENTITYNUM_NONE)
	{
		ACEMV_ChangeBotAngle(self);

		self->client->ps.velocity[0] = self->bs.move_vector[0] * 360;
		self->client->ps.velocity[1] = self->bs.move_vector[1] * 360;

		return;
	}

	// Check to see if stuck, and if so try to free us
	// Also handles crouching
	if(VectorLength(self->client->ps.velocity) < 37)
	{
		// Keep a random factor just in case....
		if(random() > 0.1 && ACEMV_SpecialMove(self, ucmd))
			return;

		self->bs.viewAngles[YAW] += random() * 180 - 90;

		ucmd->forwardmove = 127;

		return;
	}

	// Otherwise move as fast as we can
	ucmd->forwardmove = 127;

	ACEMV_ChangeBotAngle(self);
#endif
}


///////////////////////////////////////////////////////////////////////
// Wandering code (based on old ACE movement code) 
///////////////////////////////////////////////////////////////////////
void ACEMV_Wander(gentity_t * self, usercmd_t * ucmd)
{
#if 1
	vec3_t          tmp;

	// Do not move
	if(self->bs.next_move_time > level.time)
		return;

	// Special check for elevators, stand still until the ride comes to a complete stop.
	/*
	 * FIXME
	if(self->groundentity != NULL && self->groundentity->use == Use_Plat)
		if(self->groundentity->moveinfo.state == STATE_UP || self->groundentity->moveinfo.state == STATE_DOWN)	// only move when platform not
		{
			self->velocity[0] = 0;
			self->velocity[1] = 0;
			self->velocity[2] = 0;
			self->next_move_time = level.time + 500;
			return;
		}
		
	*/


	// Is there a target to move to
	if(self->bs.movetarget)
	{
		ACEMV_MoveToGoal(self, ucmd);
	}

	
	////////////////////////////////
	// Swimming?
	////////////////////////////////
	VectorCopy(self->s.origin, tmp);
	tmp[2] += 24;

	if(trap_PointContents(tmp, self->s.number) & MASK_WATER)
	{
		// If drowning and no node, move up
		if(self->client->airOutTime > 0)
		{
			ucmd->upmove = 1;
			self->bs.viewAngles[PITCH] = -45;
		}
		else
			ucmd->upmove = 15;

		ucmd->forwardmove = 100;
	}
	else
	{
		//self->client->>airOutTime = 0;	// probably shound not be messing with this, but
	}

	////////////////////////////////
	// Lava?
	////////////////////////////////
	tmp[2] -= 48;
	if(trap_PointContents(tmp, self->s.number) & (CONTENTS_LAVA | CONTENTS_SLIME))
	{
		//  safe_bprintf(PRINT_MEDIUM,"lava jump\n");
		self->bs.viewAngles[YAW] += random() * 360 - 180;
		ucmd->forwardmove = 127;
		ucmd->upmove = 127;
		return;
	}

	if(ACEMV_CheckEyes(self, ucmd))
		return;

	// Check for special movement if we have a normal move (have to test)
	if(VectorLength(self->client->ps.velocity) < 37)
	{
		if(random() > 0.1 && ACEMV_SpecialMove(self, ucmd))
			return;

		self->bs.viewAngles[YAW] += random() * 180 - 90;

		// if there is ground continue otherwise wait for next move
		if(/*!M_CheckBottom ||*/ self->s.groundEntityNum != ENTITYNUM_NONE)
			ucmd->forwardmove = 127;

		return;
	}

	ucmd->forwardmove = 127;
#endif
}

///////////////////////////////////////////////////////////////////////
// Attack movement routine
//
// NOTE: Very simple for now, just a basic move about avoidance.
//       Change this routine for more advanced attack movement.
///////////////////////////////////////////////////////////////////////
void ACEMV_Attack(gentity_t * self, usercmd_t * ucmd)
{
#if 0
	float           c;
	vec3_t          target;
	vec3_t          angles;

	// Randomly choose a movement direction
	c = random();

	if(c < 0.2 && ACEMV_CanMove(self, MOVE_LEFT))
		ucmd->sidemove -= 400;
	else if(c < 0.4 && ACEMV_CanMove(self, MOVE_RIGHT))
		ucmd->sidemove += 400;

	if(c < 0.6 && ACEMV_CanMove(self, MOVE_FORWARD))
		ucmd->forwardmove += 400;
	else if(c < 0.8 && ACEMV_CanMove(self, MOVE_FORWARD))
		ucmd->forwardmove -= 400;

	if(c < 0.95)
		ucmd->upmove -= 200;
	else
		ucmd->upmove += 200;

	// Set the attack 
	ucmd->buttons = BUTTON_ATTACK;

	// Aim
	VectorCopy(self->enemy->s.origin, target);

	// modify attack angles based on accuracy (mess this up to make the bot's aim not so deadly)
//  target[0] += (random()-0.5) * 20;
//  target[1] += (random()-0.5) * 20;

	// Set direction
	VectorSubtract(target, self->s.origin, self->move_vector);
	vectoangles(self->move_vector, angles);
	VectorCopy(angles, self->bs.viewAngles);

//  if(debug_mode)
//      debug_printf("%s attacking %s\n",self->client->pers.netname,self->enemy->client->pers.netname);
#endif
}

#endif
