#pragma once

///////////////////////////  I N C L U D E S  ////////////////////////////

// System.
#include <memory>
#include <vector>

// Local.
#include "Clock.h"
#include "Entity.h"

////////////////  F O R W A R D  D E C L A R A T I O N S  ////////////////

class Cannon;

//////////////////  C L A S S  D E C L A R A T I O N S  //////////////////

class C_Application
{
public:

	static unsigned int GetRed();
	static unsigned int GetGreen();
	static unsigned int GetBlue();
	static unsigned int GetBlack();
	static unsigned int GetWhite();
	
	C_Application( const float screenWidth, const float screenHeight );
	~C_Application() {}

	// Tick is called on fixed framerate (50fps).
	void Tick( const unsigned int pressedKeys, const float deltaTime );

	float GetScreenWidth() const { return static_cast< float >( m_ScreenWidth ); }
	float GetScreenHeight() const { return static_cast< float >( m_ScreenHeight ); }

	// Spawning requests.
	void RequestSpawnCannon();
	void RequestSpawnProjectile( const Vector2D& position, const Vector2D& facing );
	void RequestSpawnClock( const Vector2D& position = Vector2D::GetZero(), 
		const Vector2D& velocity = Vector2D::GetZero(), const Vector2D& halfDiag = Clock::s_DEFAULT_BBHALFDIAG );

	static const unsigned int s_LEFT;
	static const unsigned int s_UP;
	static const unsigned int s_RIGHT;
	static const unsigned int s_DOWN;
	static const unsigned int s_SPACE;

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
	// Check that there are still clocks on screen, if not, respawns the first two big clocks.
	void CheckRestart();

	const float m_ScreenWidth;
	const float m_ScreenHeight;
	const unsigned int m_ScreenBackgroundColor;

	float m_DeltaTime;

	// Keeping a separate raw pointer to the cannon entity for accessing it with ease.
	Cannon* m_Cannon;
	// All the entities currently in game.
	std::vector< std::unique_ptr< Entity > > m_Entities;
	// Entities that are waiting to be spawned (avoids invalidating iterators if, for instance, an entity wants to
	// spawn another one while updating).
	std::vector< std::unique_ptr< Entity > > m_ToBeSpawnedEntities;
};
