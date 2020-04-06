// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"
#include "Math/UnrealMathUtility.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    FBullCowCount Count;

    SetupGame();

}

void UBullCowCartridge::OnInput(const FString &PlayerInput) // When the player hits enter
{

    if (!bGameOver)
    {
        PlayerGuess(PlayerInput);
    }
    else
    {
        SetupGame();
    }
}

void UBullCowCartridge::SetupGame()
{
    bGameOver = false;

    MinWordLength = 6;
    MaxWordLength = 12;

    GetValidWords(WordList); // Get Words from master list and make sure they are valid

    HiddenWord = ValidWords[FMath::FRandRange(0, ValidWords.Num()-1)];
    Lives = HiddenWord.Len();

    ClearScreen();

    PrintLine(FString::Printf(TEXT("%s"), *HiddenWord));

    PrintLine(TEXT("Welcome to the \"Bull / Cow\" Game!\n"));
    PrintLine(FString::Printf(TEXT("Guess the %i letter ISOGRAM"), HiddenWord.Len()));
    PrintLine(TEXT("You have %i lives left"), Lives);
    PrintLine(TEXT("Type your guess and Press \"ENTER\""));
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(FString::Printf(TEXT("\n The word was %s\n"), *HiddenWord));
    PrintLine(FString::Printf(TEXT("Game Over!")));
}

void UBullCowCartridge::PlayerGuess(const FString &Guess)
{
    // win condition
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("You Win!...Press \"Enter\" to play again"));
        bGameOver = true;
        return;
    }
    else
    {
        // Check Length
        if (Guess.Len() != HiddenWord.Len())
        {
            PrintLine(TEXT("Your Guess contained %i character(s)"), Guess.Len());
            PrintLine(TEXT("Guess a word that contains %i characters"), HiddenWord.Len());
        }
        else
        {
            // Check Isogram
            if (!IsIsogram(Guess))
            {
                // print shit
                PrintLine("Not an ISOGRAM...\nAn ISOGRAM has no repeating characters");
            }
            else
            {
                --Lives;
                // Check Lives
                if (Lives >= 1)
                {
                    //// Show the Bulls / Cows
                    //int32 Bulls, Cows;
                    //GetBullCows(Guess, Bulls, Cows);
                    FBullCowCount Score = GetBullCows(Guess);
                    //PrintLine(TEXT("You have %i Bulls and %i Cows"), Bulls, Cows);
                    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);

                    PrintLine(TEXT("Try again..."));
                    PrintLine(FString::Printf(TEXT("You have %i lives left"), Lives));
                }
                else
                {
                    EndGame();
                    return;
                }
            }
        }
    }
}

void UBullCowCartridge::CheckGuessLength(const FString &Guess)
{
    if (Guess.Len() != HiddenWord.Len())
    {
        bIsCorrectLength = false;
        PrintLine(TEXT("Your Guess contained %i character(s)"), Guess.Len());
        PrintLine(TEXT("Guess a word that contains %i characters"), HiddenWord.Len());
    }
    else
    {
        bIsCorrectLength = true;
        return;
    }
}

bool UBullCowCartridge::IsIsogram(const FString &Guess) const
{
    for (int i = 0; i < Guess.Len(); i++)
    {
        for (int j = i + 1; j < Guess.Len(); j++)
        {
            if (Guess[i] == Guess[j])
            {
                return false;
                PrintLine(FString::Printf(TEXT("You repeated a letter")));
                PrintLine(FString::Printf(TEXT("%c:%c"), Guess[i], Guess[j]));
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString> &List)
{
    for (FString word : List)
    {
        if (IsIsogram(word))
        {
            if (word.Len() >= MinWordLength && word.Len() <= MaxWordLength)
            {
                ValidWords.Emplace(word);
            }
        }
    }
    return ValidWords;
}

void UBullCowCartridge::GetBullCows(const FString& Guess, int32& BullCount, int32& CowCount) const
{
    BullCount = 0;
    CowCount = 0;

    for (int32 i = 0; i < Guess.Len(); i++)
    {
        if (Guess[i] == HiddenWord[i])
        {
            BullCount++;
            continue;
        }
        for (int32 j = 0; j < Guess.Len(); j++)
        {
            if (Guess[i] == HiddenWord[j])
            {
                CowCount++;
                break;
            }
        }
    }
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 i = 0; i < Guess.Len(); i++)
    {
        if (Guess[i] == HiddenWord[i])
        {
            Count.Bulls++;
            continue;
        }
        for (int32 j = 0; j < Guess.Len(); j++)
        {
            if (Guess[i] == HiddenWord[j])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}

#if WITH_EDITOR
void UBullCowCartridge::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    MinWordLength = 4;
    MaxWordLength = 8;

    Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif