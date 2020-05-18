#pragma once

#ifndef NEO_PASTE
#define NEO_PASTE_1(a, b) a##b
#define NEO_PASTE(a, b) NEO_PASTE_1(a, b)
#endif

#define NEO_TEST_CONCEPT_1(Ident, ...)                                                             \
    template <typename Void = void>                                                                \
    constexpr int NEO_PASTE(_neo_test_concept_fn_, Ident)() requires(__VA_ARGS__) {                \
        return 0;                                                                                  \
    }                                                                                              \
    static_assert(NEO_PASTE(_neo_test_concept_fn_, Ident)() == 0)

#define NEO_TEST_CONCEPT(...)                                                                      \
    NEO_TEST_CONCEPT_1(NEO_PASTE(__LINE__, NEO_PASTE(_, __COUNTER__)), __VA_ARGS__)
