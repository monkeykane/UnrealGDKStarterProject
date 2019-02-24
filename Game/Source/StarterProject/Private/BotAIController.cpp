// Fill out your copyright notice in the Description page of Project Settings.

#include "BotAIController.h"
#include "Bot.h"
#include "StarterProjectCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "SpatialNetDriver.h"
#include "Connection/SpatialWorkerConnection.h"
#include "BotGameState.h"

ABotAIController::ABotAIController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComp"));

	BrainComponent = BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));

	bWantsPlayerState = true;
}

void ABotAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	ABot* Bot = Cast<ABot>(InPawn);

	// start behavior
	if (Bot && Bot->BotBehavior)
	{
		if (Bot->BotBehavior->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*Bot->BotBehavior->BlackboardAsset);
		}

		EnemyKeyID = BlackboardComp->GetKeyID("Enemy");

		BehaviorComp->StartTree(*(Bot->BotBehavior));
	}
}

void ABotAIController::UnPossess()
{
	Super::UnPossess();

	BehaviorComp->StopTree();
}

void ABotAIController::BeginInactiveState()
{
	Super::BeginInactiveState();	
}


void ABotAIController::FindClosestEnemy()
{
	APawn* MyBot = GetPawn();
	if (MyBot == NULL)
	{
		return;
	}

	const FVector MyLoc = MyBot->GetActorLocation();
	float BestDistSq = MAX_FLT;
	AStarterProjectCharacter* BestPawn = NULL;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		AStarterProjectCharacter* TestPawn = Cast<AStarterProjectCharacter>(*It);
		if (TestPawn != nullptr)
		{
			const float DistSq = (TestPawn->GetActorLocation() - MyLoc).SizeSquared();
			if (DistSq < BestDistSq)
			{
				BestDistSq = DistSq;
				BestPawn = TestPawn;
			}
		}
	}

	if (BestPawn)
	{
		SetEnemy(BestPawn);
	}
}

bool ABotAIController::FindClosestEnemyWithLOS(APawn* ExcludeEnemy)
{
	bool bGotEnemy = false;
	APawn* MyBot = GetPawn();
	if (MyBot != NULL)
	{
		const FVector MyLoc = MyBot->GetActorLocation();
		float BestDistSq = MAX_FLT;
		AStarterProjectCharacter* BestPawn = NULL;

		for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
		{
			AStarterProjectCharacter* TestPawn = Cast<AStarterProjectCharacter>(*It);
			if (TestPawn && TestPawn != ExcludeEnemy )
			{
				if (HasWeaponLOSToEnemy(TestPawn, true) == true)
				{
					const float DistSq = (TestPawn->GetActorLocation() - MyLoc).SizeSquared();
					if (DistSq < BestDistSq)
					{
						BestDistSq = DistSq;
						BestPawn = TestPawn;
					}
				}
			}
		}
		if (BestPawn)
		{
			SetEnemy(BestPawn);
			bGotEnemy = true;
		}
	}
	return bGotEnemy;
}

bool ABotAIController::HasWeaponLOSToEnemy(AActor* InEnemyActor, const bool bAnyEnemy) const
{

	ABot* MyBot = Cast<ABot>(GetPawn());

	bool bHasLOS = false;
	// Perform trace to retrieve hit info
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(AIWeaponLosTrace), true, GetPawn());
	TraceParams.bTraceAsyncScene = true;

	TraceParams.bReturnPhysicalMaterial = true;
	FVector StartLocation = MyBot->GetActorLocation();
	StartLocation.Z += GetPawn()->BaseEyeHeight; //look from eyes

	FHitResult Hit(ForceInit);
	const FVector EndLocation = InEnemyActor->GetActorLocation();
	GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_GameTraceChannel1, TraceParams);
	if (Hit.bBlockingHit == true)
	{
		// Theres a blocking hit - check if its our enemy actor
		AActor* HitActor = Hit.GetActor();
		if (Hit.GetActor() != NULL)
		{
			if (HitActor == InEnemyActor)
			{
				bHasLOS = true;
			}
			else if (bAnyEnemy == true)
			{
				// Its not our actor, maybe its still an enemy ?
				ACharacter* HitChar = Cast<ACharacter>(HitActor);
				if (HitChar != NULL)
				{
					bHasLOS = true;
				}
			}
		}
	}



	return bHasLOS;
}

void ABotAIController::ShootEnemy()
{
	if (HasAuthority())
	{
		ABot* MyBot = Cast<ABot>(GetPawn());

		bool bCanShoot = false;
		APawn* Enemy = GetEnemy();
		if (Enemy != nullptr)
		{
			if (LineOfSightTo(Enemy, MyBot->GetActorLocation()))
			{
				bCanShoot = true;
			}
		}

		if (bCanShoot)
		{
			//MyBot->StartWeaponFire();
			MyBot->ServerThrowGrenade();
		}
	}
}

void ABotAIController::SetEnemy(class APawn* InPawn)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValue<UBlackboardKeyType_Object>(EnemyKeyID, InPawn);
		SetFocus(InPawn);
	}
}

class APawn* ABotAIController::GetEnemy() const
{
	if (BlackboardComp)
	{
		return Cast<APawn>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));
	}

	return NULL;
}


void ABotAIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	// Look toward focus
	FVector FocalPoint = GetFocalPoint();
	if (!FocalPoint.IsZero() && GetPawn())
	{
		FVector Direction = FocalPoint - GetPawn()->GetActorLocation();
		FRotator NewControlRotation = Direction.Rotation();

		NewControlRotation.Yaw = FRotator::ClampAxis(NewControlRotation.Yaw);

		SetControlRotation(NewControlRotation);

		APawn* const P = GetPawn();
		if (P && bUpdatePawn)
		{
			P->FaceRotation(NewControlRotation, DeltaTime);
		}

	}
}

void ABotAIController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	// Stop the behaviour tree/logic
	BehaviorComp->StopTree();

	// Stop any movement we already have
	StopMovement();

	// Clear any enemy
	SetEnemy(NULL);
}

FString ABotAIController::GetDefaultPlayerName()
{
	//if (USpatialNetDriver* SpatialNetDriver = Cast<USpatialNetDriver>(GetNetDriver()))
	//{
	//	return SpatialNetDriver->Connection->GetWorkerId();
	//}
	return "Bot" + FGuid::NewGuid().ToString() + GetName();
}


// Called when the game starts or when spawned
void ABotAIController::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		//if (ABotGameState* GS = GetWorld()->GetGameState<ABotGameState>())
		//{
		//	GS->RegesterPlayer(GetDefaultPlayerName());
		//}
	}

}
