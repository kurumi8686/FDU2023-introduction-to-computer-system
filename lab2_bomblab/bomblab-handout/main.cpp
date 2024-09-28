/*
 *  JWC's Bomb+, Version 3.3
 *  Copyright 2023, FDU JWC. All rights reserved.
 *  
 *  LICENSE:
 *
 *  FDU JWC hereby grants you (the VICTIM) explicit permission to use
 *  this bomb. This is a time limited license, which expires on the
 *  death of the VICTIM.
 *  We take no responsibility for damage, frustration, insanity,
 *  bug-eyes, carpal-tunnel syndrome, loss of sleep, or other harm to
 *  the VICTIM.
 *  This license is null and void where the BOMB is prohibited by law.
 */

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "phases.h"

/*
 * Note to self: Remember to erase this file so my victims will have
 * no idea what is going on, and so they will all blow up in a
 * spectaculary fiendish explosion.
 */

void read_line(char* input)
{
    char ch;
    int i;
    for (i = 0; i <= 40; i++) {
        ch = getchar();
        if (ch == '\n') {
            input[i] = '\0';
            return;
        } else {
            input[i] = ch;
        }
    }
    input[i] = '\0';

    while (getchar() != '\n'); // clear the input buffer
}

int main()
{
    char input[40];
    long secret_key = 0;

    printf("You have 6 phases with which to blow yourself up. Have a nice day!\n");

    read_line(input);
    phase_1(input);
    puts("Phase 1 defused. How about the next one?");

    read_line(input);
    phase_2(input);
    puts("That's number 2. Keep going!");

    read_line(input);
    phase_3(input);
    puts("Halfway there!");

    read_line(input);
    phase_4(input);
    puts("So you got that one. Try this one.");

    read_line(input);
    phase_5(input);
    puts("Good work! On to the next...");

    read_line(input);
    phase_6(input);
    puts("Cool! your skill on Reverse Engineering is great.");

    /* Note to self: The secret key should be zero when release! */
    if (secret_key) {
        /* The code shouldn't go here */

        slow_put("Welcome to the secret phase of Bomb+!\n");
        slow_put("It is called");
        slow_slow_put("... THE STEINS;GATE\n");

        read_line(input);
        secret_phase(input);

        puts("You are really a Master of Reverse Engineering!");
        true_ending();
    }

    return 0;
}
