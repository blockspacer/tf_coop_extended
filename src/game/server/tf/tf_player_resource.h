//========= Copyright � 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: TF's custom CPlayerResource
//
// $NoKeywords: $
//=============================================================================//

#ifndef TF_PLAYER_RESOURCE_H
#define TF_PLAYER_RESOURCE_H
#ifdef _WIN32
#pragma once
#endif

class CTFPlayerResource : public CPlayerResource
{
	DECLARE_CLASS( CTFPlayerResource, CPlayerResource );
	
public:
	DECLARE_SERVERCLASS();

	CTFPlayerResource();

	virtual void UpdatePlayerData( void );
	virtual void Spawn( void );

	int	GetTotalScore( int iIndex );

	float GetHealthPercent();
protected:
	CNetworkArray( int,	m_iTotalScore, MAX_PLAYERS+1 );
	CNetworkArray( int, m_iMaxHealth, MAX_PLAYERS+1 );
	CNetworkArray( int, m_iMaxBuffedHealth, MAX_PLAYERS + 1 );
	CNetworkArray( int, m_iPlayerClass, MAX_PLAYERS+1 );
	//CNetworkArray( bool, m_bArenaSpectator, MAX_PLAYERS + 1 );
	//CNetworkArray( int, m_iActiveDominations , MAX_PLAYERS+1 );
	//CNetworkArray( float, m_flNextRespawnTime , MAX_PLAYERS+1 );
	//CNetworkArray( int, m_iChargeLevel, MAX_PLAYERS+1 );
	//CNetworkArray( int, m_iDamage, MAX_PLAYERS+1 );
	//CNetworkArray( int, m_iDamageAssist, MAX_PLAYERS+1 );
	//CNetworkArray( int, m_iDamageBoss, MAX_PLAYERS+1 );
	//CNetworkArray( int, m_iHealing, MAX_PLAYERS+1 );
	//CNetworkArray( int, m_iHealingAssist, MAX_PLAYERS+1 );
	//CNetworkArray( int, m_iDamageBlocked, MAX_PLAYERS+1 );
	CNetworkArray( int, m_iCurrencyCollected , MAX_PLAYERS+1 );
	//CNetworkArray( int, m_iBonusPoints , MAX_PLAYERS+1 );
	//CNetworkArray( int, m_iPlayerLevel , MAX_PLAYERS+1 );
	CNetworkArray( int, m_iKillstreak, MAX_PLAYERS+1 );
	//CNetworkArray( int, m_iStreaks, MAX_PLAYERS+1 );
	//CNetworkArray( int, m_iUpgradeRefundCredits, MAX_PLAYERS+1 );
	//CNetworkArray( int, m_iBuybackCredits, MAX_PLAYERS+1 );
	//CNetworkArray( int, m_iPlayerClassWhenKilled, MAX_PLAYERS+1 );
};

inline CTFPlayerResource *GetTFPlayerResource( void )
{
	if ( !g_pPlayerResource )
		return NULL;

	return assert_cast<CTFPlayerResource *>( g_pPlayerResource );
}

#endif // TF_PLAYER_RESOURCE_H