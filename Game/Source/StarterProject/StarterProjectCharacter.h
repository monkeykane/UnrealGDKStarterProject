// Copyright (c) Improbable Worlds Ltd, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "EntityRegistry.h"
#include "GameFramework/Character.h"
#include "AProjectile.h"
#include "AFakeProjectile.h"
#include "StarterProjectCharacter.generated.h"

UCLASS(config=Game, SpatialType)
class AStarterProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AStarterProjectCharacter();

	virtual void BeginPlay() override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Weapon")
	void HitByGrenade(AAProjectile* proj);


protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	void Interact();

	UFUNCTION(Server, Reliable, WithValidation)
	void TestRPC();

	UFUNCTION(NetMulticast, Unreliable, WithValidation)
	void TestMulticast();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	// homework 
	UFUNCTION()
		void OnRep_CurrentHealth();

	float TakeDamage(float Damage, const struct FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(CrossServer, Reliable)
	void TakeDamageCrossServer(float Damage, const struct FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	// RPC to tell the server to spawn grenade
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSpawnGrenade();

	// RPC to tell the server to throw grenade.
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerThrowGrenade();

	// Max health this character can have.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "1"))
	int32 MaxHealth;

	// Current health of the character, can be at most MaxHealth.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Health", ReplicatedUsing = OnRep_CurrentHealth)
	int32 CurrentHealth;

	// Game score point.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Score")
	int32 ScorePoint;


	// Cube to spawn when the player presses "SpawnGrenade".
	UPROPERTY(EditDefaultsOnly, Category = "Homework/Grenade")
	TSubclassOf<AAProjectile>	GrenadeTemplate;

	// Cube to spawn when the player presses "SpawnGrenade".
	UPROPERTY(EditDefaultsOnly, Category = "Homework/Grenade")
	TSubclassOf<AAFakeProjectile>	FakeGrenadeTemplate;

	// Maximum distance at which the player can interact with objects.
	UPROPERTY(EditDefaultsOnly, Category = "Homework/Grenade", meta = (ClampMin = "0.0"))
	float InteractDistance;

	FTimerHandle	TimeHandle_Recover;

private:
	AAProjectile * currentProjectile = nullptr;
	AAFakeProjectile* currentFakeProjectile = nullptr;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	// Spawn Grenade
	void SpawnGrenade();

	// Throw Grenade
	void ThrowGrenade();

	void OnRecover();
};

