//
// Created by hp on 2022-09-09.
//

#include "define.h"
Conjunctive_Normal_Form_List cnf;//定义cnf
char filename[100];//定义文件名


extern int sudoku_table[18][9];//定义全局变量int类型二维数组存储双数独终盘
extern int users_sudoku[18][9];//存储输出的含空格数独问题格局
extern int shuffle_value[164];//存储被打乱的变元

extern ArgueValue* ValueList;//定义变元真值表

//定义solve函数，输出交互界面
void solve(int op) {
    switch (op) {
    case 1: solve_cnf(); break;
    case 2: solve_sudoku(); break;
    default:break;
    }
    return;
}

//定义solve_cnf函数，输出cnf文件求解交互界面
void solve_cnf() {
    int create_flag = 0, d;
    clock_t start = 0, finish = 0;//记录DPLL函数调用的起始和终止时间
    int duration = 0;//记录SAT求解时间
    system(("cls"));
    printf("请输入想要求解的cnf文件的名称：");
    scanf("%s", filename);
    FILE* fp;

    if ((fp = fopen(filename, "r")) == NULL) {
        printf("Fail to open file!\n");
        exit(0);  //退出程序（结束程序）
    }

    int op1;

    printf("请选择求解方案：\n");
    printf("方案 0 ：未优化前顺序选取策略\n");
    printf("方案 1 ：最优非递归求解策略\n");
    printf("方案 2 ：递归最短子句优先策略\n");
    printf("方案 3 ：递归最大频率优先策略\n");
    printf("方案 4 ：递归VSIDS选取策略\n");

    scanf("%d", &op1);

    switch (op1) {
    case 0:
        create_flag = CreateCNF(fp, cnf);
        if (!create_flag) {
            printf("Fail to create cnf!");
            exit(0);  //退出程序（结束程序）
        }
        start = clock();
        d = DPLL1(cnf, 0);
        //    check_print2(cnf);
        //    print2(cnf);
        finish = clock();
        duration = (finish - start);

        printf("求解文件用时%dms\n", duration);
        if (d == 1) {
            printf("该cnf文件有解！\n");
        }
        else
            printf("该cnf文件无解！\n");
        break;
    case 1:
        create_flag = CreateCNF2(fp, cnf);
        if (!create_flag) {
            printf("Fail to create cnf!");
            exit(0);  //退出程序（结束程序）
        }
        start = clock();
        d = DPLL2(cnf);
        //    check_print2(cnf);
        //    print2(cnf);
        finish = clock();
        duration = (finish - start);

        printf("求解文件用时%dms\n", duration);
        if (d == 1) {
            printf("该cnf文件有解！\n");
        }
        else
            printf("该cnf文件无解！\n");
        break;
    case 2:
        create_flag = CreateCNF(fp, cnf);
        if (!create_flag) {
            printf("Fail to create cnf!");
            exit(0);  //退出程序（结束程序）
        }
        start = clock();
        d = DPLL1(cnf, 1);
        //    check_print2(cnf);
        //    print2(cnf);
        finish = clock();
        duration = (finish - start);

        printf("求解文件用时%dms\n", duration);
        if (d == 1) {
            printf("该cnf文件有解！\n");
        }
        else
            printf("该cnf文件无解！\n");
        break;
    case 3:
        create_flag = CreateCNF(fp, cnf);
        if (!create_flag) {
            printf("Fail to create cnf!");
            exit(0);  //退出程序（结束程序）
        }
        start = clock();
        d = DPLL1(cnf, 2);
        //    check_print2(cnf);
        //    print2(cnf);
        finish = clock();
        duration = (finish - start);

        printf("求解文件用时%dms\n", duration);
        if (d == 1) {
            printf("该cnf文件有解！\n");
        }
        else
            printf("该cnf文件无解！\n");
        break;
    case 4:
        create_flag = CreateCNF(fp, cnf);
        if (!create_flag) {
            printf("Fail to create cnf!");
            exit(0);  //退出程序（结束程序）
        }
        start = clock();
        d = DPLL1(cnf, 3);
        //    check_print2(cnf);
        //    print2(cnf);
        finish = clock();
        duration = (finish - start);

        printf("求解文件用时%dms\n", duration);
        if (d == 1) {
            printf("该cnf文件有解！\n");
        }
        else
            printf("该cnf文件无解！\n");
        break;
    default:
        printf("选取策略无效，请正确选取！\n");
        break;
    }
    fclose(fp);
    printf("请选择是否保存解文件【y/n】：");
    char s[10];
    scanf("%s", s);
    if (s[0] == 'y') {
        store_document(cnf, filename, d, duration);
    }
    int op2 = 1;
    while (op2) {
        system("cls");
        printf("选择查看信息：\n");
        printf("1.遍历cnf文件\n");
        printf("2.验证解的正确性\n");
        printf("3.查看解的结构\n");
        printf("0.退出\n\n");
        
        printf("请选择想要查看的信息：");
        scanf("%d", &op2);
        switch (op2) {
        case 1:traveser_cnf(cnf); getchar(); getchar(); break;
        case 2:prove_cnf(cnf); getchar(); getchar(); break;
        case 3:show_cnf(cnf); getchar(); getchar(); break;
        case 0:break;
        default:printf("请正确选择查看信息，否则请按0退出"); getchar(); getchar(); break;
        }
    }
    free(ValueList);
    DestroyCNF(cnf);

}

//定义solve_sudoku函数，输出数独求解交互界面
void solve_sudoku() {
    char c, v;

    initgraph(1024, 684);	// 创建绘图窗口，大小为 640x480 像素

    settextstyle(16, 8, _T("Courier"));	// 设置字体
    // 设置颜色
    settextcolor(BLACK);
    setlinecolor(BLACK);
    setlinestyle(PS_SOLID, 2);
    fillrectangle(0, 0, 1024, 700);

    setfillcolor(WHITE);
    setlinecolor(BLACK);

    setbkcolor(WHITE);

    IMAGE picture;

    loadimage(&picture, "c.jpg", 1024, 700);

    putimage(0, 0, &picture);

    loadimage(&picture, "d.jpg", 250, 254);

    putimage(774, 446, &picture);
    //欢迎进入数据结构的数独世界
    settextstyle(45, 20, _T("微软雅黑"));
    outtextxy(230, 300, _T("欢迎进入数据结构的数独世界 "));

    wchar_t str0[] = L"  请按ENTER键开始";
    settextstyle(45, 20, _T("微软雅黑"));
    outtextxy(330, 400, _T(" 请按ENTER键开始"));

    char s0;

    while (TRUE) {
        s0 = _getch();
        if (s0 == 13)
            break;
    }
    fillrectangle(0, 0, 1024, 700);

    settextstyle(50, 25, _T("微软雅黑"));
    outtextxy(185, 280, _T("请选择难度："));
    settextstyle(40, 18, _T("楷体"));
    outtextxy(525, 150, _T("简单（请按数字盘 1）"));
    outtextxy(525, 300, _T("中等（请按数字盘 2）"));
    outtextxy(525, 450, _T("复杂（请按数字盘 3）"));

    int dig_slt = 0;

    while (TRUE) {
        s0 = _getch();
        if (s0 == 49) {
            dig_slt = 1;
            outtextxy(325, 550, _T("正在加载中……"));
            break;
        }
        if (s0 == 50) {
            dig_slt = 2;
            outtextxy(325, 550, _T("正在加载中……"));
            break;
        }
        if (s0 == 51) {
            dig_slt = 3;
            outtextxy(325, 550, _T("正在加载中……"));
            break;
        }
    }

    CreateSudoku();
    Dig_Hole_Easy(dig_slt);

    fillrectangle(0, 0, 1024, 700);
    TCHAR o = _T(' ');
    for (int i = 1; i <= 9; i++)
        for (int j = 1; j <= 9; j++) {
            if (users_sudoku[i - 1][j - 1] != 1) {
                setfillcolor(YELLOW);
                fillrectangle(10 + 35 * j, 10 + 35 * i, 45 + 35 * j, 45 + 35 * i);
            }
            else {
                setfillcolor(WHITE);
                fillrectangle(10 + 35 * j, 10 + 35 * i, 45 + 35 * j, 45 + 35 * i);
            }

            RECT r = { 10 + 35 * j, 10 + 35 * i, 45 + 35 * j, 45 + 35 * i };
            TCHAR s[5];
        }
    for (int i = 1; i <= 9; i++)
        for (int j = 1; j <= 9; j++) {
            if (users_sudoku[i + 8][j - 1] != 1) {
                setfillcolor(YELLOW);
                fillrectangle(10 + 35 * (6 + j), 10 + 35 * (6 + i), 45 + 35 * (6 + j), 45 + 35 * (6 + i));
            }
            else {
                setfillcolor(WHITE);
                fillrectangle(10 + 35 * (6 + j), 10 + 35 * (6 + i), 45 + 35 * (6 + j), 45 + 35 * (6 + i));
            }
            TCHAR s[5];

            _stprintf(s, _T("%d"), sudoku_table[i + 8][j - 1]);		// 高版本 VC 推荐使用 _stprintf_s 函数
            settextstyle(29, 21, s);
            outtextxy(16 + 35 * (6 + i), 13 + 35 * (6 + j), s);
            outtextxy(16 + 35 * (6 + j), 13 + 35 * (6 + i), s);
        }

    TCHAR s[50];
    clock_t end;
    clock_t start, finish;
    int duration;
    for (int i = 1; i <= 9; i++)
        for (int j = 1; j <= 9; j++) {
            _stprintf(s, _T("%d"), sudoku_table[i - 1][j - 1]);		// 高版本 VC 推荐使用 _stprintf_s 函数
            settextstyle(29, 21, s);

            if (users_sudoku[i - 1][j - 1] == 1)
                outtextxy(16 + 35 * j, 13 + 35 * i, o);
            else {
                outtextxy(16 + 35 * j, 13 + 35 * i, s);
            }
                
        }
    for (int i = 1; i <= 9; i++)
        for (int j = 1; j <= 9; j++) {

            _stprintf(s, _T("%d"), sudoku_table[i + 8][j - 1]);		// 高版本 VC 推荐使用 _stprintf_s 函数
            settextstyle(29, 21, s);

            if (users_sudoku[i + 8][j - 1] == 1)
                outtextxy(16 + 35 * (6 + j), 13 + 35 * (6 + i), o);
            else {
                outtextxy(16 + 35 * (6 + j), 13 + 35 * (6 + i), s);
            }
        }
    wchar_t str1[] = L"  SUDOKU";
    fillrectangle(710, 15, 900, 150);
    settextstyle(18, 16, s);
    outtextxy(712, 32, _T(" 双数独游戏"));
    int x, y, l, r, a = 0, b = 0;
    ExMessage m, n;		// 定义消息变量
    settextcolor(BLACK);
    start = clock();
    end = clock();
    _stprintf(s, _T("%d"), 0);		// 高版本 VC 推荐使用 _stprintf_s 函数
    settextstyle(29, 21, s);
    outtextxy(832, 110, s);
    wchar_t str2[] = L"  错误次数:";
    settextstyle(15, 12, s);
    outtextxy(712, 250, _T(" 游戏说明："));
    outtextxy(610, 275, _T(" 请点击对应空格准备填入数字；"));
    outtextxy(610, 325, _T(" 数字请通过数字键盘输入；"));
    outtextxy(610, 350, _T(" 输入错误时显示W（WRONG）；"));
    outtextxy(610, 375, _T(" 按 ESC 键退出；"));
    outtextxy(610, 400, _T(" 请关注自己的错误次数，"));
    outtextxy(610, 425, _T(" 超过15次会自动退出。"));
    outtextxy(610, 450, _T(" 祝你游戏愉快！！！"));
    settextstyle(29, 21, s);
    o = _T('W');
    settextcolor(RED);
    int wrong_times = 0, flag = 0;
    while (true)
    {
        // 获取一条鼠标或按键消息
        m = getmessage(EX_MOUSE | EX_KEY);
        switch (m.message)
        {
        case WM_LBUTTONDOWN:
            a = m.x, b = m.y;
            if (a > 45 && a < 360 && b > 45 && b < 360) {
                x = 0, y = 0;
                l = 0, r = 0;
                while (x < a) {
                    l++;
                    x = 10 + 35 * l;
                }
                while (y < b) {
                    r++;
                    y = 10 + 35 * r;
                }
                if (users_sudoku[r - 2][l - 2] != 1)
                    break;
                if (m.ctrl)
                    // 画一个大方块
                    rectangle(m.x - 5, m.y - 5, m.x + 5, m.y + 5);
                else
                    // 画一个小方块
                    rectangle(m.x - 2, m.y - 2, m.x + 2, m.y + 2);
            }
            if (a > 255 && a < 360 && b > 255 && b < 360) {
                break;
            }
            if (a > 255 && a < 570 && b > 255 && b < 570) {
                x = 0, y = 0;
                l = 6, r = 6;
                while (x < a) {
                    l++;
                    x = 10 + 35 * l;
                }
                while (y < b) {
                    r++;
                    y = 10 + 35 * r;
                }
                if (users_sudoku[r - 2 - 6 + 9][l - 2 - 6] != 1)
                    break;
                if (m.ctrl)
                    // 画一个大方块
                    rectangle(m.x - 5, m.y - 5, m.x + 5, m.y + 5);
                else
                    // 画一个小方块
                    rectangle(m.x - 2, m.y - 2, m.x + 2, m.y + 2);
            }
            break;

        case WM_KEYDOWN:
            if (m.vkcode == VK_ESCAPE) {
                flag = 1;
                break;
            }
                
            if (a > 45 && a < 360 && b > 45 && b < 360) {
                x = 0, y = 0;
                l = 0, r = 0;
                while (x < a) {
                    l++;
                    x = 10 + 35 * l;
                }
                while (y < b) {
                    r++;
                    y = 10 + 35 * r;
                }
                c = sudoku_table[r - 2][l - 2] + 48;
                v = _getch();
                if (v == c) {
                    outtextxy(16 + 35 * (l - 1), 13 + 35 * (r - 1), c);
                }
                else {
                    outtextxy(16 + 35 * (l - 1), 13 + 35 * (r - 1), o);
                    wrong_times++;
                    _stprintf(s, _T("%d"), wrong_times);		// 高版本 VC 推荐使用 _stprintf_s 函数
                    outtextxy(832, 110, s);
                }
            }
            if (a > 255 && a < 360 && b > 255 && b < 360) {
                break;
            }
            if (a > 255 && a < 570 && b > 255 && b < 570) {
                x = 0, y = 0;
                l = 6, r = 6;
                while (x < a) {
                    l++;
                    x = 10 + 35 * l;
                }
                while (y < b) {
                    r++;
                    y = 10 + 35 * r;
                }
                c = sudoku_table[r - 2 - 6 + 9][l - 2 - 6] + 48;
                v = _getch();
                if (v == c) {
                    outtextxy(16 + 35 * (l - 1), 13 + 35 * (r - 1), c);
                }
                else {
                    outtextxy(16 + 35 * (l - 1), 13 + 35 * (r - 1), o);
                    wrong_times++;
                    _stprintf(s, _T("%d"), wrong_times);		// 高版本 VC 推荐使用 _stprintf_s 函数
                    outtextxy(832, 110, s);
                }
            }
            break;
        }
        if (flag) break;
        if (wrong_times > 15) break;
    }
    _getch();				// 按任意键继续
    closegraph();			// 关闭绘图窗口
}