#pragma once

///////////////////////////  I N C L U D E S  ////////////////////////////

// Local.
#include "Entity.h"

///////////////////////////  C O N S T A N T S  //////////////////////////

static const Vector2D k_CLOCK_DEFAULT_BBHALFDIAG = Vector2D( 50.f, 50.f );

////////////////  F O R W A R D  D E C L A R A T I O N S  ////////////////

class C_Application;

//////////////////  C L A S S  D E C L A R A T I O N S  //////////////////

// A clock that shows the current time, and splits into smaller versions of itself when hit by projectiles.
class Clock : public Entity
{
public:

	explicit Clock( C_Application* owner, const Vector2D& position = Vector2D::GetZero(), 
		const Vector2D& velocity = Vector2D::GetZero(), const Vector2D& halfDiag = s_DEFAULT_BBHALFDIAG );

	// Draw the clock.
	virtual void Render() override;

	// Collisions handling.
	// Split the clock if it's hit by a projectile.
	virtual void HandleCollision( Entity* other ) override;
	// Bounce off the screen by altering the velocity vector.
	virtual void HandleScreenBordersCollision() override;

	static const Vector2D s_DEFAULT_BBHALFDIAG;
	static const Vector2D s_DEFAULT_MINHALFDIAG;
	static const unsigned int s_DEFAULT_COLOR;
	static const unsigned int s_DEFAULT_HOURSHANDCOLOR;
	static const unsigned int s_DEFAULT_MINUTESHANDCOLOR;
	static const float s_DEFAULT_MINVELOCITY;
	static const float s_DEFAULT_MAXVELOCITY;
	static const float s_DEFAULT_HOURSANGLESPAN;
	static const float s_DEFAULT_MINUTESANGLESPAN;

private:

	// Moves the clock to a random position on screen and gives it a random velocity and facing.
	void Randomize();
	// Compute a random position within the screen range boundaries.
	Vector2D GetRandomPosition() const;
	// Compute a velocity vector that has a random rotation and a random magnitude chosen within the default range.
	Vector2D GetRandomVelocity() const;

	// Spawn 2 different new clocks, half the size of the current one, and placed side by side diagonally. Then
	// destroy the current clock.
	void Split();
};
