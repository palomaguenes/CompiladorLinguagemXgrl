%{
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

struct Tipo {
  string nome;  // nome na nossa linguagem
  string decl;  // declaração correspondente em c-assembly
  string fmt;   // formato para "printf"
};

Tipo Integer = { "numerosemponto", "int", "d" };
Tipo Float =   { "numerocomponto", "float", "f" };
Tipo Double =  { "numerograndecomponto", "double", "lf" };
Tipo Boolean = { "vouf", "int", "d" };
Tipo String =  { "palavra", "char[256]", "s" };
Tipo Char =    { "simbolo", "char", "c" };

struct Atributo {
  string v, c;
  Tipo t;
  vector<string> lst;
}; 

#define YYSTYPE Atributo

int yylex();
int yyparse();
void yyerror(const char *);
void erro( string );

map<string,Tipo> ts;
map< string, map< string, Tipo > > tiporesultado;

ostream& operator << ( ostream& o, const vector<string>& st ) {
  cout << "[ ";
  for( vector<string>::const_iterator itr = st.begin();
       itr != st.end(); ++itr )
    cout << *itr << " "; 
       
  cout << "]";
  return o;     
}

void declara_variavel( Atributo& ss, 
                       const Atributo& s1, const Atributo& s3 ) {
  ss.c = "";
  for( int i = 0; i < s1.lst.size(); i++ ) {
    if( ts.find( s1.lst[i] ) != ts.end() ) 
      erro( "Você já declarou a variável " + s1.lst[i] );
    else {
      ts[ s1.lst[i] ] = s3.t; 
      ss.c += s3.t.decl + " " + s1.lst[i] + ";\n"; 
    }  
  }
}

void gera_codigo_atribuicao( Atributo& ss, 
                             const Atributo& s1, 
                             const Atributo& s3 ) {
  if( s1.t.nome == s3.t.nome ){
    ss.c = s1.c + s3.c + "  " + s1.v + " = " + s3.v + ";\n";
  }
}

void busca_tipo_da_variavel( Atributo& ss, const Atributo& s1 ) {
  if( ts.find( s1.v ) == ts.end() )
        erro( "Variável não declarada: " + s1.v );
  else {
    ss.t = ts[ s1.v ];
    ss.v = s1.v;
  }
}

string par( Tipo a, Tipo b ) {
  return a.nome + "," + b.nome;  
}

void gera_codigo_operador( Atributo& ss, 
                           const Atributo& s1, 
                           const Atributo& s2, 
                           const Atributo& s3 ) {
  if( tiporesultado.find( s2.v ) != tiporesultado.end() ) {
    if( tiporesultado[s2.v].find( par( s1.t, s3.t ) ) != tiporesultado[s2.v].end() ) {
      ss.v = "t1"; // Precisa gerar um nome de variável temporária.
      ss.t =  tiporesultado[s2.v][par( s1.t, s3.t )];
      ss.c = s1.c + s3.c + "  " + ss.v + " = " + s1.v + s2.v + s3.v + ";\n";
    }
    else
      erro( "O operador '" + s2.v + "' não está definido para os tipos " + s1.t.nome + " e " + s3.t.nome + "." );
  }
  else
    erro( "Operador '" + s2.v + "' não definido." );
}

%}

%token _ID _TUDAO _USANDOISSO _EXECUTEISSO _SE _EHVERDADE _EHMENTIRA 
%token _MOSTRE _ATRIB _COM _FACA _ENQUANTO _REPITA _EXECUTE 
%token _NUMEROSEMPONTO _PALAVRA _NUMEROCOMPONTO _NUMEROGRANDECOMPONTO _SIMBOLO _VOUF
%token _ESCOLHA _SEFOR _OK _CASOCONTRARIO
%token _FUNCAO _RECEBE _RETORNA _NADA

%token _CTE_PALAVRA _CTE_NUMEROSEMPONTO _CTE_NUMEROCOMPONTO _CTE_NUMEROGRANDECOMPONTO _CTE_SIMBOLO

%token _RESTO

%nonassoc '>' '<' '=' ">=" "<="
%left _RESTO
%left '+' '-'
%left '*' '/'

%start S

%%

S : TUDAO { cout << $1.c << endl; }
  ;
    
TUDAO  : _TUDAO '{' USANDOISSO FUNCTIONDECLS EXECUTEISSO '}'
		{ $$.c = "#include <stdlib.h>\n"
             "#include <stdio.h>\n\n" + $3.c + "\n" + $4.c +
				 "int main() {\n" + $5.c + "}\n";	  
		 }
       ;

USANDOISSO  : _USANDOISSO '{' DECLS '}'  { $$.c = $3.c; }
	    	| { $$.c = ""; }
	    	;

FUNCTIONDECLS : FUNCTIONDECL FUNCTIONDECLS
			  |
			  ;

FUNCTIONDECL : _FUNCAO _ID _RECEBE '(' PARAMETROS ')' _RETORNA '('_ID ':' TIPO ')' '{' USANDOISSO EXECUTEISSO'}'
			 | _FUNCAO _ID _RECEBE '(' PARAMETROS ')' '{' USANDOISSO EXECUTEISSO'}'
			 ;

PARAMETROS : DECL ',' PARAMETROS
		   | DECL
		   | 
           ;
     
EXECUTEISSO : _EXECUTEISSO '{' MIOLOS '}'
		  {$$.c = $3.c; }
	    | {$$.c = "";}
	    ;

DECLS : DECL ';' DECLS { $$.c = $1.c + $3.c; }
      |	DECL ';'
      ;
     
DECL : IDS ':' TIPO  { declara_variavel( $$, $1, $3 ); }     
     ;      

IDS : IDS '&' _ID { $$.lst = $1.lst; $$.lst.push_back( $3.v ); }
    | _ID		  { $$.lst.push_back( $1.v ); }
    ; 

TIPO : _NUMEROSEMPONTO		{ $$.t = Integer; }
     | _NUMEROCOMPONTO		{ $$.t = Float; }
	 | _NUMEROGRANDECOMPONTO { $$.t = Double; }
     | _PALAVRA TAM_PALAVRA
	 | _SIMBOLO			{ $$.t = Char; }
	 | _VOUF			{ $$.t = Boolean; }
     ;

TAM_PALAVRA: '[' _CTE_NUMEROSEMPONTO ']'
           |
           ; 

  
MIOLOS : MIOLO MIOLOS
		 { $$.c = $1.c + $2.c; }
       | 
		 { $$.c = ""; }
       ;
       
MIOLO : CHAMADAFUNCAO
      | CMD
      ;              

CHAMADAFUNCAO: _ID '(' PARAM_CHAMADA ')' ';'
	     ;     

PARAM_CHAMADA: FS
	     ;

FS: F ',' FS
  | F
  |
  ;

CMD : MOSTRE
    | CMD_SE
	| CMD_ATRIB
	| CMD_FOR
	| CMD_WHILE
	| CMD_DOWHILE
	| CMD_ESCOLHA
    ; 

CMD_ESCOLHA : _ESCOLHA F '{' ESCOLHAS '}'
			;

ESCOLHAS : CASOS CASOCONTRARIO
		 ;

CASOS : CASO CASOS
	  | 
	  ;

CASO : _SEFOR F ':' MIOLOS _OK
	 ;

CASOCONTRARIO : _CASOCONTRARIO ':' MIOLOS
			  ;

MOSTRE: _MOSTRE E ';' { $$.c = "  printf( \"%"+ $2.t.fmt + "\\n\", " + $2.v + " );\n"; }
      ; 

CMD_ATRIB : IDATR INDICE _ATRIB E ';'
			      { gera_codigo_atribuicao( $$, $1, $4); }
	  	    | IDATR INDICE _ATRIB CHAMADAFUNCAO
          ;

CMD_ATRIB_SPV : _ID INDICE _ATRIB E
     				  ;

IDATR: _ID { busca_tipo_da_variavel( $$, $1 ); }	
	   ;
          
INDICE : '[' EXPS ']' INDICE
       |
       ;         
       
EXPS : E ',' EXPS
     | E
     ;

CMD_SE : _SE '(' E ')' _EHVERDADE '{' CMD '}'
	   | _SE '(' E ')' _EHVERDADE '{' CMD '}' _EHMENTIRA '{' CMD '}'
	   ;

CMD_FOR : _COM '(' CMD_ATRIB_SPV ')' _FACA '(' CMD_ATRIB_SPV ')' _ENQUANTO '(' E ')' '{' CMD '}'
        ;

CMD_WHILE : _REPITA _SE '(' E ')' '{' CMD '}'
		  ;

CMD_DOWHILE : _EXECUTE '{' CMD '}' _REPITA _SE '(' E ')' ';'
			;

E : E '+' E { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '-' E { gera_codigo_operador( $$, $1, $2, $3 ); }     
  | E '*' E { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '/' E { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '>' E 
  | E '<' E 
  | E "<=" E
  | E ">=" E
  | _RESTO '(' E '/' E ')' 
  | E '=' E 
  | F
  ;
  
F : _CTE_PALAVRA        { $$ = $1; $$.t = String; }
  | _CTE_NUMEROSEMPONTO { $$ = $1; $$.t = Integer; }
  | _CTE_NUMEROCOMPONTO { $$ = $1; $$.t = Float; }
  | _CTE_NUMEROGRANDECOMPONTO { $$ = $1; $$.t = Double; }
  | _CTE_SIMBOLO        { $$ = $1; $$.t = Char; }
  | _ID
  | '(' E ')'
  ;
 
%%

#include "lex.yy.c"

void erro( string st ) {
  yyerror( st.c_str() );
  exit( 1 );
}

void yyerror( const char* st )
{
   if( strlen( yytext ) == 0 )
     fprintf( stderr, "%s\nNo final do arquivo\n", st );
   else  
     fprintf( stderr, "%s\nProximo a: %s\nlinha/coluna: %d/%d\n", st, 
              yytext, yylineno, yyrowno - (int) strlen( yytext ) );
}

void inicializa_tabela_de_resultado_de_operacoes() {
  
    map< string, Tipo > r;

    r[par(Integer, Integer)] = Integer;    
    r[par(Integer, Float)] = Float;    
    r[par(Integer, Double)] = Double;    
    r[par(Float, Integer)] = Float;    
    r[par(Float, Float)] = Float;    
    r[par(Float, Double)] = Double;    
    r[par(Double, Integer)] = Double;    
    r[par(Double, Float)] = Double;    
    r[par(Double, Double)] = Double;    

    tiporesultado[ "-" ] = r; 
    tiporesultado[ "*" ] = r; 
    tiporesultado[ "/" ] = r; 

    r[par(Char, Char)] = String;      
    r[par(String, Char)] = String;      
    r[par(Char, String)] = String;    
    r[par(String, String)] = String;    
    tiporesultado[ "+" ] = r; 
}


int main( int argc, char* argv[] )
{
  inicializa_tabela_de_resultado_de_operacoes();
  yyparse();
}

