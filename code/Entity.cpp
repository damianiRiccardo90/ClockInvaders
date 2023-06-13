///////////////////////////  I N C L U D E S  ////////////////////////////

// Header.
#include "Entity.h"

// Local.
#include "C_Application.h"
#include "Vector2D.h"

////////////////  F U N C T I O N  D E F I N I T I O N S  ////////////////

Entity::Entity( C_Application* owner, const unsigned int color, const Vector2D& halfDiagBB, 
	const Vector2D& position, const Vector2D& facing /*= Vector2D::s_Up*/, 
	const Vector2D& velocity /*= Vector2D::s_Zero*/ )
	: m_Owner( owner )
	, m_Color( color )
	, m_BBHalfDiagonal( halfDiagBB.Abs() )
	, m_Position( position )
	, m_Facing( facing )
	, m_Velocity( velocity )
	, m_IsPendingDestruction( false )
{}

void Entity::Tick( const float deltaTime )
{
	UpdatePosition( deltaTime );
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
	const Vector2D lowRtVt = Vector2D( xPos + xBBDiag, yPos + yBBDiag );
	const Vector2D lowRtVtOther = Vector2D( xPosOther + xBBDiagOther, yPosOther + yBBDiagOther );
	// Upper left vertices (from this Entity, and the other one).
	const Vector2D upLtVt = Vector2D( xPos - xBBDiag, yPos - yBBDiag );
	const Vector2D upLtVtOther = Vector2D( xPosOther - xBBDiagOther, yPosOther - yBBDiagOther );

	// Check if the horizontal ranges do not overlap.
	if ( lowRtVt.GetX() < upLtVtOther.GetX() || lowRtVtOther.GetX() < upLtVt.GetX() )
	{
		return false;
	}

	// Check if the vertical ranges do not overlap.
	if ( upLtVt.GetY() < lowRtVtOther.GetY() || upLtVtOther.GetY() < lowRtVt.GetY() )
	{
		return false;
	}

	return true;
}

bool Entity::IsCollidingWithScreenBorders() const
{
	return IsCollidingWithScreenHorizontally() || IsCollidingWithScreenVertically();
}

bool Entity::IsCollidingWithScreenHorizontally() const
{
	if ( !m_Owner ) return false;

	const float xBBDiag = m_BBHalfDiagonal.GetX();
	const float screenWidth = m_Owner->GetScreenWidth();
	const float xMin = 0.f + xBBDiag;
	const float xMax = screenWidth - xBBDiag;
	const float xPos = m_Position.GetX();

	return ( xPos >= xMax ) || ( xPos <= xMin );
}

bool Entity::IsCollidingWithScreenVertically() const
{
	if ( !m_Owner ) return false;

	const float yBBDiag = m_BBHalfDiagonal.GetY();
	const float screenHeight = m_Owner->GetScreenHeight();
	const float yMin = 0.f + yBBDiag;
	const float yMax = screenHeight - yBBDiag;
	const float yPos = m_Position.GetY();

	return ( yPos >= yMax ) || ( yPos <= yMin );
}

void Entity::UpdatePosition( const float deltaTime )
{
	// Update the position using the velocity vector, scaled by the frame time.
	m_Position += ( m_Velocity * deltaTime );

	ClampPosition();
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
