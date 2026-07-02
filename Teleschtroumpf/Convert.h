#pragma once

// Here we define the pin used for the command of the led
#define LED 36

// Here we define the timers for the waiting
#define DOT_TIMER 500
#define DASH_TIMER 1000
#define INTER_DOT_DASH_TIMER 500
#define LETTER_TIMER 1000
#define WORD_TIMER 2000
#define PHRASE_TIMER 3000


String convertCharToMorse(char ch);
String convertPhraseToMorse(String phrase);
void exportdotanddash(char dotdash);