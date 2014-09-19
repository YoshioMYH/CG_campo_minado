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
    bool campo_mina;            // Campo cont�m uma mina ?
    bool revelado;              // Campo revalado ?
    bool protegido;       // Campo com Bandeira
    int minas_adja;             // Quantidade de Minas Adjacentes
    int pos_x;                  // Posi��o X do Campo
    int pos_y;                  // Posi��o Y do Campo
};

// VARIAVEIS GLOBAIS, colocar um 'G_'
GLint windowsSize_x = 600;      // Largura da janela
GLint windowsSize_y = 600;      // Altura da janela

int G_minas = 0;                // Quantidade de minas no campo
int G_minasatuais = 0;

int G_bandeiras = 0;            // Quantidade de bandeiras no jogo. No Bandeiras = No Minas
int G_bandeiras_atuais = 0;     // Quantidade de bandeiras colocados no tabuleiro

int G_linhas = 0;               // Quantidade de Linhas do tabuleiro
int G_colunas = 0;              // Quantidade de Colunas do tabuleiro

int G_click_pos_x = 0.0;        // Posicao X do clique do mouse
int G_click_pos_y = 0.0;        // Posicao Y do clique do mouse
int G_operacao_desenho = 0;     // Determina qual operacao o desenho deve fazer
/*
0 = nao modifica tabuleiro [nao possivel via E/S]
1 = revelar um campo [Mouse LB]
2 = posicionar bandeira [Mouse RB]
*/
int G_timer = 0;                 // Mant�m o tempo decorrido de jogo
int G_dificuldade = 0;           // Mant�m a dificuldade selecionada do jogo

std::vector<Campo> campo_minado; // Similar a um ArrayList [Java] para representar o Campo Minado

// FUN��ES
template <typename T>
std::string to_string(T value){     //fun��o to_string criada na m�o
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

static void iniciaCampos();                                             // Funcionando
static void AcrescentaMina();                                           // Funcionando
static void Calc_Minas_Adjacentes();                                     //
static void MenuTemporario();                                           // Funcionando [Melhorar]
static void renderText(const char *text, int length, int x, int y);     // Funcionando
static void mostraTempo(int value);                                     // Funcionando
static void mostraMinas();                                              // Funcionando
static void acrescentaMarcacao(const char *opcao, int x, int y);        // n�o funcionando, estou procurando arrumar - By Alex
static void DesenhaQntMinasAdjacentes(int qnt, int x, int y);           //
static void Quadrado();                                                 // Funcionando [Melhorar]
static void Revelado();                                                 // Funcionando [Melhorar]
static void Bandeira();                                                 // Funcionando [Melhorar]
static void Tabuleiro();                                                // Funcionando

static void Calculo_Desenho(int linha, int coluna, int indice);         // Funcionando
static bool Calculo_Posicao(int linha, int coluna);                     // Funcionando
static void Revelar_Campo(int indice);                                  // Funcionando [Melhorar]
static void Alternar_Protecao(int indice);                              // Funcionando

static void Atualiza_tamanho(int largura, int altura);                  // Funcionando
static void Atualiza_desenho(void);                                     // Funcionando [Melhorar]

static void teclado(unsigned char tecla, int x, int y);                 // Funcionando
static void mouse(int botao, int estado, int x, int y);                 // Funcionando

static void Limpa();                                                    // Funcionando, por�m n�o sei ser� necess�rio ainda
static void Teste();

static void iniciaCampos()
{// Funcao que inicializa todos os campos com valores nulos
    int i = 0;
    int j = 0;
    int cont = 0;
    for(i = 0; i < G_linhas; i++){
        for(j = 0; j < G_colunas; j++){
            campo_minado.push_back(Campo());
            campo_minado[cont].campo_mina = false;
            campo_minado[cont].revelado = false;
            campo_minado[cont].protegido = false;
            campo_minado[cont].minas_adja = 0;
            campo_minado[cont].pos_x = i;
            campo_minado[cont].pos_y = j;
            printf("\nCampo: %2d  Valor: %2d  PosX: %2d  PosY: %2d", cont, campo_minado[cont].minas_adja, campo_minado[cont].pos_x, campo_minado[cont].pos_y);
            cont++;
        }
    }

}

static void  AcrescentaMina()
{// Funcao para gerar as posicoes das Minas no Tabuleiro
    srand(time(NULL));
    int i = 0;
    int minimo = 0;
    int maximo = 0;
    int cont = 0;
    maximo = (G_linhas * G_colunas) - 1;
    while(cont != G_minas){
        printf("\n\n[DEBUG]: Cont: %d  Minas: %d", cont, G_minas);
        i = rand() % (maximo - minimo + 1) + minimo;
        campo_minado.push_back(Campo());
        if(campo_minado[i].campo_mina == false){
            campo_minado[i].campo_mina = true;
            cont++;
        }
        printf("\n[DEBUG]:i: %d  Campo: %d  Mina? : %d", i, i, campo_minado[i].campo_mina);
    }
}

void Calc_Minas_Adjacentes()
{
    int i;
    int j;
    int tmpx, tmpy;
    for(i=0; i<G_linhas*G_colunas; i++){
        campo_minado.push_back(Campo());
        if(campo_minado[i].campo_mina){
            tmpx = campo_minado[i].pos_x;
            tmpy = campo_minado[i].pos_y;
            for(j=0; j<G_linhas*G_colunas; j++){
                campo_minado.push_back(Campo());
                if(campo_minado[j].pos_x == tmpx-1 && campo_minado[j].pos_y == tmpy-1){
                    campo_minado[j].minas_adja += 1;
                }
                if(campo_minado[j].pos_x == tmpx && campo_minado[j].pos_y == tmpy-1){
                    campo_minado[j].minas_adja += 1;
                }
                if(campo_minado[j].pos_x == tmpx+1 && campo_minado[j].pos_y == tmpy-1){
                    campo_minado[j].minas_adja += 1;
                }
                if(campo_minado[j].pos_x == tmpx-1 && campo_minado[j].pos_y == tmpy){
                    campo_minado[j].minas_adja += 1;
                }
                if(campo_minado[j].pos_x == tmpx+1 && campo_minado[j].pos_y == tmpy){
                    campo_minado[j].minas_adja += 1;
                }
                if(campo_minado[j].pos_x == tmpx-1 && campo_minado[j].pos_y == tmpy+1){
                    campo_minado[j].minas_adja += 1;
                }
                if(campo_minado[j].pos_x == tmpx && campo_minado[j].pos_y == tmpy+1){
                    campo_minado[j].minas_adja += 1;
                }
                if(campo_minado[j].pos_x == tmpx+1 && campo_minado[j].pos_y == tmpy+1){
                    campo_minado[j].minas_adja += 1;
                }
            }
        }
    }
    //Utilizado apenas para quest�es de DEBUG
    /*for(i=0; i<G_linhas*G_colunas; i++){
        printf("\nCampo: %d Minas Adja: %d", i, campo_minado[i].minas_adja);

    }*/
}
static void DesenhaQntMinasAdjacentes(int qnt, int x, int y){
    std::string text;
    text = to_string(qnt);
    glColor3f(0.0, 0.0, 0.0);
    //renderText("Minas", 5, 80, 50);
    renderText(text.data(), text.size(), x, y);
}

static void MenuTemporario()
{// Menu para selecionar o n�vel de dificuldade
    printf("Iniciando Menu Temporario \nEscolha a dificuldade(0-Noob, 1-Menos Noob, 2-SabeUmPouco): \n");
    scanf("%d", &G_dificuldade);
    switch(G_dificuldade){
        case 1: // Normal
            G_linhas = 10;
            G_colunas = 10;
            G_minas = 15;
            G_minasatuais = 15;
            break;
        case 2: // Hard
            G_linhas = 15;
            G_colunas = 15;
            G_minas = 30;
            G_minasatuais = 30;
            break;
        case 0: // Easy
        default:
            G_linhas = 5;
            G_colunas = 5;
            G_minas = 5;
            G_minasatuais = 5;
            break;
    }
}

static void renderText(const char *text, int length, int x, int y)
{// Fun��o para renderizar o texto
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

static void mostraTempo(int value)
{// Fun��o para mostrar o tempo decorrido
    std::string text;
    G_timer = glutGet(GLUT_ELAPSED_TIME);
    text = to_string(G_timer/1000); //Converte o G_timer para uma string
    /*glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(-4, -9.8, 0);
    glScalef(2, 1, 0);
    Limpa();
    glPopMatrix();*/
    glColor3f(0.0, 0.0, 0.0); //Seta a cor do texto como preto
    renderText("Tempo Decorrido", 15, 21, 6);
    renderText(text.data(), text.size(), 32, 2);
    glutTimerFunc(1000,mostraTempo, 1);
    //glutPostRedisplay();
}

static void mostraMinas()
{// Fun��o para mostra a quantidade de bombas iniciais em campo.
    std::string text;
    text = to_string(G_minasatuais);
    /*glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(2, -9.8, 0);
    Limpa();
    glPopMatrix();*/
    glColor3f(0.0, 0.0, 0.0);
    renderText("Minas", 5, 59, 6);
    renderText(text.data(), text.size(), 61, 2);
}

static void Menus()
{// Cria os campos dos Menus
    glBegin(GL_LINE_LOOP);
        glVertex2f(0, 0);
        glVertex2f(0, 1);
        glVertex2f(1, 1);
        glVertex2f(1, 0);
    glEnd();
}

static void Limpa()
{//Cria quadrado de fundo para o tempo e minas, por�m n�o sei se ser� necess�rio ainda
    glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(0, 1);
        glVertex2f(1, 1);
        glVertex2f(1, 0);
    glEnd();
}

static void Quadrado()
{// Campo nao revelado, o tamanho atual do campo � de X = 5% do windowsSize_x | Y = 5% do windowsSize_y
    glPushMatrix();
        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_LINE_LOOP);
            glVertex2f(0, 0);
            glVertex2f(0, 1);
            glVertex2f(1, 1);
            glVertex2f(1, 0);
        glEnd();
    glPopMatrix();
}

static void Revelado()
{// Campo revelado sem mina, mostra o numero de minas adjacentes, o tamanho atual do campo � de X = 5% do windowsSize_x | Y = 5% do windowsSize_y
    glPushMatrix();
        glBegin(GL_QUADS);
            glColor3f(0.0, 0.0, 1.0);
            glVertex2f(0, 0);
            glVertex2f(0, 1);
            glVertex2f(1, 1);
            glVertex2f(1, 0);
        glEnd();
    glPopMatrix();
}

static void Mina()
{// Campo revelado com mina, Game Over
    glPushMatrix();
        glBegin(GL_QUADS);
            glColor3f(1.0, 0.0, 0.0);
            glVertex2f(0, 0);
            glVertex2f(0, 1);
            glVertex2f(1, 1);
            glVertex2f(1, 0);
        glEnd();
    glPopMatrix();
}

static void Bandeira()
{// Campo protegido, representa um possivel local com uma mina, o tamanho atual do campo � de X = 5% do windowsSize_x | Y = 5% do windowsSize_y
    glPushMatrix();
        glBegin(GL_QUADS);
            glColor3f(0.0, 1.0, 0.0);
            glVertex2f(0, 0);
            glVertex2f(0, 1);
            glVertex2f(1, 1);
            glVertex2f(1, 0);
        glEnd();
    glPopMatrix();
}

static void Tabuleiro()
{// Funcao que gera o Tabuleiro
    float posStart_x = (float(G_colunas) / 2) * (-1);       // posicao x de onde comecar a desenhar
    float posStart_y = (float(G_linhas) / 2) * (-1);        // posicao y de onde comecar a desenhar

    int linha = 0;                                          // iterador das linhas
    int coluna = 0;                                         // iterador das colunas

    int cont = 0;                                           // contador para indicar o indice do campo

    glTranslatef(posStart_x, posStart_y, 0.0);              // translada para as posicoes X e Y iniciais do Tabuleiro
    glPushMatrix();
    for(linha = 0; linha < G_linhas; linha++)               // iterar nas linhas
    {
        glPushMatrix();
        for(coluna = 0; coluna < G_colunas; coluna++)       // iterar nas colunas
        {
            Calculo_Desenho(linha, coluna, cont);           // determinar o que desenhar, campo nao revelado, campo revelado ou uma mina
            cont++;                                         // incrementa o contador do indice;
            glTranslatef(1.0, 0.0, 0.0);                    // translada em x para posicionar a proxima coluna
        }
        glPopMatrix();                                      // retorna a posicao X inicial
        glTranslatef(0.0, 1.0, 0.0);                        // translada em y para posicionar a proxima linha
    }
    glPopMatrix();                                          // retorna a posicao Y inicial
}

static void Calculo_Desenho(int linha, int coluna, int indice)
{// Funcao para determinar o que desenhar, com base no clique do Mouse realizado
    int operador = Calculo_Posicao(linha, coluna) ? G_operacao_desenho : 0;
    // operador ternario,
    //se o clique corresponder ao campo da interacao do atual do Tabuleiro, fazer operacoes do clqiue do mouse (revelar um campo, operador = 1, ou colocar/retirar a protecao(bandeira), operador = 2))
    //senao apenas mostrar as proprieades dos campos (operador = 0)
    switch(operador)
    {
        case 0:                                                         // apenas mostrar o campo conforme sua propriedade, Campo Revelado, Campo Protegido, Campo Mina ou Campo nao Revelado
            if(campo_minado[indice].revelado)
            {
                Revelado();
            }
            else if(campo_minado[indice].protegido)
            {
                Bandeira();
            }
            else if(campo_minado[indice].campo_mina)                    // Condicao para fins de DEBUG
            {
                Mina();
            }
            else
            {
                Quadrado();
            }
            break;
        case 1:                                                         // Funcao para revelar um campo
            Revelar_Campo(indice);
            break;
        case 2:                                                         // Funcao para alternar a propriedade de Protegido
            Alternar_Protecao(indice);
            break;
    }
}

static bool Calculo_Posicao(int linha, int coluna)
{// Calcula se o local clicado, corresponde ao campo da itera��o na funcao Tabuleiro
    float coluna_minimo = coluna * (windowsSize_x * 0.05);                          // representa o "pixel" minimo no eixo x
    float coluna_maximo = coluna * (windowsSize_x * 0.05) + (windowsSize_y * 0.05); // representa o "pixel" maximo no eixo x, nada mais que o minimo + tamanho do campo (5% do tamanho da tela)
    float linha_minimo = linha * (windowsSize_y * 0.05);                            // representa o "pixel" minimo do eixo y
    float linha_maximo = linha * (windowsSize_y * 0.05) + (windowsSize_y * 0.05);   // representa o "pixel" maximo do eixo y, nada mais que o minimo + tamanho do campo (5% do tamanho da tela)
    // 0.05: relembrar que o quadrado tera 5% do tamanho da tela
    if(G_timer != 0)                                                                // if temporario para que nao revele logo no inicio algum campo
    {
        if((G_click_pos_x > coluna_minimo) && (G_click_pos_x < coluna_maximo))          // Verificao do clique no eixo X
        {
            if((G_click_pos_y > linha_minimo) && (G_click_pos_y < linha_maximo))        // Verificao do clique no eixo Y
            {
                return true;
            }
        }
    }
    return false;
}

static void Revelar_Campo(int indice)
{// Funcao para revelar um campo
    if(campo_minado[indice].revelado)                   // Campo ja revelado ?
    {
        Revelado();

        // Mostrar Numeros Adjacentes
        printf("\n     Campo ja revelado.");
    }
    else                                                // Campo nao revelado
    {
        if(campo_minado[indice].protegido)              // Campo com bandeira ?
        {
            Bandeira();
            printf("\n     Campo protegido.");
        }
        else                                            // Campo sem bandeira
        {
            if(campo_minado[indice].campo_mina)         // Campo com mina ?
            {
                Mina();
                printf("\n     Campo com mina.");
                printf("\n\n     --- Game Over ---\n\n");
                exit(0);                                // desativar para fins de DEBUG
                campo_minado[indice].revelado = true;
            }
            else                                        // Campo sem mina
            {
                Revelado();
                // Mostrar Numeros Adjacentes
                campo_minado[indice].revelado = true;
            }
        }
    }
}

static void Alternar_Protecao(int indice)
{// Funcao para alternar a protecao
    if(campo_minado[indice].protegido)                  // Campo protegido ?
    {
        Quadrado();
        G_minasatuais++;
        // Incrementar o numero de Bandeiras disponiveis
        campo_minado[indice].protegido = false;
    }
    else                                                // Campo nao protegido
    {
        if(campo_minado[indice].revelado)               // Campo revelado ?
        {
            Revelado();
            // Mostrar Numeros Adjacentes
            printf("\n     Campo ja revelado.");
        }
        else                                            // Campo nao revelado
        {
            if(G_minasatuais > 0)
            {
                Bandeira();
                G_minasatuais--;
            // Decrementar o numero de Bandeiras disponiveis
            campo_minado[indice].protegido = true;
            }
        }
    }
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

        // Desenhar Tabuleiro
        glPushMatrix();
            Tabuleiro();
        glPopMatrix();

        //Menu para tempo
        glPushMatrix();
            glTranslatef(-6.0, -10.0, 0);
            glScalef(5.0, 1.8, 0);
            glColor3f(1.0f, 0.0f, 0.0f);
            Menus();
        glPopMatrix();

        //Menu para bombas
        glPushMatrix();
            glTranslatef(0.0, -10.0, 0);
            glScalef(5.0, 1.8, 0);
            glColor3f(0.0f, 1.0f, 0.0f);
            Menus();
        glPopMatrix();

        mostraTempo(1000);
        mostraMinas();

    glPopMatrix();
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
    int i;
    if(botao == GLUT_LEFT_BUTTON){
        if(estado == GLUT_DOWN){
            printf("\n[DEBUG]: Apertou botao esquerdo mouse");

            G_click_pos_x = x - (windowsSize_x / 2);                    // ajusta a posi��o do mouse para combinar com o viewport, posicao x real - janela X / 2
            G_click_pos_x = G_click_pos_x + ((float(windowsSize_x) * 0.05) * (float(G_colunas) / 2));
            G_click_pos_y = (windowsSize_y / 2) - y ;                   // ajusta a posi��o do mouse para combinar com o viewport, anela Y / 2 - posicao y real
            G_click_pos_y = G_click_pos_y + ((float(windowsSize_y) * 0.05) * (float(G_linhas) / 2));
            printf("\n X: %d,  Y:  %d\n", G_click_pos_x, G_click_pos_y);

            G_operacao_desenho = 1;
            glutPostRedisplay();
        }
    }else if(botao == GLUT_RIGHT_BUTTON){
        if(estado == GLUT_DOWN){
            printf("\n[DEBUG]: Apertou Botao direito mouse");

            G_click_pos_x = x - (windowsSize_x / 2);                    // ajusta a posi��o do mouse para combinar com o viewport, posicao x real - janela X / 2
            G_click_pos_x = G_click_pos_x + ((float(windowsSize_x) * 0.05) * (float(G_colunas) / 2));
            G_click_pos_y = (windowsSize_y / 2) - y ;                   // ajusta a posi��o do mouse para combinar com o viewport, anela Y / 2 - posicao y real
            G_click_pos_y = G_click_pos_y + ((float(windowsSize_y) * 0.05) * (float(G_linhas) / 2));
            printf("\n X: %d,  Y:  %d\n", G_click_pos_x, G_click_pos_y);

            G_operacao_desenho = 2;
            glutPostRedisplay();
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
    Calc_Minas_Adjacentes();
    glutDisplayFunc(Atualiza_desenho);
    glutReshapeFunc(Atualiza_tamanho);

    glutKeyboardFunc(teclado);
    glutMouseFunc(mouse);

    G_timer = glutGet(GLUT_ELAPSED_TIME);
    //glutTimerFunc(1000, mostraTempo, 1);

    glClearColor(1,1,1,1);
    glutMainLoop();

    return 0;
}
