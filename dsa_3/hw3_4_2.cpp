#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stack>
#include <iostream>

#define MAX 1100000
#define toChar_MAX 200
#define toInt_MAX 256
#define pow_MAX 7
#define MIN 12
#define PLU 13
#define MUL 20
#define DIV 21
#define ADD 30
#define SUB 31
#define PAR 100
#define SIN 102
#define COS 103
#define EXP 104
#define LOG 105
#define POW 106
#define SQR 107
#define FAB 108

/*
Sign-Value Table
Sign		-	+	*	/	+	-	(	s	c	e	l	p	r	f
Value		-12	-13	-20	-21	-30	-31	-100	-102	-103	-104	-105	-106	-107	-108
*/

class Postfix
{
public:
	int c;
	double v;
};

char infix[MAX],tmp[MAX], toChar[toChar_MAX][5];
int toInt[toInt_MAX];
Postfix postfix[MAX];

void toPostfix()
{
	std::stack<int> op_stack;
	int i = 0, p = 0;
	while(infix[i] != '\n')
	{
		if( (infix[i] <= '9' && infix[i] >= '0') || infix[i] == '.' )
		{
			int index_pow = -1;
			postfix[p].c = 1;
			postfix[p].v = 0;
			while(infix[i] <= '9' && infix[i] >= '0')	postfix[p].v = postfix[p].v * 10 + (int)(infix[i++] - '0');
			if(infix[i] == '.')	i++;
			while(infix[i] <= '9' && infix[i] >= '0')	postfix[p].v = postfix[p].v + pow(10, index_pow--)  * (int)(infix[i++] - '0');
			p++;	i--;
		}
		else if(infix[i] == '(' || infix[i] == 'm' || infix[i] == 'u')	
		{
			op_stack.push( toInt[ (int)infix[i] ] );
			printf("Encounter \"%s\":\n\tPush \"%s\" Into The Stack\n", toChar[ -op_stack.top() ], toChar[ -op_stack.top() ]);
		}
		else if(infix[i] == ')')
		{
			printf("Encounter \")\":\n");
			while(op_stack.top() > toInt[ (int)'(' ])
			{
				postfix[p++].c = op_stack.top();
				printf("\tPop \"%s\" From The Stack\n", toChar[ -op_stack.top() ]);
				op_stack.pop();
			}
			if(op_stack.top() < toInt[ (int)'(' ])	postfix[p++].c = op_stack.top();
			printf("\tPop \"%s\" From The Stack\n", toChar[ -op_stack.top() ]);
			op_stack.pop();
		}
		else if(infix[i] == ',')
		{
			printf("Encounter \",\":\n");
			while(op_stack.top() > toInt[ (int)'(' ])
			{
				postfix[p++].c = op_stack.top();
				printf("\tPop \"%s\" From The Stack\n", toChar[ -op_stack.top() ]);
				op_stack.pop();
			}
		}
		else if(infix[i] == 's' || infix[i] == 'c' || infix[i] == 'e' || infix[i] == 'l' || infix[i] == 'p' || infix[i] == 'f')
		{
			int pos = toInt[ (int)infix[i] ];
			if(infix[i + 2] == 'r')	pos = toInt[ (int)'r'];
			if(infix[i + 3] != '(')	i++;
			op_stack.push(pos);
			printf("Encounter \"%s\":\n", toChar[ -pos ]);
			printf("\tPush \"%s\" Into The Stack\n", toChar[ -op_stack.top() ]);
			i += 3;
		}
		else
		{
			printf("Encounter \"%s\":\n", toChar[ -toInt[ (int)infix[i] ] ]);
			while((!op_stack.empty()) && ((-op_stack.top()) / 10) <= ((-toInt[ (int)infix[i] ]) / 10))
			{
				postfix[p++].c = op_stack.top();
				printf("\tPop \"%s\" From The Stack\n", toChar[ -op_stack.top() ]);
				op_stack.pop();
			}
			op_stack.push( toInt[ (int)infix[i] ] );
			printf("\tPush \"%s\" Into The Stack\n", toChar[ -op_stack.top() ]);
		}
		i++;
	}
	printf("Encounter nothing:\n");
	while(!op_stack.empty())
	{
		postfix[p++].c = op_stack.top();
		printf("\tPop \"%s\" From The Stack\n", toChar[ -op_stack.top() ]);
		op_stack.pop();
	}
	postfix[p].c = -1;
	return;
}

void toValue()
{
	std::stack<double> val_stack;
	int p = 0;
	double v_1 = 0, v_2 = 0;
	while(postfix[p].c != -1)
	{
		if(postfix[p].c >= 0)	val_stack.push(postfix[p].v);
		else
		{
			v_2 = val_stack.top();
			val_stack.pop();
			if(postfix[p].c == -MIN)	val_stack.push(-v_2);
			else if(postfix[p].c == -SIN)	val_stack.push(sin(v_2));
			else if(postfix[p].c == -COS)	val_stack.push(cos(v_2));
			else if(postfix[p].c == -EXP)	val_stack.push(exp(v_2));
			else if(postfix[p].c == -LOG)	val_stack.push(log(v_2));
			else if(postfix[p].c == -SQR)	val_stack.push(sqrt(v_2));
			else if(postfix[p].c == -FAB)	val_stack.push(fabs(v_2));
			else if(postfix[p].c == -PLU)	val_stack.push(+v_2);
			else
			{
				v_1 = val_stack.top();
				val_stack.pop();
				if(postfix[p].c == -ADD)	val_stack.push(v_1 + v_2);
				else if(postfix[p].c == -SUB)	val_stack.push(v_1 - v_2);
				else if(postfix[p].c == -MUL)	val_stack.push(v_1 * v_2);
				else if(postfix[p].c == -DIV)	val_stack.push(v_1 / v_2);
				else if(postfix[p].c == -POW)	val_stack.push(pow(v_1, v_2));
			}
		}
		p++;
	}
	if(val_stack.size() != 1)	printf("Error Form\n");
	else	printf("RESULT: %.6lf\n", val_stack.top());
	return;
}

int main()
{
	toInt[(int)'+'] = -ADD;	strcpy(toChar[ADD], "+");
	toInt[(int)'-'] = -SUB;	strcpy(toChar[SUB], "-");
	toInt[(int)'*'] = -MUL;	strcpy(toChar[MUL], "*");
	toInt[(int)'/'] = -DIV;	strcpy(toChar[DIV], "/");
	toInt[(int)'u'] = -PLU;	strcpy(toChar[PLU], "+");
	toInt[(int)'m'] = -MIN;	strcpy(toChar[MIN], "-");
	toInt[(int)'('] = -PAR; strcpy(toChar[PAR], "(");
	toInt[(int)'s'] = -SIN;	strcpy(toChar[SIN], "sin");
	toInt[(int)'c'] = -COS;	strcpy(toChar[COS], "cos");
	toInt[(int)'e'] = -EXP;	strcpy(toChar[EXP], "exp");
	toInt[(int)'l'] = -LOG;	strcpy(toChar[LOG], "log");
	toInt[(int)'p'] = -POW;	strcpy(toChar[POW], "pow");
	toInt[(int)'r'] = -SQR;	strcpy(toChar[SQR], "sqrt");
	toInt[(int)'f'] = -FAB;	strcpy(toChar[FAB], "fabs");
	while(fgets(tmp, MAX, stdin) != NULL)
	{
		if(tmp[1] == '\0')	continue;
		int j = 0;
		for(int i = 0 ; tmp[i] != '\n' ; i++)
			if(tmp[i] != ' ')
			{
				if(tmp[i] == '+' || tmp[i] == '-')
					if(j == 0 || ((infix[j - 1] > '9' || infix[j - 1] < '0') && infix[j - 1] != ')' && infix[j - 1] != '.'))
					{
						if(tmp[i] == '-')	infix[j++] = 'm';
						else	infix[j++] = 'u';
						continue;
					}
				infix[j++] = tmp[i];
			}
		infix[j] = '\n';
		printf("# transform from infix to postfix\n");
		toPostfix();
		printf("# postfix expression transforming complete\n");
		printf("Postfix Exp: %.6lf", postfix[0].v);
		for(int i = 1 ; postfix[i].c != -1 ; i++)
		{
			if(postfix[i].c >= 0)	printf(" %.6lf", postfix[i].v);
			else printf(" %s", toChar[ (-postfix[i].c) ]);
		}
		printf("\n");
		toValue();
	}
	return 0;
}
