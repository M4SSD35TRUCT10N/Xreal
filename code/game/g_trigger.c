/*
=======================================================================================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Quake III Arena source code; if not, write to the Free
Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
=======================================================================================================================================
*/

#include "g_local.h"

/*
=======================================================================================================================================
InitTrigger
=======================================================================================================================================
*/
void InitTrigger(gentity_t *self) {

	if (!VectorCompare(self->s.angles, vec3_origin)) {
		G_SetMovedir(self->s.angles, self->movedir);
	}

	if (strstr(self->model, ".ase") || strstr(self->model, ".lwo") || strstr(self->model, ".prt")) {
		// don't set brush model
	} else {
		trap_SetBrushModel(self, self->model);
	}

	self->r.contents = CONTENTS_TRIGGER; // replaces the -1 from trap_SetBrushModel
	self->r.svFlags = SVF_NOCLIENT;
}

/*
=======================================================================================================================================
Multi_Wait

The wait time has passed, so set back up for another activation.
=======================================================================================================================================
*/
void Multi_Wait(gentity_t *ent) {
	ent->nextthink = 0;
}

/*
=======================================================================================================================================
Use_Trigger_Multiple

The trigger was just activated, ent->activator should be set to the activator so it can be held through a delay, so wait for the delay
time before firing.
=======================================================================================================================================
*/
void Use_Trigger_Multiple(gentity_t *ent, gentity_t *activator) {

	ent->activator = activator;

	if (ent->nextthink) {
		return; // can't retrigger until the wait is over
	}

	if (activator->client) {
		if (ent->red_only && activator->client->sess.sessionTeam != TEAM_RED) {
			return;
		}

		if (ent->blue_only && activator->client->sess.sessionTeam != TEAM_BLUE) {
			return;
		}
	}

	G_UseTargets(ent, ent->activator);
#ifdef G_LUA
	// Lua API callbacks
	if (ent->luaTrigger) {
		if (activator) {
			G_LuaHook_EntityTrigger(ent->luaTrigger, ent->s.number, activator->s.number);
		} else {
			G_LuaHook_EntityTrigger(ent->luaTrigger, ent->s.number, ENTITYNUM_WORLD);
		}
	}
#endif
	if (ent->wait > 0) {
		ent->think = Multi_Wait;
		ent->nextthink = level.time + (ent->wait + ent->random * crandom()) * 1000;
	} else {
		// we can't just remove (self) here, because this is a touch function called while looping through area links...
		ent->touch = 0;
		ent->nextthink = level.time + FRAMETIME;
		ent->think = G_FreeEntity;
	}
}

/*
=======================================================================================================================================
Use_Multi
=======================================================================================================================================
*/
void Use_Multi(gentity_t *ent, gentity_t *other, gentity_t *activator) {
	Use_Trigger_Multiple(ent, activator);
}

/*
=======================================================================================================================================
Touch_MultiTrigger
=======================================================================================================================================
*/
void Touch_MultiTrigger(gentity_t *self, gentity_t *other, trace_t *trace) {

	if (!other->client) {
		return;
	}

	Use_Trigger_Multiple(self, other);
}

/*QUAKED trigger_multiple (.5 .5 .5) ? RED_ONLY BLUE_ONLY
"wait" : Seconds between triggerings, 0.5 default, -1 = one time only.
"random" wait variance, default is 0
Variable sized repeatable trigger. Must be targeted at one or more entities.
so, the basic time between firing is a random time between
(wait - random) and (wait + random)
*/
void SP_trigger_multiple(gentity_t *ent) {

	G_SpawnFloat("wait", "0.5", &ent->wait);
	G_SpawnFloat("random", "0", &ent->random);
	G_SpawnBoolean("red_only", "0", &ent->red_only);
	G_SpawnBoolean("blue_only", "0", &ent->blue_only);

	if (ent->random >= ent->wait && ent->wait >= 0) {
		ent->random = ent->wait - FRAMETIME;
		G_Printf("trigger_multiple has random >= wait\n");
	}

	ent->touch = Touch_MultiTrigger;
	ent->use = Use_Multi;

	InitTrigger(ent);
	trap_LinkEntity(ent);
}

/*
=======================================================================================================================================

	TRIGGER ALWAYS

=======================================================================================================================================
*/

/*
=======================================================================================================================================
multi_flagonly_trigger

The trigger was just activated.
=======================================================================================================================================
*/
void multi_flagonly_trigger(gentity_t *ent, gentity_t *activator) {

	ent->activator = activator;

	if (!activator->client) {
		return;
	}

	if (ent->red_only && activator->client->ps.powerups[PW_BLUEFLAG]) {
		G_UseTargets(ent, ent->activator);
		Team_CaptureFlag(ent, activator, TEAM_RED);
	} else if (ent->blue_only && activator->client->ps.powerups[PW_REDFLAG]) {
		G_UseTargets(ent, ent->activator);
		Team_CaptureFlag(ent, activator, TEAM_BLUE);
	}
}

/*
=======================================================================================================================================
Touch_Flagonly_Multi
=======================================================================================================================================
*/
void Touch_Flagonly_Multi(gentity_t *self, gentity_t *other, trace_t *trace) {

	if (!other->client) {
		return;
	}

	multi_flagonly_trigger(self, other);
}

/*QUAKED trigger_flagonly_multiple(.5 .5 .5) ?
Player must be carrying the appropriate flag for it to trigger.
Either red_only or blue_only must be set.
*/
void SP_trigger_flagonly_multiple(gentity_t *ent) {

	G_SpawnBoolean("red_only", "0", &ent->red_only);
	G_SpawnBoolean("blue_only", "0", &ent->blue_only);

	ent->touch = Touch_Flagonly_Multi;
	//ent->use = Use_Multi;

	InitTrigger(ent);
	trap_LinkEntity(ent);
}

/*
=======================================================================================================================================
Trigger_Always_Think
=======================================================================================================================================
*/
void Trigger_Always_Think(gentity_t *ent) {

	G_UseTargets(ent, ent);
#ifdef G_LUA
	// Lua API callbacks
	if (ent->luaTrigger) {
		G_LuaHook_EntityTrigger(ent->luaTrigger, ent->s.number, ent->s.number);
	}
#endif
	G_FreeEntity(ent);
}

/*QUAKED trigger_always (.5 .5 .5) (-8 -8 -8) (8 8 8)
This trigger will always fire. It is activated by the world.
*/
void SP_trigger_always(gentity_t *ent) {

	// we must have some delay to make sure our use targets are present
	ent->nextthink = level.time + 300;
	ent->think = Trigger_Always_Think;
}

/*
=======================================================================================================================================

	TRIGGER PUSH

=======================================================================================================================================
*/

/*
=======================================================================================================================================
Touch_PushTrigger
=======================================================================================================================================
*/
void Touch_PushTrigger(gentity_t *self, gentity_t *other, trace_t *trace) {

	if (!other->client) {
		return;
	}
#ifdef G_LUA
	// Lua API callbacks
	if (self->luaTrigger) {
		G_LuaHook_EntityTrigger(self->luaTrigger, self->s.number, other->s.number);
	}
#endif
	BG_TouchJumpPad(&other->client->ps, &self->s);
}

/*
=======================================================================================================================================
AimAtTarget

Calculate origin2 so the target apogee will be hit.
=======================================================================================================================================
*/
void AimAtTarget(gentity_t *self) {
	gentity_t *ent;
	vec3_t origin;
	float height, gravity, time, forward;
	float dist;

	VectorAdd(self->r.absmin, self->r.absmax, origin);
	VectorScale(origin, 0.5, origin);

	ent = G_PickTarget(self->target);

	if (!ent) {
		G_FreeEntity(self);
		return;
	}

	height = ent->s.origin[2] - origin[2];
	gravity = -g_gravityZ.value;
	time = sqrt(height / (.5 * gravity));

	if (!time) {
		G_FreeEntity(self);
		return;
	}
	// set s.origin2 to the push velocity
	VectorSubtract(ent->s.origin, origin, self->s.origin2);

	self->s.origin2[2] = 0;
	dist = VectorNormalize(self->s.origin2);
	forward = dist / time;

	VectorScale(self->s.origin2, forward, self->s.origin2);

	self->s.origin2[2] = time * gravity;
}

/*QUAKED trigger_push (.5 .5 .5) ?
Must point at a target_position, which will be the apex of the leap.
This will be client side predicted, unlike target_push
*/
void SP_trigger_push(gentity_t *self) {

	InitTrigger(self);
	// unlike other triggers, we need to send this one to the client
	self->r.svFlags &= ~SVF_NOCLIENT;
	// make sure the client precaches this sound
	G_SoundIndex("sound/world/jumppad.wav");

	self->s.eType = ET_PUSH_TRIGGER;
	self->touch = Touch_PushTrigger;
	self->think = AimAtTarget;
	self->nextthink = level.time + FRAMETIME;

	trap_LinkEntity(self);
}

/*
=======================================================================================================================================
Use_Target_Push
=======================================================================================================================================
*/
void Use_Target_Push(gentity_t *self, gentity_t *other, gentity_t *activator) {

	if (!activator->client) {
		return;
	}

	if (activator->client->ps.pm_type != PM_NORMAL) {
		return;
	}

	if (activator->client->ps.powerups[PW_FLIGHT]) {
		return;
	}
#ifdef G_LUA
	// Lua API callbacks
	if (self->luaTrigger) {
		G_LuaHook_EntityTrigger(self->luaTrigger, self->s.number, activator->s.number);
	}
#endif
	VectorCopy(self->s.origin2, activator->client->ps.velocity);
	// play fly sound every 1.5 seconds
	if (activator->fly_sound_debounce_time < level.time) {
		activator->fly_sound_debounce_time = level.time + 1500;
		G_Sound(activator, CHAN_AUTO, self->soundIndex);
	}
}

/*QUAKED target_push (.5 .5 .5) (-8 -8 -8) (8 8 8) BOUNCEPAD
Pushes the activator in the direction.of angle, or towards a target apex.
"speed" defaults to 1000
if "bouncepad", play bounce noise instead of windfly
*/
void SP_target_push(gentity_t *self) {
	qboolean bouncepad;

	if (!self->speed) {
		self->speed = 1000;
	}

	G_SetMovedir(self->s.angles, self->s.origin2);
	VectorScale(self->s.origin2, self->speed, self->s.origin2);
	// Tr3B: FIXME sounds
	G_SpawnBoolean("bouncepad", "0", &bouncepad);

	if (bouncepad) {
		self->soundIndex = G_SoundIndex("sound/world/jumppad.wav");
	} else {
		self->soundIndex = G_SoundIndex("sound/misc/windfly.wav");
	}

	if (self->target) {
		VectorCopy(self->s.origin, self->r.absmin);
		VectorCopy(self->s.origin, self->r.absmax);
		self->think = AimAtTarget;
		self->nextthink = level.time + FRAMETIME;
	}

	self->use = Use_Target_Push;
}

/*
=======================================================================================================================================

	TRIGGER TELEPORT

=======================================================================================================================================
*/

/*
=======================================================================================================================================
Touch_TeleporterTrigger
=======================================================================================================================================
*/
void Touch_TeleporterTrigger(gentity_t *self, gentity_t *other, trace_t *trace) {
	gentity_t *dest;

	if (!other->client) {
		return;
	}

	if (other->client->ps.pm_type == PM_DEAD) {
		return;
	}
	// spectators only?
	if ((self->spawnflags & 1) && other->client->sess.sessionTeam != TEAM_SPECTATOR) {
		return;
	}

	dest = G_PickTarget(self->target);

	if (!dest) {
		G_Printf("Couldn't find teleporter destination\n");
		return;
	}
#ifdef G_LUA
	// Lua API callbacks
	if (self->luaTrigger) {
		G_LuaHook_EntityTrigger(self->luaTrigger, self->s.number, other->s.number);
	}
#endif
	TeleportPlayer(other, dest->s.origin, dest->s.angles);
}

/*QUAKED trigger_teleport (.5 .5 .5) ? SPECTATOR
Allows client side prediction of teleportation events.
Must point at a target_position, which will be the teleport destination.

If spectator is set, only spectators can use this teleport
Spectator teleporters are not normally placed in the editor, but are created
automatically near doors to allow spectators to move through them
*/
void SP_trigger_teleport(gentity_t *self) {

	InitTrigger(self);
	// unlike other triggers, we need to send this one to the client unless is a spectator trigger
	if (self->spawnflags & 1) {
		self->r.svFlags |= SVF_NOCLIENT;
	} else {
		self->r.svFlags &= ~SVF_NOCLIENT;
	}
	// make sure the client precaches this sound
	G_SoundIndex("sound/world/jumppad.wav");

	self->s.eType = ET_TELEPORT_TRIGGER;
	self->touch = Touch_TeleporterTrigger;

	trap_LinkEntity(self);
}

/*
=======================================================================================================================================

	TRIGGER HURT

=======================================================================================================================================
*/

/*
=======================================================================================================================================
Use_Trigger_Hurt
=======================================================================================================================================
*/
void Use_Trigger_Hurt(gentity_t *self, gentity_t *other, gentity_t *activator) {

	if (self->r.linked) {
		trap_UnlinkEntity(self);
	} else {
		trap_LinkEntity(self);
	}
}

/*
=======================================================================================================================================
Touch_HurtTrigger
=======================================================================================================================================
*/
void Touch_HurtTrigger(gentity_t *self, gentity_t *other, trace_t *trace) {
	int dflags;

	if (!other->takedamage) {
		return;
	}

	if (self->timestamp > level.time) {
		return;
	}

	if (self->slow) {
		self->timestamp = level.time + 1000;
	} else {
		self->timestamp = level.time + FRAMETIME;
	}
	// play sound
	if (!self->silent) {
		G_Sound(other, CHAN_AUTO, self->soundIndex);
	}
#ifdef G_LUA
	// Lua API callbacks
	if (self->luaTrigger) {
		G_LuaHook_EntityTrigger(self->luaTrigger, self->s.number, other->s.number);
	}
#endif
	if (self->no_protection)
		dflags = DAMAGE_NO_PROTECTION;
	} else {
		dflags = 0;
	}

	G_Damage(other, self, self, NULL, NULL, self->damage, dflags, MOD_TRIGGER_HURT);
}

/*QUAKED trigger_hurt (.5 .5 .5) ? START_OFF - SILENT NO_PROTECTION SLOW
Any entity that touches this will be hurt.
It does dmg points of damage each server frame
Targeting the trigger will toggle its on / off state.

SILENT			suppresses playing the sound
SLOW			changes the damage rate to once per second
NO_PROTECTION	*nothing* stops the damage

"dmg"			default 5 (whole numbers only)
*/
void SP_trigger_hurt(gentity_t *self) {

	InitTrigger(self);

	G_SpawnBoolean("start_off", "0", &self->start_off);
	G_SpawnBoolean("silent", "0", &self->silent);
	G_SpawnBoolean("no_protection", "0", &self->no_protection);
	G_SpawnBoolean("slow", "0", &self->slow);

	self->soundIndex = G_SoundIndex("sound/player/fry.ogg");
	self->touch = Touch_HurtTrigger;

	if (!self->damage) {
		self->damage = 5;
	}

	self->r.contents = CONTENTS_TRIGGER;
	/*
	if (self->spawnflags & 2) {
		self->use = Use_Trigger_Hurt;
	}
	*/
	// link in to the world if starting active
	if (!self->start_off) {
		trap_LinkEntity(self);
	}
}

/*
=======================================================================================================================================

	TIMER

=======================================================================================================================================
*/

/*
=======================================================================================================================================
Func_Timer_Think
=======================================================================================================================================
*/
void Func_Timer_Think(gentity_t *self) {

	G_UseTargets(self, self->activator);
#ifdef G_LUA
	// Lua API callbacks
	if (self->luaTrigger) {
		if (self->activator) {
			G_LuaHook_EntityTrigger(self->luaTrigger, self->s.number, self->activator->s.number);
		} else {
			G_LuaHook_EntityTrigger(self->luaTrigger, self->s.number, self->s.number);
		}
	}
#endif
	// set time before next firing
	self->nextthink = level.time + 1000 * (self->wait + crandom() * self->random);
}

/*
=======================================================================================================================================
Use_Func_Timer
=======================================================================================================================================
*/
void Use_Func_Timer(gentity_t *self, gentity_t *other, gentity_t *activator) {

	self->activator = activator;
	// if on, turn it off
	if (self->nextthink) {
		self->nextthink = 0;
		return;
	}
	// turn it on
	Func_Timer_Think(self);
}

/*QUAKED func_timer (0.3 0.1 0.6) (-8 -8 -8) (8 8 8) START_ON
This should be renamed trigger_timer...
Repeatedly fires its targets.
Can be turned on or off by using.

"wait"			base time between triggering all targets, default is 1
"random"		wait variance, default is 0
so, the basic time between firing is a random time between
(wait - random) and (wait + random)

*/
void SP_func_timer(gentity_t *self) {
	qboolean start_on;

	G_SpawnFloat("random", "1", &self->random);
	G_SpawnFloat("wait", "1", &self->wait);

	self->use = Use_Func_Timer;
	self->think = Func_Timer_Think;

	if (self->random >= self->wait) {
		self->random = self->wait - FRAMETIME;
		G_Printf("func_timer at %s has random >= wait\n", vtos(self->s.origin));
	}

	G_SpawnBoolean("start_on", "0", &start_on);

	if (start_on) {
		self->nextthink = level.time + FRAMETIME;
		self->activator = self;
	}

	self->r.svFlags = SVF_NOCLIENT;
}