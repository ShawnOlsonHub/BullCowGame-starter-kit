// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"


void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    Isograms = GetValidWords(HWords);
    StartGame();   
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if(bGameOver)
    {
        ClearScreen();
        StartGame();
    }
    else
    {
        ProcessGuess(PlayerInput);
    }
}

void UBullCowCartridge::StartGame()
{
    bGameOver = false;
    PrintLine(TEXT("Yall about to play some cow game!\n"));
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    PlayerLives = HiddenWord.Len();

    //Welcome Player
    PrintLine(TEXT("Press enter and guess the %i letter word\n"), HiddenWord.Len()); 
    PrintLine(TEXT("Hint!\nThere are no duplicate letters in the word\n"));

    PrintLine(TEXT("The hidden word is %s"), *HiddenWord);
};

void UBullCowCartridge::EndGame()
{
	bGameOver = true;
    PrintLine(TEXT("Press enter to play again."));
};

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    FBullCowCount Score = GetBullCows(Guess);
    ClearScreen();
    if(Guess == HiddenWord)
    {
        PrintLine(TEXT("You winner you!"));
        EndGame();
        return;
    }

    if(!IsIsogram(Guess))
    {
        PrintLine(TEXT("The word has no duplicate letters, \ntry again!"));
        return;
    }

    if(Guess.Len() == HiddenWord.Len())
    {
        PrintLine(TEXT("Oops! Try again.\n"));  
        Guesses.Emplace(Guess);
        --PlayerLives;
        PrintLine(TEXT("You have %i correct letters and %i misses!"), Score.Hits, Score.Misses);
        PrintLine(TEXT("A Hit is a letter in the exact right place \nA Miss means a letter is wrong"));
        PrintLine(TEXT("Here are the words you have tried:"));
        for (int32 GuessIndex = 0; GuessIndex < Guesses.Num(); GuessIndex++)
        {
            PrintLine(TEXT("%s"), *Guesses[GuessIndex]); 
        }    
    }
    else
    { 
        PrintLine(TEXT("Thats the wrong number of letters!\nTry again to guess the %i letter word\nYou have %i lives left."), HiddenWord.Len(), PlayerLives);
    }

    if(PlayerLives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("Aw you have run out of guesses :C\n"));
        PrintLine(TEXT("Would you like to try again?\nType yes to restart, or no to give up!"));
        if(Guess == "yes")
        {
            ClearScreen();
            PlayerLives = HiddenWord.Len();
            PrintLine(TEXT("Ok, try again!\nYou need to guess the %i letter word!\n"), HiddenWord.Len());
        }
        else if(Guess == "no")
        {
            ClearScreen();
            PrintLine(TEXT("Aw beans, thanks for playing!"));
            PrintLine(TEXT("Your hidden word was %s"), *HiddenWord);
            EndGame();
            return;
        }        
    }
};

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    int32 Index = 0;
    //Check each letter in the word against the Index, then increment the index to check the each letter
    for (Index; Index < Word.Len(); Index++)
    {
        for(int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if(Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    
    return true;      
};

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;
    for (FString CurrentWord : WordList)
    {
        if(CurrentWord.Len() >= 4 && CurrentWord.Len() <= 8 && IsIsogram(CurrentWord))
        {
            ValidWords.Emplace(CurrentWord);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    //Hit is a letter in the exact right spot
    //Miss is any letter in the wrong spot
    //Check to see if they letter at the index, is the same as the letter at that index in the hidden word.
    for(int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if(Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Hits++;
            continue;
        }
        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if(Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Misses++;
                break;
            }
        }    
    }
    return Count;
}