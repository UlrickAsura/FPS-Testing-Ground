// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TestingGroundCharacter.generated.h"

UCLASS()
class TESTINGGROUND_API ATestingGroundCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestingGroundCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void UnPossessed() override;

	virtual void PossessedBy(AController* NewController) override;
public:	

	/*virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;*/
	UFUNCTION()
	void OnDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void TakeDamage();

	UFUNCTION(BlueprintCallable)
	void PullTrigger();

	UFUNCTION(BlueprintCallable)
	bool IsDead();
protected:
	/** Death Camera spawn point */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun")
	class UArrowComponent* DeathCameraSpawn;

	/** Third person camera */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* TPCamera;
	/** Third person spring arm for the camera */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* TPCameraBoom;
	/** First person camera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* FPCamera;
	/** Mesh for the first person view */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	class USkeletalMeshComponent* FPMesh;
	/** Class used to spawn the player gun */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun")
	TSubclassOf<class AGun> GunTemplate;
	/** Instance of the gun used by the character */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Gun")
	class AGun* Gun;
	/** AnimMontage to play for the first person*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (DisplayName = "Third Person Animation"))
	class UAnimMontage* TPFireAnimation;

	/** AnimMontage to play for the third person*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (DisplayName = "First Person Animation"))
	class UAnimMontage* FPFireAnimation;
protected:

	/** Represent the life of the character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	int32 Health;
};