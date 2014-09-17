#include <gl/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <conio.h>
// ESTRUTURAS, TIPOS CUSTOMIZADOS
struct Campo{
    bool campo_mina;            // Campo cont�m uma mina ?
    bool revelado;              // Campo revalado ?
    int minas_adja;             // Quantidade de Minas Adjacentes
    int pos_x;                  // Posi��o X do Campo
    int pos_y;                  // Posi��o Y do Campo
};

// VARIAVEIS GLOBAIS
int G_linhas = 0;
int G_colunas = 0;
int linha, coluna;
int G_pos_x = 0;
int G_pos_y = 0;

int timer = 0;
int dificuldade = 0;
std::vector<Campo> campo_minado;                                            // Similar a um ArrayList [Java] para representar o Campo Minado

// FUN��ES
template <typename T>
std::string to_string(T value){//fun��o to_string criada na m�o
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}

void renderText(const char *text, int length, int x, int y); // Funcionando
void mostraTempo(int value);        // Funcionando
void mostraBombas(); // Funcionando
void acrescentaMarcacao(const char *opcao, int x, int y); // n�o funcionando, estou procurando arrumar - By Alex


void MenuTemporario(){
    printf("Iniciando Menu Temporario \nEscolha a dificuldade(0-Noob, 1-Menos Noob, 2-SabeUmPouco): \n");
    scanf("%d", &dificuldade);
    if(dificuldade == 0){
        linha = 5;
        coluna = 5;
    }else if(dificuldade == 1){
        linha = 10;
        coluna = 10;
    }else if(dificuldade == 2){
        linha = 20;
        coluna = 20;
    }else {
        linha = 20;
        coluna = 20;
    }
}


void renderText(const char *text, int length, int x, int y){ // Fun��o para renderizar o texto
    glMatrixMode(GL_PROJECTION);
    double *matrix = new double[16];
    glGetDoublev(GL_PROJECTION_MATRIX, matrix);
    glLoadIdentity();
    glOrtho(0, 100, 0, 100, -5, 5);
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

void mostraTempo(int value){ //Fun��o para mostrar o tempo decorrido
    std::string text;
    timer = glutGet(GLUT_ELAPSED_TIME);
    text = to_string(timer/1000); //Converte o timer para uma string
    glColor3f(0.0, 0.0, 0.0); //Seta a cor do texto como preto
    renderText("Tempo Decorrido", 15, 25, 6);
    renderText(text.data(), text.size(), 32, 2);
    //glutPostRedisplay();
    //glutTimerFunc(1000,mostraTempo, 1);
}

void mostraBombas(){
    renderText("Minas", 5, 60, 6);
    if(dificuldade == 0){
        renderText("5", 1, 63, 2);
    }else if(dificuldade == 1){
        renderText("15", 2, 62, 2);
    }else if(dificuldade == 2){
        renderText("30", 2, 62, 2);
    }else {
        renderText("None Selected", 13, 57, 2);
    }
}
void acrescentaMarcacao(const char *opcao, int x, int y){
    std::string text;
    text = opcao; //Converte o timer para uma string
    printf("\n[DEBUG] : Evento seta op��o: %s\n", text.data());
    glColor3f(0.0, 0.0, 0.0); //Seta a cor do texto como preto
    renderText(text.data(), text.size(), x, y);

}

// VERIFICAR SE IR� SER MANTIDO
static void MenuPrincipal(int operador){
    switch( operador )
    {
        case 1:
            //fun��o bandeira;
            break;
        case 2:
            //funca��o interroga��o;
            break;
    }

    glutPostRedisplay();
}

// VERIFICAR SE IR� SER MANTIDO
static void CriaMenus(){
    int menu;

    menu = glutCreateMenu(MenuPrincipal);
    glutAddMenuEntry("Bandeira", 1);
    glutAddMenuEntry("Interroga��o", 2);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

static void Quadrado(){         // Com o atual ViewPort, gerar� um quadrado com 70 x 70 pixels
    glBegin(GL_LINE_LOOP);
        glVertex2i(0, 0);
        glVertex2i(0, 1);
        glVertex2i(1, 1);
        glVertex2i(1, 0);
    glEnd();
}

static void Revelado(){           // Com o atual ViewPort, gerar� um quadrado com 70 x 70 pixels
    glBegin(GL_QUADS);
        glVertex2i(0, 0);
        glVertex2i(0, 1);
        glVertex2i(1, 1);
        glVertex2i(1, 0);
    glEnd();
}

static void Tabuleiro(int linhas, int colunas, int pos_x, int pos_y){
    int linha, coluna, marcadorLinha, marcadorColuna;
    if(linhas == 5){
        marcadorLinha = (linhas - 2) * -1;
        marcadorColuna = (colunas -2) * -1;
    }else if(linhas == 10){
        marcadorLinha = (linhas - 5) * -1;
        marcadorColuna = (colunas -5) * -1;
    }else{
        marcadorLinha = (linhas - 10) * -1;
        marcadorColuna = (colunas -10) * -1;
    }


    for(linha = marcadorLinha; linha < linhas+ marcadorLinha; linha++)
    {
        for(coluna = marcadorColuna; coluna < colunas+ marcadorColuna; coluna++)
        {
            //printf("x = %d, y = %d\n",linha ,coluna); //Printa no console as posi��es desenhadas
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

void Atualiza_tamanho(int largura, int altura){
    glViewport(0, 0, 820, 820);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluOrtho2D (-10.0f, 10.0f, -12.0f, 12.0f);
    //gluOrtho2D (0.0f, 10.0f, 12.0f, 0.0f);


    printf("\n[DEBUG] : Evento Atualiza tamanho\n");
}

static void Atualiza_desenho(void){
    //int linha = 20;     // Inicia variaveis para linhas correspondentes ao tabuleiro
    //int coluna = 20;    // Inicia variaveis para colunas correspondentes ao tabuleiro
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
        Tabuleiro(linha, coluna, G_pos_x, G_pos_y);
    glPopMatrix();

    glPushMatrix();//Quadrado para tempo
        glTranslatef(-6.0, -12.0, 0);
        glScalef(5.0, 1.8, 0);
        glColor3f(1.0f, 0.0f, 0.0f);
        Quadrado();
    glPopMatrix();

    glPushMatrix();//Quadrado para bombas
        glTranslatef(0.0, -12.0, 0);
        glScalef(5.0, 1.8, 0);
        glColor3f(0.0f, 1.0f, 0.0f);
        Quadrado();
    glPopMatrix();

    mostraTempo(1000);
    mostraBombas();
    glFlush();

    //printf("\n[DEBUG] : Evento Atualiza desenho\n");
}


static void teclado(unsigned char tecla, int x, int y){
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

void mouse(int botao, int estado, int x, int y){
    if(botao == GLUT_LEFT_BUTTON){
        if(estado == GLUT_DOWN){
            printf("\n[DEBUG]: Apertou botao esquerdo mouse");
            acrescentaMarcacao("teste", x, y);
            printf("\n X: %d,  Y:  %d\n", x, y);
            G_pos_x = x;
            G_pos_y = y;
            glutPostRedisplay();
        }
        if(estado == GLUT_UP){
            printf("[DEBUG]: Soltou botao esquerdo mouse");
        }
    }else if(botao == GLUT_RIGHT_BUTTON){
        if(estado == GLUT_DOWN){
            printf("\n[DEBUG]: Apertou Botao direito mouse");
            printf("\n[DEBUG]: Botao para colocar bandeira\n");
        }
        if(estado == GLUT_UP){
            printf("\n[DEBUG]: Soltou botao direito mouse");
        }
    }else{
        printf("Nao entendo o que vc quer clicar");
    }
}

int main(){
    MenuTemporario();
    glutInitWindowSize(820,820);
    glutInitWindowPosition(300,0);
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
