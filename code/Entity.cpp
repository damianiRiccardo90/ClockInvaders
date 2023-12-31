///////////////////////////  I N C L U D E S  ////////////////////////////

// Header.
#include "Entity.h"

// Local.
#include "C_Application.h"
#include "graphics.h"

////////////////  F U N C T I O N  D E F I N I T I O N S  ////////////////

Entity::Entity( C_Application* owner, const unsigned int color, const Vector2D& halfDiagBB, 
	const Vector2D& position, const Vector2D& facing /*= Vector2D::GetUp()*/, 
	const Vector2D& velocity /*= Vector2D::GetZero()*/ )
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
	return IsOverlapping( other );
}

void Entity::HandleCollision( Entity* other )
{
	if ( !other ) return;

	if ( IsOverlapping( *other ) ) ApplyMTV( *other );
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

bool Entity::IsOverlapping( const Entity& other ) const
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
	const Vector2D lowerRightVertex = Vector2D( xPos + xBBDiag, yPos + yBBDiag );
	const Vector2D lowerRightVertexOther = Vector2D( xPosOther + xBBDiagOther, yPosOther + yBBDiagOther );
	// Upper left vertices (from this Entity, and the other one).
	const Vector2D upperLeftVertex = Vector2D( xPos - xBBDiag, yPos - yBBDiag );
	const Vector2D upperLeftVertexOther = Vector2D( xPosOther - xBBDiagOther, yPosOther - yBBDiagOther );

	// Check if the horizontal ranges do not overlap.
	if ( lowerRightVertex.GetX() < upperLeftVertexOther.GetX() || 
		lowerRightVertexOther.GetX() < upperLeftVertex.GetX() )
	{
		return false;
	}

	// Check if the vertical ranges do not overlap.
	if ( lowerRightVertex.GetY() < upperLeftVertexOther.GetY() || 
		lowerRightVertexOther.GetY() < upperLeftVertex.GetY() )
	{
		return false;
	}

	return true;
}

void Entity::ApplyMTV( Entity& other )
{
	const float xPos = m_Position.GetX();
	const float yPos = m_Position.GetY();
	const float xPosOther = other.m_Position.GetX();
	const float yPosOther = other.m_Position.GetY();

	const float xBBDiag = m_BBHalfDiagonal.GetX();
	const float yBBDiag = m_BBHalfDiagonal.GetY();
	const float xBBDiagOther = other.m_BBHalfDiagonal.GetX();
	const float yBBDiagOther = other.m_BBHalfDiagonal.GetY();

	// Calculate the overlap along the x-axis.
	const float xOverlap = ( xBBDiag + xBBDiagOther ) - std::abs( xPos - xPosOther );

	// Calculate the overlap along the y-axis.
	const float yOverlap = ( yBBDiag + yBBDiagOther ) -	std::abs( yPos - yPosOther );

	// Determine the direction of the MTV.
	const float xMTVDir = ( xPos < xPosOther ) ? -1.f : 1.f;
	const float yMTVDir = ( yPos < yPosOther ) ? -1.f : 1.f;

	// Choose the smaller overlap axis.
	if ( std::abs( xOverlap ) < std::abs( yOverlap ) )
	{
		// Apply the MTV along the x-axis, halved, to both entities (mitigates snapping).
		const float xMTVHalf = xMTVDir * xOverlap / 2;
		m_Position.SetX( xPos + xMTVHalf );
		other.m_Position.SetX( xPosOther - xMTVHalf );
	}
	else
	{
		// Apply the MTV along the y-axis, halved, to both entities (mitigates snapping).
		const float yMTVHalf = yMTVDir * yOverlap / 2;
		m_Position.SetY( yPos + yMTVHalf );
		other.m_Position.SetY( yPosOther - yMTVHalf );
	}
}
