// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotSpeedPickup.h"

#include "TPS/BlasterComponents/BuffComponent.h"
#include "TPS/Character/BlasterCharacter.h"

void AShotSpeedPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
	if(BlasterCharacter)
	{
		UBuffComponent* Buff = BlasterCharacter->GetBuff();
		if(Buff)
		{
			Buff->BuffShotSpeed(ShotSpeedRate, BuffTime);
		}
		
		Destroy();
	}
}

