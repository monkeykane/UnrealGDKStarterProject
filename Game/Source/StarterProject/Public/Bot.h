// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StarterProjectCharacter.h"
#include "Bot.generated.h"

/**
 * 
 */
UCLASS()
class STARTERPROJECT_API ABot : public AStarterProjectCharacter
{
	GENERATED_BODY()
	
public:
	ABot();

	UPROPERTY(EditAnywhere, Category = Behavior)
	class UBehaviorTree* BotBehavior;
	
	virtual void FaceRotation(FRotator NewRotation, float DeltaTime = 0.f) override;
};
