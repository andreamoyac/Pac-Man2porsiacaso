#pragma once
#include "Entity.h"
#include "TileMap.h"

//Representation model size: 32x32
#define PLAYER_FRAME_SIZE		16

//Logical model size: 12x28
#define PLAYER_PHYSICAL_WIDTH	16
#define PLAYER_PHYSICAL_HEIGHT	16

//Horizontal speed and vertical speed while falling down
#define PLAYER_SPEED			2


//Frame animation delay while on a ladder
#define ANIM_LADDER_DELAY		(2*ANIM_DELAY)



//Logic states
enum class State { IDLE, WALKING, DEAD, SCATTER, CHASE, FRIGHTENED };
enum class Look { RIGHT, LEFT, UP, DOWN };

//Rendering states
enum class PlayerAnim {
	IDLE_LEFT, IDLE_RIGHT, IDLE_UP, IDLE_DOWN,
	WALKING_LEFT, WALKING_RIGHT, WALKING_UP, WALKING_DOWN,
	DEATH,
	TELEPORT_LEFT, TELEPORT_RIGHT,
	NUM_ANIMATIONS
};

class Player: public Entity
{
public:
	Player(const Point& p, State s, Look view);
	~Player();
	
	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void InitScore();
	void IncrScore(int n);
	int GetScore();

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();


private:
	bool IsLookingRight() const;
	bool IsLookingLeft() const;
	bool IsLookingUp() const;
	bool IsLookingDown() const;

	//Player movement
	void MoveX();
	void MoveY();

	//Animations
	void SetAnimation(int id);
	PlayerAnim GetAnimation();
	void Stop();
	void StartWalkingLeft();
	void StartWalkingRight();
	void StartWalkingUp();
	void StartWalkingDown();
	void ChangeAnimRight();
	void ChangeAnimLeft();
	void ChangeAnimUp();
	void ChangeAnimDown();
	void Dead();


	State state;
	Look look;

	TileMap *map;

	int score;
};

/*

----------------------------------------
PACMAN
------------------------------------------
-PACMAN_WIDTH: int
-PACMAN_HEIGHT: int
-x:int
-y:int
-counter:int
-pelletsEaten: int
-xIncrement: int
-yIncrement: int
-characterDirection: String
-hasPower: boolean

---------------------------------------

+Pacman(int, int)
+getX():int
+getY():int
+setX(int): void
+setY(int): void
+setDirection(String):void
+isFacingBlock():boolean
+respawn():void
+colOnMaze():int
+rowOnMaze():int
+incrementPelletsEaten():int
+hasPower(): boolean
+givePower():void
+resetPower():void
+move():void
+moveRight():void
+moveLeft():void
+paintCharacter(Graphics):void
+moveUp():void
+moveDown():void

+checkChangeDirection():boolean
+checkMove():boolean

------------------------------------------------------------------------------
CHARACTER
---------------------------------------
-UP: int
-DOWN:int
-LEFT: int
-RIGHT:int
---------------------------------------
+getX():int
+getY():int
+setX(int): void
+setY(int): void
+setDirection(String):void
+move():void
+isFacingBlock():boolean
+respawn():void
+paintCharacter(Graphics):void

+getSpeed():int
+setSpeed(int):void
+getCurrentDirection():DirectionEnum 
+setCurrentDirection(DirectionEnum):void
+getNextDirection():DirectionEnum
+setNextDirection(DirectionEnum):void
(DirectionEnum=(UP,DOWN,RIGHT,LEFT
------------------------------------------------------------------------------
GHOST
------------------------------------------
-GHOST_WIDTH: int
-GHOST_HEIGHT: int
-x:int
-y:int
-characterDirection: String

-colour: String
-name:String

---------------------------------------

+Ghost(int, int)
+getX():int
+getY():int
+setX(int): void
+setY(int): void
+setDirection(String):void
+getDirection():String
+isFacingBlock():boolean
+isAtIntersection():boolean
+allowableDirectionsAtIntersection:Arrayist<String>
+reverse(): void
+respawn():void
+paintCharacter(Graphics):void


+isVulnerable():boolean
+setVulnerable(boolean):void
+isDead():boolean
setDead(boolean):void

------------------------------------------------------------------------------
MAZEBLOCK
------------------------------------------
-BLOCK_WIDTH: int
-BLOCK_HEIGHT: int
-x:int
-y:int

---------------------------------------
+MazeBlock(int, int)
+paintBlock(Graphics):void

-isWall: Boolean
-isDot:Boolean
-isFood:Boolean
-isDoorway:Boolean


+setWall():void
+setDot():void
+setDoorway():void
+setFood():void

------------------------------------------------------------------------------
PELLETS
------------------------------------------
-PELLET_WIDTH: int
-PELLET_HEIGHT: int
-x: int
-y:int
------------------------------------------
+Pellet(int, int)
+paintPellet(Graphics):void

------------------------------------------------------------------------------
POWERUP
------------------------------------------
+Powerup(int, int)
+paintPellet(Graphics):void

------------------------------------------------------------------------------
GAMEBOARD
------------------------------------------
-titleScreenImage: Image
-endGameImage: Image
-winScreenImage: Image
-pacman: Pacman
-ghosts: Ghost[]
-score: int
-numLives: int
-mazeBlockPositions: int[][]
-mazeBlocks: MazeBlock[][]
-tempPellets:Pellet[][]
-showTitleScreen: boolean
-showWinScreen: boolean
-showEndGameScreen: boolean
-initiateNewGame: int
-pacmanDying: int
-timer:long
-font: Font
-font2: Font


-foodEat: int
-allFood:int
----------------------------------
+GameBoard()
+reset():void
+repositionCharacters():void
+paintComponent(Graphics):void
+collided():boolean
+pacmanCanEatGhost():int
+eatGhost():void
+decrementLives():void
+getLives():int
+eatPellets(): void
+incrementScore(int): void
+getScore():int

+noMoreFood(TileEnim[][]):boolean
*/