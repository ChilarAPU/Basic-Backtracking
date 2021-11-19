// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Maze_Generation.generated.h"

class AInstanceManager;
UCLASS()
class MAZEGENERATIONC_API AMaze_Generation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMaze_Generation();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	int Seed;
	
	FRandomStream RandStream;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	FIntPoint SizeOfMaze;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	FIntPoint GCurrentLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	float TileSize;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze")
	TSet<FIntPoint> PathTiles;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze")
    TSet<FIntPoint> WallTiles;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze")
	TSet<FIntPoint> ToCheck;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze")
	TSet<FIntPoint> EndPaths;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze")
	TSet<FIntPoint> BridgeTiles;

	//Next three variables are specific to spawning the instances
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	UStaticMesh* FloorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	UStaticMesh* WallMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	class AInstanceManager* InstanceManager;

	//Temporary

	UPROPERTY(BlueprintReadWrite, Category = "Maze")
	USceneComponent* RootComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	UHierarchicalInstancedStaticMeshComponent* TempFloor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Maze")
	UHierarchicalInstancedStaticMeshComponent* TempWall;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;


	FIntPoint GetRandLocInBounds(FRandomStream L_RandStream);

	void GetNeighbourAndBridge(int L_Index, FIntPoint Location, FIntPoint* Floor, FIntPoint* Bridge);

	bool CheckIsValidLocation(FIntPoint Location);

	TArray<FIntPoint> GetNeightbourLocation(int Distance, FIntPoint Location);

	bool CheckIfLocationExceedsSize(FIntPoint Location);

	int GetNeighbourCount(FIntPoint Location);

	FTransform BuildTilesLoc(FIntPoint Location, float ZAxis, int RotationIndex);

	
	//Trying something new
	void GenerateMaze(int L_Seed, FIntPoint L_SizeOfMaze, TSet<FIntPoint>* PathTiles_Out, TSet<FIntPoint>* EndPaths_Out);

	void AddNeighbourLoc(FIntPoint* L_SizeOfMaze, FRandomStream* L_RandStream, FIntPoint* CurrentLocation, TSet<FIntPoint>* L_PathTiles,
		TSet<FIntPoint>* L_BridgeTiles, TSet<FIntPoint>* L_ToCheck, TSet<FIntPoint>* L_EndPaths);

	void BackTrack(FIntPoint* CurrentLocation, TSet<FIntPoint>* L_ToCheck, TSet<FIntPoint>* L_BridgeTiles, TSet<FIntPoint>* L_PathTiles);

	void SpawnMazeMeshes();
};
