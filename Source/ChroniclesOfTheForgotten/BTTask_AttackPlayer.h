// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AttackPlayer.generated.h"

/**
 *
 */
UCLASS()
class CHRONICLESOFTHEFORGOTTEN_API UBTTask_AttackPlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_AttackPlayer();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

};
