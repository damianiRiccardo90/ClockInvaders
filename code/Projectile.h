#pragma once

///////////////////////////  I N C L U D E S  ////////////////////////////

// Local.
#include "Entity.h"

///////////////////////////  C O N S T A N T S  //////////////////////////

static const float k_PROJECTILE_DEFAULT_HALFLENGTH = 3.f;

////////////////  F O R W A R D  D E C L A R A T I O N S  ////////////////

class C_Application;

//////////////////  C L A S S  D E C L A R A T I O N S  //////////////////

// The projectile (a line) that the cannon shoots at the enemies (the clocks, basically).
class Projectile : public Entity
{
public:

	Projectile( C_Application* owner, const Vector2D& position, const Vector2D& facing );

	// Draw a line.
	virtual void Render() override;

	// Collisions handling.
	// Destroy the projectile when it hits the screen borders.
	virtual void HandleScreenBordersCollision() override;
};
