#pragma once

///////////////////////////  I N C L U D E S  ////////////////////////////

// Local.
#include "Entity.h"

//////////////////  C L A S S  D E C L A R A T I O N S  //////////////////

// The projectile (a line) that the cannon shoots at the enemies (the clocks, basically).
class Projectile : public Entity
{
public:

	Projectile( C_Application* owner, const Vector2D& position, const Vector2D& facing );

	// Draw a line.
	virtual void Render() override;

	// Collisions handling.
	// Destroy the projectile when it hits the screen borders, or a clock entity.
	virtual void HandleCollision( Entity* other ) override;
	virtual void HandleScreenBordersCollision() override;

	static const unsigned int s_DEFAULT_COLOR;
	static const float s_DEFAULT_VELOCITY;
	static const float s_DEFAULT_HALFLENGTH;
};
