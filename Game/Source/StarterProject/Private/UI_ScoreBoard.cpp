// Fill out your copyright notice in the Description page of Project Settings.

#include "UI_ScoreBoard.h"
#include "UI_Score_Widget.h"
#include "Components/PanelWidget.h"



void UUI_ScoreBoard::UpdateScoreList(const TArray<FPlayerScore>& PlayerLists, const FString& owner )
{
	PlayerScoresContainerWidget->ClearChildren();

	for (const FPlayerScore& fps : PlayerLists)
	{
		bool bSelf = (fps.PlayerName == owner);
		if (!ScoreWidgets.Contains(fps.PlayerName))
		{
			check(ScoreWidgetTemplate);

			UUI_Score_Widget* NewWidget = CreateWidget<UUI_Score_Widget>(GetOwningPlayer(), ScoreWidgetTemplate);
			NewWidget->SetPlayerName(fps.PlayerName, bSelf);
			ScoreWidgets.Emplace(fps.PlayerName, NewWidget);
		}

		UUI_Score_Widget* PlayersWidget = ScoreWidgets[fps.PlayerName];
		PlayersWidget->SetScores(fps.ScorePoints, bSelf);
		PlayerScoresContainerWidget->AddChild(PlayersWidget);

	}

}
