/***************************************************************************************************************************
Header Files
****************************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <assert.h>

/***************************************************************************************************************************
Constant Declarations 
****************************************************************************************************************************/
#define DELIMITER 1
#define VARIABLE 2
#define NUMBER 3
#define MAX_LINE	100

char token[80];
char tok_type;
char *prog;
/***************************************************************************************************************************
Structure Declarations 
****************************************************************************************************************************/
typedef struct struct_node
{
	char nd_num;
	int refe;
	int oper;
	int asap;
	int Nasap;
	int alap;
	int Fasap;
	
} node;

/***************************************************************************************************************************
Function Declarations 
****************************************************************************************************************************/
void get_token(void);
int isdelim(char);

/***************************************************************************************************************************
  Main Function
****************************************************************************************************************************/

void main(int argc,char **argv) 
{
FILE *infile, *resfile;
node graph[100];

int i=0, counter, Mnode, Mnode_id=0;
int mult_count=0, alu_count=0, oper_count=0;
int C=1, min_asap, flag;
int Nasap[100], Fasap[100];
char line[100], buf[100];

infile = fopen(argv[1], "r");

while(fgets(buf, MAX_LINE, infile)!='\0')
  {
	prog=buf;
	counter=0;
	do
	{
		get_token();
		switch(tok_type)
		{
			case NUMBER:
				if (counter==0)
				{
				graph[i].nd_num= atoi(token);
				if (Mnode_id < graph[i].nd_num) Mnode_id = graph[i].nd_num;
				}
				if (counter ==1)
				{
				graph[i].refe = atoi(token);
				}
				if (counter == 2)
				{
				graph[i].asap = atoi(token);
				if (i==0)	min_asap = graph[i].asap;
				else if(graph[i].asap < min_asap)	min_asap = graph[i].asap;
				}
				if (counter==3)
				{
				graph[i].alap = atoi(token);
				}
				counter++;
			break;

			case VARIABLE:
				if (counter < 2)
				{
					if(! strcmp(token, "source"))
					{
					graph[i].oper=0;
					}
					if (! strcmp (token, "sink"))
					{
					graph[i].oper=3;
					}
				}
			break;

			case DELIMITER:
				if (counter > 0 && counter < 2)	
				{	
					if (strchr("*", *token)) //this condition is for multiplication
					{
					graph[i].oper=1;
					mult_count++;
					}
					if (strchr("+-/%^=()<>",*token))  //this condition is for all, except multiplication
					{
					graph[i].oper=2;
					alu_count++;
					}
				}
			break;



			default:
			break;
		}
	}
	while (counter < 4);
	Mnode=i;
	i++;
	
  } 		//by the end of this line I have all the node/operation done, now i can use them inside my conditions

	resfile = fopen ("equations.lp", "w");
/*
	fprintf(resfile, "nodenumber\t operation\t referance\t asap\t alap");
	fprintf(resfile, "\n");
	for (i=0; i<=Mnode; i++)
		{
		fprintf (resfile, "\t%d \t %d \t\t %d \t\t %d \t %d", graph[i].nd_num, graph[i].oper, graph[i].refe, graph[i].asap, graph[i].alap);
		fprintf(resfile, "\n");
		}

		
/*My code start*/
//printf("mult_count= %d", mult_count);
fprintf(resfile, "min: a1 + a2;\n");

for (i=0; i<=Mnode; i++)
{
	if ( graph[i].oper !=0 && graph[i].oper != 3)
	{
		fprintf(resfile, "C%d: ",graph[i].nd_num );
		if (graph[i].asap == graph[i].alap )
		{
			fprintf(resfile,"X%d%d ", graph[i].nd_num, graph[i].asap-min_asap );
		}
		
		if (graph[i].asap != graph[i].alap )
		{
			for (int j= graph[i].asap-min_asap ; j <= graph[i].alap-min_asap ; j++)
			{
				fprintf(resfile,"X%d%d ", graph[i].nd_num, j );
				if ( j < graph[i].alap-min_asap )	fprintf(resfile, " + ");
			}
		}
		fprintf(resfile, "=%d;\n", graph[i].refe );
	}
	//specialcase
	if ( graph[i].oper == 3 )
	{
		fprintf(resfile, "C%d: X%d%d =%d;\n \n", Mnode, Mnode, graph[i].asap-min_asap, graph[i].refe);
	}
}

/*************************************************************
-----------------------equations for MULT----------------------
**************************************************************/

for (i=0; i<=Mnode; i++)
{
	if (graph[i].oper != 3 | graph[i].oper != 1 )
	{
		Nasap[i]= graph[i].asap - min_asap;
		Fasap[i]= graph[i].alap - graph[i].asap  ;
	}
}


fprintf(resfile, "\n");
int sameasap =1; 
flag=1;
for (int k =1; k<=Mnode; k++)
{
fprintf(resfile, "C%d:",Mnode+sameasap);
	for (i=0; i<=Mnode; i++)
	{
		if ( graph[i].oper == 1)
		{
						
			if ( Nasap[i] == sameasap && graph[i].asap == graph[i].alap )
			{
				fprintf(resfile, " X%d%d ", graph[i].nd_num, sameasap );
				fprintf(resfile, "+");
				flag=0;
			}
				
			if ( graph[i].asap != graph[i].alap && Nasap[i] == sameasap  && Fasap[i] >=0 )
			{
				fprintf(resfile, "X%d%d", graph[i].nd_num, sameasap );
				if ( graph[i].asap-min_asap < graph[i].alap-min_asap )fprintf(resfile, " + ");
				Nasap[i]++;
				Fasap[i]--;
				flag=0;
			}
				 
		}
		
	}
if (flag==0)
	{
		fprintf(resfile, " -a1<= 0;\n");
	}
sameasap++;
flag=1;
}

/***************************************************************
----------------------equations for ALU-------------------------
****************************************************************/
for (i=0; i<=Mnode; i++)
{
	if (graph[i].oper != 3 | graph[i].oper != 1 )
	{
		Nasap[i]= graph[i].asap - min_asap;
		Fasap[i]= graph[i].alap - graph[i].asap  ;
	}
}

int plus=Mnode+sameasap-1;
fprintf(resfile, "\n\n");
sameasap =1;
flag=1;
for (int  k =1; k<=alu_count; k++)
{
fprintf(resfile, "C%d:",plus+sameasap);
	for (i=0; i<=Mnode; i++)
	{
		if ( graph[i].oper == 2)
		{
			if ( Nasap[i] == sameasap && Fasap[i] == 0  )
			{
				fprintf(resfile, " X%d%d ", graph[i].nd_num,sameasap  );
				fprintf(resfile, "+");
				flag=0;
			}
			
			if ( graph[i].asap != graph[i].alap && Nasap[i] == sameasap  && Fasap[i] > 0 )
			{
				fprintf(resfile, " X%d%d ", graph[i].nd_num, sameasap );
				if ( graph[i].asap-min_asap < graph[i].alap-min_asap )fprintf(resfile, " + ");
				Nasap[i]++;
				Fasap[i]--;
				flag=0;
			}
		}
		
	}
if (flag==0)
	{
		fprintf(resfile, " -a2<= 0;\n");
	}
sameasap++;
flag=1;
}


/***********************************************************************************
---------------------this section is for integer portion---------------------------
************************************************************************************/
fprintf(resfile, "\n\n");
fprintf(resfile, "int a1, a2");
for (i=1; i<=Mnode; i++)
{
	if (graph[i].asap != graph[i].alap)
	{
		for (int j= graph[i].asap-min_asap ; j <= graph[i].alap -graph[i].asap+1 ; j++)
		{
			fprintf (resfile,", X%d%d", graph[i].nd_num, j );
		}
	}
if( i == Mnode ) fprintf(resfile, ";");
}

/*My code ends*/

// 1. Read the benchmark files(.asapalap) and store it in your own defined data structure 
// 2. Form the linear equations using the information in the data structure and print in a file(ilpfile) 
// 3. Use lp solver to solve the inequalities and store the output of lpsolver in file.
      //lpsover command: lp_solve "ilpfile" 
// 4. The two functions(get_token,isdelim) can be used for reading the benchmark files(.asapalap). 
      //Using these two functions in your implementation is optional. 


fclose(infile);
fclose(resfile);
}//end of main



/***************************************************************************************************************************
 Read a group of characters together return as a single string refered as token
(This piece code is taken form the Text Book: C: The complte reference by Herbert Schildt(4th Edition) page no:592) 
****************************************************************************************************************************/
void get_token(void)
{
register char *temp;

tok_type = 0;
temp = token;
*temp = '\0';
//printf("here");
if(!*prog) return; /* at end of expression */

while(isspace(*prog))  ++prog; /* skip over the white space */

if(strchr("<+-*/%^=()", *prog) ){
  tok_type = DELIMITER;
  /* advance to the next char */
  *temp++ = *prog++;  }

else if( isalpha(*prog) ){
  while((!isdelim(*prog))&&(!isspace(*prog))  )
    *temp++ = *prog++;
    tok_type = VARIABLE;  }
	
else if( isdigit(*prog) ) {
  while( (!isdelim(*prog))&&(!isspace(*prog)) )
    *temp++ = *prog++;
    tok_type = NUMBER;   }
*temp = '\0';
}//end
/***************************************************************************************************************************
 Return true if c is a delimiter
(This piece code is taken form the Text Book: C: The complte reference by Herbert Schildt(4th Edition) page no:593) 
****************************************************************************************************************************/
int isdelim(char c)
{
if( strchr("<+-/*%^=()", c) || c==9 || c==0 || c=='\r' )  return 1;
return 0;
}//end
/***************************************************************************************************************************/
