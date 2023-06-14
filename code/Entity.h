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

	Entity( C_Application* owner, const unsigned int color, const Vector2D& halfDiagBB, const Vector2D& position, 
		const Vector2D& facing = Vector2D::s_UP, const Vector2D& velocity = Vector2D::s_ZERO );
	virtual ~Entity() {}

	virtual void Tick( const float deltaTime );
	virtual void Render() = 0;

	// Collisions handling.
	virtual bool IsCollidingWith( const Entity& other ) const;
	virtual void HandleCollision( Entity* other );
	virtual bool IsCollidingWithScreenBorders() const;
	virtual bool IsCollidingWithScreenHorizontally() const;
	virtual bool IsCollidingWithScreenVertically() const;
	virtual void HandleScreenBordersCollision() {}

	void SetPosition( const Vector2D& pos ) { m_Position = pos; }
	Vector2D GetPosition() const { return m_Position; }
	void SetFacing( const Vector2D& vec ) { m_Facing = vec; }
	Vector2D GetFacing() const { return m_Facing; }
	void SetVelocity( const Vector2D& vel ) { m_Velocity = vel; }
	Vector2D GetVelocity() const { return m_Velocity; }

	void Destroy() { m_IsPendingDestruction = true; }
	bool IsPendingDestruction() const { return m_IsPendingDestruction; }

	// Debugging.
	void RenderBB();

protected:

	// Position handling.
	void UpdatePosition( const float deltaTime );

	// Collisions handling.
	// Avoids going outside of the screen boundaries.
	void ClampPosition();
	// Detects overlapping.
	bool IsOverlapping( const Entity& other ) const;
	// Avoid overlapping between entities.
	void ApplyMTV( Entity& other );

	C_Application* m_Owner;
	const unsigned int m_Color;
	// The vector that goes from the center of a rectangle to its lower right vertex, used to define the 
	// bounding box that is required for collision detection (and it's handy for other computations as well).
	const Vector2D m_BBHalfDiagonal;

	// The center of the object.
	Vector2D m_Position;
	// The facing vector.
	Vector2D m_Facing;
	// The vector that represent the current velocity of the object when it is moving.
	Vector2D m_Velocity;

	bool m_IsPendingDestruction;
};
