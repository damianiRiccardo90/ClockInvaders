///////////////////////////  I N C L U D E S  ////////////////////////////

// Header.
#include "Cannon.h"

// Local.
#include "C_Application.h"
#include "graphics.h"
#include "Projectile.h"

//////////////  S T A T I C  M E M B E R  V A R I A B L E S  /////////////

const Vector2D Cannon::s_DEFAULT_BBHALFDIAG = Vector2D( 25.f, 25.f );
const unsigned int Cannon::s_DEFAULT_COLOR = C_Application::GetBlue();
const float Cannon::s_DEFAULT_OFFSETFROMBOTTOM = 50.f;
const float Cannon::s_DEFAULT_BURSTFIREDELAY = 0.001f;
const float Cannon::s_DEFAULT_MINROTATIONANGLE = 0.f;
const float Cannon::s_DEFAULT_MAXROTATIONANGLE = Vector2D::GetPI();
const float Cannon::s_DEFAULT_ANGULARVELOCITY = 1.5f;

////////////////  F U N C T I O N  D E F I N I T I O N S  ////////////////

Cannon::Cannon( C_Application* owner )
	: Entity( owner, s_DEFAULT_COLOR, s_DEFAULT_BBHALFDIAG, owner ? Vector2D( owner->GetScreenWidth() / 2.f, 
		owner->GetScreenHeight() - s_DEFAULT_OFFSETFROMBOTTOM ) : Vector2D::GetZero() )
	, m_AngularVelocity( 0.f )
	, m_BurstFireDelayTimer( 0.f )
	, m_IsFiring( false )
{}

void Cannon::Tick( const float deltaTime )
{
	Entity::Tick( deltaTime );

	if ( m_AngularVelocity )
	{
		m_Facing.ClampRotate( m_AngularVelocity * deltaTime, s_DEFAULT_MINROTATIONANGLE, 
			s_DEFAULT_MAXROTATIONANGLE );
	}

	if ( m_IsFiring )
	{
		UpdateFiring( deltaTime );
	}
}

void Cannon::Render()
{
	Entity::Render();

	const float rotationAngle = 2 * Vector2D::GetPI() / 3; // 120 degrees in radians

	// Scaling the facing vector by the BB radius.
	Vector2D radius = m_Facing * s_DEFAULT_BBHALFDIAG.GetX();

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
		m_BurstFireDelayTimer = s_DEFAULT_BURSTFIREDELAY;
	}

	m_IsFiring = enable;
}

void Cannon::UpdateFiring( const float deltaTime )
{
	if ( m_BurstFireDelayTimer <= 0.f )
	{
		Fire();

		// Reset the timer.
		m_BurstFireDelayTimer = s_DEFAULT_BURSTFIREDELAY;
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
	const Vector2D muzzlePos = m_Position + m_Facing * s_DEFAULT_BBHALFDIAG.GetX();
	// Get projectile center.
	const Vector2D projPos = muzzlePos + m_Facing * Projectile::s_DEFAULT_HALFLENGTH;

	m_Owner->RequestSpawnProjectile( projPos, m_Facing );
}
