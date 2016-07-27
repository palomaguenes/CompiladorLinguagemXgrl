%{
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

struct Tipo {
  string nome;  // nome na nossa linguagem
  string decl;  // declaração correspondente em c-assembly
  string fmt;   // formato para "printf"
  int dim;
};

Tipo Integer =	{ "numerosemponto", "int", "d" };
Tipo Float =	{ "numerocomponto", "float", "f" };
Tipo Double =	{ "numerograndecomponto", "double", "lf" };
Tipo Boolean =	{ "vouf", "int", "d" };
Tipo String =	{ "palavra", "char", "s" };
Tipo Char = 	{ "simbolo", "char", "c" };
Tipo Void = 	{"vazio", "void"};

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

vector< map<string,Tipo> > ts;
map<string, vector<Tipo> > tf; 

map< string, map< string, Tipo > > tiporesultado;

map< string, int > temp_global;
map< string, int > temp_local;
map< string, int > nlabel;
bool escopo_local = false;

struct Sw{
	Atributo var;
	vector<string> casos;
	vector<string> cod_casos;
};

Sw swvar;

int nsw = 0;

string toString( int n ) {
  char buf[256] = "";
  
  sprintf( buf, "%d", n );
  
  return buf;
}

int toInt( string valor ) {
  int aux = 0;
  
  sscanf( valor.c_str(), "%d", &aux );
  
  return aux;
}

void generateLabels(std::vector<string>* label, string tipo){

	string label_parte1 = "sw_" + toString(nsw) + "_";

	for (int i = 0; i < swvar.casos.size()-1 ; i++){
		
		if (tipo == "simbolo"){
			(*label).push_back( label_parte1 + swvar.casos[i].at(1) );
		}
		else{
			(*label).push_back( label_parte1 + swvar.casos[i] );
		} 
	}

	(*label).push_back( label_parte1 + "default" +":;\n");

}

void gera_switch(Atributo& ss){

	if (swvar.var.t.nome != "numerosemponto" && swvar.var.t.nome != "simbolo"){
		erro("ESCOLHA recebe numerosemponto e simbolo com parâmetros!\n");
	}
	
	vector<string> labels; 
	generateLabels(&labels, swvar.var.t.nome);
	
	for (int i = 0; i < swvar.casos.size()-1 ; i++){
		ss.c = ss.c + "  if(" + swvar.var.v + " == " + swvar.casos[i] + ") goto " + labels[i] + ";\n";
	}

	int indice = swvar.casos.size()-1;

	ss.c = "\n" + ss.c + labels[indice] + swvar.cod_casos[indice] + "  goto fim_sw_" + toString(nsw) + ";\n";

	for (int i = 0; i < swvar.casos.size()-1 ; i++){
		ss.c = ss.c + labels[i] + ":;\n" + swvar.cod_casos[i] + "  goto fim_sw_" + toString(nsw) + ";\n";
	}

	ss.c = ss.c + "fim_sw_" + toString(nsw) + ":;\n\n";

}

void insereFuncao(string nome, Tipo retorno){

	vector<Tipo> v;

	if ( tf.find( nome ) != tf.end() ){
		
		v = tf[nome];
		for (int i = 0; i < v.size(); i++){
			if ( v[i].nome == retorno.nome){
				erro("A função de nome '"+ nome +"' com retorno '"+ retorno.nome +"' já existe!");			
			}
		}

		v.push_back(retorno);
	}
	else{

		v.push_back(retorno);
		tf[nome] = v;
	}

}

void empilha_nova_tabela_de_simbolos() {
  ts.push_back( map< string, Tipo >() );
}

void desempilha_tabela_de_simbolos() {
  ts.pop_back();
}


string gera_nome_var( Tipo t ) {
  return "t_" + t.nome + "_" + 
   toString( ++(escopo_local ? temp_local : temp_global)[t.nome] );
}

string gera_nome_label( string cmd ) {
  return "L_" + cmd + "_" + toString( ++nlabel[cmd] );
}

ostream& operator << ( ostream& o, const vector<string>& st ) {
  cout << "[ ";
  for( vector<string>::const_iterator itr = st.begin();
       itr != st.end(); ++itr )
    cout << *itr << " "; 
       
  cout << "]";
  return o;     
}


string trata_dimensoes_decl_var( Tipo t ) {
  string aux;
  
  if (t.dim != 0)
  		aux += "[" + toString(t.dim) + "]";
           
  return aux;
}

// 'Atributo&': o '&' significa passar por referência (modifica).
void declara_variavel( Atributo& ss, 
                       vector<string> lst, 
                       Tipo tipo ) {
  ss.c = "";
  for( int i = 0; i < lst.size(); i++ ) {
    if( ts[ts.size()-1].find( lst[i] ) != ts[ts.size()-1].end() ) 
      erro( "Variável já declarada: " + lst[i] );
    else {
      ts[ts.size()-1][ lst[i] ] = tipo; 
      ss.c += tipo.decl + " " + lst[i] 
              + trata_dimensoes_decl_var( tipo ) + ";\n"; 
    }  
  }
}

void declara_variavel( Atributo& ss, string nome, Tipo tipo ) {
  vector<string> lst;
  lst.push_back( nome );
  declara_variavel( ss, lst, tipo );
}
	

void gera_codigo_atribuicao( Atributo& ss, 
                             const Atributo& s1, 
                             const Atributo& s3 ) {
  
  if( s1.t.nome == s3.t.nome &&  s1.t.nome == "palavra" ) {
    ss.c = s1.c + s3.c + "  " 
           + "strncpy( " + s1.v + ", " + s3.v + ", " + 
           toString( s1.t.dim) + " );\n";
  }else if( s1.t.nome == s3.t.nome ){
    ss.c = s1.c + s3.c + "  " + s1.v + " = " + s3.v + ";\n";
  }

}

void gera_codigo_atribuicao_funcao( Atributo& ss, 
                             const Atributo& s1, 
                             const Atributo& s3 ) {

	vector<Tipo> v = tf[s3.v];

	bool existe = false;

	for (int i = 0; i < v.size(); i++){
		if ( v[i].nome == s1.t.nome){
			existe = true;
			break;	
		}
	}

	if (!existe)
		erro("A função '"+ s3.v +"' com retorno '"+ s1.t.nome +"' não existe!");

	if( s1.t.nome == "palavra" ) {
		ss.c = s1.c + s3.c + "  " 
		       + "strncpy( " + s1.v + ", " + s3.v + ", " + 
		       toString( s1.t.dim) + " );\n";
  	}else{
    	ss.c = "  " +s1.v + " =" + s3.c + "\n";
	}
}

void busca_tipo_da_variavel( Atributo& ss, const Atributo& s1 ) {
  if( ts[ts.size()-1].find( s1.v ) == ts[ts.size()-1].end() )
        erro( "Variável não declarada: " + s1.v );
  else {
    ss.t = ts[ts.size()-1][ s1.v ];
    ss.v = s1.v;
  }
}

string par( Tipo a, Tipo b ) {
  return a.nome + "," + b.nome;  
}

void gera_codigo_operador( Atributo& ss,  const Atributo& s1, const Atributo& s2, const Atributo& s3 ) {

  if( tiporesultado.find( s2.v ) != tiporesultado.end() ) {

    if( tiporesultado[s2.v].find( par( s1.t, s3.t ) ) != tiporesultado[s2.v].end() ) {
     
      ss.t =  tiporesultado[s2.v][par( s1.t, s3.t )];
      ss.v = gera_nome_var( ss.t );
      ss.c = s1.c + s3.c + "  " + ss.v + " = " + s1.v + s2.v + s3.v + ";\n";
    }
    else
      erro( "O operador '" + s2.v + "' não está definido para os tipos " + s1.t.nome + " e " + s3.t.nome + "." );
 
  }
  else
    erro( "Operador '" + s2.v + "' não definido." );
}

void gera_codigo_operador_unario( Atributo& ss, const Atributo& op, const Atributo& value){
	
	if( (op.v == "!" && value.t.nome == "vouf") ||  
		((op.v == "+" || op.v == "-") && ( value.t.nome == "numerosemponto" 
											|| value.t.nome == "numerocomponto"
											|| value.t.nome == "numerograndecomponto" )) ){
		string temp1 = gera_nome_var(value.t);
		ss.c = value.c + "  " +  temp1 + " = " + op.v + value.v + ";\n";
		ss.v = temp1;
		ss.t = value.t;
	}
	else{
		erro("Não é possível utilizar operador unário: "+op.v+" com: "+value.t.nome);
	}

}


string declara_nvar_temp( Tipo t, int qtde ) {
  string aux = "";
   
  for( int i = 1; i <= qtde; i++ )
    aux += t.decl + " t_" + t.nome + "_" + toString( i ) + ";\n";
    
  return aux;  
}

string declara_var_temp( map< string, int >& temp ) {
  string decls = 
    declara_nvar_temp( Integer, temp[Integer.nome] ) +
    declara_nvar_temp( Float, temp[Float.nome] ) +
    declara_nvar_temp( Double, temp[Double.nome] ) +
    declara_nvar_temp( String, temp[String.nome] ) +
    declara_nvar_temp( Char, temp[Char.nome] ) +
    declara_nvar_temp( Boolean, temp[Boolean.nome] ) +
    "\n";
  
  temp.clear();
  
  return decls;
}

void gera_cmd_se(Atributo& ss, const Atributo& exp, const Atributo& cmd_ehverdade, string cmd_ehmentira){

  string lbl_ehverdade = gera_nome_label( "ehverdade" );
  string lbl_fim_se = gera_nome_label( "fim_se" );
  
  if( exp.t.nome != Boolean.nome )
    erro( "A expressão do SE deve ser booleana!" );

  ss.c = exp.c + 
         "\nif( " + exp.v + " ) goto " + lbl_ehverdade + ";\n" +
         cmd_ehmentira + "  goto " + lbl_fim_se + ";\n\n" +
         lbl_ehverdade + ":;\n" + 
         cmd_ehverdade.c + "\n" +
         lbl_fim_se + ":;\n";

}

void gera_cmd_repita(Atributo& ss, const Atributo& exp, const Atributo& cmd){

	string lbl_repita = gera_nome_label("repita");
	string lbl_fim_repita = gera_nome_label("fim_repita");

	if( exp.t.nome != Boolean.nome )
    	erro( "A expressão do REPITA deve ser booleana!" );

	ss.c = "\n" + lbl_repita + ":;\n" +
			exp.c +
          	"  "+ exp.v + "= !" + exp.v + ";\n" +
			"if ( "+ exp.v + " ) goto "+ lbl_fim_repita  + ";\n" +
			cmd.c +
			"  goto " + lbl_repita + ";\n\n"+
			lbl_fim_repita + ":;\n\n";	

}

void gera_cmd_execute_repita(Atributo& ss, const Atributo& exp, const Atributo& cmd){

	string lbl_execute = gera_nome_label("execute");

	if( exp.t.nome != Boolean.nome )
    	erro( "A expressão do EXECUTE/REPITA deve ser booleana!" );

	ss.c = "\n" + lbl_execute + ":;\n" +
			cmd.c + 
			exp.c +
			"if ( "+ exp.v + " ) goto "+ lbl_execute + ";\n";

}

void gera_cmd_com(Atributo& ss, const Atributo& atr1, const Atributo& atr2, const Atributo& exp, const Atributo& cmd){

	string lbl_fim_com = gera_nome_label("fim_com");
	string lbl_inicio_com = gera_nome_label("inicio_com");

	if( exp.t.nome != Boolean.nome )
    	erro( "A expressão do COM/FAÇA deve ser booleana!" );

	ss.c = atr1.c
			+ "\n" + lbl_inicio_com + ":;\n" +
			exp.c +
			"  "+ exp.v + "= !" + exp.v + ";\n" +
			"\nif ( "+ exp.v + " ) goto " + lbl_fim_com + ";\n"+
			cmd.c+
			atr2.c+
			"  goto " + lbl_inicio_com + ";\n\n"+
			lbl_fim_com + ":;\n\n";

}


string ajeita_parametros_funcao(string params){

	if (params=="")
		return "";

	string s;
	
	while (params.find(";") != std::string::npos){
		s = (params.replace(params.find(";"), 2, ", "));
	}

	while (params.find("*") != std::string::npos){
		s = (params.replace(params.find("*"), 5, "[256]"));
	}
	
	s = s.erase(s.size()-2);

	return s;
}


void gera_codigo_funcao_com_retorno( Atributo& ss,
						             string nome,
									 const Atributo& retorno,
									 string var_retorno,
						             string params,
						             const Atributo& vars,
						             const Atributo& codigo ) {

	string tipo_retorno = retorno.t.decl;
	string codigo_retorno = retorno.c;

	if (retorno.t.nome == String.nome){
		tipo_retorno = "char*";
		codigo_retorno = tipo_retorno + var_retorno + ";\n";
	}

	string new_params = ajeita_parametros_funcao(params);

	ss.c = tipo_retorno + " " + nome + "( " + new_params + " ) {\n  " +
			codigo_retorno +
			declara_var_temp( temp_local ) + "  " +
			vars.c +
			codigo.c +
			"  return " + var_retorno +";\n}\n\n";
}

void gera_codigo_funcao_sem_retorno( Atributo& ss,
						             string nome, 
						             string params,
						             const Atributo& vars,
						             const Atributo& codigo ){

	string new_params = ajeita_parametros_funcao(params);

	ss.c = "void " + nome + "( " + new_params + " ) {\n" +
		 declara_var_temp( temp_local ) + "  " + 
		 vars.c +
		 codigo.c +
		 "\n}\n\n";
} 


string criachamadafuncao( Atributo& ss, Atributo& s1, Atributo& s3){

	if ( tf.find( s1.v ) == tf.end() ){
		erro("Funcao " + s1.v + " não existe!");	
	}
	
	return "  " + s1.v + "(" + s3.v + ");\n";
}    

%}

%token _ID _TUDAO _USANDOISSO _EXECUTEISSO _SE _EHVERDADE _EHMENTIRA 
%token _MOSTRE _VALE _COM _FACA _ENQUANTO _REPITA _EXECUTE _LE 
%token _NUMEROSEMPONTO _PALAVRA _NUMEROCOMPONTO _NUMEROGRANDECOMPONTO _SIMBOLO _VOUF
%token _ESCOLHA _SEFOR _OK _CASOCONTRARIO
%token _FUNCAO _RECEBE _RETORNA _NADA

%token _CTE_PALAVRA _CTE_NUMEROSEMPONTO _CTE_NUMEROCOMPONTO _CTE_NUMEROGRANDECOMPONTO _CTE_SIMBOLO

%token _RESTO _SOBRE

%token _OU _E _NAO

%left _OU
%left _E
%left _NAO
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
                    "#include <string.h>\n"
                    "#include <stdio.h>\n\n" + 
                    declara_var_temp( temp_global ) +
                    $3.c + "\n" + $4.c +
                    "int main() {\n" + $5.c + "}\n";
       }
       ;

USANDOISSO  : _USANDOISSO '{' DECLS '}'  { $$.c = $3.c; }
	    	| { $$.c = ""; }
	    	;

FUNCTIONDECLS : FUNCTIONDECL FUNCTIONDECLS 
				{ $$.c = $1.c + $2.c; }
			  |
				{ $$.c = ""; }
			  ;

FUNCTIONDECL :	NOME_FUNCAO
				'(' PARAMETROS ')' _RETORNA '('_ID ':' TIPO ')'
				{ declara_variavel( $9, $7.v, $9.t ); 
					insereFuncao($1.v,$9.t); } 
				'{' USANDOISSO EXECUTEISSO'}'
				{ gera_codigo_funcao_com_retorno( $$, $1.v, $9, $7.v, $3.c, $13, $14 ); 
					escopo_local = false;
            		desempilha_tabela_de_simbolos(); }
				
			 |	NOME_FUNCAO				
				'(' PARAMETROS ')' '{' USANDOISSO EXECUTEISSO'}' 
				 { insereFuncao($1.v, Void);
					gera_codigo_funcao_sem_retorno( $$, $1.v, $3.c, $6, $7 ); 
					escopo_local = false;
					desempilha_tabela_de_simbolos(); }
			 ;

NOME_FUNCAO : _FUNCAO _ID _RECEBE 
				{ escopo_local = true; 
					empilha_nova_tabela_de_simbolos();
					$$.v = $2.v;}
			;


PARAMETROS : DECL ',' PARAMETROS 
			{ $$.c = $1.c + $3.c; }
		   | DECL
			{ $$.c = $1.c; }
		   |
			{ $$.c = ""; }
           ;
     
EXECUTEISSO : _EXECUTEISSO '{' MIOLOS '}'
		  {$$.c = $3.c; }
	    | {$$.c = "";}
	    ;

DECLS : DECL ';' DECLS { $$.c = $1.c + $3.c; }
      |	DECL ';'
      ;
     
DECL : IDS ':' TIPO  { declara_variavel( $$, $1.lst, $3.t ); }     
     ;      

IDS : IDS '&' _ID { $$.lst = $1.lst; $$.lst.push_back( $3.v ); }
    | _ID		  { $$.lst.push_back( $1.v ); }
    ; 

TIPO : _NUMEROSEMPONTO		{ $$.t = Integer; }
     | _NUMEROCOMPONTO		{ $$.t = Float; }
     | _NUMEROGRANDECOMPONTO { $$.t = Double; }
     | _PALAVRA TAM_PALAVRA	{ $$.t = $2.t; }
     | _SIMBOLO			{ $$.t = Char; }
     | _VOUF			{ $$.t = Boolean; }
     ;

TAM_PALAVRA: '[' _CTE_NUMEROSEMPONTO ']'
							{ $$.t = String; $$.t.dim = toInt( $2.v ); }
           |	{ $$.t = String; }
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
				{ $$.c = $3.c + criachamadafuncao($$, $1, $3);
					$$.v = $1.v;}
             ;


PARAM_CHAMADA: ES
				{$$ = $1; }
			 |
				{$$.v = $$.c = ""; }
	         ;

ES: E ',' ES 
	{   $$.c = $1.c + $3.c;
		$$.v = $1.v + ',' + $3.v; }
  | E
	{$$ = $1; }
  ;

CMDS: CMD CMDS 
		{ $$.c = $1.c + $2.c; }
	|
		{ $$.c = ""; }
	;

CMD : MOSTRE
    | LE
    | CMD_SE
	| CMD_ATRIB
	| CMD_FOR
	| CMD_WHILE
	| CMD_DOWHILE
	| CMD_ESCOLHA
    ; 

CMD_ESCOLHA : _ESCOLHA F '{' ESCOLHAS '}'
			 { swvar.var = $2 ; 
			   gera_switch($$);
			   nsw++;
			 }
			;

ESCOLHAS : CASOS CASOCONTRARIO
			{ swvar.casos.push_back("default");
			  swvar.cod_casos.push_back($2.c);
			}
		 ;

CASOS : CASO CASOS
	  | 
	  ;

CASO : _SEFOR F ':' MIOLOS _OK
	  	{ swvar.casos.push_back($2.v);
		  swvar.cod_casos.push_back($4.c);
		}
	 ;

CASOCONTRARIO : _CASOCONTRARIO ':' MIOLOS
				{$$.c = $3.c;}
			  ;

MOSTRE: _MOSTRE E ';' { $$.c = "  printf( \"%"+ $2.t.fmt + "\\n\", " + $2.v + " );\n"; }
      ; 

LE : _LE IDATR ';' 
	{ $$.c = "  scanf( \"%"+ $2.t.fmt + "\", &"+ $2.v + " );\n"; }
   ;

CMD_ATRIB : IDATR INDICE _VALE E ';'
			{ gera_codigo_atribuicao( $$, $1, $4); }
	  	  | IDATR INDICE _VALE CHAMADAFUNCAO
			{ gera_codigo_atribuicao_funcao($$, $1, $4); }
          ;

CMD_ATRIB_SPV : IDATR INDICE _VALE E
				{ gera_codigo_atribuicao( $$, $1, $4); }
			  ;

IDATR: _ID { busca_tipo_da_variavel( $$, $1 ); }	
	   ;
          
INDICE : '[' EXPS ']' INDICE
       |
       ;         
       
EXPS : E ',' EXPS
     | E
     ;

CMD_SE : _SE '(' E ')' _EHVERDADE '{' CMDS '}'
		{ gera_cmd_se( $$, $3, $7, ""); }
	   | _SE '(' E ')' _EHVERDADE '{' CMDS '}' _EHMENTIRA '{' CMDS '}'
		{ gera_cmd_se( $$, $3, $7, $11.c ); }
	   ;

CMD_FOR : _COM '(' CMD_ATRIB_SPV ')' _FACA '(' CMD_ATRIB_SPV ')' _ENQUANTO '(' E ')' '{' CMDS '}'
		{ gera_cmd_com( $$, $3, $7, $11, $14 ); }
        ;

CMD_WHILE : _REPITA _SE '(' E ')' '{' CMDS '}'
			{ gera_cmd_repita( $$, $4, $7 ); }
		  ;

CMD_DOWHILE : _EXECUTE '{' CMDS '}' _REPITA _SE '(' E ')' ';'
			 { gera_cmd_execute_repita( $$, $8, $3 ); }
			;

E : E '+' E { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '-' E { gera_codigo_operador( $$, $1, $2, $3 ); }     
  | E '*' E { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '/' E { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '>' E { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '<' E { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E "<=" E { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E ">=" E { gera_codigo_operador( $$, $1, $2, $3 ); }
  |	E _E E   { gera_codigo_operador( $$, $1, $2, $3); }
  | E _OU E  { gera_codigo_operador( $$, $1, $2, $3); }
  | _RESTO '(' E _SOBRE E ')' { gera_codigo_operador( $$, $3, $1, $5 ); }
  | E '=' E { gera_codigo_operador( $$, $1, $2, $3 ); }
  |	'+' E	{ gera_codigo_operador_unario( $$, $1, $2 );}
  |	'-' E	{ gera_codigo_operador_unario( $$, $1, $2 );}
  | _NAO E	{ gera_codigo_operador_unario( $$, $1, $2 );}
  | F
  ;
  
F : _CTE_PALAVRA        { $$ = $1; $$.t = String; }
  | _CTE_NUMEROSEMPONTO { $$ = $1; $$.t = Integer; }
  | _CTE_NUMEROCOMPONTO { $$ = $1; $$.t = Float; }
  | _CTE_NUMEROGRANDECOMPONTO { $$ = $1; $$.t = Double; }
  | _CTE_SIMBOLO        { $$ = $1; $$.t = Char; }
  | _ID                 { busca_tipo_da_variavel( $$, $1 ); }
  | '(' E ')'           { $$ = $2; }
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
	tiporesultado[ "%" ] = r; 
    
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

	r.clear();
	r[par(Integer, Integer)] = Boolean; 
	r[par(Float, Float)] = Boolean;    
	r[par(Float, Double)] = Boolean;    
	r[par(Double, Float)] = Boolean;    
	r[par(Double, Double)] = Boolean;    
	r[par(Char, Char)] = Boolean;      
	r[par(String, Char)] = Boolean;      
	r[par(Char, String)] = Boolean;    
	r[par(String, String)] = Boolean;    
	r[par(Boolean, Boolean)] = Boolean;    
	tiporesultado["=="] = r;
	tiporesultado["!="] = r;
	tiporesultado[">="] = r;
	tiporesultado[">"] = r;
	tiporesultado["<"] = r;
	tiporesultado["<="] = r;

	r.clear();
	r[par(Boolean, Boolean)] = Boolean;
	tiporesultado["&&"] = r;
	tiporesultado["||"] = r;

}

void inicializa_tamanho_String() {
  
	int tamanho = 256;  
  	String.dim = tamanho;
}


int main( int argc, char* argv[] )
{
	inicializa_tamanho_String();
  	inicializa_tabela_de_resultado_de_operacoes();
  	empilha_nova_tabela_de_simbolos();
  	yyparse();
}

