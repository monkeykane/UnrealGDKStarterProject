// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "StarterProjectCharacter.h"
#include "StarterProjectGameMode.h"
#include "BotGameState.h"
#include "Bot.h"
#include "BotAIController.h"
#include "UI_Score_Widget.h"
#include "UI_ScoreBoard.h"
#include "SpatialNetDriver.h"
#include "Connection/SpatialWorkerConnection.h"
#include "UnrealNetwork.h"

AMyPlayerController::AMyPlayerController()
	: Scoreboard(nullptr)
{

}


void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("ShowScoreboard", IE_Pressed, this, &AMyPlayerController::ShowScoreboard);
	InputComponent->BindAction("ShowScoreboard", IE_Released, this, &AMyPlayerController::HideScoreboard);
}

// Called when the game starts or when spawned
void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if ( !HasAuthority() )
		InitScoreboard();
	if (HasAuthority())
	{
		if (ABotGameState* GS = GetWorld()->GetGameState<ABotGameState>())
		{
			GS->RegesterPlayer(GetDefaultPlayerName());
		}
	}
}


void AMyPlayerController::InitScoreboard()
{
	check(GetNetMode() != NM_DedicatedServer);

	if (Scoreboard)
	{
		return;
	}

	check(ScoreboardTemplate != nullptr);
	Scoreboard = CreateWidget<UUI_ScoreBoard>(this, ScoreboardTemplate);

	if (Scoreboard == nullptr)
	{
		return;
	}

	//if (ATPSGameState* GS = GetWorld()->GetGameState<ATPSGameState>())
	//{
	//	// Register a listener between the GameState's score list and the scoreboard's update function.
	//	FSGTeamScoresUpdatedDelegate UpdateScoreboardCallback;
	//	UpdateScoreboardCallback.BindUObject(Scoreboard, &UTPSScoreboard::UpdateTeamScores);
	//	GS->RegisterScoreChangeListener(UpdateScoreboardCallback);
	//}
}

void AMyPlayerController::ShowScoreboard()
{
	check(GetNetMode() != NM_DedicatedServer);

	SetScoreboardIsVisible(true);
}

void AMyPlayerController::HideScoreboard()
{
	check(GetNetMode() != NM_DedicatedServer);
	SetScoreboardIsVisible(false);
}

FString AMyPlayerController::GetDefaultPlayerName()
{
	//if (USpatialNetDriver* SpatialNetDriver = Cast<USpatialNetDriver>(GetNetDriver()))
	//{
	//	return SpatialNetDriver->Connection->GetWorkerId();
	//}
	PlayerName = "Player" + GetName();
	return PlayerName;
}

void AMyPlayerController::SetScoreboardIsVisible(bool bIsVisible)
{
	if (Scoreboard == nullptr || Scoreboard->IsInViewport() == bIsVisible)
	{
		return;
	}

	if (bIsVisible)
	{
		if (ABotGameState* GS = GetWorld()->GetGameState<ABotGameState>())
		{
			Scoreboard->UpdateScoreList(GS->PlayerLists, PlayerName);
		}

		
		Scoreboard->AddToViewport();
	}
	else
	{
		Scoreboard->RemoveFromViewport();
	}
}

// homework
void AMyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyPlayerController, PlayerName);
}
