// Fill out your copyright notice in the Description page of Project Settings.

#include "BotGameState.h"
#include "EngineUtils.h"
#include "MyPlayerController.h"
#include "BotAIController.h"
#include "UnrealNetwork.h"


ABotGameState::ABotGameState()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABotGameState::BeginPlay()
{
	if (HasAuthority() )
	{
		PlayerLists.Reserve(3);
		
		for (TActorIterator<AMyPlayerController> It(GetWorld()); It; ++It)
		{
			AMyPlayerController* pc = *It;
			RegesterPlayer(pc->GetDefaultPlayerName());
		}
		//for (TActorIterator<ABotAIController> It(GetWorld()); It; ++It)
		//{
		//	ABotAIController* pc = *It;
		//	RegesterPlayer(pc->GetDefaultPlayerName());
		//}


	}
}

void ABotGameState::RegesterPlayer(const FString& name)
{
	for (int index = 0; index < PlayerLists.Num(); ++index)
	{
		if (name == PlayerLists[index].PlayerName)
		{
			return;
		}
	}

	FPlayerScore fps;
	fps.PlayerName = name;
	fps.ScorePoints = 0;

	PlayerLists.Add(fps);
}

void ABotGameState::UpdateScorePoints(const FString& name, int scorePoint)
{
	for (int index = 0; index < PlayerLists.Num(); ++index)
	{
		if (name == PlayerLists[index].PlayerName )
		{
			PlayerLists[index].ScorePoints += scorePoint;
		}
	}

	// Re-sort team scores after updating the killer's team score.
	PlayerLists.Sort([](const FPlayerScore& lhs, const FPlayerScore& rhs)
	{
		// Sort in reverse order.
		return lhs.ScorePoints > rhs.ScorePoints;
	});
}

void ABotGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABotGameState, PlayerLists);
}

void ABotGameState::OnRep_PlayerScores()
{
	
}
