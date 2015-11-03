// Author: Emmanuel Odeke <emm.odeke@gmail.com>
// An implementation of a dterministic one-tape Turing machine that on a binary non-negative
// input x, determines whether x is divisible by three.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef enum {
    Unknown=-1, Rejected=0, Accepted=1,
} State;

size_t bitLength(const size_t v) {
    if (v <= 1)
        return 1;

    size_t len = 0; 
    size_t temp = 1;

    while (temp <= v) {
        len += 1;
        temp <<= 1;
    }

    return len;
}

State divisibleByThree(const char *tape) {
    // Idea here is from examination of normal
    // binary division by means of XOR-ing from the 
    // MSB with 0x3 or 0b11

    // Inclusion of Epsilon the empty string
    // Should produce an accepting state
    if (tape == NULL)
        return Accepted;

    size_t len = strlen(tape);
    if (len < 1)
        return Accepted;

    size_t i;
    long long unsigned int bitfilling = 0;

    // Create the number itself
    for (i = len; i > 0; i--) {
        size_t iprecise = i - 1;
        if (tape[iprecise] != '1')
            continue;

        bitfilling |= 1 << (len - i);
    }
            
    while (bitfilling > 1) {
        int bl = bitLength(bitfilling);
        int bx = bl - 2;
        bitfilling ^= (size_t)(0x3 << bx);
    }

    // No remainder expected if truly divisible by 3
    return bitfilling == 1 ? Rejected : Accepted;
}

void printState(State st) {
    printf("%s", st == Accepted ? "accepted" : (st == Rejected ? "rejected": "unknown"));
}

int main() {
    struct tc{
        char *tape;
        State finalState;
    }testCases[] = {
            {.tape="110", .finalState=Accepted},
            {.tape="0", .finalState=Accepted},
            {.tape="1", .finalState=Rejected},
            {.tape="10", .finalState=Rejected},
            {.tape="1001", .finalState=Accepted},
            {.tape="11000011010011111", .finalState=Accepted},
    };

    size_t i;
    size_t sz = sizeof(testCases)/sizeof(testCases[0]);

    for (i=0; i < sz; ++i) {
        struct tc curTC = testCases[i];

        State finalState = divisibleByThree(curTC.tape);
        assert(finalState == curTC.finalState);

    #ifdef DEBUG
        printf("%s ****\n", curTC.tape);
        printState(finalState);
        printf("\n****\n");
    #endif // DEBUG

    }

    return 0;
}
