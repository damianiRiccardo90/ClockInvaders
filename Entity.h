#pragma once

///////////////////////////  I N C L U D E S  ////////////////////////////

// Local.
#include "Vector2D.h"

////////////////  F O R W A R D  D E C L A R A T I O N S  ////////////////

class C_Application;

//////////////////  C L A S S  D E C L A R A T I O N S  //////////////////

// An object that can be rendered on screen.
class Entity
{
public:

	explicit Entity( const C_Application* owner, const unsigned int color, const Vector2D& halfDiagBB, 
		const Vector2D& pos );
	virtual ~Entity() {}

	virtual void Tick( const float deltaTime );
	virtual void Render() = 0;
	virtual bool IsCollidingWith( const Entity& other ) const;

	void SetVelocity( const Vector2D& Velocity ) { m_Velocity = Velocity; }

protected:

	// Avoids going outside of the screen boundaries.
	void ClampPosition();

	const C_Application* m_Owner;
	const unsigned int m_Color;
	// The vector that goes from the center of a rectangle to its lower right vertex, use to define the 
	// bounding box that is required for collision detection.
	const Vector2D m_BBHalfDiagonal;

	// The center of the object.
	Vector2D m_Position;
	// The facing vector.
	Vector2D m_Facing;
	// The vector that represent the current velocity of the object when it is moving.
	Vector2D m_Velocity;
};
