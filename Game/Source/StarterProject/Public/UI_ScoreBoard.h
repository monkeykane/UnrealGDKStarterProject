// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BotGameState.h"
#include "UI_ScoreBoard.generated.h"



/**
 * 
 */
UCLASS()
class STARTERPROJECT_API UUI_ScoreBoard : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateScoreList(const TArray<FPlayerScore>& PlayerLists ,const FString& owner);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Team Score")
		TSubclassOf<class UUI_Score_Widget> ScoreWidgetTemplate;

	UPROPERTY(VisibleAnywhere, Category = "Team Score")
		TMap<FString, UUI_Score_Widget*>		ScoreWidgets;

	UPROPERTY(meta = (BindWidget))
		class UPanelWidget* PlayerScoresContainerWidget;

	
	
};
