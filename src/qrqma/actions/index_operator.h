#pragma once

#include "context.h"

namespace qrqma {
namespace grammar {
namespace ops {
struct index;
}
}

namespace actions {

template<typename Rule> 
struct action;

template <> struct action<grammar::ops::index> {
    static void apply(const std::string &in, Context &context);
    template <typename Input>
    static void apply(const Input &in, Context &context) {
        apply(in.string(), context);
    }
};

} // namespace actions
} // namespace qrqma
