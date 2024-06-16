
// OcclusionAwarePlayerController.cpp

#include "OcclusionAwarePlayerController.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Containers/Set.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"

AOcclusionAwarePlayerController::AOcclusionAwarePlayerController()
{
  CapsulePercentageForTrace = 1.0f;
  DebugLineTraces = true;
  IsOcclusionEnabled = true;
}

void AOcclusionAwarePlayerController::BeginPlay()
{
  Super::BeginPlay();
  
  // Make sure to set the values of the components in the blueprint!
}

void AOcclusionAwarePlayerController::SyncOccludedActors()
{
  if (!ShouldCheckCameraOcclusion()) return;
  // Camera is currently colliding, show all current occluded actors
  // and do not perform further occlusion
  if (ActiveSpringArm->bDoCollisionTest)
  {
    ForceShowOccludedActors();
    return;
  }
  if (GetPawn() == nullptr) return;

  FVector Start = ActiveCamera->GetComponentLocation();
  FVector End = GetPawn()->GetActorLocation();

  TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectTypes;
  CollisionObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));

  TArray<AActor*> ActorsToIgnore; 
  TArray<FHitResult> OutHits;

  auto ShouldDebug = DebugLineTraces ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

  bool bGotHits = UKismetSystemLibrary::CapsuleTraceMultiForObjects(
    GetWorld(), Start, End, ActiveCapsuleComponent->GetScaledCapsuleRadius() * CapsulePercentageForTrace,
    ActiveCapsuleComponent->GetScaledCapsuleHalfHeight() * CapsulePercentageForTrace, CollisionObjectTypes, true,
    ActorsToIgnore,
    ShouldDebug,
    OutHits, true);
  if (bGotHits)
  {
    // The list of actors hit by the line trace, that means that they are occluded from view
    TSet<const AActor*> ActorsJustOccluded;

    // Hide actors that are occluded by the camera
    for (FHitResult Hit : OutHits)
    {
      const AActor* HitActor = Cast<AActor>(Hit.GetActor());
      HideOccludedActor(HitActor);
      ActorsJustOccluded.Add(HitActor);
    }

    // Show actors that are currently hidden but that are not occluded by the camera anymore 
    for (auto& Elem : OccludedActors)
    {
      if (!ActorsJustOccluded.Contains(Elem.Value.Actor) && Elem.Value.IsOccluded)
      {
        ShowOccludedActor(Elem.Value);

        if (DebugLineTraces)
        {
          UE_LOG(LogTemp, Warning,
                 TEXT("Actor %s was occluded, but it's not occluded anymore with the new hits."), *Elem.Value.Actor->GetName());
        }
      }
    }
  }
  else
  {
    ForceShowOccludedActors();
  }
}

bool AOcclusionAwarePlayerController::HideOccludedActor(const AActor* Actor)
{
  FCameraOccludedActor* ExistingOccludedActor = OccludedActors.Find(Actor);

  if (ExistingOccludedActor && ExistingOccludedActor->IsOccluded)
  {
    if (DebugLineTraces) UE_LOG(LogTemp, Warning, TEXT("Actor %s was already occluded. Ignoring."),
                                *Actor->GetName());
    return false;
  }

  if (ExistingOccludedActor && IsValid(ExistingOccludedActor->Actor))
  {
    ExistingOccludedActor->IsOccluded = true;
    OnHideOccludedActor(*ExistingOccludedActor);

    if (DebugLineTraces) UE_LOG(LogTemp, Warning, TEXT("Actor %s exists, but was not occluded. Occluding it now."), *Actor->GetName());
  }
  else
  {
    UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(
      Actor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
    if (StaticMesh == nullptr)
    {
      if (DebugLineTraces) UE_LOG(LogTemp, Warning, TEXT("Actor %s does not have a static mesh component. Ignoring."), *Actor->GetName());
      return false;
    }

    FCameraOccludedActor OccludedActor;
    OccludedActor.Actor = Actor;
    OccludedActor.StaticMesh = StaticMesh;
    OccludedActor.Materials = StaticMesh->GetMaterials();
    OccludedActor.IsOccluded = true;
    OccludedActors.Add(Actor, OccludedActor);
    OnHideOccludedActor(OccludedActor);

    if (DebugLineTraces) UE_LOG(LogTemp, Warning, TEXT("Actor %s does not exist, creating and occluding it now."), *Actor->GetName());
  }

  return true;
}


void AOcclusionAwarePlayerController::ForceShowOccludedActors()
{
  for (auto& Elem : OccludedActors)
  {
    if (Elem.Value.IsOccluded)
    {
      ShowOccludedActor(Elem.Value);

      if (DebugLineTraces) UE_LOG(LogTemp, Warning, TEXT("Actor %s was occluded, force to show again."), *Elem.Value.Actor->GetName());
    }
  }
}

void AOcclusionAwarePlayerController::ShowOccludedActor(FCameraOccludedActor& OccludedActor)
{
  if (!IsValid(OccludedActor.Actor))
  {
    OccludedActors.Remove(OccludedActor.Actor);
  }

  OccludedActor.IsOccluded = false;
  OnShowOccludedActor(OccludedActor);
}

bool AOcclusionAwarePlayerController::OnShowOccludedActor(const FCameraOccludedActor& OccludedActor) const
{
  for (int matIdx = 0; matIdx < OccludedActor.Materials.Num(); ++matIdx)
  {
    OccludedActor.StaticMesh->SetMaterial(matIdx, OccludedActor.Materials[matIdx]);
  }

  return true;
}

bool AOcclusionAwarePlayerController::OnHideOccludedActor(const FCameraOccludedActor& OccludedActor) const
{
  for (int i = 0; i < OccludedActor.StaticMesh->GetNumMaterials(); ++i)
  {
    OccludedActor.StaticMesh->SetMaterial(i, FadeMaterial);
  }

  return true;
}

// Fix client-side navigation after possessing a pawn. See https://forums.unrealengine.com/t/characters-dont-simple-move-to-location-after-possessing/833539/5
void AOcclusionAwarePlayerController::SimpleClientNavMove(const FVector& Destination)
{
	UPathFollowingComponent* PathFollowingComp = FindComponentByClass<UPathFollowingComponent>();
	if (PathFollowingComp == nullptr)
	{
		PathFollowingComp = NewObject<UPathFollowingComponent>(this);
		PathFollowingComp->RegisterComponentWithWorld(GetWorld());
		PathFollowingComp->Initialize();
	}

	if (!PathFollowingComp->IsPathFollowingAllowed())
	{
		// After a client respawn we need to reinitialize the path following component
		// The default code path that sorts this out only fires on the server after a Possess
		PathFollowingComp->Initialize();
	}
  PathFollowingComp->UpdateCachedComponents();
	RequestSetNewMoveDestination(Destination);
}

void AOcclusionAwarePlayerController::OnPossess(APawn* InPawn)
{
  if (GetLocalRole() > ROLE_Authority)
  {
    UPathFollowingComponent* PathFollowingComp = FindComponentByClass<UPathFollowingComponent>();
    if (PathFollowingComp)
    {
      PathFollowingComp->UpdateCachedComponents();
    }
  }
  Super::OnPossess(InPawn);
}

// Requests a destination set for the client and server.
void AOcclusionAwarePlayerController::RequestSetNewMoveDestination(const FVector DestLocation)
{
  ClientSetNewMoveDestination(DestLocation);
  ServerSetNewMoveDestination(DestLocation);
}

// Requests a destination set for the client (Comes first, since client calls it by clicking).
void AOcclusionAwarePlayerController::ClientSetNewMoveDestination_Implementation(const FVector DestLocation)
{
  SetNewMoveDestination(DestLocation);
  UE_LOG(LogActor, Warning, TEXT("Client set new move destination: %s"), *DestLocation.ToString());
}

// Requests a destination set for the server (Comes after, to replicate client movement server-side).
void AOcclusionAwarePlayerController::ServerSetNewMoveDestination_Implementation(const FVector DestLocation)
{
  SetNewMoveDestination(DestLocation);
  UE_LOG(LogActor, Warning, TEXT("Server set new move destination: %s"), *DestLocation.ToString());
}

// Common destination setting and movement implementation.
void AOcclusionAwarePlayerController::SetNewMoveDestination(const FVector DestLocation)
{
  UE_LOG(LogActor, Warning, TEXT("Moving towards: %s"), *DestLocation.ToString());
  UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
  /*float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());
  if (APawn* const MyPawn = GetPawn())
  {
    UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);

    // We need to issue move command only if far enough in order for walk animation to play correctly.
    //if (Distance > 120.0f) 
      UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
  }*/
}

// Cancels the active movement of the player.
void AOcclusionAwarePlayerController::CancelActiveMovement_Implementation()
{
  if (APawn* const MyPawn = GetPawn())
  {
    if (UPathFollowingComponent* PathFollowingComp = MyPawn->FindComponentByClass<UPathFollowingComponent>())
    {
      PathFollowingComp->AbortMove(*this, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest);
    }
    if (UCharacterMovementComponent* CharMoveComp = MyPawn->FindComponentByClass<UCharacterMovementComponent>())
    {
      CharMoveComp->StopMovementImmediately();
    }
    UE_LOG(LogActor, Warning, TEXT("Movement cancelled."));
  }
}

