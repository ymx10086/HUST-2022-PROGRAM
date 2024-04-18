//
// Created by hp on 2022-09-01.
//

#include "define.h"

/*������Ч��ȫ�ֱ���*/
extern ArgueValue* ValueList;//�����Ԫ��ֵ��
bool isConflict;
int prehandle_stack[MAXN], pre_top;
int decision_stack[MAXN], dec_top;
int decision_num[MAXN];
int conflict_stack_[MAXN], con_top;
double time_sum;

int cnt = 0, times = 1, cnt0 = 0;

//����CreateCNF2����������CNF������
status CreateCNF2(FILE* fp, Conjunctive_Normal_Form_List& cnf) {
    isConflict = FALSE;
    char readfile[20];//�����ַ����������¼���ļ���
    int i, tmp;//���ͱ�������
    Clause_List clause_tmp1, clause_tmp2;//������ʱ������

    //��ʼ��CNF
    cnf = (Conjunctive_Normal_Form*)malloc(sizeof(Conjunctive_Normal_Form));
    cnf->first_clause = NULL;

    //CNF�ļ���ȡ
    while (fscanf(fp, "%s", readfile) != EOF) { //ѭ�����ļ�
        if (strcmp(readfile, "p") == 0) //������ȡ�ļ�ֱ����ʼ��־��p��
            break;
    }
    while (fscanf(fp, "%s", readfile) != EOF) {
        if (strcmp(readfile, "cnf") == 0) {//���ļ��ж����ַ�����cnf��
            fscanf(fp, "%d", &cnf->literal_num);//��ȡCNF�ļ���Ԫ��������literal_num
            fscanf(fp, "%d", &cnf->clause_num);//��ȡCNF�ļ��Ӿ�����������clause_num
            break;
        }
    }

    //printf("literal_num:%d\nclause_num:%d\n", cnf->literal_num, cnf->clause_num);//���������Ϣ

    //���岢��ʼ����Ԫ��ֵ��
    ValueList = (ArgueValue*)malloc((cnf->literal_num + 2) * sizeof(ArgueValue));
    if (!ValueList) return OVERFLOW;//û����ɹ�������OVERFLOW
    for (i = 1; i <= cnf->literal_num; i++) {
        ValueList[i].is_value = 0;
        ValueList[i].tag = 0;
        ValueList[i].dep = 0;
        ValueList[i].num = 0;
        ValueList[i].nev = 0;
        ValueList[i].pos = 0;
        ValueList[i].score = 0;
        ValueList[i].first = NULL;
    }

    //����CNF��ʽ�ṹ
    if (cnf->clause_num) {
        if (fscanf(fp, "%d", &tmp) != EOF && tmp != 0) {
            //����CNF�Ӿ��ͷָ��
//            printf("%d ", tmp);
            clause_tmp1 = (Clause_List)malloc(sizeof(Clause_Node));
            if (!clause_tmp1) return OVERFLOW;
            cnf->first_clause = clause_tmp1;//����ͷ�ڵ�
            clause_tmp2 = clause_tmp1;
            CreateClause2(fp, clause_tmp1, tmp);//�������Ӧ�Ӿ�����
            clause_tmp1->next_clause = NULL;
            i = 2;
            //����������CNF�Ӿ���ʽ�ṹ
            while (i++ <= cnf->clause_num) {
                fscanf(fp, "%d", &tmp);
                //                printf("%d ", tmp);
                clause_tmp1 = (Clause_List)malloc(sizeof(Clause_Node));
                if (!clause_tmp1) return OVERFLOW;
                CreateClause2(fp, clause_tmp1, tmp);//�������Ӧ�Ӿ�����
                clause_tmp2->next_clause = clause_tmp1;
                clause_tmp2 = clause_tmp1;
                clause_tmp1->next_clause = NULL;
            }
        }
    }
    return OK;
}

//����CreateClause2�����������Ӿ�����
//���룺�ļ�ָ�룬�Ӿ���ʽ��㣬��ֵ
//�����״̬
status CreateClause2(FILE* fp, Clause_List& sentence, int first) {
    int num = 1, tmp;//����������Ŀ����ʱ����
    //    printf("%d ", first);
    if (!first) return ERROR;
    Literal_List literal_tmp1, literal_tmp2;//������ʱ������
    literal_tmp1 = (Literal_List)malloc(sizeof(Literal_Node));
    if (!literal_tmp1) return OVERFLOW;
    //����������ʽ�ṹͷ�ڵ�
    literal_tmp1->literal = first;
    ValueList[abs(first)].num++;
    first > 0 ? ValueList[abs(first)].pos++ : ValueList[abs(first)].nev++;
    if (!ValueList[abs(first)].first) {
        literal_tmp1->head = sentence;
        ValueList[abs(first)].first = literal_tmp1;
        ValueList[abs(first)].first->next_same_literal = NULL;
    }
    else {
        literal_tmp1->head = sentence;
        literal_tmp1->next_same_literal = ValueList[abs(first)].first;
        ValueList[abs(first)].first = literal_tmp1;
    }
    literal_tmp1->next_literal = NULL;
    literal_tmp2 = literal_tmp1;
    sentence->first_literal = literal_tmp1;
    sentence->watch_literal[0] = literal_tmp1->literal;

    // ��������������
    fscanf(fp, "%d", &tmp);
    if (!tmp) {
        prehandle_stack[pre_top++] = first;
        int op = (first > 0 ? 1 : -1);
        int oldliteral = ValueList[abs(first)].is_value;
        if (oldliteral && oldliteral != op) {
            isConflict = TRUE;
        }
        ValueList[abs(first)].is_value = (first > 0 ? 1 : -1);
    }
    while (tmp != 0) {
        //        printf("%d ", tmp);
        ValueList[abs(tmp)].num++;
        tmp > 0 ? ValueList[abs(tmp)].pos++ : ValueList[abs(tmp)].nev++;
        literal_tmp1 = (Literal_List)malloc(sizeof(Literal_Node));
        if (!literal_tmp1) return OVERFLOW;
        literal_tmp1->literal = tmp;
        literal_tmp1->next_literal = NULL;
        literal_tmp2->next_literal = literal_tmp1;
        literal_tmp2 = literal_tmp1;
        if (!ValueList[abs(tmp)].first) {
            literal_tmp1->head = sentence;
            ValueList[abs(tmp)].first = literal_tmp1;
            ValueList[abs(tmp)].first->next_same_literal = NULL;
        }
        else {
            literal_tmp1->head = sentence;
            literal_tmp1->next_same_literal = ValueList[abs(tmp)].first;
            ValueList[abs(tmp)].first = literal_tmp1;
        }
        num++;
        fscanf(fp, "%d", &tmp);//��ȡ��һ����
    }if (sentence->first_literal->next_literal)
        sentence->watch_literal[1] = sentence->first_literal->next_literal->literal;
    sentence->number = num;
    //    printf("%d", num);
    //    printf("\n");
    return OK;
}

//ע�⴫�����ǰһ�����
//����DestroyClause2�����������Ӿ���
//���룺�־���ʽ�ڵ�
//�����״̬
status DestroyClause2(Clause_List& sentence) {
    Clause_List p;
    p = sentence->next_clause;
    if (!p) return ERROR;//�жϺ�����
    sentence->next_clause = p->next_clause;
    while (p->first_literal->next_literal) DestroyLiteral2(p->first_literal);
    free(p->first_literal);
    p->first_literal = NULL;
    free(p);
    p = NULL;
    return OK;
}

//ע�⴫�����ǰһ�����
//����DestroyLiteral�������������ֽ��
//���룺������ʽ���
//�����״̬
status DestroyLiteral2(Literal_List& word) {
    Literal_List p;
    p = word->next_literal;
    if (!p) return ERROR;//�жϺ�����
    word->next_literal = p->next_literal;
    free(p);
    p = NULL;
    return OK;
}

//����DestroyCNF2����������CNF�ļ�
//���룺cnfָ��
//�����״̬
status DestroyCNF2(Conjunctive_Normal_Form_List& cnf) {
    if (!cnf) return ERROR;//�жϺ�����
    Clause_List clause_tmp1;
    while (cnf->first_clause->next_clause) DestroyClause2(cnf->first_clause);
    clause_tmp1 = (Clause_List)malloc(sizeof(Clause_Node));
    clause_tmp1->next_clause = cnf->first_clause;
    DestroyClause2(clause_tmp1);
    free(clause_tmp1);
    clause_tmp1 = NULL;
    free(cnf);
    cnf = NULL;
    return OK;
}

//����Select_Literal3�������������߲���
int select_literal3(Conjunctive_Normal_Form_List cnf) {

    //gettimeofday(&st, NULL);

    int tmp0, flag = 0, tmp_literal, stp;
    for (tmp0 = 1; tmp0 <= cnf->literal_num; tmp0++) {
        if (!ValueList[tmp0].is_value) {
            flag = 1;
        }
    }
    srand(int(time(0)));
    int a;
    if (!flag) return 0;
    if (cnt0 > 5000 * times) {
        times++;
        for (tmp0 = 1; tmp0 <= cnf->literal_num; tmp0++) {
            ValueList[tmp0].is_value = 0;
            ValueList[tmp0].dep = 0;
        }
        //        printf("conflict:%d\n\n\n\n\n\n\n\n", times);
    }

    Clause_List p;
    Literal_List q = NULL, r;
    int literal;
    while (con_top) {
        literal = conflict_stack_[--con_top];
        q = ValueList[abs(literal)].first;
        while (q) {
            if (q->head->tag) {
                q = q->next_same_literal;
                continue;
            }
            r = q->head->first_literal;
            while (r) {
                if (r->tag) {
                    r = r->next_literal;
                    continue;
                }
                ValueList[abs(r->literal)].score += cnf->literal_num / (r->head->number * r->head->number);
                if (ValueList[abs(r->literal)].score > 5000) {
                    //                    printf("score:%d\n", ValueList[abs(r->literal)].score);
                    for (tmp0 = 1; tmp0 <= cnf->literal_num; tmp0++) {
                        ValueList[tmp0].score /= 50;
                    }
                }
                r->literal > 0 ? ValueList[abs(r->literal)].pos++ : ValueList[abs(r->literal)].nev++;
                r = r->next_literal;
            }
            q = q->next_same_literal;
        }
    }
    stp = 0;
    for (tmp0 = 1; tmp0 <= cnf->literal_num; tmp0++) {
        if (!ValueList[tmp0].is_value && ValueList[tmp0].score >= stp) {
            stp = ValueList[tmp0].score;
            literal = tmp0;
        }
    }
    cnt++;
    //gettimeofday(&ed, NULL);

    //time_total = (ed.tv_sec - st.tv_sec) + (ed.tv_usec - st.tv_usec) / 1000000.0;
    //time_sum += time_total;
    //    printf("[time_total%d]:%lf S\n", cnt, time_total);
    if (ValueList[abs(literal)].pos >= ValueList[abs(literal)].nev)
        return abs(literal);
    else
        return -abs(literal);
}

//����Select_Literal2�������������߲���
int select_literal2(Conjunctive_Normal_Form_List cnf) {
    int tmp0, flag = 0;
    for (tmp0 = 1; tmp0 <= cnf->literal_num; tmp0++) {
        ValueList[tmp0].num = 0;
        ValueList[tmp0].pos = 0;
        ValueList[tmp0].nev = 0;
        ValueList[tmp0].score = 0;
        if (!ValueList[tmp0].is_value)
            flag = 1;
    }
    if (!flag) return 0;
    Clause_List p;
    Literal_List q = NULL, r;
    int literal;
    while (con_top) {
        literal = conflict_stack_[--con_top];
        q = ValueList[abs(literal)].first;
        while (q) {
            r = q->head->first_literal;
            while (r) {
                if (ValueList[r->literal].is_value) {
                    r = r->next_literal;
                    continue;
                }
                ValueList[abs(r->literal)].score += (cnf->literal_num / (r->head->number * r->head->number));
                r->literal > 0 ? ValueList[abs(r->literal)].pos++ : ValueList[abs(r->literal)].nev++;
                r = r->next_literal;
            }
            q = q->next_same_literal;
        }
    }
    Clause_List h = NULL;
    for (p = cnf->first_clause; p; p = p->next_clause) {
        flag = 0;
        q = p->first_literal;
        while (q) {
            if (q->literal * ValueList[abs(q->literal)].is_value > 0) {
                flag = 1;
                break;
            }
            q = q->next_literal;
        }
        if (flag) continue;
        r = p->first_literal;
        while (r) {
            ValueList[abs(r->literal)].num++;
            ValueList[abs(r->literal)].score += (cnf->literal_num / (r->head->number * r->head->number));
            r->literal > 0 ? ValueList[abs(r->literal)].pos++ : ValueList[abs(r->literal)].nev++;
            if (ValueList[abs(r->literal)].score > 5000) {
                //                printf("score:%d\n", ValueList[abs(r->literal)].score);
                for (int i = 1; i <= cnf->literal_num; i++) {
                    ValueList[i].score /= 50;
                }
            }
            r = r->next_literal;
        }
    }
    int i, score = 0;
    literal = 0;
    for (i = 1; i <= cnf->literal_num; i++) {
        if (!ValueList[i].is_value && ValueList[i].score >= score) {
            score = ValueList[i].score;
            literal = i;
        }
    }
    if (ValueList[literal].pos >= ValueList[literal].nev)
        return abs(literal);
    else
        return -abs(literal);
}

//����update_storevalue���������´洢����
int update_value(Conjunctive_Normal_Form_List cnf) {
    int tmp0, tmp_literal;
    int score;
    Literal_List p = NULL;
    for (tmp0 = 1; tmp0 <= cnf->literal_num; tmp0++) {
        p = ValueList[tmp0].first;
        score = 0;
        while (p) {
            score += (cnf->literal_num) / (p->head->number * p->head->number);
            p = p->next_literal;
        }
        ValueList[tmp0].score = score;
    }
    return OK;
}

//����Select_Literal�������������߲���
int select_literal(Conjunctive_Normal_Form_List cnf) {

    //gettimeofday(&st, NULL);

    int tmp0, flag = 0, tmp_literal, stp;
    for (tmp0 = 1; tmp0 <= cnf->literal_num; tmp0++) {
        if (!ValueList[tmp0].is_value) {
            flag = 1;
        }
    }
    srand(int(time(0)));
    int a;
    if (!flag) return 0;
    if (cnt0 > 100 * times) {
        times++;
        for (tmp0 = 1; tmp0 <= cnf->literal_num; tmp0++) {
            ValueList[tmp0].is_value = 0;
        }
        //        printf("conflict:%d\n\n\n\n\n\n\n\n", times);
    }

    Clause_List p;
    Literal_List q = NULL, r;
    int literal;
    while (con_top) {
        literal = conflict_stack_[--con_top];
        q = ValueList[abs(literal)].first;
        while (q) {
            if (q->head->tag) {
                q = q->next_same_literal;
                continue;
            }
            r = q->head->first_literal;
            while (r) {
                if (r->tag) {
                    r = r->next_literal;
                    continue;
                }
                ValueList[abs(r->literal)].score += cnf->literal_num / (r->head->number * r->head->number);
                if (ValueList[abs(r->literal)].score > 500) {
                    //                    printf("score:%d\n", ValueList[abs(r->literal)].score);
                    for (tmp0 = 1; tmp0 <= cnf->literal_num; tmp0++) {
                        ValueList[tmp0].score /= 10;
                    }
                }
                r->literal > 0 ? ValueList[abs(r->literal)].pos++ : ValueList[abs(r->literal)].nev++;
                r = r->next_literal;
            }
            q = q->next_same_literal;
        }
    }
    stp = 0;
    for (tmp0 = 1; tmp0 <= cnf->literal_num; tmp0++) {
        if (!ValueList[tmp0].is_value && ValueList[tmp0].score >= stp) {
            stp = ValueList[tmp0].score;
            literal = tmp0;
        }
    }
    cnt++;
    //gettimeofday(&ed, NULL);

    time_total = (ed.tv_sec - st.tv_sec) + (ed.tv_usec - st.tv_usec) / 1000000.0;
    time_sum += time_total;
    //    printf("[time_total%d]:%lf S\n", cnt, time_total);
    if (ValueList[abs(literal)].pos >= ValueList[abs(literal)].nev)
        return abs(literal);
    else
        return -abs(literal);
}

//����decide���������б�������
status decide(Conjunctive_Normal_Form_List& cnf) {
    if (pre_top)
        return TRUE;
    int part_literal = select_literal3(cnf);
    if (!part_literal) {
        return FALSE;
    }
    else {
        decision_stack[dec_top] = part_literal;
        decision_num[dec_top] = 1;
        dec_top++;
        return TRUE;
    }
}

//����bcp���̣�ʵ�ֵ��Ӿ䴫��
status bcp(Conjunctive_Normal_Form_List& cnf) {
    int literal;
    if (dec_top) {
        literal = decision_stack[dec_top - 1];
        prehandle_stack[pre_top++] = literal;
    }
    Literal_List x = NULL;
    Clause_List p = NULL;
    while (pre_top) {
        int pre_literal = prehandle_stack[--pre_top];
        //        printf("(%d)", pre_literal);
        ValueList[abs(pre_literal)].is_value = (pre_literal > 0 ? 1 : -1);
        ValueList[abs(pre_literal)].dep = dec_top;
        for (x = ValueList[abs(pre_literal)].first; x; x = x->next_same_literal) {
            p = x->head;
            if (p->number == 1) {
                continue;
            }
            if ((abs(p->watch_literal[0]) == abs(pre_literal)) || (abs(p->watch_literal[1]) == abs(pre_literal))) {
                int up, st;
                if (abs(p->watch_literal[0]) == abs(pre_literal)) {
                    up = 0;
                    st = 1;
                }
                else {
                    up = 1;
                    st = 0;
                }
                if (p->watch_literal[up] == pre_literal) {
                    continue;
                }
                Literal_List q = NULL;
                for (q = p->first_literal; q; q = q->next_literal) {
                    if (q->literal == p->watch_literal[0] || q->literal == p->watch_literal[1]) {
                        continue;
                    }
                    if (q->literal * ValueList[abs(q->literal)].is_value < 0) {
                        continue;
                    }
                    break;
                }
                if (q == NULL) {
                    if (!ValueList[abs(p->watch_literal[st])].is_value) {
                        prehandle_stack[pre_top++] = p->watch_literal[st];
                    }
                    else if (p->watch_literal[st] * ValueList[abs(p->watch_literal[st])].is_value < 0) {
                        conflict_stack_[con_top++] = pre_literal;
                        return FALSE;
                    }
                }
                else {
                    p->watch_literal[up] = q->literal;
                }
            }
        }
    }
    return TRUE;
}

//����resolveConflict�����������ͻ
status resolveConflict(Conjunctive_Normal_Form_List& cnf) {
    //    printf("a");
    int i;
    while (dec_top > 0 && decision_num[dec_top - 1] == 2) {
        decision_num[dec_top - 1] = 0;
        dec_top--;
    }
    if (!dec_top) {
        return FALSE;
    }
    decision_num[dec_top - 1]++;
    decision_stack[dec_top - 1] = -decision_stack[dec_top - 1];
    pre_top = 0;
    for (i = 1; i <= cnf->literal_num; i++) {
        if (ValueList[i].dep >= dec_top) {
            ValueList[i].is_value = 0;
            ValueList[i].dep = 0;
        }
    }
    return TRUE;
}

//����DPLL1��������Ϊ����cnf�ļ��ĵ�һ��DPLL�ļ�
status DPLL2(Conjunctive_Normal_Form_List cnf) {
    update_value(cnf);
    if (isConflict) {
        // conflict in single literal clauses
        return FALSE;
    }
    while (TRUE) {
        if (!decide(cnf)) {
            //            printf("[total_time_sum]:%lf", time_sum * 1000);
            return true;
        }
        while (!bcp(cnf)) {
            cnt0++;
            if (!resolveConflict(cnf)) {
                //                check_print2(cnf);
                return false;
            }
        }
    }
}