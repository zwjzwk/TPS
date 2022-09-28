// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuffComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_API UBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBuffComponent();
	friend class ABlasterCharacter;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Heal(float HealAmount, float HealingTime); //回血buff
	void BuffSpeed(float BuffBaseSpeed, float BuffCrouchSpeed, float BuffTime); //加速buff
	void BuffShotSpeed(float Rate, float BuffTime); //射速buff
	void SetInitialSpeeds(float BaseSpeed, float CrouchSpeed);

protected:
	virtual void BeginPlay() override;
	void HealRampUp(float DeltaTime);
	

private:
	UPROPERTY()
	class ABlasterCharacter* Character;

	//回血buff相关
	bool bHealing = false;
	float HealingRate = 0.f;
	float AmountToHeal = 0.f;

	//加速buff相关
	FTimerHandle SpeedBuffTimer;
	void ResetSpeed();
	float InitialBaseSpeed;
	float InitialCrouchSpeed;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpeedBuff(float BaseSpeed, float CrouchSpeed);

	//射速buff相关
	FTimerHandle ShotBuffTimer;
	void ResetShotSpeed();
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastShotSpeedBuff(float Rate);

};
