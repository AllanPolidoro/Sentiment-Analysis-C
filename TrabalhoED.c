//Author: Allan dos Santos Polidoro

//Sentiment Analysis

#define MAX_WORD 256
#define MAX_LINE 2048

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int ABP_cont_CONSULTA = 0;
int AVL_cont_CONSULTA = 0;

typedef struct InfoA{
    char word[MAX_WORD];
    int polarity;
}TipoInfo;

typedef struct TNodoABP{
    TipoInfo info;
    struct TNodoABP *esq;
    struct TNodoABP *dir;
}pNodoABP;

typedef struct TNodoAVL{
    TipoInfo info;
    int FB;
    struct TNodoAVL *esq;
    struct TNodoAVL *dir;
}pNodoAVL;

void SentAnalysisABP(char lex[MAX_WORD], char sent[MAX_WORD], char output[MAX_WORD]);
void SentAnalysisAVL(char lex[MAX_WORD], char sent[MAX_WORD], char output[MAX_WORD]);
pNodoABP* InicializaABP();
pNodoAVL* InicializaAVL();
pNodoABP* InsereABP(pNodoABP *a, TipoInfo ch);
pNodoAVL* InsereAVL(pNodoAVL *a, TipoInfo key, int* ok);
pNodoAVL* Caso1 (pNodoAVL* a , int *ok);
pNodoAVL* Caso2 (pNodoAVL *a , int *ok);
pNodoAVL* rotacao_direita(pNodoAVL *pt);
pNodoAVL* rotacao_esquerda(pNodoAVL *pt);
pNodoAVL* rotacao_dupla_direita (pNodoAVL* pt);
pNodoAVL* rotacao_dupla_esquerda (pNodoAVL* pt);
int consultaABP(pNodoABP *a, char chave[MAX_WORD]);
int consultaAVL(pNodoAVL *a, char chave[MAX_WORD]);

int main(int argc, char *argv[]){ //argc conta o número de parâmetros e argv armazena as strings correspondentes aos parâmentros digitados
    int tipoA = 0;
    char lex[MAX_WORD], sent[MAX_WORD], output[MAX_WORD];

    strcpy (lex, argv[1]);
    strcpy (sent, argv[2]);
    strcpy (output, argv[3]);

    if (argc!=4){  //numero de parametros esperado é 4: nome do programa (argv[0]), nome do arq léxico (argv[1]), nome do arq de sentenças(argv[2]), nome do arq de saida(argv[3]).
        printf ("Número incorreto de parametros.\n Para executar o programa digite: Sentiment_Analysis lexico.txt sentencas.txt saida.txt\n");
        return 1;
    }
    else{
        while (!tipoA){
            printf ("Escolha o tipo da aplicacao (1-ABP ; 2-AVL): ");
            scanf ("%d", &tipoA);

            if(tipoA == 1)
                SentAnalysisABP(lex, sent, output);
            else{
                if(tipoA == 2)
                    SentAnalysisAVL(lex, sent, output);
                else{
                    tipoA = 0;
                    printf ("Tipo de aplicacao invalido.\n");
                }
            }
        }
    }
    return 0;
}

pNodoABP* InicializaABP(){
    return NULL;
}

pNodoAVL* InicializaAVL(){
    return NULL;
}

pNodoABP* InsereABP(pNodoABP *a, TipoInfo ch){
    if (a == NULL){
        a = (pNodoABP*) malloc(sizeof(pNodoABP));
        a->info = ch;
        a->esq = NULL;
        a->dir = NULL;
        return a;
    }
    else{
        if (strcmp(ch.word, a->info.word) < 0)
            a->esq = InsereABP(a->esq,ch);
        else
            a->dir = InsereABP(a->dir,ch);
    }
    return a;
}

pNodoAVL* InsereAVL(pNodoAVL *a, TipoInfo x, int *ok){
    if (!a){
        a = (pNodoAVL*) malloc(sizeof(pNodoAVL));
        a->info = x;
        a->esq = NULL;
        a->dir = NULL;
        a->FB = 0;
	    *ok = 1;
    }
    else{
        if (strcmp(x.word, a->info.word) < 0){
            a->esq = InsereAVL(a->esq,x,ok);
            if (*ok){
                switch (a->FB) {
                    case -1:  a->FB = 0; *ok = 0; break;
                    case  0:  a->FB = 1;  break;
                    case  1:  a = Caso1(a,ok); break;
                }
            }
        }
        else{
            a->dir = InsereAVL(a->dir,x,ok);
            if (*ok){
                switch (a->FB) {
                    case  1:  a->FB = 0; *ok = 0; break;
                    case  0:  a->FB = -1; break;
                    case -1:  a = Caso2(a,ok); break;
                }
            }
        }
    }
    return a;
}

pNodoAVL* Caso1 (pNodoAVL* a , int *ok){
    pNodoAVL *ptu;

    ptu = a->esq;
	if (ptu->FB == 1)
        a = rotacao_direita(a);
    else
        a = rotacao_dupla_direita(a);
    a->FB = 0;
	*ok = 0;
	return a;
}

pNodoAVL* Caso2 (pNodoAVL *a , int *ok){
    pNodoAVL *ptu;

	ptu = a->dir;
	if (ptu->FB == -1)
       a=rotacao_esquerda(a);
    else
       a=rotacao_dupla_esquerda(a);
	a->FB = 0;
	*ok = 0;
	return a;
}

pNodoAVL* rotacao_direita(pNodoAVL *pt){
    pNodoAVL* ptu;

    ptu = pt->esq;
    pt->esq = ptu->dir;
    ptu->dir = pt;
    pt->FB = 0;
    pt = ptu;
    return pt;
}

pNodoAVL* rotacao_esquerda(pNodoAVL *pt){
    pNodoAVL* ptu;

    ptu = pt->dir;
    pt->dir = ptu->esq;
    ptu->esq = pt;
    pt->FB = 0;
    pt = ptu;
    return pt;
}

pNodoAVL* rotacao_dupla_direita (pNodoAVL* pt){
    pNodoAVL* ptu, *ptv;

    ptu = pt->esq;
    ptv = ptu->dir;
    ptu->dir = ptv->esq;
    ptv->esq = ptu;
    pt->esq = ptv->dir;
    ptv->dir = pt;
    if (ptv->FB == 1)   pt->FB = -1;
        else pt->FB = 0;
    if (ptv->FB == -1)  ptu->FB = 1;
        else ptu->FB = 0;
    pt = ptv;
    return pt;
}

pNodoAVL* rotacao_dupla_esquerda (pNodoAVL* pt){
    pNodoAVL *ptu, *ptv;

    ptu = pt->dir;
    ptv = ptu->esq;
    ptu->esq = ptv->dir;
    ptv->dir = ptu;
    pt->dir = ptv->esq;
    ptv->esq = pt;
    if (ptv->FB == -1) pt->FB = 1;
        else pt->FB = 0;
    if (ptv->FB == 1) ptu->FB = -1;
        else ptu->FB = 0;
    pt = ptv;
    return pt;
}

int consultaABP(pNodoABP *a, char chave[MAX_WORD]){
    while (a){
        ABP_cont_CONSULTA++;
        ABP_cont_CONSULTA++;
        if (!strcmp(chave, a->info.word))
            return a->info.polarity;
        else{
            ABP_cont_CONSULTA++;
            if (strcmp(chave, a->info.word) < 0)
                a = a->esq;
            else
                a = a->dir;
        }
    }
    return 0; //se não achou
}

int consultaAVL(pNodoAVL *a, char chave[MAX_WORD]){
    while (a){
        AVL_cont_CONSULTA++;
        AVL_cont_CONSULTA++;
        if (!strcmp(chave, a->info.word))
            return a->info.polarity;
        else{
            AVL_cont_CONSULTA++;
            if (strcmp(chave, a->info.word) < 0)
                a = a->esq;
            else
                a = a->dir;
        }
    }
    return 0; //se não achou
}

void SentAnalysisABP(char lex[MAX_WORD], char sent[MAX_WORD], char output[MAX_WORD]){
    int i = 0, polarLine = 0;
    int TOTALcompCons = 0;
    int TOTALpolar = 0;
    char temp_line[MAX_LINE], temp_word[MAX_WORD], *word;
    char separador[] = {" ,.&*%\?!;/-'@\"$#=><()][}{:\n\t"};
    TipoInfo temp;
    pNodoABP *t;

    t = InicializaABP();

    FILE *F_lex;
    FILE *F_sent;
    FILE *F_output;

    if(!(F_lex = fopen(lex, "r"))){
        printf ("\nErro ao abrir o lexico.");
        return;
    }
    else{
        if(!(F_sent = fopen(sent, "r"))){
            printf ("\nErro ao abrir o arquivo de sentencas.");
            return;
        }
        else{
            if(!(F_output = fopen(output, "w"))){
                printf ("\nErro ao abrir o arquivo de saida.");
                return;
            }
            else{
                while(!feof(F_lex)){
                    if(fgets(temp_line, MAX_WORD, F_lex)){
                        strcpy(temp.word, strtok(temp_line, " \t"));
                        temp.polarity = atoi(strtok(NULL, " \n"));
                        t = InsereABP(t, temp);
                    }
                }
                while(!feof(F_sent)){
                    polarLine = 0;
                    if(fgets(temp_line, MAX_LINE, F_sent)){
                        fputs(temp_line, F_output);
                        fprintf(F_output,"\n");
                        for(i = 0; i < strlen(temp_line); i++)
                            temp_line[i] = tolower(temp_line[i]);
                        word = strtok(temp_line, " .,-\n");
                        while(word){
                            strcpy(temp_word, word);
                            polarLine += consultaABP(t, temp_word);
                            word = strtok(NULL, " .,-\n");
                        }
                        TOTALcompCons += ABP_cont_CONSULTA;
                        TOTALpolar += polarLine;
                        fprintf(F_output,"Polaridade: %d\t\tComparacoes: %d\n", polarLine, ABP_cont_CONSULTA);
                        for(i = 0; i < 60; i++)
                            fprintf(F_output,"-");
                        fprintf(F_output,"\n");
                        ABP_cont_CONSULTA = 0;
                    }
                }
                for(i = 0; i < 60; i++)
                    fprintf(F_output,"=");
                fprintf(F_output,"\n\nPolaridade Total: %d\nComparacoes TOTAIS: %d\n\n", TOTALpolar, TOTALcompCons);
                for(i = 0; i < 60; i++)
                    fprintf(F_output,"=");
                fclose(F_output);
            }fclose(F_sent);
        }fclose(F_lex);
    }
}

void SentAnalysisAVL(char lex[MAX_WORD], char sent[MAX_WORD], char output[MAX_WORD]){
    int i = 0, polarLine = 0, *ok;
    int TOTALcompCons = 0;
    int TOTALpolar = 0;
    char temp_line[MAX_LINE], temp_word[MAX_WORD], *word;
    char separador[] = {" ,.&*%\?!;/-'@\"$#=><()][}{:\n\t"};
    TipoInfo temp;
    pNodoAVL *t;

    t = InicializaAVL();

    FILE *F_lex;
    FILE *F_sent;
    FILE *F_output;

    if(!(F_lex = fopen(lex, "r"))){
        printf ("\nErro ao abrir o lexico.");
        return;
    }
    else{
        if(!(F_sent = fopen(sent, "r"))){
            printf ("\nErro ao abrir o arquivo de sentencas.");
            return;
        }
        else{
            if(!(F_output = fopen(output, "w"))){
                printf ("\nErro ao abrir o arquivo de saida.");
                return;
            }
            else{
                while(!feof(F_lex)){
                    if(fgets(temp_line, MAX_WORD, F_lex)){
                        strcpy(temp.word, strtok(temp_line, " \t"));
                        temp.polarity = atoi(strtok(NULL, " \n"));
                        t = InsereAVL(t, temp, ok);
                    }
                }
                while(!feof(F_sent)){
                    polarLine = 0;
                    if(fgets(temp_line, MAX_LINE, F_sent)){
                        fputs(temp_line, F_output);
                        fprintf(F_output,"\n");
                        for(i = 0; i < strlen(temp_line); i++)
                            temp_line[i] = tolower(temp_line[i]);
                        word = strtok(temp_line, " .,-\n");
                        while(word){
                            strcpy(temp_word, word);
                            polarLine += consultaAVL(t, temp_word);
                            word = strtok(NULL, " .,-\n");
                        }
                        TOTALcompCons += AVL_cont_CONSULTA;
                        TOTALpolar += polarLine;
                        fprintf(F_output,"Polaridade: %d\t\tComparacoes: %d\n", polarLine, AVL_cont_CONSULTA);
                        for(i = 0; i < 60; i++)
                            fprintf(F_output,"-");
                        fprintf(F_output,"\n");
                        AVL_cont_CONSULTA = 0;
                    }
                }
                for(i = 0; i < 60; i++)
                    fprintf(F_output,"=");
                fprintf(F_output,"\n\nPolaridade Total: %d\nComparacoes TOTAIS: %d\n\n", TOTALpolar, TOTALcompCons);
                for(i = 0; i < 60; i++)
                    fprintf(F_output,"=");
                fclose(F_output);
            }fclose(F_sent);
        }fclose(F_lex);
    }
}

