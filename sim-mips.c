// List the full names of ALL group members at the top of your code.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
//feel free to add here any additional library names you may need 
#define SINGLE 1
#define BATCH 0
#define REG_NUM 32

long pgm_c=0;
long mips_reg[REG_NUM];
enum opcode{add, addi, sub, mul, lw, sw, beq};
        struct Instruction{
                enum opcode code;
                long rs;
                long rt;
                long rd;
		long Imm;
        };

        struct Instruction Imem[256];

        //Latches

        struct latches{

                enum opcode inOpcode;
                long inValue1;
                long inValue2;
                long inValue3;
		long inValue4;

                enum opcode outOpcode;
                long outValue1;
                long outValue2;
                long outValue3;
        };

 
        struct latches IFtoID_latch;

        struct latches IDtoEX_latch;

        struct latches EXtoMEM_latch;

        struct latches MEMtoWB_latch;

        //Start of stages

        void IF(){
		IFtoID_latch.inOpcode=Imem[pgm_c/4].code;
		IFtoID_latch.inValue1=Imem[pgm_c/4].rs;
		IFtoID_latch.inValue2=Imem[pgm_c/4].rt;
		IFtoID_latch.inValue3=Imem[pgm_c/4].rd;
		IFtoID_latch.inValue4=Imem[pgm_c/4].Imm;
        }

	void ID(){
		long dstReg;
		long srcReg1;
		long srcReg2;
		long Imm;

		if(IFtoID_latch.inOpcode==0 | IFtoID_latch.inOpcode==1 || IFtoID_latch.inOpcode==2){
			 dstReg=IFtoID_latch.inValue1; //$rd
			 srcReg1=mips_reg[IFtoID_latch.inValue2];//contents of $rs
			 srcReg2=mips_reg[IFtoID_latch.inValue3];//contents of $rt
			  
         		 IDtoEX_latch.inValue1=dstReg;
             		 IDtoEX_latch.inValue2=srcReg1;
           	 	 IDtoEX_latch.inValue3=srcReg2;

		}
    		IDtoEX_latch.inOpcode=IFtoID_latch.inOpcode;
	}
	void EX(){
		
		if(IDtoEX_latch.inOpcode==0){
			EXtoMEM_latch.inValue2=IDtoEX_latch.inValue2 + IDtoEX_latch.inValue3; //send $rs+$rt out 
			EXtoMEM_latch.inValue1=IDtoEX_latch.inValue1; //address for result to be stored
		}
		EXtoMEM_latch.inOpcode=IDtoEX_latch.inOpcode; //transfer opcode
	}

	void MEM(){
		if(EXtoMEM_latch.inOpcode==0){
			MEMtoWB_latch.inValue1=EXtoMEM_latch.inValue1;//transfer destination address
			MEMtoWB_latch.inValue2=EXtoMEM_latch.inValue2;//transfer result
		}
		MEMtoWB_latch.inOpcode=EXtoMEM_latch.inOpcode; //transfer opcode
	}

	void WB(){
		if(MEMtoWB_latch.inOpcode==0){
		mips_reg[MEMtoWB_latch.inValue1]=MEMtoWB_latch.inValue2;
		
		}
	}

	char *regNumberConverter(char *string){
		char * items[5]; //each item seperately
                char * tok; //item seperated by delimiter
             	char c[sizeof(int)]; 
		int i=0;
		char* money;
		int boolReg=0;
		int boolFound=0;
		tok = strtok(string," "); 
		while(tok!=NULL){
			items[i]=tok;
			tok=strtok(NULL," ");
        		i++;	
		 }
		for(int j=1;j<4;j++){
			money=items[j];
			boolReg=0;
			boolFound=0;
			if(money[0]=='$'){
				boolReg=1;
			}		
			char* name[] = {"zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7", "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"};
			char* nums[] = {"0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31"};
			for(int i=0;i<32;i++){
				if(strcmp(name[i],items[j]+1)==0 || strcmp(nums[i],items[j]+1)==0){
					snprintf(c, sizeof(int), "%d", i);
					items[j]=malloc(sizeof(c));
					strcpy(items[j],c);
					boolFound=1;
				}
			}
			if(boolReg==1 && boolFound==0){
				//haltSimulation
				printf("halt\n");
			}
		}

		char * converted= (char *) malloc(1+strlen(items[0])+strlen(items[1])+strlen(items[2])+strlen(items[3]));
		strcpy(converted,items[0]);
		strcat(converted," ");
		strcat(converted,items[1]);
		strcat(converted," ");
		strcat(converted,items[2]);
		strcat(converted," ");
		strcat(converted,items[3]);
		printf("%s\n",converted);
	
		return converted;
	}
	struct Instruction *parser(char *output){
		char *fields[4];
		char *tok;
		tok = strtok(string," ");
                while(tok!=NULL){
                         fields[i]=tok;
                         tok=strtok(NULL," ");
                         i++;
			printf(fields[i]);
                 }
	}

int main (int argc, char *argv[]){
	int sim_mode=0;//mode flag, 1 for single-cycle, 0 for batch
	int c,m,n;
	int i;//for loop counter
	long sim_cycle=0;//simulation cycle counter
	//define your own counter for the usage of each pipeline stage here
	
/*	int test_counter=0;
	FILE *input=NULL;
	FILE *output=NULL;
	printf("The arguments are:");
	
	for(i=1;i<argc;i++){
		printf("%s ",argv[i]);
	}
	printf("\n");
	if(argc==7){
		if(strcmp("-s",argv[1])==0){
			sim_mode=SINGLE;
		}
		else if(strcmp("-b",argv[1])==0){
			sim_mode=BATCH;
		}
		else{
			printf("Wrong sim mode chosen\n");
			exit(0);
		}
		
		m=atoi(argv[2]);
		n=atoi(argv[3]);
		c=atoi(argv[4]);
		input=fopen(argv[5],"r");
		output=fopen(argv[6],"w");
		
	}
	
	else{
		printf("Usage: ./sim-mips -s m n c input_name output_name (single-sysle mode)\n or \n ./sim-mips -b m n c input_name  output_name(batch mode)\n");
		printf("m,n,c stand for number of cycles needed by multiplication, other operation, and memory access, respectively\n");
		exit(0);
	}
	if(input==NULL){
		printf("Unable to open input or output file\n");
		exit(0);
	}
	if(output==NULL){
		printf("Cannot create output file\n");
		exit(0);
	}
	//initialize registers and program counter
	if(sim_mode==1){*/
		for (i=0;i<REG_NUM;i++){
			mips_reg[i]=0;
		}
//}
/*	Imem[0].opcode=0;
	Imem[0].value1=10;
	mips_reg[0]=2;
	Imem[0].value2=0;
	mips_reg[1]=3;
	Imem[0].value3=1;
	IF();
	ID();
	EX();
	MEM();
	WB();
	printf("%ld",mips_reg[10]);
	printf("\n");
*/
        char string[] = "add $t7 4 $zero";
	char* convertee=regNumberConverter(string);
	parser(convertee);
		
}
