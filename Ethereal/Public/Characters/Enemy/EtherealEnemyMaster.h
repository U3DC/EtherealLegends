// © 2014 - 2016 Soverance Studios
// www.soverance.com

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

// http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "EnemyNames.h"
#include "Characters/EtherealCharacterMaster.h"
#include "Characters/Player/EtherealPlayerMaster.h"
#include "Management/BattleTypes.h"
#include "EtherealEnemyMaster.generated.h"

UCLASS()
class ETHEREAL_API AEtherealEnemyMaster : public AEtherealCharacterMaster
{
	GENERATED_BODY()
	
public:

	AEtherealEnemyMaster(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	// DEFAULTS

	// The name of this enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	EEnemyNames Name;

	// What type of battle this enemy initiates
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	EBattleTypes BattleType;

	// Enemy's current target. will always be the player
	// Change this to an Ethereal Player Master soon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	AEtherealPlayerMaster* Target;

	// CONTROL STATES
	
	// Whether or not the enemy is in range of the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Controls)
	bool InRange;

	// Whether or not the enemy is currently moving
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Controls)
	bool IsMoving;

	// Whether or not the enemy was hit by the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Controls)
	bool IsHit;

	// Whether or not the enemy is a boss enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Controls)
	bool IsBoss;

	// Whether or not the enemy has been aggroed by the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Controls)
	bool IsAggroed;

	// BASE STATS

	// Enemy Level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float Level;

	// Enemy ATK value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float ATK;

	// Enemy DEF value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float DEF;

	// Enemy SPD value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float SPD;

	// Enemy Current HP value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float HP_Current;

	// Enemy Max HP value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float HP_Max;

	// The time this enemy will wait between taking turns
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float AttackDelay;

	// The damage output of this enemy. Used to calculate final damage taken by the player.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float DamageOutput;

	// COMPONENTS

	/** This emitter is active when the enemy is hit. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
	UParticleSystemComponent* HitFX;
	UParticleSystem* P_HitFX;

	/** This emitter is active when this enemy dies. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
	UParticleSystemComponent* DeathFX;
	UParticleSystem* P_DeathFX;

	/** Effect played when enemy vanishes after dying. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
	UParticleSystemComponent* DisappearFX;
	UParticleSystem* P_DisappearFX;

	// Disappear Audio
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
	UAudioComponent* DisappearAudio;
	USoundCue* S_DisappearAudio;

	// Melee Radius Collider
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	USphereComponent* MeleeRadius = CreateDefaultSubobject<USphereComponent>(TEXT("MeleeRadius"));

	// FUNCTIONS

	// Sets the initial stats of the player, based on his level
	UFUNCTION(BlueprintCallable, Category = Stats)
	void SetBaseStats();

	// Sets the initial stats of the player, based on his level
	UFUNCTION(BlueprintCallable, Category = Stats)
	bool ForceHPCaps();

	// Sets the initial stats of the player, based on his level
	UFUNCTION(BlueprintCallable, Category = Stats)
	float DealDamage(float BaseATK);
};
