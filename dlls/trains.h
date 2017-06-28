/*
    Copyright 2001 to 2004. The Battle Grounds Team and Contributors

    This file is part of the Battle Grounds Modification for Half-Life.

    The Battle Grounds Modification for Half-Life is free software;
    you can redistribute it and/or modify it under the terms of the
    GNU Lesser General Public License as published by the Free
    Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    The Battle Grounds Modification for Half-Life is distributed in
    the hope that it will be useful, but WITHOUT ANY WARRANTY; without
    even the implied warranty of MERCHANTABILITY or FITNESS FOR A
    PARTICULAR PURPOSE.  See the GNU Lesser General Public License
    for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with The Battle Grounds Modification for Half-Life;
    if not, write to the Free Software Foundation, Inc., 59 Temple Place,
    Suite 330, Boston, MA  02111-1307  USA

    You must obey the GNU Lesser General Public License in all respects for
    all of the code used other than code distributed with the Half-Life
    SDK developed by Valve.  If you modify this file, you may extend this
    exception to your version of the file, but you are not obligated to do so.
    If you do not wish to do so, delete this exception statement from your
    version.
*/

/***
*
*	Copyright (c) 1999, 2000 Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/

#ifndef TRAINS_H
#define TRAINS_H

// Tracktrain spawn flags
#define SF_TRACKTRAIN_NOPITCH		0x0001
#define SF_TRACKTRAIN_NOCONTROL		0x0002
#define SF_TRACKTRAIN_FORWARDONLY	0x0004
#define SF_TRACKTRAIN_PASSABLE		0x0008

// Spawnflag for CPathTrack
#define SF_PATH_DISABLED		0x00000001
#define SF_PATH_FIREONCE		0x00000002
#define SF_PATH_ALTREVERSE		0x00000004
#define SF_PATH_DISABLE_TRAIN	0x00000008
#define SF_PATH_ALTERNATE		0x00008000

// Spawnflags of CPathCorner
#define SF_CORNER_WAITFORTRIG	0x001
#define SF_CORNER_TELEPORT		0x002
#define SF_CORNER_FIREONCE		0x004

//#define PATH_SPARKLE_DEBUG		1	// This makes a particle effect around path_track entities for debugging
class CPathTrack : public CPointEntity
{
public:
	void		Spawn( void );
	void		Activate( void );
	void		KeyValue( KeyValueData* pkvd);
	
	void		SetPrevious( CPathTrack *pprevious );
	void		Link( void );
	void		Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );

	CPathTrack	*ValidPath( CPathTrack *ppath, int testFlag );		// Returns ppath if enabled, NULL otherwise
	void		Project( CPathTrack *pstart, CPathTrack *pend, Vector *origin, float dist );

	static CPathTrack *Instance( edict_t *pent );

	CPathTrack	*LookAhead( Vector *origin, float dist, int move );
	CPathTrack	*Nearest( Vector origin );

	CPathTrack	*GetNext( void );
	CPathTrack	*GetPrevious( void );

	virtual int		Save( CSave &save );
	virtual int		Restore( CRestore &restore );
	
	static	TYPEDESCRIPTION m_SaveData[];
#if PATH_SPARKLE_DEBUG
	void EXPORT Sparkle(void);
#endif

	float		m_length;
	string_t	m_altName;
	CPathTrack	*m_pnext;
	CPathTrack	*m_pprevious;
	CPathTrack	*m_paltpath;
};


class CFuncTrackTrain : public CBaseEntity
{
public:
	void Spawn( void );
	void Precache( void );

	void Blocked( CBaseEntity *pOther );
	void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	void KeyValue( KeyValueData* pkvd );

	void EXPORT Next( void );
	void EXPORT Find( void );
	void EXPORT NearestPath( void );
	void EXPORT DeadEnd( void );

	void NextThink( float thinkTime, bool alwaysThink );

	void SetTrack( CPathTrack *track ) { m_ppath = track->Nearest(pev->origin); }
	void SetControls( entvars_t *pevControls );
	bool OnControls( entvars_t *pev );

	void StopSound ( void );
	void UpdateSound ( void );
	
	static CFuncTrackTrain *Instance( edict_t *pent );

	virtual int		Save( CSave &save );
	virtual int		Restore( CRestore &restore );
	
	static	TYPEDESCRIPTION m_SaveData[];
	virtual int	ObjectCaps( void ) { return (CBaseEntity :: ObjectCaps() & ~FCAP_ACROSS_TRANSITION) | FCAP_DIRECTIONAL_USE; }

	virtual void	OverrideReset( void );

	CPathTrack	*m_ppath;
	float		m_length;
	float		m_height;
	float		m_speed;
	float		m_dir;
	float		m_startSpeed;
	Vector		m_controlMins;
	Vector		m_controlMaxs;
	int			m_soundPlaying;
	int			m_sounds;
	float		m_flVolume;
	float		m_flBank;
	float		m_oldSpeed;

private:
	unsigned short m_usAdjustPitch;
};

#endif
