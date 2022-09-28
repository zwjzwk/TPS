// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPS/Interfaces/InteractWithCrosshairsInterface.h"
#include "Enemy.generated.h"

UCLASS()
class TPS_API AEnemy : public ACharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;


public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
