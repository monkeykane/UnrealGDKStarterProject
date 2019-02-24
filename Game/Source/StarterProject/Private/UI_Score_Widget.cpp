// Fill out your copyright notice in the Description page of Project Settings.

#include "UI_Score_Widget.h"

#include "Components/TextBlock.h"



void UUI_Score_Widget::SetPlayerName(const FString& NewPlayerName, bool bSelf)
{
	if (PlayerNameText)
	{
		PlayerNameText->SetText(FText::FromString("[ME]"+NewPlayerName));
	}
}

void UUI_Score_Widget::SetScores(int NewPlayerScore, bool bSelf)
{
	if (PlayerScoreText)
	{
		PlayerScoreText->SetText(FText::AsNumber(NewPlayerScore));
	}
}
