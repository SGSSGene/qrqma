#pragma once
#include "../tao/pegtl.hpp"
#include "expression.h"
#include "spaces.h"

namespace qrqma {
namespace grammar {
namespace pegtl = tao::pegtl;

struct grammar;

struct control_statement_o_brackets : pegtl::string<123, 37> {}; // {%
struct control_statement_c_brackets : pegtl::string<37, 125> {}; // %}

template<typename Token, typename Rule>
struct control_statement_outer : pegtl::if_must<
    pegtl::at<pegtl::seq<control_statement_o_brackets, padded<Token>>>,
    Rule
    > {};
    
template <typename Token, typename... R>
struct control_statement_token
    : pegtl::seq<control_statement_o_brackets,
                 pegtl::until<control_statement_c_brackets,
                              pegtl::if_must<pegtl::seq<padded<Token>, padded<R>...>>>> {};

struct if_token : pegtl::keyword<'i', 'f'> {};
struct if_statement         : control_statement_token<if_token , expression> {};
struct if_content           : pegtl::seq<pegtl::success, grammar> {};

struct else_statement       : control_statement_token<pegtl::keyword<'e', 'l', 's', 'e'>> {};
struct else_content         : pegtl::seq<pegtl::success, grammar> {};

struct if_control_statement : pegtl::if_must<
    if_statement, 
    if_content, 
    pegtl::opt<else_statement, else_content>, 
    control_statement_token<pegtl::keyword<'e', 'n', 'd', 'i', 'f'>>
    > {};

struct set_token : pegtl::keyword<'s', 'e', 't'> {};
struct set_identifier : identifier {};
struct set_expression : expression {};
struct set_control_statement : control_statement_token<set_token , set_identifier, pegtl::one<'='>, set_expression> {};

struct for_token : pegtl::keyword<'f', 'o', 'r'> {};
struct for_identifier : identifier {};
struct for_container_identifier : pegtl::seq<pegtl::success, expression> {};
struct for_content : pegtl::seq<pegtl::success, grammar> {};
struct for_control_statement : pegtl::seq<
    control_statement_token<for_token, pegtl::list<for_identifier, padded<pegtl::one<','>>>, pegtl::keyword<'i', 'n'>, for_container_identifier>,
    for_content,
    control_statement_token<pegtl::keyword<'e', 'n', 'd', 'f', 'o', 'r'>>
    > {};


struct extends_token : pegtl::keyword<'e', 'x', 't', 'e', 'n', 'd', 's'> {};
struct extends_statement : control_statement_token<extends_token , string_literal> {};
struct extends_body : pegtl::seq<pegtl::success, grammar> {};
struct extends_control_statement : pegtl::seq<extends_statement, extends_body> {};

struct block_token : pegtl::keyword<'b', 'l', 'o', 'c', 'k'> {};
struct block_content : pegtl::seq<pegtl::success, grammar> {};
struct block_identifier : identifier {};
struct block_statement : control_statement_token<block_token, block_identifier> {};
struct block_control_statement : pegtl::seq<
    block_statement, 
    block_content,
    control_statement_token<pegtl::keyword<'e', 'n', 'd', 'b', 'l', 'o', 'c', 'k'>, pegtl::star<pegtl::any> > 
    > {};

struct block_control_statement_outer   : control_statement_outer<block_token, block_control_statement> {};
struct if_control_statement_outer      : control_statement_outer<if_token, if_control_statement> {};
struct set_control_statement_outer     : control_statement_outer<set_token, set_control_statement> {};
struct for_control_statement_outer     : control_statement_outer<for_token, for_control_statement> {};
struct extends_control_statement_outer : control_statement_outer<extends_token, extends_control_statement> {};

struct control_statement : pegtl::sor<
                    block_control_statement_outer,
                    if_control_statement_outer, 
                    set_control_statement_outer, 
                    for_control_statement_outer,
                    extends_control_statement_outer
                    > {};

}
}