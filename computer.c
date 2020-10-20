#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "computer.h"
#undef mips			/* gcc already has a def for mips */

unsigned int endianSwap(unsigned int);

void PrintInfo (int changedReg, int changedMem);
unsigned int Fetch (int);
void Decode (unsigned int, DecodedInstr*, RegVals*);
int Execute (DecodedInstr*, RegVals*);
int Mem(DecodedInstr*, int, int *);
void RegWrite(DecodedInstr*, int, int *);
void UpdatePC(DecodedInstr*, int);
void PrintInstruction (DecodedInstr*);

/*Globally accessible Computer variable*/
Computer mips;
RegVals rVals;

/*
 *  Return an initialized computer with the stack pointer set to the
 *  address of the end of data memory, the remaining registers initialized
 *  to zero, and the instructions read from the given file.
 *  The other arguments govern how the program interacts with the user.
 */
void InitComputer (FILE* filein, int printingRegisters, int printingMemory,
  int debugging, int interactive) {
    int k;
    unsigned int instr;

    /* Initialize registers and memory */

    for (k=0; k<32; k++) {
        mips.registers[k] = 0;
    }
    
    /* stack pointer - Initialize to highest address of data segment */
    mips.registers[29] = 0x00400000 + (MAXNUMINSTRS+MAXNUMDATA)*4;

    for (k=0; k<MAXNUMINSTRS+MAXNUMDATA; k++) {
        mips.memory[k] = 0;
    }

    k = 0;
    while (fread(&instr, 4, 1, filein)) {
	/*swap to big endian, convert to host byte order. Ignore this.*/
        mips.memory[k] = ntohl(endianSwap(instr));
        k++;
        if (k>MAXNUMINSTRS) {
            fprintf (stderr, "Program too big.\n");
            exit (1);
        }
    }

    mips.printingRegisters = printingRegisters;
    mips.printingMemory = printingMemory;
    mips.interactive = interactive;
    mips.debugging = debugging;
}

unsigned int endianSwap(unsigned int i) {
    return (i>>24)|(i>>8&0x0000ff00)|(i<<8&0x00ff0000)|(i<<24);
}

/*
 *  Run the simulation.
 */
void Simulate () {
    char s[40];  /* used for handling interactive input */
    unsigned int instr;
    int changedReg=-1, changedMem=-1, val;
    DecodedInstr d;
    
    /* Initialize the PC to the start of the code section */
    mips.pc = 0x00400000;
    while (1) {
        if (mips.interactive) {
            printf ("> ");
            fgets (s,sizeof(s),stdin);
            if (s[0] == 'q') {
                return;
            }
        }

        /* Fetch instr at mips.pc, returning it in instr */
        instr = Fetch (mips.pc);

        printf ("Executing instruction at %8.8x: %8.8x\n", mips.pc, instr);

        /* 
	 * Decode instr, putting decoded instr in d
	 * Note that we reuse the d struct for each instruction.
	 */
        Decode (instr, &d, &rVals);

        /*Print decoded instruction*/
        PrintInstruction(&d);

        /* 
	 * Perform computation needed to execute d, returning computed value 
	 * in val 
	 */
        val = Execute(&d, &rVals);

	UpdatePC(&d,val);

        /* 
	 * Perform memory load or store. Place the
	 * address of any updated memory in *changedMem, 
	 * otherwise put -1 in *changedMem. 
	 * Return any memory value that is read, otherwise return -1.
         */
        val = Mem(&d, val, &changedMem);

        /* 
	 * Write back to register. If the instruction modified a register--
	 * (including jal, which modifies $ra) --
         * put the index of the modified register in *changedReg,
         * otherwise put -1 in *changedReg.
         */
        RegWrite(&d, val, &changedReg);

        PrintInfo (changedReg, changedMem);
    }
}

/*
 *  Print relevant information about the state of the computer.
 *  changedReg is the index of the register changed by the instruction
 *  being simulated, otherwise -1.
 *  changedMem is the address of the memory location changed by the
 *  simulated instruction, otherwise -1.
 *  Previously initialized flags indicate whether to print all the
 *  registers or just the one that changed, and whether to print
 *  all the nonzero memory or just the memory location that changed.
 */
void PrintInfo ( int changedReg, int changedMem) {
    int k, addr;
    printf ("New pc = %8.8x\n", mips.pc);
    if (!mips.printingRegisters && changedReg == -1) {
        printf ("No register was updated.\n");
    } else if (!mips.printingRegisters) {
        printf ("Updated r%2.2d to %8.8x\n",
        changedReg, mips.registers[changedReg]);
    } else {
        for (k=0; k<32; k++) {
            printf ("r%2.2d: %8.8x  ", k, mips.registers[k]);
            if ((k+1)%4 == 0) {
                printf ("\n");
            }
        }
    }
    if (!mips.printingMemory && changedMem == -1) {
        printf ("No memory location was updated.\n");
    } else if (!mips.printingMemory) {
        printf ("Updated memory at address %8.8x to %8.8x\n",
        changedMem, Fetch (changedMem));
    } else {
        printf ("Nonzero memory\n");
        printf ("ADDR	  CONTENTS\n");
        for (addr = 0x00400000+4*MAXNUMINSTRS;
             addr < 0x00400000+4*(MAXNUMINSTRS+MAXNUMDATA);
             addr = addr+4) {
            if (Fetch (addr) != 0) {
                printf ("%8.8x  %8.8x\n", addr, Fetch (addr));
            }
        }
    }
}

/*
 *  Return the contents of memory at the given address. Simulates
 *  instruction fetch. 
 */
unsigned int Fetch ( int addr) {
    return mips.memory[(addr-0x00400000)/4];
}

/* Decode instr, returning decoded instruction. */
void Decode ( unsigned int instr, DecodedInstr* d, RegVals* rVals) {
     int opc = 0;

    opc = instr>>26; //gets opcode
    d->op = opc;

    if(opc == 0) //R format
    {
        d->type = R;
        d->regs.r.funct = instr & 0x0000003f;
        instr = instr>>6;
        d->regs.r.shamt = instr & 0x000001f;
        instr = instr>>5;
        d->regs.r.rd = instr & 0x00001f;
        instr = instr>>5;
        d->regs.r.rt = instr & 0x001f;
        instr = instr >> 5;
        d->regs.r.rs = instr & 0x01f;
        rVals->R_rs = mips.registers[d->regs.r.rs];
        rVals->R_rt = mips.registers[d->regs.r.rt];
        rVals->R_rd = mips.registers[d->regs.r.rd];
        if(d->regs.r.rd == 0 && d->regs.r.funct != 8)
        {
            printf("Invalid Instruction");
            exit(0);
        }

    }
    else if(opc == 2 || opc == 3) //J format
    {
        d->type = J;
        d->regs.j.target = (instr & 0x03ffffff) << 2;
        d->regs.j.target = (mips.pc & 0xf0000000) + d->regs.j.target;

    }
    else if(opc == 4 || opc == 5 || opc == 9 || opc == 12 || opc == 13 || opc == 15 || opc == 35 || opc == 43) //I format
    {
        d->type = I;
        d->regs.i.addr_or_immed = instr & 0x0000ffff;
        if((d->regs.i.addr_or_immed & 0x8000) > 0) //negative value
        {
            d->regs.i.addr_or_immed = d->regs.i.addr_or_immed | 0xffff0000;
        }
        instr = instr >> 16;
        d->regs.i.rt = instr & 0x001f;
        instr = instr >> 5;
        d->regs.i.rs = instr & 0x01f;
        rVals->R_rs = mips.registers[d->regs.i.rs];
        rVals->R_rt = mips.registers[d->regs.i.rt];
        if(d->op != 4 && d->op != 5)
        {
            if(d->regs.i.rt == 0)
            {
                printf("Invalid Instruction");
                exit(0);
            }

        }

    }
    else
    {
        printf("Invalid Instruction");
        exit(0);
    }
    
    
}

/*
 *  Print the disassembled version of the given instruction
 *  followed by a newline.
 */
void PrintInstruction ( DecodedInstr* d) {

    if(d->op == 0) //R format
    {
        if(d->regs.r.funct == 0)
        {
            printf("sll");
            printf("\t");
            printf("$%d, ", d->regs.r.rd);
            printf("$%d, ", d->regs.r.rt);
            printf("$%d, ", d->regs.r.shamt);
            printf("\n");
        }
        else if(d->regs.r.funct == 2)
        {
            printf("srl");
            printf("\t");
            printf("$%d, ", d->regs.r.rd);
            printf("$%d, ", d->regs.r.rt);
            printf("$%d, ", d->regs.r.shamt);
            printf("\n");
        }
        else if(d->regs.r.funct == 8)
        {
            printf("jr");
            printf("\t");
            printf("$%d, ", d->regs.r.rs);
            printf("\n");
        }
        else if(d->regs.r.funct == 33)
        {
            printf("addu");
            printf("\t");
            printf("$%d, ", d->regs.r.rd);
            printf("$%d, ", d->regs.r.rs);
            printf("$%d, ", d->regs.r.rt);
            printf("\n");
        }
        else if(d->regs.r.funct == 35)
        {
            printf("subu");
            printf("\t");
            printf("$%d, ", d->regs.r.rd);
            printf("$%d, ", d->regs.r.rs);
            printf("$%d, ", d->regs.r.rt);
            printf("\n");
        }
        else if(d->regs.r.funct == 36)
        {
            printf("and");
            printf("\t");
            printf("$%d, ", d->regs.r.rd);
            printf("$%d, ", d->regs.r.rs);
            printf("$%d, ", d->regs.r.rt);
            printf("\n");
        }
        else if(d->regs.r.funct == 37)
        {
            printf("or");
            printf("\t");
            printf("$%d, ", d->regs.r.rd);
            printf("$%d, ", d->regs.r.rs);
            printf("$%d, ", d->regs.r.rt);
            printf("\n");
        }
        else if(d->regs.r.funct == 42)
        {
            printf("slt");
            printf("\t");
            printf("$%d, ", d->regs.r.rd);
            printf("$%d, ", d->regs.r.rs);
            printf("$%d, ", d->regs.r.rt);
            printf("\n");
        }
        else
        {
			printf("Invalid Instruction");
			exit(0);
		}
    }
    else if(d->op == 2 || d->op == 3) //J format
    {
        if(d->op == 3)
        {
            printf("jal");
            printf("\t");
            printf("0x%.8x", d->regs.j.target);
            printf("\n");
        }
        else if(d->op == 2)
        {
            printf("j");
            printf("\t");
            printf("0x%.8x", d->regs.j.target);
            printf("\n");  
        }
        else
        {
			printf("Invalid Instruction");
			exit(0);
		}
        
    }
    else //I format
    {
        int z = 0;
        if(d->op == 4)
        {
            printf("beq");
            printf("\t");
            printf("$%d, ", d->regs.r.rs);
            printf("$%d, ", d->regs.r.rt);
            z = d->regs.i.addr_or_immed;
            z = mips.pc + 4 + (z<<2);
            printf("0x%8.8x", z);
            printf("\n");
        }
        else if(d->op == 5)
        {
            printf("bne");
            printf("\t");
            printf("$%d, ", d->regs.r.rs);
            printf("$%d, ", d->regs.r.rt);
            z = d->regs.i.addr_or_immed;
            z = mips.pc + 4 + (z<<2);
            printf("0x%8.8x", z);
            printf("\n");
        }
        else if(d->op == 9)
        {
            printf("addiu");
            printf("\t");
            printf("$%d, ", d->regs.r.rt);
            printf("$%d, ", d->regs.r.rs);
            unsigned int immmmm = d->regs.i.addr_or_immed;
            printf("%d", immmmm);
            printf("\n");
        }
        else if(d->op == 12)
        {
            printf("andi");
            printf("\t");
            printf("$%d, ", d->regs.r.rt);
            printf("$%d, ", d->regs.r.rs);
            unsigned int immmmm = d->regs.i.addr_or_immed;
            printf("$%d", immmmm);
            printf("\n");
        }
        else if(d->op == 13)
        {
            printf("ori");
            printf("\t");
            printf("$%d, ", d->regs.r.rt);
            printf("$%d, ", d->regs.r.rs);
            unsigned int immmmm = d->regs.i.addr_or_immed;
            printf("$%d", immmmm);
            printf("\n");
        }
        else if(d->op == 15)
        {
            printf("lui");
            printf("\t");
            printf("$%d, ", d->regs.r.rt);
            printf("$%d", d->regs.i.addr_or_immed);
            printf("\n");
        }
        else if(d->op == 35)
        {
            printf("lw");
            printf("\t");
            printf("$%d, ", d->regs.r.rt);
            printf("$%d", d->regs.i.addr_or_immed);
            printf("$%d, ", d->regs.r.rs);
            printf("\n");
        }
        else if(d->op == 43)
        {
            printf("sw");
            printf("\t");
            printf("$%d, ", d->regs.r.rt);
            printf("$%d", d->regs.i.addr_or_immed);
            printf("$%d, ", d->regs.r.rs);
            printf("\n");
        }
        else
        {
			printf("Invalid Instruction");
			exit(0);
		}
    }
    
}

/* Perform computation needed to execute d, returning computed value */
int Execute ( DecodedInstr* d, RegVals* rVals) {
    if(d->op == 0)
    {
        if(d->regs.r.funct == 0)//sll
        {
            return (mips.registers[d->regs.r.rt] << d->regs.r.shamt);
        }
        else if(d->regs.r.funct == 2)//srl
        {
            return (mips.registers[d->regs.r.rt] >> d->regs.r.shamt);
        }
        else if(d->regs.r.funct == 8)//jr
        {
            return (mips.registers[31]);
        }
        else if(d->regs.r.funct == 33)//addu
        {
            return (mips.registers[d->regs.r.rs] + mips.registers[d->regs.r.rt]);
        }
        else if(d->regs.r.funct == 35)//subu
        {
            return (mips.registers[d->regs.r.rs] - mips.registers[d->regs.r.rt]);
        }
        else if(d->regs.r.funct == 36)//and
        {
            return (mips.registers[d->regs.r.rs] & mips.registers[d->regs.r.rt]);
        }
        else if(d->regs.r.funct == 37)//or
        {
            return (mips.registers[d->regs.r.rs] | mips.registers[d->regs.r.rt]);
        }
        else if(d->regs.r.funct == 42)//slt
        {
            return (mips.registers[d->regs.r.rs] - mips.registers[d->regs.r.rt] < 0);
        }
        else
        {
            printf("Invalid Instruction");
			exit(0);
        }
        
    }
    else if (d->op == 2 || d->op == 3)
    {
        if(d->op == 3)//jal
        {
            return d->regs.j.target;
        }
        else if(d->op == 2)//j
        {
            return d->regs.j.target;
        }
        else
        {
            printf("Invalid Instruction");
			exit(0);
        }
    }
    else
    {
        if(d->op == 4)//beq
        {
            if(mips.registers[d->regs.i.rs] - mips.registers[d->regs.i.rt] == 0)
            {
                return 1;
            }
            else 
            {
                return 0;
            }
        }
        else if(d->op == 5)//bne
        {
            if(mips.registers[d->regs.i.rs] - mips.registers[d->regs.i.rt] != 0)
            {
                return 1;
            }
            else 
            {
                return 0;
            }
        }
        else if(d->op == 9)//addiu
        {
            return (mips.registers[d->regs.i.rs] + d->regs.i.addr_or_immed);
        }
        else if(d->op == 12)//andi
        {
            return (mips.registers[d->regs.i.rs] & d->regs.i.addr_or_immed);
        }
        else if(d->op == 13)//ori
        {
            return (mips.registers[d->regs.i.rs] | d->regs.i.addr_or_immed);
        }
        else if(d->op == 15)//lui
        {
            return ((d->regs.i.addr_or_immed << 16) & 0xFFFF0000);
        }
        else if(d->op == 35)//lw
        {
            return (mips.registers[d->regs.i.rs] - (d->regs.i.addr_or_immed/4+1)*4);
        }
        else if(d->op == 43)//sw
        {
            return (mips.registers[d->regs.i.rs] - (d->regs.i.addr_or_immed/4+1)*4);
        }
        else
        {
            printf("Invalid Instruction");
			exit(0);
        }
    }
   return 0;
}

/* 
 * Update the program counter based on the current instruction. For
 * instructions other than branches and jumps, for example, the PC
 * increments by 4 (which we have provided).
 */
void UpdatePC ( DecodedInstr* d, int val) {
    mips.pc = mips.pc + 4;
    if(d->op == 0)
    {
        if(d->regs.r.funct == 8)//jr
        {
            mips.pc = val;
        }
    }
    else if(d->op == 2 || d->op == 3)
    {
        mips.pc = val;//j & jal
    }
    else if(val == 1)
    {
        if(d->op == 4)//beq
        {
            mips.pc = mips.pc + ((d->regs.i.addr_or_immed)*4);
        }
        else if(d->op == 5)//bne
        {
            mips.pc = mips.pc + ((d->regs.i.addr_or_immed)*4);
        }
    }
}

/*
 * Perform memory load or store. Place the address of any updated memory 
 * in *changedMem, otherwise put -1 in *changedMem. Return any memory value 
 * that is read, otherwise return -1. 
 *
 * Remember that we're mapping MIPS addresses to indices in the mips.memory 
 * array. mips.memory[0] corresponds with address 0x00400000, mips.memory[1] 
 * with address 0x00400004, and so forth.
 *
 */


int Mem( DecodedInstr* d, int val, int *changedMem) {
    //only required for lw and sw
    *changedMem = -1;


    if(d->op == 35)
    {
        val = Fetch(val);
    }
    else if(d->op == 45)
    {
        int rt2 = mips.registers[d->regs.i.rt];
        mips.memory[(val - 0x00400000)/ 4] = rt2;
        *changedMem = val;
    }
    
    return val;
}

/* 
 * Write back to register. If the instruction modified a register--
 * (including jal, which modifies $ra) --
 * put the index of the modified register in *changedReg,
 * otherwise put -1 in *changedReg.
 */
void RegWrite( DecodedInstr* d, int val, int *changedReg) {
    if(d->op == 0)
    {
        if(d->regs.r.funct == 8)//jr since it doesn't change the register
        {
            *changedReg = -1;
        }
        else
        {
            mips.registers[d->regs.r.rd] = val;
            *changedReg = d->regs.r.rd;
        }
    }

    else if(d->op == 2)//j also doesn't change the register
    {
        *changedReg = -1;
    }
    else if(d->op == 3)
    {
        mips.registers[31] = (d->regs.j.target) - 4;
        *changedReg =31;
    }
    
    else if(d->op == 9 || d->op == 12 || d->op == 13 || d->op == 15 || d->op == 35)
    {
        mips.registers[d->regs.i.rt] = val;
        *changedReg = d->regs.i.rt;
    }
    else
    {
        *changedReg = -1;
    }
    
        
}
