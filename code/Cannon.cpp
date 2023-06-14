///////////////////////////  I N C L U D E S  ////////////////////////////

// Header.
#include "Cannon.h"

// Local.
#include "C_Application.h"
#include "graphics.h"
#include "Projectile.h"
#include "Vector2D.h"

///////////////////////////  C O N S T A N T S  //////////////////////////

static const Vector2D k_CANNON_DEFAULT_BBHALFDIAG = Vector2D( 25.f, 25.f );
static const unsigned int k_CANNON_DEFAULT_COLOR = Color::k_BLUE;
static const float k_CANNON_DEFAULT_OFFSETFROMBOTTOM = 50.f;
static const float k_CANNON_DEFAULT_BURSTFIREDELAY = 0.001f;
static const float k_CANNON_DEFAULT_MINROTATIONANGLE = 0.f;
static const float k_CANNON_DEFAULT_MAXROTATIONANGLE = k_PI;

////////////////  F U N C T I O N  D E F I N I T I O N S  ////////////////

Cannon::Cannon( C_Application* owner )
	: Entity( owner, k_CANNON_DEFAULT_COLOR, k_CANNON_DEFAULT_BBHALFDIAG, owner ? 
		Vector2D( owner->GetScreenWidth() / 2.f, owner->GetScreenHeight() - k_CANNON_DEFAULT_OFFSETFROMBOTTOM ) : 
		Vector2D::s_ZERO )
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
			k_CANNON_DEFAULT_MINROTATIONANGLE, k_CANNON_DEFAULT_MAXROTATIONANGLE );
	}

	if ( m_IsFiring )
	{
		UpdateFiring( deltaTime );
	}
}

void Cannon::Render()
{
	const float rotationAngle = 2 * k_PI / 3; // 120 degrees in radians

	// Scaling the facing vector by the BB radius.
	Vector2D radius = m_Facing * k_CANNON_DEFAULT_BBHALFDIAG.GetX();

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
		m_BurstFireDelayTimer = k_CANNON_DEFAULT_BURSTFIREDELAY;
	}

	m_IsFiring = enable;
}

void Cannon::UpdateFiring( const float deltaTime )
{
	if ( m_BurstFireDelayTimer <= 0.f )
	{
		Fire();

		// Reset the timer.
		m_BurstFireDelayTimer = k_CANNON_DEFAULT_BURSTFIREDELAY;
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
	const Vector2D muzzlePos = m_Position + m_Facing * k_CANNON_DEFAULT_BBHALFDIAG.GetX();
	// Get projectile center.
	const Vector2D projPos = muzzlePos + m_Facing * k_PROJECTILE_DEFAULT_HALFLENGTH;

	m_Owner->RequestSpawnProjectile( projPos, m_Facing );
}
