#include <stdint.h>
#include <stdlib.h>
#include <utility>
#include <type_traits>

// ====================
// Some custom type traits for member pointers
template<typename T>
struct member_pointer_types {
    using member_type = T;
    using class_type = T;
};
template<typename T, typename U>
struct member_pointer_types<T U::*> {
    using member_type = T;
    using class_type = U;
};

template<typename T>
struct member_type {
using type = member_pointer_types<T>::member_type;
};
template<typename T>
using member_type_t = member_type<T>::type;

template<typename T>
struct member_class {
using type = member_pointer_types<T>::class_type;
};
template<typename T>
using member_class_t = member_class<T>::type;
// ====================

// This member is private, right?
struct Test {
private:
    int32_t data = 5;
};
// Not if *magical jank* has anything to say about it!


// This exists just to create arbitrary overloads
template<size_t lookup>
struct public_reader_tag {};

// MAGIC
template<size_t lookup, auto member_ptr> requires(std::is_member_object_pointer_v<decltype(member_ptr)>)
struct public_reader {
    // For some reason, defining functions in a friend
    // statement puts the definition in global scope
    // but with a declaration scoped to the enclosing template.
    // This means that an external declaration can call these
    // definitions despite not knowing *anything* about the
    // template parameters.

    // Smuggle type information about the member pointer out of this template struct...
    friend consteval auto unprivate_types(public_reader_tag<lookup>) {
        return std::pair<member_type_t<decltype(member_ptr)>,member_class_t<decltype(member_ptr)>>{};
    }

    // Using arguments/return types that don't depend directly on 
    // the member_ptr parameter, apply that pointer to the argument.
    //
    // WARNING: For some reason this explodes with multiple definition errors
    // if public_reader is instantiated multiple times (even with different member pointers).
    // If anyone can think of a way to implement this without being a template,
    // then it should be possible to scale up.
    template<size_t lookup_val>
    friend inline constexpr decltype(unprivate_types(public_reader_tag<lookup_val>{}))::first_type&
    unprivate(typename decltype(unprivate_types(public_reader_tag<lookup_val>{}))::second_type& val) {
        return val.*member_ptr;
    }
};

// Macro wrapper around all the weird declarations
#define declare_public_readers(type, member) \
/* Magic number constant to retrieve the correct member pointer*/ \
static inline constexpr size_t type ## _ ## member ## _lookup = __COUNTER__; \
\
/* Declarations of explicit instantiations ignore \
access specifiers for some reason, so a pointer to a private \
member can be passed in. */ \
template struct public_reader<type##_##member##_lookup, &type::member>; \
\
/* Declare the type retrieval function */ \
consteval auto unprivate_types(public_reader_tag<type##_##member##_lookup>); \
\
/* Declare the function for reading from the private member pointer,
ideally using as few template parameters as possible. */ \
template<size_t lookup_val> \
inline constexpr decltype(unprivate_types(public_reader_tag<lookup_val>{}))::first_type& \
unprivate(typename decltype(unprivate_types(public_reader_tag<lookup_val>{}))::second_type& val)

// Set up the unprivate function for Test.data
declare_public_readers(Test, data);

// Test this crap
constexpr auto unprivate_test() {
    Test test_val;
    return unprivate<Test_data_lookup>(test_val);
}

// Hey, it works
static_assert(unprivate_test() == 5);

int main() {
    return unprivate_test();
}