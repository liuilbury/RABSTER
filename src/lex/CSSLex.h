//
// Created by liulizhang on 2020/6/10.
//

#ifndef CSSPARSER__CSSLEX_H_
#define CSSPARSER__CSSLEX_H_
#include "CSSToken.h"
#include <vector>
enum CSSDFAStatus
{
	Start,
	iDentStart,
	iDent,
	NMStart,
	NMChar,
	EscapeStartInNMStart,
	EscapeStartInNMChar,
	EscapeStartInHash,
	EscapeStartInATKeyword,
	HashStart,
	Hash,
	Ws,
	AtKeyWordStart,
	AtKeyWord,
	include,
	dot,
	end,
	blockStart,
	blockEnd,
	comma,
	plus,
	greater,
	tidle,
	dashMatch,
	prefixMatch,
	suffixMatch,
	subStringMatch,
	includes,
	star,
	colon,
	semicolon,
	leftSqureBracket,
	rightSqureBracket,
	equal,
	string1Start,
	string1End,
	string2Start,
	string2End,
	annotationStart,
	annotationEnd,
	function,
	numberStart,
	num,
	rightBracket,
	minus,
	LexError
};
struct Pos
{
	int firstPos;
	int forwardPos;
};
class CSSLex
{
 public:
	Pos pos;
	std::string buffer;
	CSSToken* GetToken();
	std::vector<CSSToken> tokenCache;
	void SetBufferSource(const std::string& bufferString);
	void SetBufferString(const std::string& bufferString);
 private:
	static bool isStringChar(uint8_t c);
	static bool isURLChar(uint8_t c);
	static bool isDigit(unsigned char c);
	static bool isWs(unsigned char);
	static bool isHexCharacter(unsigned char c);
	static bool isLetter(unsigned char c);
	static bool isNMStart(unsigned char c);

	std::string getNMChars();
	unsigned char getEscape();
	std::string getString();
	std::string getStringChars();
	std::string getDigits();

	CSSToken* CSSTokenHash();
	CSSToken* CSSTokenAtkeyword();
	CSSToken* CSSTokenString();
	CSSToken* CSSTokenNumberOrPercentageOrDimension();
	CSSToken* CSSTokenIdentOrFunction();
	CSSToken* CSSTokenURIOrUnicodeRangeOrIdentOrFunction();
	CSSToken* CSSTokenURL();
	CSSToken* CSSTokenComment();
	CSSToken* CSSTokenMatch();
	CSSToken* CSSTokenUnicodeRange();
	void emitToken(CSSToken* token);
};
#define IDENT_START_SIGN    '-'
#define UNDER_LINE_SIGN     '_'
#define BACK_SPLASH         '\\'
#define HASH_SIGN           '#'
#define KEYWORD_SIGN        '@'
#define BLOCK_START_SIGN    '{'
#define BLOCK_END_SIGN      '}'
#define EQUAL_SIGN          '='
#define COMMA_SIGN          ','
#define PLUS_SIGN           '+'
#define TIDLE_SIGN          '~'
#define GREATER_SIGN        '>'
#define COLON_SIGN          ':'
#define LEFT_SQURE_BRACKET  '['
#define RIGHT_SQURE_BRACKET ']'
#endif //CSSPARSER__CSSLEX_H_