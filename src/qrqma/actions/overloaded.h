#pragma once


namespace qrqma {
namespace actions {
namespace detail {

template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template <class... Ts> overloaded(Ts...)->overloaded<Ts...>;

}
}
}
