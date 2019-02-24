// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Score_Widget.generated.h"

/**
 * 
 */
UCLASS()
class STARTERPROJECT_API UUI_Score_Widget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetPlayerName(const FString& NewPlayerName, bool bSelf);
	void SetScores(int NewPlayerScore, bool bSelf);

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerNameText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerScoreText;
};
