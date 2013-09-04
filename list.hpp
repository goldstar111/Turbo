/* 
 * File:   list.hpp
 * Author: manu343726
 *
 * Created on 1 de septiembre de 2013, 13:17
 */

#ifndef LIST_HPP
#define	LIST_HPP

#undef max
#undef min

#include "basic_types.hpp"

#include <sstream>

namespace mpl
{
    
    /* mpl::list */
    
    template<typename... Ts>
    struct list
    {
        static const std::size_t lenght = sizeof...(Ts);
        static const std::size_t size = lenght;
        static const bool is_empty = lenght == 0;
    };   
        
    using empty_list = mpl::list<>;
    
    namespace 
    {
        template<typename LIST>
        struct _to_string;
        
        //length > 2 (Recursive case)
        template<typename HEAD , typename... TAIL>
        struct _to_string<mpl::list<HEAD,TAIL...>>
        {
            friend std::ostream& operator<<(std::ostream& os , _to_string<mpl::list<HEAD,TAIL...>>)
            {
                return os << mpl::to_string<HEAD>() << ',' << _to_string<mpl::list<TAIL...>>();
            }       
        };
        
        //length == 1 (Base case)
        template<typename HEAD>
        struct _to_string<mpl::list<HEAD>>
        {
            friend std::ostream& operator<<(std::ostream& os , _to_string<mpl::list<HEAD>>)
            {
                return os << mpl::to_string<HEAD>();
            }       
        };
        
        //length == 0 (Base case)
        template<>
        struct _to_string<mpl::empty_list>
        {
            friend std::ostream& operator<<(std::ostream& os , _to_string<mpl::empty_list>)
            {
                return os;
            }       
        };
    }
    
    template<typename... Ts>
    struct to_string_t<mpl::list<Ts...>>
    {
        operator std::string() const
        {
            std::stringstream os;
            
            os << '[' << _to_string<mpl::list<Ts...>>() << ']';
            
            return os.str();
        }
    };
    
    namespace 
    {
        /* mpl::index_of */
    
        //Forward-declaration:
        template<typename LIST , typename T>
        struct _index_of;

        //Recursive case:
        template<typename HEAD , typename... TAIL , typename T>
        struct _index_of<mpl::list<HEAD,TAIL...>,T>
        {
            static const int tmp = _index_of<mpl::list<TAIL...>,T>::value;
            static const int value = tmp < 0 ? -1 : tmp + 1;
        };

        //Base case:
        template<typename... TAIL , typename T>
        struct _index_of<mpl::list<T,TAIL...>,T> : public mpl::integer<0> {};

        //Base case:
        template<typename T>
        struct _index_of<mpl::list<>,T> : public mpl::integer<-1> {};
        
        
        /* mpl::contains */
        
        template<typename LIST , typename T>
        struct _contains : public mpl::boolean<_index_of<LIST,T>::value >= 0> {};


        /* mpl::type_at */

        //Forward declaration:
        template<typename LIST , std::size_t INDEX>
        struct _type_at;

        //Recursive case:
        template<typename HEAD , typename... TAIL , std::size_t INDEX>
        struct _type_at<mpl::list<HEAD,TAIL...> , INDEX> : public mpl::type_t< typename _type_at<mpl::list<TAIL...>,INDEX - 1>::type> {};
        
        //Base case:
        template<typename HEAD , typename... TAIL>
        struct _type_at<mpl::list<HEAD,TAIL...>,0> : public mpl::type_t<HEAD> {};
        
        //Base case:
        template<std::size_t INDEX>
        struct _type_at<mpl::empty_list,INDEX>
        {
            static_assert( mpl::empty_list::lenght == 0 , "Index out of bounds");
        };
        
        
        /* mpl::split */
        
        template<typename LEFT_TYPES , typename RIGHT_TYPES , std::size_t INDEX>
        struct _splitter;
        
        //Recursive case:
        template<typename... LEFT_TYPES , typename HEAD , typename... TAIL , std::size_t INDEX>
        struct _splitter<mpl::list<LEFT_TYPES...>,mpl::list<HEAD,TAIL...>,INDEX>
        {
            using next = _splitter<mpl::list<LEFT_TYPES...,HEAD> , mpl::list<TAIL...> , INDEX - 1>;
            using right = typename next::right;
            using left  = typename next::left;
        };
        
        //Base case:
        template<typename... LEFT_TYPES , typename HEAD , typename... TAIL>
        struct _splitter<mpl::list<LEFT_TYPES...> , mpl::list<HEAD,TAIL...> , 0>
        {
            using right = mpl::list<HEAD,TAIL...>;
            using left  = mpl::list<LEFT_TYPES...>;
        };
        
        //Base case:
        template<typename... LEFT_TYPES , std::size_t INDEX>
        struct _splitter<mpl::list<LEFT_TYPES...> , mpl::empty_list , INDEX>
        {
            static_assert( mpl::empty_list::lenght == 0 , "Index out of bounds" );
        };
        
        template<typename LIST , typename INDEX>
        struct _split : public _splitter<LIST,mpl::empty_list,INDEX::value> {};
    }
    
    
    template<typename LIST , typename T>
    using index_of = mpl::integer<_index_of<LIST,T>::value>;
    
    template<typename LIST , typename T>
    using contains = _contains<LIST,T>;
    
    template<typename LIST , typename INDEX>
    using type_at = typename _type_at<LIST,INDEX::value>::type;
    
    template<typename LIST , typename INDEX>
    using split = _split<LIST,INDEX>;
    
    template<typename LIST , typename INDEX>
    using split_left = typename _split<LIST,INDEX>::left;
    
    template<typename LIST , typename INDEX>
    using split_right = typename _split<LIST,INDEX>::right;
}

#endif	/* LIST_HPP */

