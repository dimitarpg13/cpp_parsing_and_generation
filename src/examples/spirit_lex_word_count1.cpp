#include <iostream>
#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/bind/bind.hpp>
#include <boost/ref.hpp>
#include "example.hpp"

namespace lex = boost::spirit::lex;


///////////////////////////////////////////////////////////////////////////////
////  Token id definitions
///////////////////////////////////////////////////////////////////////////////
//[wcf_token_ids
enum token_ids
{
   ID_WORD = 1000,
   ID_EOL,
   ID_CHAR
};
//]

//[wcf_token_definition
/* the template `word_count_tokens` defines three different tokens:
   `ID_WORD`, `ID_EOL`, and `ID_CHAR`, representing a word (anything
   except a whitespace or a newline), a newline character, and any
   other character (`ID_WORD`, `ID_EOL`, and `ID_CHAR` are enum values
   representing the token ids, but could be anything else convertible
   to an integer as well).
   The direct base class of any token definition class needs to be the
   template `lex::lexer<>`, where the corresponding template parameter 
   ( here: `lex:lexertl::lexer<BaseIterator>`) defines which 
   underlying lexer engine has to be used to provide the required state
   machine functionality. In this example we use the Lexertl based 
  lexer engine as the underlying lexer type. 
 */ 
template <typename Lexer>
struct word_count_tokens : lex::lexer<Lexer>
{
    word_count_tokens()
    {
        // define tokens (the regular expression to match and the corresponding
        // token id) and add them to the lexer
        this->self.add
            ("[^ \t\n]+", ID_WORD) // words (anything except ' ', '\t' or '\n')
            ("\n", ID_EOL)         // newline characters
            (".", ID_CHAR)         // anything else is a plain character
        ;
    }

};
//]

//[wcf_functor
/* in this example the struct `counter` is used as a functor counting the 
   characters, words, and lines in the analyzed input sequence by identifying
   the matched tokens as passed from the /Spirit.Lex/ library.
 */
struct counter
{
   //<- this is an implementation detail specific to boost::bind and doesn't show 
   //   up in the documentation
   typedef bool result_type;
   //->

   // the function operator gets called for each of the matched tokens
   // c, l, w are references to the counters used to keep track of 
   // the numbers
   template <typename Token>
   bool operator()(Token const& t, std::size_t& c, std::size_t& w, std::size_t& l) const
   {
      switch (t.id()) {
      case ID_WORD:  // matched a word
      // since we're using a default token type in this example, every
      // token instance contains a `iterator_range<BaseIterator>` as its token
      // attribute pointing to the matched character sequence in the input
        ++w; c += t.value().size();
        break;
      case ID_EOL:   // matched a newline character
        ++l; ++c;
        break;
      case ID_CHAR:  // matched something else
        ++c;
        break;
      }
      return true;   // always continue to tokenize
   }

};
//]

int main(int argc, char* argv[]) 
{
    // these variables are used to count characters, words and lines
    std::size_t c = 0, w = 0, l = 0;

    // read input from the given file
    std::string str (read_from_file(1 == argc ? "word_count.input" : argv[1]));

    // create the token definition instance needed to invoke the lexical analyzer
    word_count_tokens<lex::lexertl::lexer<> > word_count_functor;

    // tokenize the given string, the bound functor gets invoked for each of
    // the matched tokens
    using boost::placeholders::_1;
    char const* first = str.c_str();
    char const* last = &first[str.size()];
    bool r = lex::tokenize(first, last, word_count_functor,
            boost::bind(counter(), _1, boost::ref(c), boost::ref(w), boost::ref(l)));

    // print results
    if (r) {
        std::cout << "lines: " << l << ", words: " << w
            << ", characters: " << c << "\n";
    }
    else {
       std::string rest(first, last);
       std::cout << "Lexical analysis failed\n" << "stopped at: \"" 
                 << rest << "\"\n";
    }
    return 0;
}
