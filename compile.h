#ifndef COMPILE_H
#define COMPILE_H
#include <iostream>
#include <string>
#include <vector>
#include<cstdlib>
#include"ctype.h"
using namespace std;
string tokenword[]={
    "ARRAY",
    "OF",
    "RECORD",
    "RETURN",
    "PROGRAM",
    "PROCEDURE",
    "TYPE",
    "VAR",
    "IF",
    "THEN",
    "ELSE",
    "FI",
    "INTEGER",
    "CHAR",
    "WHILE",
    "DO",
    "ENDWH",
    "BEGIN",
    "END",
    "READ",
    "WRITE",
    "END_OF_FILE",
    "ERROR",
    "ID",
    "INTC",
    "CHARC",
    /*特殊符号:= = < + - * / （ ） . : ; , [ ] ..*/
    "ASSIGN",
    "EQ",
    "LT",
    "PLUS",
    "MINUS",
    "TIMES",
    "OVER",
    "LPAREN",
    "RPAREN",
    "DOT",
    "COLON",
    "SEMI",
    "COMMA",
    "LMIDPAREN",
    "RMIDPAREN",
    "UNDERANGE"
};
typedef enum
{
  START,
  INASSIGN,
  INCOMMENT,
  INCHAR,
  INID,
  INNUM,
  INRANG,
  DONE
} stateType;
typedef enum
{
  ARRAY,
  OF,
  RECORD,
  RETURN,
  PROGRAM,
  PROCEDURE,
  TYPE,
  VAR,
  IF,
  THEN,
  ELSE,
  FI,
  INTEGER,
  CHAR,
  WHILE,
  DO,
  ENDWH,
  BEGIN,
  END,


  READ,
  WRITE,
  END_OF_FILE,
  ERROR,
  ID,
  INTC,
  CHARC,
  /*特殊符号:= = < + - * / （ ） . : ; , [ ] ..*/
  ASSIGN,
  EQ,
  LT,
  PLUS,
  MINUS,
  TIMES,
  OVER,
  LPAREN,
  RPAREN,
  DOT,
  COLON,
  SEMI,
  COMMA,
  LMIDPAREN,
  RMIDPAREN,
  UNDERANGE
} LexType;
typedef struct token
{
  int linenum;
  LexType Lex;
  string word;
} TokenNode;
static struct
{
  string resword;
  LexType tok;


} resWord[] = {{"program", PROGRAM}, {"type", TYPE}, {"var", VAR}, {"procedure", PROCEDURE}, {"begin", BEGIN}, {"end", END}, {"array", ARRAY}, {"of", OF}, {"record", RECORD}, {"if", IF}, {"then", THEN}, {"else", ELSE}, {"fi", FI}, {"while", WHILE}, {"do", DO}, {"endwh", ENDWH}, {"read", READ}, {"write", WRITE}, {"return", RETURN}, {"integer", INTEGER}, {"char", CHAR}};

LexType markReserve(string &s)
{
  for (int i = 0; i < 21; ++i)
  {
    if (s==resWord[i].resword)
    {
      return resWord[i].tok;
    }
  }
  return ID;
}
void setTokenList(string &s, vector<TokenNode> &t)
{
  int linenum = 1;
  int spos = 0;
  int slength = s.length();
  s=s+"@";
  stateType state = START;
  char c;
  while (spos<slength)
  {
    state = START;
    TokenNode currentToken;
    currentToken.word = "";
    while (state != DONE)
    {
      if (spos == slength+1)


      {
        break;
      }
      c = s[spos++];
      switch (state)
      {
      case START:
      {
        if (isdigit(c))
        {
          currentToken.word.append(1, c);
          state = INNUM;
        }
        else if (isalpha(c))
        {
          currentToken.word.append(1, c);
          state = INID;
        }
        else if (c == ':')
        {
          state = INASSIGN;
        }
        else if (c == '.')
        {
          state = INRANG;
        }
        else if (c == '\'')
        {
          currentToken.word.append(1, c);
          state = INCHAR;
        }
        else if ((c == ' ') || (c == '\t') || (c == '\n'))
        {
          if (c == '\n')
          {
            linenum++;
          }
        }
        else if (c == '{')
        {
          state = INCOMMENT;
        }
        else
        {
          state=DONE;
          switch(c){
            case '@':
              currentToken.Lex = END_OF_FILE;
              break;


            case '=':
              currentToken.Lex = EQ;
              break;
            case '<':
              currentToken.Lex = LT;
              break;
            case '+':
              currentToken.Lex = PLUS;
              break;
            case '-':
              currentToken.Lex = MINUS;
              break;
            case '*':
              currentToken.Lex = TIMES;
              break;
            case '/':
              currentToken.Lex = OVER;
              break;
            case '(':
              currentToken.Lex = LPAREN;
              break;
            case ')':
              currentToken.Lex = RPAREN;
              break;
            case ';':
              currentToken.Lex = SEMI;
              break;
            case ',':
              currentToken.Lex = COMMA;
              break;
            case '[':
              currentToken.Lex = LMIDPAREN;
              break;
            case ']':
              currentToken.Lex = RMIDPAREN;
              break;
            default:
              currentToken.Lex = ERROR;
              break;
          }
        }
        break;
      }
      case INASSIGN:
      {
        state = DONE;
        if(c=='=')
          currentToken.Lex=ASSIGN;
        else{
          spos--;
          currentToken.Lex=ERROR;


        }
        break;
      }
      case INRANG:
      {
        state=DONE;
        if(c=='.'){
          currentToken.Lex=UNDERANGE;
        }else{
          spos--;
          currentToken.Lex = DOT;
        }
        break;
      }
      case INCOMMENT:
      {
        if(c=='\n'){
          linenum++;
        }
        if(c=='}'){
          state=START;
          currentToken.word="";


        }
        break;
      }
      case INNUM:
      {
        if(!isdigit(c)){
          if(isalpha(c)){
            while(isalpha(s[spos])){
              spos++;
            }
            state=DONE;
            currentToken.Lex = ERROR;
          }
          else{
            spos--;
            state = DONE;
            currentToken.Lex = INTC;
          }
        }
        else{
          currentToken.word.append(1,c);
        }


        break;
      }
      case INID:
      {
        if(!isalnum(c)){
           state=DONE;
           spos--;
           currentToken.Lex = markReserve(currentToken.word);
         }
        else{
         currentToken.word.append(1,c);
         }
         break;
      }
      case INCHAR:
      {
        if(isalpha(c)){
          char c0=s[spos++];
          if(c0 == '\''){
            currentToken.word.append(1,c);
            state=DONE;
            currentToken.Lex=CHARC;
          }
          else{
            spos-=2;
            state=DONE;
            currentToken.Lex = ERROR;
          }
        }
        else
        {
          spos--;
          state=DONE;
          currentToken.Lex=ERROR;
        }
        break;
      }
      case DONE:
      {
        break;
      }
      default:
      {
        state = DONE;
        currentToken.Lex = ERROR;
        break;
      }
      }
    }
    currentToken.linenum =linenum;
    t.push_back(currentToken);
  }
}
//语法树节点类型
typedef enum nodeKind
{
  ProK,
  PheadK,
  TypeK,
  VarK,
  ProcDecK,
  StmlK,
  DecK,
  StmtK,
  ExpK
} nodekind;
string nk[] = {"ProK", "PheadK", "TypeK", "VarK", "ProcDecK", "StmlK", "DecK", "StmtK", "ExpK"};
//声明类型
typedef enum kindDetail
{
  ArrayK,
  CharK,
  IntegerK,
  RecordK,
  IdK,
  IfK,
  WhileK,
  AssignK,
  ReadK,
  WriteK,
  CallK,
  ReturnK,
  OpK,
  ConstK
} kDetail;
string kd[] = {"ArrayK", "CharK", "IntegerK", "RecordK", "IdK", "IfK", "WhileK", "AssignK", "ReadK", "WriteK", "CallK", "ReturnK", "OpK", "ConstK"};
//语法树节点结构
typedef struct treeNode
{
  vector<treeNode *> child; //孩子树
  struct treeNode *Sibling; //兄弟树
  vector<string> otherInfo;
  nodekind nK;   //语法树节点类型
  kindDetail kDdec; //具体类型
  kindDetail kDstmt;
  kindDetail kDexp;
  kindDetail childkD;
  int linenum;   //行号
} node;
//语法分析
//语法分析部分
//递归下降法
/*目的输出语法树 输出错误信息*/
//总程序
node * parse(vector<TokenNode>::iterator &it);
node * program(vector<TokenNode>::iterator &it);
//程序头
node * programHead(vector<TokenNode>::iterator &it);
//程序声明包括类型变量函数声明
node * declarePart(vector<TokenNode>::iterator &it);
node * typeDec(vector<TokenNode>::iterator &it);
node * typeDeclaration(vector<TokenNode>::iterator &it);
node * typeDecList(vector<TokenNode>::iterator &it);
node * typeDecMore(vector<TokenNode>::iterator &it);
void typeId(node *t,vector<TokenNode>::iterator &it);
void typeName(node *t,vector<TokenNode>::iterator &it);
void baseType(node *t,vector<TokenNode>::iterator &it);
void structureType(node *t,vector<TokenNode>::iterator &it);
void arrayType(node *t,vector<TokenNode>::iterator &it);
void recType(node *t,vector<TokenNode>::iterator &it);
node * fieldDecList(node *t,vector<TokenNode>::iterator &it);
node * fieldDecMore(vector<TokenNode>::iterator &it);
void idList(node *t,vector<TokenNode>::iterator &it);
void idMore(node *t,vector<TokenNode>::iterator &it);
node * varDec(vector<TokenNode>::iterator &it);
node * varDeclaration(vector<TokenNode>::iterator &it);
node * varDecList(vector<TokenNode>::iterator &it);
node * varDecMore(vector<TokenNode>::iterator &it);
void varIdList(node *t,vector<TokenNode>::iterator &it);
void varIdMore(node *t,vector<TokenNode>::iterator &it);
node * procDec(vector<TokenNode>::iterator &it);
node * procDeclaration(vector<TokenNode>::iterator &it);
void paramList(node *t,vector<TokenNode>::iterator &it);
node * paramDecList(vector<TokenNode>::iterator &it);
node * paramMore(vector<TokenNode>::iterator &it);
node * param(vector<TokenNode>::iterator &it);
void formList(node *t,vector<TokenNode>::iterator &it);
void fidMore(node *t,vector<TokenNode>::iterator &it);
node * procDecPart(vector<TokenNode>::iterator &it);
//程序体由语句序列组成
node * procBody(vector<TokenNode>::iterator &it);
node * programBody(vector<TokenNode>::iterator &it);
//语句序列
node * stmList(vector<TokenNode>::iterator &it);
node * stmMore(vector<TokenNode>::iterator &it);
//语句
node * stm(vector<TokenNode>::iterator &it);
node * assCall(vector<TokenNode>::iterator &it);
node * assignmentRest(vector<TokenNode>::iterator &it);
node * conditionalStm(vector<TokenNode>::iterator &it);
node * loopStm(vector<TokenNode>::iterator &it);
node * inputStm(vector<TokenNode>::iterator &it);
node * outputStm(vector<TokenNode>::iterator &it);
node * returnStm(vector<TokenNode>::iterator &it);
node * callStmRest(vector<TokenNode>::iterator &it);
node * actParamList(vector<TokenNode>::iterator &it);
node * actParamMore(vector<TokenNode>::iterator &it);
//表达式
node * exp(vector<TokenNode>::iterator &it);
node *expRel(vector<TokenNode>::iterator &it);
node *expRelMore(vector<TokenNode>::iterator &it);
node * term(vector<TokenNode>::iterator &it);
node * termMore(vector<TokenNode>::iterator &it);
node * factor(vector<TokenNode>::iterator &it);
node * factorMore(vector<TokenNode>::iterator &it);
node * variable(vector<TokenNode>::iterator &it);
void variMore(node *t,vector<TokenNode>::iterator &it);
node * fieldvar(vector<TokenNode>::iterator &it);
void fieldvarMore(node *t,vector<TokenNode>::iterator &it);


//错误存储字符串
string err="";
//错误存储函数
void Error(string &s,string s0, int n)
{
   s+="Error in line" + to_string(n) + ":" + s0 + "\n";
}
void otherError(string &s,string s0)
{
  s+=s0+"\n";
}
////////////
//主程序部分//
////////////
node *parse(vector<TokenNode>::iterator &it){
  node *t=NULL;
  t=program(it);
  if(t==NULL){
    otherError(err,"编译失败，请检查程序后重试");
  }
  if(it->Lex=END_OF_FILE){
    return t;
  }else{
    otherError(err,"编译失败，请检查程序后重试");
  }
}

/**************program部分***********/
node * program(vector<TokenNode>::iterator &it){
  node *root = new node();
  root->nK= ProK;
  node * t=programHead(it);                //三部分
  node * q=declarePart(it);
  node * s=programBody(it);
  if(t!=NULL){
    root->child.push_back(t);
  }
  else{
    otherError(err,"缺少程序头部");
  }
  if(q!=NULL){
    root->child.push_back(q);
  }
  if (s != NULL)
  {
    root->child.push_back(s);
  }
  if(it->Lex == DOT){
    it++;
  }
  return root;
}
/////////////////////////////
//////////programhead部分/////
/////////////////////////////
node * programHead(vector<TokenNode>::iterator &it){
  node *phead = NULL;
  if(it->Lex!=PROGRAM){
    Error(err,"缺少program",it->linenum);
  }
  it++;
  if(it->Lex!=ID){
    Error(err,"缺少标识符",it->linenum);
  }else{
    phead=new node();
    phead->nK=PheadK;
    phead->otherInfo.push_back(it->word);
  }
  it++;
  return phead;
}


////////////////////////////////
//////////declarePart   部分/////
////////////////////////////////

/**************declarePart 主函数********/
node * declarePart(vector<TokenNode>::iterator &it){
  node *typeP=NULL;
  node *varP=NULL;
  node *tp;
  tp = typeDec(it);
  if(tp!=NULL){
    tp->nK=TypeK;
    typeP=new node();
    typeP->nK=TypeK;
    typeP->child.push_back(tp);
  }

  node *vp;
  cout<<tokenword[it->Lex]<<"hao "<<it->linenum<<endl;
  vp = varDec(it);
  if(vp!=NULL){
    vp->nK=VarK;
    varP=new node();
    varP->nK=VarK;
  }
   cout<<tokenword[it->Lex]<<"hao nima "<<it->linenum<<endl;
  node *pp;
  pp = procDec(it);
  if(pp!=NULL){
    pp->nK=ProcDecK;
  }
  if(varP==NULL){
    varP=pp;
  }
  if(typeP==NULL){
    typeP=varP;
  }
  if(typeP!=varP){
    typeP->Sibling=varP;
  }
  if(varP!=pp){
    varP->Sibling=pp;
  }
  return typeP;
}





/************typedec部分********/
node * typeDec(vector<TokenNode>::iterator &it){
  node *tp=NULL;
  if(it->Lex==TYPE){
    tp=typeDeclaration(it);
  }else{
   if(it->Lex!=VAR&&it->Lex!=PROCEDURE&&it->Lex!=BEGIN){;
     Error(err,"缺少var|procdure|begin",it->linenum);
     it++;    //跳过错误单词
   }
  }
  return tp;
}
node * typeDeclaration(vector<TokenNode>::iterator &it){
  node *tp=NULL;
  if(it->Lex==TYPE){
    it++;
          cout<<tokenword[it->Lex]<<endl;
    tp=typeDecList(it);
    tp->nK=TypeK;

  }
  if(tp==NULL){
    Error(err,"缺少type",it->linenum);
  }
  return tp;
}
/****typedeclist和他的三个子函数***/
node * typeDecList(vector<TokenNode>::iterator &it){
  node *tp=new node();
  node *t=NULL;
  typeId(tp,it);
        cout<<tokenword[it->Lex]<<endl;
  if(it->Lex!=EQ){
    Error(err,"缺少 =",it->linenum);
  }
  it++;
    cout<<tokenword[it->Lex]<<endl;
  typeName(tp,it);
    cout<<tokenword[it->Lex]<<endl;
  if(it->Lex!=SEMI){
    cout<<it->Lex<<endl;
    Error(err,"缺少 ;",it->linenum);
  }
  it++;
    cout<<tokenword[it->Lex]<<endl;
  t=typeDecMore(it);
      cout<<tokenword[it->Lex]<<endl;
  if(t!=NULL){
    tp->Sibling=t;
  }

  return tp;
}
node *typeDecMore(vector<TokenNode>::iterator &it){
  node *t=NULL;
  cout<<tokenword[it->Lex]<<endl;
  if(it->Lex==VAR||it->Lex==PROCEDURE||it->Lex==BEGIN){
    t=NULL;
  }
  else if(it->Lex==ID){
    t=typeDecList(it);
  }
  else{
      Error(err,"type 声明错误",it->linenum);
      it++;
  }
  return t;
}
void typeId(node *t,vector<TokenNode>::iterator &it){
  if(it->Lex==ID&&t!=NULL){
    t->otherInfo.push_back(it->word);
    it++;
  }else{
    Error(err,"缺少ID",it->linenum);
  }
}
void typeName(node *t,vector<TokenNode>::iterator &it){
  if(it->Lex==ID){
    t->kDdec=IdK;
      it++;
  }else if(it->Lex==INTEGER||it->Lex==CHAR){
    baseType(t,it);
      it++;
  }else if(it->Lex==ARRAY||it->Lex==RECORD){
    structureType(t,it);
  }else{
    cout<<tokenword[it->Lex]<<it->linenum<<endl;
    Error(err,"缺少int|char|ID|ARRAY|RECORD",it->linenum);
  }

}
void baseType(node *t,vector<TokenNode>::iterator &it){
  if(it->Lex==INTEGER){
    t->kDdec=IntegerK;
  }else if(it->Lex==CHAR){
    t->kDdec=CharK;
  }
}
void structureType(node *t,vector<TokenNode>::iterator &it){
  if(it->Lex==ARRAY){
    arrayType(t,it);
    it++;
  }else if(it->Lex==RECORD){
    t->kDdec=RecordK;
    recType(t,it);
      cout<<tokenword[it->Lex]<<"huahauahauhau"<<endl;
  }
}
void arrayType(node *t,vector<TokenNode>::iterator &it){
  if(it->Lex!=ARRAY){
    Error(err,"缺少 array",it->linenum);
  }
  it++;
  if(it->Lex!=LMIDPAREN){
    Error(err,"缺少 [",it->linenum);
  }
  it++;
  if(it->Lex==INTC){
    t->otherInfo.push_back(it->word);
    it++;
  }else{
    Error(err,"缺少数组下界",it->linenum);
  }
  if(it->Lex!=UNDERANGE){
    Error(err,"缺少  ..",it->linenum);
  }
  it++;
    if(it->Lex==INTC){
    t->otherInfo.push_back(it->word);
    it++;
  }else{
    Error(err,"缺少数组上界",it->linenum);
  }
  if(it->Lex!=RMIDPAREN){
    Error(err,"缺少 ]",it->linenum);
  }
  it++;
  if(it->Lex!=OF){
    Error(err,"缺少 of",it->linenum);
  }
  it++;
  baseType(t,it);
  t->childkD=t->kDdec;
  t->kDdec=ArrayK;
}
void recType(node *t,vector<TokenNode>::iterator &it){
  node *p=NULL;
  if(it->Lex!=RECORD){
    Error(err,"缺少 record",it->linenum);
  }
  it++;
  p=fieldDecList(t,it);
  cout<<tokenword[it->Lex]<<"  "<<it->linenum<<endl;
  if(p==NULL){
    Error(err,"record 声明出错",it->linenum);
  }else{
    t->child.push_back(p);
  }
  if(it->Lex!=END){
    Error(err,"缺少 end",it->linenum);
  }
  it++;
}
node * fieldDecList(node *t,vector<TokenNode>::iterator &it){
  node *p=NULL;
  if(t==NULL){
    return t;
  }
  if(it->Lex==INTEGER||it->Lex==CHAR){
    baseType(t,it);
    idList(t,it);
    if(it->Lex==SEMI){
      it++;
      p=fieldDecMore(it);
    }else{
      Error(err,"缺少 ;",it->linenum);
    }
  }else if(it->Lex==ARRAY){
    arrayType(t,it);
    idList(t,it);
    if(it->Lex==SEMI){
      it++;
      p=fieldDecMore(it);
    }else{
      cout<<it->linenum<<endl;
      Error(err,"缺少 ;",it->linenum);
    }
  }else{
    it++;
  }
  if(p!=NULL){
  t->Sibling=p;
  }
  return t;
}
node * fieldDecMore(vector<TokenNode>::iterator &it){
  node *t=NULL;
  if(it->Lex==END){
    return t;
  }else if(it->Lex==INTEGER||it->Lex==CHAR||it->Lex==ARRAY){
    t=new node();
    t=fieldDecList(t,it);
  }else{
    Error(err,"缺少 end",it->linenum);
    it++;
  }
  return t;
}
void idList(node *t,vector<TokenNode>::iterator &it){
  it++;
  if(it->Lex==ID){
    t->otherInfo.push_back(it->word);
  }else{
    Error(err,"缺少ID 在integer|char 后",it->linenum);
  }
  idMore(t,it);
}
void idMore(node *t,vector<TokenNode>::iterator &it){
  it++;
  if(it->Lex==SEMI){

  }else if(it->Lex==COMMA){
    idList(t,it);
  }else{
    it++;
  }
}



/************vardec部分********/
node * varDec(vector<TokenNode>::iterator &it){
  cout<<tokenword[it->Lex]<<"  "<<it->linenum<<endl;
  node *t=NULL;
  if(it->Lex==PROCEDURE||it->Lex==BEGIN){
    return t;
  }else if(it->Lex==VAR){
    t=varDeclaration(it);
  }else{
    Error(err,"缺少 procedure|var|begin",it->linenum);
    it++;
  }
  return t;
}
node * varDeclaration(vector<TokenNode>::iterator &it){
  node *t=NULL;
  if(it->Lex==VAR){
    it++;
    t=varDecList(it);
  }
  if(t==NULL){
    Error(err,"缺少变量声明",it->linenum);
  }
  return t;
}
node * varDecList(vector<TokenNode>::iterator &it){
  node *t=new node();
  node *p=NULL;
  cout<<tokenword[it->Lex]<<it->linenum<<endl;
  if(t!=NULL){
    typeName(t,it);
    varIdList(t,it);
    if(it->Lex==SEMI){
      it++;
    }
    p=varDecMore(it);
    t->Sibling=p;
  }
  return t;
}
node * varDecMore(vector<TokenNode>::iterator &it){
  node *t=NULL;
  if(it->Lex==PROCEDURE||it->Lex==BEGIN){
    t=NULL;
  }else if(it->Lex==INTEGER||it->Lex==CHAR||it->Lex==ARRAY||it->Lex==RECORD||it->Lex==ID){
    t=varDecList(it);
  }else{
    Error(err,"缺少过程声明",it->linenum);
    it++;
  }
  return t;
}
void varIdList(node *t,vector<TokenNode>::iterator &it){
  if(it->Lex==ID){
    t->otherInfo.push_back(it->word);
    it++;
  }else{
    Error(err,"缺少 ID",it->linenum);
    it++;
  }
  varIdMore(t,it);
}
void varIdMore(node *t,vector<TokenNode>::iterator &it){
  if(it->Lex==SEMI){

  }else if(it->Lex==COMMA){
    it++;
    varIdList(t,it);
  }else{
    Error(err,"缺少 ,|;",it->linenum);
    it++;
  }
}



/************procdec部分********/
node * procDec(vector<TokenNode>::iterator &it){
  node *t=NULL;

  if(it->Lex==BEGIN){
    t=NULL;
  }else if(it->Lex==PROCEDURE){
    t=procDeclaration(it);
  }else{
    Error(err,"缺少函数声明",it->linenum);
    it++;
  }
  return t;
}
node * procDeclaration(vector<TokenNode>::iterator &it){
  node *t=new node();

  if(it->Lex!=PROCEDURE){
    Error(err,"缺少 procedure",it->linenum);
  }
  if(t!=NULL&&it->Lex==PROCEDURE){
    it++;
    if(it->Lex==ID){

      t->otherInfo.push_back(it->word);
    }else{
      Error(err,"缺少 ID",it->linenum);
    }
    it++;
    if(it->Lex==LPAREN){

      it++;
      paramList(t,it);
    }else{
       Error(err,"缺少 (",it->linenum);
    }

    if(it->Lex==RPAREN){
      it++;
     if(it->Lex==SEMI){
        it++;
        node *t1=procDecPart(it);
        t->child.push_back(t1);
        node *t2=procBody(it);
        t->child.push_back(t2);
      }
      else{
        cout<<it->Lex<<endl;
        Error(err,"缺少 ;",it->linenum);
      }
    }else{
       Error(err,"缺少 )",it->linenum);
    }
  }
  return t;
}
void paramList(node *t,vector<TokenNode>::iterator &it){
  node *p=NULL;
  if(it->Lex==RPAREN){
    p=NULL;
  }else if(it->Lex==INTEGER||it->Lex==CHAR||it->Lex==RECORD||it->Lex==ARRAY||it->Lex==ID||it->Lex==VAR){
    p=paramDecList(it);
    t->child.push_back(p);
  }else{
    it++;
  }
}
node * paramDecList(vector<TokenNode>::iterator &it){
  node *t=NULL;
  node *p=NULL;
  t=param(it);
  p=paramMore(it);
  if(p!=NULL){
    t->Sibling=p;
  }
  return t;
}
node * paramMore(vector<TokenNode>::iterator &it){
  node *t=NULL;
  if(it->Lex==RPAREN){
    t=NULL;
  }else if(it->Lex==SEMI){
    it++;
    t=paramDecList(it);
    if(t=NULL){
      Error(err,"缺少函数声明",it->linenum);
    }
  }else{
    it++;
  }
  return t;
}
node * param(vector<TokenNode>::iterator &it){
  node *t=new node();
  t->nK=ProcDecK;
  if(t!=NULL){
    if(it->Lex==INTEGER||it->Lex==CHAR||it->Lex==RECORD||it->Lex==ARRAY||it->Lex==ID){
      typeName(t,it);
      formList(t,it);
    }else if(it->Lex==VAR){
      it++;
      typeName(t,it);
      formList(t,it);
    }else{
      it++;
    }
  }
  return t;
}
void formList(node *t,vector<TokenNode>::iterator &it){
  if(it->Lex==ID){
    t->otherInfo.push_back(it->word);
  }
  it++;
  fidMore(t,it);

}
void fidMore(node *t,vector<TokenNode>::iterator &it){
  if(it->Lex==SEMI){
  }else if(it->Lex==RPAREN){
  }else if(it->Lex==COMMA){
    it++;
    formList(t,it);
  }else{
    it++;
  }
}
node * procDecPart(vector<TokenNode>::iterator &it){
  node *t=NULL;
  t=declarePart(it);
  return t;
}





/************过程体部分********/
node * procBody(vector<TokenNode>::iterator &it){
  node *t=NULL;

  t=programBody(it);

  if(t==NULL){

  }
  return t;
}
node * programBody(vector<TokenNode>::iterator &it){
  node *t=new node();
  t->nK=StmlK;

    if(it->Lex==BEGIN&&t!=NULL){
      node *p=NULL;
      it++;
      p=stmList(it);
      t->child.push_back(p);
    }
    if(it->Lex==END){
      return t;
    }else{
      return NULL;
    }
}

node * stmList(vector<TokenNode>::iterator &it){
  node *t=NULL;
  node *p=NULL;
  t=stm(it);
  p=stmMore(it);
  if(p!=NULL){
    t->Sibling=p;
  }
  return t;
}
node * stmMore(vector<TokenNode>::iterator &it){
  node *t=NULL;
  cout<<tokenword[it->Lex]<<endl;
  if(it->Lex!=END&&it->Lex==ENDWH&&it->Lex!=ELSE&&it->Lex!=FI){
    Error(err,"缺少end|else|fi|endwh",it->linenum);
  }else if(it->Lex==SEMI){
    it++;
    t=stmList(it);
  }else{
    it++;
  }
  return t;
}

node * stm(vector<TokenNode>::iterator &it){
  node *t=NULL;
  cout<<tokenword[it->Lex]<<endl;
  if(it->Lex==IF){
    t=conditionalStm(it);
  }else if(it->Lex==WHILE){
    t=loopStm(it);
  }else if(it->Lex==RETURN){
    t=returnStm(it);
  }else  if(it->Lex==READ){
    t=inputStm(it);
  }else if(it->Lex==WRITE){
    t=outputStm(it);
  }else if(it->Lex==ID){
    it++;
    t=assCall(it);
  }else{
    Error(err,"非期望单词",it->linenum);
    it++;
  }

  return t;
}
node * assCall(vector<TokenNode>::iterator &it){
  node *t=new node();
  t->nK=StmlK;
  if(it->Lex==ASSIGN||it->Lex==DOT||it->Lex==LMIDPAREN){
    t=assignmentRest(it);
  }else if(it->Lex==LPAREN){
    t=callStmRest(it);
  }else{
    it++;
  }
  return t;
}
node * assignmentRest(vector<TokenNode>::iterator &it){
  node *t=new node();
  t->nK=StmlK;
  t->kDstmt=AssignK;
  if(it->Lex==ASSIGN&&t!=NULL){

    it++;
    node *p=expRel(it);
    t->child.push_back(p);
  }
      cout<<tokenword[it->Lex]<<"xbcbcbcb"<<it->word<<endl;
  return t;
}
node * conditionalStm(vector<TokenNode>::iterator &it){
  node *t=new node();
  t->nK=StmlK;
  t->kDstmt=IfK;
  if(it->Lex==IF&&t!=NULL){
    it++;
    node*p=expRel(it);
    p->nK=ExpK;
    t->child.push_back(p);
  }else{
    it++;
    Error(err,"缺少if",it->linenum);
  }
  if(it->Lex==THEN&&t!=NULL){
    node *p=stm(it);
    p->nK=StmlK;
    t->child.push_back(p);
  }else{
    it++;
    Error(err," 缺少then ",it->linenum);
  }
  if(it->Lex==ELSE&&t!=NULL){
    node *p=stm(it);
    p->nK=StmlK;
    t->child.push_back(p);
  }else{
    it++;
    Error(err," 缺少else ",it->linenum);
  }
  if(it->Lex==FI){
    it++;
  }
  return t;

}
node * loopStm(vector<TokenNode>::iterator &it){
  node *t=new node();
  t->nK=StmlK;
  if(it->Lex==WHILE&&t!=NULL){
    it++;
    node *p=expRel(it);
    p->nK=ExpK;
    t->child.push_back(p);
  }else{
    it++;
    Error(err,"缺少while",it->linenum);
  }
  if(it->Lex==DO){
    node *p=stmList(it);
    p->nK=StmlK;
    t->child.push_back(p);
  }else{
    it++;
    Error(err,"缺少do",it->linenum);
  }
  return t;
}
node * inputStm(vector<TokenNode>::iterator &it){
  node *t=new node();
  t->nK=StmlK;
  if(it->Lex==READ){
    it++;
  }
  if(it->Lex==LPAREN&&t!=NULL){
    it++;
    if(it->Lex==ID){
      t->otherInfo.push_back(it->word);
    }else{
      Error(err,"缺少ID",it->linenum);
    }
  }else{
          Error(err,"缺少 (",it->linenum);
  }
  it++;
  if(it->Lex==RPAREN){
    it++;
  }else{
      Error(err,"缺少 )",it->linenum);
  }
  return t;
}
node * outputStm(vector<TokenNode>::iterator &it){
  node *t=new node();
  t->nK=StmlK;
  if(it->Lex==WRITE){
    it++;
  }
  if(it->Lex==LPAREN&&t!=NULL){
    it++;
    node *p=expRel(it);
    p->nK=StmlK;
    t->child.push_back(p);
  }else{
    Error(err,"缺少 (",it->linenum);
  }
  if(it->Lex==RPAREN){
    it++;
  }else{
     cout<<tokenword[it->Lex]<<"hauhau"<<endl;
      Error(err,"缺少 )",it->linenum);
  }
  return t;
}
node * returnStm(vector<TokenNode>::iterator &it){
  node *t=new node();
  if(it->Lex==RETURN){
    t->nK=StmlK;
    t->kDstmt=ReturnK;
  }
  it++;
  return t;
}
node * callStmRest(vector<TokenNode>::iterator &it){
  node *t=new node();
  t->nK=StmlK;
  t->kDstmt=CallK;
  if(it->Lex==LPAREN&&t!=NULL){
    it++;
    node *p=actParamList(it);
    if(p!=NULL){
      t->child.push_back(p);
    }
  }else{
    cout<<tokenword[it->Lex]<<"  海阿弗尼斯  "<<it->word<<endl;
    Error(err,"缺少 (",it->linenum);
  }
  it++;
  if(it->Lex==RPAREN){
    it++;
  }
  return t;
}
node * actParamList(vector<TokenNode>::iterator &it){
  node *t=NULL;
  if(it->Lex==RPAREN){
    return t;
  } else if(it->Lex==ID||it->Lex==INTC){
    t=expRel(it);
    if(t!=NULL){
      node *p=actParamMore(it);
      t->Sibling=p;
    }
  }else{
    it++;
    Error(err,"缺少 ）",it->linenum);
  }
  return t;
}
node * actParamMore(vector<TokenNode>::iterator &it){
  node *t=NULL;
  if(it->Lex==RPAREN){
    return t;
  }
  else if(it->Lex==COMMA){
    it++;
    t=actParamList(it);
  }else{
    Error(err,"缺少 )",it->linenum);
    it++;
  }
  return t;
}
node * expRel(vector<TokenNode>::iterator &it){
  node *t=exp(it);
  node *p=expRelMore(it);
  t->Sibling=p;
  return t;
}
node * expRelMore(vector<TokenNode>::iterator &it){
  node *t=NULL;
  if(it->Lex==LT||it->Lex==EQ){
    it++;
    t=exp(it);
  }
  return t;

}
node * exp(vector<TokenNode>::iterator &it){
  node *t=term(it);
  node *p=termMore(it);
  return t;
}

node * term(vector<TokenNode>::iterator &it){
  node *t=factor(it);
  node *p=factorMore(it);
  t->Sibling=p;
  return t;
}
node * termMore(vector<TokenNode>::iterator &it){
    node *t=NULL;
    if(it->Lex==PLUS||it->Lex==MINUS)
    {
        it++;
        t=exp(it);
    }else if(it->Lex!=SEMI&&it->Lex!=END&&it->Lex!=ELSE&&it->Lex!=FI&&it->Lex!=ENDWH&&it->Lex!=RPAREN&&it->Lex==COMMA&&it->Lex==LT&&it->Lex==EQ&&it->Lex!=DO&&it->Lex!=THEN&&it->Lex!=RMIDPAREN){
      Error(err,"需要 OR",it->linenum);
    }
    return t;
}
node * factor(vector<TokenNode>::iterator &it){
  node * t=new node();
  t->nK=ExpK;
  if(it->Lex==INTC){
    t->otherInfo.push_back(it->word);
    it++;
  }else if(it->Lex==ID){
    t=variable(it);
  }else if(it->Lex==LPAREN){
    it++;
    t=expRel(it);
  if(it->Lex==RPAREN){

  }
  }
  return t;
}
node *factorMore(vector<TokenNode>::iterator &it){
    node *t=NULL;
    if(it->Lex==TIMES||it->Lex==OVER)
    {
        it++;
        t=term(it);
    }else if(it->Lex!=SEMI&&it->Lex!=END&&it->Lex!=ELSE&&it->Lex!=FI&&it->Lex!=ENDWH&&it->Lex!=RPAREN&&it->Lex==COMMA&&it->Lex==LT&&it->Lex==EQ&&it->Lex!=DO&&it->Lex!=THEN&&it->Lex!=PLUS&&it->Lex!=RMIDPAREN){
      Error(err,"需要 OR",it->linenum);
    }
    return t;
}
node * variable(vector<TokenNode>::iterator &it){
  node *t=NULL;
  if(it->Lex==ID){
    t=new node();
    t->nK=ExpK;
    it++;
    variMore(t,it);
  }
  return t;
}
void variMore(node *t,vector<TokenNode>::iterator &it){
  if(it->Lex!=OVER&&it->Lex!=RPAREN&&it->Lex!=SEMI&&it->Lex!=COMMA&&it->Lex!=THEN&&it->Lex!=ELSE&&it->Lex!=FI&&it->Lex!=DO&&it->Lex!=ENDWH&&it->Lex!=END){

  }else if(it->Lex==LMIDPAREN){
    it++;
    t=expRel(it);
  }else if(it->Lex==DOT){
    it++;
    t=fieldvar(it);
  }
}
node * fieldvar(vector<TokenNode>::iterator &it){
  node *t=new node();
  if(t!=NULL){
    if(it->Lex!=ID){
         Error(err,"缺少 id",it->linenum);
    }
    it++;
    fieldvarMore(t,it);
  }
  return t;
}
void fieldvarMore(node *t,vector<TokenNode>::iterator &it){
  if(it->Lex==OVER||it->Lex==RPAREN||it->Lex==SEMI||it->Lex==COMMA||it->Lex==THEN||it->Lex==ELSE||it->Lex==FI||it->Lex==DO||it->Lex==ENDWH||it->Lex==END){

  }else if(it->Lex==LMIDPAREN){
    t=expRel(it);
  }else{
    it++;
  }
  if(it->Lex!=RMIDPAREN){
    Error(err,"缺少 ]",it->linenum);
  }
}

#endif // COMPILE_H
