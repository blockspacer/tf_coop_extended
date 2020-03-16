//========= Copyright � 1996-2005, Valve LLC, All rights reserved. ============
//
//=============================================================================
#ifndef TF_TEAM_H
#define TF_TEAM_H
#ifdef _WIN32
#pragma once
#endif

#include "utlvector.h"
#include "team.h"
#include "tf_shareddefs.h"

class CBaseObject;
class CAI_BaseNPC;

//=============================================================================
// TF Teams.
//
class CTFTeam : public CTeam
{
	DECLARE_CLASS( CTFTeam, CTeam );
	DECLARE_SERVERCLASS();

public:

	CTFTeam();

	// Classes.
//	int				GetNumOfClass( TFClass iClass );

	// TF Teams.
//	CTFTeam			*GetEnemyTeam();
	void			SetColor( color32 color );
	color32			GetColor( void );

	// Score.
	void			ShowScore( CBasePlayer *pPlayer );

	// Objects.
	void			AddObject( CBaseObject *pObject );
	void			RemoveObject( CBaseObject *pObject );
	bool			IsObjectOnTeam( CBaseObject *pObject ) const;
	int				GetNumObjects( int iObjectType = -1 );
	CBaseObject		*GetObject( int num );

	// NPCs.
	void			AddNPC( CAI_BaseNPC *pNPC );
	void			RemoveNPC( CAI_BaseNPC *pNPC );
	bool			IsNPCOnTeam( CAI_BaseNPC *pNPC ) const;
	int				GetNumNPCs( void );
	CAI_BaseNPC		*GetNPC( int num );

	// Flag Captures
	int				GetFlagCaptures( void ) { return m_nFlagCaptures; }
	void			SetFlagCaptures( int nCaptures ) { m_nFlagCaptures = nCaptures; }
	void			IncrementFlagCaptures( void ) { m_nFlagCaptures++; }

	// Roles
	void			SetRole( int iTeamRole ) { m_iRole = iTeamRole; }
	int				GetRole( void ) { return m_iRole; }
	
	void 			GetOpposingTFTeamList( CUtlVector<CTFTeam *> *pTeamList );

	int				GetWeapon( int iIndex );
	bool			HasWeapon( int iWeapon );
	int				GetNumWeapons( void );
	void			AddWeapon( int iWeapon );
	void			RemoveWeapon( int iWeapon );
	void			RemoveAllWeapons( void );
private:
	
	color32						m_TeamColor;
	CUtlVector< CHandle<CBaseObject> >	m_aObjects;			// List of team objects.
	CUtlVector< CAI_BaseNPC * >		m_aNPCs;				// List of team NPCs.

	CNetworkVar( int, m_nFlagCaptures );
	CNetworkVar( int, m_iRole );

	CUtlVector<int> m_aWeapons;
};

class CTFTeamManager
{
public:

	CTFTeamManager();

	// Creation/Destruction.
	bool	Init( void );
	void    Shutdown( void );

	bool	IsValidTeam( int iTeam );
	int		GetTeamCount( void );
	CTFTeam *GetTeam( int iTeam );
	CTFTeam *GetSpectatorTeam();

	color32 GetUndefinedTeamColor( void );

	void AddTeamScore( int iTeam, int iScoreToAdd );

	void IncrementFlagCaptures( int iTeam );
	int GetFlagCaptures( int iTeam );

	// Screen prints.
	void PlayerCenterPrint( CBasePlayer *pPlayer, const char *msg_name, const char *param1 = NULL, const char *param2 = NULL, const char *param3 = NULL, const char *param4 = NULL );
	void TeamCenterPrint( int iTeam, const char *msg_name, const char *param1 = NULL, const char *param2 = NULL, const char *param3 = NULL, const char *param4 = NULL );
	void PlayerTeamCenterPrint( CBasePlayer *pPlayer, const char *msg_name, const char *param1 = NULL, const char *param2 = NULL, const char *param3 = NULL, const char *param4 = NULL );

	// Vox

private:

	int		Create( const char *pName, color32 color );

private:

	color32	m_UndefinedTeamColor;
};

extern CTFTeamManager *TFTeamMgr();
extern CTFTeam *GetGlobalTFTeam( int iIndex );

#endif // TF_TEAM_H