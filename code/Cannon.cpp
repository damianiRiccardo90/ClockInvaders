///////////////////////////  I N C L U D E S  ////////////////////////////

// Header.
#include "Cannon.h"

// Local.
#include "C_Application.h"
#include "graphics.h"
#include "Projectile.h"
#include "Vector2D.h"

//////////////  S T A T I C  M E M B E R  V A R I A B L E S  /////////////

const Vector2D Cannon::s_Default_BBHalfDiag( 25.f, 25.f );
const unsigned int Cannon::s_Default_Color = C_Application::s_Color_Blue;
const float Cannon::s_Default_OffsetFromBottom = 50.f;
const float Cannon::s_Default_AngularVelocity = 1.5f;
const float Cannon::s_Default_BurstFireDelay = 0.05f;
const float Cannon::s_Default_RotationAngleMin = 0.f;
const float Cannon::s_Default_RotationAngleMax = Vector2D::s_PI;

////////////////  F U N C T I O N  D E F I N I T I O N S  ////////////////

Cannon::Cannon( C_Application* owner )
	: Entity( owner, s_Default_Color, s_Default_BBHalfDiag,	owner ? Vector2D( owner->GetScreenWidth() / 2.f, 
		owner->GetScreenHeight() - s_Default_OffsetFromBottom ) : Vector2D::s_Zero )
	, m_AngularVelocity( 0.f )
	, m_BurstFireDelayTimer( 0.f )
	, m_IsFiring( false )
{}

void Cannon::Tick( const float deltaTime )
{
	Entity::Tick( deltaTime );

	if ( m_AngularVelocity )
	{
		m_Facing.ClampRotate( m_AngularVelocity * deltaTime, 
			s_Default_RotationAngleMin, s_Default_RotationAngleMax );
	}

	if ( m_IsFiring )
	{
		UpdateFiring( deltaTime );
	}
}

void Cannon::Render()
{
	const float rotationAngle = 2 * Vector2D::s_PI / 3; // 120 degrees in radians

	// Scaling the facing vector by the BB radius.
	Vector2D radius = m_Facing * s_Default_BBHalfDiag.GetX();

	const Vector2D vertex1 = m_Position + radius;

	// Rotate the radius by 120 degrees twice to obtain the other two vertices
	const Vector2D vertex2 = m_Position + radius.Rotate( rotationAngle );
	const Vector2D vertex3 = m_Position + radius.Rotate( rotationAngle );

	const int xVertex1 = static_cast< int >( vertex1.GetX() );
	const int yVertex1 = static_cast< int >( vertex1.GetY() );
	const int xVertex2 = static_cast< int >( vertex2.GetX() );
	const int yVertex2 = static_cast< int >( vertex2.GetY() );
	const int xVertex3 = static_cast< int >( vertex3.GetX() );
	const int yVertex3 = static_cast< int >( vertex3.GetY() );
	DrawLine( xVertex1, yVertex1, xVertex2, yVertex2, m_Color );
	DrawLine( xVertex2, yVertex2, xVertex3, yVertex3, m_Color );
	DrawLine( xVertex3, yVertex3, xVertex1, yVertex1, m_Color );
}

void Cannon::SetFiring( const bool enable )
{
	if ( m_IsFiring == enable ) return;

	if ( enable )
	{
		// Reset the timer.
		m_BurstFireDelayTimer = s_Default_BurstFireDelay;
	}

	m_IsFiring = enable;
}

void Cannon::UpdateFiring( const float deltaTime )
{
	if ( m_BurstFireDelayTimer <= 0.f )
	{
		Fire();

		// Reset the timer.
		m_BurstFireDelayTimer = s_Default_BurstFireDelay;
	}
	else
	{
		m_BurstFireDelayTimer -= deltaTime;
	}
}

void Cannon::Fire()
{
	if ( !m_Owner ) return;

	// Get muzzle position.
	const Vector2D muzzlePos = m_Position + m_Facing * s_Default_BBHalfDiag.GetX();
	// Get projectile center.
	const Vector2D projPos = muzzlePos + m_Facing * Projectile::s_Default_HalfLength;

	m_Owner->RequestSpawnEntity( Entity::Type::PROJECTILE, projPos, m_Facing );
}
