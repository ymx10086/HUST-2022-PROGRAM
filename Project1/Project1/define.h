//
// Created by hp on 2022-09-04.
//

#ifndef SUMIT_DEFINE_H
#define SUMIT_DEFINE_H

#define _CRT_SECURE_NO_WARNINGS

//引入头文件
#include <graphics.h>		// 引用图形库头文件
#include <conio.h>
#include<iostream>
#include<cstdio>
#include<algorithm>
#include<list>
#include<cstring>
#include<sstream>
#include<queue>
#include<vector>
#include<map>
#include<fstream>
#include<set>
#include<bitset>
#include<cmath>
#include<unordered_map>
#include<random>
#include<ctime>
#include<sys/utime.h>

//有效定义
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASTABLE -1
#define INCREASEMENT 100
#define MAXN 0x7FFFFF
#define _CRT_SECURE_NO_WARNINGS

static struct timeval st;
static struct timeval ed;
static double time_total;


//结构体相互引用交叉定义
struct Clause_Node;
struct Literal_Node;
typedef struct Clause_Node Clause_Node, * Clause_List;
typedef struct Literal_Node Literal_Node, * Literal_List;
typedef int status;

/*定义子句链式结构节点*/
struct Clause_Node {
    int number;//定义子句中的文字数，可判定是否为单子句
    int tag;//定义字句是否被处理
    Literal_List first_literal;//定义文字节点，指向第一文字
    int watch_literal[2];//定义被监视节点
    struct Clause_Node* next_clause;//定义字句节点，指向下一子句
};

/*定义文字链式结构节点*/
struct Literal_Node {
    int literal;//定义整型文字
    int tag;//定义文字是否被处理
    struct Literal_Node* next_literal;//定义下一文字节点
    struct Literal_Node* next_same_literal;//定义下一相同节点
    struct Clause_Node* head;//定义该文字对应子句句首
};

/*定义CNF链式结构结点，存储CNF信息*/
typedef struct Conjunctive_Normal_Form {
    int literal_num;//定义文字数目
    int clause_num;//定义子句数目
    Clause_List first_clause;//定义子句头节点，指向CNF的第一个子句
}Conjunctive_Normal_Form, * Conjunctive_Normal_Form_List;

typedef struct ArgueValue {
    int tag;//是否被判断真假
    int is_value;//判断真值情况
    int dep;//栈的深度
    int score;//该变量的得分
    int num;//文字总数
    int pos;//正文字数目
    int nev;//负文字数目
    Literal_List first;//首子句指针
}ArgueValue;

/*CNF文件读取处理函数*/
status DestroyClause(Clause_List& sentence);//定义DestroyClause函数，销毁子句结点
status DestroyLiteral(Literal_List& word);//定义DestroyLiteral函数，销毁文字结点
status DestroyCNF(Conjunctive_Normal_Form_List& cnf);//定义DestroyCNF函数，销毁CNF文件

//func_DPLL1所用函数
status CreateCNF(FILE* fp, Conjunctive_Normal_Form_List& cnf);//定义CreateCNF函数，创建CNF链表结果
status CreateClause(FILE* fp, Clause_List& sentence, int first);//定义CreateClause函数，创建子句链表
status DeleteLiteral(Conjunctive_Normal_Form_List& cnf, int literal);//定义DeleteLiteral函数，删除cnf结构中所有的literal文字
status RecoverLiteral(Conjunctive_Normal_Form_List& cnf, int literal);//定义RecoverLiteral函数，回复特定literal文字
status update_storevalue(Conjunctive_Normal_Form_List& cnf);//定义update_storevalue函数，更新存储文字
status update_storevalue2(Conjunctive_Normal_Form_List& cnf);//定义update_storevalue函数，更新存储文字
status DPLL1(Conjunctive_Normal_Form_List cnf, int op);//定义DPLL1函数，作为处理cnf文件的第一个DPLL文件
int Select_Literal_DPLL0(Conjunctive_Normal_Form_List& cnf);//定义Select_Literal函数，变量决策策略
int Select_Literal_DPLL1(Conjunctive_Normal_Form_List& cnf);//定义Select_Literal函数，变量决策策略
int Select_Literal_DPLL2(Conjunctive_Normal_Form_List& cnf);//定义Select_Literal函数，变量决策策略
int Select_Literal_DPLL3(Conjunctive_Normal_Form_List& cnf);//定义Select_Literal函数，变量决策策略
status DPLL1_Partition(Conjunctive_Normal_Form_List& cnf, int literal, int op);//定义DPLL1_Partition函数，变量分裂规则
status DPLL1_Recover(Conjunctive_Normal_Form_List& cnf, int literal);//定义DPLL1_Recover函数，变量回溯规则
Clause_List HasUnitClause(Conjunctive_Normal_Form_List& cnf);//定义HasUnitClause函数，评估CNF是否含有单子句
status print(Conjunctive_Normal_Form_List& cnf);//定义print函数，输出真值表，用于验证和调试


//func_DPLL2所用函数
status CreateCNF2(FILE* fp, Conjunctive_Normal_Form_List& cnf);//定义CreateCNF函数，创建CNF链表结果
status CreateClause2(FILE* fp, Clause_List& sentence, int first);//定义CreateClause函数，创建子句链表
status DestroyClause2(Clause_List& sentence);//定义DestroyClause函数，销毁子句结点
status DestroyLiteral2(Literal_List& word);//定义DestroyLiteral函数，销毁文字结点
status DestroyCNF2(Conjunctive_Normal_Form_List& cnf);//定义DestroyCNF函数，销毁CNF文件
status print2(Conjunctive_Normal_Form_List& cnf);//定义print函数，输出真值表，用于验证和调试
status DPLL2(Conjunctive_Normal_Form_List cnf);//定义DPLL1函数，作为处理cnf文件的第一个DPLL文件
status check_print2(Conjunctive_Normal_Form_List& cnf);//定义check_print2函数，进行检查

status CreateSudoku();//定义CreateSudoku函数，创建数独终盘
status Dig_Hole_Easy(int dig);//定义Dig_Hole_Easy函数，挖洞法生成数独
status sudokusat(int m, int n, Conjunctive_Normal_Form_List cnf);//定义sudokusat函数，求解数独问题
status SudokuFinalPrint(void);//定义SudokuFinalPrint函数，输出数独游戏
status DPLL2_SUDOKU(Conjunctive_Normal_Form_List cnf, int t[], int top);//定义DPLL2_SUDOKU函数，调用SAT求解器
int print_time(Conjunctive_Normal_Form_List cnf);//定义print_time函数，输出求解时间

void solve(int op);//定义solve函数，输出交互界面
void solve_cnf();//定义solve_cnf函数，输出cnf文件求解交互界面
void solve_sudoku();//定义solve_sudoku函数，输出数独求解交互界面
void store_document(Conjunctive_Normal_Form_List& cnf, char* filename, int d, int time0);//定义store_document函数，存储文件
void traveser_cnf(Conjunctive_Normal_Form_List cnf);//定义traveser_cnf函数，进行CNF文件遍历
void prove_cnf(Conjunctive_Normal_Form_List cnf);//定义prove_cnf函数，进行验证
void show_cnf(Conjunctive_Normal_Form_List cnf);//定义show_cnf函数，输出cnf文件求解结果
#endif //SUMIT_DEFINE_H
