// Copyright Epic Games, Inc. All Rights Reserved.

#include "BTTask_FindRandomPatrolLocation.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomPatrolLocation::UBTTask_FindRandomPatrolLocation()
{
    NodeName = "Find Random Patrol Location";
    PatrolRadius = 500.0f;
}

EBTNodeResult::Type UBTTask_FindRandomPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    APawn* ControlledPawn = AIController->GetPawn();

    if (AIController && ControlledPawn)
    {
        UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
        if (NavSystem)
        {
            FVector Origin = ControlledPawn->GetActorLocation();
            FNavLocation RandomLocation;

            if (NavSystem->GetRandomPointInNavigableRadius(Origin, PatrolRadius, RandomLocation))
            {
                AIController->GetBlackboardComponent()->SetValueAsVector(PatrolLocationKey.SelectedKeyName, RandomLocation.Location);
                return EBTNodeResult::Succeeded;
            }
        }
    }

    return EBTNodeResult::Failed;
}
