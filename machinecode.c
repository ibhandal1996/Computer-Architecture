//Iqbal Bhandal
//CSE 140
//Lab2

//#define _CRT_SECURE_NO_DEPRECATE //only needed for Visual Studio
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

//Function declaration
int op(char*, char);
int reg(char*, char);
void imm(char*, char);


int main()
{
	char* opc = 0;
	char* space = 0;
	char* rs = 0;
	char* rt = 0;
	char* rd = 0;
	char input[50] = "";
	char inputcpy[50] = "";
	char MIP[32] = "";
	int numReg = 0;
	int numFun = 0;
	int i = 0;
	bool sft = false;

	fgets(input, sizeof(input), stdin); //user input

	int j = 5; //required to be higher than 4 for if statements
	space = strtok(input, " "); //helps with traversing through instruction
	strcpy(MIP, "000000"); //since all instructions are in R format
	do{
		if (i != 4)
		{
			i++;
			if (i == 1)
			{
				opc = space;
				//shift for sll and srl because of change in shamt
				if (strcmp(opc, "sll") == 0 || strcmp(opc, "srl") == 0)
				{
					sft = true;
				}
			}
			else if (i == 2)
			{
				rd = space;
			}
			else if (i == 3)
			{
				rs = space;
			}
			else if (i == 4)
			{
				rt = space;
				rt[strlen(rt) - 1] = 0; //gets rid of \n at end
				j = 0; //resets j to move forward to produce output
			}
		}
		
		if (j == 0)
		{
			printf("Operation:");
			printf("%s", opc);
			printf("\n");
			j++;
		}
		if (j == 1)
		{
			printf("rs:");
			printf("%s", rs);
			// if we pass a number between 0 to 31 
			if (strcmp(rs, "0") == 0 || strcmp(rs, "1") == 0 || strcmp(rs, "2") == 0 || strcmp(rs, "3") == 0 || strcmp(rs, "4") == 0 || strcmp(rs, "5") == 0 || strcmp(rs, "6") == 0 || strcmp(rs, "7") == 0 || strcmp(rs, "8") == 0 || strcmp(rs, "9") == 0 || strcmp(rs, "10") == 0 || strcmp(rs, "11") == 0 || strcmp(rs, "12") == 0 || strcmp(rs, "13") == 0 || strcmp(rs, "14") == 0 || strcmp(rs, "15") == 0 || strcmp(rs, "16") == 0 || strcmp(rs, "17") == 0 || strcmp(rs, "18") == 0 || strcmp(rs, "19") == 0 || strcmp(rs, "20") == 0 || strcmp(rs, "21") == 0 || strcmp(rs, "22") == 0 || strcmp(rs, "23") == 0 || strcmp(rs, "24") == 0 || strcmp(rs, "25") == 0 || strcmp(rs, "26") == 0 || strcmp(rs, "27") == 0 || strcmp(rs, "28") == 0 || strcmp(rs, "29") == 0 || strcmp(rs, "30") == 0 || strcmp(rs, "31") == 0)
			{
				imm(rs, MIP);
				printf("\n");
			}
			else
			{
				numReg = reg(rs, MIP);
				printf("(R");
				printf("%i", numReg);
				printf(")");
				printf("\n");
			}
			j++;
		}
		if (j == 2)
		{
			printf("rt:");
			printf("%s", rt);
			// if we pass a number between 0 to 31
			if (strcmp(rt, "0") == 0 || strcmp(rt, "1") == 0 || strcmp(rt, "2") == 0 || strcmp(rt, "3") == 0 || strcmp(rt, "4") == 0 || strcmp(rt, "5") == 0 || strcmp(rt, "6") == 0 || strcmp(rt, "7") == 0 || strcmp(rt, "8") == 0 || strcmp(rt, "9") == 0 || strcmp(rt, "10") == 0 || strcmp(rt, "11") == 0 || strcmp(rt, "12") == 0 || strcmp(rt, "13") == 0 || strcmp(rt, "14") == 0 || strcmp(rt, "15") == 0 || strcmp(rt, "16") == 0 || strcmp(rt, "17") == 0 || strcmp(rt, "18") == 0 || strcmp(rt, "19") == 0 || strcmp(rt, "20") == 0 || strcmp(rt, "21") == 0 || strcmp(rt, "22") == 0 || strcmp(rt, "23") == 0 || strcmp(rt, "24") == 0 || strcmp(rt, "25") == 0 || strcmp(rt, "26") == 0 || strcmp(rt, "27") == 0 || strcmp(rt, "28") == 0 || strcmp(rt, "29") == 0 || strcmp(rt, "30") == 0 || strcmp(rt, "31") == 0)
			{
				imm(rt, MIP);
				printf("\n");
			}
			else
			{
				numReg = reg(rt, MIP);
				if (sft == false)
				{
					printf("(R");
					printf("%i", numReg);
					printf(")");
					printf("\n");
				}
				else
				{
					printf("\n");
				}
			}
			j++;
		}
		if (j == 3)
		{
			printf("rd:");
			printf("%s", rd);
			// if we pass a number between 0 to 31
			if (strcmp(rd, "0") == 0 || strcmp(rd, "1") == 0 || strcmp(rd, "2") == 0 || strcmp(rd, "3") == 0 || strcmp(rd, "4") == 0 || strcmp(rd, "5") == 0 || strcmp(rd, "6") == 0 || strcmp(rd, "7") == 0 || strcmp(rd, "8") == 0 || strcmp(rd, "9") == 0 || strcmp(rd, "10") == 0 || strcmp(rd, "11") == 0 || strcmp(rd, "12") == 0 || strcmp(rd, "13") == 0 || strcmp(rd, "14") == 0 || strcmp(rd, "15") == 0 || strcmp(rd, "16") == 0 || strcmp(rd, "17") == 0 || strcmp(rd, "18") == 0 || strcmp(rd, "19") == 0 || strcmp(rd, "20") == 0 || strcmp(rd, "21") == 0 || strcmp(rd, "22") == 0 || strcmp(rd, "23") == 0 || strcmp(rd, "24") == 0 || strcmp(rd, "25") == 0 || strcmp(rd, "26") == 0 || strcmp(rd, "27") == 0 || strcmp(rd, "28") == 0 || strcmp(rd, "29") == 0 || strcmp(rd, "30") == 0 || strcmp(rd, "31") == 0)
			{
				imm(rd, MIP);
				printf("\n");
			}
			else
			{
				numReg = reg(rd, MIP);
				printf("(R");
				printf("%i", numReg);
				printf(")");
				printf("\n");
			}
			j++;
		}
		if (j == 4)
		{
			if (sft == false)
			{
				strcat(MIP, "00000");
				printf("shamt:");
				printf("0");
				printf("\n");
			}
			else
			{
				printf("shamt:");
				printf("%s", rt);
				printf("\n");
				imm(rt, MIP);
			}
		}
		space = strtok(NULL, ", "); //overlooks , from instruction
	} while (space != NULL);
	if (rs == 0) // this is for inputs with 1 register
	{
		rs = rd;
		rt = "0";
		rd = "0";
		strtok(rs, "\n");
		printf("Operation:");
		printf("%s", opc);
		printf("\n");
		printf("rs:");
		printf("%s", rs);
		numReg = reg(rs, MIP);
		printf("(R");
		printf("%i", numReg);
		printf(")");
		printf("\n");
		printf("rt:");
		printf("%s", rt);
		numReg = reg(rt, MIP);
		printf("(R");
		printf("%i", numReg);
		printf(")");
		printf("\n");
		printf("rd:");
		printf("%s", rd);
		numReg = reg(rd, MIP);
		printf("(R");
		printf("%i", numReg);
		printf(")");
		printf("\n");
		printf("shamt:");
		printf("0");
		printf("\n");
		strcat(MIP, "00000");
	}
	numFun = op(opc, MIP);
	printf("Funct:");
	printf("%i", numFun);
	printf("\n");
	printf("Machine Code:");
	printf("%s", MIP);

	//system("pause"); //only needed for Visual Studio
	return 0;

}

int op(char* opc, char MIP[])
{
	int funNum = 0;
	if (strcmp(opc, "add") == 0)
	{
		strcat(MIP, "100000");
		funNum = 32;
	}
	if (strcmp(opc, "addu") == 0)
	{
		strcat(MIP, "100001");
		funNum = 36;
	}
	if (strcmp(opc, "and") == 0)
	{
		strcat(MIP, "100100");
		funNum = 32;
	}
	if (strcmp(opc, "jr") == 0)
	{
		strcat(MIP, "001000");
		funNum = 8;
	}
	if (strcmp(opc, "nor") == 0)
	{
		strcat(MIP, "100111");
		funNum = 39;
	}
	if (strcmp(opc, "or") == 0)
	{
		strcat(MIP, "100101");
		funNum = 37;
	}
	if (strcmp(opc, "slt") == 0)
	{
		strcat(MIP, "101010");
		funNum = 42;
	}
	if (strcmp(opc, "sltu") == 0)
	{
		strcat(MIP, "101011");
		funNum = 43;
	}
	if (strcmp(opc, "sll") == 0)
	{
		strcat(MIP, "000000");
		funNum = 0;
	}
	if (strcmp(opc, "srl") == 0)
	{
		strcat(MIP, "000010");
		funNum = 2;
	}
	if (strcmp(opc, "sub") == 0)
	{
		strcat(MIP, "100100");
		funNum = 34;
	}
	if (strcmp(opc, "subu") == 0)
	{
		strcat(MIP, "100011");
		funNum = 35;
	}
	return funNum;
}

int reg(char* r, char MIP[])
{
	int regi = 0;
	if (strcmp(r, "zero") == 0)
	{
		strcat(MIP, "00000");
		regi = 0;
	}
	else if (strcmp(r, "at") == 0)
	{
		strcat(MIP, "00001");
		regi = 1;
	}
	else if (strcmp(r, "v0") == 0)
	{
		strcat(MIP, "00010");
		regi = 2;
	}
	else if (strcmp(r, "v1") == 0)
	{
		strcat(MIP, "00011");
		regi = 3;
	}
	else if (strcmp(r, "a0") == 0)
	{
		strcat(MIP, "00100");
		regi = 4;
	}
	else if (strcmp(r, "a1") == 0)
	{
		strcat(MIP, "00101");
		regi = 5;
	}
	else if (strcmp(r, "a2") == 0)
	{
		strcat(MIP, "00110");
		regi = 6;
	}
	else if (strcmp(r, "a3") == 0)
	{
		strcat(MIP, "00111");
		regi = 6;
	}
	else if (strcmp(r, "t0") == 0)
	{
		strcat(MIP, "01000");
		regi = 8;
	}
	else if (strcmp(r, "t1") == 0)
	{
		strcat(MIP, "01001");
		regi = 9;
	}
	else if (strcmp(r, "t2") == 0)
	{
		strcat(MIP, "01010");
		regi = 10;
	}
	else if (strcmp(r, "t3") == 0)
	{
		strcat(MIP, "01011");
		regi = 11;
	}
	else if (strcmp(r, "t4") == 0)
	{
		strcat(MIP, "01100");
		regi = 12;
	}
	else if (strcmp(r, "t5") == 0)
	{
		strcat(MIP, "01101");
		regi = 13;
	}
	else if (strcmp(r, "t6") == 0)
	{
		strcat(MIP, "01110");
		regi = 14;
	}
	else if (strcmp(r, "t7") == 0)
	{
		strcat(MIP, "01111");
		regi = 15;
	}
	else if (strcmp(r, "s0") == 0)
	{
		strcat(MIP, "10000");
		regi = 16;
	}
	else if (strcmp(r, "s1") == 0)
	{
		strcat(MIP, "10001");
		regi = 17;
	}
	else if (strcmp(r, "s2") == 0)
	{
		strcat(MIP, "10010");
		regi = 18;
	}
	else if (strcmp(r, "s3") == 0)
	{
		strcat(MIP, "10011");
		regi = 19;
	}
	else if (strcmp(r, "s4") == 0)
	{
		strcat(MIP, "10100");
		regi = 20;
	}
	else if (strcmp(r, "s5") == 0)
	{
		strcat(MIP, "10101");
		regi = 21;
	}
	else if (strcmp(r, "s6") == 0)
	{
		strcat(MIP, "10110");
		regi = 22;
	}
	else if (strcmp(r, "s7") == 0)
	{
		strcat(MIP, "10111");
		regi = 23;
	}
	else if (strcmp(r, "t8") == 0)
	{
		strcat(MIP, "11000");
		regi =24;
	}
	else if (strcmp(r, "t9") == 0)
	{
		strcat(MIP, "11001");
		regi = 25;
	}
	else if (strcmp(r, "k0") == 0)
	{
		strcat(MIP, "11010");
		regi = 26;
	}
	else if (strcmp(r, "k1") == 0)
	{
		strcat(MIP, "11011");
		regi = 27;
	}
	else if (strcmp(r, "gp") == 0)
	{
		strcat(MIP, "11100");
		regi = 28;
	}
	else if (strcmp(r, "sp") == 0)
	{
		strcat(MIP, "11101");
		regi = 29;
	}
	else if (strcmp(r, "fp") == 0)
	{
		strcat(MIP, "11110");
		regi = 30;
	}
	else if (strcmp(r, "ra") == 0)
	{
		strcat(MIP, "11111");
		regi = 31;
	}
	else
	{
		strcat(MIP, "00000"); //if we have a imm for the reg that has a shift
		regi = 0;
	}
	return regi;
}

void imm(char* r, char MIP[])
{
	if (strcmp(r, "0") == 0)
	{
		strcat(MIP, "00000");
	}
	else if (strcmp(r, "1") == 0)
	{
		strcat(MIP, "00001");
	}
	else if (strcmp(r, "2") == 0)
	{
		strcat(MIP, "00010");
	}
	else if (strcmp(r, "3") == 0)
	{
		strcat(MIP, "00011");
	}
	else if (strcmp(r, "4") == 0)
	{
		strcat(MIP, "00100");
	}
	else if (strcmp(r, "5") == 0)
	{
		strcat(MIP, "00101");
	}
	else if (strcmp(r, " 6") == 0)
	{
		strcat(MIP, "00110");
	}
	else if (strcmp(r, "6") == 0)
	{
		strcat(MIP, "00111");
	}
	else if (strcmp(r, " 8") == 0)
	{
		strcat(MIP, "01000");
	}
	else if (strcmp(r, "9") == 0)
	{
		strcat(MIP, "01001");
	}
	else if (strcmp(r, "10") == 0)
	{
		strcat(MIP, "01010");
	}
	else if (strcmp(r, "11") == 0)
	{
		strcat(MIP, "01011");
	}
	else if (strcmp(r, "12") == 0)
	{
		strcat(MIP, "01100");
	}
	else if (strcmp(r, "13") == 0)
	{
		strcat(MIP, "01101");
	}
	else if (strcmp(r, "14") == 0)
	{
		strcat(MIP, "01110");
	}
	else if (strcmp(r, "15") == 0)
	{
		strcat(MIP, "01111");
	}
	else if (strcmp(r, "16") == 0)
	{
		strcat(MIP, "10000");
	}
	else if (strcmp(r, "17") == 0)
	{
		strcat(MIP, "10001");
	}
	else if (strcmp(r, "18") == 0)
	{
		strcat(MIP, "10010");
	}
	else if (strcmp(r, "19") == 0)
	{
		strcat(MIP, "10011");
	}
	else if (strcmp(r, "20") == 0)
	{
		strcat(MIP, "10100");
	}
	else if (strcmp(r, "21") == 0)
	{
		strcat(MIP, "10101");
	}
	else if (strcmp(r, "22") == 0)
	{
		strcat(MIP, "10110");
	}
	else if (strcmp(r, "23") == 0)
	{
		strcat(MIP, "10111");
	}
	else if (strcmp(r, "24") == 0)
	{
		strcat(MIP, "11000");
	}
	else if (strcmp(r, "25") == 0)
	{
		strcat(MIP, "11001");
	}
	else if (strcmp(r, "26") == 0)
	{
		strcat(MIP, "11010");
	}
	else if (strcmp(r, "27") == 0)
	{
		strcat(MIP, "11011");
	}
	else if (strcmp(r, "28") == 0)
	{
		strcat(MIP, "11100");
	}
	else if (strcmp(r, "29") == 0)
	{
		strcat(MIP, "11101");
	}
	else if (strcmp(r, "30") == 0)
	{
		strcat(MIP, "11110");
	}
	else if (strcmp(r, "31") == 0)
	{
		strcat(MIP, "11111");
	}
}
