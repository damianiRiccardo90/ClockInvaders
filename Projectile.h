#pragma once

///////////////////////////  I N C L U D E S  ////////////////////////////

// Local.
#include "Entity.h"

////////////////  F O R W A R D  D E C L A R A T I O N S  ////////////////

class C_Application;
class Vector2D;

//////////////////  C L A S S  D E C L A R A T I O N S  //////////////////

// The cannon that the player can use to shoot at enemies.
class Projectile : public Entity
{
public:

	static const Vector2D s_Default_BBHalfDiag;

	Projectile( const C_Application* owner, const Vector2D& pos );

	virtual void Tick( const float deltaTime ) override;
	virtual void Render() override;
};
