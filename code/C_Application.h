#pragma once

///////////////////////////  I N C L U D E S  ////////////////////////////

// System.
#include <memory>
#include <vector>

// Local.
#include "Clock.h"
#include "Entity.h"
#include "graphics.h"

///////////////////////////  C O N S T A N T S  //////////////////////////

namespace Color
{
	static const unsigned int k_RED   = GetRGB( 255, 0,   0 );
	static const unsigned int k_GREEN = GetRGB( 0,   255, 0 );
	static const unsigned int k_BLUE  = GetRGB( 0,   0,   255 );
	static const unsigned int k_BLACK = GetRGB( 0,   0,   0 );
	static const unsigned int k_WHITE = GetRGB( 255, 255, 255 );
}

namespace Key
{
	static const unsigned int k_LEFT  = 0x01;
	static const unsigned int k_UP    = 0x02;
	static const unsigned int k_RIGHT = 0x04;
	static const unsigned int k_DOWN  = 0x08;
	static const unsigned int k_SPACE = 0x10;
}

static const bool k_DEBUGGING_ACTIVE = false;

////////////////  F O R W A R D  D E C L A R A T I O N S  ////////////////

class Cannon;

//////////////////  C L A S S  D E C L A R A T I O N S  //////////////////

class C_Application
{
public:
	
	C_Application( const float screenWidth, const float screenHeight );
	~C_Application() {}

	// Tick is called on fixed framerate (50fps).
	void Tick( const unsigned int pressedKeys, const float deltaTime );

	float GetScreenWidth() const { return m_ScreenWidth; }
	float GetScreenHeight() const { return m_ScreenHeight; }

	// Spawning requests.
	void RequestSpawnCannon();
	void RequestSpawnProjectile( const Vector2D& position, const Vector2D& facing );
	void RequestSpawnClock( const Vector2D& position = Vector2D::s_ZERO, 
		const Vector2D& velocity = Vector2D::s_ZERO, const Vector2D& halfDiag = k_CLOCK_DEFAULT_BBHALFDIAG );

private:

	// Process the input from the keyboard.
	void ProcessInput( const unsigned int pressedKeys );
	// Check the collision between entities and the screen borders, and handle them.
	void CheckEntityCollisions();
	// Make all the entities "tick".
	void UpdateEntities();
	// Spawns all the entities that were requested during this frame.
	void SpawnEntities();
	// Draw every entity on screen.
	void RenderEntities();
	// Iterate over each entity to check if we should remove them from the game.
	void CheckEntityDestruction();

	void ClearScreen();
	void SpawnStartingEntities();

	const float m_ScreenWidth;
	const float m_ScreenHeight;
	const int m_ScreenBackgroundColor;

	float m_DeltaTime;

	// Keeping a separate raw pointer to the cannon entity for accessing it with ease.
	Cannon* m_Cannon;
	// All the entities currently in game.
	std::vector< std::unique_ptr< Entity > > m_Entities;
	// Entities that are waiting to be spawned (avoids invalidating iterators if, for instance, an entity wants to
	// spawn another one while updating).
	std::vector< std::unique_ptr< Entity > > m_ToBeSpawnedEntities;
};
