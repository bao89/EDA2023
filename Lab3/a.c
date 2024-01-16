#include "util.h"
#include "cudd.h"
#include <stdio.h>
#include <stdlib.h>

void write_dd(DdManager *gbm, DdNode *dd, char* filename)
{
	FILE *outfile; // output file pointer for .dot file
	outfile = fopen(filename, "w");
	DdNode **ddnodearray = (DdNode**)malloc(sizeof(DdNode*)); // initialize the function array
	ddnodearray[0] = dd;
	Cudd_DumpDot(gbm, 1, ddnodearray, NULL, NULL, outfile); // dump the function to .dot file
	free(ddnodearray);
	fclose(outfile); // close the file */
}
int main(int argc, char *argv[])
{
	FILE *in;
	FILE *out;
	char abc;
	in = fopen(argv[1], "r");
	out = fopen(argv[2], "w");

	DdManager *gbm; /* Global BDD manager. */
	char filename[30];
	gbm = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0); /* Initialize a new BDD manager. */
	DdNode *bdd1, *tmp, *tmp_neg, *var;
	bdd1 = Cudd_ReadOne(gbm);
	Cudd_Ref(bdd1);
	abc = fgetc(in);
	//¥ý«Ø²Ä¤@³¡¤À
	while (abc != '+'&&abc != '.')
	{
		if (abc > 96)//¤p¼g
		{
			var = Cudd_bddIthVar(gbm, abc - 97);
			tmp = Cudd_bddAnd(gbm, var, bdd1);
			Cudd_Ref(tmp);
			Cudd_RecursiveDeref(gbm, bdd1);
			bdd1 = tmp;
		}
		else
		{
			var = Cudd_bddIthVar(gbm, abc - 65);
			tmp_neg = Cudd_Not(var); /*Perform NOT Boolean operation*/
			tmp = Cudd_bddAnd(gbm, tmp_neg, bdd1); /*Perform AND Boolean operation*/
			Cudd_Ref(tmp);
			Cudd_RecursiveDeref(gbm, bdd1);
			bdd1 = tmp;
		}
		abc = fgetc(in);
	}
	//­Y¦³²Ä¤G³¡¤À
	if (abc != '.')
	{
		while (abc != '.')
		{
			abc = fgetc(in);
			DdNode *bdd2;
			bdd2 = Cudd_ReadOne(gbm);
			Cudd_Ref(bdd2);

			while (abc != '+'&&abc != '.')
			{
				if (abc > 96)//¤p¼g
				{
					var = Cudd_bddIthVar(gbm, abc - 97);
					tmp = Cudd_bddAnd(gbm, var, bdd2);
					Cudd_Ref(tmp);
					Cudd_RecursiveDeref(gbm, bdd2);
					bdd2 = tmp;
				}
				else
				{
					var = Cudd_bddIthVar(gbm, abc - 65);
					tmp_neg = Cudd_Not(var); /*Perform NOT Boolean operation*/
					tmp = Cudd_bddAnd(gbm, tmp_neg, bdd2); /*Perform AND Boolean operation*/
					Cudd_Ref(tmp);
					Cudd_RecursiveDeref(gbm, bdd2);
					bdd2 = tmp;
				}
				abc = fgetc(in);
			}
			Cudd_Ref(bdd1);
			bdd1 = Cudd_bddOr(gbm, bdd1, bdd2);
		}
	}
	Cudd_Ref(bdd1);
	int* order;
	int count = 0;

	while( (abc = fgetc(in))!=EOF)//¦Y´«¦æ
	{
		abc = fgetc(in);//¦Y²Ä¤@­Ó¦r
		//printf("%c", abc);
		order = malloc(sizeof(int));
		order[0] = abc - 97;
		count = 1;
		abc = fgetc(in);//¦Y²Ä¤G­Ó¦r
		while (abc != '.')
		{
			//printf("%c", abc);
			count++;
			order = realloc(order, count * sizeof(int));
			order[count - 1] = abc - 97;
			abc = fgetc(in);
		}
		Cudd_ShuffleHeap(gbm, order);
   if(Cudd_DagSize(bdd1)>1)
  		fprintf(out, "%d\n", Cudd_DagSize(bdd1)+1);
	  else
     fprintf(out, "%d\n", Cudd_DagSize(bdd1));
  }
	//sprintf(filename, "a.dot"); /*Write .dot filename to a string*/
	//write_dd(gbm, bdd1, filename);  /*Write the resulting cascade dd to a file*/
	//fprintf(out, "%d\n", Cudd_DagSize(bdd1));
	//bdd1 = Cudd_BddToAdd(gbm, bdd1); /*Convert BDD to ADD for display purpose*/

	Cudd_Quit(gbm);
	fclose(in);
	fclose(out);
	return 0;

}
