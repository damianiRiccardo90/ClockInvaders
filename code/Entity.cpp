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
	const float xPos = m_Position.GetX();
	const float yPos = m_Position.GetY();
	const float xPosOther = other.m_Position.GetX();
	const float yPosOther = other.m_Position.GetY();

	const float xBBDiag = m_BBHalfDiagonal.GetX();
	const float yBBDiag = m_BBHalfDiagonal.GetY();
	const float xBBDiagOther = other.m_BBHalfDiagonal.GetX();
	const float yBBDiagOther = other.m_BBHalfDiagonal.GetY();

	// Lower right vertices (from this Entity, and the other one).
	const Vector2D lowRxVx = Vector2D( xPos + xBBDiag, yPos + yBBDiag );
	const Vector2D lowRxVxOther = Vector2D( xPosOther + xBBDiagOther, yPosOther + yBBDiagOther );
	// Upper left vertices (from this Entity, and the other one).
	const Vector2D upLxVx = Vector2D( xPos - xBBDiag, yPos - yBBDiag );
	const Vector2D upLxVxOther = Vector2D( xPosOther - xBBDiagOther, yPosOther - yBBDiagOther );

	// Check if the horizontal ranges do not overlap.
	if ( lowRxVx.GetX() < upLxVxOther.GetX() || lowRxVxOther.GetX() < upLxVx.GetX() )
	{
		return false;
	}

	// Check if the vertical ranges do not overlap.
	if ( upLxVx.GetY() < lowRxVxOther.GetY() || upLxVxOther.GetY() < lowRxVx.GetY() )
	{
		return false;
	}

	return true;
}

bool Entity::IsCollidingWithScreenBorders() const
{
	if ( !m_Owner ) return false;

	const float xBBDiag = m_BBHalfDiagonal.GetX();
	const float screenWidth = m_Owner->GetScreenWidth();
	const float xMin = 0.f + xBBDiag;
	const float xMax = screenWidth - xBBDiag;
	const float xPos = m_Position.GetX();

	const bool isCollidingHorizontally = ( xPos + xBBDiag >= xMax ) || ( xPos - xBBDiag <= xMin );
	if ( isCollidingHorizontally ) return true;

	const float yBBDiag = m_BBHalfDiagonal.GetY();
	const float screenHeight = m_Owner->GetScreenHeight();
	const float yMin = 0.f + yBBDiag;
	const float yMax = screenHeight - yBBDiag;
	const float yPos = m_Position.GetY();

	const bool isCollidingVertically = ( yPos + yBBDiag >= yMax ) || ( yPos - yBBDiag <= yMin );
	if ( isCollidingVertically ) return true;

	return false;
}

void Entity::ClampPosition()
{
	if ( !m_Owner ) return;

	const float xBBDiag = m_BBHalfDiagonal.GetX();
	const float yBBDiag = m_BBHalfDiagonal.GetY();
	const float screenWidth = m_Owner->GetScreenWidth();
	const float screenHeight = m_Owner->GetScreenHeight();

	const float xMin = 0.f + xBBDiag;
	const float xMax = screenWidth - xBBDiag;
	const float yMin = 0.f + yBBDiag;
	const float yMax = screenHeight - yBBDiag;

	m_Position.Clamp( xMin, xMax, yMin, yMax );
}
