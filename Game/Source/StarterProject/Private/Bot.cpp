// Fill out your copyright notice in the Description page of Project Settings.


#include "Bot.h"
#include "Engine.h"

#include "BotAIController.h"

ABot::ABot()
{
	AIControllerClass = ABotAIController::StaticClass();

	bUseControllerRotationYaw = true;
}

void ABot::FaceRotation(FRotator NewRotation, float DeltaTime)
{
	FRotator CurrentRotation = FMath::RInterpTo(GetActorRotation(), NewRotation, DeltaTime, 8.0f);

	Super::FaceRotation(CurrentRotation, DeltaTime);
}
