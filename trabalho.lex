%{
int yyrowno = 1;
void trata_folha();
void trata_aspas_simples();
%}
WS      [\t\n ]
DIGITO  [0-9]
LETRA   [A-Za-z_]
ID      {LETRA}({LETRA}|{DIGITO})*
REAL ({DIGITO}+[.]{DIGITO}*)|({DIGITO}*[.]{DIGITO}+)
FLOAT {DIGITO}+("."{DIGITO}+)

USANDOISSO 			[Uu][Ss][Aa][Nn][Dd][Oo][[Ii][Ss][Ss][Oo]
PALAVRA				[Pp][Aa][Ll][Aa][Vv][Rr][Aa]
SIMBOLO				[Ss][Ii][Mm][Bb][Oo][Ll][Oo]
NUMEROSEMPONTO		[Nn][Uu][Mm][Ee][Rr][Oo][Ss][Ee][Mm][Pp][Oo][Nn][Tt][Oo]
NUMEROCOMPONTO		[Nn][Uu][Mm][Ee][Rr][Oo][Cc][Oo][Mm][Pp][Oo][Nn][Tt][Oo]
NUMEROGRANDECOMPONTO[Nn][Uu][Mm][Ee][Rr][Oo][Gg][Rr][Aa][Nn][Dd][Ee][Cc][Oo][Mm][Pp][Oo][Nn][Tt][Oo]
VOUF				[Vv][Oo][Uu][Ff]
EXECUTEISSO			[Ee][Xx][Ee][Cc][Uu][Tt][Ee][Ii][Ss][Ss][Oo]

MOSTRE		[Mm][Oo][Ss][Tt][Rr][Ee]
SE			[Ss][Ee]
EHVERDADE	[Ee][Hh][Vv][Ee][Rr][Dd][Aa][Dd][Ee]
EHMENTIRA	[Ee][Hh][Mm][Ee][Nn][Tt][Ii][Rr][Aa]
COM			[Cc][Oo][Mm]
FACA		[Ff][Aa][Cc][Aa]
ENQUANTO	[Ee][Nn][Qq][Uu][Aa][Nn][Tt][Oo]
REPITA		[Rr][Ee][Pp][Ii][Tt][Aa]
EXECUTE		[Ee][Xx][Ee][Cc][Uu][Tt][Ee]

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
CTE_NUMEROCOMPONTO {FLOAT}
CTE_NUMEROGRANDECOMPONTO {REAL}
%%

{WS} {}

"TUDAO"					{  trata_folha(); return _TUDAO; }
{USANDOISSO} 			{  trata_folha(); return _USANDOISSO; }
{PALAVRA} 				{  trata_folha(); return _PALAVRA; }
{NUMEROSEMPONTO} 		{  trata_folha(); return _NUMEROSEMPONTO; }
{NUMEROCOMPONTO} 		{  trata_folha(); return _NUMEROCOMPONTO; }
{NUMEROGRANDECOMPONTO} 	{  trata_folha(); return _NUMEROGRANDECOMPONTO; }
{SIMBOLO}				{  trata_folha(); return _SIMBOLO; }
{VOUF}					{  trata_folha(); return _VOUF; }
{EXECUTEISSO} 			{  trata_folha(); return _EXECUTEISSO; }

{MOSTRE} 	{  trata_folha(); return _MOSTRE; }
{SE}		{  trata_folha(); return _SE; }
{EHVERDADE} {  trata_folha(); return _EHVERDADE; }
{EHMENTIRA} {  trata_folha(); return _EHMENTIRA; }
{COM} 		{  trata_folha(); return _COM; }
{FACA} 		{  trata_folha(); return _FACA; }
{ENQUANTO} 	{  trata_folha(); return _ENQUANTO; }
{REPITA}	{  trata_folha(); return _REPITA; }
{EXECUTE}   {  trata_folha(); return _EXECUTE; }

{ESCOLHA}  {  trata_folha(); return _ESCOLHA; }
{SEFOR}  {  trata_folha(); return _SEFOR; }
{OK}  {  trata_folha(); return _OK; }
{CASOCONTRARIO}  {  trata_folha(); return _CASOCONTRARIO; }

{FUNCAO}	{  trata_folha(); return _FUNCAO; }
{RECEBE}	{  trata_folha(); return _RECEBE; }
{RETORNA}	{  trata_folha(); return _RETORNA; }
{NADA}		{  trata_folha(); return _NADA; }

{CTE_SIMBOLO} 				{ trata_aspas_simples(); return _CTE_SIMBOLO; }
{CTE_PALAVRA} 				{ trata_aspas_simples(); return _CTE_PALAVRA; }
{CTE_NUMEROSEMPONTO} 		{ trata_folha(); return _CTE_NUMEROSEMPONTO; }
{CTE_NUMEROCOMPONTO} 		{ trata_folha(); return _CTE_NUMEROCOMPONTO; }
{CTE_NUMEROGRANDECOMPONTO} 	{ trata_folha(); return _CTE_NUMEROGRANDECOMPONTO; }

{VALE}			{ trata_folha(); return _ATRIB; }

{RESTO}			{  trata_folha(); return _RESTO; }

{ID}  {  trata_folha(); return _ID; }

.     {  trata_folha(); return yytext[0]; }

%%

void trata_folha() {
	yylval.v = yytext;
	yylval.t.nome = "";
  yylval.t.decl = "";
  yylval.t.fmt = "";
  yylval.c = "";
	yylval.lst.clear();
  
  yyrowno += strlen( yytext ); 
}

void trata_aspas_simples() {
  trata_folha(); 
  yylval.v = "\"" + yylval.v.substr( 1, yylval.v.length()-2 ) + "\""; 
}
 


