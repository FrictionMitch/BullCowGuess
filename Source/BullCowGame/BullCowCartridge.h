// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

struct FBullCowCount
{
	int32 Bulls = 0;
	int32 Cows = 0;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString &Input) override;

	void SetupGame();
	void EndGame();
	void PlayerGuess(const FString &Guess);
	void CheckGuessLength(const FString &Guess);
	void GetBullCows(const FString& Guess, int32& BullCount, int32& CowCount) const;

	FBullCowCount GetBullCows(const FString& Guess) const;

	bool IsIsogram(const FString &Guess) const;

	TArray<FString> GetValidWords(const TArray<FString> &List);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Word Length")
		int32 MinWordLength = 4;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Word Length")
		int32 MaxWordLength = 8;

#if WITH_EDITOR
	// Editor-centric code for changing properties
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
		override;
#endif

	// Your declarations go below!
	private:
	
	bool bIsCorrectLength;
	bool bIsIsogram;
	bool bGameOver;
	int32 Lives;
	int32 IsogramLength;
	FString HiddenWord;
	TArray<FString> ValidWords;
};
