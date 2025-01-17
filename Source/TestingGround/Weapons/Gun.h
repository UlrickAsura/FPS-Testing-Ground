// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS(Blueprintable, BlueprintType)
class TESTINGGROUND_API AGun : public AActor
{
	GENERATED_BODY()
	
	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	class USkeletalMeshComponent* FP_Gun;

public:	
	// Sets default values for this actor's properties
	AGun();
	
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<class AProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	class USoundBase* FireSound;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	class USceneComponent* FP_MuzzleLocation;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	/** Fires a projectile. */
	void OnFire();

};