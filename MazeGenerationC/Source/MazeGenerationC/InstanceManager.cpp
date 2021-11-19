// Fill out your copyright notice in the Description page of Project Settings.


#include "InstanceManager.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

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

void AInstanceManager::AddNewMeshInstance(UStaticMesh* Mesh, FTransform Transform)
{
	if (Mesh)
	{
		if (InstanceComponentList.Find(Mesh))
        	{
        		AddNewMeshInstance(Mesh, Transform);
        	} else
        	{
        		GlobalMesh->AddInstance(Transform);
        		InstanceComponentList.Add(Mesh, GlobalMesh);
        		GlobalMesh->SetStaticMesh(Mesh);
        		GlobalMesh->AddInstanceWorldSpace(Transform);
        		
        	}
	}
	
}

void AInstanceManager::RemoveAllMeshInstances(UStaticMesh* Mesh)
{
	TArray<UStaticMesh*> MapKeys;
	InstanceComponentList.GetKeys(MapKeys);
	for (int i = 0; i < MapKeys.Num() - 1; i++)
	{
		
		InstanceComponentList.Find(MapKeys[i]);
		//UInstancedStaticMeshComponent::ClearInstances(InstanceComponentList.Find(MapKeys[i]));
		GlobalMesh->ClearInstances();
		
	}
}

void AInstanceManager::RemoveMeshInstance(UStaticMesh* Mesh, int Index)
{
	if (InstanceComponentList.Find(Mesh))
	{
		RemoveMeshInstance(Mesh, Index);
	}
}



