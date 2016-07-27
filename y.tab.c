/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "trabalho.y" /* yacc.c:339  */

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
                       Tipo tipo,
					   string dimensao) {
  ss.c = "";
  for( int i = 0; i < lst.size(); i++ ) {
    if( ts[ts.size()-1].find( lst[i] ) != ts[ts.size()-1].end() ) 
      erro( "Variável já declarada: " + lst[i] );
    else {
      ts[ts.size()-1][ lst[i] ] = tipo; 
      ss.c += tipo.decl + " " + lst[i] 
              + trata_dimensoes_decl_var( tipo ) + dimensao + ";\n"; 
    }  
  }
}

void declara_variavel( Atributo& ss, string nome, Tipo tipo ) {
  vector<string> lst;
  lst.push_back( nome );
  declara_variavel( ss, lst, tipo, "" );
}
	

void gera_codigo_atribuicao( Atributo& ss, 
                             const Atributo& s1,
							 const Atributo& s2,
                             const Atributo& s3 ) {
  
  if( s1.t.nome == s3.t.nome &&  s1.t.nome == "palavra" ) {
    ss.c = s1.c + s3.c + "  " 
           + "strncpy( " + s1.v + ", " + s3.v + ", " + 
           toString( s1.t.dim) + " );\n";
  }else if( s1.t.nome == s3.t.nome ){
    ss.c = s1.c + s3.c + "  " + s1.v + s2.c + " = " + s3.v + ";\n";
  }

}

void gera_codigo_atribuicao_funcao( Atributo& ss, 
                             const Atributo& s1,
						     const Atributo& s2, 
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
		ss.c = s1.c + s2.c + s3.c + "  " 
		       + "strncpy( " + s1.v + ", " + s3.v + ", " + 
		       toString( s1.t.dim) + " );\n";
  	}else{
    	ss.c = "  " + s1.v + s2.c + " =" + s3.c + "\n";
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

void gera_labels(std::vector<string>* label, string tipo){

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

void gera_cmd_switch(Atributo& ss){

	if (swvar.var.t.nome != "numerosemponto" && swvar.var.t.nome != "simbolo"){
		erro("ESCOLHA recebe numerosemponto e simbolo com parâmetros!\n");
	}
	
	vector<string> labels; 
	gera_labels(&labels, swvar.var.t.nome);
	
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

void checa_tipo_exp(string tipo1, string tipo2){

	if (tipo1 != Integer.nome && tipo2 != Integer.nome)
		erro("Indices devem ser do tipo numerocomponto");
}


#line 534 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    _ID = 258,
    _TUDAO = 259,
    _USANDOISSO = 260,
    _EXECUTEISSO = 261,
    _SE = 262,
    _EHVERDADE = 263,
    _EHMENTIRA = 264,
    _MOSTRE = 265,
    _VALE = 266,
    _COM = 267,
    _FACA = 268,
    _ENQUANTO = 269,
    _REPITA = 270,
    _EXECUTE = 271,
    _LE = 272,
    _NUMEROSEMPONTO = 273,
    _PALAVRA = 274,
    _NUMEROCOMPONTO = 275,
    _NUMEROGRANDECOMPONTO = 276,
    _SIMBOLO = 277,
    _VOUF = 278,
    _ESCOLHA = 279,
    _SEFOR = 280,
    _OK = 281,
    _CASOCONTRARIO = 282,
    _FUNCAO = 283,
    _RECEBE = 284,
    _RETORNA = 285,
    _NADA = 286,
    _CTE_PALAVRA = 287,
    _CTE_NUMEROSEMPONTO = 288,
    _CTE_NUMEROCOMPONTO = 289,
    _CTE_NUMEROGRANDECOMPONTO = 290,
    _CTE_SIMBOLO = 291,
    _RESTO = 292,
    _SOBRE = 293,
    _X = 294,
    _OU = 295,
    _E = 296,
    _NAO = 297
  };
#endif
/* Tokens.  */
#define _ID 258
#define _TUDAO 259
#define _USANDOISSO 260
#define _EXECUTEISSO 261
#define _SE 262
#define _EHVERDADE 263
#define _EHMENTIRA 264
#define _MOSTRE 265
#define _VALE 266
#define _COM 267
#define _FACA 268
#define _ENQUANTO 269
#define _REPITA 270
#define _EXECUTE 271
#define _LE 272
#define _NUMEROSEMPONTO 273
#define _PALAVRA 274
#define _NUMEROCOMPONTO 275
#define _NUMEROGRANDECOMPONTO 276
#define _SIMBOLO 277
#define _VOUF 278
#define _ESCOLHA 279
#define _SEFOR 280
#define _OK 281
#define _CASOCONTRARIO 282
#define _FUNCAO 283
#define _RECEBE 284
#define _RETORNA 285
#define _NADA 286
#define _CTE_PALAVRA 287
#define _CTE_NUMEROSEMPONTO 288
#define _CTE_NUMEROCOMPONTO 289
#define _CTE_NUMEROGRANDECOMPONTO 290
#define _CTE_SIMBOLO 291
#define _RESTO 292
#define _SOBRE 293
#define _X 294
#define _OU 295
#define _E 296
#define _NAO 297

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);



/* Copy the second part of user declarations.  */

#line 666 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   398

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  62
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  40
/* YYNRULES -- Number of rules.  */
#define YYNRULES  94
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  221

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   299

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    59,     2,
      54,    55,    50,    48,    57,    49,     2,    51,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    56,    58,
      44,    45,    43,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    60,     2,    61,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    52,     2,    53,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    46,    47
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   493,   493,   496,   506,   507,   510,   513,   518,   516,
     525,   533,   540,   542,   545,   548,   550,   553,   554,   557,
     560,   562,   565,   567,   568,   571,   572,   573,   574,   575,
     576,   579,   581,   585,   588,   591,   592,   595,   601,   604,
     607,   610,   614,   617,   620,   621,   622,   623,   624,   625,
     626,   627,   630,   639,   645,   646,   649,   655,   659,   662,
     666,   668,   672,   676,   679,   682,   686,   689,   691,   695,
     699,   703,   707,   708,   709,   710,   711,   712,   713,   714,
     715,   716,   717,   718,   719,   720,   721,   722,   725,   726,
     727,   728,   729,   730,   731
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "_ID", "_TUDAO", "_USANDOISSO",
  "_EXECUTEISSO", "_SE", "_EHVERDADE", "_EHMENTIRA", "_MOSTRE", "_VALE",
  "_COM", "_FACA", "_ENQUANTO", "_REPITA", "_EXECUTE", "_LE",
  "_NUMEROSEMPONTO", "_PALAVRA", "_NUMEROCOMPONTO",
  "_NUMEROGRANDECOMPONTO", "_SIMBOLO", "_VOUF", "_ESCOLHA", "_SEFOR",
  "_OK", "_CASOCONTRARIO", "_FUNCAO", "_RECEBE", "_RETORNA", "_NADA",
  "_CTE_PALAVRA", "_CTE_NUMEROSEMPONTO", "_CTE_NUMEROCOMPONTO",
  "_CTE_NUMEROGRANDECOMPONTO", "_CTE_SIMBOLO", "_RESTO", "_SOBRE", "_X",
  "_OU", "_E", "_NAO", "'>'", "'<'", "'='", "\">=\"", "\"<=\"", "'+'",
  "'-'", "'*'", "'/'", "'{'", "'}'", "'('", "')'", "':'", "','", "';'",
  "'&'", "'['", "']'", "$accept", "S", "TUDAO", "USANDOISSO",
  "FUNCTIONDECLS", "FUNCTIONDECL", "$@1", "NOME_FUNCAO", "PARAMETROS",
  "EXECUTEISSO", "DECLS", "DECL", "TAMANHOARRAY", "IDS", "TIPO",
  "TAM_PALAVRA", "MIOLOS", "MIOLO", "CHAMADAFUNCAO", "PARAM_CHAMADA", "ES",
  "CMDS", "CMD", "CMD_ESCOLHA", "ESCOLHAS", "CASOS", "CASO",
  "CASOCONTRARIO", "MOSTRE", "LE", "CMD_ATRIB", "CMD_ATRIB_SPV", "IDATR",
  "INDICE", "CMD_SE", "CMD_FOR", "CMD_WHILE", "CMD_DOWHILE", "E", "F", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,    62,    60,    61,   298,   299,    43,    45,
      42,    47,   123,   125,    40,    41,    58,    44,    59,    38,
      91,    93
};
# endif

#define YYPACT_NINF -123

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-123)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-1)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      20,   -25,    28,  -123,    25,  -123,   -21,     4,    30,    35,
      33,     4,   -14,  -123,    -3,    -7,   -37,    23,     1,     7,
    -123,    30,  -123,    30,    24,    59,  -123,   194,  -123,     8,
      26,  -123,  -123,    17,  -123,  -123,  -123,  -123,    47,  -123,
      27,    31,    58,    32,    75,    36,    84,    22,    43,   194,
    -123,  -123,  -123,  -123,  -123,  -123,    38,  -123,  -123,  -123,
    -123,   -23,    30,    66,  -123,    62,  -123,    58,    58,  -123,
    -123,  -123,  -123,  -123,  -123,    48,    58,    58,    58,    58,
     187,  -123,    84,    49,   205,  -123,    50,    53,  -123,  -123,
      58,    99,    57,    25,  -123,    52,    82,    61,  -123,   219,
     234,    58,   -34,   -30,   -30,   247,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,  -123,    63,    38,
      58,    67,   205,  -123,    96,   125,    94,   116,    33,  -123,
    -123,    64,    58,   124,   315,  -123,   338,   -34,   347,   347,
     347,   347,   347,   -30,   -30,  -123,  -123,   111,   123,   260,
     120,  -123,    22,    88,   110,    96,    85,    27,  -123,   203,
      90,    91,  -123,  -123,    95,    58,    98,    58,    97,   143,
     100,  -123,   101,  -123,  -123,    58,  -123,    24,  -123,   205,
     273,    84,   327,   205,   104,   194,   194,   144,   106,   102,
    -123,   107,   126,    58,   127,  -123,  -123,  -123,   145,   149,
    -123,   286,  -123,   112,   128,   129,   119,    25,   205,    58,
    -123,    33,   146,   299,   147,  -123,   130,  -123,   205,   150,
    -123
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     2,     5,     1,     0,     7,     0,     0,
      16,     7,     0,    24,     0,     0,     0,     0,     0,     0,
       6,    14,     4,    18,     0,     0,    11,    34,     3,     0,
      13,    17,    25,    32,    26,    27,    29,    30,    22,    23,
      63,     0,     0,     0,     0,     0,     0,     0,     0,    34,
      35,    36,    51,    44,    45,    47,    66,    46,    48,    49,
      50,     0,    14,     0,    28,    21,    19,    39,     0,    93,
      88,    89,    90,    91,    92,     0,     0,     0,     0,     0,
       0,    87,     0,     0,    43,    63,     0,     0,    15,    33,
       0,     0,     0,     5,    12,     0,     0,     0,    38,    41,
       0,     0,    86,    84,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,    66,
       0,     0,    43,    59,    55,     0,     0,     0,    16,    31,
      20,     0,     0,     0,     0,    94,    81,    80,    76,    77,
      83,    79,    78,    72,    73,    74,    75,     0,     0,     0,
       0,    42,     0,     0,     0,    55,    65,    93,    61,     0,
       0,     0,    37,    40,     0,     0,     0,     0,     0,     0,
       0,    52,     0,    53,    54,     0,    60,     0,    10,    43,
       0,     0,    62,    43,     0,    34,    34,     0,     0,     0,
      82,     0,     0,     0,     0,    57,    64,     8,    67,     0,
      70,     0,    56,     0,     0,     0,     0,     5,    43,     0,
      71,    16,     0,     0,     0,    68,     0,     9,    43,     0,
      69
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -123,  -123,  -123,   -90,   167,  -123,  -123,  -123,   134,  -122,
     158,   -13,  -123,  -123,    21,  -123,   -47,  -123,    76,  -123,
      81,  -104,   -26,  -123,  -123,    68,  -123,  -123,  -123,  -123,
    -123,    44,   -41,   105,  -123,  -123,  -123,  -123,   -42,   -43
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,     7,    10,    11,   203,    12,    29,    19,
      14,    15,    66,    16,    38,    64,    48,    49,    50,    97,
      98,   121,   122,    52,   153,   154,   155,   173,    53,    54,
      55,   118,    56,    91,    57,    58,    59,    60,    99,    81
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      80,    51,    89,   128,    87,    86,   161,    92,    30,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   151,    24,
     115,   116,    25,    51,     1,    69,   100,     4,     5,    93,
       6,     8,     9,    13,   102,   103,   104,   105,    17,    18,
      21,   119,    32,    33,    34,    35,    36,    37,   125,    30,
      22,    23,    26,    27,    70,    71,    72,    73,    74,   134,
      28,    69,    39,    61,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   189,    79,    63,   149,   192,
      65,    67,    83,    62,   159,    68,    82,    85,    84,   214,
      70,    71,    72,    73,    74,    75,    88,   157,    90,    95,
      76,    96,   101,   120,   212,   124,    77,    78,   123,   170,
     126,   127,    79,   129,   219,   130,   131,   211,   147,   160,
     150,   152,   162,   180,   166,   182,    70,    71,    72,    73,
      74,    75,   164,   187,   167,   169,    76,   172,   194,   195,
     119,   171,    77,    78,   178,   175,   177,   179,    79,   183,
     184,   201,   181,   202,   204,   198,   185,   186,   193,    51,
      51,   197,   199,   205,   207,   106,   107,   213,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   210,    20,   200,
     208,    31,   218,   209,   106,   107,   156,   108,   109,   110,
     111,   112,   113,   114,   115,   116,    94,    40,   188,   215,
     217,    41,   158,   220,    42,   196,    43,     0,    85,    44,
      45,    46,    41,   163,     0,    42,     0,    43,    47,     0,
      44,    45,    46,   174,   148,   191,     0,   106,   107,    47,
     108,   109,   110,   111,   112,   113,   114,   115,   116,     0,
       0,     0,     0,   106,   107,   117,   108,   109,   110,   111,
     112,   113,   114,   115,   116,     0,     0,     0,     0,   106,
     107,   176,   108,   109,   110,   111,   112,   113,   114,   115,
     116,     0,     0,     0,   106,   107,   132,   108,   109,   110,
     111,   112,   113,   114,   115,   116,     0,   106,   107,   133,
     108,   109,   110,   111,   112,   113,   114,   115,   116,     0,
     106,   107,   135,   108,   109,   110,   111,   112,   113,   114,
     115,   116,     0,   106,   107,   168,   108,   109,   110,   111,
     112,   113,   114,   115,   116,     0,   106,   107,   190,   108,
     109,   110,   111,   112,   113,   114,   115,   116,     0,   106,
     107,   206,   108,   109,   110,   111,   112,   113,   114,   115,
     116,     0,     0,   165,   216,   106,   107,     0,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   106,   107,     0,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   107,
       0,   108,   109,   110,   111,   112,   113,   114,   115,   116,
      -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116
};

static const yytype_int16 yycheck[] =
{
      42,    27,    49,    93,    47,    46,   128,    30,    21,    43,
      44,    45,    46,    47,    48,    49,    50,    51,   122,    56,
      50,    51,    59,    49,     4,     3,    68,    52,     0,    52,
       5,    52,    28,     3,    76,    77,    78,    79,     3,     6,
      54,    82,    18,    19,    20,    21,    22,    23,    90,    62,
      53,    58,    29,    52,    32,    33,    34,    35,    36,   101,
      53,     3,     3,    55,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   179,    54,    60,   120,   183,
      33,    54,     7,    57,   126,    54,    54,     3,    52,   211,
      32,    33,    34,    35,    36,    37,    53,     3,    60,    33,
      42,    39,    54,    54,   208,    52,    48,    49,    58,   152,
      11,    54,    54,    61,   218,    33,    55,   207,    55,     3,
      53,    25,    58,   165,    13,   167,    32,    33,    34,    35,
      36,    37,     8,   175,    11,    15,    42,    27,   185,   186,
     181,    53,    48,    49,    53,    60,    56,    52,    54,    52,
       7,   193,    54,    26,     9,    53,    56,    56,    54,   185,
     186,    55,    55,    14,    52,    40,    41,   209,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    58,    11,    53,
      52,    23,    52,    54,    40,    41,    61,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    62,     3,   177,    53,
      53,     7,   126,    53,    10,    61,    12,    -1,     3,    15,
      16,    17,     7,   132,    -1,    10,    -1,    12,    24,    -1,
      15,    16,    17,   155,   119,   181,    -1,    40,    41,    24,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    -1,
      -1,    -1,    -1,    40,    41,    58,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    -1,    -1,    -1,    -1,    40,
      41,    58,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    -1,    -1,    -1,    40,    41,    57,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    -1,    40,    41,    55,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    -1,
      40,    41,    55,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    -1,    40,    41,    55,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    -1,    40,    41,    55,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    -1,    40,
      41,    55,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    -1,    -1,    38,    55,    40,    41,    -1,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    40,    41,    -1,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    41,
      -1,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      43,    44,    45,    46,    47,    48,    49,    50,    51
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    63,    64,    52,     0,     5,    65,    52,    28,
      66,    67,    69,     3,    72,    73,    75,     3,     6,    71,
      66,    54,    53,    58,    56,    59,    29,    52,    53,    70,
      73,    72,    18,    19,    20,    21,    22,    23,    76,     3,
       3,     7,    10,    12,    15,    16,    17,    24,    78,    79,
      80,    84,    85,    90,    91,    92,    94,    96,    97,    98,
      99,    55,    57,    60,    77,    33,    74,    54,    54,     3,
      32,    33,    34,    35,    36,    37,    42,    48,    49,    54,
     100,   101,    54,     7,    52,     3,    94,   101,    53,    78,
      60,    95,    30,    52,    70,    33,    39,    81,    82,   100,
     100,    54,   100,   100,   100,   100,    40,    41,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    58,    93,    94,
      54,    83,    84,    58,    52,   100,    11,    54,    65,    61,
      33,    55,    57,    55,   100,    55,   100,   100,   100,   100,
     100,   100,   100,   100,   100,   100,   100,    55,    95,   100,
      53,    83,    25,    86,    87,    88,    61,     3,    80,   100,
       3,    71,    58,    82,     8,    38,    13,    11,    55,    15,
     101,    53,    27,    89,    87,    60,    58,    56,    53,    52,
     100,    54,   100,    52,     7,    56,    56,   100,    76,    83,
      55,    93,    83,    54,    78,    78,    61,    55,    53,    55,
      53,   100,    26,    68,     9,    14,    55,    52,    52,    54,
      58,    65,    83,   100,    71,    53,    55,    53,    52,    83,
      53
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    62,    63,    64,    65,    65,    66,    66,    68,    67,
      67,    69,    70,    70,    70,    71,    71,    72,    72,    73,
      74,    74,    74,    75,    75,    76,    76,    76,    76,    76,
      76,    77,    77,    78,    78,    79,    79,    80,    81,    81,
      82,    82,    83,    83,    84,    84,    84,    84,    84,    84,
      84,    84,    85,    86,    87,    87,    88,    89,    90,    91,
      92,    92,    93,    94,    95,    95,    95,    96,    96,    97,
      98,    99,   100,   100,   100,   100,   100,   100,   100,   100,
     100,   100,   100,   100,   100,   100,   100,   100,   101,   101,
     101,   101,   101,   101,   101
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     6,     4,     0,     2,     0,     0,    15,
       8,     3,     3,     1,     0,     4,     0,     3,     2,     4,
       3,     1,     0,     3,     1,     1,     1,     1,     2,     1,
       1,     3,     0,     2,     0,     1,     1,     5,     1,     0,
       3,     1,     2,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     5,     2,     2,     0,     5,     3,     3,     3,
       5,     4,     4,     1,     6,     3,     0,     8,    12,    15,
       8,    10,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     6,     3,     2,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 493 "trabalho.y" /* yacc.c:1646  */
    { cout << (yyvsp[0]).c << endl; }
#line 1933 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 497 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = "#include <stdlib.h>\n"
                    "#include <string.h>\n"
                    "#include <stdio.h>\n\n" + 
                    declara_var_temp( temp_global ) +
                    (yyvsp[-3]).c + "\n" + (yyvsp[-2]).c +
                    "int main() {\n" + (yyvsp[-1]).c + "}\n";
       }
#line 1945 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 506 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[-1]).c; }
#line 1951 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 507 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = ""; }
#line 1957 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 511 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[-1]).c + (yyvsp[0]).c; }
#line 1963 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 513 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = ""; }
#line 1969 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 518 "trabalho.y" /* yacc.c:1646  */
    { declara_variavel( (yyvsp[-1]), (yyvsp[-3]).v, (yyvsp[-1]).t ); 
					insereFuncao((yyvsp[-9]).v,(yyvsp[-1]).t); }
#line 1976 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 521 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_funcao_com_retorno( (yyval), (yyvsp[-14]).v, (yyvsp[-6]), (yyvsp[-8]).v, (yyvsp[-12]).c, (yyvsp[-2]), (yyvsp[-1]) ); 
					escopo_local = false;
            		desempilha_tabela_de_simbolos(); }
#line 1984 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 527 "trabalho.y" /* yacc.c:1646  */
    { insereFuncao((yyvsp[-7]).v, Void);
					gera_codigo_funcao_sem_retorno( (yyval), (yyvsp[-7]).v, (yyvsp[-5]).c, (yyvsp[-2]), (yyvsp[-1]) ); 
					escopo_local = false;
					desempilha_tabela_de_simbolos(); }
#line 1993 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 534 "trabalho.y" /* yacc.c:1646  */
    { escopo_local = true; 
					empilha_nova_tabela_de_simbolos();
					(yyval).v = (yyvsp[-1]).v;}
#line 2001 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 541 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[-2]).c + (yyvsp[0]).c; }
#line 2007 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 543 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[0]).c; }
#line 2013 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 545 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = ""; }
#line 2019 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 549 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c = (yyvsp[-1]).c; }
#line 2025 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 550 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c = "";}
#line 2031 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 553 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[-2]).c + (yyvsp[0]).c; }
#line 2037 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 557 "trabalho.y" /* yacc.c:1646  */
    { declara_variavel( (yyval), (yyvsp[-3]).lst, (yyvsp[-1]).t, (yyvsp[0]).c ); }
#line 2043 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 561 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = '[' + toString(toInt((yyvsp[-2]).v) * toInt((yyvsp[0]).v)) + ']' ; }
#line 2049 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 563 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = '[' + (yyvsp[0]).v + ']'; }
#line 2055 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 565 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = ""; }
#line 2061 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 567 "trabalho.y" /* yacc.c:1646  */
    { (yyval).lst = (yyvsp[-2]).lst; (yyval).lst.push_back( (yyvsp[0]).v ); }
#line 2067 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 568 "trabalho.y" /* yacc.c:1646  */
    { (yyval).lst.push_back( (yyvsp[0]).v ); }
#line 2073 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 571 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = Integer; }
#line 2079 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 572 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = Float; }
#line 2085 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 573 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = Double; }
#line 2091 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 574 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = (yyvsp[0]).t; }
#line 2097 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 575 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = Char; }
#line 2103 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 576 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = Boolean; }
#line 2109 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 580 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = String; (yyval).t.dim = toInt( (yyvsp[-1]).v ); }
#line 2115 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 581 "trabalho.y" /* yacc.c:1646  */
    { (yyval).t = String; }
#line 2121 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 586 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[-1]).c + (yyvsp[0]).c; }
#line 2127 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 588 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = ""; }
#line 2133 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 596 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[-2]).c + criachamadafuncao((yyval), (yyvsp[-4]), (yyvsp[-2]));
					(yyval).v = (yyvsp[-4]).v;}
#line 2140 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 602 "trabalho.y" /* yacc.c:1646  */
    {(yyval) = (yyvsp[0]); }
#line 2146 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 604 "trabalho.y" /* yacc.c:1646  */
    {(yyval).v = (yyval).c = ""; }
#line 2152 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 608 "trabalho.y" /* yacc.c:1646  */
    {   (yyval).c = (yyvsp[-2]).c + (yyvsp[0]).c;
		(yyval).v = (yyvsp[-2]).v + ',' + (yyvsp[0]).v; }
#line 2159 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 611 "trabalho.y" /* yacc.c:1646  */
    {(yyval) = (yyvsp[0]); }
#line 2165 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 615 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = (yyvsp[-1]).c + (yyvsp[0]).c; }
#line 2171 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 617 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = ""; }
#line 2177 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 631 "trabalho.y" /* yacc.c:1646  */
    { swvar.var = (yyvsp[-3]) ; 
			   gera_cmd_switch((yyval));
			   nsw++;
			   Sw clear_sw;
			   swvar = clear_sw;
			 }
#line 2188 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 640 "trabalho.y" /* yacc.c:1646  */
    { swvar.casos.push_back("default");
			  swvar.cod_casos.push_back((yyvsp[0]).c);
			}
#line 2196 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 650 "trabalho.y" /* yacc.c:1646  */
    { swvar.casos.push_back((yyvsp[-3]).v);
		  swvar.cod_casos.push_back((yyvsp[-1]).c);
		}
#line 2204 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 656 "trabalho.y" /* yacc.c:1646  */
    {(yyval).c = (yyvsp[0]).c;}
#line 2210 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 659 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = "  printf( \"%"+ (yyvsp[-1]).t.fmt + "\\n\", " + (yyvsp[-1]).v + " );\n"; }
#line 2216 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 663 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = "  scanf( \"%"+ (yyvsp[-1]).t.fmt + "\", &"+ (yyvsp[-1]).v + " );\n"; }
#line 2222 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 667 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_atribuicao( (yyval), (yyvsp[-4]), (yyvsp[-3]), (yyvsp[-1])); }
#line 2228 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 669 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_atribuicao_funcao((yyval), (yyvsp[-3]), (yyvsp[-2]), (yyvsp[0])); }
#line 2234 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 673 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_atribuicao( (yyval), (yyvsp[-3]), (yyvsp[-2]), (yyvsp[0])); }
#line 2240 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 676 "trabalho.y" /* yacc.c:1646  */
    { busca_tipo_da_variavel( (yyval), (yyvsp[0]) ); }
#line 2246 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 680 "trabalho.y" /* yacc.c:1646  */
    { checa_tipo_exp((yyvsp[-5]).t.nome , (yyvsp[-4]).t.nome); 
		  (yyval).c = "[" + toString( toInt((yyvsp[-4]).v) * toInt((yyvsp[-1]).v) ) + "]" ;}
#line 2253 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 683 "trabalho.y" /* yacc.c:1646  */
    { checa_tipo_exp((yyvsp[-2]).t.nome, Integer.nome); 
			(yyval).c = "[" + (yyvsp[-1]).v + "]";}
#line 2260 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 686 "trabalho.y" /* yacc.c:1646  */
    { (yyval).c = ""; }
#line 2266 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 690 "trabalho.y" /* yacc.c:1646  */
    { gera_cmd_se( (yyval), (yyvsp[-5]), (yyvsp[-1]), ""); }
#line 2272 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 692 "trabalho.y" /* yacc.c:1646  */
    { gera_cmd_se( (yyval), (yyvsp[-9]), (yyvsp[-5]), (yyvsp[-1]).c ); }
#line 2278 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 696 "trabalho.y" /* yacc.c:1646  */
    { gera_cmd_com( (yyval), (yyvsp[-12]), (yyvsp[-8]), (yyvsp[-4]), (yyvsp[-1]) ); }
#line 2284 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 700 "trabalho.y" /* yacc.c:1646  */
    { gera_cmd_repita( (yyval), (yyvsp[-4]), (yyvsp[-1]) ); }
#line 2290 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 704 "trabalho.y" /* yacc.c:1646  */
    { gera_cmd_execute_repita( (yyval), (yyvsp[-2]), (yyvsp[-7]) ); }
#line 2296 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 707 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2302 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 708 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2308 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 709 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2314 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 710 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2320 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 711 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2326 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 712 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2332 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 713 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2338 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 714 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2344 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 715 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0])); }
#line 2350 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 716 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0])); }
#line 2356 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 717 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-3]), (yyvsp[-5]), (yyvsp[-1]) ); }
#line 2362 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 718 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador( (yyval), (yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]) ); }
#line 2368 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 719 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador_unario( (yyval), (yyvsp[-1]), (yyvsp[0]) );}
#line 2374 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 720 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador_unario( (yyval), (yyvsp[-1]), (yyvsp[0]) );}
#line 2380 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 721 "trabalho.y" /* yacc.c:1646  */
    { gera_codigo_operador_unario( (yyval), (yyvsp[-1]), (yyvsp[0]) );}
#line 2386 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 725 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); (yyval).t = String; }
#line 2392 "y.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 726 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); (yyval).t = Integer; }
#line 2398 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 727 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); (yyval).t = Float; }
#line 2404 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 728 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); (yyval).t = Double; }
#line 2410 "y.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 729 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); (yyval).t = Char; }
#line 2416 "y.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 730 "trabalho.y" /* yacc.c:1646  */
    { busca_tipo_da_variavel( (yyval), (yyvsp[0]) ); }
#line 2422 "y.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 731 "trabalho.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-1]); }
#line 2428 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2432 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 734 "trabalho.y" /* yacc.c:1906  */


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

