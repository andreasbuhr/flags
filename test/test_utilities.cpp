// Copyright 2024 Tobias Loew.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#define TEST_NAMESPACE test_utilities
#include "include_test.hpp"

#include <boost/core/lightweight_test_trait.hpp>
#include <boost/flags.hpp>

#include "include_test_post.hpp"

#if defined(TEST_FLAGS_LINKING)
namespace TEST_NAMESPACE {
#endif // defined(TEST_FLAGS_LINKING)

enum class flags_enum : unsigned int {
    bit_0 = boost::flags::nth_bit(0), // == 0x01
    bit_1 = boost::flags::nth_bit(1), // == 0x02
    bit_2 = boost::flags::nth_bit(2), // == 0x04
    bit_3 = boost::flags::nth_bit(3), // == 0x08
};

// enable flags_enum
constexpr inline bool boost_flags_enable(flags_enum) { return true; }


// helpers
template<typename E>
constexpr auto to_underlying(E value)
#if defined(TEST_NO_CXX14_DECLTYPE_AUTO)
-> typename std::underlying_type<E>::type
#endif // defined(TEST_NO_CXX14_DECLTYPE_AUTO)
{
    return static_cast<typename std::underlying_type<E>::type>(value);
}



void test_any() {
    using namespace boost::flags;

    flags_enum a = flags_enum::bit_0;
    flags_enum b = flags_enum::bit_1;

    BOOST_TEST(any(a));
    BOOST_TEST(any(a | b));
    BOOST_TEST(!any(a & b));

#if defined(TEST_COMPILE_FAIL_COMPLEMENT_ANY)
    auto v = any(~a);
#endif // defined(TEST_COMPILE_FAIL_COMPLEMENT_ANY)
}


void test_none() {
    using namespace boost::flags;

    flags_enum a = flags_enum::bit_0;
    flags_enum b = flags_enum::bit_1;

    BOOST_TEST(!none(a));
    BOOST_TEST(!none(a | b));
    BOOST_TEST(none(a & b));

#if defined(TEST_COMPILE_FAIL_COMPLEMENT_NONE)
    auto v = none(~a);
#endif // defined(TEST_COMPILE_FAIL_COMPLEMENT_NONE)
}

void test_subset() {
    using namespace boost::flags;

    flags_enum a = flags_enum::bit_0;
    flags_enum b = flags_enum::bit_1;

    BOOST_TEST(subset(flags_enum{}, a));
    BOOST_TEST(!subset(a, flags_enum{}));
    BOOST_TEST(subset(a, a | b));
    BOOST_TEST(!subset(a | b, a | b));
    BOOST_TEST(!subset(a | b, a));
    BOOST_TEST(!subset(b, a));
}

void test_subseteq() {
    using namespace boost::flags;

    flags_enum a = flags_enum::bit_0;
    flags_enum b = flags_enum::bit_1;

    BOOST_TEST(subseteq(flags_enum{}, a));
    BOOST_TEST(!subseteq(a, flags_enum{}));
    BOOST_TEST(subseteq(a, a | b));
    BOOST_TEST(subseteq(a | b, a | b));
    BOOST_TEST(!subseteq(a | b, a));
    BOOST_TEST(!subseteq(b, a));
}

void test_intersect() {
    using namespace boost::flags;

    flags_enum a = flags_enum::bit_0;
    flags_enum b = flags_enum::bit_1;

    BOOST_TEST(!intersect(a, flags_enum{}));
    BOOST_TEST(!intersect(flags_enum{}, a));
    BOOST_TEST(intersect(a, a | b));
    BOOST_TEST(intersect(a | b, a | b));
    BOOST_TEST(intersect(a | b, a));
    BOOST_TEST(!intersect(b, a));
}

void test_disjoint() {
    using namespace boost::flags;

    flags_enum a = flags_enum::bit_0;
    flags_enum b = flags_enum::bit_1;

    BOOST_TEST(disjoint(a, flags_enum{}));
    BOOST_TEST(disjoint(flags_enum{}, a));
    BOOST_TEST(!disjoint(a, a | b));
    BOOST_TEST(!disjoint(a | b, a | b));
    BOOST_TEST(!disjoint(a | b, a));
    BOOST_TEST(disjoint(b, a));
}

void test_make_null() {
    using namespace boost::flags;

    flags_enum a = flags_enum::bit_0;

    BOOST_TEST(make_null(flags_enum{}) == flags_enum{});
    BOOST_TEST(make_null(a) == flags_enum{});
}


void test_make_if() {
    using namespace boost::flags;

    flags_enum a = flags_enum::bit_0;

    BOOST_TEST(make_if(a, false) == flags_enum{});
    BOOST_TEST(make_if(a, true) == a);
}


void test_modify() {
    using namespace boost::flags;

    flags_enum a = flags_enum::bit_0;
    flags_enum b = flags_enum::bit_1;

    BOOST_TEST(modify(a, b, false) == a);
    BOOST_TEST(modify(a, b, true) == (a | b));
}


void test_modify_inplace() {
    using namespace boost::flags;

    flags_enum a = flags_enum::bit_0;
    flags_enum b = flags_enum::bit_1;

    {
        auto v = a;
        modify_inplace(v, b, false);
        BOOST_TEST(v == a);
    }

    {
        auto v = a;
        modify_inplace(v, b, true);
        BOOST_TEST(v == (a | b));
    }
}


int main() {
    report_config();
    test_any();
    test_none();
    test_subset();
    test_subseteq();
    test_intersect();
    test_disjoint();
    test_make_null();
    test_make_if();
    test_modify();
    test_modify_inplace();
    std::cout << "__MINGW32__ not defined\n";

    return boost::report_errors();
}

#if defined(TEST_FLAGS_LINKING)
} // namespace TEST_NAMESPACE
#endif // defined(TEST_FLAGS_LINKING)
