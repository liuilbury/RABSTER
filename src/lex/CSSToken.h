//
// Created by liulizhang on 2020/6/11.
//

#ifndef CSSPARSER__CSSTOKEN_H_
#define CSSPARSER__CSSTOKEN_H_
#include <string>
typedef enum CSSTokenType {
  CSS_TOKEN_IDENT, CSS_TOKEN_ATKEYWORD, CSS_TOKEN_HASH,
  CSS_TOKEN_FUNCTION, CSS_TOKEN_STRING, CSS_TOKEN_INVALID_STRING,
  CSS_TOKEN_URI, CSS_TOKEN_UNICODE_RANGE, CSS_TOKEN_CHAR,
  CSS_TOKEN_NUMBER, CSS_TOKEN_PERCENTAGE, CSS_TOKEN_DIMENSION,

  /* Those tokens that want strings interned appear above */
  CSS_TOKEN_LAST_INTERN,

  CSS_TOKEN_CDO, CSS_TOKEN_CDC, CSS_TOKEN_S, CSS_TOKEN_COMMENT,
  CSS_TOKEN_INCLUDES, CSS_TOKEN_DASHMATCH, CSS_TOKEN_PREFIXMATCH,
  CSS_TOKEN_SUFFIXMATCH, CSS_TOKEN_SUBSTRINGMATCH, CSS_TOKEN_EOF
} css_token_type;

class CSSToken {
 public:
  CSSTokenType type;
  std::string data;
  std::string idata;
  unsigned int col;
  unsigned int line;
  CSSToken(){
    type=CSS_TOKEN_IDENT;
  }
};

#endif //CSSPARSER__CSSTOKEN_H_
