#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

//nel main passare l'indirizzo della variabile da valorizzare (&var)

//setta alla variabile int passata come primo argomento con il valore del secondo
//utile per passare le funzioni della libreria <errno.h>
//restituisce il valore dell'errore
int set_error(int *ret, int value);

//esegue la funzone set_error e stampa la stringa passata come argomento
int set_err_msg(int *ret, int value, char* msg);