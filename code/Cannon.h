#pragma once

///////////////////////////  I N C L U D E S  ////////////////////////////

// Local.
#include "Entity.h"

////////////////  F O R W A R D  D E C L A R A T I O N S  ////////////////

class C_Application;
class Vector2D;

//////////////////  C L A S S  D E C L A R A T I O N S  //////////////////

// The cannon that the player can use to shoot at enemies.
class Cannon : public Entity
{
public:

	static const Vector2D s_Default_BBHalfDiag;
	static const float s_Default_OffsetFromBottom;
	static const float s_Default_AngularVelocity;
	static const float s_Default_BurstFireDelay;
	static const float s_Default_RotationAngleMin;
	static const float s_Default_RotationAngleMax;


	Cannon( C_Application* owner );

	virtual void Tick( const float deltaTime ) override;
	// Draw an equilateral triangle.
	virtual void Render() override;

	// No entity can collide with the cannon.
	virtual bool IsCollidingWith( const Entity& other ) const override { return false; }
	// This entity's position is fixed so it makes no sense to check if it collides with the screen borders.
	virtual bool IsCollidingWithScreenBorders() const override { return false; }

	void SetAngularVelocity( const float angularVelocity ) { m_AngularVelocity = angularVelocity; }
	
	// Enables/disables burst firing.
	void SetFiring( const bool enable );

private:

	// Handle firing.
	void UpdateFiring( const float deltaTime );
	// Fire a projectile.
	void DoFire();

	float m_AngularVelocity;
	float m_BurstFireDelayTimer;
	bool m_IsFiring;
};
