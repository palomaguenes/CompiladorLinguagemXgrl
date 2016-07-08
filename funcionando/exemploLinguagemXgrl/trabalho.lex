WS      [\t\n ]
DIGITO  [0-9]
LETRA   [A-Za-z_]
ID      {LETRA}({LETRA}|{DIGITO})*

USANDOISSO 		[Uu][Ss][Aa][Nn][Dd][Oo][[Ii][Ss][Ss][Oo]


PROGRAM 	[Pp][Rr][Oo][Gg][Rr][Aa][Mm]
BEGIN   	[Bb][Ee][Gg][Ii][Nn]
END			[Ee][Nn][Dd]
WRITELN		[Ww][Rr][Ii][Tt][Ee][Ll][Nn]
WRITE		[Ww][Rr][Ii][Tt][Ee]
INTEGER		[Ii][Nn][Tt][Ee][Gg][Ee][Rr]
STRING		[Ss][Tt][Rr][Ii][Nn][Gg]
VAR			[Vv][Aa][Rr]
IF			[Ii][Ff]
THEN		[Tt][Hh][Ee][Nn]
ELSE		[Ee][Ll][Ss][Ee]
FOR			[Ff][Oo][Rr]
TO			[Tt][Oo]
DO			[Dd][Oo]
FUNCTION	[Ff][Uu][Nn][Cc][Tt][Ii][Oo][Nn]

CTE_STRING	"'"([^'\n]|"''")*"'"
CTE_INTEGER {DIGITO}+

%%

{WS} {}
{USANDOISSO} 		{ yylval = yytext; return _USANDOISSO; }


{PROGRAM} 	{ yylval = yytext; return _PROGRAM; }
{BEGIN} 	{ yylval = yytext; return _BEGIN; }
{END} 		{ yylval = yytext; return _END; }
{WRITELN} 	{ yylval = yytext; return _WRITELN; }
{WRITE} 	{ yylval = yytext; return _WRITE; }
{STRING} 	{ yylval = yytext; return _STRING; }
{INTEGER} 	{ yylval = yytext; return _INTEGER; }
{VAR} 		{ yylval = yytext; return _VAR; }
{IF} 		{ yylval = yytext; return _IF; }
{THEN} 		{ yylval = yytext; return _THEN; }
{ELSE} 		{ yylval = yytext; return _ELSE; }
{FOR} 		{ yylval = yytext; return _FOR; }
{TO} 		{ yylval = yytext; return _TO; }
{DO} 		{ yylval = yytext; return _DO; }
{FUNCTION}  { yylval = yytext; return _FUNCTION; }

{CTE_STRING} 	{ yylval = yytext; return _CTE_STRING; }
{CTE_INTEGER} 	{ yylval = yytext; return _CTE_INTEGER; }

":="			{ yylval = yytext; return _ATRIB; }

{ID}  { yylval = yytext; return _ID; }

.     { yylval = yytext; return yytext[0]; }

%%

 


