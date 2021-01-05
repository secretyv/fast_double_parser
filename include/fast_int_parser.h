#ifndef FAST_INT_PARSER_H
#define FAST_INT_PARSER_H

#include <cstring>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#ifdef _MSC_VER
#  include <intrin.h>
#endif

namespace fast_int_parser {

namespace private_part {
#ifdef _MSC_VER
#  ifndef really_inline
#     define really_inline __forceinline
#  endif // really_inline
#else  // _MSC_VER
#  ifndef really_inline
#     define really_inline __attribute__((always_inline)) inline
#  endif // really_inline
#endif // _MSC_VER

// Heavily inspired from
// https://bitbucket.org/andreyv/cppiotest/src/default/iotest.cpp
// https://www.geeksforgeeks.org/fast-io-for-competitive-programming/

/*
Work function to read signed numbers.
Read from first to atmost last, extracting one signed number to result.
Returns the updated iterator.
throw(std::domain_error,         // on Empty record
      std::invalid_argument)     // on Invalid record
*/
template <typename InputIt, typename OutputType>
really_inline InputIt parse_number_signed(InputIt first, InputIt last, OutputType& result)
{
   // Skip leading white spaces, if any.
   while (first != last && *first <= ' ') ++first;
   if (first == last) throw std::domain_error("Empty record");

   // Get sign
   bool sign = *first == '-';
   if (sign)
   { 
      ++first;
   }
   if (first == last || *first < '0' || *first > '9') 
      throw std::invalid_argument("Invalid record");

   // Read
   OutputType res = *first++ - '0';
   while (first != last && *first >= '0' && *first <= '9')
   {
      res = res * 10 + (*first++ - '0');
   }

   result = sign ? -res : res;
   return first;
}

/*
Work function to read unsigned numbers.
Read from first to atmost last, extracting one unsigned number to outInt.
Returns the updated iterator.
throw(std::domain_error,         // on Empty record
      std::invalid_argument)     // on Invalid record
*/
template <typename InputIt, typename OutputType>
really_inline InputIt parse_number_unsigned(InputIt first, InputIt last, OutputType& outInt)
{
   // Skip white spaces
   while (first != last && *first <= ' ') ++first;
   if (first == last)                throw std::domain_error("Empty record");
   if (*first < '0' or *first > '9') throw std::invalid_argument("Invalid record");

   // Read
   OutputType res = *first++ - '0';
   while (first != last && *first >= '0' && *first <= '9')
   {
      res = res * 10 + (*first++ - '0');
   }

   outInt = res;
   return first;
}

/*
Work function to parse many values.
Read from first to last, extracting numbers to result.
Returns the updated iterator.
throw(std::invalid_argument)     // on Invalid record
*/
template <typename InputIt, typename OutputIt, typename OutputType>
InputIt parse_many(InputIt first, InputIt last, OutputIt result, size_t maxItems)
{
   try
   {
      if constexpr (std::is_unsigned_v<OutputType>)
      {
         OutputType val;
         size_t n = 0;
         while (first != last && n != maxItems)
         {
            first = private_part::parse_number_unsigned(first, last, val);
            *result = val;
            ++result;
            ++n;
         }
      }
      else
      {
         OutputType val;
         size_t n = 0;
         while (first != last && n != maxItems)
         {
            first = private_part::parse_number_signed(first, last, val);
            *result = val;
            ++result;
            ++n;
         }
      }
   }
   catch (const std::domain_error&)
   {
   }
   return first;
}

/*
Work function to parse one value.
Read from first to atmost last, extracting one number to result.
Returns the updated iterator or NULL on error.
*/
template <typename InputIt, typename OutputIt, typename OutputType>
InputIt parse_one(InputIt first, InputIt last, OutputIt result)
{
   try
   {
      OutputType val;
      const char* last = first + strlen(first);
      if constexpr (std::is_unsigned_v<OutputType>)
         first = private_part::parse_number_unsigned(first, last, val);
      else
         first = private_part::parse_number_signed(first, last, val);
      *result = val;
   }
   catch (const std::domain_error&)
   {
      return NULL;
   }
   catch (const std::invalid_argument&)
   {
      return NULL;
   }
   return first;
}

}  // namespace private_part


//=============================================================================
//=============================================================================
// Parser one number

// Parse one number at first in result.
// On success return the new position, on error return NULL.
template <typename ResType>
char const* parse_number(char const* first, ResType* result)
{
   typedef char const* InputIt;
   typedef ResType* OutputIt;
   typedef ResType  OutputType;
   InputIt last = first + strlen(first);
   return private_part::parse_one<InputIt, OutputIt, OutputType>(first, last, result);
}

// Parse one number at first in result.
// On success return the new position, on error return NULL.
template <typename Container>
char const* parse_number(char const* first, std::back_insert_iterator<Container> result)
{
   typedef char const* InputIt;
   typedef std::back_insert_iterator<Container> OutputIt;
   typedef typename OutputIt::container_type::value_type OutputType;
   InputIt last = first + strlen(first);
   return private_part::parse_one<InputIt, OutputIt, OutputType>(first, last, result);
}

// Parse one number at first in result.
// On success return the new position, on error return NULL.
template <typename OutputIt>
char const* parse_number(char const* first, OutputIt result)
{
   typedef char const* InputIt;
   typedef typename OutputIt::value_type OutputType;
   InputIt last = first + strlen(first);
   return private_part::parse_one<InputIt, OutputIt, OutputType>(first, last, result);
}

// Parse one number at first in outInt.
// On success return the new position, on error return NULL.
template <typename ResType>
char const* parse_number(char const* first, char const* last, ResType* result)
{
   typedef char const* InputIt;
   typedef ResType* OutputIt;
   typedef ResType  OutputType;
   return private_part::parse_one<InputIt, OutputIt, OutputType>(first, last, result);
}


//=============================================================================
//=============================================================================
// Parser all numbers

// Parse all numbers from first to last in result.
// On success return the new position, on error throw std::invalid_argument.
template <typename InputIt, typename ResType>
InputIt parse_numbers(InputIt first, 
                      InputIt last, 
                      ResType* result, 
                      size_t maxItems = std::numeric_limits<size_t>::max())
{
   typedef ResType* OutputIt;
   typedef ResType  OutputType;
   return private_part::parse_many<InputIt, OutputIt, OutputType>(first, last, result, maxItems);
}

// Parse all numbers from first to last in result.
// On success return the new position, on error throw std::invalid_argument.
template <typename InputIt, typename OutputIt>
InputIt parse_numbers(InputIt first,
                      InputIt last,
                      OutputIt result,
                      size_t maxItems = std::numeric_limits<size_t>::max())
{
   typedef typename OutputIt::value_type OutputType;
   return private_part::parse_many<InputIt, OutputIt, OutputType>(first, last, result, maxItems);
}

// Parse all numbers from first to last in result.
// On success return the new position, on error throw std::invalid_argument.
template <typename InputIt, typename Container>
InputIt parse_numbers(InputIt first,
                      InputIt last,
                      std::back_insert_iterator<Container> result,
                      size_t maxItems = std::numeric_limits<size_t>::max())
{
   typedef std::back_insert_iterator<Container> OutputIt;
   typedef typename OutputIt::container_type::value_type OutputType;
   return private_part::parse_many<InputIt, OutputIt, OutputType>(first, last, result, maxItems);
}

} // namespace fast_int_parser

#endif   //  FAST_INT_PARSER_H
