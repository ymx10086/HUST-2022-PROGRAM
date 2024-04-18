//
// Created by hp on 2022-09-04.
//


#include "define.h"

int sudoku_table[18][9];//定义全局变量int类型二维数组存储双数独终盘
int users_sudoku[18][9];//存储输出的含空格数独问题格局
int shuffle_value[164];//存储被打乱的变元
extern ArgueValue* ValueList;//定义变元真值表
enum { easy = 1, medium, difficult };
int easynum = 50, midnum = 80, difnum = 110;

//定义CreatePreSudokuFile函数，规约数独为CNF文件
FILE* CreatePreSudokuFile(Conjunctive_Normal_Form_List& cnf) {
    int x, y, z, i, j, k, l;
    FILE* fp;
    fp = fopen("SudokuTableBase.cnf", "w");
    if (fp == NULL) {
        printf("File open error!\n");
        exit(0);
    }
    fprintf(fp, "p cnf 1458 20736\n");//共有729个变元，9*9个数独空格每个格对应9个变元，填入1～9中某一值则对应变元为真，其他为假

    //每个格内只能含有1-9中的一个
    for (x = 0; x < 9; x++) {
        for (y = 0; y < 9; y++)
            for (z = 1; z < 9; z++)
                for (i = z + 1; i <= 9; i++)
                    fprintf(fp, "%d %d 0\n", -(81 * x + 9 * y + z), -(81 * x + 9 * y + i));
    }
    for (x = 9; x < 18; x++) {
        for (y = 0; y < 9; y++)
            for (z = 1; z < 9; z++)
                for (i = z + 1; i <= 9; i++)
                    fprintf(fp, "%d %d 0\n", -(81 * x + 9 * y + z), -(81 * x + 9 * y + i));
    }
    //每一行1-9只出现一次
    for (x = 0; x < 9; x++) {
        for (z = 1; z <= 9; z++)
            for (y = 0; y < 8; y++)
                for (i = y + 1; i <= 8; i++)
                    fprintf(fp, "%d %d 0\n", -(81 * x + 9 * y + z), -(81 * x + 9 * i + z));
    }

    for (x = 9; x < 18; x++) {
        for (z = 1; z <= 9; z++)
            for (y = 0; y < 8; y++)
                for (i = y + 1; i <= 8; i++)
                    fprintf(fp, "%d %d 0\n", -(81 * x + 9 * y + z), -(81 * x + 9 * i + z));
    }
    //每一列1-9只出现一次
    for (y = 0; y < 9; y++) {
        for (z = 1; z <= 9; z++)
            for (x = 0; x < 8; x++)
                for (i = x + 1; i <= 8; i++)
                    fprintf(fp, "%d %d 0\n", -(81 * x + 9 * y + z), -(81 * i + 9 * y + z));
    }

    for (y = 0; y < 9; y++) {
        for (z = 1; z <= 9; z++)
            for (x = 9; x < 17; x++)
                for (i = x + 1; i <= 17; i++)
                    fprintf(fp, "%d %d 0\n", -(81 * x + 9 * y + z), -(81 * i + 9 * y + z));
    }
    //每一宫1-9只出现一次
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++)
            for (z = 1; z <= 9; z++)
                for (x = 0; x < 3; x++)
                    for (y = 0; y < 3; y++)
                        for (k = x + 1; k < 3; k++)
                            for (l = 0; l < 3; l++)
                                if (y != l)
                                    fprintf(fp, "%d %d 0\n", -(81 * (3 * i + x) + 9 * (3 * j + y) + z),
                                        -(81 * (3 * i + k) + 9 * (3 * j + l) + z));
    }

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++)
            for (z = 1; z <= 9; z++)
                for (x = 0; x < 3; x++)
                    for (y = 0; y < 3; y++)
                        for (k = x + 1; k < 3; k++)
                            for (l = 0; l < 3; l++)
                                if (y != l)
                                    fprintf(fp, "%d %d 0\n", -(729 + 81 * (3 * i + x) + 9 * (3 * j + y) + z),
                                        -(729 + 81 * (3 * i + k) + 9 * (3 * j + l) + z));
    }
    //每一格一定要有1-9中的一个数字
    for (x = 0; x < 9; x++) {
        for (y = 0; y < 9; y++) {
            for (z = 1; z <= 9; z++)
                fprintf(fp, "%d ", 81 * x + 9 * y + z);
            fprintf(fp, "0\n");
        }
    }

    for (x = 9; x < 18; x++) {
        for (y = 0; y < 9; y++) {
            for (z = 1; z <= 9; z++)
                fprintf(fp, "%d ", 81 * x + 9 * y + z);
            fprintf(fp, "0\n");
        }
    }
    //双数独重叠部分等价关系
    for (x = 6; x < 9; x++) {
        for (y = 6; y < 9; y++)
            for (z = 1; z <= 9; z++) {
                fprintf(fp, "%d %d 0\n%d %d 0\n", -(81 * x + 9 * y + z), (729 + 81 * (x - 6) + 9 * (y - 6) + z),
                    (81 * x + 9 * y + z), -(729 + 81 * (x - 6) + 9 * (y - 6) + z));
            }
    }
    fclose(fp);
    fp = fopen("SudokuTableBase.cnf", "r");
    return fp;
}

void swap(int& x, int& y) {
    int tmp = x;
    x = y;
    y = tmp;
    return;
}

//定义CreateSudoku函数，创建数独终盘
status CreateSudoku() {
    srand(int(time(0)));
    int i, j, k;
    for (i = 0; i < 9; i++) {
        sudoku_table[0][i] = i + 1;
    }
    for (i = 0; i < 9; i++) {
        j = rand() % 9;
        if (i != j) {
            swap(sudoku_table[0][i], sudoku_table[0][j]);
        }
    }
    int move[] = { 0, 3, 6, 1, 4, 7, 2, 5, 8 };
    for (j = 1; j < 9; j++) {
        int tmp[9], tag = 0;
        for (i = 8 - move[j] + 1; i <= 8; i++) {
            tmp[tag++] = sudoku_table[0][i];
        }
        for (i = 0; i <= 8 - move[j]; i++) {
            sudoku_table[j][i + move[j]] = sudoku_table[0][i];
        }
        for (i = 0; i < tag; i++) {
            sudoku_table[j][i] = tmp[i];
        }
    }
    sudoku_table[9][0] = sudoku_table[6][6], sudoku_table[9][1] = sudoku_table[6][7], sudoku_table[9][2] = sudoku_table[6][8];
    sudoku_table[9][3] = sudoku_table[8][6], sudoku_table[9][4] = sudoku_table[8][7], sudoku_table[9][5] = sudoku_table[8][8];
    sudoku_table[9][6] = sudoku_table[7][6], sudoku_table[9][7] = sudoku_table[7][7], sudoku_table[9][8] = sudoku_table[7][8];
    for (j = 10; j < 18; j++) {
        int tmp[9], tag = 0;
        for (i = 8 - move[j - 9] + 1; i <= 8; i++) {
            tmp[tag++] = sudoku_table[9][i];
        }
        for (i = 0; i <= 8 - move[j - 9]; i++) {
            sudoku_table[j][i + move[j - 9]] = sudoku_table[9][i];
        }
        for (i = 0; i < tag; i++) {
            sudoku_table[j][i] = tmp[i];
        }
    }
    return OK;
}

extern int prehandle_stack[MAXN];
extern int pre_top, dec_top;

//定义DPLL2_SUDOKU函数，调用SAT求解器
status DPLL2_SUDOKU(Conjunctive_Normal_Form_List cnf, int t[], int top) {
    //    printf("%d", top);
    pre_top = 0;
    dec_top = 0;
    int i;
    for (i = 0; i < top; i++) {
        prehandle_stack[pre_top++] = t[i];
    }
    //    printf("pre_top%d", pre_top);
    //    printf("lllll%dllll", cnf->first_clause->watch_literal[0]);
    return DPLL2(cnf);
}

//定义sudokusat函数，求解数独问题
status sudokusat(int m, int n, Conjunctive_Normal_Form_List cnf) {
    int x, y, z, t, i;
    int tmp[2000], tmp0, ans = 0;
    for (t = 1; t <= 9; t++) {
        tmp0 = 0;
        pre_top = 0, dec_top = 0;
        for (x = 0; x < 18; x++)
            for (y = 0; y < 9; y++) {
                if (users_sudoku[x][y] != 1) {
                    for (z = 1; z <= 9; z++) {
                        if (z == sudoku_table[x][y]) {
                            tmp[tmp0++] = 81 * x + 9 * y + z;
                        }
                        else {
                            tmp[tmp0++] = -(81 * x + 9 * y + z);
                        }
                    }
                }
            }
        for (z = 1; z <= 9; z++) {
            if (z == t) {
                tmp[tmp0++] = 81 * m + 9 * n + z;
            }
            else {
                tmp[tmp0++] = -(81 * m + 9 * n + z);
            }
        }
        ans += DPLL2_SUDOKU(cnf, tmp, tmp0);
        for (i = 1; i <= cnf->literal_num; i++) {
            ValueList[i].is_value = 0;
            ValueList[i].tag = 0;
            ValueList[i].dep = 0;
            ValueList[i].num = 0;
            ValueList[i].nev = 0;
            ValueList[i].pos = 0;
            ValueList[i].score = 0;
        }
        //        printf("%d ", ans);
        if (ans > 1) return FALSE;
    }
    return true;
}

//定义Dig_Hole_Easy函数，挖洞法生成数独
status Dig_Hole_Easy(int dig) {
    int dig_num = dig * 45;
    srand(int(time(0)));
    int x, y, ans, randnum, d, cnt = 1, i, b, z;
    Conjunctive_Normal_Form_List cnf;
    FILE* fp = CreatePreSudokuFile(cnf);
    CreateCNF2(fp, cnf);
    for (x = 0; x < 18; x++)
        for (y = 0; y < 9; y++)
            users_sudoku[x][y] = 0;
    for (i = 1; i <= 162; i++)
        shuffle_value[i] = i;
    for (i = 162; i > 2; i--) {
        randnum = rand() % i + 1;
        if (i != randnum) {
            d = shuffle_value[i];
            shuffle_value[i] = shuffle_value[randnum];
            shuffle_value[randnum] = d;
        }
    }
    for (i = 1; i <= dig_num && cnt < 162; i++) {
        x = (shuffle_value[cnt] - 1) / 9;
        y = (shuffle_value[cnt] - 1) % 9;
        //        x = rand() % 18;
        //        y = rand() % 9;
        cnt++;
        if (users_sudoku[x][y] != -1 && users_sudoku[x][y] != 1) {
            users_sudoku[x][y] = 1;
            if (x >= 6 && x <= 8 && y >= 6 && y <= 8) {
                users_sudoku[x + 3][y - 6] = 1;
            }
            else if (x >= 9 && x <= 11 && y >= 0 && y <= 2) {
                users_sudoku[x - 3][y + 6] = 1;
            }
            ans = sudokusat(x, y, cnf);

            if (!ans) {
                users_sudoku[x][y] = -1;
                if (x >= 6 && x <= 8 && y >= 6 && y <= 8) {
                    users_sudoku[x + 3][y - 6] = -1;
                }
                else if (x >= 9 && x <= 11 && y >= 0 && y <= 2) {
                    users_sudoku[x - 3][y + 6] = -1;
                }
                i--;
                continue;
            }
            continue;
        }
        i--;
    }
    print_time(cnf);
    return OK;
}

//定义print_time函数，输出求解时间
int print_time(Conjunctive_Normal_Form_List cnf) {
    clock_t start, end;

    int x, y, z, t, i;
    int tmp[2000], tmp0, ans = 0;
    tmp0 = 0;
    pre_top = 0, dec_top = 0;
    for (x = 0; x < 18; x++)
        for (y = 0; y < 9; y++) {
            if (users_sudoku[x][y] != 1) {
                for (z = 1; z <= 9; z++) {
                    if (z == sudoku_table[x][y]) {
                        tmp[tmp0++] = 81 * x + 9 * y + z;
                    }
                    else {
                        tmp[tmp0++] = -(81 * x + 9 * y + z);
                    }
                }
            }
        }
    start = clock();
    ans += DPLL2_SUDOKU(cnf, tmp, tmp0);
    end = clock();
    for (i = 1; i <= cnf->literal_num; i++) {
        ValueList[i].is_value = 0;
        ValueList[i].tag = 0;
        ValueList[i].dep = 0;
        ValueList[i].num = 0;
        ValueList[i].nev = 0;
        ValueList[i].pos = 0;
        ValueList[i].score = 0;
    }
    printf("求解数独所用时间：%dms\n", end - start);
    return true;
}

//定义print_time函数，输出求解时间
status SudokuFinalPrint(void) {
    int x, y;
    for (x = 0; x < 18; x++) {
        printf("\n");
        for (y = 0; y < 9; y++) {
            if (users_sudoku[x][y] == 1)
                printf("0 ");
            else
                printf("%d ", sudoku_table[x][y]);
        }
    }
    return 0;
}

