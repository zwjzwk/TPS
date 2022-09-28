// Fill out your copyright notice in the Description page of Project Settings.


#include "Shotgun.h"

#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "TPS/Character/BlasterCharacter.h"
#include "TPS/Enemy/Enemy.h"

void AShotgun::Fire(const FVector& HitTarget)
{
	AWeapon::Fire(HitTarget);
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	AController* InstigatorController = OwnerPawn->GetController();

	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (MuzzleFlashSocket)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector Start = SocketTransform.GetLocation();

		TMap<AActor*, uint32> HitMap; //击中身体的子弹
		TMap<AActor*, int32> HeadShotHitMap; //击中头部的子弹
		for (uint32 i = 0; i < NumberOfPellets; i++)
		{
			FHitResult FireHit;
			WeaponTraceHit(Start, HitTarget, FireHit);

			AActor* Enemy = Cast<ABlasterCharacter>(FireHit.GetActor()) ? nullptr : FireHit.GetActor();
			if (Enemy && HasAuthority() && InstigatorController)
			{
				const bool bHeadShot = FireHit.BoneName.ToString() == FString("head");
				if(bHeadShot)
				{
					if (HeadShotHitMap.Contains(Enemy))
					{
						HeadShotHitMap[Enemy]++;
					}
					else
					{
						HeadShotHitMap.Emplace(Enemy, 1);
					}
				}
				else
				{
					if (HitMap.Contains(Enemy))
					{
						HitMap[Enemy]++;
					}
					else
					{
						HitMap.Emplace(Enemy, 1);
					}
				}
				
			}

			if (ImpactParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					ImpactParticles,
					FireHit.ImpactPoint,
					FireHit.ImpactNormal.Rotation()
				);
			}
			if (HitSound)
			{
				UGameplayStatics::PlaySoundAtLocation(
					this,
					HitSound,
					FireHit.ImpactPoint,
					.5f,
					FMath::FRandRange(-.5f, .5f)
				);
			}
		}

		TMap<AActor*, float> DamageMap; //总伤害表
		for (auto HitPair : HitMap) //遍历身体伤害，加进伤害表
		{
			if (HitPair.Key)
			{
				DamageMap.Emplace(HitPair.Key, HitPair.Value * Damage);
			}
		}
		
		for (auto HeadShotHitPair : HeadShotHitMap) //遍历头部伤害，加进伤害表
		{
			if (HeadShotHitPair.Key)
			{
				if(DamageMap.Contains(HeadShotHitPair.Key))
				{
					DamageMap[HeadShotHitPair.Key] += HeadShotHitPair.Value * Damage * 1.4f;
				}
				else
				{
					DamageMap.Emplace(HeadShotHitPair.Key, HeadShotHitPair.Value * Damage * 1.4f);
				}
			}
		}
		
		for (auto DamagePair : DamageMap) //应用总伤害
		{
			if (DamagePair.Key && HasAuthority() && InstigatorController)
			{
				UGameplayStatics::ApplyDamage(
					DamagePair.Key,
					DamagePair.Value,
					InstigatorController,
					this,
					UDamageType::StaticClass()
				);
			}
		}
	}
}
