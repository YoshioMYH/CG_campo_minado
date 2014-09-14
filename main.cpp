
#include <gl/glut.h>
#include <stdlib.h>
#include <stdio.h>

int GrausBraco1 = 0;
/* Funções  tratamento de eventos  */
struct tabuleiro{
    int linha;
    int coluna;
    int valor;
};


static void Quadrado()
{
    glBegin(GL_LINE_LOOP);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 1, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(1, 0, 0);
    glEnd();
}

void Atualiza_tamanho(int largura, int altura)
{
    glViewport(0, 0, largura, altura);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    //gluOrtho2D (-10.0f, 10.0f, -10.0f, 10.0f);
    gluOrtho2D (0.0f, 10.0f, -2.0f, 10.0f);
}

static void Atualiza_desenho(void)
{
    int linha=0, coluna=0; //Inicia variaveis para linha e coluna correspondentes ao tabuleiro
    int campo[100]; //Era para ser um vetor utilizado como cada quadrado da tabela
    glClear(GL_COLOR_BUFFER_BIT);

    for(linha=0; linha < 10 ; linha++ )//for para desenhar as linhas
    {
        for(coluna=0; coluna < 10 ; coluna++ )//for para desenhar as colunas
        {
            printf("x = %d, y = %d\n",linha ,coluna);//Printa no console as posições desenhadas
            glPushMatrix();
            glTranslatef(linha, coluna, 0);
            glColor3f(0.0f, 0.0f, 1.0f);
            Quadrado();
            glPopMatrix();
        }
    }
    glPushMatrix();//Quadrado para tempo
        glTranslatef(1.0, -1.1, 0);
        glScalef(2.0, 1.0, 0);
        glColor3f(1.0f, 0.0f, 0.0f);
        Quadrado();
    glPopMatrix();

    glPushMatrix();//Quadrado para bombas
        glTranslatef(7.0, -1.1, 0);
        glScalef(2.0, 1.0, 0);
        glColor3f(0.0f, 1.0f, 0.0f);
        Quadrado();
    glPopMatrix();

    glFlush();
}


static void teclado(unsigned char tecla, int x, int y)
{
    switch (tecla)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
        default:
            printf("\nNenhum evento atribuido a tecla\n");
            break;
    }
}

void mouse(int botao, int estado, int x, int y)
{
    switch ( botao ) {
        case GLUT_LEFT_BUTTON:
          printf("\n X: %d,  Y:  %d\n", x, y);
          break;
        case GLUT_RIGHT_BUTTON:
          printf("\n Botao para colocar bandeira\n");
          break;
    }
}

int main()
{
    glutInitWindowSize(700,700);
    glutInitWindowPosition(300,100);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE );

    glutCreateWindow("Campo Minado");

    glutDisplayFunc(Atualiza_desenho);
    glutReshapeFunc(Atualiza_tamanho);
    glutKeyboardFunc(teclado);
    glClearColor(1,1,1,1);
    glutMouseFunc(mouse);
    glutGet(GLUT_ELAPSED_TIME);
    glutMainLoop();

    return 0;
}
