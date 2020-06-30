//
// Created by liulizhang on 2020/6/10.
//

#include "CSSLex.h"
#include <sstream>
#include <fstream>
#define NextChar(buffer) pos.forwardPos >= buffer.size() ? 0 : buffer[pos.forwardPos++]
#define WS_CASE ' ': case '\r': case '\n': case '\t': case '\f'
#define NUMBER_CASE '0': case '1': case '2': case '3': case '4':\
case '5': case '6': case '7': case '8': case '9': case '.': case '+'
#define CHAR_CASE 'a': case 'b': case 'c': case 'd': case 'e': \
case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': \
case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': \
case 't': /*  'u'*/ case 'v': case 'w': case 'x': case 'y': case 'z': \
case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': \
case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': \
case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': /*  'U'*/ \
case 'V': case 'W': case 'X': case 'Y': case 'Z': case '_'
#define MATCH_CASE '~': case '|': case '^': case '$': case '*'
void CSSLex::SetBufferSource(const std::string& fileName)
{
	std::ifstream in(fileName);
	std::ostringstream buf;
	buf << in.rdbuf();
	buffer = buf.str();
	in.close();
}
void CSSLex::SetBufferString(const std::string& bufferString)
{
	buffer = bufferString;
}
void CSSLex::emitToken(CSSToken* token)
{

}
CSSToken* CSSLex::CSSTokenAtkeyword()//ATKEYWORD = '@' ident  = '@' '-'? nmstart nmchar*
{
	CSSToken* token = new CSSToken();
	unsigned char c;
	if ('-' == (c = NextChar(buffer)))token->data += c;
	else --pos.forwardPos;
	if (!isNMStart((c = NextChar(buffer))))
	{
		token->data += c;
		token->data = '@' + token->data;
		token->type = CSS_TOKEN_CHAR;
		return token;
	}
	if (c == '\\')
	{
		token->data += getEscape();
	}
	token->data += getNMChars();
	token->type = CSS_TOKEN_ATKEYWORD;
	return token;
}
CSSToken* CSSLex::CSSTokenHash()//HASH = '#' name  = '#' nmchar+
{
	CSSToken* token = new CSSToken();
	token->data = getNMChars();
	if (token->data.size() == 0)
	{
		token->type = CSS_TOKEN_CHAR;
	}
	else
	{
		token->type = CSS_TOKEN_HASH;
	}
	return token;
}
CSSToken* CSSLex::CSSTokenString()//string = '"' (stringchar | "'")* '"' | "'" (stringchar | '"')* "'"
{
	CSSToken* token = new CSSToken();
	char c = buffer[pos.forwardPos - 1];
	token->data = c + getString();
	token->type = CSS_TOKEN_STRING;
	return token;
}
CSSToken* CSSLex::CSSTokenNumberOrPercentageOrDimension()
//NUMBER = num = [-+]? ([0-9]+ | [0-9]* '.' [0-9]+)
//PERCENTAGE = num '%'
//DIMENSION = num ident
{
	CSSToken* token = new CSSToken();
	char c = buffer[pos.forwardPos - 1];
	token->data = c + getDigits();
	c = NextChar(buffer);
	if (c == '.')
	{
		token->data += c;
		c = NextChar(buffer);
	}
	if (c == '%')
	{
		token->data += c;
		token->type = CSS_TOKEN_PERCENTAGE;
		return token;
	}
	else if (c == '\\')
	{
		token->data += getEscape();
	}
	else
	{
		--pos.forwardPos;
	}
	std::string str = getNMChars();
	if (str.empty())
	{
		token->type = CSS_TOKEN_NUMBER;
	}
	else
	{
		token->data += str;
		token->type = CSS_TOKEN_DIMENSION;
	}
	return token;
}
CSSToken* CSSLex::CSSTokenComment()//COMMENT = '/' '*' [^*]* '*'+ ([^/] [^*]* '*'+)* '/'
{
	CSSToken* token = new CSSToken();
	unsigned char c = NextChar(buffer);
	if (c != '*')
	{
		token->data = '/';
		token->type = CSS_TOKEN_CHAR;
		return token;
	}
	unsigned last = NextChar(buffer);
	while ((c = NextChar(buffer)))
	{
		if (c == '/' && last == '*')break;
	}
	token->type = CSS_TOKEN_COMMENT;
	return token;
}
CSSToken* CSSLex::CSSTokenIdentOrFunction()
//IDENT = ident = [-]? nmstart nmchar*
//FUNCTION = ident '(' = [-]? nmstart nmchar* '('
{
	CSSToken* token = new CSSToken();
	char c = buffer[pos.forwardPos - 1];
	token->data = c + getNMChars();
	c = NextChar(buffer);
	if (c == '(')
	{
		//token->data+=c;
		token->type = CSS_TOKEN_FUNCTION;
	}
	else
	{
		--pos.forwardPos;
		token->type = CSS_TOKEN_IDENT;
	}
	return token;
}
CSSToken* CSSLex::CSSTokenMatch()
//INCLUDES       = "~="
//DASHMATCH      = "|="
//PREFIXMATCH    = "^="
//SUFFIXMATCH    = "$="
//SUBSTRINGMATCH = "*="
{
	CSSToken* token = new CSSToken();
	unsigned char c = NextChar(buffer);
	if (c != '=')
	{
		--pos.forwardPos;
		token->type = CSS_TOKEN_CHAR;
		return token;
	}
	switch (buffer[pos.firstPos])
	{
	case '~':
		token->type = CSS_TOKEN_INCLUDES;
		break;
	case '|':
		token->type = CSS_TOKEN_DASHMATCH;
		break;
	case '^':
		token->type = CSS_TOKEN_PREFIXMATCH;
		break;
	case '$':
		token->type = CSS_TOKEN_SUFFIXMATCH;
		break;
	case '*':
		token->type = CSS_TOKEN_SUBSTRINGMATCH;
		break;
	}
	token->data = buffer[pos.firstPos] + '=';
	return token;
}
CSSToken* CSSLex::CSSTokenURL()//URI = "url(" w (string | urlchar*) w ')'
{
	return nullptr;
}
CSSToken* CSSLex::CSSTokenURIOrUnicodeRangeOrIdentOrFunction()
//URI = "url(" w (string | urlchar*) w ')'
//UNICODE-RANGE = [Uu] '+' [0-9a-fA-F?]{1,6}(-[0-9a-fA-F]{1,6})?
//IDENT = ident = [-]? nmstart nmchar*
//FUNCTION = ident '(' = [-]? nmstart nmchar* '('
{
	unsigned char c;
	if ('r' == (c = NextChar(buffer)) || 'R' == c)
	{
		return CSSTokenURL();
	}
	else if (c == '+')
	{
		return CSSTokenUnicodeRange();
	}
	return CSSTokenIdentOrFunction();
}
CSSToken* CSSLex::GetToken()
{
	CSSToken* newToken = new CSSToken();
	std::string data;
	bool stopLoop = false;
	pos.firstPos = pos.forwardPos;
	static CSSDFAStatus STATUS;
	char c = NextChar(buffer);//读取下一个字符
	switch (STATUS)
	{
	case Start:
		switch (c)
		{
		case '@'://ATKEYWORD = '@' ident  = '@' '-'? nmstart nmchar*
		{
			newToken = CSSTokenAtkeyword();
			break;
		}
		case '#'://HASH = '#' name  = '#' nmchar+
		{
			newToken = CSSTokenHash();
			break;
		}
		case '"':
		case '\''://STRING = string
		{
			newToken = CSSTokenString();
			break;
		}
		case '/'://COMMENT = '/' '*' [^*]* '*'+ ([^/] [^*]* '*'+)* '/'
		{
			newToken = CSSTokenComment();
		}
		case 'u':
		case 'U':
		{
			newToken = CSSTokenURIOrUnicodeRangeOrIdentOrFunction();
		}
		case NUMBER_CASE:
		{
			newToken = CSSTokenNumberOrPercentageOrDimension();
			break;
		}
		case CHAR_CASE:
		{
			newToken = CSSTokenIdentOrFunction();
			break;
		}
		case MATCH_CASE:
		{
			newToken = CSSTokenMatch();
			break;
		}
		case WS_CASE:
		{
			newToken->data=c;
			newToken->type=CSS_TOKEN_S;
			break;
		}
		case '>':
		{
			char cc=NextChar(buffer);
			newToken->data=c;
			if(cc=='=')
			newToken->data+=cc;
			newToken->type=CSS_TOKEN_CHAR;
			break;
		}
		default:
		{
			newToken->data = c;
			newToken->type = CSS_TOKEN_CHAR;
			break;
		}
		}
		break;
	}
	tokenCache.push_back(*newToken);
	return newToken;
}
unsigned char CSSLex::getEscape()//escape     ::= unicode | '\' [^\n\r\f0-9a-fA-F]
{
	unsigned char c = NextChar(buffer);
	if (c == '\n' || c == '\r' || c == '\f' || isHexCharacter(c))
	{
		return EOF;
	}
	return c;
}
std::string CSSLex::getNMChars()//nmchar = [a-zA-Z] | '-' | '_' | nonascii | escape
{
	unsigned char c;
	std::string s;
	while (isNMStart(c = NextChar(buffer)))
	{
		if (c != '\\')
		{
			s += c;
		}
		else
		{
			s += getEscape();
		}
	}
	if (c != 0)--pos.forwardPos;
	return s;
}
std::string CSSLex::getString()//string = '"' (stringchar | "'")* '"' | "'" (stringchar | '"')* "'"
{
	unsigned char quote = buffer[pos.forwardPos - 1];
	unsigned char permittedquote = (quote == '"') ? '\'' : '"';
	unsigned char c;
	std::string s;
	while (quote != (c = NextChar(buffer)))
	{
		if (c == permittedquote)
		{
			s += c;
		}
		else if (isStringChar(c))
		{
			s += getStringChars();
		}
		else if (c != quote)
		{
			throw;
		}
	}
	if (c != 0)--pos.forwardPos;
	return s;
}
std::string CSSLex::getStringChars()//stringchar = urlchar | ' ' | ')' | '\' nl
{
	unsigned char c;
	std::string s;
	while (isStringChar(c = NextChar(buffer)) || c == '\\')
	{
		if (c != '\\')
		{
			s += c;
		}
		else
		{
			s += getEscape();
		}
	}
	if (c != 0)--pos.forwardPos;
	return s;
}
std::string CSSLex::getDigits()//digit = [0-9]
{
	unsigned char c;
	std::string s;
	while (isDigit(c = NextChar(buffer)))
	{
		s += getEscape();
	}
	if (c != 0)--pos.forwardPos;
	return s;
}
inline bool CSSLex::isDigit(unsigned char c)//数字
{
	return (c >= '0' && c <= '9');
}

inline bool CSSLex::isLetter(unsigned char c)//字母
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

inline bool CSSLex::isHexCharacter(unsigned char c)//16进制
{
	return isDigit(c) || (c >= 'a' && c < 'f') || (c > 'A' && c < 'F');
}
inline bool CSSLex::isWs(unsigned char c)//空格
{
	return (c == ' ' || c == '\r' || c == '\n' || c == '\f' || c == '\t');
}
inline bool CSSLex::isNMStart(unsigned char c)//nmstart    ::= [a-zA-Z] | '_' | nonascii | escape
{
	return (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_' || c >= 0x80 || c == '\\');
}
inline bool CSSLex::isStringChar(unsigned char c)
{
	return isURLChar(c) || c == ' ' || c == ')';
}

inline bool CSSLex::isURLChar(unsigned char c)
{
	return c == '\t' || c == '!' || ('#' <= c && c <= '&') || c == '(' ||
		   ('*' <= c && c <= '~') || c >= 0x80 || c == '\\';
}
CSSToken* CSSLex::CSSTokenUnicodeRange()//UNICODE-RANGE = [Uu] '+' [0-9a-fA-F?]{1,6}(-[0-9a-fA-F]{1,6})?
{
	return nullptr;
}
