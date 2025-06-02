#include <GL/glut.h>
#include <cstdlib> //bilbioteca que contem o rand() e o srand()
#include <ctime>

/*
	OBJETIVOS na colisão:
		. troca de direção OK
		. troca de cor OK
		. aumento de tamanho OK
	
 */

float quadradox = 0.0;
float quadradoy = 0.0;

float mover = 0.01f;
float tamanho = 0.2;// representa o lado do quadrado

//cores separadas para poder trocar a numeração delas
float red = 1.0f;
float green = 1.0f;
float blue = 1.0f;


void redimensiona(int largura, int altura){
	
	//define a dimensao da janela ultilizada para desenhar
	glViewport(0, 0, largura, altura);
	
	//define que as transformacoes afetarao a projecao
	glMatrixMode(GL_PROJECTION);
	
	//reseta a matriz da projecao
	glLoadIdentity();
	
	//Define uma projecao ortgonal
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	
	//Define que as operacoes afetam a modelagem e a visao
	glMatrixMode(GL_MODELVIEW);
}

void desenho(){
	
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(red, green, blue);
	
	//salva a matriz da transfomacao
	glPushMatrix();
	//move o objeto pela cena	
	glTranslatef(quadradox, quadradoy, 0.0f);
		
		glBegin(GL_QUADS);
			glVertex2f(-tamanho / 2.0f, -tamanho / 2.0f); // Vértice inferior esquerdo
    		glVertex2f( tamanho / 2.0f, -tamanho / 2.0f); // Vértice inferior direito
    		glVertex2f( tamanho / 2.0f,  tamanho / 2.0f); // Vértice superior direito
    		glVertex2f(-tamanho / 2.0f,  tamanho / 2.0f); // Vértice superior esquerdo
    glEnd();
		
		//Restaura a matriz da transformacao anterior
		glPopMatrix();
		
	//troca os buffers para exibir o desenho
	glutSwapBuffers();
	//Executa os comandos opengl
	glFlush();
	
}

void movimento(int valor){
	//atualiza a posicao do quadrado
	quadradox += mover;
	
	//muda a direcao do movimento ao bater na borda
	if (quadradox > 1.0f || quadradox < -1.0f) {
		mover *= -1;
		
		//aleatoriedade para a mudança das cores
		//rand() -> gera numero aleatorio entre 0.0 e 1.0(limitado pelo RAND_MAX)
		red = (float)rand() / RAND_MAX;
		green = (float)rand() / RAND_MAX;
		blue = (float)rand() / RAND_MAX;
		
		//aumento do tamanho até 50% do seu tamanho
		if (tamanho < 30.0) {
			tamanho = tamanho + (tamanho / 100);
		}
	}
	//define que a janela sea redesenhada
	glutPostRedisplay();
	//intervalo par aa chamada da funcao
	glutTimerFunc(16, movimento, 0);
}

void iniciar(){
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
}

int main(int argc, char** argv){
	
	//iniciam o gerador de numeros baseado no tempo atual
	srand(time(NULL));
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Trabalho Estácio");
	
	iniciar();
	glutDisplayFunc(desenho);
	glutReshapeFunc(redimensiona);
	glutTimerFunc(16, movimento, 0);
	
	glutMainLoop();
}
