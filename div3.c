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

inline int cAtoi(const char c) {
    return c == '1' ? 1 : 0;
}

State divisibleByThree(const char *tape) {
    // Idea here is from examination of normal
    // binary division by means of XOR-ing from the 
    // MSB with 0x3 or 0b11

    // Inclusion of Epsilon the empty string -> AcceptingState
    if (tape == NULL)
        return Accepted;

    // Trim out all the leading 0s
    size_t i = 0;
    for (i=0; ;i++) {
        if (tape[i] == '\0')
            return Accepted;

        if (cAtoi(tape[i]) != 0)
            break;
    }

    const char *offset = tape + i;
    size_t len = strlen(offset);

    char *tape_copy = malloc(len);
    assert(tape_copy != NULL);

    tape_copy = strncpy(tape_copy, offset, len);
    size_t v, end;

    // Iterate until only one character is left
    // the value of the last character determines the divisibility
    for (i = 0, end = len-1; i < end; i++) {
        v = cAtoi(*tape_copy) << 1;
        if (v == 0)
            break;

        tape_copy++;
        v |= cAtoi(*tape_copy);

        // Write it back to the head
        *tape_copy = '0' + ((v ^ 0x3) & 0x1);
    }

    // No remainder expected if truly divisible by 3
    return cAtoi(*tape_copy) == 1 ? Rejected: Accepted;
}

void printState(State st) {
    printf("%s", st == Accepted ? "accepted" : (st == Rejected ? "rejected": "unknown"));
}

int main() {
    struct tc{
        char *tape;
        State finalState;
    }testCases[] = {
            {.tape="11000", .finalState=Accepted},
            {.tape="110", .finalState=Accepted},
            {.tape="0", .finalState=Accepted},
            {.tape="1", .finalState=Rejected},
            {.tape="10", .finalState=Rejected},
            {.tape="1001", .finalState=Accepted},
            {.tape="10010", .finalState=Accepted},
            {.tape="00000000000", .finalState=Accepted},
            {.tape="00000000010", .finalState=Rejected},
            {.tape="00000000011", .finalState=Accepted},
            {.tape="11000011010011111", .finalState=Accepted},
            {.tape="1100001101000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000011111", .finalState=Accepted},
            {.tape="000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", .finalState=Accepted},
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
