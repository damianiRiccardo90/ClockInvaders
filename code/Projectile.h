#pragma once

///////////////////////////  I N C L U D E S  ////////////////////////////

// Local.
#include "Entity.h"

////////////////  F O R W A R D  D E C L A R A T I O N S  ////////////////

class C_Application;

//////////////////  C L A S S  D E C L A R A T I O N S  //////////////////

// The projectile (a line) that the cannon shoots at the enemies (the clocks, basically).
class Projectile : public Entity
{
public:

	static const float s_Default_HalfLength;
	static const unsigned int s_Default_Color;
	static const float s_Default_Velocity;

	Projectile( C_Application* owner, const Vector2D& position, const Vector2D& facing );

	// Draw a line.
	virtual void Render() override;

	// Collisions handling.
	// Destroy the projectile when it hits the screen borders.
	virtual void HandleScreenBordersCollision() override;
};
