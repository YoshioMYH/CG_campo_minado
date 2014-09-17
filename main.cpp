#include <gl/glut.h>

#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <iostream>
#include <sstream>

#include <vector>

// ESTRUTURAS, TIPOS CUSTOMIZADOS
struct Campo
{
    bool campo_mina;            // Campo contém uma mina ?
    bool revelado;              // Campo revalado ?
    int minas_adja;             // Quantidade de Minas Adjacentes
    int pos_x;                  // Posição X do Campo
    int pos_y;                  // Posição Y do Campo
};

// VARIAVEIS GLOBAIS
int G_linhas = 0;
int G_colunas = 0;

int G_pos_x = 0;
int G_pos_y = 0;

int timer = 0;

std::vector<Campo> campo_minado;                                            // Similar a um ArrayList [Java] para representar o Campo Minado

// FUNÇÕES
template <typename T>
std::string to_string(T value)//função to_string criada na mão
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}

void renderText(const char *text, int length, int x, int y){ // Função para renderizar o texto
    glMatrixMode(GL_PROJECTION);
    double *matrix = new double[16];
    glGetDoublev(GL_PROJECTION_MATRIX, matrix);
    glLoadIdentity();
    glOrtho(0, 800, 0, 600, -5, 5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2i(x, y);
    for(int i=0; i<length; i++){
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(matrix);
    glMatrixMode(GL_MODELVIEW);
}

void mostraTempo(int value){ //Função para mostrar o tempo decorrido
    std::string text;
    timer = glutGet(GLUT_ELAPSED_TIME);
    text = to_string(timer/1000); //Converte o timer para uma string
    glColor3f(0.0, 0.0, 0.0); //Seta a cor do texto como preto
    renderText(text.data(), text.size(), 135, 65);
    glutPostRedisplay();
    glutTimerFunc(1000,mostraTempo, 1);

}

void acrescentaMarcacao(const char *opcao, int x, int y){
    std::string text;
    text = opcao; //Converte o timer para uma string
    printf("\n[DEBUG] : Evento seta opção: %s\n", text.data());
    glColor3f(0.0, 0.0, 0.0); //Seta a cor do texto como preto
    renderText(text.data(), text.size(), x, y);

}

// VERIFICAR SE IRÁ SER MANTIDO
static void MenuPrincipal(int operador)
{
    switch( operador )
    {
        case 1:
            //função bandeira;
            break;
        case 2:
            //funcação interrogação;
            break;
    }

    glutPostRedisplay();
}

// VERIFICAR SE IRÁ SER MANTIDO
static void CriaMenus()
{
    int menu;

    menu = glutCreateMenu(MenuPrincipal);
    glutAddMenuEntry("Bandeira", 1);
    glutAddMenuEntry("Interrogação", 2);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

static void Quadrado()         // Com o atual ViewPort, gerará um quadrado com 70 x 70 pixels
{
    glBegin(GL_LINE_LOOP);
        glVertex2i(0, 0);
        glVertex2i(0, 1);
        glVertex2i(1, 1);
        glVertex2i(1, 0);
    glEnd();
}

static void Revelado()           // Com o atual ViewPort, gerará um quadrado com 70 x 70 pixels
{
    glBegin(GL_QUADS);
        glVertex2i(0, 0);
        glVertex2i(0, 1);
        glVertex2i(1, 1);
        glVertex2i(1, 0);
    glEnd();
}

static void Tabuleiro(int linhas, int colunas, int pos_x, int pos_y)
{
    int linha = 0;
    int coluna = 0;

    for(linha = 0; linha < linhas; linha++)
    {
        for(coluna = 0; coluna < colunas; coluna++)
        {
            //printf("x = %d, y = %d\n",linha ,coluna); //Printa no console as posições desenhadas
            //printf("\nPos_x: %d Pos_y: %d\n", pos_x, pos_y);
            if(pos_x > 0 && pos_y > 0)
            {
                if((pos_x >= linha * 70) &&
                   (pos_x <  linha * 70 + 70))
                {
                    if(pos_y >= coluna * 70 &&
                        pos_y <  coluna * 70 + 70)
                    {
                        printf("\nRevelar Quadrado X: %d, Y: %d\n", linha, coluna);
                        glPushMatrix();
                            glTranslated(linha, coluna, 0);
                            glColor3f(0.0f, 0.0f, 1.0f);
                            Revelado();
                        glPopMatrix();
                        G_pos_x = 0;
                        G_pos_y = 0;
                    }

                    else
                    {
                        glPushMatrix();
                        glTranslated(linha, coluna, 0);
                        glColor3f(0.0f, 0.0f, 1.0f);
                        Quadrado();
                        glPopMatrix();
                    }
                }

                else
                {
                    glPushMatrix();
                    glTranslated(linha, coluna, 0);
                    glColor3f(0.0f, 0.0f, 1.0f);
                    Quadrado();
                    glPopMatrix();
                }
            }
            else
            {
                glPushMatrix();
                glTranslated(linha, coluna, 0);
                glColor3f(0.0f, 0.0f, 1.0f);
                Quadrado();
                glPopMatrix();
            }
        }
    }

    int valor = 0;
};

void Atualiza_tamanho(int largura, int altura)
{
    glViewport(0, 0, largura, altura);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    //gluOrtho2D (-10.0f, 10.0f, -10.0f, 10.0f);
    gluOrtho2D (0.0f, 10.0f, -2.0f, 10.0f);

    printf("\n[DEBUG] : Evento Atualiza tamanho\n");
}

static void Atualiza_desenho(void)
{
    int linha = 11;     // Inicia variaveis para linhas correspondentes ao tabuleiro
    int coluna = 11;    // Inicia variaveis para colunas correspondentes ao tabuleiro
    int campo[100];     // Era para ser um vetor utilizado como cada quadrado da tabela
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
        Tabuleiro(linha, coluna, G_pos_x, G_pos_y);
    glPopMatrix();

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

    mostraTempo(1000);

    glFlush();

    //printf("\n[DEBUG] : Evento Atualiza desenho\n");
}


static void teclado(unsigned char tecla, int x, int y)
{
    switch (tecla)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
        case 't': //atualizar tempo
            glutPostRedisplay();
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
            acrescentaMarcacao("teste", x, y);
            printf("\n X: %d,  Y:  %d\n", x, y);
            G_pos_x = x;
            G_pos_y = y;
            glutPostRedisplay();
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

    timer = glutGet(GLUT_ELAPSED_TIME);
    glutTimerFunc(1000, mostraTempo, 1);
    CriaMenus();

    glutMainLoop();

    return 0;
}
