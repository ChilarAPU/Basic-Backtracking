// Fill out your copyright notice in the Description page of Project Settings.


#include "InstanceManager.h"

// Sets default values
AInstanceManager::AInstanceManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInstanceManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInstanceManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInstanceManager::RemoveAllInstances()
{
	TArray<UStaticMesh*> Keys;
	InstanceComponentList.GenerateKeyArray(Keys);
	for (int i = 0; i < Keys.Num(); i++)
	{
		//ClearInstanceComponents(InstanceComponentList.Find(Keys[i]));
	}
}

/*bool AMaze_Generation::AddNewMeshInstance(UStaticMesh* Mesh, FTransform Transform)
{
	UInstancedStaticMeshComponent* Key;
	if (InstanceComponentList.Find(Mesh))
	{
		AddNewMeshInstance(Mesh, Transform);
		return true;
	} else
	{
		InstanceComponentList.Add(Mesh, Key);
		Key->SetStaticMesh(Mesh);
		AddNewMeshInstance(Mesh, Transform);
		return true;
	}
}*/

