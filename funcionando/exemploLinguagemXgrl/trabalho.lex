WS      [\t\n ]
DIGITO  [0-9]
LETRA   [A-Za-z_]
ID      {LETRA}({LETRA}|{DIGITO})*
REAL ({DIGITO}+[.]{DIGITO}*)|({DIGITO}*[.]{DIGITO}+)

USANDOISSO 		[Uu][Ss][Aa][Nn][Dd][Oo][[Ii][Ss][Ss][Oo]
PALAVRA			[Pp][Aa][Ll][Aa][Vv][Rr][Aa]
SIMBOLO			[Ss][Ii][Mm][Bb][Oo][Ll][Oo]
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

ESCOLHA 	[Ee][Ss][Cc][Oo][Ll][Hh][Aa]
SEFOR		[Ss][Ee][Ff][Oo][Rr]
OK		[Oo][Kk]
CASOCONTRARIO	[Cc][Aa][Ss][Oo][Cc][Oo][Nn][Tt][Rr][Aa][Rr][Ii][Oo]

FUNCAO	[Ff][Uu][Nn][Cc][Aa][Oo]
RECEBE	[Rr][Ee][Cc][Ee][Bb][Ee]
RETORNA	[Rr][Ee][Tt][Oo][Rr][Nn][Aa]
NADA	[Nn][Aa][Dd][Aa]

VALE	[Vv][Aa][Ll][Ee]

RESTO [Rr][Ee][Ss][Tt][Oo]

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
{SIMBOLO}			{ yylval = yytext; return _SIMBOLO; }
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

{ESCOLHA}  { yylval = yytext; return _ESCOLHA; }
{SEFOR}  { yylval = yytext; return _SEFOR; }
{OK}  { yylval = yytext; return _OK; }
{CASOCONTRARIO}  { yylval = yytext; return _CASOCONTRARIO; }

{FUNCAO}	{ yylval = yytext; return _FUNCAO; }
{RECEBE}	{ yylval = yytext; return _RECEBE; }
{RETORNA}	{ yylval = yytext; return _RETORNA; }
{NADA}		{ yylval = yytext; return _NADA; }

{CTE_SIMBOLO} 			{ yylval = yytext; return _CTE_SIMBOLO; }
{CTE_PALAVRA} 			{ yylval = yytext; return _CTE_PALAVRA; }
{CTE_NUMEROSEMPONTO} 	{ yylval = yytext; return _CTE_NUMEROSEMPONTO; }
{CTE_NUMEROCOMPONTO} 	{ yylval = yytext; return _CTE_NUMEROCOMPONTO; }

{VALE}			{ yylval = yytext; return _ATRIB; }

{RESTO}			{ yylval = yytext; return _RESTO; }

{ID}  { yylval = yytext; return _ID; }

.     { yylval = yytext; return yytext[0]; }

%%

 


