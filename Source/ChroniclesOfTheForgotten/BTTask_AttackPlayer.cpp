// Copyright Epic Games, Inc. All Rights Reserved.

#include "BTTask_AttackPlayer.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "WolfAiCharacter.h" // To call the attack function
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_AttackPlayer::UBTTask_AttackPlayer()
{
    NodeName = "Attack Player";
}

EBTNodeResult::Type UBTTask_AttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController)
    {
        AWolfAiCharacter* Wolf = Cast<AWolfAiCharacter>(AIController->GetPawn());
        AActor* TargetActor = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName));

        if (Wolf && TargetActor)
        {
            // Here we would call a function on the wolf to play an attack animation and apply damage.
            // For now, we'll just log a message.
            if (GEngine)
            {
                FString Msg = FString::Printf(TEXT("%s attacks %s!"), *Wolf->GetName(), *TargetActor->GetName());
                GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, Msg);
            }

            // The task finishes immediately. In a real game, you might wait for an animation to finish.
            return EBTNodeResult::Succeeded;
        }
    }

    return EBTNodeResult::Failed;
}
