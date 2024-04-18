//
// Created by hp on 2022-09-04.
//

#ifndef SUMIT_DEFINE_H
#define SUMIT_DEFINE_H

#define _CRT_SECURE_NO_WARNINGS

//����ͷ�ļ�
#include <graphics.h>		// ����ͼ�ο�ͷ�ļ�
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

//��Ч����
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


//�ṹ���໥���ý��涨��
struct Clause_Node;
struct Literal_Node;
typedef struct Clause_Node Clause_Node, * Clause_List;
typedef struct Literal_Node Literal_Node, * Literal_List;
typedef int status;

/*�����Ӿ���ʽ�ṹ�ڵ�*/
struct Clause_Node {
    int number;//�����Ӿ��е������������ж��Ƿ�Ϊ���Ӿ�
    int tag;//�����־��Ƿ񱻴���
    Literal_List first_literal;//�������ֽڵ㣬ָ���һ����
    int watch_literal[2];//���屻���ӽڵ�
    struct Clause_Node* next_clause;//�����־�ڵ㣬ָ����һ�Ӿ�
};

/*����������ʽ�ṹ�ڵ�*/
struct Literal_Node {
    int literal;//������������
    int tag;//���������Ƿ񱻴���
    struct Literal_Node* next_literal;//������һ���ֽڵ�
    struct Literal_Node* next_same_literal;//������һ��ͬ�ڵ�
    struct Clause_Node* head;//��������ֶ�Ӧ�Ӿ����
};

/*����CNF��ʽ�ṹ��㣬�洢CNF��Ϣ*/
typedef struct Conjunctive_Normal_Form {
    int literal_num;//����������Ŀ
    int clause_num;//�����Ӿ���Ŀ
    Clause_List first_clause;//�����Ӿ�ͷ�ڵ㣬ָ��CNF�ĵ�һ���Ӿ�
}Conjunctive_Normal_Form, * Conjunctive_Normal_Form_List;

typedef struct ArgueValue {
    int tag;//�Ƿ��ж����
    int is_value;//�ж���ֵ���
    int dep;//ջ�����
    int score;//�ñ����ĵ÷�
    int num;//��������
    int pos;//��������Ŀ
    int nev;//��������Ŀ
    Literal_List first;//���Ӿ�ָ��
}ArgueValue;

/*CNF�ļ���ȡ������*/
status DestroyClause(Clause_List& sentence);//����DestroyClause�����������Ӿ���
status DestroyLiteral(Literal_List& word);//����DestroyLiteral�������������ֽ��
status DestroyCNF(Conjunctive_Normal_Form_List& cnf);//����DestroyCNF����������CNF�ļ�

//func_DPLL1���ú���
status CreateCNF(FILE* fp, Conjunctive_Normal_Form_List& cnf);//����CreateCNF����������CNF������
status CreateClause(FILE* fp, Clause_List& sentence, int first);//����CreateClause�����������Ӿ�����
status DeleteLiteral(Conjunctive_Normal_Form_List& cnf, int literal);//����DeleteLiteral������ɾ��cnf�ṹ�����е�literal����
status RecoverLiteral(Conjunctive_Normal_Form_List& cnf, int literal);//����RecoverLiteral�������ظ��ض�literal����
status update_storevalue(Conjunctive_Normal_Form_List& cnf);//����update_storevalue���������´洢����
status update_storevalue2(Conjunctive_Normal_Form_List& cnf);//����update_storevalue���������´洢����
status DPLL1(Conjunctive_Normal_Form_List cnf, int op);//����DPLL1��������Ϊ����cnf�ļ��ĵ�һ��DPLL�ļ�
int Select_Literal_DPLL0(Conjunctive_Normal_Form_List& cnf);//����Select_Literal�������������߲���
int Select_Literal_DPLL1(Conjunctive_Normal_Form_List& cnf);//����Select_Literal�������������߲���
int Select_Literal_DPLL2(Conjunctive_Normal_Form_List& cnf);//����Select_Literal�������������߲���
int Select_Literal_DPLL3(Conjunctive_Normal_Form_List& cnf);//����Select_Literal�������������߲���
status DPLL1_Partition(Conjunctive_Normal_Form_List& cnf, int literal, int op);//����DPLL1_Partition�������������ѹ���
status DPLL1_Recover(Conjunctive_Normal_Form_List& cnf, int literal);//����DPLL1_Recover�������������ݹ���
Clause_List HasUnitClause(Conjunctive_Normal_Form_List& cnf);//����HasUnitClause����������CNF�Ƿ��е��Ӿ�
status print(Conjunctive_Normal_Form_List& cnf);//����print�����������ֵ��������֤�͵���


//func_DPLL2���ú���
status CreateCNF2(FILE* fp, Conjunctive_Normal_Form_List& cnf);//����CreateCNF����������CNF������
status CreateClause2(FILE* fp, Clause_List& sentence, int first);//����CreateClause�����������Ӿ�����
status DestroyClause2(Clause_List& sentence);//����DestroyClause�����������Ӿ���
status DestroyLiteral2(Literal_List& word);//����DestroyLiteral�������������ֽ��
status DestroyCNF2(Conjunctive_Normal_Form_List& cnf);//����DestroyCNF����������CNF�ļ�
status print2(Conjunctive_Normal_Form_List& cnf);//����print�����������ֵ��������֤�͵���
status DPLL2(Conjunctive_Normal_Form_List cnf);//����DPLL1��������Ϊ����cnf�ļ��ĵ�һ��DPLL�ļ�
status check_print2(Conjunctive_Normal_Form_List& cnf);//����check_print2���������м��

status CreateSudoku();//����CreateSudoku������������������
status Dig_Hole_Easy(int dig);//����Dig_Hole_Easy�������ڶ�����������
status sudokusat(int m, int n, Conjunctive_Normal_Form_List cnf);//����sudokusat�����������������
status SudokuFinalPrint(void);//����SudokuFinalPrint���������������Ϸ
status DPLL2_SUDOKU(Conjunctive_Normal_Form_List cnf, int t[], int top);//����DPLL2_SUDOKU����������SAT�����
int print_time(Conjunctive_Normal_Form_List cnf);//����print_time������������ʱ��

void solve(int op);//����solve�����������������
void solve_cnf();//����solve_cnf���������cnf�ļ���⽻������
void solve_sudoku();//����solve_sudoku���������������⽻������
void store_document(Conjunctive_Normal_Form_List& cnf, char* filename, int d, int time0);//����store_document�������洢�ļ�
void traveser_cnf(Conjunctive_Normal_Form_List cnf);//����traveser_cnf����������CNF�ļ�����
void prove_cnf(Conjunctive_Normal_Form_List cnf);//����prove_cnf������������֤
void show_cnf(Conjunctive_Normal_Form_List cnf);//����show_cnf���������cnf�ļ������
#endif //SUMIT_DEFINE_H
