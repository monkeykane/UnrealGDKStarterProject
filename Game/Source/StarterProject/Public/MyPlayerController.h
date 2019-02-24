// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STARTERPROJECT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	
public:
	AMyPlayerController();

	// [client] Sets whether the scoreboard should be visible.
	void SetScoreboardIsVisible(bool bIsVisible);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Gets a default player name based upon the worker's ID.
	// Generates a GUID if we're not running on a SpatialOS worker.
	FString GetDefaultPlayerName();

	// Game score point.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Score")
	FString		PlayerName;

protected:
	virtual void SetupInputComponent() override;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


private:
	
	// [client] Initializes the scoreboard UI.
	void InitScoreboard();

	// [client] Shows the scoreboard UI.
	void ShowScoreboard();

	// [client] Hides the scoreboard UI.
	void HideScoreboard();

	// Scoreboard UI class to use in-game.
	UPROPERTY(EditAnywhere, Category = "TPS UI")
		TSubclassOf<class UUI_ScoreBoard> ScoreboardTemplate;

	// The current scoreboard UI widget instance.
	UPROPERTY(Transient)
		class UUI_ScoreBoard* Scoreboard;
};
