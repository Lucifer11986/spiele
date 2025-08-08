// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindRandomPatrolLocation.generated.h"

/**
 *
 */
UCLASS()
class CHRONICLESOFTHEFORGOTTEN_API UBTTask_FindRandomPatrolLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FindRandomPatrolLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector PatrolLocationKey;

	UPROPERTY(EditAnywhere, Category = "AI")
	float PatrolRadius;
};
