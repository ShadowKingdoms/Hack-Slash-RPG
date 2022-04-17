#pragma once

#include "CoreMinimal.h"
#include "LostArcAbilityInterface.h"
#include "Components/ActorComponent.h"
#include "LostArcCharacterEquipComponent.generated.h"

UENUM(BlueprintType)
enum EAccessoryType
{
	Necklace UMETA(DisplayName = "Necklace"),
	Earring UMETA(DisplayName = "Earring"),
	Ring UMETA(DisplayName = "Ring")
};

USTRUCT(Atomic, BlueprintType)
struct FEquipSlot
{
	GENERATED_BODY()

	UPROPERTY() // Heap 영역을 가리키는 포인터는 UPROPERTY() 매크로가 필수이다. UPROPERTY()가 없으면 할당된 heap 영역을 담당하는 포인터가 없는 걸로 인식하여 GC가 힙영역을 null로 만들기 때문이다.
	TArray<class ULostArcItemEquipBase*> EquipArray;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquipSlotUpdateDelegate, int32);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LOSTARC_API ULostArcCharacterEquipComponent : public UActorComponent, public ILostArcAbilityInterface
{
	GENERATED_BODY()
	
public:	
	FOnEquipSlotUpdateDelegate EquipSlotUpdate;
	
	ULostArcCharacterEquipComponent();
	virtual void UseAbility(int32 SlotIndex) override;
	virtual void SwappingSlot(int32 OwnerIndex, int32 DistIndex, UActorComponent* OwnerComponent) override;
	virtual bool SetAbility(ULostArcAbilityBase* OwnerAbility, int32 SlotIndex = -1) override;
	virtual ULostArcAbilityBase* GetAbility(int32 SlotIndex, bool bTrans = false) override;
	bool EquipSlotEmptyCheck(ULostArcItemEquipBase* CheckItem);
	
	EAccessoryType IndexDecoding(int32 &SlotIndex, bool bTrans = true);
	int32 IndexEncoding(EAccessoryType AcType, int32 Index);
	
protected:
	virtual void InitializeComponent() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item, meta = (AllowPrivateAccess = true))
	TMap<TEnumAsByte<EAccessoryType>, int32> EquipMaxSlot;

	UPROPERTY()
	TMap<TEnumAsByte<EAccessoryType>, FEquipSlot> EquipSlot;
	
	ILostArcAbilityInterface* Interface;
};