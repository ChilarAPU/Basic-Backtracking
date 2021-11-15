// Fill out your copyright notice in the Description page of Project Settings.


#include "Maze_Generation.h"

#include "InstanceManager.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMaze_Generation::AMaze_Generation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//default values
	Seed = 0;
	SizeOfMaze = FIntPoint(10,10);
	TileSize = 100;

	//temporary
	TempFloor = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("TempFloor"));
	
	//GenerateMaze();
	/*
	Seed = 0;
	SizeOfMaze = FIntPoint(10, 10);
	TileSize = 100;
	
	if (InstanceManager)
	{
		InstanceManager->RemoveAllInstances();
		SizeOfMaze.X = (SizeOfMaze.X /2) * 2;
		SizeOfMaze.Y = (SizeOfMaze.Y /2) * 2;
	}

	TSet<FIntPoint> PathTiles_Out, EndTiles_Out;
	GenerateMaze(Seed, SizeOfMaze, &PathTiles_Out, &EndTiles_Out);
	UE_LOG(LogTemp,Warning, TEXT("Constructor"));
	PathTiles = PathTiles_Out;
	EndPaths = EndTiles_Out;
	*/
}

// Called when the game starts or when spawned
void AMaze_Generation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMaze_Generation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMaze_Generation::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	//Forcing the value to be an even number
	SizeOfMaze.X = (SizeOfMaze.X / 2) * 2;
	SizeOfMaze.Y = (SizeOfMaze.Y / 2) * 2;
	/*GenerateMaze();
	TArray<FIntPoint> PathTilesArray = PathTiles.Array();
	for (int i = 0; i < PathTilesArray.Num() - 1; i++)
	{
		FIntPoint Temp = PathTilesArray[i];
		FVector TempLoc = FVector(Temp.X * TileSize, Temp.Y * TileSize, 0);
		TempFloor->AddInstance(FTransform(TempLoc));
	}
	*/
	B_GenerateMaze(Seed, SizeOfMaze, &PathTiles, &EndPaths);
	TArray<FIntPoint> PathTilesArray = PathTiles.Array();
	for (int i = 0; i < PathTiles.Num(); i++)
	{
		FIntPoint Temp = PathTilesArray[i];
		FVector TempLoc = FVector(Temp.X * TileSize, Temp.Y * TileSize, 0);
		TempFloor->AddInstance(FTransform(TempLoc));
	}
}

void AMaze_Generation::GenerateMaze()
{
	//This removes the Old Maze from the sets. This does not erase the mesh instances. This will be handles by the instance manager
	PathTiles.Empty();
	EndPaths.Empty();
	TempFloor->ClearInstances();
	
	
	UKismetMathLibrary::SetRandomStreamSeed(RandStream, Seed);
	FIntPoint StartingLocation = GetRandLocInBounds(RandStream);
	FIntPoint CurrentLocation = StartingLocation;
	AddNeighbourLocation(RandStream, CurrentLocation);
	while (ToCheck.Num() > 0)
	{
		Backtrack(CurrentLocation);
		AddNeighbourLocation(RandStream, CurrentLocation);
	}
}

FIntPoint AMaze_Generation::GetRandLocInBounds(FRandomStream L_RandStream)
{
	FIntPoint RandomLocation;

	RandomLocation.X = UKismetMathLibrary::RandomIntegerInRangeFromStream(0, SizeOfMaze.X, L_RandStream);
	RandomLocation.Y = UKismetMathLibrary::RandomIntegerInRangeFromStream(0, SizeOfMaze.Y, L_RandStream);
	
	return RandomLocation;
}

void AMaze_Generation::AddNeighbourLocation(FRandomStream L_RandStream, FIntPoint CurrentLoc)
{
	int NeighbourIndex = UKismetMathLibrary::RandomIntegerInRangeFromStream(0, 3, L_RandStream);
	FIntPoint Floor, Bridge;
	GetNeighbourAndBridge(NeighbourIndex, CurrentLoc, &Floor, &Bridge);
	bool bValid = CheckIsValidLocation(Floor);
	if (bValid)
	{
		//GetNeighbourAndBridge(NeighbourIndex, CurrentLoc, &Floor, &Bridge); //This function call may not be necessary
		PathTiles.Emplace(Floor);
		BridgeTiles.Emplace(Bridge);
		ToCheck.Emplace(Floor);
		CurrentLoc = Floor;
		AddNeighbourLocation(L_RandStream, CurrentLoc);
	} else
	{
		int N = 0;
		while (N <= 4)
		{
			NeighbourIndex = UKismetMathLibrary::SelectInt(0, NeighbourIndex ++, NeighbourIndex + 1 >= 4);
			N++;
		}
		int NeighbourCount = GetNeighbourCount(CurrentLoc);
		//indicates it is an end tile
		if (NeighbourCount == 1)
		{
			EndPaths.Emplace(CurrentLoc);
		}
	}
}

void AMaze_Generation::GetNeighbourAndBridge(int L_Index, FIntPoint Location, FIntPoint* Floor, FIntPoint* Bridge)
{
	TArray<FIntPoint> FloorDirection = GetNeightbourLocation(2, Location);
	*Floor = FloorDirection[L_Index];

	TArray<FIntPoint> BridgeDirection = GetNeightbourLocation(1, Location);
	*Bridge = BridgeDirection[L_Index];
}

bool AMaze_Generation::CheckIsValidLocation(FIntPoint Location)
{
	bool bExceedSize = CheckIfLocationExceedsSize(Location);
	if (!(bExceedSize || PathTiles.Contains(Location) || BridgeTiles.Contains(Location)))
	{
		return true;
	} else
	{
		return false;
	}
 }

TArray<FIntPoint> AMaze_Generation::GetNeightbourLocation(int Distance, FIntPoint Location)
{
	TArray<FIntPoint> NeighbourDirection;

	NeighbourDirection.Emplace(FIntPoint(Distance, 0) + Location);
	NeighbourDirection.Emplace(FIntPoint(0, Distance) + Location);
	NeighbourDirection.Emplace(FIntPoint(Distance * -1, 0) + Location);
	NeighbourDirection.Emplace(FIntPoint(0, Distance * -1) + Location);

	return NeighbourDirection;
}

bool AMaze_Generation::CheckIfLocationExceedsSize(FIntPoint Location)
{
	if (Location.X < 0 || Location.Y < 0 || Location.X > SizeOfMaze.X || Location.Y > SizeOfMaze.Y)
	{
		return true;
	} else
	{
		return false;
	}
}

int AMaze_Generation::GetNeighbourCount(FIntPoint Location)
{
	int Counter = 0;
	TArray<FIntPoint> Neighbours = GetNeightbourLocation(1, Location);
	for (int i = 0; i < Neighbours.Num() - 1; i++)
	{
		if (PathTiles.Contains(Neighbours[i]))
		{
			Counter++;
		}
	}
	return Counter;
}

void AMaze_Generation::Backtrack(FIntPoint CurrentLoc)
{
	TArray<FIntPoint> ToCheckArray = ToCheck.Array();
	CurrentLoc = ToCheckArray.Last();
	ToCheck.Remove(CurrentLoc);
	if (BridgeTiles.Num() > 0)
	{
		TArray<FIntPoint> BridgeTilesArray = BridgeTiles.Array();
    	PathTiles.Emplace(BridgeTilesArray.Last());
    	BridgeTiles.Remove(BridgeTilesArray.Last());	
	}
	
}

void AMaze_Generation::B_GenerateMaze(int L_Seed, FIntPoint L_SizeOfMaze, TSet<FIntPoint>* PathTiles_Out,
	TSet<FIntPoint>* EndPaths_Out)
{
	PathTiles_Out->Empty();
	EndPaths_Out->Empty();
	TempFloor->ClearInstances();
	
	UKismetMathLibrary::SetRandomStreamSeed(RandStream, L_Seed);
	FIntPoint* SizeOfMaze_L = &L_SizeOfMaze;
	FIntPoint StartingLocation = GetRandLocInBounds(RandStream);
	FIntPoint CurrentLocation = StartingLocation;
	
	AddNeighbourLoc_B(SizeOfMaze_L, &RandStream, &CurrentLocation, PathTiles_Out, &BridgeTiles, &ToCheck, EndPaths_Out);
	while (ToCheck.Num() > 0)
	{
		BackTrack_B(&CurrentLocation, &ToCheck, &BridgeTiles, PathTiles_Out);
		AddNeighbourLoc_B(SizeOfMaze_L, &RandStream, &CurrentLocation, PathTiles_Out, &BridgeTiles, &ToCheck, EndPaths_Out);
	}
}

void AMaze_Generation::AddNeighbourLoc_B(FIntPoint* L_SizeOfMaze, FRandomStream* L_RandStream, FIntPoint* CurrentLocation,
	TSet<FIntPoint>* L_PathTiles, TSet<FIntPoint>* L_BridgeTiles, TSet<FIntPoint>* L_ToCheck, TSet<FIntPoint>* L_EndPaths)
{
	int NeighbourIndex = UKismetMathLibrary::RandomIntegerInRangeFromStream(0, 3, *L_RandStream);
	FIntPoint Floor, Bridge;
	GetNeighbourAndBridge(NeighbourIndex, *CurrentLocation, &Floor, &Bridge);
	/*hbool bValid = CheckIsValidLocation(Floor);
	if (bValid)
	{
		L_PathTiles->Emplace(Floor);
		L_BridgeTiles->Emplace(Bridge);
		L_ToCheck->Emplace(Floor);
		AddNeighbourLoc_B(L_SizeOfMaze, L_RandStream, &Floor, L_PathTiles, L_BridgeTiles, L_ToCheck, L_EndPaths);
	} else
	{
		int N = 0;
		while (N <= 4)
		{
			NeighbourIndex = UKismetMathLibrary::SelectInt(0, NeighbourIndex + 1, NeighbourIndex + 1 >= 4);
			N++;
			if (N > 4)
			{
				int NeighbourCount = GetNeighbourCount(*CurrentLocation);
				//indicates it is an end tile
				if (NeighbourCount == 1)
				{
					L_EndPaths->Emplace(*CurrentLocation);
				}
			}
		} 
		
	}*/
	while (!CheckIsValidLocation(Floor))
	{
		if (NeighbourIndex >= 4)
		{
			int NeighbourCount = GetNeighbourCount(*CurrentLocation); //meaning we have checked all location around current location
			//indicates it is an end tile
			if (NeighbourCount == 1)
			{
				L_EndPaths->Emplace(*CurrentLocation);
			}
			NeighbourIndex = 0;
			break;
		}
		GetNeighbourAndBridge(NeighbourIndex, *CurrentLocation, &Floor, &Bridge);
		if (CheckIsValidLocation(Floor))
		{
			break;
		}
		NeighbourIndex++;
	}
	if (CheckIsValidLocation(Floor))
	{
		L_PathTiles->Emplace(Floor);
		L_BridgeTiles->Emplace(Bridge);
		L_ToCheck->Emplace(Floor);
		AddNeighbourLoc_B(L_SizeOfMaze, L_RandStream, &Floor, L_PathTiles, L_BridgeTiles, L_ToCheck, L_EndPaths);
	}
	
}

void AMaze_Generation::BackTrack_B(FIntPoint* CurrentLocation, TSet<FIntPoint>* L_ToCheck, TSet<FIntPoint>* L_BridgeTiles,
	TSet<FIntPoint>* L_PathTiles)
{
	TArray<FIntPoint> ToCheckArray = L_ToCheck->Array();
	*CurrentLocation = ToCheckArray.Last();
	L_ToCheck->Remove(ToCheckArray.Last());
	if (L_BridgeTiles->Num() > 0)
	{
		TArray<FIntPoint> BridgeTilesArray = L_BridgeTiles->Array();
		L_PathTiles->Emplace(BridgeTilesArray.Last());
		L_BridgeTiles->Remove(BridgeTilesArray.Last());	
	}
}




