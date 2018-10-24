//
// Copyright (C) 2018 Dominik S. Buse <buse@ccs-labs.org>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#ifndef VEINS_UTILS_ENUMBITSET_H_
#define VEINS_UTILS_ENUMBITSET_H_

#include <bitset>
#include <type_traits>

namespace Veins {

/*
 * Helper struct to define the maximum element of a given Enum.
 *
 * Provide ``max'' member for every enum type that should be used as an
 * underlying type for a EnumBitset.
 */
template<typename T>
struct EnumTraits;

/*
 * Bitset template for scoped (class) enums.
 *
 * Wrapper around the original std::bitset to use a user-defined scoped
 * enum as the type for the bitset.
 * Allows implementation of typesafe sets.
 */
template<typename T>
class EnumBitset {
private:
    using enum_underlying_type = typename std::underlying_type<T>::type;
    std::bitset<static_cast<enum_underlying_type>(EnumTraits<T>::max) + 1> bits;

    typename std::underlying_type<T>::type get_value(T v) const
    {
        return static_cast<typename std::underlying_type<T>::type>(v);
    }

public:
    EnumBitset()
        : bits() {}
    EnumBitset(unsigned long long val)
        : bits(val) {}

    bool operator==(const EnumBitset<T>& rhs) const
    {
        return bits == rhs.bits;
    }

    bool operator!=(const EnumBitset<T>& rhs) const
    {
        return bits != rhs.bits;
    }

    constexpr bool operator[](std::size_t pos) const
    {
        return bits[pos];
    }

    bool test(T pos) const
    {
        return bits.test(get_value(pos));
    }

    EnumBitset& set(T pos, bool value = true)
    {
        bits.set(get_value(pos), value);
        return *this;
    }

    EnumBitset& flip(T pos)
    {
        bits.flip(get_value(pos));
        return *this;
    }

    std::string to_string() const
    {
        return bits.to_string();
    }

    unsigned long to_ulong() const
    {
        return bits.to_ulong();
    }

    unsigned long long to_ullong() const
    {
        return bits.to_ullong();
    }
};

} // namespace Veins

#endif
