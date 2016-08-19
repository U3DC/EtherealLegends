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

#include "Ethereal.h"
#include "EtherealEnemyMaster.h"

// Sets default values
AEtherealEnemyMaster::AEtherealEnemyMaster(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Get Assets, References Obtained Via Right Click in Editor
	ConstructorHelpers::FObjectFinder<UParticleSystem> HitParticleObject(TEXT("ParticleSystem'/Game/Particles/Emitter/P_blood_splash.P_blood_splash'"));
	ConstructorHelpers::FObjectFinder<UParticleSystem> DeathParticleObject(TEXT("ParticleSystem'/Game/Particles/Emitter/EnemyDeath.EnemyDeath'"));
	ConstructorHelpers::FObjectFinder<UParticleSystem> DisappearParticleObject(TEXT("ParticleSystem'/Game/Particles/Emitter/EnemyDisappear.EnemyDisappear'"));
	ConstructorHelpers::FObjectFinder<USoundCue> DisappearAudioObject(TEXT("SoundCue'/Game/Audio/Party/EnemyDisappear_Cue.EnemyDisappear_Cue'"));
	
	// Set Default Objects
	P_HitFX = HitParticleObject.Object;
	P_DeathFX = DeathParticleObject.Object;
	P_DisappearFX = DisappearParticleObject.Object;
	S_DisappearAudio = DisappearAudioObject.Object;

	// Create
	HitFX = NewObject<UParticleSystemComponent>(this, TEXT("HitFX"));
	DeathFX = NewObject<UParticleSystemComponent>(this, TEXT("DeathFX"));
	DisappearFX = NewObject<UParticleSystemComponent>(this, TEXT("DisappearFX"));
	DisappearAudio = NewObject<UAudioComponent>(this, TEXT("DisappearAudio"));
	
	// Assignment
	HitFX->Template = P_HitFX;
	DeathFX->Template = P_DeathFX;
	DisappearFX->Template = P_DisappearFX;
	DisappearAudio->Sound = S_DisappearAudio;

	HitFX->bAutoActivate = false;
	DeathFX->bAutoActivate = false;
	DisappearFX->bAutoActivate = false;
	DisappearAudio->bAutoActivate = false;

}

// Called when the game starts or when spawned
void AEtherealEnemyMaster::BeginPlay()
{

}

// Sets the Enemy's default stats
void AEtherealEnemyMaster::SetBaseStats()
{
	float HPMod = 2000;
	float StatMod = 7.5f;

	if (IsBoss)
	{
		HPMod = 2000;
		StatMod = 7.5f;
		
	}
	else
	{
		HPMod = 200;
		StatMod = 5.1f;	
	}

	HP_Current = Level * HPMod;
	HP_Max = Level * HPMod;
	ATK = Level * StatMod;
	DEF = Level * StatMod;
	SPD = Level * StatMod;
}

// Force HP Caps keeps the enemy's HP between 0 and Max
bool AEtherealEnemyMaster::ForceHPCaps()
{
	bool Kill = false;

	if (HP_Current > HP_Max)
	{
		HP_Current = HP_Max;
	}
	else if (HP_Current < 0)
	{
		HP_Current = 0;
		Kill = true;
	}

	return Kill;
}

// Calculates the damage dealt by the enemy this turn. Must pass in the base ATK value of the specific attack.
float AEtherealEnemyMaster::DealDamage(float BaseATK)
{
	float atk1 = (ATK + BaseATK) / 32;
	float atk2 = (ATK * BaseATK) / 32;
	float atk3 = atk1 * atk2;
	float atk4 = (atk3 + ATK) * 40;
	DamageOutput = atk4;

	return DamageOutput;
}