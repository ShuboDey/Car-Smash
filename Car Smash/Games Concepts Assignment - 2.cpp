// Games Concepts Assignment - 2.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <sstream>
#include <iostream>


using namespace std; 
using namespace tle;

struct GameModels 
{
	IModel* model; 
};
struct enemyCar 
{
	bool isBallRed = false; 
};
struct Vector3
{
	float x = 0;
	float y = 0;
	float z = 0;
}; 

const float kCamXPos = 0;
const float kCamYPos = 15.0f;
const float kCamZPos = -60.0f;
const float kCamRotationValue = 15.0f;

const float kSkyXPos = 0; 
const float kSkyYPos = -960.0f;
const float kSkyZPos = 0;

const float staticEnemy0XPos = -20.0f;
const float staticEnemy0ZPos = 20.0f;
const float staticEnemy1XPos = 20.0f;
const float staticEnemy1ZPos = 20.0f;
const float staticEnemy2XPos = -20.0f;
const float staticEnemy2ZPos = 0;
const float staticEnemy3XPos = 20.0f;
const float staticEnemy3ZPos = 0;

const float movingEnemy0XPos = -30.0f;
const float movingEnemy0ZPos = 15.0f;
const float movingEnemy1XPos = 30.0f;
const float movingEnemy1ZPos = 30.0f;
const float movingEnemy2XPos = -30.0f;
const float movingEnemy2ZPos = -30.0f;
const float movingEnemy3XPos = 30.0f;
const float movingEnemy3ZPos = -15.0f;
const float movingEnemiesRotationValue = 90.0f;

const float ballYPos = 2.5f;

const int kTrees = 160;
const float kTreeDistance = 360.0f / 160.0f;
const float kRadius = 50.0f;
const float kPi = 3.14159f; 
const float treeYPos = 0;

const float kSpeed = 5.0f; 
const float kRotationSpeed = 25.0f; 
const float kDragFactor = -0.0004f; 

const EKeyCode kQuitProgram = EKeyCode::Key_Escape;
const EKeyCode kPauseButton = EKeyCode::Key_P;

const float kCam2Y = 3.15f;
const float kCam2Z = 5.09f;
const float kCam3X = 0.002f;
const float kCam3Y = 1.51f;
const float kCam3Z = 2.09f;

enum CollisionAxis { xAxis, zAxis, none };

CollisionAxis CheckPlayerToEnemyCarCollision(GameModels pCars, GameModels eCars, Vector3 old)
{
	const float kEnemyCarMinX = -0.94f;
	const float kEnemyCarMinY = 0.00f;
	const float kEnemyCarMinZ = -1.97f;

	const float kEnemyCarMaxX = 0.94f;
	const float kEnemyCarMaxY = 1.50f;
	const float kEnemyCarMaxZ = 1.97f;

	const float boxWidthX = kEnemyCarMaxX - kEnemyCarMinX;
	const float boxWidthY = kEnemyCarMaxY - kEnemyCarMinY;
	const float boxWidthZ = kEnemyCarMaxZ - kEnemyCarMinZ;

	const float kboxRadiusX = boxWidthX / 2;
	const float kboxRadiusY = boxWidthY / 2;
	const float kboxRadiusZ = boxWidthZ / 2;

	float playerCarXCoordinate = pCars.model->GetX();
	float playerCarYCoordinate = pCars.model->GetY();
	float playerCarZCoordinate = pCars.model->GetZ();

	float enemyCarXCoordinate = eCars.model->GetX();
	float enemyCarYCoordinate = eCars.model->GetY();
	float enemyCarZCoordinate = eCars.model->GetZ();

	const float kRadius = 1.97f;

	float distanceX = playerCarXCoordinate - enemyCarXCoordinate;
	float distanceY = playerCarYCoordinate - enemyCarYCoordinate;
	float distanceZ = playerCarZCoordinate - enemyCarZCoordinate;
	float distance = sqrt(distanceX * distanceX + distanceY * distanceY + distanceZ * distanceZ);

	if (distance > kRadius + kboxRadiusX)
	{
		return none;;
	}
	if (playerCarXCoordinate < enemyCarXCoordinate + kboxRadiusX + kRadius && playerCarXCoordinate > enemyCarXCoordinate - kboxRadiusX - kRadius &&
		playerCarYCoordinate < enemyCarYCoordinate + kboxRadiusY + kRadius && playerCarYCoordinate > enemyCarYCoordinate - kboxRadiusY - kRadius &&
		playerCarZCoordinate < enemyCarZCoordinate + kboxRadiusZ + kRadius && playerCarZCoordinate > enemyCarZCoordinate - kboxRadiusZ - kRadius)
	{
		if (old.x > kEnemyCarMinX && old.x < kEnemyCarMaxX)
		{
			return xAxis;
		}
		else if (old.z > kEnemyCarMinZ && old.z < kEnemyCarMaxZ)
		{
			return zAxis;
		}
	}
	return none;
}

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media" );

	/**** Set up your scene here ****/

	ICamera* myCamera;
	myCamera = myEngine->CreateCamera(kManual, kCamXPos, kCamYPos, kCamZPos);
	myCamera->RotateLocalX(kCamRotationValue);

	IMesh* groundMesh;
	groundMesh = myEngine->LoadMesh("ground.x");
	IModel* groundModel;
	groundModel = groundMesh->CreateModel();

	IMesh* skyMesh;
	skyMesh = myEngine->LoadMesh("skybox01.x");
	IModel* skyModel;
	skyModel = skyMesh->CreateModel(kSkyXPos, kSkyYPos, kSkyZPos); 

	IMesh* treeMesh;
	treeMesh = myEngine->LoadMesh("Tree.x");
	GameModels trees[kTrees];
	for (int i = 0; i < kTrees; i++)
	{
		float degrees = i * kTreeDistance;
		float radians = degrees * kPi / 180.0f;

		float x = kRadius * cos(radians);
		float z = kRadius * sin(radians);

		trees[i].model = treeMesh->CreateModel(x, treeYPos, z);
	}

	IMesh* playerCarMesh; 
	playerCarMesh = myEngine->LoadMesh("4x4jeep.x"); 
	GameModels playerCar; 
	playerCar.model = playerCarMesh->CreateModel(); 

	IMesh* enemyCarMesh; 
	enemyCarMesh = myEngine->LoadMesh("audi.x");  
	GameModels staticEnemies[4]; 
	for (int i = 0; i < 4; i++) 
	{
		staticEnemies[i].model = enemyCarMesh->CreateModel(); 
	}
	staticEnemies[0].model->SetPosition(staticEnemy0XPos, 0, staticEnemy0ZPos); 
	staticEnemies[1].model->SetPosition(staticEnemy1XPos, 0, staticEnemy1ZPos); 
	staticEnemies[2].model->SetPosition(staticEnemy2XPos, 0, staticEnemy2ZPos); 
	staticEnemies[3].model->SetPosition(staticEnemy3XPos, 0, staticEnemy3ZPos);

	IMesh* staticBallMesh; 
	staticBallMesh = myEngine->LoadMesh("ball.x"); 
	GameModels staticEnemyBalls[4]; 
	for (int i = 0; i < 4; i++) 
	{
		staticEnemyBalls[i].model = staticBallMesh->CreateModel(0, ballYPos, 0); 
		staticEnemyBalls[i].model->AttachToParent(staticEnemies[i].model); 
	}

	IMesh* enemiesCarMesh;
	enemiesCarMesh = myEngine->LoadMesh("estate.x");
	GameModels movingEnemies[4];
	for (int i = 0; i < 4; i++)
	{
		movingEnemies[i].model = enemiesCarMesh->CreateModel();
	}

	movingEnemies[0].model->SetPosition(movingEnemy0XPos, 0, movingEnemy0ZPos);
	movingEnemies[0].model->RotateY(movingEnemiesRotationValue);
	float moving0xCoordinate = 0;

	movingEnemies[1].model->SetPosition(movingEnemy1XPos, 0, movingEnemy1ZPos);
	movingEnemies[1].model->RotateY(-movingEnemiesRotationValue);
	float moving1xCoordinate = 0;

	movingEnemies[2].model->SetPosition(movingEnemy2XPos, 0, movingEnemy2ZPos);
	movingEnemies[2].model->RotateY(movingEnemiesRotationValue);
	float moving2xCoordinate = 0;

	movingEnemies[3].model->SetPosition(movingEnemy3XPos, 0, movingEnemy3ZPos);
	movingEnemies[3].model->RotateY(-movingEnemiesRotationValue);
	float moving3xCoordinate = 0;

	IMesh* movingBallMesh;
	movingBallMesh = myEngine->LoadMesh("ball.x");
	GameModels movingEnemyBalls[4];
	for (int i = 0; i < 4; i++)
	{
		movingEnemyBalls[i].model = movingBallMesh->CreateModel(0, ballYPos, 0);
		movingEnemyBalls[i].model->AttachToParent(movingEnemies[i].model);
	}

	float frameTime; 
	frameTime = myEngine->Timer();

	bool isGamePaused = false; 
	bool cameraIsAttached = false; 
	bool isGameOver = false; 

	enemyCar ballstate[4]; 
	for (int i = 0; i < 4; i++) 
	{
		ballstate[i].isBallRed = false; 
	}

	Vector3 momentum, oldMomentum, thrust, drag; 

	ISprite* backdrop; 
	backdrop = myEngine->CreateSprite("Backdrop.jpg", 300.0f, 660.0f); 

	IFont* myFont; 
	myFont = myEngine->LoadFont("Monotype Corsiva", 50); 

	int numRedBalls = 0;
	int totalEnemyBalls = 4;
	int score = 0; 
	int fixedScore = 10; 
	bool enemyHit[4] = { false, false, false, false };

	float speed = 5.0f;
	float ballSpeed = 0.005f;
	float movingBallYCoordinate = 0;

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		frameTime = myEngine->Timer(); 

		/**** Update your scene each frame here ****/

		float prevX = playerCar.model->GetX(); 
		float prevY = playerCar.model->GetY(); 
		float prevZ = playerCar.model->GetZ(); 
		 
	

		if (myEngine->KeyHit(kQuitProgram)) 
		{
			myEngine->Stop(); 
		}
		if (isGamePaused == false) 
		{
			for (int i = 0; i < 4; i++)
			{
				if (ballstate[i].isBallRed == false) 
				{
					if (myEngine->KeyHit(Key_1))
					{
						myCamera->DetachFromParent();
						myCamera->SetPosition(kCamXPos, kCamYPos, kCamZPos);
					}
					else if (myEngine->KeyHit(Key_2))
					{
						if (cameraIsAttached == false)
						{
							myCamera->AttachToParent(playerCar.model);
							myCamera->SetPosition(playerCar.model->GetX(), playerCar.model->GetY() + kCam2Y, playerCar.model->GetZ() - kCam2Z); 
							cameraIsAttached = true;
						}
						else if (cameraIsAttached == true)
						{
							myCamera->DetachFromParent();
							myCamera->SetPosition(kCamXPos, kCamYPos, kCamZPos);
							cameraIsAttached = false;
						}
					}
					else if (myEngine->KeyHit(Key_3))
					{
						if (cameraIsAttached == false)
						{
							myCamera->AttachToParent(playerCar.model);
							myCamera->SetPosition(playerCar.model->GetX() - kCam3X, playerCar.model->GetY() + kCam3Y, playerCar.model->GetZ() + kCam3Z);
							cameraIsAttached = true;
						}
						else if (cameraIsAttached == true)
						{
							myCamera->DetachFromParent();
							myCamera->SetPosition(kCamXPos, kCamYPos, kCamZPos); 
							cameraIsAttached = false;
						}
					}
					movingEnemies[0].model->MoveX(speed * frameTime);
					moving0xCoordinate = movingEnemies[0].model->GetX();
					if (moving0xCoordinate >= 30.0f || moving0xCoordinate <= -30.0f)
					{
						speed = speed * -1;
						movingEnemies[0].model->RotateY(180.0f);
					}

					movingEnemies[1].model->MoveX(-speed * frameTime);
					moving1xCoordinate = movingEnemies[1].model->GetX();
					if (moving1xCoordinate <= -30.0f || moving1xCoordinate >= 30.0f)
					{
						speed = speed * -1;
						movingEnemies[1].model->RotateY(180.0f);
					}

					movingEnemies[2].model->MoveX(speed * frameTime);
					moving2xCoordinate = movingEnemies[2].model->GetX();
					if (moving2xCoordinate >= 30.0f || moving2xCoordinate <= -30.0f)
					{
						speed = speed * -1;
						movingEnemies[2].model->RotateY(180.0f);
					}

					movingEnemies[3].model->MoveX(-speed * frameTime);
					moving3xCoordinate = movingEnemies[3].model->GetX();
					if (moving3xCoordinate <= -30.0f || moving3xCoordinate >= 30.0f)
					{
						speed = speed * -1;
						movingEnemies[3].model->RotateY(180.0f);
					}

					for (int i = 0; i < 4; i++)
					{
						movingEnemyBalls[i].model->MoveY(ballSpeed);
						movingBallYCoordinate = movingEnemyBalls[i].model->GetY();
						if (movingBallYCoordinate >= 5.0f || movingBallYCoordinate <= 2.5f)
						{
							ballSpeed = ballSpeed * -1;
						}
					}
					float matrix[4][4]; 
					playerCar.model->GetMatrix(&matrix[0][0]); 
					Vector3 localZ = { matrix[2][0], matrix[2][1], matrix[2][2] }; 

					Vector3 oldPlayerCarPos = { playerCar.model->GetX(), playerCar.model->GetY(), playerCar.model->GetZ() };   

					if (myEngine->KeyHeld(Key_W)) 
					{
						//playerCar.model->MoveLocalZ(kMoveSpeed * frameTime);
						thrust = { localZ.x * kSpeed * frameTime, localZ.y * kSpeed * frameTime, localZ.z * kSpeed * frameTime }; 
					}
					else if (myEngine->KeyHeld(Key_S))
					{
						//playerCar.model->MoveLocalZ(-kMoveSpeed * frameTime);
						thrust = { localZ.x * -kSpeed * frameTime, localZ.y * -kSpeed * frameTime, localZ.z * -kSpeed * frameTime };
					}
					else
					{
						thrust = { 0, 0, 0 };
					}

					if (myEngine->KeyHeld(Key_D))
					{
						playerCar.model->RotateY(kRotationSpeed * frameTime);
					}
					else if (myEngine->KeyHeld(Key_A))
					{
						playerCar.model->RotateY(-kRotationSpeed * frameTime);
					}

					drag = { oldMomentum.x * kDragFactor, oldMomentum.y * kDragFactor, oldMomentum.z * kDragFactor };

					momentum = { thrust.x + oldMomentum.x + drag.x,
								 thrust.y + oldMomentum.y + drag.y,
								 thrust.z + oldMomentum.z + drag.z };
					oldMomentum = momentum;
					playerCar.model->Move(momentum.x * frameTime, momentum.y * frameTime, momentum.z * frameTime);

					bool allBallsRed = true; 

					CollisionAxis checkAxis; 
					for (int i = 0; i < 4; i++) 
					{
						if (ballstate[i].isBallRed == false)
						{ 
							CollisionAxis checkAxis = CheckPlayerToEnemyCarCollision(playerCar, staticEnemies[i], oldPlayerCarPos); 
							if (checkAxis == xAxis) 
							{
								playerCar.model->SetPosition(oldPlayerCarPos.x, oldPlayerCarPos.y, oldPlayerCarPos.z); 
								momentum.z *= (-1.0f * 0.5f); 
								playerCar.model->Move(momentum.x * frameTime, momentum.y * frameTime, momentum.z * frameTime); 
								staticEnemyBalls[i].model->SetSkin("red.PNG"); 
								playerCar.model->SetPosition(prevX, prevY, prevZ);
								ballstate[i].isBallRed = true; 
								numRedBalls++; 
								if (enemyHit[i] == false) 
								{
									score = score + fixedScore; 
								}
								enemyHit[i] = true; 
							}
							else if (checkAxis == zAxis) 
							{
								playerCar.model->SetPosition(oldPlayerCarPos.x, oldPlayerCarPos.y, oldPlayerCarPos.z); 
								momentum.x *= (-1.0f * 0.5f); 
								playerCar.model->Move(momentum.x * frameTime, momentum.y * frameTime, momentum.z * frameTime); 
								staticEnemyBalls[i].model->SetSkin("red.PNG"); 
								playerCar.model->SetPosition(prevX, prevY, prevZ); 
								ballstate[i].isBallRed = true; 
								numRedBalls++; 
								if (enemyHit[i] == false) 
								{ 
									score = score + fixedScore; 
								} 
								enemyHit[i] = true; 
							}
							oldMomentum = momentum; 
						}
						else
						{
							allBallsRed = false;  
						}
					}
					stringstream scoreText; 
					scoreText << "score: " << score; 
					myFont->Draw(scoreText.str(), 300.0f, 660.0f, kBlack); 
				}
				for (int i = 0; i < 4; i++)
				{
					if (numRedBalls == totalEnemyBalls) 
					{
						isGameOver = !isGameOver; 
						stringstream gameOverText; 
						gameOverText << " Game Over"; 
						myFont->Draw(gameOverText.str(), 550.0f, 300.0f, kBlack); 
					}
				}
			}
		}	
		if (myEngine->KeyHit(kPauseButton)) 
		{
			isGamePaused = !isGamePaused; 
		}
		else if (isGamePaused == true) 
		{
			stringstream pausedText; 
			pausedText << " Game Paused! please press 'p' to continue."; 
			myFont->Draw(pausedText.str(), 300.0f, 660.0f, kBlack); 
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
