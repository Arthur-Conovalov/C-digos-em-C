#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Desenvolver um programa em linguagem C que implemente um sistema básico de cadastro de alunos persistente, 
permitindo as seguintes operações:
.Adicionar aluno -OK
.Listar alunos cadastrados -OK
.Editar informações de um aluno -OK
.Apagar registro de um aluno -OK
*/

//constantes
#define NOME_TAMANHO 50
#define NOME_ARQUIVO "cadastro_alunos.txt"

//estrutura para as variaveis do produto
typedef struct{
	char nome[50];
	char curso[100];
	int matricula;
}Aluno;

Aluno lista[100]; //array prar até 100 alunos

void cadastrar(FILE *arquivo){
	Aluno novo;
	
	char buffer[10]; // para ler a matricula como string
	
	printf("\n ---CADASTRO DE ALUNO---\n");
	printf("Nome: ");
	//limita a entrada de dados a 49 caracteres até a quebra de linha
	fgets(novo.nome, sizeof(novo.nome), stdin);
	novo.nome[strcspn(novo.nome, "\n")] = 0;
	
	printf("Curso: ");
	fgets(novo.curso, sizeof(novo.curso), stdin);
	novo.curso[strcspn(novo.curso, "\n")] = 0;
	
	printf("Matricula: ");
	fgets(buffer, sizeof(buffer), stdin); // recebe a variavel da matriculça como string
	novo.matricula = atoi(buffer); // converte a variavel em string para int
	
	//gravando no arquivo
	fprintf(arquivo, "Aluno: %s\nCurso: %s\nMatricula: %d\n\n", 
	novo.nome, novo.curso, novo.matricula);
	
	printf("\nAluno Cadastrado!\n");
	
	
}

void listar(FILE *arquivo){
	
	printf("\n---LISTA DE ALUNOS---\n");
	char linha[100];//armazena os dados de cada linha
	
	while(fgets(linha, sizeof(linha), arquivo) != NULL){
		
		printf("%s", linha);
		
	}
	
}

int carregarAlunos(Aluno lista[], int tamanhoMaximo, const char *nomeArquivo){
	// r para somente leitura 
	FILE *arquivo = fopen(nomeArquivo, "r");
	
	if (arquivo == NULL){
		
		printf("Arquivo não encontrado.");
		return 1;
		
	}
	
	int contador = 0;
	char linha[100];
	while (contador < tamanhoMaximo && fgets(linha, sizeof(linha), arquivo) != NULL){
		//OBS: os if's estão aninhados para melhor segurança na ordem dos dados.
		//lê nome
		if(strncmp(linha, "Aluno: ", 7) == 0){ 
			//copia o nome
			strcpy(lista[contador].nome, linha + 7);
			// remove o "\n"
			lista[contador].nome[strcspn(lista[contador].nome, "\n")] = 0; 
			
			//verifica a leitura da próxima linha do arquivo
			if(fgets(linha, sizeof(linha), arquivo) == NULL) break;
			
			//lê o curso
			if(strncmp(linha, "Curso: ", 7) == 0){
				//copia o curso
				strcpy(lista[contador].curso, linha + 7);
				// remove o "\n"
				lista[contador].curso[strcspn(lista[contador].curso, "\n")] = 0; 
				
				//verifica a leitura da próxima linha do arquivo
				if(fgets(linha, sizeof(linha), arquivo) == NULL) break;
				//lê matricula
				if(strncmp(linha, "Matricula: ", 11) == 0){
					//converte o valor da matricula para int
					lista[contador].matricula = atoi(linha +11);
					contador++;
					
					}
				}
			}
		}
		
		fclose(arquivo);
    return contador; // quantos alunos foram carregados
	}
	
	void editar(){
		
		int total = carregarAlunos(lista, 100, NOME_ARQUIVO);
		int matriculaAluno;
		char buffer[10]; //buffer da matricula
		
		//procura o aluno pela matricula
		printf("Digite a matricula do aluno que dejesa editar: ");
		fgets(buffer, sizeof(buffer), stdin);
		matriculaAluno = atoi(buffer);//string -> int
		
		int encontrado = 0;
		for(int i = 0; i < total; i++){
			
			if(lista[i].matricula == matriculaAluno){
				printf("Novo Nome: ");//editar o nome
				fgets(lista[i].nome, sizeof(lista[i].nome), stdin);
				lista[i].nome[strcspn(lista[i].nome, "\n")] = 0;
				
				printf("Novo Curso: ");//editar o curso
				fgets(lista[i].curso, sizeof(lista[i].curso), stdin);
				lista[i].curso[strcspn(lista[i].curso, "\n")] = 0;
				
				encontrado = 1;
				break;
			}
		}
		
		if(!encontrado){
			printf("Aluno não encontrado\n");
			return;	
		}
		
		//sobrescrevendo o arquivo com o aluno editado
		FILE *arquivo = fopen(NOME_ARQUIVO, "w"); 
		
		for(int i = 0; i<total; i++){
			
			fprintf(arquivo, "Aluno: %s\nCurso: %s\nMatricula: %d\n\n", 
			lista[i].nome, lista[i].curso, lista[i].matricula);
		}
		fclose(arquivo);
		
		printf("Aluno editado com sucesso!\n");		
	}
	
	void apagar(){
		
		Aluno lista[100];
		int total = carregarAlunos(lista, 100, NOME_ARQUIVO);
		
		if(total == 0){
			printf("Nenhum Aluno Cadastrado. \n");
			return;
		}
		
		int matriculaAluno;
		printf("Digite a matricula do aluno a ser removido: ");
		scanf("%d", &matriculaAluno);
		
		FILE *arquivoNovo = fopen("temp.txt", "w");
		if(arquivoNovo == NULL){
			printf("Erro ao criar arquivo temporário. \n");
			return;
		}
		
		int removido = 0;
		int i;
		for(i =0; i < total; i++){
		if(lista[i].matricula == matriculaAluno){
            removido = 1; // aluno encontrado e removido
        } else {
		fprintf(arquivoNovo, "Aluno: %s\nCurso: %s\nMatricula: %d\n\n",
                lista[i].nome, lista[i].curso, lista[i].matricula);
		}
	}
		fclose(arquivoNovo);
		
		if(removido){
		remove(NOME_ARQUIVO); 
		rename("temp.txt", NOME_ARQUIVO);
		printf("Aluno removido com sucesso. \n");	
		}else{
		remove("temp.txt"); //não encontrou, então descarta o novo
		printf("Aluno com matricula %d não encontrado. \n", matriculaAluno);
		}
	}

	
	
	
	
	
int main(){
	
	int opcao;
	//tenta abrir/criar o arquivo
	FILE *arquivo;
	
	//a+ permite adicionar ao final do arquivo e ler o conteudo
	//arquivo = fopen(NOME_ARQUIVO, "a+");
	
	if(arquivo == NULL){
		
		printf("Erro ao abrir o arquivo");
		return 1;
	}
	
	do{
		printf("\n--- MENU DE CADASTRO DE ALUNOS ---\n");
		printf("\t1- CADASTRAR\n");
		printf("\t2- LISTA\n");
		printf("\t3- EDITAR\n");
		printf("\t4- APAGAR\n");
		printf("\t0- Sair\n");
		printf("Escolha uma opcao: \n");
		scanf("%d", &opcao);
		getchar(); // para "consumir" a quebra de linha "\n" e não atrapalhar na hora do menu
		
		 switch(opcao){
        case 1: {
            FILE *arquivo = fopen(NOME_ARQUIVO, "a");
            if(arquivo == NULL){
                printf("Erro ao abrir o arquivo\n");
                break;
            }
            cadastrar(arquivo);
            fclose(arquivo);
            break;
        }
        case 2: {
            FILE *arquivo = fopen(NOME_ARQUIVO, "r");
            if(arquivo == NULL){
                printf("Arquivo vazio ou não encontrado\n");
                break;
            }
            listar(arquivo);
            fclose(arquivo);
            break;
        }
        case 3:
            editar();
            break;
        case 4:
            apagar();
            break;
        case 0:
            printf("Saindo do programa\n");
            break;
        default:
            printf("Opcao invalida.\n");
    }
} while(opcao != 0);
	

	fclose(arquivo);
}
