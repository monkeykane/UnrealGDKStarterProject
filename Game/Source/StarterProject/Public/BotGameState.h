// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "StarterProjectCharacter.h"
#include "BotGameState.generated.h"

USTRUCT()
struct FPlayerScore {
	GENERATED_BODY()

		UPROPERTY()
		FString PlayerName;

	UPROPERTY()
		int32 ScorePoints;
};

/**
 * 
 */
UCLASS(SpatialType = Singleton)
class STARTERPROJECT_API ABotGameState : public AGameStateBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	ABotGameState();

	void RegesterPlayer(const FString& name);
	void UpdateScorePoints(const FString& name, int scorePoint);

	UFUNCTION()
	void OnRep_PlayerScores();

	UPROPERTY(ReplicatedUsing = OnRep_PlayerScores)
	TArray<FPlayerScore>		PlayerLists;
	
};
