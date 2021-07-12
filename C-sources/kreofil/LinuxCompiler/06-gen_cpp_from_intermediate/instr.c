

/******************************************************************
 instr.c - определния функций обработки инструкций
******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_c.h"
#include "main_t.h"
#include "main_e.h"

void er(int);

// Создание и заполнение структуры команды
struct INSTRUCTION* INSTRUCTION_constructor(opc, arg1, arg2, rez)
  opcType opc;
  struct OPERAND* arg1;
  struct OPERAND* arg2;
  struct OPERAND* rez;
{
  struct INSTRUCTION* iPtr;
  if((iPtr = (struct INSTRUCTION*)malloc(sizeof(struct INSTRUCTION)))==NULL)
  {
    er(23); exit(1);
  }
  iPtr->opc = opc;
  iPtr->arg1 = arg1;
  iPtr->arg2 = arg2;
  iPtr->rez = rez;
  iPtr->next = NULL;
  iPtr->line = line;
  iPtr->column = column;
  return iPtr;
}

// Создание и заполнение структуры списка команд промежуточного представления
struct INSTR_LIST *INSTR_LIST_constructor(fInstrPtr, eInstrPtr, opdPtr)
  struct INSTRUCTION *fInstrPtr;
  struct INSTRUCTION *eInstrPtr;
  struct OPERAND *opdPtr;
{
  struct INSTR_LIST *iListPtr;
  if((iListPtr = (struct INSTR_LIST*)malloc(sizeof(struct INSTR_LIST)))==NULL)
  {
    er(23); exit(1);
  }
  iListPtr->firstInstr = fInstrPtr;
  iListPtr->lastInstr = eInstrPtr;
  iListPtr->lastOpd = opdPtr;
  return iListPtr;
}

// Конкатенация двух списков команд в первый
// Замена или установка операнда списка не производится.
// Она должна осуществляться явно в программе анализатора
void INSTR_LIST_cat(iListRecPtrPtr, iListSrcPtr)
  struct INSTR_LIST **iListRecPtrPtr;
  struct INSTR_LIST *iListSrcPtr;
{
  // Проверка  на пустоту списков команд
  if(*iListRecPtrPtr == NULL) // Список приемника пуст
    if(iListSrcPtr == NULL) // Список источника пуст
      return; // - Получили пустой результат
    else { // а источник есть
      *iListRecPtrPtr = iListSrcPtr;
      return;
    }
  else // Список приемника есть
    if(iListSrcPtr == NULL) // Список источника пуст
      return; // - Получили результат, равный приемнику
  // Конкатенация реально происходит при непустом источнике
  if(iListSrcPtr->firstInstr != NULL) { // Источник не является пустым списком
    if((*iListRecPtrPtr)->firstInstr != NULL) // Приемник - не пустой список
      (*iListRecPtrPtr)->lastInstr->next = iListSrcPtr->firstInstr;
    else
      (*iListRecPtrPtr)->firstInstr = iListSrcPtr->firstInstr;
    (*iListRecPtrPtr)->lastInstr  = iListSrcPtr->lastInstr;
  }
}

// Добавление к списку команд отдельной команды
void INSTR_LIST_append(iListRecPtrPtr, instrPtr)
  struct INSTR_LIST **iListRecPtrPtr;
  struct INSTRUCTION *instrPtr;
{
  // Проверка на отсутствие списка команд и его формирование в этом случае
  if(*iListRecPtrPtr == NULL) {// список команд отсутствует
    *iListRecPtrPtr = INSTR_LIST_constructor(NULL, NULL, NULL);
    (*iListRecPtrPtr)->firstInstr = instrPtr;
    (*iListRecPtrPtr)->lastInstr = instrPtr;
    return;
  }
  if((*iListRecPtrPtr)->firstInstr != NULL)
    (*iListRecPtrPtr)->lastInstr->next = instrPtr;
  else
    (*iListRecPtrPtr)->firstInstr = instrPtr;
  (*iListRecPtrPtr)->lastInstr = instrPtr;
}

// Вывод списка команд
void INSTR_LIST_out(iListRec, outfil)
  struct INSTR_LIST *iListRec;
  FILE* outfil;
{
  int arg1Ident, arg2Ident, rezIdent;
  struct INSTRUCTION* tmpPtr;
  fprintf(outfil, "\n           Список команд:\n");
  if(iListRec == NULL) {
    fprintf(outfil, "Список команд отсутствует!\n");
    return;
  }
  if(iListRec->firstInstr == NULL) {
    fprintf(outfil, "Список команд пуст!\n");
    return;
  }
  for(tmpPtr = iListRec->firstInstr; tmpPtr != NULL; tmpPtr = tmpPtr->next) {
    char *opcName;
    switch(tmpPtr->opc) {
      case addOpc: 	 opcName = "ADD";	break;
      case assOpc: 	 opcName = "ASSIGN";	break;
      case divOpc: 	 opcName = "DIV";	break;
      case eqOpc: 	 opcName = "EQ";	break;
      case emptyOpc: 	 opcName = "EMPTY";	break;
      case exitOpc: 	 opcName = "EXIT";	break;
      case geOpc: 	 opcName = "GE";	break;
      case gotoOpc: 	 opcName = "GOTO";	break;
      case gtOpc: 	 opcName = "GT";	break;
      case ifOpc: 	 opcName = "IF";	break;
      case inOpc: 	 opcName = "IN";	break;
      case indexOpc: 	 opcName = "INDEX";	break;
      case labelOpc: 	 opcName = "LABEL";	break;
      case leOpc: 	 opcName = "LE";	break;
      case ltOpc: 	 opcName = "LT";	break;
      case minOpc: 	 opcName = "MIN";	break;
      case modOpc: 	 opcName = "MOD";	break;
      case multOpc: 	 opcName = "MULT";	break;
      case neOpc: 	 opcName = "NE";	break;
      case outOpc: 	 opcName = "OUT";	break;
      case subOpc: 	 opcName = "SUB";	break;
      case skipOutOpc: 	 opcName = "SKIPOUT";	break;
      case spaceOutOpc:  opcName = "SPACEOUT";	break;
      case tabOutOpc:    opcName = "TABOUT";	break;
      default: opcName = "NODEF";
    }
    arg1Ident = (tmpPtr->arg1 == NULL) ? 0 : tmpPtr->arg1->ident;
    arg2Ident = (tmpPtr->arg2 == NULL) ? 0 : tmpPtr->arg2->ident;
    rezIdent = (tmpPtr->rez == NULL) ? 0 : tmpPtr->rez->ident;

    fprintf(outfil, "%s\t%d,\t%d,\t%d\t// Строка = %d, столбец = %d\n",
      opcName, arg1Ident, arg2Ident, rezIdent, tmpPtr->line, tmpPtr->column
    );
  }
}

// Вывод генерируемого операнда
void cpp_opd_out(struct OPERAND* opd, FILE* outfil) {
    if(opd == NULL) {
        printf("Incorrect empty operand using in semantic model!!!\n");
        return;
    }
    switch(opd->typ) {
        case nameVarOpd: // объявленная переменная
            fprintf(outfil, "%s ", opd->val.var->name);
            break;
        case tmpVarOpd:
            fprintf(outfil, "_V%d_ ", opd->ident);
            break;
        case pointerOpd:
            fprintf(outfil, "_pV%d_ ", opd->ident);
            break;
        case constOpd:
            if(opd->val.cons->typ == INTTYP) {
                fprintf(outfil, "%d", opd->val.cons->val.unum);
            } else {
                fprintf(outfil, "%f", opd->val.cons->val.fnum);
            }
            break;
        case labelOpd:
            fprintf(outfil, "_%d", opd->ident);
            break;
        default:
            fprintf(outfil, "unkonown operand!!!");
    }
}

// Вывод в нужном месте '*', определяющей доступ к содержимому через указатель
void cppOutAstIfNeed(struct OPERAND* opd) {
    if(opd->typ == pointerOpd) {
        fprintf(outfil, "*");
    }
}

// Установка символа операции и генерация самой операции
void cpp_gen2(opcName, arg1, arg2, rez)
    char* opcName;
    struct OPERAND* arg1;
    struct OPERAND* arg2;
    struct OPERAND* rez;
{
    fprintf(outfil, "    ");
    cpp_opd_out(rez, outfil);
    fprintf(outfil, " = ");
    cppOutAstIfNeed(arg1);
    cpp_opd_out(arg1, outfil);
    fprintf(outfil, " %s ", opcName);
    cppOutAstIfNeed(arg2);
    cpp_opd_out(arg2, outfil);
    fprintf(outfil, ";\n");
}

// Генерация операторов и операций c++
void cpp_INSTR_LIST_out(iListRec, outfil)
    struct INSTR_LIST *iListRec;
    FILE* outfil;
{
    //int arg1Ident, arg2Ident, rezIdent;
    struct INSTRUCTION* tmpPtr;
    fprintf(outfil, "\n// Порождаемая главная функция\n");
    fprintf(outfil, "int main() {\n");
    if(iListRec == NULL) {
        fprintf(outfil, "    // Список команд отсутствует!\n");
        fprintf(outfil, "    return 0;\n}\n");
        return;
    }
    if(iListRec->firstInstr == NULL) {
        fprintf(outfil, "Список команд пуст!\n");
        fprintf(outfil, "    return 0;\n}\n");
        return;
    }
    for(tmpPtr = iListRec->firstInstr; tmpPtr != NULL; tmpPtr = tmpPtr->next) {
        char *opcName;
        switch(tmpPtr->opc) {
            case inOpc:
                fprintf(outfil, "    cin >> ");
                cppOutAstIfNeed(tmpPtr->arg1);
                cpp_opd_out(tmpPtr->arg1, outfil);
                fprintf(outfil, ";\n");
                break;
            case outOpc:
                fprintf(outfil, "    cout << ");
                cppOutAstIfNeed(tmpPtr->arg1);
                cpp_opd_out(tmpPtr->arg1, outfil);
                fprintf(outfil, ";\n");
                break;
            case skipOutOpc:
                fprintf(outfil, "    cout << endl;\n");
                break;
            case spaceOutOpc:
                fprintf(outfil, "    cout << \" \";\n");
                break;
            case tabOutOpc:
                fprintf(outfil, "    cout << \"\t\";\n");
                break;
            case labelOpc:
                cpp_opd_out(tmpPtr->arg1, outfil);
                fprintf(outfil, ":\n");
                break;
            case gotoOpc:
                fprintf(outfil, "    goto ");
                cpp_opd_out(tmpPtr->arg1, outfil);
                fprintf(outfil, ";\n");
                break;
            case addOpc:
                cpp_gen2("+", tmpPtr->arg1, tmpPtr->arg2, tmpPtr->rez);
                break;
            case subOpc:
                cpp_gen2("-", tmpPtr->arg1, tmpPtr->arg2, tmpPtr->rez);
                break;
            case multOpc:
                cpp_gen2("*", tmpPtr->arg1, tmpPtr->arg2, tmpPtr->rez);
                break;
            case divOpc:
                cpp_gen2("/", tmpPtr->arg1, tmpPtr->arg2, tmpPtr->rez);
                break;
            case modOpc:
                cpp_gen2("%", tmpPtr->arg1, tmpPtr->arg2, tmpPtr->rez);
                break;
            case eqOpc:
                cpp_gen2("==", tmpPtr->arg1, tmpPtr->arg2, tmpPtr->rez);
                break;
            case neOpc:
                cpp_gen2("!=", tmpPtr->arg1, tmpPtr->arg2, tmpPtr->rez);
                break;
            case leOpc:
                cpp_gen2("<=", tmpPtr->arg1, tmpPtr->arg2, tmpPtr->rez);
                break;
            case ltOpc:
                cpp_gen2("<", tmpPtr->arg1, tmpPtr->arg2, tmpPtr->rez);
                break;
            case geOpc:
                cpp_gen2(">=", tmpPtr->arg1, tmpPtr->arg2, tmpPtr->rez);
                break;
            case gtOpc:
                cpp_gen2(">", tmpPtr->arg1, tmpPtr->arg2, tmpPtr->rez);
                break;
            case assOpc:
                fprintf(outfil, "    ");
                cppOutAstIfNeed(tmpPtr->arg2);
                cpp_opd_out(tmpPtr->arg2, outfil);
                fprintf(outfil, " = ");
                cppOutAstIfNeed(tmpPtr->arg1);
                cpp_opd_out(tmpPtr->arg1, outfil);
                fprintf(outfil, ";\n");
                break;
            case emptyOpc:
                fprintf(outfil, "    ;\n");
                break;
            case exitOpc:
                fprintf(outfil, "    exit(1);\n");
                break;
            case ifOpc:
                fprintf(outfil, "    if(");
                cppOutAstIfNeed(tmpPtr->arg1);
                cpp_opd_out(tmpPtr->arg1, outfil);
                fprintf(outfil, "); else goto ");
                cpp_opd_out(tmpPtr->arg2, outfil);
                fprintf(outfil, ";\n");
                break;
            case minOpc:
//                fprintf(outfil, "    auto ");
                cpp_opd_out(tmpPtr->arg2, outfil);
                fprintf(outfil, " = -");
                cppOutAstIfNeed(tmpPtr->arg1);
                cpp_opd_out(tmpPtr->arg1, outfil);
                fprintf(outfil, ";\n");
                break;
            case indexOpc:
                fprintf(outfil, "    ");
                cpp_opd_out(tmpPtr->rez, outfil);
                fprintf(outfil, " = (");
                cpp_opd_out(tmpPtr->arg1, outfil);
                fprintf(outfil, " + ");
                cpp_opd_out(tmpPtr->arg2, outfil);
                fprintf(outfil, ");\n");
                break;
            default:
                printf("Incorrect Instruction for Code Generation!!!");
                exit(1);
        }
//     fprintf(outfil, "%s\t%d,\t%d,\t%d\t// Строка = %d, столбец = %d\n",
//       opcName, arg1Ident, arg2Ident, rezIdent, tmpPtr->line, tmpPtr->column
//     );
    }
    fprintf(outfil, "    return 0;\n}\n");
}


// Получение типа операнда с предварительной проверкой на соответствие
scalType getType(struct OPERAND* opd) {
    if(opd == NULL) {
        printf("Incorrect empty operand using in semantic model!!!\n");
        exit(1);
    }
    switch(opd->typ) {
        case nameVarOpd: // объявленная переменная
            return opd->val.var->typ;
        case tmpVarOpd:
            return opd->val.tmpVarType;
        case pointerOpd:
            return opd->val.pointerType;
        case constOpd:
            return opd->val.cons->typ;
        default:
            printf("Incorrect operand!!!");
    }
    exit(1);
}

// Формирование типов у временных операндов в зависимости от выполняемой операции
// и типов ее аргументов.
// Осуществляется путем перебора списка команд, поиском и анализом промежуточных переменных
// и установкой их типа при нахождении. Нужно только для операций, порождающих промежуточные переменные
void scan_types_tmp_var(iListRec)
    struct INSTR_LIST *iListRec;
{
    //int i = 1;
    struct INSTRUCTION* tmpPtr;
    if(iListRec == NULL) {
        // Нечего проверять
        return;
    }
    if(iListRec->firstInstr == NULL) {
        // Нечего проверять
        return;
    }
    for(tmpPtr = iListRec->firstInstr; tmpPtr != NULL; tmpPtr = tmpPtr->next) {
        char *opcName;
        scalType arg1Type;
        scalType arg2Type;
        scalType rezType;

        int arg1Ident = (tmpPtr->arg1 == NULL) ? 0 : tmpPtr->arg1->ident;
        int arg2Ident = (tmpPtr->arg2 == NULL) ? 0 : tmpPtr->arg2->ident;
        int rezIdent = (tmpPtr->rez == NULL) ? 0 : tmpPtr->rez->ident;
        
        //printf("%d) arg1:%d, arg2:%d, rez:%d\n", i++, arg1Ident, arg2Ident, rezIdent);
        switch(tmpPtr->opc) {
            case addOpc:
            case subOpc:
            case multOpc:
            case divOpc:
                arg1Type = getType(tmpPtr->arg1);
                arg2Type = getType(tmpPtr->arg2);
                // Анализ вариантов типов аргументов
                if((arg1Type==INTTYP)&&(arg2Type==INTTYP)) {
                    rezType = INTTYP;
                } else {
                    rezType = FLOATTYP;
                }
                //printf("arith\n");
                tmpPtr->rez->val.tmpVarType = rezType;
                break;
            case modOpc:
                if((arg1Type==INTTYP)&&(arg2Type==INTTYP)) {
                    rezType = INTTYP;
                } else {
                    printf("Incorrect type of MOD operation");
                    exit(1);
                }
                //printf("mod\n");
                tmpPtr->rez->val.tmpVarType = rezType;
                break;
            case eqOpc:
            case neOpc:
            case leOpc:
            case ltOpc:
            case geOpc:
            case gtOpc:
                //printf("cmp\n");
                tmpPtr->rez->val.tmpVarType = INTTYP;
                break;
            case assOpc:
                //printf("ass\n");
                if(tmpPtr->arg2->typ == tmpVarOpd) {
                    tmpPtr->arg2->val.tmpVarType = getType(tmpPtr->arg1);
                }
                break;
            case minOpc:
                //printf("min\n");
                tmpPtr->arg2->val.tmpVarType = getType(tmpPtr->arg1);
                break;
            case indexOpc:
                tmpPtr->rez->val.pointerType = getType(tmpPtr->arg1);
                break;
        }
    }
}
