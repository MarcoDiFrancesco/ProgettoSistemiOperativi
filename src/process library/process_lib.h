#include "counter_new.h"

//argc, argv[]: argomenti da passare a terminale
void c(int argc, string argv[], int N, int M);

//M: numero dei processi Q da creare
//pipePQ: pipe di comunicazione tra questo processo Q e il suo padre P
//pipeCP: pipe di comunicazione tra questo processo P e il suo padre C
//n_proc: numero del processo P creato da C
//dimpart: dimensione della partizione da esaminare
//files: lista dei file da esaminare
int p(int M, int pipePQ[][2], int pipeCP[][2], int n_proc, int dimpart, string files[]);

//pipePQ: pipe di comunicazione tra questo processo Q e il suo padre P
//dimpart: dimensione della partizione da esaminare
//section: indice della partizione (n° processo Q)
//files: lista dei file da esaminare
int q(int pipePQ[][2], int dimpart, int section, string files[]);