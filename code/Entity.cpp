///////////////////////////  I N C L U D E S  ////////////////////////////

// Header.
#include "Entity.h"

// Local.
#include "C_Application.h"
#include "Vector2D.h"

////////////////  F U N C T I O N  D E F I N I T I O N S  ////////////////

Entity::Entity( const C_Application* owner, const unsigned int color, const Vector2D& halfDiagBB, 
	const Vector2D& pos )
	: m_Owner( owner )
	, m_Color( color )
	, m_BBHalfDiagonal( halfDiagBB.Abs() )
	, m_Position( pos )
	, m_Facing( Vector2D::s_Up )
	, m_Velocity( Vector2D::s_Zero )
{}

void Entity::Tick( const float deltaTime )
{
	// TODO: This might be wrapped inside and UpdatePosition() function.
	// Update the position using the velocity vector, scaled by the frame time.
	m_Position += ( m_Velocity * deltaTime );
	
	ClampPosition();

	// TODO: This might be wrapped inside and UpdateVelocity() function.
	// TODO: Implement (and call here) a function IsCollidingWithScreenEdge() and bounce from the edge.
}

bool Entity::IsCollidingWith( const Entity& other ) const
{
	/*
	const float xBBDiag;
	const float yBBDiag;
	const float xBBDiagOther;
	const float yBBDiagOther;

	// Check horizontal overlap
	if ( m_BBHalfDiagonal.GetX() <= rect2.a.x || rect2.c.x <= rect1.a.x ) {
		return false;
	}

	// Check vertical overlap
	if ( rect1.a.y <= rect2.d.y || rect2.a.y <= rect1.d.y ) {
		return false;
	}
	*/

	return true;
}

void Entity::ClampPosition()
{
	if ( !m_Owner ) return;

	const float xDiagonal = m_BBHalfDiagonal.GetX();
	const float yDiagonal = m_BBHalfDiagonal.GetY();
	const float screenWidth = m_Owner->GetScreenWidth();
	const float screenHeight = m_Owner->GetScreenHeight();

	const float xMin = 0.f + xDiagonal;
	const float xMax = screenWidth - xDiagonal;
	const float yMin = 0.f + yDiagonal;
	const float yMax = screenHeight - yDiagonal;

	m_Position.Clamp( xMin, xMax, yMin, yMax );
}
