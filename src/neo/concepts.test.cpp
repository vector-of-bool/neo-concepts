// This just tests that we can compile with the concepts.hpp header

#include "./concepts.hpp"

#include <memory>
#include <string>
#include <vector>

using namespace neo;

struct empty {};
struct irregular {
    irregular(int) {}
};

struct nontrivial {
    ~nontrivial() {}
};

struct immobile {
    immobile(const immobile&) = delete;
};

struct callable_thing {
    void operator()(int, int);
    void operator()(std::string);
    void operator()(std::string, std::string, int);
};

struct int_tester {
    bool operator()(int);
};

struct int_comparer {
    bool operator()(int, int);
};

struct empty_derived : empty {
};

#define EXPAND(V) V
#define PASTE(a, b) a##b

#define TEST_CONCEPT_1(N, ConceptName, ...)                                                        \
    int  PASTE(foo_, N)() requires ConceptName<__VA_ARGS__> { return N; }                          \
    auto PASTE(value_, N) = PASTE(foo_, N)()

#define TEST_CONCEPT(ConceptName, ...) TEST_CONCEPT_1(__COUNTER__, ConceptName, __VA_ARGS__)

TEST_CONCEPT(same_as, int, int);
TEST_CONCEPT(same_as, bool, bool);
TEST_CONCEPT(same_as, empty, empty);
TEST_CONCEPT(!same_as, empty, bool);
TEST_CONCEPT(same_as, void, void);

TEST_CONCEPT(convertible_to, int, float);
TEST_CONCEPT(convertible_to, void, void);
TEST_CONCEPT(convertible_to, const char*, std::string);
TEST_CONCEPT(!convertible_to, bool, std::string);

TEST_CONCEPT(derived_from, empty_derived, empty);
TEST_CONCEPT(!derived_from, empty_derived, bool);

TEST_CONCEPT(integral, int);
TEST_CONCEPT(!integral, float);
TEST_CONCEPT(integral, char);

TEST_CONCEPT(signed_integral, int);
TEST_CONCEPT(signed_integral, signed int);
TEST_CONCEPT(signed_integral, signed);
TEST_CONCEPT(signed_integral, signed char);
TEST_CONCEPT(!signed_integral, unsigned);
TEST_CONCEPT(!signed_integral, unsigned char);

TEST_CONCEPT(destructible, int);
TEST_CONCEPT(destructible, empty);
TEST_CONCEPT(destructible, std::string);

TEST_CONCEPT(constructible_from, float, int);
TEST_CONCEPT(constructible_from, std::string, char, std::size_t);
TEST_CONCEPT(!constructible_from, std::string, empty);
TEST_CONCEPT(constructible_from, std::string);

TEST_CONCEPT(default_constructible, std::string);
TEST_CONCEPT(default_constructible, int);
TEST_CONCEPT(!default_constructible, irregular);

TEST_CONCEPT(move_constructible, int);
TEST_CONCEPT(move_constructible, std::string);
TEST_CONCEPT(move_constructible, std::unique_ptr<int>);
TEST_CONCEPT(!move_constructible, immobile);

TEST_CONCEPT(copy_constructible, int);
TEST_CONCEPT(copy_constructible, std::string);
TEST_CONCEPT(!copy_constructible, std::unique_ptr<int>);
TEST_CONCEPT(!copy_constructible, immobile);

TEST_CONCEPT(assignable_from, int&, int);
TEST_CONCEPT(!assignable_from, int&, std::string);
TEST_CONCEPT(assignable_from, std::string&, const char*);

TEST_CONCEPT(movable, int);
TEST_CONCEPT(movable, std::string);
TEST_CONCEPT(!movable, immobile);
TEST_CONCEPT(movable, std::unique_ptr<int>);

TEST_CONCEPT(copyable, std::string);
TEST_CONCEPT(copyable, int);
TEST_CONCEPT(!copyable, immobile);
TEST_CONCEPT(!copyable, std::unique_ptr<int>);

TEST_CONCEPT(trivially_copyable, int);
TEST_CONCEPT(!trivially_copyable, std::string);
TEST_CONCEPT(trivially_copyable, empty);

TEST_CONCEPT(trivial_type, int);
TEST_CONCEPT(trivial_type, empty);
TEST_CONCEPT(!trivial_type, nontrivial);

TEST_CONCEPT(simple_boolean, int);
TEST_CONCEPT(simple_boolean, char);
TEST_CONCEPT(simple_boolean, bool);
TEST_CONCEPT(!simple_boolean, std::string);

TEST_CONCEPT(equality_comparable, std::string);
TEST_CONCEPT(equality_comparable, int);
TEST_CONCEPT(equality_comparable, bool);
TEST_CONCEPT(!equality_comparable, empty);

TEST_CONCEPT(totally_ordered, int);
TEST_CONCEPT(totally_ordered, std::string);
TEST_CONCEPT(!totally_ordered, empty);

TEST_CONCEPT(totally_ordered_with, int, float);
TEST_CONCEPT(totally_ordered_with, float, int);
TEST_CONCEPT(totally_ordered_with, bool, int);
TEST_CONCEPT(!totally_ordered_with, bool, std::string);

TEST_CONCEPT(semiregular, int);
TEST_CONCEPT(semiregular, std::string);
TEST_CONCEPT(semiregular, std::vector<int>);
TEST_CONCEPT(!semiregular, irregular);
TEST_CONCEPT(!semiregular, std::unique_ptr<int>);
TEST_CONCEPT(semiregular, std::shared_ptr<int>);

TEST_CONCEPT(invocable, callable_thing, int, int);
TEST_CONCEPT(invocable, callable_thing, std::string);
TEST_CONCEPT(!invocable, callable_thing, int);
TEST_CONCEPT(!invocable, callable_thing);
TEST_CONCEPT(invocable, callable_thing, std::string, std::string, int);
TEST_CONCEPT(!invocable, std::string, int);

TEST_CONCEPT(predicate, int_tester, int);
TEST_CONCEPT(!predicate, int_tester, std::string);
TEST_CONCEPT(!predicate, std::string, int);

TEST_CONCEPT(relation, int_comparer, int, int);
TEST_CONCEPT(relation, int_comparer, int, float);
TEST_CONCEPT(!relation, int_comparer, int, std::string);

int main() {}