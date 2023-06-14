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

	static const Vector2D s_Default_BBHalfDiag;
	static const Vector2D s_Default_MinSize;
	static const unsigned int s_Default_Color;
	static const unsigned int s_Default_HoursHandColor;
	static const unsigned int s_Default_MinutesHandColor;
	static const float s_Default_MinVelocity;
	static const float s_Default_MaxVelocity;
	static const float s_Default_HoursAngleSpan;
	static const float s_Default_MinutesAngleSpan;

	explicit Clock( C_Application* owner, const Vector2D& position = Vector2D::s_ZERO, 
		const Vector2D& velocity = Vector2D::s_ZERO, const Vector2D& halfDiag = k_CLOCK_DEFAULT_BBHALFDIAG );

	// Draw the clock.
	virtual void Render() override;

	// Collisions handling.
	// Split the clock if it's hit by a projectile.
	virtual void HandleCollision( Entity* other ) override;
	// Bounce off the screen by altering the velocity vector.
	virtual void HandleScreenBordersCollision() override;

private:

	// Moves the clock to a random position on screen and gives it a random velocity and facing.
	void Randomize();
	Vector2D GetRandomPosition() const;
	Vector2D GetRandomVelocity() const;
	float GetRandomFloat( const float min, const float max ) const;
	bool GetRandomBool() const;
	// Spawn 2 different new clocks, half the size of the current one, and placed side by side diagonally. Then
	// destroy the current clock.
	void Split();
};
