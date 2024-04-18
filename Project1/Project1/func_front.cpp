//
// Created by hp on 2022-09-09.
//

#include "define.h"
Conjunctive_Normal_Form_List cnf;//����cnf
char filename[100];//�����ļ���


extern int sudoku_table[18][9];//����ȫ�ֱ���int���Ͷ�ά����洢˫��������
extern int users_sudoku[18][9];//�洢����ĺ��ո�����������
extern int shuffle_value[164];//�洢�����ҵı�Ԫ

extern ArgueValue* ValueList;//�����Ԫ��ֵ��

//����solve�����������������
void solve(int op) {
    switch (op) {
    case 1: solve_cnf(); break;
    case 2: solve_sudoku(); break;
    default:break;
    }
    return;
}

//����solve_cnf���������cnf�ļ���⽻������
void solve_cnf() {
    int create_flag = 0, d;
    clock_t start = 0, finish = 0;//��¼DPLL�������õ���ʼ����ֹʱ��
    int duration = 0;//��¼SAT���ʱ��
    system(("cls"));
    printf("��������Ҫ����cnf�ļ������ƣ�");
    scanf("%s", filename);
    FILE* fp;

    if ((fp = fopen(filename, "r")) == NULL) {
        printf("Fail to open file!\n");
        exit(0);  //�˳����򣨽�������
    }

    int op1;

    printf("��ѡ����ⷽ����\n");
    printf("���� 0 ��δ�Ż�ǰ˳��ѡȡ����\n");
    printf("���� 1 �����ŷǵݹ�������\n");
    printf("���� 2 ���ݹ�����Ӿ����Ȳ���\n");
    printf("���� 3 ���ݹ����Ƶ�����Ȳ���\n");
    printf("���� 4 ���ݹ�VSIDSѡȡ����\n");

    scanf("%d", &op1);

    switch (op1) {
    case 0:
        create_flag = CreateCNF(fp, cnf);
        if (!create_flag) {
            printf("Fail to create cnf!");
            exit(0);  //�˳����򣨽�������
        }
        start = clock();
        d = DPLL1(cnf, 0);
        //    check_print2(cnf);
        //    print2(cnf);
        finish = clock();
        duration = (finish - start);

        printf("����ļ���ʱ%dms\n", duration);
        if (d == 1) {
            printf("��cnf�ļ��н⣡\n");
        }
        else
            printf("��cnf�ļ��޽⣡\n");
        break;
    case 1:
        create_flag = CreateCNF2(fp, cnf);
        if (!create_flag) {
            printf("Fail to create cnf!");
            exit(0);  //�˳����򣨽�������
        }
        start = clock();
        d = DPLL2(cnf);
        //    check_print2(cnf);
        //    print2(cnf);
        finish = clock();
        duration = (finish - start);

        printf("����ļ���ʱ%dms\n", duration);
        if (d == 1) {
            printf("��cnf�ļ��н⣡\n");
        }
        else
            printf("��cnf�ļ��޽⣡\n");
        break;
    case 2:
        create_flag = CreateCNF(fp, cnf);
        if (!create_flag) {
            printf("Fail to create cnf!");
            exit(0);  //�˳����򣨽�������
        }
        start = clock();
        d = DPLL1(cnf, 1);
        //    check_print2(cnf);
        //    print2(cnf);
        finish = clock();
        duration = (finish - start);

        printf("����ļ���ʱ%dms\n", duration);
        if (d == 1) {
            printf("��cnf�ļ��н⣡\n");
        }
        else
            printf("��cnf�ļ��޽⣡\n");
        break;
    case 3:
        create_flag = CreateCNF(fp, cnf);
        if (!create_flag) {
            printf("Fail to create cnf!");
            exit(0);  //�˳����򣨽�������
        }
        start = clock();
        d = DPLL1(cnf, 2);
        //    check_print2(cnf);
        //    print2(cnf);
        finish = clock();
        duration = (finish - start);

        printf("����ļ���ʱ%dms\n", duration);
        if (d == 1) {
            printf("��cnf�ļ��н⣡\n");
        }
        else
            printf("��cnf�ļ��޽⣡\n");
        break;
    case 4:
        create_flag = CreateCNF(fp, cnf);
        if (!create_flag) {
            printf("Fail to create cnf!");
            exit(0);  //�˳����򣨽�������
        }
        start = clock();
        d = DPLL1(cnf, 3);
        //    check_print2(cnf);
        //    print2(cnf);
        finish = clock();
        duration = (finish - start);

        printf("����ļ���ʱ%dms\n", duration);
        if (d == 1) {
            printf("��cnf�ļ��н⣡\n");
        }
        else
            printf("��cnf�ļ��޽⣡\n");
        break;
    default:
        printf("ѡȡ������Ч������ȷѡȡ��\n");
        break;
    }
    fclose(fp);
    printf("��ѡ���Ƿ񱣴���ļ���y/n����");
    char s[10];
    scanf("%s", s);
    if (s[0] == 'y') {
        store_document(cnf, filename, d, duration);
    }
    int op2 = 1;
    while (op2) {
        system("cls");
        printf("ѡ��鿴��Ϣ��\n");
        printf("1.����cnf�ļ�\n");
        printf("2.��֤�����ȷ��\n");
        printf("3.�鿴��Ľṹ\n");
        printf("0.�˳�\n\n");
        
        printf("��ѡ����Ҫ�鿴����Ϣ��");
        scanf("%d", &op2);
        switch (op2) {
        case 1:traveser_cnf(cnf); getchar(); getchar(); break;
        case 2:prove_cnf(cnf); getchar(); getchar(); break;
        case 3:show_cnf(cnf); getchar(); getchar(); break;
        case 0:break;
        default:printf("����ȷѡ��鿴��Ϣ�������밴0�˳�"); getchar(); getchar(); break;
        }
    }
    free(ValueList);
    DestroyCNF(cnf);

}

//����solve_sudoku���������������⽻������
void solve_sudoku() {
    char c, v;

    initgraph(1024, 684);	// ������ͼ���ڣ���СΪ 640x480 ����

    settextstyle(16, 8, _T("Courier"));	// ��������
    // ������ɫ
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
    //��ӭ�������ݽṹ����������
    settextstyle(45, 20, _T("΢���ź�"));
    outtextxy(230, 300, _T("��ӭ�������ݽṹ���������� "));

    wchar_t str0[] = L"  �밴ENTER����ʼ";
    settextstyle(45, 20, _T("΢���ź�"));
    outtextxy(330, 400, _T(" �밴ENTER����ʼ"));

    char s0;

    while (TRUE) {
        s0 = _getch();
        if (s0 == 13)
            break;
    }
    fillrectangle(0, 0, 1024, 700);

    settextstyle(50, 25, _T("΢���ź�"));
    outtextxy(185, 280, _T("��ѡ���Ѷȣ�"));
    settextstyle(40, 18, _T("����"));
    outtextxy(525, 150, _T("�򵥣��밴������ 1��"));
    outtextxy(525, 300, _T("�еȣ��밴������ 2��"));
    outtextxy(525, 450, _T("���ӣ��밴������ 3��"));

    int dig_slt = 0;

    while (TRUE) {
        s0 = _getch();
        if (s0 == 49) {
            dig_slt = 1;
            outtextxy(325, 550, _T("���ڼ����С���"));
            break;
        }
        if (s0 == 50) {
            dig_slt = 2;
            outtextxy(325, 550, _T("���ڼ����С���"));
            break;
        }
        if (s0 == 51) {
            dig_slt = 3;
            outtextxy(325, 550, _T("���ڼ����С���"));
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

            _stprintf(s, _T("%d"), sudoku_table[i + 8][j - 1]);		// �߰汾 VC �Ƽ�ʹ�� _stprintf_s ����
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
            _stprintf(s, _T("%d"), sudoku_table[i - 1][j - 1]);		// �߰汾 VC �Ƽ�ʹ�� _stprintf_s ����
            settextstyle(29, 21, s);

            if (users_sudoku[i - 1][j - 1] == 1)
                outtextxy(16 + 35 * j, 13 + 35 * i, o);
            else {
                outtextxy(16 + 35 * j, 13 + 35 * i, s);
            }
                
        }
    for (int i = 1; i <= 9; i++)
        for (int j = 1; j <= 9; j++) {

            _stprintf(s, _T("%d"), sudoku_table[i + 8][j - 1]);		// �߰汾 VC �Ƽ�ʹ�� _stprintf_s ����
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
    outtextxy(712, 32, _T(" ˫������Ϸ"));
    int x, y, l, r, a = 0, b = 0;
    ExMessage m, n;		// ������Ϣ����
    settextcolor(BLACK);
    start = clock();
    end = clock();
    _stprintf(s, _T("%d"), 0);		// �߰汾 VC �Ƽ�ʹ�� _stprintf_s ����
    settextstyle(29, 21, s);
    outtextxy(832, 110, s);
    wchar_t str2[] = L"  �������:";
    settextstyle(15, 12, s);
    outtextxy(712, 250, _T(" ��Ϸ˵����"));
    outtextxy(610, 275, _T(" ������Ӧ�ո�׼���������֣�"));
    outtextxy(610, 325, _T(" ������ͨ�����ּ������룻"));
    outtextxy(610, 350, _T(" �������ʱ��ʾW��WRONG����"));
    outtextxy(610, 375, _T(" �� ESC ���˳���"));
    outtextxy(610, 400, _T(" ���ע�Լ��Ĵ��������"));
    outtextxy(610, 425, _T(" ����15�λ��Զ��˳���"));
    outtextxy(610, 450, _T(" ף����Ϸ��죡����"));
    settextstyle(29, 21, s);
    o = _T('W');
    settextcolor(RED);
    int wrong_times = 0, flag = 0;
    while (true)
    {
        // ��ȡһ�����򰴼���Ϣ
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
                    // ��һ���󷽿�
                    rectangle(m.x - 5, m.y - 5, m.x + 5, m.y + 5);
                else
                    // ��һ��С����
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
                    // ��һ���󷽿�
                    rectangle(m.x - 5, m.y - 5, m.x + 5, m.y + 5);
                else
                    // ��һ��С����
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
                    _stprintf(s, _T("%d"), wrong_times);		// �߰汾 VC �Ƽ�ʹ�� _stprintf_s ����
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
                    _stprintf(s, _T("%d"), wrong_times);		// �߰汾 VC �Ƽ�ʹ�� _stprintf_s ����
                    outtextxy(832, 110, s);
                }
            }
            break;
        }
        if (flag) break;
        if (wrong_times > 15) break;
    }
    _getch();				// �����������
    closegraph();			// �رջ�ͼ����
}