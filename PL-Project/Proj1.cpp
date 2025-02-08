#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <stdio.h>

#define DEBUG true
#define DEBUG_Print false

using namespace std;

int gLine = 1 ;
int gColumn = 0 ;
int gLevel = 1 ;
int M = 0 ;
bool errorMsg = false ; // if 已經印了 Error Msg => true 

enum tokenType {
  LEFT_PAREN = 0,    // '('
  RIGHT_PAREN = 1,   // ')'
  INT = 2,           // '123', '+123', '-123'
  STRING = 3,        // "Hello, world!"
  DOT = 4,           // '.'
  FLOAT = 5,         // '123.567', '123.', '.567', '+123.4', '-.123'
  NIL = 6,           // 'nil', or '#f'
  T = 7,             // 't' or '#t'
  QUOTE = 8,         // '
  SYMBOL = 9
};

void DP( bool printIt, string str ) {
  if ( printIt )
    cout << str << endl;
} // Debug Print

bool isWhiteSpace(char temp) ;

char getAChar() ;

void SkipWhiteSpace( char & temp ) ;

bool isSeparator( char temp ) ;

bool isInt( string str ) ;

bool isFloat( string str ) ;

class Token {     // 紀錄token資訊
  public:
    // int state ;   // 此token的層數(for proj1 output)
    int type ;    // token type (int, symbol...)
    string name ; // token name
    Token *left ;
    Token *right ;

  void whatKindOfToken( string str, vector<Token> & expr ) {
    Token * t = new Token() ;
    expr.push_back( *t );
    expr[expr.size()-1].name = str ;
    if ( str == "(" ) expr[expr.size()-1].type = LEFT_PAREN ;
    else if ( str == ")" ) expr[expr.size()-1].type = RIGHT_PAREN ;
    else if ( str == "." ) expr[expr.size()-1].type = DOT ;
    else if ( str == "nil" || str == "#f" || str == "()" ) expr[expr.size()-1].type = NIL ;
    else if ( str == "t" || str == "#t" ) expr[expr.size()-1].type = T ;
    else if ( str == "'" ) expr[expr.size()-1].type = QUOTE ;
    else if ( isInt( str ) ) expr[expr.size()-1].type = INT ;
    else if ( isFloat( str ) ) expr[expr.size()-1].type = FLOAT ;
    else if ( str[0] == '"' && str[str.size()-1] == '"' ) expr[expr.size()-1].type = STRING ;
    else if ( str[0] == '"' && str[str.size()-1] != '"' ) 
      cout << "ERROR (no closing quote) : END-OF-LINE encountered at line " << gLine << ", column " << gColumn << endl ;
    else expr[expr.size()-1].type = SYMBOL ;
  } // whatKindOfToken

  vector<Token> getToken( char & remainCh) { // Scanner
    int parenCount = 0;
    char temp = '\0', chnext = '\0';
    string str ;
    vector<Token> expr ;
    // temp = getchar() ;
    if ( remainCh == '\0' ) SkipWhiteSpace( temp ) ;
    else {
      temp = remainCh ;
      remainCh = '\0' ;
    } // else

    while ( temp != EOF ) {
      while ( isSeparator(temp) == false && temp != EOF ) {  // if read separator, stop and check  
        str = str + temp;
        temp = getAChar() ;
      } // while temp != separator, EOF

      if ( isWhiteSpace(temp) ) {  // 0 or 1 token
        if (str != "") {  // 1 token 
          whatKindOfToken( str, expr );
        } // if
      } // if temp = WS
      else if ( temp == ';' ) {
        /* comment */
        while ( temp != '\n' && temp != EOF) 
          temp = getAChar() ;
      } // if temp = ;
      else if ( temp == '"' ) { 
        /* string */
        str = str + temp ;
        chnext = cin.peek() ;
        while ( chnext != '"' && chnext != EOF) {
          temp = getAChar() ;      
          str = str + temp ;
          if ( temp == '\\' ) {
            chnext = cin.peek() ;
            if ( chnext == '"' || chnext == '\\' ) {
              temp = getAChar() ;
              str = str + temp ;
            } // if
          } // if
          chnext = cin.peek() ;
        } //while

        if ( chnext == EOF ) {
          cout << "ERROR (no closing quote) : END-OF-LINE encountered at Line " << gLine <<  " Column " << gColumn << endl ;
          return expr ;
        } // if

        temp = getAChar() ;
        str = str + temp ;
        whatKindOfToken( str, expr );
      } // if temp = "
      else if ( temp == '(' || temp == ')' || temp == '\'') {  // 1 or 2 token
        if (str != "") {  // 2 token 
          whatKindOfToken( str, expr );
          str.clear();
        } // if

        str = str + temp ;
        if ( temp == '(' ) {
          chnext = cin.peek() ;
          while ( isWhiteSpace( chnext ) ) {
            temp = getAChar() ;
            chnext = cin.peek() ;
          } // while

          if ( chnext == ')' ) {  // ()
            temp = getAChar() ;
            str = str + temp ;
          } // if
        } // if 
        whatKindOfToken( str, expr );
        if ( str == "(" ) parenCount++ ;
        else if ( str == ")" ) {
          parenCount-- ;
          if ( parenCount == 0 ) return expr ;
          else if ( parenCount < 0 ) {
            expr.pop_back() ;
            remainCh = ')' ;
            return expr ;
          } // else if
        } // else if

      } // if temp = ( , ) , '
      else {  // if temp = EOF
        if (str != "") whatKindOfToken( str, expr );
      } // else

      if ( parenCount == 0 ){  
        if ( expr.size() == 1 && expr[0].name == "'" ) ;  // 若只輸入數字(ex:43, 5)就要回傳, 若 expr[0] == '\'' => 後面還會有()
        else return expr ;
      } // if

      if ( temp != EOF ) {
        temp = getAChar() ;
        str.clear();
      } // if 
    } // while temp != EOF, '\n'

    // **********print no ')' ERROR message**********
    cout << "ERROR (no more input) : END-OF-FILE encountered" ;

    return expr ;
  } // getToken()

  bool isExit( vector<Token> expr ) {
    // if ( expr.size() == 0 ) return false ;  不確定需不需要    ex:輸入'\n'
    if ( expr[0].name == "(" && expr[1].name == "exit" && expr[2].name == ")" )
      return true ;
    return false; 
  }

  bool isATOM( Token token ) {
    if ( token.type == SYMBOL || token.type == INT || token.type == FLOAT || token.type == STRING || token.type == NIL || token.type == T )
      return true ;
    return false;
  } // isATOM

  bool isSexp( vector<Token> & expr, bool option ) {
    // option is for recursive S-exp, default is false
    // option: false means need to print ERROR msg
    //         true means need only return false 
    // <S-exp> ::= <ATOM> 
    //           | LP <S-exp> {<S-exp>} [DOT <S-exp>] RP
    //           | '<S-exp>
    // <ATOM>  ::= SYMBOL | INT | FLOAT | STRING | NIL | T | ()

    // get 把expr[0] delete 掉
    // peek 只看expr[0]
    // vector<Token> tokens = expr ;
    
    if ( isATOM( expr[0] ) ) {
      expr.erase(expr.begin()) ;
      return true ;
    } // if 
    else if ( expr[0].name == "(" ) {
      expr.erase(expr.begin()) ;
      if ( isSexp(expr, false) ) {//12
        while ( isSexp(expr, true) ) ;//(
        if ( errorMsg == true ) return false ;  // if 已經印 Error Msg 就不繼續做，直接return
        if ( expr[0].name == "." ) {
          expr.erase(expr.begin()) ;
          if ( isSexp(expr, false) ) {
            if ( expr[0].name == ")" ) return true ;
          } // if 
        } // if
        else if ( expr[0].name == ")" ) {
          return true ;
        }
        else { // ERROR for expected )
          if ( errorMsg == false) {
            cout << "ERROR (unexpected token) : ')' expected when token at Line " << gLine <<  " Column " << gColumn << " is >>" << expr[0].name << "<<\n" ;
            errorMsg = true ;
            while ( getAChar() != '\n' ) ;
          } // if

          return false ;
        } // else
      }
      else {// ERROR for expected ATOM, (
        if ( errorMsg == false) {
          cout << "ERROR (unexpected token) : atom or '(' expected when token at Line " << gLine <<  " Column " << gColumn << " is >>" << expr[0].name << "<<\n" ;
          errorMsg = true ;
          while ( getAChar() != '\n' ) ;
        } // if

        return false ;
      } // else
    }
    else if ( expr[0].name == "'" ) {
      expr.erase(expr.begin()) ;
      if ( isSexp(expr, false) ) return true ;
    }
    else {
      if ( option == true ) {
        return false ;
      } // if
      else {// ERROR for expected ATOM, (
        if ( errorMsg == false) {
          cout << "ERROR (unexpected token) : atom or '(' expected when token at Line " << gLine <<  " Column " << gColumn << " is >>" << expr[0].name << "<<\n" ;
          errorMsg = true ;
          while ( getAChar() != '\n' ) ;
        } // if

        return false ;
      } // else
    }

    return false ;
  } // isSexp

  int findMatchingParenIndex( vector<Token> expr, int start, int end, string findLorR ) {
    int count = 0 ;
    if ( findLorR == "findR" ) {   // 遇到'('，要找到對應的')'
      for ( int i = start; i < end; i++ ) {  // start < end
        if ( expr[i].name == "(" ) count++ ;
        else if ( expr[i].name == ")" ) count-- ;
        if ( count == 0 ) return i ;
      } // for
    } // if
    else if ( findLorR == "findL" ) {   // 遇到')'，要找到對應的'('   
      for ( int i = start; i > end; i-- ) {  // start > end
        if ( expr[i].name == "(" ) count++ ;
        else if ( expr[i].name == ")" ) count-- ;
        if ( count == 0 ) return i ;
      } // for
    } // else if

    return -1 ;
  } // findMatchingParenIndex

  void addToken( vector<Token> & expr, int index, int type, string name ) {
    Token *t = new Token() ;
    t->name = name ;
    t->type = type ;
    expr.insert( expr.begin()+index, *t ) ;
  } // addToken

  void translate( vector<Token> & expr ) {
    /*Rules of translation :
      1. ( □  ◇  ◎  ■ . ◇ ) is directly translated into
        ( □ . ( ◇ . ( ◎ . ( ■ . ◇ ))))

      2. ( □  ◇  ◎  ■ ) is first translated into ( □  ◇  ◎  ■ . nil ),
        and then translated into ( □ . ( ◇ . ( ◎ . ( ■ . nil ))))
    */
    /*
      1. 加上 .nil   (), #f, nil => nil      t, #t => #t
      2. 刪除 .()   需要紀錄(, )位置
      3. 加上 .()
    */

    // int count = 0 ;  // counting number for matching parentheses
    int j = 0, k = 0 ;
    // Step 1
    for ( int i = 0; i < expr.size(); i++ ) {
      if ( expr[i].name == "(" ) {    // check point or add .nil
        j = findMatchingParenIndex( expr, i, expr.size(), "findR" ) ;
        if ( expr[j-1].name == ")" ) { // ')' 前是 (S-exp) => 去找 (S-exp) 前有沒有 '.'
          k = findMatchingParenIndex( expr, j-1, i, "findL" ) ;
          if ( expr[k-1].name != "." ) {  // (S-exp) 前沒有 '.' => expr[j].name == ")" 前要加 .nil
            addToken( expr, j, DOT, "." ) ;
            addToken( expr, j+1, NIL, "nil" ) ;
            i = 0 ;
          } // if 
        } // if
        else if ( expr[j-2].name != "." ) { // ')' 前不是 (S-exp)且沒有 .nil => 加上 .nil
          addToken( expr, j, DOT, "." ) ;
          addToken( expr, j+1, NIL, "nil" ) ;
          i = 0 ;
        } // if
      } // if
      else if ( expr[i].name == "()" || expr[i].name == "#f" ) expr[i].name = "nil" ;
      else if ( expr[i].name == "t" ) expr[i].name = "#t" ;

    } // for ( i < expr.size)
    DP(DEBUG, "Translate Step1 succeeded!") ;
    /*for ( int i = 0; i < expr.size(); i++ )
      cout << expr[i].name << " " ;
    cout << endl ;*/

    // Step 2
    for ( int i = 0 ; i < expr.size() ; i++ ) {
      if ( expr[i].name == "." && expr[i+1].name == "(" ) {
        j = findMatchingParenIndex( expr, i+1, expr.size(), "findR" ) ;
        expr.erase(expr.begin()+j) ;
        expr.erase(expr.begin()+i+1) ;
        expr.erase(expr.begin()+i) ;
        i = 0;
      } // if
    } // for i < expr.size
    DP(DEBUG, "Translate Step2 succeeded!") ;

    // Step 3
    for ( int i = 0; i < expr.size(); i++ ) {  // . 前面的前面不是'(' => 要加.()
      if ( expr[i].name == "." ) {
        if ( expr[i-1].name == ")" ) {  // . 前面是 (S-exp) 的 ')'
          j = findMatchingParenIndex( expr, i-1, 0, "findL" ) ;
          if ( expr[j-1].name != "(" ) { // 加 .()
            if ( expr[i+1].name == "(") { // . 後面是 (S-exp) 的 '('
              k = findMatchingParenIndex( expr, i+1, expr.size(), "findR" ) ;
              addToken( expr, k+1, RIGHT_PAREN, ")" ) ;
              addToken( expr, j, DOT, "." ) ;
              addToken( expr, j+1, LEFT_PAREN, "(" ) ;
            } // if
            else { // . 後面是 ATOM
              addToken( expr, i+2, RIGHT_PAREN, ")" ) ;
              addToken( expr, j, DOT, "." ) ;
              addToken( expr, j+1, LEFT_PAREN, "(" ) ;
            } // else
            i = 0 ;
          } // if
        } // if
        else if ( expr[i-2].name != "(" ) {  // . 前面是 ATOM
          if ( expr[i+1].name == "(") { // . 後面是 (S-exp) 的 '('
            j = findMatchingParenIndex( expr, i+1, expr.size(), "findR" ) ;
            addToken( expr, j+1, RIGHT_PAREN, ")" ) ;
            addToken( expr, i-1, DOT, "." ) ;
            addToken( expr, i, LEFT_PAREN, "(" ) ;
          } // if
          else { // . 後面是 ATOM
            addToken( expr, i+2, RIGHT_PAREN, ")" ) ;
            addToken( expr, i-1, DOT, "." ) ;
            addToken( expr, i, LEFT_PAREN, "(" ) ;
          } // else
          i = 0 ;
        } // else if

      } // if
    } // for i < expr.size
    DP(DEBUG, "Translate Step3 succeeded!") ;
  } // translate

  vector<Token> slicing ( vector<Token> expr, int start, int end ) {
    vector<Token> tokens ;
    copy(expr.begin() + start, expr.begin() + end, back_inserter(tokens));
    return tokens ;
  } // slicing vector

  void prettyPrint( vector<Token> & expr ) {
    /*Rules for printing an S-expression s
      if s is an atom
        then print s with no leading white space and with one trailing '\n'
            note : For 'nil', '()' and '#f', always print 'nil'.
            note : For '#t' and 't', always print '#t'.
            
      else { // s is of the form : '(' s1 s2 ... sn [ '.' snn ] ')'
        Example: (((1 . 2) (3 4) 5 . 6) 7 . 8)
        let M be the number of characters that are already 
            printed on the current line
            
        print '(', print one space, print s1
        print M+2 spaces, print s2
        ...
        print M+2 spaces, print sn
        if there are '.' and snn following sn
          print M+2 spaces, print '.', print '\n'
          print M+2 spaces, print snn
        print M spaces, print ')', print '\n'
    } // else s is of the form : '(' s1 s2 ... sn [ '.' snn ] ')'*/
    int j = 0 ;
    bool check = true ;
    if ( expr.size() == 1  && isATOM(expr[0]) == true ) {
      cout << expr[0].name << endl ;
      return ;
    } // if
    else {
      cout << "( " ;
      if ( expr[0].name == "(" ) {
        j = findMatchingParenIndex( expr, 0, expr.size(), "findR" ) ;
        expr.erase(expr.begin()+j) ;
        expr.erase(expr.begin()) ;
      } // if

      while ( expr[0].name != "." ) {
        if ( expr[0].name == "(" ) {
          for ( int i = 0 ; i < M+2 ; i++ ) cout << " " ;
          prettyPrint( slicing( expr, 0, findMatchingParenIndex( expr, 0, expr.size(), "findR" )+1 ) ) ;
        } // if 
        else {
          for ( int i = 0 ; i < M+2 ; i++ ) cout << " " ;
          prettyPrint( slicing( expr, 0, 1)) ;
        } // else 
      } // while


    } // else
    
  } // prettyPrint

  void buildCons( vector<Token> expr, Token *head ) {
    translate( expr ) ;
    
    // build cons
    /*if ( expr[0].name == "(" ) expr.erase( expr.begin() ) ;
    if ( expr[expr.size()-1].name == ")" ) expr.pop_back() ;*/
    head = addNode(expr) ;
    DP(DEBUG, "addNode succeeded!") ;
    M = 0 ;
    prettyPrint(head) ;
    DP(DEBUG, "prettyPrint succeeded!") ;
  } // buildCons

  Token *addNode( vector<Token> expr ) {
    if (expr.size() == 1) { // ATOM
      cout << " " << expr[0].name << endl ;
      Token *t = new Token() ;
      t->name = expr[0].name ;
      t->type = expr[0].type ;
      t->left = NULL ;
      t->right = NULL ;
      return t ;
    } // if
    else {
      int parenCount = 0 ;
      int start = 0, end = 0, dotindex = -1 ;
      if ( expr[0].name == "(" ) expr.erase( expr.begin() ) ;
      if ( expr[expr.size()-1].name == ")" ) expr.pop_back() ;
      for ( int i = 0 ; i < expr.size() && dotindex == -1; i++ ) {
        if ( expr[i].name == "(" ) parenCount++ ;
        else if ( expr[i].name == ")" ) parenCount-- ;
        if ( parenCount == 0 && expr[i].name == "." ) 
          dotindex = i ;
      } // for

      vector<Token> tokensL = slicing(expr, 0, dotindex) ;
      vector<Token> tokensR = slicing(expr, dotindex+1, expr.size()) ;
      cout << "Slicing tokensL: " ;
      for ( int i = 0; i < tokensL.size(); i++ )
        cout << tokensL[i].name << " " ;
      cout << endl << "Slicing tokensR: " ;
      for ( int i = 0; i < tokensR.size(); i++ )
        cout << tokensR[i].name << " " ;

      Token *t = new Token() ;
      t->left = addNode(tokensL) ;
      t->right = addNode(tokensR) ;
      return t ;
    } // else
  } // addNode
}; // class Token

int main() {
  cout << "Welcome to OurScheme!" << endl << "> " ;
  char temp, remainCh = '\0' ;
  // bool otherToken = false ;
  vector<Token> expr ;  // expr is made up of tokens
  vector<Token> tokens ;
  Token *head = NULL ;
  Token t ;
    
  bool exit = false ;
  while ( exit == false ) {
    // getToken
    expr.clear() ;
    expr = t.getToken(remainCh) ;
    DP(DEBUG, "GetToken succeeded!") ;
    if ( DEBUG_Print )
      for ( int i = 0; i < expr.size(); i++ ) cout << i << ": " << expr[i].name << endl ;

    if ( t.isExit( expr ) == true ) exit = true ;
    else { // get a token串
      /*for ( int i = 0; i < expr.size(); i++ )
        cout << i << ": " << expr[i].name << endl ;*/
      
      // 檢查文法
      tokens = expr ;
      if ( t.isSexp( tokens, false ) == true ) {
        DP(DEBUG, "Parser succeeded!") ;

        tokens = expr ;
        t.prettyPrint(tokens) ;
        DP(DEBUG, "PrettyPrint succeeded!") ;
        if ( DEBUG_Print )
          for ( int i = 0; i < expr.size(); i++ ) cout << i << ": " << expr[i].name << endl ;

        //t.buildCons(expr, head) ;
      } // if
        // do something like calculate 3+5 = 8

      errorMsg = false ;
      cout << endl << "> " ;
    } // else
  } // while()

  if ( exit == true )
    cout << "ERROR (no more input) : END-OF-FILE encountered" ;
      
  cout << endl << "Thanks for using OurScheme!" ;
  cin >> temp ;
  return 0;
} // main()

bool isWhiteSpace(char temp) {
  if ( temp == '\n' || temp == '\t' || temp == ' ' )
    return true ;
  return false ;
} // isWhiteSpace()

char getAChar() { // get a character and count gLine, gColumn
  char ch = '\0' ;
  ch = getchar() ;
  if ( ch == '\n' ) {
    gLine++ ;
    gColumn = 0 ;
  } // if
  else gColumn++ ;

  return ch ;
} // getAChar

void SkipWhiteSpace( char & temp ) {
  temp = getAChar() ;
  while ( isWhiteSpace(temp) ) temp = getAChar() ;
} // SkipWhiteSpace

bool isSeparator( char temp ) {
  if ( isWhiteSpace(temp) || temp == '(' || temp == ')' || temp == '\'' || temp == '"' || temp == ';')
    return true ;
  return false ;
} // isSeparator()

bool isInt( string str ) {
  for ( int i = 0; i < str.size(); i++ ) {
    if ( str[0] == '+' || str[0] == '-' ) ;
    else if ( str[i] >= '0' && str[i] <= '9' ) ;
    else return false ;
  }

  return true ;
} // isInt()

bool isFloat( string str ) {
  int check = 0 ;
  for ( int i = 0; i < str.size(); i++ ) {
    if ( str[0] == '+' || str[0] == '-' ) ;
    else if ( str[i] >= '0' && str[i] <= '9' ) ;
    else if ( str[i] == '.' ) check++ ;
    else return false ;

    if ( check > 1 ) return false ;
  } // for

  return true ;
} // isFloat()
