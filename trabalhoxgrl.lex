WS      [\t\n ]
DIGITO  [0-9]
LETRA   [A-Za-z_]
ID      {LETRA}({LETRA}|{DIGITO})*
REAL 	({DIGITO}+[.]{DIGITO}*)|({DIGITO}*[.]{DIGITO}+)

USANDOISSO 	[Uu][Ss][Aa][Nn][Dd][Oo][Ii][Ss][Ss][Oo]
PALAVRA		[Pp][Aa][Ll][Aa][Vv][Rr][Aa]
NUMEROSEMPONTO	[Nn][Uu][Mm][Ee][Rr][Oo][Ss][Ee][Mm][Pp][Oo][Nn][Tt][Oo]
NUMEROCOMPONTO	[Nn][Uu][Mm][Ee][Rr][Oo][Cc][Oo][Mm][Pp][Oo][Nn][Tt][Oo]
EXECUTEISSO	[Ee][Xx][Ee][Cc][Uu][Tt][Ee][Ii][Ss][Ss][Oo]

WRITELN		[Ww][Rr][Ii][Tt][Ee][Ll][Nn]
WRITE		[Ww][Rr][Ii][Tt][Ee]


VAR			[Vv][Aa][Rr]
IF			[Ii][Ff]
THEN		[Tt][Hh][Ee][Nn]
ELSE		[Ee][Ll][Ss][Ee]
FOR			[Ff][Oo][Rr]
TO			[Tt][Oo]
DO			[Dd][Oo]
FUNCTION	[Ff][Uu][Nn][Cc][Tt][Ii][Oo][Nn]

CTE_PALAVRA	"'"([^'\n]|"''")*"'"
CTE_NUMEROSEMPONTO {DIGITO}+
CTE_NUMEROCOMPONTO {REAL}

%%

{WS} {}

{USANDOISSO} 		{ yylval = yytext; return _USANDOISSO; }
{PALAVRA} 		{ yylval = yytext; return _PALAVRA; }
{NUMEROSEMPONTO} 	{ yylval = yytext; return _NUMEROSEMPONTO; }
{NUMEROCOMPONTO} 	{ yylval = yytext; return _NUMEROCOMPONTO; }

{EXECUTEISSO} 	{ yylval = yytext; return _EXECUTEISSO; }
{WRITELN} 	{ yylval = yytext; return _WRITELN; }
{WRITE} 	{ yylval = yytext; return _WRITE; }
{VAR} 		{ yylval = yytext; return _VAR; }
{IF} 		{ yylval = yytext; return _IF; }
{THEN} 		{ yylval = yytext; return _THEN; }
{ELSE} 		{ yylval = yytext; return _ELSE; }
{FOR} 		{ yylval = yytext; return _FOR; }
{TO} 		{ yylval = yytext; return _TO; }
{DO} 		{ yylval = yytext; return _DO; }
{FUNCTION}  { yylval = yytext; return _FUNCTION; }

{CTE_PALAVRA} 		{ yylval = yytext; return _CTE_PALAVRA; }
{CTE_NUMEROSEMPONTO} 	{ yylval = yytext; return _CTE_NUMEROSEMPONTO; }
{CTE_NUMEROCOMPONTO} 	{ yylval = yytext; return _CTE_NUMEROCOMPONTO; }

":="			{ yylval = yytext; return _ATRIB; }

{ID}  { yylval = yytext; return _ID; }

.     { yylval = yytext; return yytext[0]; }

%%

 


