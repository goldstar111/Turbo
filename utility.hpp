/* 
 * File:   utility.hpp
 * Author: manu343726
 *
 * Created on 30 de mayo de 2014, 16:41
 */

#ifndef UTILITY_HPP
#define	UTILITY_HPP

#include "basic_types.hpp"
#include "function.hpp"

#include <cstdint>

/*
 * The namespace tml::util contains a set sparse metaprogramming utilities.
 */

namespace tml
{
    namespace util
    {
        namespace func
        {
            template<typename... Ts>
            struct pack_length : public tml::function<tml::size_t<sizeof...(Ts)>>
            {};
            
            template<typename T>
            struct sizeof_bits : public tml::function<tml::size_t<sizeof(T) * CHAR_BITS>>
            {};
        }
        
        /*
         * Computes the length of a variadic pack
         */
        template<typename... ARGS>
        using pack_length = typename tml::util::func::pack_length<ARGS...>::result;
        
        /*
         * Computes the size in bits of a certain type T
         */
        template<typename T>
        using sizeof_bits = typename tml::util::func::sizeof_bits<T>::result;
    }
}

#endif	/* UTILITY_HPP */

