#include "define.h"

/*定义有效的全局变量*/
//char filename[100];//定义文件名
//Conjunctive_Normal_Form_List cnf;//定义cnf
extern ArgueValue* ValueList;//定义变元真值表
int op;

extern int sudoku_table[18][9];//定义全局变量int类型二维数组存储双数独终盘
extern int users_sudoku[18][9];//存储输出的含空格数独问题格局
extern int shuffle_value[164];//存储被打乱的变元
using namespace std;

int main() {

    while (true) {
        system("cls");
        printf("可选功能：\n");
        printf("1.cnf文件求解\n");
        printf("2.SUDOKU游戏\n");
        printf("0.退出\n\n");
        printf("请选择你想要实现的功能：");
        scanf("%d", &op);
        if (op == 0)
            break;
        solve(op);
        getchar(); getchar();
    }

    return 0;
}