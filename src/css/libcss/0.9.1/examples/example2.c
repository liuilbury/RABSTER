//
// Created by liulizhang on 2020/6/10.
//
#include <inttypes.h>
#include <stdio.h>

#include <parserutils/charset/utf8.h>
#include <parserutils/input/inputstream.h>

#include <libcss/libcss.h>

#include "charset/detect.h"
#include "utils/utils.h"

#include "lex/lex.h"


int main(){
  FILE *fp;
  parserutils_inputstream *stream;
  parserutils_inputstream_create("UTF-8",
								 CSS_CHARSET_DICTATED,css__charset_extract,
								 &stream);
  css_lexer *lexer;
  css__lexer_create(stream, &lexer);
  fp = NULL;
  if(fp==NULL){
    printf("sb");
  }
}