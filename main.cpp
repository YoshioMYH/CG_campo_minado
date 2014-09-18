#include <gl/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <time.h>

// ESTRUTURAS, TIPOS CUSTOMIZADOS
struct Campo{
    bool campo_mina;            // Campo contém uma mina ?
    bool revelado;              // Campo revalado ?
    bool campo_protegido;       // Campo com Bandeira
    int minas_adja;             // Quantidade de Minas Adjacentes
    int pos_x;                  // Posição X do Campo
    int pos_y;                  // Posição Y do Campo
};

// VARIAVEIS GLOBAIS, colocar um 'G_'
GLint windowsSize_x = 600;
GLint windowsSize_y = 600;
int G_linhas = 0;
int G_colunas = 0;
int G_linha = 0;
int G_coluna = 0;
int G_minas = 0;
int G_pos_x = 0;
int G_pos_y = 0;

int G_timer = 0;
int G_dificuldade = 0;

std::vector<Campo> campo_minado;                                            // Similar a um ArrayList [Java] para representar o Campo Minado

// FUNÇÕES
template <typename T>
std::string to_string(T value){     //função to_string criada na mão
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

static void iniciaCampos();                                            //
static void AcrescentaMina();                                          // Funcionando
static void CampoMinasAdjacentes();                                    //
static void MenuTemporario();                                          //
static void renderText(const char *text, int length, int x, int y);    // Funcionando
static void mostraTempo(int value);                                    // Funcionando
static void mostraMinas();                                             // Funcionando
static void acrescentaMarcacao(const char *opcao, int x, int y);       // não funcionando, estou procurando arrumar - By Alex
static void Quadrado();                                                // Funcionando
static void Revelado();                                                // Funcionando
static void Tabuleiro(int linhas, int colunas, int pos_x, int pos_y);  // Funcionando [Incrementar]
static void Atualiza_tamanho(int largura, int altura);                 // Funcionando
static void Atualiza_desenho(void);                                    // Funcionando [Incrementar]
static void teclado(unsigned char tecla, int x, int y);                // Funcionando
static void mouse(int botao, int estado, int x, int y);                // Funcionando [Incrementar]

static void iniciaCampos(){
    int i = 0;
    int j = 0;
    int cont = 0;
    for(i = 0; i < G_linha; i++){                                 //Inicia todos os quadrados
        for(j = 0; j < G_coluna; j++){
            campo_minado.push_back(Campo());
            campo_minado[cont].campo_mina = false;
            campo_minado[cont].revelado = false;
            campo_minado[cont].campo_protegido = false;
            campo_minado[cont].minas_adja = 0;
            campo_minado[cont].pos_x = i;                   //Arrumar para marcar corretamente
            campo_minado[cont].pos_y = j;                   //Arrumar para marcar corretamente
            //printf("\nCampo: %d  Valor: %d  Posx: %d  Posy: %d", cont, campo_minado[cont].minas_adja, campo_minado[cont].pos_x, campo_minado[cont].pos_y);
            cont++;
        }
    }

}

static void AcrescentaCoordenada(int controlador, int posqx, int posqy){
    campo_minado.push_back(Campo());
    campo_minado[controlador].pos_x = posqx;
    campo_minado[controlador].pos_y = posqy;
    printf("\n[DEBUG]: Campo: %d , Pos x: %d , Pos y: %d ", controlador, posqx, posqy);
}

static void  AcrescentaMina(){
    srand(time(NULL));
    int i = 0;
    int minimo = 0;
    int maximo = 0;
    int cont = 0;
    maximo = (G_linha * G_linha)-1;
    while(cont != G_minas){
        //printf("\n\n[DEBUG]: Cont: %d  Minas: %d", cont, G_minas);
        i = rand()%(maximo-minimo+1)+minimo;
        campo_minado.push_back(Campo());
        if(campo_minado[i].campo_mina == false){
            campo_minado[i].campo_mina = true;
            cont++;
        }
        //printf("\n[DEBUG]:i: %d  Campo: %d  Mina? : %d", i, i, campo_minado[i].campo_mina);
    }
}

static void CampoMinasAdjacentes(){

}

void MenuTemporario(){
    printf("Iniciando Menu Temporario \nEscolha a dificuldade(0-Noob, 1-Menos Noob, 2-SabeUmPouco): \n");
    scanf("%d", &G_dificuldade);
    switch(G_dificuldade){
        case 0:
            G_linha = 5;
            G_coluna = 5;
            G_minas = 5;
            break;
        case 1:
            G_linha = 10;
            G_coluna = 10;
            G_minas = 15;
            break;
        case 2:
            G_linha = 15;
            G_coluna = 15;
            G_minas = 30;
            break;
        default:
            G_linha = 5;
            G_coluna = 5;
            G_minas = 5;
            break;
    }
}

static void renderText(const char *text, int length, int x, int y){ // Função para renderizar o texto
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

static void mostraTempo(int value){ //Função para mostrar o tempo decorrido
    std::string text;
    G_timer = glutGet(GLUT_ELAPSED_TIME);
    text = to_string(G_timer/1000); //Converte o G_timer para uma string
    glColor3f(0.0, 0.0, 0.0); //Seta a cor do texto como preto
    renderText("Tempo Decorrido", 15, 25, 6);
    renderText(text.data(), text.size(), 32, 2);
    //glutPostRedisplay();
    //glutTimerFunc(1000,mostraTempo, 1);
}

static void mostraMinas(){ // Função para mostra a quantidade de bombas iniciais em campo.
    std::string text;
    text = to_string(G_minas);
    renderText("Minas", 5, 60, 6);
    renderText(text.data(), text.size(), 63, 2);
}

static void Quadrado(){     // Campo em branco, o tamanho atual do campo é de X = 5% do windowsSize_x | Y = 5% do windowsSize_y
    glBegin(GL_LINE_LOOP);
        glVertex2f(0, 0);
        glVertex2f(0, 1);
        glVertex2f(1, 1);
        glVertex2f(1, 0);
    glEnd();
}

static void Revelado(){     // Campo revelado, o tamanho atual do campo é de X = 5% do windowsSize_x | Y = 5% do windowsSize_y
    glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(0, 1);
        glVertex2f(1, 1);
        glVertex2f(1, 0);
    glEnd();
}

static void Tabuleiro(int linhas, int colunas, int pos_x, int pos_y){
    int linha;          // iterar nas linhas
    int coluna;         // iterar nas colunas
    int marcadorLinha;  // posicao x de onde comecar a desenhar
    int marcadorColuna; // posicao y de onde comecar a desenhar
    int controlador = 0;// controla o qaudrado atual na iteração, é utilizado por causa do vector

    switch(G_dificuldade) // verifica a G_dificuldade e atribui os marcadores de linha e coluna
    {
        case 0:
            marcadorLinha = (linhas - 2) * -1;
            marcadorColuna = (colunas -2) * -1;
            break;
        case 1:
            marcadorLinha = (linhas - 5) * -1;
            marcadorColuna = (colunas -5) * -1;
            break;
        case 2:
            marcadorLinha = (linhas - 8) * -1;
            marcadorColuna = (colunas -8) * -1;
            break;
        default:
            marcadorLinha = (linhas - 2) * -1;
            marcadorColuna = (colunas -2) * -1;
            break;
    }

    for(linha = marcadorLinha; linha < linhas + marcadorLinha; linha++)                     // itera nas linhas
    {
        for(coluna = marcadorColuna; coluna < colunas + marcadorColuna; coluna++)           // itera nas colunas da linha
        {
            if(G_timer != 0)                                                                // if temporario para que nao revele logo no inicio algum campo
            {
                if((pos_x > linha * (windowsSize_x * 0.05)) &&                              // Verificao do clique no eixo X
                   (pos_x < linha * (windowsSize_x * 0.05) + (windowsSize_x * 0.05)))       // 0.05: relembrar que o quadrado tera 5% do tamanho da tela
                {
                    if((pos_y > coluna * (windowsSize_y * 0.05)) &&                         // Verificao do clique no eixo Y
                       (pos_y < coluna * (windowsSize_y * 0.05) + (windowsSize_y * 0.05)))  // 0.05: relembrar que o quadrado tera 5% do tamanho da tela
                    {
                        printf("\nRevelar Quadrado X: %d, Y: %d\n", linha, coluna);
                        glPushMatrix();
                           glTranslated(linha, coluna, 0);
                            glColor3f(1.0f, 0.0f, 0.0f);
                            Revelado();
                        glPopMatrix();
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
            else                                                                            // else temporario
            {
                glPushMatrix();
                glTranslated(linha, coluna, 0);
                glColor3f(0.0f, 0.0f, 1.0f);
                Quadrado();
                glPopMatrix();
            }
            //AcrescentaCoordenada(controlador, pos_x, pos_y);
            //controlador++;
        }
    }

    int valor = 0;
}

static void Atualiza_tamanho(int largura, int altura){
    glViewport(0, 0, largura, altura);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D (-10.0f, 10.0f, -10.0f, 10.0f);
    //gluOrtho2D (0.0f, 10.0f, 12.0f, 0.0f);


    printf("\n[DEBUG] : Evento Atualiza tamanho\n");
}

static void Atualiza_desenho(void){
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
        Tabuleiro(G_linha, G_coluna, G_pos_x, G_pos_y);
    glPopMatrix();

    glPushMatrix();     //Quadrado para tempo
        glTranslatef(-6.0, -10.0, 0);
        glScalef(5.0, 1.8, 0);
        glColor3f(1.0f, 0.0f, 0.0f);
        Quadrado();
    glPopMatrix();

    glPushMatrix();     //Quadrado para bombas
        glTranslatef(0.0, -10.0, 0);
        glScalef(5.0, 1.8, 0);
        glColor3f(0.0f, 1.0f, 0.0f);
        Quadrado();
    glPopMatrix();

    mostraTempo(1000);
    mostraMinas();
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

static void mouse(int botao, int estado, int x, int y){
    if(botao == GLUT_LEFT_BUTTON){
        if(estado == GLUT_DOWN){
            printf("\n[DEBUG]: Apertou botao esquerdo mouse");

            G_pos_x = x - windowsSize_x / 2;                    // ajusta a posição do mouse para combinar com o viewport, posicao x real - janela X / 2
            G_pos_y = windowsSize_y / 2 - y ;                   // ajusta a posição do mouse para combinar com o viewport, anela Y / 2 - posicao y real
            printf("\n X: %d,  Y:  %d\n", G_pos_x, G_pos_y);

            glutPostRedisplay();
        }
        if(estado == GLUT_UP){
            printf("\n[DEBUG]: Soltou botao esquerdo mouse");
        }
    }else if(botao == GLUT_RIGHT_BUTTON){
        if(estado == GLUT_DOWN){
            if(G_minas > 0){
                G_minas--;
                mostraMinas();
            }
            printf("\n[DEBUG]: Apertou Botao direito mouse");
            glutPostRedisplay();
        }
        if(estado == GLUT_UP){
            printf("\n[DEBUG]: Soltou botao direito mouse");
        }
    }else{
        printf("\n[DEBUG]: Nao entendo o que vc quer clicar, pare de apertar esse botao por favor");
    }
}

int main(){
    MenuTemporario();
    glutInitWindowSize(windowsSize_x, windowsSize_y);
    glutInitWindowPosition(300, 0);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE );

    glutCreateWindow("Campo Minado");

    iniciaCampos();
    AcrescentaMina();

    glutDisplayFunc(Atualiza_desenho);
    glutReshapeFunc(Atualiza_tamanho);

    glutKeyboardFunc(teclado);
    glutMouseFunc(mouse);

    G_timer = glutGet(GLUT_ELAPSED_TIME);
    glutTimerFunc(1000, mostraTempo, 1);

    glClearColor(1,1,1,1);
    glutMainLoop();

    return 0;
}
