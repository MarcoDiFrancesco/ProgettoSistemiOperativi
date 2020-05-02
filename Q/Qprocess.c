#include "Qprocess.h"

int readFile(char* filename, char* filedata, int start, int stop){
    int sk, rd;
    int file = open(filename, O_RDONLY);
    if(file>=0){
        sk = lseek(file, start*sizeof(char), SEEK_SET);
        if(sk != -1){
            rd = read(file, filedata, stop-start);
        }
    }

    lseek(file, 0, SEEK_END);
    if(file>=0 && sk>=0 && rd>=0) return 0;
    else return -1;
}

void countLetters(int dim, char* s, int* counter){
    printf("testo cont: ");
    int i = dim-1;
    for(i; i>=0; i--){
        
        printf("%c\t", s[i]);
        if(s[i]>='a' && s[i]<='z' || s[i]>='A' && s[i]<='Z'){
            //lettera maiuscola o minuscola
            counter[0]++;
        }else{
            if(s[i]>='0' && s[i]<='9'){
                //numero
                counter[1]++;
            }else{
                if(s[i]==' '){
                    //spazi
                    counter[2]++;
                }else{
                    if(s[i]=='.' || s[i]==',' || s[i]==':' || s[i]==';' || s[i]=='?' || s[i]=='!'){
                        //punteggiatura
                        counter[3]++;
                    }else{
                        //tutto il resto
                        counter[4]++;
                    }
                }
            }      
        }
    }
}

int* processoQ(int from, int to, char* fname){
    char* testo;
    int* stats;
    int i, 
        inizio = from, 
        fine = to;
    int size;

    testo = malloc(fine*sizeof(char));
    stats = malloc(5*sizeof(int));
    i = 0;

    for(i; i<5; i++){
        stats[i] = 0;
    }

    i = readFile(fname, testo, inizio, fine);

    countLetters(size-inizio, testo, stats);

    if(i==0)
        return stats;
    else 
        return -1;
}