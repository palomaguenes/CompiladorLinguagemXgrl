#ifndef lint
static const char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20140101

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)

#define YYPREFIX "yy"

#define YYPURE 0

#line 2 "trabalho.y"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

struct Tipo {
  string nome;  /* nome na nossa linguagem*/
  string decl;  /* declaração correspondente em c-assembly*/
  string fmt;   /* formato para "printf"*/
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

/* 'Atributo&': o '&' significa passar por referência (modifica).*/
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

    

#line 385 "y.tab.c"

#ifndef YYSTYPE
typedef int YYSTYPE;
#endif

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define _ID 257
#define _TUDAO 258
#define _USANDOISSO 259
#define _EXECUTEISSO 260
#define _SE 261
#define _EHVERDADE 262
#define _EHMENTIRA 263
#define _MOSTRE 264
#define _VALE 265
#define _COM 266
#define _FACA 267
#define _ENQUANTO 268
#define _REPITA 269
#define _EXECUTE 270
#define _NUMEROSEMPONTO 271
#define _PALAVRA 272
#define _NUMEROCOMPONTO 273
#define _NUMEROGRANDECOMPONTO 274
#define _SIMBOLO 275
#define _VOUF 276
#define _ESCOLHA 277
#define _SEFOR 278
#define _OK 279
#define _CASOCONTRARIO 280
#define _FUNCAO 281
#define _RECEBE 282
#define _RETORNA 283
#define _NADA 284
#define _CTE_PALAVRA 285
#define _CTE_NUMEROSEMPONTO 286
#define _CTE_NUMEROCOMPONTO 287
#define _CTE_NUMEROGRANDECOMPONTO 288
#define _CTE_SIMBOLO 289
#define _RESTO 290
#define _SOBRE 291
#define _OU 292
#define _E 293
#define _NAO 294
#define YYERRCODE 256
static const short yylhs[] = {                           -1,
    0,    1,    2,    2,    3,    3,   10,    6,    6,    7,
    8,    8,    8,    4,    4,    5,    5,   11,   13,   13,
    9,    9,    9,    9,    9,    9,   14,   14,   12,   12,
   15,   15,   16,   18,   19,   19,   19,   21,   21,   17,
   17,   17,   17,   17,   17,   17,   28,   29,   30,   30,
   32,   31,   22,   24,   24,   36,   34,   35,   35,   37,
   37,   23,   23,   25,   26,   27,   33,   33,   33,   33,
   33,   33,   33,   33,   33,   33,   33,   33,   33,   33,
   33,   33,   20,   20,   20,   20,   20,   20,   20,
};
static const short yylen[] = {                            2,
    1,    6,    4,    0,    2,    0,    0,   15,    8,    3,
    3,    1,    0,    4,    0,    3,    2,    3,    3,    1,
    1,    1,    1,    2,    1,    1,    3,    0,    2,    0,
    1,    1,    5,    1,    3,    1,    0,    2,    0,    1,
    1,    1,    1,    1,    1,    1,    5,    2,    2,    0,
    5,    3,    3,    5,    4,    4,    1,    4,    0,    3,
    1,    8,   12,   15,    8,   10,    3,    3,    3,    3,
    3,    3,    3,    3,    3,    3,    6,    3,    2,    2,
    2,    1,    1,    1,    1,    1,    1,    1,    3,
};
static const short yydefred[] = {                         0,
    0,    0,    1,    0,    0,    0,    0,    0,    0,    0,
    0,   20,    0,    0,    0,    0,    0,    0,    5,    0,
    3,    0,    0,    0,   10,    0,    2,    0,    0,   16,
   21,    0,   22,   23,   25,   26,   18,   19,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   31,   32,   40,
   41,   42,   43,   44,   45,   46,    0,    0,    0,    0,
   24,    0,    0,   88,   83,   84,   85,   86,   87,    0,
    0,    0,    0,    0,   82,    0,    0,    0,    0,    0,
   14,   29,    0,    0,    0,    0,   11,    0,    0,   34,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   53,   57,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   27,    0,    0,    0,    0,   89,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   69,   70,    0,    0,
    0,   38,    0,    0,    0,    0,    0,    0,    0,    0,
   55,    0,    0,    0,   33,   35,    0,    0,    0,    0,
    0,    0,    0,   47,    0,   48,   49,   60,   58,   54,
    0,    9,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   77,    0,    0,    0,    0,   52,    7,    0,
    0,   65,    0,   51,    0,    0,    0,    0,    0,    0,
    0,   66,    0,    0,    0,    0,   63,    0,    8,    0,
    0,   64,
};
static const short yydgoto[] = {                          2,
    3,    6,    9,   18,   13,   10,   11,   28,   37,  195,
   14,   46,   15,   61,   47,   48,  114,   89,   90,   75,
  115,   50,   51,   52,   53,   54,   55,   56,  145,  146,
  166,  147,  117,   57,   84,  112,  118,
};
static const short yysindex[] = {                      -222,
  -72,    0,    0, -209,  -60, -206, -180, -177, -179, -206,
   48,    0,  -36,   40,  -10, -192,  -15,  -16,    0, -180,
    0, -180,  -28, -147,    0,  231,    0,   70,   68,    0,
    0,   25,    0,    0,    0,    0,    0,    0,   77,   78,
  152,   79, -133,    9,   17,   15,  231,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   43, -122, -180, -141,
    0,   17,  152,    0,    0,    0,    0,    0,    0,  111,
  152,  152,  152,  152,    0,   88, -105,  113,  241,   36,
    0,    0,  152, -100,  133, -209,    0,   92,  149,    0,
  147,   -1,  152,  180,  -13,  -13,   24,  152,  152,  152,
  152,  152,  152,  152,  152,  152,  152,  152,    0,    0,
   43,  158,  152,  241,   80,  -58,   94,  125,  164,  -51,
 -179,    0,  165,   17,  -54,  115,    0,  174,  180,  151,
  151,  151,  151,  151,  -13,  -13,    0,    0,  -39,  -34,
   31,    0,  -37,   17,  112,  -42,  -58,  152,   43,   77,
    0,  121,  181,  124,    0,    0,  130,  152,  152,  219,
  142,    7,  211,    0,  214,    0,    0,    0,    0,    0,
  -28,    0,  241,   53,  168, -105,  241,  236,  231,  231,
  238,  155,    0,  242,  157,  152,   -2,    0,    0,   34,
   16,    0,   60,    0,  176,  177,  261,  250, -209,  241,
  152,    0, -179,  187,   82,  196,    0,  205,    0,  241,
  206,    0,
};
static const short yyrindex[] = {                         0,
    0,    0,    0, -114,    0,  -82,    0,    0,  207,  -82,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  293,
    0,  210,    0,    0,    0,  212,    0,    0,  297,    0,
    0,   38,    0,    0,    0,    0,    0,    0,  -86,    0,
    0,    0,    0,    0,    0,    0, -108,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   76,    0,  293,    0,
    0,  309,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  226,    0,
    0,    0,    0,    0,    0,  -76,    0,    0,    0,    0,
  316,    0,    0,  229,  -41,  -35,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   76,    0,    0,  226,    0,   81,  267,    0,    0,    0,
  207,    0,    0,  309,    0,    0,    0,  303,  237,  266,
  270,  274,  295,  299,  -29,   -6,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   81,    0,   76,  127,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  226,    0,  323,    0,  226,    0,   86,  212,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  217,
    0,    0,    0,    0,    0,    0,    0,    0,  -76,  226,
    0,    0,  207,    0,    0,    0,    0,    0,    0,  226,
    0,    0,
};
static const short yygindex[] = {                         0,
    0,  -73,  356,  -99,  346,    0,    0,  310,  199,    0,
    3,   23,    0,    0,    0,  252,   21,    0,  248,  169,
   98,    0,    0,    0,    0,    0,    0,    0,    0,  232,
    0,    0,  441,  -70,  -66,  197,  228,
};
#define YYTABLESIZE 642
static const short yytable[] = {                         79,
   86,   79,   79,   79,   57,   80,  111,   80,   80,   80,
    4,   67,  121,   67,   67,   67,   30,   79,   79,   79,
   79,  154,   29,   80,   80,   80,   80,   24,  107,   67,
   67,   67,   67,  108,   68,    1,   68,   68,   68,  125,
  107,  105,    6,  106,  139,  108,   49,   23,    4,    5,
    4,   79,   68,   68,   68,   68,   74,   80,  101,  102,
  100,   29,    7,   67,  127,  107,  105,   49,  106,   82,
  108,  161,  107,  105,    8,  106,   12,  108,   28,   16,
   17,   28,  169,  101,  102,  100,   68,   20,   21,   25,
  101,  102,  100,  183,  107,  105,   28,  106,   22,  108,
  198,  107,  105,  206,  106,  111,  108,   26,   27,   38,
   58,   59,  101,  102,  100,   60,   62,   63,   77,  101,
  102,  100,  208,  107,  105,  203,  106,   78,  108,  107,
  105,   79,  106,   83,  108,  107,  105,  148,  106,   81,
  108,  101,  102,  100,   88,    4,  109,  101,  102,  100,
   93,  110,  113,  101,  102,  100,  107,  105,  116,  106,
   85,  108,  107,  105,  119,  106,    4,  108,   88,   88,
   30,   88,  120,   88,  101,  102,  100,    6,   57,  170,
  101,  102,  100,    4,  122,   88,   88,   88,   88,  123,
  124,   74,  107,  105,   72,  106,   73,  108,  140,   49,
   49,  187,  188,   74,  143,  153,   72,  157,   73,  107,
  105,  142,  106,   80,  108,  107,  105,  149,  106,  144,
  108,  107,  105,  155,  106,  159,  108,  101,  102,  100,
   91,  162,  160,  101,  102,  100,  164,  165,  171,  101,
  102,  100,   31,   32,   33,   34,   35,   36,  172,   79,
   79,   79,  173,   79,   79,   80,   80,   80,  176,   80,
   80,   67,   67,   67,  177,   67,   67,  178,  179,   81,
  182,  180,   81,   64,  185,  186,  194,   75,  189,  190,
   75,  192,  191,  197,   68,   68,   68,   81,   68,   68,
   98,   99,   91,  103,  104,   75,  196,  204,  199,  200,
  201,   65,   66,   67,   68,   69,   71,  211,  202,   71,
   72,  207,  163,   72,   78,   98,   99,   78,  103,  104,
  209,   81,   98,   99,   71,  103,  104,  210,   72,   75,
  212,   15,   78,   13,   17,   74,   30,   12,   74,   73,
   59,   62,   73,   76,   98,   99,   76,  103,  104,   37,
   39,   98,   99,   74,  103,  104,   36,   73,   71,   61,
   50,   76,   72,   56,   30,   19,   78,   30,   87,  181,
  151,  156,  184,   98,   99,  168,  103,  104,  167,   98,
   99,    0,  103,  104,    0,   98,   99,   74,  103,  104,
    0,   73,    0,    0,    0,   76,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  158,   98,   99,   64,  103,
  104,    0,   98,   99,    0,  103,  104,    0,   88,   88,
  150,   88,   88,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   65,   66,   67,   68,
   69,   70,    0,    0,    0,   71,    0,    0,   65,   66,
   67,   68,   69,   70,    0,    0,    0,   71,    0,   98,
   99,    0,  103,  104,    0,    0,   99,    0,  103,  104,
    0,    0,    0,   62,  103,  104,    0,   62,    0,    0,
   62,   76,   62,    0,    0,   62,   62,   39,    0,    0,
    0,   40,    0,   62,   41,   62,   42,  110,    0,   43,
   44,   40,    0,   92,   41,    0,   42,   45,    0,   43,
   44,   94,   95,   96,   97,    0,    0,   45,    0,   81,
   81,   81,    0,    0,    0,    0,    0,   75,   75,   75,
    0,    0,    0,  126,    0,    0,    0,    0,  128,  129,
  130,  131,  132,  133,  134,  135,  136,  137,  138,    0,
    0,    0,    0,  141,    0,    0,   71,   71,   71,  152,
   72,   72,   72,    0,   78,   78,   78,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   74,   74,   74,    0,   73,
   73,   73,    0,   76,   76,    0,    0,    0,  174,  175,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  193,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  205,
};
static const short yycheck[] = {                         41,
  123,   43,   44,   45,   91,   41,   77,   43,   44,   45,
  125,   41,   86,   43,   44,   45,  125,   59,   60,   61,
   62,  121,   20,   59,   60,   61,   62,   38,   42,   59,
   60,   61,   62,   47,   41,  258,   43,   44,   45,   41,
   42,   43,  125,   45,  111,   47,   26,   58,  125,  259,
  123,   93,   59,   60,   61,   62,   40,   93,   60,   61,
   62,   59,  123,   93,   41,   42,   43,   47,   45,   47,
   47,   41,   42,   43,  281,   45,  257,   47,   41,  257,
  260,   44,  149,   60,   61,   62,   93,   40,  125,  282,
   60,   61,   62,   41,   42,   43,   59,   45,   59,   47,
   41,   42,   43,  203,   45,  176,   47,  123,  125,  257,
   41,   44,   60,   61,   62,   91,   40,   40,   40,   60,
   61,   62,   41,   42,   43,  199,   45,  261,   47,   42,
   43,  123,   45,   91,   47,   42,   43,   44,   45,  125,
   47,   60,   61,   62,  286,  260,   59,   60,   61,   62,
   40,  257,   40,   60,   61,   62,   42,   43,  123,   45,
  283,   47,   42,   43,  265,   45,  281,   47,   42,   43,
  279,   45,   40,   47,   60,   61,   62,  260,  265,   59,
   60,   61,   62,  260,   93,   59,   60,   61,   62,   41,
   44,   40,   42,   43,   43,   45,   45,   47,   41,  179,
  180,  179,  180,   40,  125,  257,   43,  262,   45,   42,
   43,  114,   45,   45,   47,   42,   43,   93,   45,  278,
   47,   42,   43,   59,   45,  265,   47,   60,   61,   62,
   62,  269,  267,   60,   61,   62,  125,  280,   58,   60,
   61,   62,  271,  272,  273,  274,  275,  276,  125,  291,
  292,  293,  123,  295,  296,  291,  292,  293,   40,  295,
  296,  291,  292,  293,  123,  295,  296,  261,   58,   41,
  173,   58,   44,  257,  177,   40,  279,   41,   41,  125,
   44,  125,   41,  268,  291,  292,  293,   59,  295,  296,
  292,  293,  124,  295,  296,   59,  263,  200,  123,  123,
   40,  285,  286,  287,  288,  289,   41,  210,   59,   44,
   41,  125,  144,   44,   41,  292,  293,   44,  295,  296,
  125,   93,  292,  293,   59,  295,  296,  123,   59,   93,
  125,  125,   59,   41,  125,   41,  125,   41,   44,   41,
  265,  125,   44,   41,  292,  293,   44,  295,  296,   41,
  125,  292,  293,   59,  295,  296,   41,   59,   93,   93,
  280,   59,   93,   41,  279,   10,   93,   22,   59,  171,
  119,  124,  176,  292,  293,  148,  295,  296,  147,  292,
  293,   -1,  295,  296,   -1,  292,  293,   93,  295,  296,
   -1,   93,   -1,   -1,   -1,   93,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  291,  292,  293,  257,  295,
  296,   -1,  292,  293,   -1,  295,  296,   -1,  292,  293,
  257,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  285,  286,  287,  288,
  289,  290,   -1,   -1,   -1,  294,   -1,   -1,  285,  286,
  287,  288,  289,  290,   -1,   -1,   -1,  294,   -1,  292,
  293,   -1,  295,  296,   -1,   -1,  293,   -1,  295,  296,
   -1,   -1,   -1,  257,  295,  296,   -1,  261,   -1,   -1,
  264,   41,  266,   -1,   -1,  269,  270,  257,   -1,   -1,
   -1,  261,   -1,  277,  264,  279,  266,  257,   -1,  269,
  270,  261,   -1,   63,  264,   -1,  266,  277,   -1,  269,
  270,   71,   72,   73,   74,   -1,   -1,  277,   -1,  291,
  292,  293,   -1,   -1,   -1,   -1,   -1,  291,  292,  293,
   -1,   -1,   -1,   93,   -1,   -1,   -1,   -1,   98,   99,
  100,  101,  102,  103,  104,  105,  106,  107,  108,   -1,
   -1,   -1,   -1,  113,   -1,   -1,  291,  292,  293,  119,
  291,  292,  293,   -1,  291,  292,  293,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  291,  292,  293,   -1,  291,
  292,  293,   -1,  291,  292,   -1,   -1,   -1,  158,  159,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  186,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  201,
};
#define YYFINAL 2
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 296
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? (YYMAXTOKEN + 1) : (a))
#if YYDEBUG
static const char *yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,"'&'",0,"'('","')'","'*'","'+'","','","'-'",0,"'/'",0,0,0,0,0,0,0,0,0,0,
"':'","';'","'<'","'='","'>'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,"'['",0,"']'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
"'{'",0,"'}'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"_ID","_TUDAO","_USANDOISSO","_EXECUTEISSO",
"_SE","_EHVERDADE","_EHMENTIRA","_MOSTRE","_VALE","_COM","_FACA","_ENQUANTO",
"_REPITA","_EXECUTE","_NUMEROSEMPONTO","_PALAVRA","_NUMEROCOMPONTO",
"_NUMEROGRANDECOMPONTO","_SIMBOLO","_VOUF","_ESCOLHA","_SEFOR","_OK",
"_CASOCONTRARIO","_FUNCAO","_RECEBE","_RETORNA","_NADA","_CTE_PALAVRA",
"_CTE_NUMEROSEMPONTO","_CTE_NUMEROCOMPONTO","_CTE_NUMEROGRANDECOMPONTO",
"_CTE_SIMBOLO","_RESTO","_SOBRE","_OU","_E","_NAO","\">=\"","\"<=\"",
"illegal-symbol",
};
static const char *yyrule[] = {
"$accept : S",
"S : TUDAO",
"TUDAO : _TUDAO '{' USANDOISSO FUNCTIONDECLS EXECUTEISSO '}'",
"USANDOISSO : _USANDOISSO '{' DECLS '}'",
"USANDOISSO :",
"FUNCTIONDECLS : FUNCTIONDECL FUNCTIONDECLS",
"FUNCTIONDECLS :",
"$$1 :",
"FUNCTIONDECL : NOME_FUNCAO '(' PARAMETROS ')' _RETORNA '(' _ID ':' TIPO ')' $$1 '{' USANDOISSO EXECUTEISSO '}'",
"FUNCTIONDECL : NOME_FUNCAO '(' PARAMETROS ')' '{' USANDOISSO EXECUTEISSO '}'",
"NOME_FUNCAO : _FUNCAO _ID _RECEBE",
"PARAMETROS : DECL ',' PARAMETROS",
"PARAMETROS : DECL",
"PARAMETROS :",
"EXECUTEISSO : _EXECUTEISSO '{' MIOLOS '}'",
"EXECUTEISSO :",
"DECLS : DECL ';' DECLS",
"DECLS : DECL ';'",
"DECL : IDS ':' TIPO",
"IDS : IDS '&' _ID",
"IDS : _ID",
"TIPO : _NUMEROSEMPONTO",
"TIPO : _NUMEROCOMPONTO",
"TIPO : _NUMEROGRANDECOMPONTO",
"TIPO : _PALAVRA TAM_PALAVRA",
"TIPO : _SIMBOLO",
"TIPO : _VOUF",
"TAM_PALAVRA : '[' _CTE_NUMEROSEMPONTO ']'",
"TAM_PALAVRA :",
"MIOLOS : MIOLO MIOLOS",
"MIOLOS :",
"MIOLO : CHAMADAFUNCAO",
"MIOLO : CMD",
"CHAMADAFUNCAO : _ID '(' PARAM_CHAMADA ')' ';'",
"PARAM_CHAMADA : FS",
"FS : F ',' FS",
"FS : F",
"FS :",
"CMDS : CMD CMDS",
"CMDS :",
"CMD : MOSTRE",
"CMD : CMD_SE",
"CMD : CMD_ATRIB",
"CMD : CMD_FOR",
"CMD : CMD_WHILE",
"CMD : CMD_DOWHILE",
"CMD : CMD_ESCOLHA",
"CMD_ESCOLHA : _ESCOLHA F '{' ESCOLHAS '}'",
"ESCOLHAS : CASOS CASOCONTRARIO",
"CASOS : CASO CASOS",
"CASOS :",
"CASO : _SEFOR F ':' MIOLOS _OK",
"CASOCONTRARIO : _CASOCONTRARIO ':' MIOLOS",
"MOSTRE : _MOSTRE E ';'",
"CMD_ATRIB : IDATR INDICE _VALE E ';'",
"CMD_ATRIB : IDATR INDICE _VALE CHAMADAFUNCAO",
"CMD_ATRIB_SPV : IDATR INDICE _VALE E",
"IDATR : _ID",
"INDICE : '[' EXPS ']' INDICE",
"INDICE :",
"EXPS : E ',' EXPS",
"EXPS : E",
"CMD_SE : _SE '(' E ')' _EHVERDADE '{' CMDS '}'",
"CMD_SE : _SE '(' E ')' _EHVERDADE '{' CMDS '}' _EHMENTIRA '{' CMDS '}'",
"CMD_FOR : _COM '(' CMD_ATRIB_SPV ')' _FACA '(' CMD_ATRIB_SPV ')' _ENQUANTO '(' E ')' '{' CMDS '}'",
"CMD_WHILE : _REPITA _SE '(' E ')' '{' CMDS '}'",
"CMD_DOWHILE : _EXECUTE '{' CMDS '}' _REPITA _SE '(' E ')' ';'",
"E : E '+' E",
"E : E '-' E",
"E : E '*' E",
"E : E '/' E",
"E : E '>' E",
"E : E '<' E",
"E : E \"<=\" E",
"E : E \">=\" E",
"E : E _E E",
"E : E _OU E",
"E : _RESTO '(' E _SOBRE E ')'",
"E : E '=' E",
"E : '+' E",
"E : '-' E",
"E : _NAO E",
"E : F",
"F : _CTE_PALAVRA",
"F : _CTE_NUMEROSEMPONTO",
"F : _CTE_NUMEROCOMPONTO",
"F : _CTE_NUMEROGRANDECOMPONTO",
"F : _CTE_SIMBOLO",
"F : _ID",
"F : '(' E ')'",

};
#endif

int      yydebug;
int      yynerrs;

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#define YYINITSTACKSIZE 200

typedef struct {
    unsigned stacksize;
    short    *s_base;
    short    *s_mark;
    short    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
} YYSTACKDATA;
/* variables for the parser stack */
static YYSTACKDATA yystack;
#line 599 "trabalho.y"

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

#line 938 "y.tab.c"

#if YYDEBUG
#include <stdio.h>		/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (short *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return -1;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return -1;

    data->l_base = newvs;
    data->l_mark = newvs + i;

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab

int
YYPARSE_DECL()
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack)) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = yyname[YYTRANSLATE(yychar)];
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
        {
            goto yyoverflow;
        }
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;

    yyerror("syntax error");

    goto yyerrlab;

yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yystack.s_mark]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
                {
                    goto yyoverflow;
                }
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
                --yystack.s_mark;
                --yystack.l_mark;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = yyname[YYTRANSLATE(yychar)];
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
case 1:
#line 393 "trabalho.y"
	{ cout << yystack.l_mark[0].c << endl; }
break;
case 2:
#line 397 "trabalho.y"
	{ yyval.c = "#include <stdlib.h>\n"
                    "#include <string.h>\n"
                    "#include <stdio.h>\n\n" + 
                    declara_var_temp( temp_global ) +
                    yystack.l_mark[-3].c + "\n" + yystack.l_mark[-2].c +
                    "int main() {\n" + yystack.l_mark[-1].c + "}\n";
       }
break;
case 3:
#line 406 "trabalho.y"
	{ yyval.c = yystack.l_mark[-1].c; }
break;
case 4:
#line 407 "trabalho.y"
	{ yyval.c = ""; }
break;
case 5:
#line 411 "trabalho.y"
	{ yyval.c = yystack.l_mark[-1].c + yystack.l_mark[0].c; }
break;
case 6:
#line 413 "trabalho.y"
	{ yyval.c = ""; }
break;
case 7:
#line 418 "trabalho.y"
	{ declara_variavel( yystack.l_mark[-1], yystack.l_mark[-3].v, yystack.l_mark[-1].t ); 
					insereFuncao(yystack.l_mark[-9].v,yystack.l_mark[-1].t); }
break;
case 8:
#line 421 "trabalho.y"
	{ gera_codigo_funcao_com_retorno( yyval, yystack.l_mark[-14].v, yystack.l_mark[-6], yystack.l_mark[-8].v, yystack.l_mark[-12].c, yystack.l_mark[-2], yystack.l_mark[-1] ); 
					escopo_local = false;
            		desempilha_tabela_de_simbolos(); }
break;
case 9:
#line 427 "trabalho.y"
	{ insereFuncao(yystack.l_mark[-7].v, Void);
					gera_codigo_funcao_sem_retorno( yyval, yystack.l_mark[-7].v, yystack.l_mark[-5].c, yystack.l_mark[-2], yystack.l_mark[-1] ); 
					escopo_local = false;
					desempilha_tabela_de_simbolos(); }
break;
case 10:
#line 434 "trabalho.y"
	{ escopo_local = true; 
					empilha_nova_tabela_de_simbolos();
					yyval.v = yystack.l_mark[-1].v;}
break;
case 11:
#line 441 "trabalho.y"
	{ yyval.c = yystack.l_mark[-2].c + yystack.l_mark[0].c; }
break;
case 12:
#line 443 "trabalho.y"
	{ yyval.c = yystack.l_mark[0].c; }
break;
case 13:
#line 445 "trabalho.y"
	{ yyval.c = ""; }
break;
case 14:
#line 449 "trabalho.y"
	{yyval.c = yystack.l_mark[-1].c; }
break;
case 15:
#line 450 "trabalho.y"
	{yyval.c = "";}
break;
case 16:
#line 453 "trabalho.y"
	{ yyval.c = yystack.l_mark[-2].c + yystack.l_mark[0].c; }
break;
case 18:
#line 457 "trabalho.y"
	{ declara_variavel( yyval, yystack.l_mark[-2].lst, yystack.l_mark[0].t ); }
break;
case 19:
#line 460 "trabalho.y"
	{ yyval.lst = yystack.l_mark[-2].lst; yyval.lst.push_back( yystack.l_mark[0].v ); }
break;
case 20:
#line 461 "trabalho.y"
	{ yyval.lst.push_back( yystack.l_mark[0].v ); }
break;
case 21:
#line 464 "trabalho.y"
	{ yyval.t = Integer; }
break;
case 22:
#line 465 "trabalho.y"
	{ yyval.t = Float; }
break;
case 23:
#line 466 "trabalho.y"
	{ yyval.t = Double; }
break;
case 24:
#line 467 "trabalho.y"
	{ yyval.t = yystack.l_mark[0].t; }
break;
case 25:
#line 468 "trabalho.y"
	{ yyval.t = Char; }
break;
case 26:
#line 469 "trabalho.y"
	{ yyval.t = Boolean; }
break;
case 27:
#line 473 "trabalho.y"
	{ yyval.t = String; yyval.t.dim = toInt( yystack.l_mark[-1].v ); }
break;
case 28:
#line 474 "trabalho.y"
	{ yyval.t = String; }
break;
case 29:
#line 479 "trabalho.y"
	{ yyval.c = yystack.l_mark[-1].c + yystack.l_mark[0].c; }
break;
case 30:
#line 481 "trabalho.y"
	{ yyval.c = ""; }
break;
case 38:
#line 500 "trabalho.y"
	{ yyval.c = yystack.l_mark[-1].c + yystack.l_mark[0].c; }
break;
case 39:
#line 502 "trabalho.y"
	{ yyval.c = ""; }
break;
case 53:
#line 530 "trabalho.y"
	{ yyval.c = "  printf( \"%"+ yystack.l_mark[-1].t.fmt + "\\n\", " + yystack.l_mark[-1].v + " );\n"; }
break;
case 54:
#line 534 "trabalho.y"
	{ gera_codigo_atribuicao( yyval, yystack.l_mark[-4], yystack.l_mark[-1]); }
break;
case 56:
#line 539 "trabalho.y"
	{ gera_codigo_atribuicao( yyval, yystack.l_mark[-3], yystack.l_mark[0]); }
break;
case 57:
#line 542 "trabalho.y"
	{ busca_tipo_da_variavel( yyval, yystack.l_mark[0] ); }
break;
case 62:
#line 554 "trabalho.y"
	{ gera_cmd_se( yyval, yystack.l_mark[-5], yystack.l_mark[-1], ""); }
break;
case 63:
#line 556 "trabalho.y"
	{ gera_cmd_se( yyval, yystack.l_mark[-9], yystack.l_mark[-5], yystack.l_mark[-1].c ); }
break;
case 64:
#line 560 "trabalho.y"
	{ gera_cmd_com( yyval, yystack.l_mark[-12], yystack.l_mark[-8], yystack.l_mark[-4], yystack.l_mark[-1] ); }
break;
case 65:
#line 564 "trabalho.y"
	{ gera_cmd_repita( yyval, yystack.l_mark[-4], yystack.l_mark[-1] ); }
break;
case 66:
#line 568 "trabalho.y"
	{ gera_cmd_execute_repita( yyval, yystack.l_mark[-2], yystack.l_mark[-7] ); }
break;
case 67:
#line 571 "trabalho.y"
	{ gera_codigo_operador( yyval, yystack.l_mark[-2], yystack.l_mark[-1], yystack.l_mark[0] ); }
break;
case 68:
#line 572 "trabalho.y"
	{ gera_codigo_operador( yyval, yystack.l_mark[-2], yystack.l_mark[-1], yystack.l_mark[0] ); }
break;
case 69:
#line 573 "trabalho.y"
	{ gera_codigo_operador( yyval, yystack.l_mark[-2], yystack.l_mark[-1], yystack.l_mark[0] ); }
break;
case 70:
#line 574 "trabalho.y"
	{ gera_codigo_operador( yyval, yystack.l_mark[-2], yystack.l_mark[-1], yystack.l_mark[0] ); }
break;
case 71:
#line 575 "trabalho.y"
	{ gera_codigo_operador( yyval, yystack.l_mark[-2], yystack.l_mark[-1], yystack.l_mark[0] ); }
break;
case 72:
#line 576 "trabalho.y"
	{ gera_codigo_operador( yyval, yystack.l_mark[-2], yystack.l_mark[-1], yystack.l_mark[0] ); }
break;
case 73:
#line 577 "trabalho.y"
	{ gera_codigo_operador( yyval, yystack.l_mark[-2], yystack.l_mark[-1], yystack.l_mark[0] ); }
break;
case 74:
#line 578 "trabalho.y"
	{ gera_codigo_operador( yyval, yystack.l_mark[-2], yystack.l_mark[-1], yystack.l_mark[0] ); }
break;
case 75:
#line 579 "trabalho.y"
	{ gera_codigo_operador( yyval, yystack.l_mark[-2], yystack.l_mark[-1], yystack.l_mark[0]); }
break;
case 76:
#line 580 "trabalho.y"
	{ gera_codigo_operador( yyval, yystack.l_mark[-2], yystack.l_mark[-1], yystack.l_mark[0]); }
break;
case 77:
#line 581 "trabalho.y"
	{ gera_codigo_operador( yyval, yystack.l_mark[-3], yystack.l_mark[-5], yystack.l_mark[-1] ); }
break;
case 78:
#line 582 "trabalho.y"
	{ gera_codigo_operador( yyval, yystack.l_mark[-2], yystack.l_mark[-1], yystack.l_mark[0] ); }
break;
case 79:
#line 583 "trabalho.y"
	{ gera_codigo_operador_unario( yyval, yystack.l_mark[-1], yystack.l_mark[0] );}
break;
case 80:
#line 584 "trabalho.y"
	{ gera_codigo_operador_unario( yyval, yystack.l_mark[-1], yystack.l_mark[0] );}
break;
case 81:
#line 585 "trabalho.y"
	{ gera_codigo_operador_unario( yyval, yystack.l_mark[-1], yystack.l_mark[0] );}
break;
case 83:
#line 589 "trabalho.y"
	{ yyval = yystack.l_mark[0]; yyval.t = String; }
break;
case 84:
#line 590 "trabalho.y"
	{ yyval = yystack.l_mark[0]; yyval.t = Integer; }
break;
case 85:
#line 591 "trabalho.y"
	{ yyval = yystack.l_mark[0]; yyval.t = Float; }
break;
case 86:
#line 592 "trabalho.y"
	{ yyval = yystack.l_mark[0]; yyval.t = Double; }
break;
case 87:
#line 593 "trabalho.y"
	{ yyval = yystack.l_mark[0]; yyval.t = Char; }
break;
case 88:
#line 594 "trabalho.y"
	{ busca_tipo_da_variavel( yyval, yystack.l_mark[0] ); }
break;
case 89:
#line 595 "trabalho.y"
	{ yyval = yystack.l_mark[-1]; }
break;
#line 1402 "y.tab.c"
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
        if (yychar < 0)
        {
            if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = yyname[YYTRANSLATE(yychar)];
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yystack.s_mark, yystate);
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
    {
        goto yyoverflow;
    }
    *++yystack.s_mark = (short) yystate;
    *++yystack.l_mark = yyval;
    goto yyloop;

yyoverflow:
    yyerror("yacc stack overflow");

yyabort:
    yyfreestack(&yystack);
    return (1);

yyaccept:
    yyfreestack(&yystack);
    return (0);
}
