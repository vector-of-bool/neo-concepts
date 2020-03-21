#pragma once

#ifndef NEO_PASE
#define NEO_PASTE(a, b) a##b
#endif

#define NEO_TEST_CONCEPT_1(N, ConceptName, ...)                                                    \
    template <typename = void>                                                                     \
    int NEO_PASTE(foo_, N)() requires ConceptName<__VA_ARGS__> {                                   \
        return N;                                                                                  \
    }                                                                                              \
    auto NEO_PASTE(value_, N) = NEO_PASTE(foo_, N)()

#define NEO_TEST_CONCEPT(ConceptName, ...) NEO_TEST_CONCEPT_1(__COUNTER__, ConceptName, __VA_ARGS__)
