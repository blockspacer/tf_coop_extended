//====== Copyright � 1996-2005, Valve Corporation, All rights reserved. =======//
//
// Purpose: TF Gas Grenade ( Hallucination Grenade )
//
//=============================================================================//
#include "cbase.h"
#include "tf_weaponbase.h"
#include "tf_gamerules.h"
#include "npcevent.h"
#include "engine/IEngineSound.h"
#include "tf_weapon_grenade_gas.h"

// Server specific.
#ifdef GAME_DLL
#include "tf_player.h"
#include "items.h"
#include "tf_weaponbase_grenadeproj.h"
#include "soundent.h"
#include "KeyValues.h"
#endif

#define GRENADE_GAS_TIMER	3.0f //Seconds
#define TF_HALLUCINATION_RADIUS 35.0f

//=============================================================================
//
// TF Gas Grenade Projectile functions (Server specific).
//

#define GRENADE_MODEL "models/tfc/spy_grenade.mdl"

LINK_ENTITY_TO_CLASS( tf_weapon_grenade_gas, CTFGrenadeGasProjectile );
PRECACHE_WEAPON_REGISTER( tf_weapon_grenade_gas );

#ifdef GAME_DLL

BEGIN_DATADESC( CTFGrenadeGasProjectile )
	DEFINE_THINKFUNC( Think_Emit ),
	DEFINE_THINKFUNC( Think_Fade ),
END_DATADESC()
//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
CTFGrenadeGasProjectile* CTFGrenadeGasProjectile::Create( const Vector &position, const QAngle &angles, 
																const Vector &velocity, const AngularImpulse &angVelocity, 
																CBaseCombatCharacter *pOwner, CBaseEntity *pWeapon, float flTimer, int iFlags )
{
	CTFGrenadeGasProjectile *pGrenade = static_cast<CTFGrenadeGasProjectile*>( CTFWeaponBaseGrenadeProj::Create( "tf_weapon_grenade_gas", position, angles, velocity, angVelocity, pOwner, pWeapon ) );
	if ( pGrenade )
	{
		pGrenade->InitGrenadeTFC( velocity, angVelocity, pOwner );
		pGrenade->ApplyLocalAngularVelocityImpulse( angVelocity );
	}

	return pGrenade;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFGrenadeGasProjectile::Spawn()
{
	SetModel( GRENADE_MODEL );
	SetDetonateTimerLength( GRENADE_GAS_TIMER );

	BaseClass::Spawn();

	m_hGasEffect = NULL;
}

CTFGrenadeGasProjectile::~CTFGrenadeGasProjectile()
{
	if ( m_hGasEffect.Get() )
	{
		UTIL_Remove( m_hGasEffect );
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFGrenadeGasProjectile::Precache()
{
	PrecacheModel( GRENADE_MODEL );
	PrecacheParticleSystem( "spy_gasgrenade" );
	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFGrenadeGasProjectile::BounceSound( void )
{
	EmitSound( "Weapon_HandGrenade.GrenadeBounce" );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFGrenadeGasProjectile::DetonateThink( void )
{
	// if we're past the detonate time but still moving, delay the detonate
	if ( gpGlobals->curtime > GetDetonateTime() && VPhysicsGetObject() )
	{
		Vector vel;
		VPhysicsGetObject()->GetVelocity( &vel, NULL );

		if ( vel.Length() > 35.0 )
		{
			SetDetonateTimerLength( gpGlobals->curtime + 0.5 );
		}
	}

	BaseClass::DetonateThink();
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTFGrenadeGasProjectile::Detonate()
{
	if ( ShouldNotDetonate() )
	{
		RemoveGrenade();
		return;
	}

	// start emitting gas
	if ( !TFGameRules()->IsTFCAllowed() )
	{
		if ( VPhysicsGetObject() )
			VPhysicsGetObject()->EnableMotion( false );
	}

	m_hGasEffect = ( CTFGasGrenadeEffect * )CreateEntityByName( "tf_gas_grenade_effect" );
	CBaseEntity *pGasEffect = m_hGasEffect.Get();
	if ( pGasEffect )
	{	
		DispatchSpawn( pGasEffect );
		pGasEffect->SetAbsOrigin( GetAbsOrigin() );
	}

	EmitSound( "BaseSmokeEffect.Sound" );

	// damage / hallucination effect in waves
	m_nPulses = 20;

	SetThink( &CTFGrenadeGasProjectile::Think_Emit );
	SetNextThink( gpGlobals->curtime + 0.1f );
}

//-----------------------------------------------------------------------------
// Purpose: Emit gas pulses
//-----------------------------------------------------------------------------
void CTFGrenadeGasProjectile::Think_Emit( void )
{
	Vector vecOrigin = GetAbsOrigin();
	float flDamage = 10;
	CTakeDamageInfo info( this, GetThrower(), vec3_origin, vecOrigin, flDamage, DMG_NERVEGAS | DMG_PREVENT_PHYSICS_FORCE );

	CBaseEntity* pEntity = NULL;
	while ( ( pEntity = gEntList.FindEntityInSphere( pEntity, vecOrigin, TF_HALLUCINATION_RADIUS ) ) != NULL )
	{
		if ( !pEntity )
			continue;

		if ( pEntity == this )
			continue;

		// check for valid player
		if ( !pEntity->IsPlayer() || !pEntity->IsNPC() )
			continue;

		if ( InSameTeam( pEntity ) )
			continue;

		pEntity->TakeDamage( info );
	}

	m_nPulses--;

	if ( m_nPulses <= 0 )
	{
		// Fade out
		SetThink( &CTFGrenadeGasProjectile::Think_Fade );
	}

	SetNextThink( gpGlobals->curtime + 0.75 );
}

//-----------------------------------------------------------------------------
// Fade the projectile out over time before making it disappear
//-----------------------------------------------------------------------------
void CTFGrenadeGasProjectile::Think_Fade()
{
	color32 c = GetRenderColor();
	c.a -= 1;
	SetRenderColor( c.r, c.b, c.g, c.a );

	if ( !c.a )
	{
		UTIL_Remove( this );
	}

	SetNextThink( gpGlobals->curtime );
}

#endif


IMPLEMENT_NETWORKCLASS_ALIASED( TFGasGrenadeEffect, DT_TFGasGrenadeEffect )

BEGIN_NETWORK_TABLE( CTFGasGrenadeEffect, DT_TFGasGrenadeEffect )
END_NETWORK_TABLE()

LINK_ENTITY_TO_CLASS( tf_gas_grenade_effect, CTFGasGrenadeEffect );

#ifdef GAME_DLL

	int CTFGasGrenadeEffect::UpdateTransmitState( void )
	{
		return SetTransmitState( FL_EDICT_PVSCHECK );
	}

#else

	void CTFGasGrenadeEffect::OnDataChanged( DataUpdateType_t updateType )
	{
		if ( updateType == DATA_UPDATE_CREATED && m_pGasEffect == NULL )
		{
			m_pGasEffect = ParticleProp()->Create( "spy_gasgrenade", PATTACH_ABSORIGIN );
		}
	}

#endif // CLIENT_DLL