// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

struct FBullCowCount
{
	int32 Hits = 0;
	int32 Misses = 0;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& Input) override;
	void StartGame();
	void EndGame();
	void ProcessGuess(const FString& Guess);
	bool IsIsogram(const FString& Word) const;
	TArray<FString> GetValidWords(const TArray<FString>& WordList) const;
	//Using out parameters for hits and misses
	FBullCowCount GetBullCows(const FString& Guess) const;
	
	private:
	FString HiddenWord;
	TArray<FString> Guesses;
	int32 PlayerLives;
	bool bGameOver;
	TArray<FString> Isograms;
};
