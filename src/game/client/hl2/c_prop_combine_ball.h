//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//
#ifndef CPROPCOMBINEBALL_H_
#define CPROPCOMBINEBALL_H_

#ifdef _WIN32
#pragma once
#endif

#include "baseprojectile.h"

class C_PropCombineBall : public C_BaseProjectile
{
	DECLARE_CLASS( C_PropCombineBall, C_BaseProjectile );
	DECLARE_CLIENTCLASS();
public:

	C_PropCombineBall( void );

	virtual RenderGroup_t GetRenderGroup( void );

	virtual void	OnDataChanged( DataUpdateType_t updateType );
	virtual int		DrawModel( int flags );
	CNetworkVar( int, m_iDeflected );
protected:

	void	DrawMotionBlur( void );
	void	DrawFlicker( void );
	virtual bool	InitMaterials( void );

	Vector	m_vecLastOrigin;
	bool	m_bEmit;
	float	m_flRadius;
	bool	m_bHeld;
	bool	m_bLaunched;

	IMaterial	*m_pFlickerMaterial;
	IMaterial	*m_pBodyMaterial;
	IMaterial	*m_pBlurMaterial;
};


#endif