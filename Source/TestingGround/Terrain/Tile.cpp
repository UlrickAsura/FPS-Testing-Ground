// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "SubclassOf.h"
#include "DrawDebugHelpers.h"
#include "ActorPoolComponent.h"
#include "AI/Navigation/NavigationSystem.h"

// Sets default values
ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ActorPool = nullptr;
	NavMeshBoundsVolume = nullptr;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(FName("Shared Root")));

	MinSpawnPoint = CreateDefaultSubobject<UArrowComponent>(FName("FBox Min"));
	MinSpawnPoint->SetupAttachment(RootComponent);

	MaxSpawnPoint = CreateEditorOnlyDefaultSubobject<UArrowComponent>(FName("FBox Max"));
	MaxSpawnPoint->SetupAttachment(RootComponent);

	AttachLocation = CreateEditorOnlyDefaultSubobject<UArrowComponent>(FName("Attach Location"));
	AttachLocation->SetupAttachment(RootComponent);
	AttachLocation->SetRelativeLocation(FVector(3675.0f,0.0f,0.0f));
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (ActorPool && NavMeshBoundsVolume) {
		ActorPool->Return(NavMeshBoundsVolume);
	}
	UE_LOG(LogTemp,Warning,TEXT("[%s] EndPlay"),*GetName());
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::PositionNavMeshBoundsVolume()
{
	check(ActorPool != nullptr)
	if (ActorPool)
	{
		NavMeshBoundsVolume = ActorPool->Checkout();
		if (NavMeshBoundsVolume)
		{
			NavMeshBoundsVolume->SetActorLocation(GetActorLocation());
			GetWorld()->GetNavigationSystem()->Build();
		}
	}
}

void ATile::SetActorPool(UActorPoolComponent* ActorPoolToSet)
{
	check(ActorPoolToSet != nullptr)
	ActorPool = ActorPoolToSet;
}

FTransform ATile::GetAttachLocation() const
{
	return AttachLocation->GetComponentTransform();
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn,const FSpawnParams SpawnParams)
{
	for(const FTransform& SpawnPosition : GenerateSpawnPosition(SpawnParams))
	{
		PlaceActor(ToSpawn, SpawnPosition);
	}
}

bool  ATile::CanSpawnAtLocation(FVector Location, float Radius)
{
	FHitResult HitResult;
	FVector GlobalPosition = ActorToWorld().TransformPosition(Location);
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult, 
		GlobalPosition,
		GlobalPosition,
		FQuat::Identity, 
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);

	FColor ResultColor = (HasHit)?FColor::Red: FColor::Green;
	//DrawDebugSphere(
	//	GetWorld(),
	//	GlobalPosition,
	//	Radius,
	//	12,
	//	ResultColor,
	//	true,
	//	100
	//);
	return !HasHit;
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn,const FTransform& SpawnPosition)
{
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	if (Spawned)
	{
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SetActorTransform(SpawnPosition);
	}
}

bool ATile::FindEmptyLocation(float Radius, FVector &OutLocation)
{
	FBox Bounds = FBox(MinSpawnPoint->RelativeLocation, MaxSpawnPoint->RelativeLocation);
	//TODO PUT CONST IN HEADER FILE ?
	const int MAX_ATTEMPS = 20;
	for (size_t NbOfAttemp = 0; NbOfAttemp < MAX_ATTEMPS; NbOfAttemp++)
	{
		FVector SpawnPoint = FMath::RandPointInBox(Bounds);
		if (CanSpawnAtLocation(SpawnPoint, Radius))
		{
			OutLocation = SpawnPoint;
			return true;
		}
	}

	return false;
}

TArray<FTransform> ATile::GenerateSpawnPosition(const FSpawnParams& SpawnParam)
{
	TArray<FTransform> SpawnPoints;
	int32 NumberToSpawn = FMath::RandRange(SpawnParam.MinSpawn, SpawnParam.MaxSpawn);
	for (int32 ActorCount = 0; ActorCount < NumberToSpawn; ActorCount++)
	{
		FVector Location;
		FRotator Rot;
		Rot.Roll = FMath::RandRange(SpawnParam.MinTransform.Rotation.Roll, SpawnParam.MaxTransform.Rotation.Roll);
		Rot.Pitch = FMath::RandRange(SpawnParam.MinTransform.Rotation.Pitch,SpawnParam.MaxTransform.Rotation.Pitch);
		Rot.Yaw = FMath::RandRange(SpawnParam.MinTransform.Rotation.Yaw, SpawnParam.MaxTransform.Rotation.Yaw);
		FVector Scale;
		Scale.X = FMath::RandRange(SpawnParam.MinTransform.Scale.X, SpawnParam.MaxTransform.Scale.X);
		Scale.Y = FMath::RandRange(SpawnParam.MinTransform.Scale.Y, SpawnParam.MaxTransform.Scale.Y);
		Scale.Z = FMath::RandRange(SpawnParam.MinTransform.Scale.Z, SpawnParam.MaxTransform.Scale.Z);
		FTransform SpawnPosition(Rot,Location,Scale);
		if (FindEmptyLocation(SpawnParam.CollisionRadius * SpawnPosition.GetScale3D().GetMax(), Location))
		{
			SpawnPosition.SetLocation(Location);
			SpawnPoints.Add(SpawnPosition);
		}
	}
	return SpawnPoints;
}