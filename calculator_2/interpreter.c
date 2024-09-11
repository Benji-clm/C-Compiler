#include <stdio.h>
#include "calc2.h"
#include "y.tab.h"

int ex(nodeType *p)
{
    if (!p)
        return 0;
    switch (p->type)
    {
    // For constants and variables, simply return the values
    case typeCon:
        return p->con.value;
    case typeId:
        return sym[p->id.i];
    // For operators (or also structures, i.e {}), we store the operands in an array of pointers,
    // where each element represents one operand for the operator
    case typeOpr:
        switch (p->opr.oper)
        {
        case WHILE:
            while (ex(p->opr.op[0]))
                ex(p->opr.op[1]);
            return 0;
        case IF:
            if (ex(p->opr.op[0]))
                ex(p->opr.op[1]);
            else if (p->opr.nops > 2)
                ex(p->opr.op[2]);
            return 0;
        case PRINT:
            printf("%d\n", ex(p->opr.op[0]));
            return 0;
        case ';':
            ex(p->opr.op[0]);
            return ex(p->opr.op[1]);
        case '=':
            return sym[p->opr.op[0]->id.i] =
                       ex(p->opr.op[1]);
        case UMINUS:
            return -ex(p->opr.op[0]);
        case '+':
            return ex(p->opr.op[0]) + ex(p->opr.op[1]);
        case '-':
            return ex(p->opr.op[0]) - ex(p->opr.op[1]);
        case '*':
            return ex(p->opr.op[0]) * ex(p->opr.op[1]);
        case '/':
            return ex(p->opr.op[0]) / ex(p->opr.op[1]);
        case '<':
            return ex(p->opr.op[0]) < ex(p->opr.op[1]);
        case '>':
            return ex(p->opr.op[0]) > ex(p->opr.op[1]);
        case GE:
            return ex(p->opr.op[0]) >= ex(p->opr.op[1]);
        case LE:
            return ex(p->opr.op[0]) <= ex(p->opr.op[1]);
        case NE:
            return ex(p->opr.op[0]) != ex(p->opr.op[1]);
        case EQ:
            return ex(p->opr.op[0]) == ex(p->opr.op[1]);
        }
    }
    return 0;
}

/*
    Let's describe some specific cases we have:
    - WHILE loop
        For the while loop, using the number of "operands" (should only be 2, the condition and the statement),
        while the condition is only true, which is found by executing: ex(opr.op[0]), we keep executing the statement,
        which is stored as the second operand of the array of the operator: ex(opr.op[1]).

        -> notice the use of ex(), this means we not only evaluate the operand itself, but we recursively do so,
           meaning that if the operand itself is an operator, we also perform the operand of this operator, and so on...
        
    - IF ELSE statements
        For an IF statement, recall that the parser has the grammar rule: IF '(' expr ')' stmt ELSE stmt,
        and performs the action opr(IF, 3, $3, $5, $7). Where $3 is the expr, or the condition of the IF statement,
        $5 is the stmt, or the action of the IF statement, and $7 is the stmt, or the action of the ELSE statement.
        Hence why we have 3 operands, expr, stmt, stmt. In the interpreter, we execute the expr, if true,
        then we perform its related action, stored as the second operand of the operator, if(ex(opr.op[0])){ex(opr.op[1])}
        else IF (note that if, very important), we perform the third operand, which is the else stmt.
        Why else IF? Well, else if(p->opr.nops > 2), this to ensure that we only attempt to perform an else statement if
        it does indeed exists, avoid some undefined behaviour, which should throw an error (if not it could do anything).
*/
