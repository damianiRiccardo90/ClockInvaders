#pragma once

///////////////////////////  I N C L U D E S  ////////////////////////////

// Local.
#include "Entity.h"

////////////////  F O R W A R D  D E C L A R A T I O N S  ////////////////

class C_Application;
class Vector2D;

//////////////////  C L A S S  D E C L A R A T I O N S  //////////////////

// The projectile (a line) that the cannon shoots at the enemies (the clocks, basically).
class Projectile : public Entity
{
public:

	static const float s_Default_Size;

	Projectile( const C_Application* owner, const Vector2D& pos, const Vector2D& facing );

	virtual void Render() override;
};
