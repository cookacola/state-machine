#include <stdio.h>

#define ENTRY_STATE entry
#define END_STATE end

typedef enum { entry, mantissa, pre_mantissa, end } STATE;
typedef enum { dig, fail, neg, dot } RC;

int entry_state(char c) {
    if ((c >= '0' && c <= '9') || (c == '-')) return dig;
    return fail;
}

int mantissa_state(char c) {
    if (c >= '0' && c <= '9') return dig;
    return fail;
}

int pre_mantissa_state(char c) {
    if (c >= '0' && c <= '9') return dig;
    if (c == '.') return dot;
    return fail;
}

int after_neg_state(char c) {
    if (c >= '0' && c <= '9') return dig;
    return fail;
}

int (*fun_array[])(char) = { entry_state, mantissa_state, pre_mantissa_state};

typedef struct {
    STATE in;
    RC rc;
    STATE out;
} TRANSITION;

TRANSITION transitions[] = {
    { entry, neg, pre_mantissa},
    { entry, dig, pre_mantissa},
    { pre_mantissa, dig, pre_mantissa},
    { pre_mantissa, dot, mantissa},
    { mantissa, dig, mantissa},
};

STATE transition_lookup(STATE state, RC rc) {
    for (int i = 0; i < sizeof(transitions) / sizeof(transitions[0]); i++) {
        if (transitions[i].in == state && transitions[i].rc == rc) {
            return transitions[i].out;
        }
    }
    return end;
}

void run(char* cargo) {
  STATE current_state = ENTRY_STATE;
    RC rc;
    int (*fun_pt)(char);
    char* c = cargo;

    while (*c != '\0') {
        fun_pt = fun_array[current_state];
        rc = fun_pt(*c);
        
        if (rc == fail) {
            printf("Tried: %s, Unsuccessful at %c\n", cargo, *c);
            return;
        }

        current_state = transition_lookup(current_state, rc);
        c++;
    }

    if (rc == dig) {
      printf("Tried: %s, Success\n", cargo);
    } else {
      printf("Tried: %s, Unsuccessful at the end\n", cargo);
    }
}

int main() {
  run("12345");
  run("123.");
  run("abc");
  run("1a23");
  run("123.2");
  return 0;
}
