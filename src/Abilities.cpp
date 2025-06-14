#include "Abilities.hpp"
#include "Utils.hpp"

namespace Abilities {
    void GiveAbility(UGameplayAbility* GameplayAbility) {
		auto DefaultGameplayEffect = UObject::FindObject<UGameplayEffect>("GE_Athena_PurpleStuff_Health_C GE_Athena_PurpleStuff_Health.Default__GE_Athena_PurpleStuff_Health_C");

		DefaultGameplayEffect->GrantedAbilities[0].Ability = GameplayAbility;
		DefaultGameplayEffect->DurationPolicy = EGameplayEffectDurationType::Infinite;

		auto GameplayEffect = UObject::FindObject<UGameplayEffect>("BlueprintGeneratedClass GE_Athena_PurpleStuff_Health.GE_Athena_PurpleStuff_Health_C");

		Globals::GPlayerPawn->AbilitySystemComponent->BP_ApplyGameplayEffectToSelf(GameplayEffect, 1.0f, SDK::FGameplayEffectContextHandle());

		CLOG("Granted Ability!");
	}
}