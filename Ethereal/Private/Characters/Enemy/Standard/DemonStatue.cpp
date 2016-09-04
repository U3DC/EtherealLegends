// © 2014 - 2016 Soverance Studios
// http://www.soverance.com

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
#include "SoulEater.h"
#include "DemonStatue.h"

ADemonStatue::ADemonStatue(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Asset, Reference Obtained Via Right Click in Editor
	static ConstructorHelpers::FObjectFinder<UDestructibleMesh> DestructibleMeshObject(TEXT("DestructibleMesh'/Game/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Statues/StaticMesh/SM_Plains_Castle_DemonDoor_01_DM.SM_Plains_Castle_DemonDoor_01_DM'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> FlareRParticleObject(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Elemental/ICE/Souleater_Statue_EyeFlare.Souleater_Statue_EyeFlare'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> FlareLParticleObject(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Elemental/ICE/Souleater_Statue_EyeFlare.Souleater_Statue_EyeFlare'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> IdleRParticleObject(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Chicken/Souleater_Statue_EyeIdle.Souleater_Statue_EyeIdle'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> IdleLParticleObject(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Chicken/Souleater_Statue_EyeIdle.Souleater_Statue_EyeIdle'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> StompBlastParticleObject(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/combat/SoulEater-Statue-StompBlast.SoulEater-Statue-StompBlast'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> SoulSpawnParticleObject(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/Fire/combat/SoulEaterSpawn.SoulEaterSpawn'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> JumpAudioObject(TEXT("SoundCue'/Game/Audio/Rocks/SoulEater_Statue_Jump_Cue.SoulEater_Statue_Jump_Cue'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> StompAudioObject(TEXT("SoundCue'/Game/Audio/Rocks/SoulEater_Statue_Stomp_Cue.SoulEater_Statue_Stomp_Cue'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> ExplosionAudioObject(TEXT("SoundCue'/Game/Audio/Impacts/Rock_Explo_Slowed02_L_Cue.Rock_Explo_Slowed02_L_Cue'"));

	// Set Default Objects
	DM_DemonDoor = DestructibleMeshObject.Object;
	P_EyeFlareFX_R = FlareRParticleObject.Object;
	P_EyeFlareFX_L = FlareLParticleObject.Object;
	P_EyeIdleFX_R = IdleRParticleObject.Object;
	P_EyeIdleFX_L = IdleRParticleObject.Object;
	P_SoulSpawnFX_L = SoulSpawnParticleObject.Object;
	P_SoulSpawnFX_R = SoulSpawnParticleObject.Object;
	P_StompBlast = StompBlastParticleObject.Object;
	S_JumpAudio = JumpAudioObject.Object;
	S_StompAudio = StompAudioObject.Object;
	S_ExplosionAudio = ExplosionAudioObject.Object;

	Name = EEnemyNames::EN_DemonStatue;
	Realm = ERealms::R_Shiitake;
	BattleType = EBattleTypes::BT_Standard;

	GetCapsuleComponent()->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	GetCharacterMovement()->MaxAcceleration = 20;

	// Targeting Reticle config
	TargetingReticle->SetRelativeLocation(FVector(0, 0, 160));
	TargetingReticle->SetRelativeRotation(FRotator(0, 0, 180));
	TargetingReticle->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

	// Hit & Death Effect Config
	HitFX->SetRelativeScale3D(FVector(0.6f, 0.6f, 0.6f));
	DeathFX->SetRelativeLocation(FVector(0, 0, -88));
	DeathFX->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));
	DisappearFX->SetRelativeLocation(FVector(0, 0, -75));
	DisappearFX->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));

	// Pawn A.I. config
	PawnSensing->HearingThreshold = 150;
	PawnSensing->LOSHearingThreshold = 200;
	PawnSensing->SightRadius = 500;
	PawnSensing->SetPeripheralVisionAngle(75.0f);

	// Create and configure other components
	StompRadius = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("StompRadius"));
	StompRadius->SetupAttachment(RootComponent);
	StompRadius->SetSphereRadius(300.0f);

	DemonDoor = ObjectInitializer.CreateDefaultSubobject<UDestructibleComponent>(this, TEXT("DemonDoor"));
	DemonDoor->SetupAttachment(RootComponent);
	DemonDoor->SetDestructibleMesh(DM_DemonDoor);
	DemonDoor->SetRelativeLocation(FVector(0, 0, -88));
	DemonDoor->SetRelativeRotation(FRotator(0, -90, 0));

	EyeFlareFX_R = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("EyeFlareFX_R"));
	EyeFlareFX_R->SetupAttachment(DemonDoor);
	EyeFlareFX_R->Template = P_EyeFlareFX_R;
	EyeFlareFX_R->SetRelativeLocation(FVector(14, 28, 168));
	EyeFlareFX_R->bAutoActivate = false;

	EyeFlareFX_L = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("EyeFlareFX_L"));
	EyeFlareFX_L->SetupAttachment(DemonDoor);
	EyeFlareFX_L->Template = P_EyeFlareFX_L;
	EyeFlareFX_L->SetRelativeLocation(FVector(-14, 28, 168));
	EyeFlareFX_L->bAutoActivate = false;

	EyeIdleFX_R = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("EyeIdleFX_R"));
	EyeIdleFX_R->SetupAttachment(DemonDoor);
	EyeIdleFX_R->Template = P_EyeIdleFX_R;
	EyeIdleFX_R->SetRelativeLocation(FVector(10, 28, 168));
	EyeIdleFX_R->bAutoActivate = false;

	EyeIdleFX_L = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("EyeIdleFX_L"));
	EyeIdleFX_L->SetupAttachment(DemonDoor);
	EyeIdleFX_L->Template = P_EyeIdleFX_L;
	EyeIdleFX_L->SetRelativeLocation(FVector(10, 28, 168));
	EyeIdleFX_L->bAutoActivate = false;

	SoulSpawnFX_L = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("SoulSpawnFX_L"));
	SoulSpawnFX_L->SetupAttachment(RootComponent);
	SoulSpawnFX_L->Template = P_SoulSpawnFX_L;
	SoulSpawnFX_L->SetRelativeLocation(FVector(-150, 0, -50));
	SoulSpawnFX_L->bAutoActivate = false;

	SoulSpawnFX_R = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("SoulSpawnFX_R"));
	SoulSpawnFX_R->SetupAttachment(RootComponent);
	SoulSpawnFX_R->Template = P_SoulSpawnFX_R;
	SoulSpawnFX_R->SetRelativeLocation(FVector(150, 0, -50));
	SoulSpawnFX_R->bAutoActivate = false;

	StompBlast = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("StompBlast"));
	StompBlast->SetupAttachment(RootComponent);
	StompBlast->Template = P_StompBlast;
	StompBlast->SetRelativeLocation(FVector(0, 0, -132));
	StompBlast->SetRelativeScale3D(FVector(3, 3, 3));
	StompBlast->bAutoActivate = false;

	JumpAudio = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("JumpAudio"));
	JumpAudio->SetupAttachment(RootComponent);
	JumpAudio->Sound = S_JumpAudio;
	JumpAudio->bAutoActivate = false;

	StompAudio = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("StompAudio"));
	StompAudio->SetupAttachment(RootComponent);
	StompAudio->Sound = S_StompAudio;
	StompAudio->bAutoActivate = false;

	ExplosionAudio = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("ExplosionAudio"));
	ExplosionAudio->SetupAttachment(RootComponent);
	ExplosionAudio->Sound = S_ExplosionAudio;
	ExplosionAudio->bAutoActivate = false;
}

// Called when the game starts or when spawned
void ADemonStatue::BeginPlay()
{
	Super::BeginPlay();

	PawnSensing->OnHearNoise.AddDynamic(this, &ADemonStatue::OnHearNoise);  // bind the OnHearNoise event
	PawnSensing->OnSeePawn.AddDynamic(this, &ADemonStatue::OnSeePawn);  // bind the OnSeePawn event
	OnDeath.AddDynamic(this, &ADemonStatue::Death); // bind the death fuction to the OnDeath event 
}

void ADemonStatue::ToggleEyes(bool ShowEyes)
{
	if (ShowEyes)
	{
		EyeFlareFX_R->Activate();
		EyeFlareFX_L->Activate();
		EyeIdleFX_R->Activate();
		EyeIdleFX_R->Activate();
	}
	else
	{
		EyeFlareFX_R->Deactivate();
		EyeFlareFX_L->Deactivate();
		EyeIdleFX_R->Deactivate();
		EyeIdleFX_R->Deactivate();
	}
}

void ADemonStatue::RunToTarget()
{
	if (!IsDead)
	{
		if (!Target->IsDead)
		{
			// TO DO:  Run to target. This function is currently in Blueprints.
			// When the enemy gets within melee range of the player, do a melee attack.
			// after the attack delay, run this function again
		}
	}
}

void ADemonStatue::Death()
{
	DemonDoor->ApplyRadiusDamage(100, this->GetActorLocation(), 100, 100, true);  // Destroy the door
	ToggleEyes(false);
	FinalDeath(true, false);
	// TO DO :  destroy the AoE decal
}

void ADemonStatue::SpawnSoulEaters()
{
	ExplosionAudio->Play();
	SoulSpawnFX_L->Activate();
	SoulSpawnFX_R->Activate();

	AEtherealEnemyMaster* SoulEaterL = GetWorld()->SpawnActor<ASoulEater>(SoulSpawnFX_L->GetComponentLocation(), SoulSpawnFX_L->GetComponentRotation());
	if (SoulEaterL)
	{
		//SoulEaterL->SpawnDefaultController();
		SoulEaterL->Level = Level;
		SoulEaterL->SetBaseStats();
	}
	
	AEtherealEnemyMaster* SoulEaterR = GetWorld()->SpawnActor<ASoulEater>(SoulSpawnFX_R->GetComponentLocation(), SoulSpawnFX_R->GetComponentRotation());
	if (SoulEaterR)
	{
		//SoulEaterR->SpawnDefaultController();
		SoulEaterR->Level = Level;
		SoulEaterR->SetBaseStats();
	}
}

void ADemonStatue::StompAttack()
{
	JumpAudio->Play();
	FVector JumpStartLocation = this->GetActorLocation();
	// TO DO :  Run a timeline that lerps this actor into the air to simulate a jump
	// use JumpStartLocation as the start, and JumpStartLocation + 60 on Z as the End
	// Timeline has a two second curve, 0.75 second to go up to 1, 0.5 second delay, and another 0.75 seconds down to 0.
	// An event occurs at 0.75 spawns the AoE decal
	// When the timeline ends, destroy the AoE decal, then continue with the rest of the code
	StompBlast->Activate();
	StompAudio->Play();

	TArray<AActor*> Overlapping;  // define a local array to store hits
	StompRadius->GetOverlappingActors(Overlapping, AEtherealPlayerMaster::StaticClass()); // check for overlapping players within the stomp radius

	for (AActor* Actor : Overlapping) // for each actor found overlapping
	{
		AEtherealPlayerMaster* Player = Cast<AEtherealPlayerMaster>(Actor);  // cast to Player Master

		if (Player) // if succeeded
		{
			// TO DO :  Make the player take damage
		}
	}

	// TO DO :  call this function again after the attack delay
}

void ADemonStatue::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{

	const FString VolumeDesc = FString::Printf(TEXT(" at volume %f"), Volume);
	FString message = TEXT("Heard Actor ") + PawnInstigator->GetName() + VolumeDesc;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);

	// TODO: game-specific logic    
}

void ADemonStatue::OnSeePawn(APawn* Pawn)
{
	if (!IsAggroed)
	{
		FString message = TEXT("Saw Actor ") + Pawn->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);

		ToggleEyes(true);
		Aggro(Pawn);
		//SpawnSoulEaters();
		RunToTarget();
	}	
}
