
#include "define.h"

/*定义有效的全局变量*/
ArgueValue* ValueList;//定义变元真值表
int backtracking_stack_DPLL1[MAXN], top1;//定义回溯栈
int conflict_stack[MAXN], ctop;
int ans1;

//定义DPLL1函数，作为处理cnf文件的第一个DPLL文件
status DPLL1(Conjunctive_Normal_Form_List cnf, int op) {
    ans1 = 0;
    top1 = 0;
    ctop = 0;
    Clause_List p;
    Literal_List q;
    while ((p = HasUnitClause(cnf)) != NULL) {
        q = p->first_literal;
        while (q->tag == 1) q = q->next_literal;
        ValueList[abs(q->literal)].tag = 1;
        if (q->literal > 0)
            ValueList[abs(q->literal)].is_value = 1;
        else
            ValueList[abs(q->literal)].is_value = 0;
        backtracking_stack_DPLL1[top1++] = q->literal;
        if (!DeleteLiteral(cnf, q->literal))
            return FALSE;
        if (ans1 == cnf->clause_num) return TRUE;
    }
    int part_literal;
    if (op == 0)
        part_literal = Select_Literal_DPLL0(cnf);
    else if (op == 1)
        part_literal = Select_Literal_DPLL1(cnf);
    else if (op == 2)
        part_literal = Select_Literal_DPLL2(cnf);
    else if (op == 3)
        part_literal = Select_Literal_DPLL3(cnf);
    if (DPLL1_Partition(cnf, part_literal, op))
        return TRUE;
    DPLL1_Recover(cnf, part_literal);
    if (DPLL1_Partition(cnf, -part_literal, op))
        return TRUE;
    return FALSE;
}

//定义DPLL1_Partition函数，变量分裂规则
status DPLL1_Partition(Conjunctive_Normal_Form_List& cnf, int literal, int op) {
    ValueList[abs(literal)].tag = 1;
    if (literal > 0)
        ValueList[abs(literal)].is_value = 1;
    else
        ValueList[abs(literal)].is_value = 0;
    backtracking_stack_DPLL1[top1++] = literal;
    if (!DeleteLiteral(cnf, literal)) {
        conflict_stack[ctop++] = literal;
        return FALSE;
    }
    if (ans1 == cnf->clause_num) return TRUE;
    Clause_List p;
    Literal_List q;
    while ((p = HasUnitClause(cnf)) != NULL) {
        q = p->first_literal;
        while (q && q->tag) q = q->next_literal;
        ValueList[abs(q->literal)].tag = 1;
        if (q->literal > 0)
            ValueList[abs(q->literal)].is_value = 1;
        else
            ValueList[abs(q->literal)].is_value = 0;
        backtracking_stack_DPLL1[top1++] = q->literal;
        if (!DeleteLiteral(cnf, q->literal)) {
            conflict_stack[ctop++] = q->literal;
            return FALSE;
        }
        if (ans1 == cnf->clause_num) return TRUE;
    }
    int part_literal;
    if (op == 0)
        part_literal = Select_Literal_DPLL0(cnf);
    else if (op == 1)
        part_literal = Select_Literal_DPLL1(cnf);
    else if (op == 2)
        part_literal = Select_Literal_DPLL2(cnf);
    else if (op == 3)
        part_literal = Select_Literal_DPLL3(cnf);
    if (DPLL1_Partition(cnf, part_literal, op))
        return TRUE;
    DPLL1_Recover(cnf, part_literal);
    if (DPLL1_Partition(cnf, -part_literal, op))
        return TRUE;
    return FALSE;
}

//定义DPLL1_Recover函数，变量回溯规则
status DPLL1_Recover(Conjunctive_Normal_Form_List& cnf, int literal) {
    int tmp_literal;
    while ((tmp_literal = backtracking_stack_DPLL1[--top1]) != literal) {
        ValueList[abs(tmp_literal)].tag = 0;
        RecoverLiteral(cnf, tmp_literal);
    }
    RecoverLiteral(cnf, tmp_literal);
    return OK;
}

//定义RecoverLiteral函数，回复特定literal文字
status RecoverLiteral(Conjunctive_Normal_Form_List& cnf, int literal) {
    if (!cnf) return ERROR;
    Literal_List tmp = ValueList[abs(literal)].first;
    while (tmp) {
        if (literal == -tmp->literal) {
            tmp->head->number++;
            if (tmp->tag == -1) {
                tmp->tag = 0;
                break;
            }
            tmp->tag = 0;
        }
        else {
            tmp->head->tag--;
            if (!tmp->head->tag)
                ans1--;
            tmp->head->number++;
            if (tmp->tag == -1) {
                tmp->tag = 0;
                break;
            }
            tmp->tag = 0;
        }
        tmp = tmp->next_same_literal;
    }
    return OK;
}

//定义DeleteLiteral函数，删除cnf结构中所有的literal文字
status DeleteLiteral(Conjunctive_Normal_Form_List& cnf, int literal) {
    Literal_List tmp = ValueList[abs(literal)].first;
    while (tmp) {
        if (literal == -tmp->literal) {
            tmp->tag = 1;
            tmp->head->number--;
            if (!tmp->head->number && !tmp->head->tag) {
                tmp->tag = -1;
                return FALSE;
            }
        }
        else {
            tmp->head->tag++;
            if (tmp->head->tag == 1) {
                ans1++;
            }
            tmp->head->number--;
            tmp->tag = 1;
        }
        tmp = tmp->next_same_literal;
    }
    return OK;
}

//定义Select_Literal_DPLL0函数，变量决策策略
int Select_Literal_DPLL0(Conjunctive_Normal_Form_List& cnf) {
    //    clock_t start = 0,finish = 0;//记录DPLL函数调用的起始和终止时间
    //    int duration=0;//记录SAT求解时间
    //    start = clock();
    //    printf("1");
    int i, num_literal;
    for (i = 1; i <= cnf->literal_num; i++) {
        if (!ValueList[i].tag) {
            return i;
        }
    }
}

//定义Select_Literal_DPLL1函数，变量决策策略
int Select_Literal_DPLL1(Conjunctive_Normal_Form_List& cnf) {
    //    printf("1");
    if (!cnf) return 0;
    Clause_List p = cnf->first_clause, q = NULL;
    int num_literal = cnf->literal_num;
    update_storevalue(cnf);

    while (p) {
        if (p->tag || !p->number) {
            p = p->next_clause;
            continue;
        }
        if (p->number < num_literal) {
            num_literal = p->number;
            q = p;
        }
        p = p->next_clause;
    }
    Literal_List m = q->first_literal, n = NULL;

    int num_count = 0;
    while (m) {
        if (m->tag) {
            m = m->next_literal;
            continue;
        }
        if (ValueList[abs(m->literal)].num > num_count) {
            num_count = ValueList[abs(m->literal)].num;
            n = m;
        }
        m = m->next_literal;
    }
    if (ValueList[abs(n->literal)].pos >= ValueList[abs(n->literal)].nev)
        return abs(n->literal);
    else
        return -abs(n->literal);
}

//定义Select_Literal_DPLL2函数，变量决策策略
int Select_Literal_DPLL2(Conjunctive_Normal_Form_List& cnf) {
    //    clock_t start = 0,finish = 0;//记录DPLL函数调用的起始和终止时间
    //    int duration=0;//记录SAT求解时间
    //    start = clock();
    //    printf("1");
    int num_literal = 0, num = 0, i;
    update_storevalue(cnf);
    for (i = 1; i <= cnf->literal_num; i++) {
        if (!ValueList[i].tag && ValueList[i].num >= num) {
            num_literal = i;
            num = ValueList[i].num;
        }
    }
    if (ValueList[num_literal].pos > ValueList[num_literal].nev)
        return num_literal;
    else
        return -num_literal;
}

//定义Select_Literal_DPLL3函数，变量决策策略
int Select_Literal_DPLL3(Conjunctive_Normal_Form_List& cnf) {
    //    printf("1");
    if (!cnf) return 0;
    Clause_List p = cnf->first_clause, q = NULL;
    int score = 0, i, literal;
    update_storevalue2(cnf);
    for (i = 1; i <= cnf->literal_num; i++) {
        if (!ValueList[i].tag && ValueList[i].score >= score) {
            score = ValueList[i].score;
            literal = i;
        }
    }
    if (ValueList[literal].pos > ValueList[literal].nev)
        return literal;
    else
        return -literal;
}

//定义update_storevalue2函数，更新存储文字
status update_storevalue2(Conjunctive_Normal_Form_List& cnf) {
    int tmp0;
    for (tmp0 = 1; tmp0 <= cnf->literal_num; tmp0++) {
        ValueList[tmp0].num = 0;
        ValueList[tmp0].nev = 0;
        ValueList[tmp0].pos = 0;
        ValueList[tmp0].score = 0;
    }
    int literal;
    Literal_List q, r;
    while (ctop) {
        literal = conflict_stack[--ctop];

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
                ValueList[abs(r->literal)].score += (cnf->clause_num / (r->head->number * r->head->number));
                r->literal > 0 ? ValueList[abs(r->literal)].pos++ : ValueList[abs(r->literal)].nev++;
                r = r->next_literal;
            }
            q = q->next_same_literal;
        }
    }
    Clause_List p = cnf->first_clause;
    while (p) {
        if (p->tag) {
            p = p->next_clause;
            continue;
        }
        q = p->first_literal;
        while (q) {
            if (q->tag) {
                q = q->next_literal;
                continue;
            }
            ValueList[abs(q->literal)].score += (cnf->clause_num / (p->number * p->number));
            if (ValueList[abs(q->literal)].score > 100) {
                for (int i = 1; i <= cnf->literal_num; i++) {
                    ValueList[i].score /= 10;
                }
            }
            q->literal > 0 ? ValueList[abs(q->literal)].pos++ : ValueList[abs(q->literal)].nev++;
            q = q->next_literal;
        }
        p = p->next_clause;
    }
    return OK;
}

//定义update_storevalue函数，更新存储文字
status update_storevalue(Conjunctive_Normal_Form_List& cnf) {
    int tmp0;
    for (tmp0 = 0; tmp0 <= cnf->literal_num; tmp0++) {
        ValueList[tmp0].num = 0;
        ValueList[tmp0].nev = 0;
        ValueList[tmp0].pos = 0;
    }
    Clause_List p = cnf->first_clause;
    Literal_List q;
    while (p) {
        if (p->tag) {
            p = p->next_clause;
            continue;
        }
        q = p->first_literal;
        while (q) {
            if (q->tag) {
                q = q->next_literal;
                continue;
            }
            ValueList[abs(q->literal)].num++;
            if (ValueList[abs(q->literal)].num > 100) {
                for (int i = 1; i <= cnf->literal_num; i++) {
                    ValueList[i].num /= 10;
                }
            }
            q->literal > 0 ? ValueList[abs(q->literal)].pos++ : ValueList[abs(q->literal)].nev++;
            q = q->next_literal;
        }
        p = p->next_clause;
    }
    return OK;
}

//定义print函数，输出真值表，用于验证和调试
status print(Conjunctive_Normal_Form_List& cnf) {
    for (int i = 1; i <= cnf->literal_num; i++) {
        printf("{%d %d}", ValueList[i].tag, ValueList[i].is_value);
    }
    printf("\n");
    return OK;
}

//定义CreateCNF函数，创建CNF链表结果
status CreateCNF(FILE* fp, Conjunctive_Normal_Form_List& cnf) {
    char readfile[20];//定义字符类型数组记录在文件中
    int i, tmp;//整型变量定义
    Clause_List clause_tmp1, clause_tmp2;//定义临时结点变量

    //初始化CNF
    cnf = (Conjunctive_Normal_Form*)malloc(sizeof(Conjunctive_Normal_Form));
    cnf->first_clause = NULL;

    //CNF文件读取
    while (fscanf(fp, "%s", readfile) != EOF) { //循环读文件
        if (strcmp(readfile, "p") == 0) //持续读取文件直至开始标志’p‘
            break;
    }
    while (fscanf(fp, "%s", readfile) != EOF) {
        if (strcmp(readfile, "cnf") == 0) {//从文件中读到字符串“cnf”
            fscanf(fp, "%d", &cnf->literal_num);//读取CNF文件变元数并存入literal_num
            fscanf(fp, "%d", &cnf->clause_num);//读取CNF文件子句总数并存入clause_num
            break;
        }
    }

    printf("literal_num:%d\nclause_num:%d\n", cnf->literal_num, cnf->clause_num);//输出基本信息

    //定义并初始化变元真值表
    ValueList = (ArgueValue*)malloc((cnf->literal_num + 2) * sizeof(ArgueValue));
    if (!ValueList) return OVERFLOW;//没分配成功，返回OVERFLOW
    for (i = 1; i <= cnf->literal_num; i++) {
        ValueList[i].is_value = 0;
        ValueList[i].tag = 0;
        ValueList[i].num = 0;
        ValueList[i].nev = 0;
        ValueList[i].pos = 0;
        ValueList[i].score = 0;
        ValueList[i].first = NULL;
    }

    //创建CNF链式结构
    if (cnf->clause_num) {
        if (fscanf(fp, "%d", &tmp) != EOF && tmp != 0) {
            //创建CNF子句的头指针
//            printf("%d ", tmp);
            clause_tmp1 = (Clause_List)malloc(sizeof(Clause_Node));
            if (!clause_tmp1) return OVERFLOW;
            cnf->first_clause = clause_tmp1;//定义头节点
            clause_tmp2 = clause_tmp1;
            CreateClause(fp, clause_tmp1, tmp);//创建其对应子句链表
            clause_tmp1->next_clause = NULL;
            clause_tmp1->tag = 0;
            i = 2;
            //创建完整的CNF子句链式结构
            while (i++ <= cnf->clause_num) {
                fscanf(fp, "%d", &tmp);
                //                printf("%d ", tmp);
                clause_tmp1 = (Clause_List)malloc(sizeof(Clause_Node));
                if (!clause_tmp1) return OVERFLOW;
                CreateClause(fp, clause_tmp1, tmp);//创建其对应子句链表
                clause_tmp2->next_clause = clause_tmp1;
                clause_tmp2 = clause_tmp1;
                clause_tmp2->tag = 0;
                clause_tmp1->next_clause = NULL;
            }
        }
    }
    return OK;
}

//定义CreateClause函数，创建子句链表
//输入：文件指针，子句链式结点，首值
//输出：状态
status CreateClause(FILE* fp, Clause_List& sentence, int first) {
    int num = 1, tmp;//定义文字数目，临时变量
    if (!first) return ERROR;
    Literal_List literal_tmp1, literal_tmp2;//定义临时结点变量
    literal_tmp1 = (Literal_List)malloc(sizeof(Literal_Node));
    if (!literal_tmp1) return OVERFLOW;
    //创建文字链式结构头节点
    literal_tmp1->literal = first;
    literal_tmp1->tag = 0;
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
    literal_tmp2->tag = 0;
    sentence->first_literal = literal_tmp1;

    // 创建文字链表结点
    fscanf(fp, "%d", &tmp);
    while (tmp != 0) {
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
        literal_tmp2->tag = 0;
        num++;
        fscanf(fp, "%d", &tmp);//读取下一文字

    }
    sentence->number = num;
    return OK;
}

//注意传入的是前一个结点
//定义DestroyClause函数，销毁子句结点
//输入：字句链式节点
//输出：状态
status DestroyClause(Clause_List& sentence) {
    Clause_List p;
    p = sentence->next_clause;
    if (!p) return ERROR;//判断合理性
    sentence->next_clause = p->next_clause;
    while (p->first_literal->next_literal) DestroyLiteral(p->first_literal);
    free(p->first_literal);
    p->first_literal = NULL;
    free(p);
    p = NULL;
    return OK;
}

//注意传入的是前一个结点
//定义DestroyLiteral函数，销毁文字结点
//输入：文字链式结点
//输出：状态
status DestroyLiteral(Literal_List& word) {
    Literal_List p;
    p = word->next_literal;
    if (!p) return ERROR;//判断合理性
    word->next_literal = p->next_literal;
    free(p);
    p = NULL;
    return OK;
}

//定义DestroyCNF函数，销毁CNF文件
//输入：cnf指针
//输出：状态
status DestroyCNF(Conjunctive_Normal_Form_List& cnf) {
    if (!cnf) return ERROR;//判断合理性
    for (int i = 1; i <= cnf->literal_num; i++) {
        ValueList[i].is_value = 0;
        ValueList[i].tag = 0;
        ValueList[i].num = 0;
        ValueList[i].nev = 0;
        ValueList[i].pos = 0;
        ValueList[i].score = 0;
        ValueList[i].first = NULL;
    }
    Clause_List clause_tmp1;
    while (cnf->first_clause->next_clause) DestroyClause(cnf->first_clause);
    clause_tmp1 = (Clause_List)malloc(sizeof(Clause_Node));
    clause_tmp1->next_clause = cnf->first_clause;
    DestroyClause(clause_tmp1);
    free(clause_tmp1);
    clause_tmp1 = NULL;
    free(cnf);
    cnf = NULL;
    return OK;
}


//定义HasUnitClause函数，评估CNF是否含有单子句
//输入：cnf指针
//输出：文字链式结点
Clause_List HasUnitClause(Conjunctive_Normal_Form_List& cnf) {
    if (!cnf) return NULL;
    Clause_List p;
    for (p = cnf->first_clause; p; p = p->next_clause) {
        if (!p->tag && p->number == 1) {
            return p;
        }
    }
    return NULL;
}

status check_print2(Conjunctive_Normal_Form_List& cnf) {
    Clause_List p = cnf->first_clause;
    Literal_List q = NULL;
    int flag;
    while (p) {
        q = p->first_literal;
        flag = 0;
        while (q) {
            if (q->literal * ValueList[abs(q->literal)].is_value > 0) {
                flag = 1;
                break;
            }
            q = q->next_literal;
        }
        printf("%d\n", flag);
        p = p->next_clause;
    }
    return OK;
}

//定义store_document函数，存储文件
void store_document(Conjunctive_Normal_Form_List& cnf, char* filename, int d, int time0) {
    int i = 0;
    while (1) {
        if (filename[i] == 'c' && filename[i + 1] == 'n' && filename[i + 2] == 'f')
            break;
        i++;
    }
    filename[i] = 'r', filename[i + 1] = 'e', filename[i + 2] = 's';
    FILE* fp;
    fp = fopen(filename, "w");
    fprintf(fp, "%d\n", d);
    for (int tmp0 = 1; tmp0 < cnf->literal_num; tmp0++) {
        if (ValueList[tmp0].is_value == 1)
            fprintf(fp, "%d ", tmp0);
        else
            fprintf(fp, "%d ", -tmp0);
        if (tmp0 % 10 == 0)
            fprintf(fp, "\n");
    }
    fprintf(fp, "%d", time0);
    fclose(fp);
    return;
}

void traveser_cnf(Conjunctive_Normal_Form_List cnf) {
    Clause_List p = cnf->first_clause;
    Literal_List q = NULL;
    int flag;
    printf("变元数：%d\n子句数目：%d\n", cnf->literal_num, cnf->clause_num);
    while (p) {
        q = p->first_literal;
        while (q) {
            printf("%d ", q->literal);
            q = q->next_literal;
        }
        printf("\n");
        p = p->next_clause;
    }
    return;
}

//定义prove_cnf函数，进行验证
void prove_cnf(Conjunctive_Normal_Form_List cnf) {
    Clause_List p = cnf->first_clause;
    Literal_List q = NULL;
    int flag;
    while (p) {
        q = p->first_literal;
        flag = 0;
        while (q) {
            if (q->literal * ValueList[abs(q->literal)].is_value > 0) {
                flag = 1;
                break;
            }
            q = q->next_literal;
        }
        if (!flag) {
            printf("该cnf公式验证无解!\n");
            return;
        }
        p = p->next_clause;
    }
    printf("该cnf公式验证有解!\n");
    return;
}

//定义show_cnf函数，输出cnf文件求解结果
void show_cnf(Conjunctive_Normal_Form_List cnf) {
    for (int i = 1; i <= cnf->literal_num; i++) {
        if (ValueList[i].is_value == 1)
            printf("%d ", i);
        else
            printf("%d ", -i);
        if (!(i % 10))
            printf("\n");
    }
    return;
}