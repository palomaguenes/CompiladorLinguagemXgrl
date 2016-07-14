WS      [\t\n ]
DIGITO  [0-9]
LETRA   [A-Za-z_]
ID      {LETRA}({LETRA}|{DIGITO})*
REAL ({DIGITO}+[.]{DIGITO}*)|({DIGITO}*[.]{DIGITO}+)

USANDOISSO 		[Uu][Ss][Aa][Nn][Dd][Oo][[Ii][Ss][Ss][Oo]
PALAVRA			[Pp][Aa][Ll][Aa][Vv][Rr][Aa]
NUMEROSEMPONTO	[Nn][Uu][Mm][Ee][Rr][Oo][Ss][Ee][Mm][Pp][Oo][Nn][Tt][Oo]
NUMEROCOMPONTO	[Nn][Uu][Mm][Ee][Rr][Oo][Cc][Oo][Mm][Pp][Oo][Nn][Tt][Oo]
EXECUTEISSO		[Ee][Xx][Ee][Cc][Uu][Tt][Ee][Ii][Ss][Ss][Oo]

MOSTRE		[Mm][Oo][Ss][Tt][Rr][Ee]
SE			[Ss][Ee]
EHVERDADE	[Ee][Hh][Vv][Ee][Rr][Dd][Aa][Dd][Ee]
EHMENTIRA	[Ee][Hh][Mm][Ee][Nn][Tt][Ii][Rr][Aa]
COM			[Cc][Oo][Mm]
FACA		[Ff][Aa][Cc][Aa]
ENQUANTO	[Ee][Nn][Qq][Uu][Aa][Nn][Tt][Oo]
REPITA		[Rr][Ee][Pp][Ii][Tt][Aa]
EXECUTE		[Ee][Xx][Ee][Cc][Uu][Tt][Ee]
FUNCTION	[Ff][Uu][Nn][Cc][Tt][Ii][Oo][Nn]

VALE	[Vv][Aa][Ll][Ee]

CTE_SIMBOLO "'"([^'\n]|"''")"'"
CTE_PALAVRA	"'"([^'\n]|"''")*"'"
CTE_NUMEROSEMPONTO {DIGITO}+
CTE_NUMEROCOMPONTO {REAL}

%%

{WS} {}

"TUDAO"				{ yylval = yytext; return _TUDAO; }
{USANDOISSO} 		{ yylval = yytext; return _USANDOISSO; }
{PALAVRA} 			{ yylval = yytext; return _PALAVRA; }
{NUMEROSEMPONTO} 	{ yylval = yytext; return _NUMEROSEMPONTO; }
{NUMEROCOMPONTO} 	{ yylval = yytext; return _NUMEROCOMPONTO; }
{EXECUTEISSO} 		{ yylval = yytext; return _EXECUTEISSO; }

{MOSTRE} 	{ yylval = yytext; return _MOSTRE; }
{SE}		{ yylval = yytext; return _SE; }
{EHVERDADE} { yylval = yytext; return _EHVERDADE; }
{EHMENTIRA} { yylval = yytext; return _EHMENTIRA; }
{COM} 		{ yylval = yytext; return _COM; }
{FACA} 		{ yylval = yytext; return _FACA; }
{ENQUANTO} 	{ yylval = yytext; return _ENQUANTO; }
{REPITA}	{ yylval = yytext; return _REPITA; }
{EXECUTE}   { yylval = yytext; return _EXECUTE; }
{FUNCTION}  { yylval = yytext; return _FUNCTION; }

{CTE_SIMBOLO} 			{ yylval = yytext; return _CTE_SIMBOLO; }
{CTE_PALAVRA} 			{ yylval = yytext; return _CTE_PALAVRA; }
{CTE_NUMEROSEMPONTO} 	{ yylval = yytext; return _CTE_NUMEROSEMPONTO; }
{CTE_NUMEROCOMPONTO} 	{ yylval = yytext; return _CTE_NUMEROCOMPONTO; }

{VALE}			{ yylval = yytext; return _ATRIB; }

{ID}  { yylval = yytext; return _ID; }

.     { yylval = yytext; return yytext[0]; }

%%

 


