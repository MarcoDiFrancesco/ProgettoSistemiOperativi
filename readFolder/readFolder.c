#include <stdlib.h>
#include <stdio.h>
#include <sys/file.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

int main(){
	char* result;
	int i;
	
	
	
	system("cd ./folder && ls >> elenco.txt");
	system("cd ./folder && mv elenco.txt ..");
	

	int file = open("elenco.txt", O_RDONLY);	
	
	int dim = lseek(file, 0, SEEK_END);
	result = malloc(dim);
	lseek(file, 0, SEEK_SET);
	
	if(read(file, result, dim)<0){
		printf("--->cazzi");	
	}
	
	
	for(i=0; i<dim; i++){
		printf("%c", result[i]);
	}

	return 0;
}