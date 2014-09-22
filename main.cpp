#include <gl/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <time.h>
#include <conio.h>

// CONSTANTES
#define INTERVALO_TEMPO 1000

// ESTRUTURAS, TIPOS CUSTOMIZADOS
struct Campo{
    bool campo_mina;            // Campo contém uma mina ?
    bool revelado;              // Campo revalado ?
    bool protegido;             // Campo com Bandeira
    int minas_adja;             // Quantidade de Minas Adjacentes
    int pos_x;                  // Posição X do Campo
    int pos_y;                  // Posição Y do Campo
};

// VARIAVEIS GLOBAIS, colocar um 'G_'
GLint windowsSize_x = 600;      // Largura da janela
GLint windowsSize_y = 600;      // Altura da janela

bool G_Game_Over = 0;           // Condição para terminar o jogo

int G_minas = 0;                // Quantidade de minas no campo
int G_nao_revelados = 0;        // Quantidade de campos não revelados e que não são Minas
int G_bandeiras = 0;            // Quantidade de bandeiras no jogo. No Bandeiras = No Minas

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
int G_timer = 0;                 // Mantém o tempo decorrido de jogo
int G_dificuldade = 0;           // Mantém a dificuldade selecionada do jogo

std::vector<Campo> campo_minado; // Similar a um ArrayList [Java] para representar o Campo Minado

// FUNÇÕES
template <typename T>
std::string to_string(T value){     //função to_string criada na mão
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

static void inicializaCampos();                                             // Funcionando
static void AcrescentaMina();                                           // Funcionando

static void MenuTemporario();                                           // Funcionando [Melhorar]
static void renderText(const char *text, int length, int x, int y);     // Funcionando
static void mostraMinas();                                              // Funcionando
static void mostraTempo(int value);                                     // Funcionando

static void Menus();                                                    // Funcionando
static void Quadrado();                                                 // Funcionando [Melhorar]
static void Revelado();                                                 // Funcionando [Melhorar]
static void Minas_Adjacentes(int indice);                               // Funcionando
static void Desenha_Minas();                                            //
static void Mina();                                                     // Funcionando
static void Bandeira();                                                 // Funcionando [Melhorar]
static void Tabuleiro();                                                // Funcionando

static void Calc_Minas_Adjacentes();                                    //
static void Calculo_Desenho(int linha, int coluna, int indice);         // Funcionando
static bool Calculo_Posicao(int linha, int coluna);                     // Funcionando
static void Revelar_Campo(int indice);                                  // Funcionando [Melhorar]
static void Alternar_Protecao(int indice);                              // Funcionando

static void Timer(int value);                                           // Funcionando
static void Atualiza_tamanho(int largura, int altura);                  // Funcionando
static void Atualiza_desenho(void);                                     // Funcionando [Melhorar]

static void teclado(unsigned char tecla, int x, int y);                 // Funcionando
static void mouse(int botao, int estado, int x, int y);                 // Funcionando

static void renderGameOver();                                           // Funcionando
static void renderVenceu();                                             // Funcionando
static void AbreJogoGameOver();                                         // Funcionando
static void ExpandeArea(int indice);                                    // Funcionando

static void inicializaCampos()
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
            //printf("\nCampo: %2d  Valor: %2d  PosX: %2d  PosY: %2d", cont, campo_minado[cont].minas_adja, campo_minado[cont].pos_x, campo_minado[cont].pos_y);
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
        if(campo_minado[i].campo_mina == false)
        {
            if(campo_minado[i].revelado != true){
                campo_minado[i].campo_mina = true;
                cont++;
            }
        }
        printf("\n[DEBUG]:i: %d  Campo: %d  Mina? : %d", i, i, campo_minado[i].campo_mina);
    }
    G_bandeiras = G_minas;
}

void Calc_Minas_Adjacentes()
{
    int i;
    int j;
    int tmpx, tmpy;
    for(i=0; i<G_linhas*G_colunas; i++){
        if(campo_minado[i].campo_mina){
            tmpx = campo_minado[i].pos_x;
            tmpy = campo_minado[i].pos_y;
            for(j=0; j<G_linhas*G_colunas; j++){
                if(campo_minado[j].campo_mina){
                    campo_minado[j].minas_adja = -1;
                }else{
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
    }
    //Utilizado apenas para questões de DEBUG
    /*for(i=0; i<G_linhas*G_colunas; i++){
        printf("\nCampo: %d Minas Adja: %d", i, campo_minado[i].minas_adja);

    }*/
}

static void MenuTemporario()
{// Menu para selecionar o nível de dificuldade
    printf("Iniciando Menu Temporario \nEscolha a dificuldade(0-Noob, 1-Menos Noob, 2-SabeUmPouco): \n");
    scanf("%d", &G_dificuldade);
    switch(G_dificuldade){
        case 1: // Normal
            G_linhas = 10;
            G_colunas = 10;
            G_minas = 15;
            break;
        case 2: // Hard
            G_linhas = 15;
            G_colunas = 15;
            G_minas = 30;
            break;
        case 0: // Easy
        default:
            G_linhas = 5;
            G_colunas = 5;
            G_minas = 5;
            break;
    }
}

static void renderText(const char *text, int length, int x, int y)
{// Função para renderizar o texto
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

static void mostraMinas()
{// Função para mostra a quantidade de bombas iniciais em campo.
    std::string text;
    text = to_string(G_bandeiras);
    glColor3f(0.0, 0.0, 0.0);
    renderText("Minas", 5, 59, 6);
    renderText(text.data(), text.size(), 61, 2);
}

static void mostraTempo()
{
    std::string text;
    G_timer = glutGet(GLUT_ELAPSED_TIME);
    text = to_string(G_timer/1000);                     //Converte o G_timer para uma string
    glColor3f(0.0, 0.0, 0.0);                           //Seta a cor do texto como preto
    renderText("Tempo Decorrido", 15, 21, 6);
    renderText(text.data(), text.size(), 32, 2);
}

static void AbreJogoGameOver(){ // Quando acabo o jogo revela todos os campos
    int i;
    int controlador;
    std::string text;
    switch(G_linhas){
        case 5:
            controlador = 39;
            break;
        case 10:
            controlador = 27;
            break;
        case 15:
            controlador = 14;
    }

    for(i=0; i<G_linhas*G_colunas; i++){
        text = to_string(campo_minado[i].minas_adja);
        glPushMatrix();
        if(campo_minado[i].minas_adja != -1){
            glColor3f(0.0, 0.0, 0.0);
        }else{
            glColor3f(1.0, 0.0, 0.0);
        }
        if(!campo_minado[i].revelado){
            renderText(text.data(), text.size(), campo_minado[i].pos_y*5+controlador, campo_minado[i].pos_x*5+controlador);
        }
        glPopMatrix();
    }
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

static void Quadrado()
{// Campo nao revelado, o tamanho atual do campo é de X = 5% do windowsSize_x | Y = 5% do windowsSize_y
    glPushMatrix();
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_LINE_LOOP);
            glVertex2f(0, 0);
            glVertex2f(0, 1);
            glVertex2f(1, 1);
            glVertex2f(1, 0);
        glEnd();
    glPopMatrix();
}

static void Revelado()
{// Campo revelado sem mina, mostra o numero de minas adjacentes, o tamanho atual do campo é de X = 5% do windowsSize_x | Y = 5% do windowsSize_y
    glPushMatrix();
        glBegin(GL_QUADS);
            glColor3f(0.4, 0.4, 0.4);
            glVertex2f(0, 0);
            glVertex2f(0, 1);
            glVertex2f(1, 1);
            glVertex2f(1, 0);
        glEnd();
    glPopMatrix();
}

static void Minas_Adjacentes(int indice)
{// Mostrar o numero de Minas adjacentes do campo
    std::string text;                                               // texto a ser desenhado, quantidade de minas adjacentes
    text = to_string(campo_minado[indice].minas_adja);              // guarda o numero de minas adjacentes do campo, em formato de string
    glPushMatrix();
        glColor3f(0.0, 0.0, 0.0);                                   // Cor: preto
        //glScalef(0.005, 0.005, 0.0);                              // Solucao 02
        //glTranslatef(50, 50, 0.0);                                // Solucao 02
        glRasterPos2f(0.3, 0.3);                                    // Solucao 01 ... desloca o local a ser renderizado o texto ...

        void * font = GLUT_BITMAP_9_BY_15;                          // Solucao 01 ... Seleciona a fonte do texto -> http://openglut.sourceforge.net/group__bitmapfont.html
        //void* font = GLUT_STROKE_ROMAN;                           // Solucao 02 ... Seleciona a fonte do texto -> http://openglut.sourceforge.net/group__fonts.html
        for(std::string::iterator iterador_string = text.begin(); iterador_string != text.end(); ++iterador_string) // intera entre os caracteres do texto
        {
            char caractere = *iterador_string;                      // converte o iterador para caractere
            glutBitmapCharacter(font, caractere);                   // Solucao 01 ... desenha o caractere
            //glutStrokeCharacter(font, caractere);                 // Solucao 02 ... desenha o caractere
        }
    glPopMatrix();
}

static void ExpandeArea(int indice){
    int j;
    int tmpx, tmpy;
    if(campo_minado[indice].minas_adja == 0){
        tmpx = campo_minado[indice].pos_x;
        tmpy = campo_minado[indice].pos_y;
        for(j=0; j<G_linhas*G_colunas; j++){
            if(campo_minado[j].protegido == false && campo_minado[j].revelado == false){
                if(campo_minado[j].pos_x == tmpx-1 && campo_minado[j].pos_y == tmpy-1){
                    //Revelado();
                    Minas_Adjacentes(j);
                    campo_minado[j].revelado = true;
                    G_nao_revelados--;
                    if(campo_minado[j].minas_adja == 0) ExpandeArea(j);
                    //Revelar e Mostrar Adjacentes
                }
                if(campo_minado[j].pos_x == tmpx && campo_minado[j].pos_y == tmpy-1){
                    //Revelado();
                    Minas_Adjacentes(j);
                    campo_minado[j].revelado = true;
                    G_nao_revelados--;
                    if(campo_minado[j].minas_adja == 0) ExpandeArea(j);
                    //Revelar e Mostrar Adjacentes
                }
                if(campo_minado[j].pos_x == tmpx+1 && campo_minado[j].pos_y == tmpy-1){
                    //Revelado();
                    Minas_Adjacentes(j);
                    campo_minado[j].revelado = true;
                    G_nao_revelados--;
                    if(campo_minado[j].minas_adja == 0) ExpandeArea(j);
                    //Revelar e Mostrar Adjacentes
                }
                if(campo_minado[j].pos_x == tmpx-1 && campo_minado[j].pos_y == tmpy){
                    //Revelado();
                    Minas_Adjacentes(j);
                    campo_minado[j].revelado = true;
                    G_nao_revelados--;
                    if(campo_minado[j].minas_adja == 0) ExpandeArea(j);
                    //Revelar e Mostrar Adjacentes
                }
                if(campo_minado[j].pos_x == tmpx+1 && campo_minado[j].pos_y == tmpy){
                    //Revelado();
                    Minas_Adjacentes(j);
                    campo_minado[j].revelado = true;
                    G_nao_revelados--;
                    if(campo_minado[j].minas_adja == 0) ExpandeArea(j);
                    //Revelar e Mostrar Adjacentes
                }
                if(campo_minado[j].pos_x == tmpx-1 && campo_minado[j].pos_y == tmpy+1){
                    //Revelado();
                    Minas_Adjacentes(j);
                    campo_minado[j].revelado = true;
                    G_nao_revelados--;
                    if(campo_minado[j].minas_adja == 0) ExpandeArea(j);
                    //Revelar e Mostrar Adjacentes
                }
                if(campo_minado[j].pos_x == tmpx && campo_minado[j].pos_y == tmpy+1){
                    //Revelado();
                    Minas_Adjacentes(j);
                    campo_minado[j].revelado = true;
                    G_nao_revelados--;
                    if(campo_minado[j].minas_adja == 0) ExpandeArea(j);
                    //Revelar e Mostrar Adjacentes
                }
                if(campo_minado[j].pos_x == tmpx+1 && campo_minado[j].pos_y == tmpy+1){
                    //Revelado();
                    Minas_Adjacentes(j);
                    campo_minado[j].revelado = true;
                    G_nao_revelados--;
                    if(campo_minado[j].minas_adja == 0) ExpandeArea(j);
                    //Revelar e Mostrar Adjacentes
                }
            }
        }
    }
    glutPostRedisplay();
}

static void Desenha_Minas(){
    glPushMatrix();
    glScalef(1.0, 1.0, 1.0);
        glColor3f(1.0, 0.0, 0.0);
        Mina();
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
{// Campo protegido, representa um possivel local com uma mina, o tamanho atual do campo é de X = 5% do windowsSize_x | Y = 5% do windowsSize_y
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
                Minas_Adjacentes(indice);
            }
            else if(campo_minado[indice].protegido)
            {
                Bandeira();
            }
            else if(campo_minado[indice].campo_mina)                    // Condicao para fins de DEBUG
            {
                Quadrado();
            }
            else
            {
                Quadrado();
            }
            break;
        case 1:                                                         // Funcao para revelar um campo
            Revelar_Campo(indice);
            G_operacao_desenho = 0;                                     // Operacao concluida (campo revelado), retorna ao modo de operacao apenas de atualizacao do tabuleiro
            break;
        case 2:                                                         // Funcao para alternar a propriedade de Protegido
            Alternar_Protecao(indice);
            G_operacao_desenho = 0;                                     // Operacao concluida (alternado protecao), retorna ao modo de operacao apenas de atualizacao do tabuleiro
            break;
    }
}

static bool Calculo_Posicao(int linha, int coluna)
{// Calcula se o local clicado, corresponde ao campo da iteração na funcao Tabuleiro
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
        Minas_Adjacentes(indice);
        //printf("\n     Campo ja revelado.");
    }
    else                                                // Campo nao revelado
    {
        if(campo_minado[indice].protegido)              // Campo com bandeira ?
        {
            Bandeira();
            //printf("\n     Campo protegido.");
        }
        else                                            // Campo sem bandeira
        {
            if(campo_minado[indice].campo_mina)         // Campo com mina ?
            {
                glutMouseFunc(NULL);
                G_Game_Over = true;
                Desenha_Minas();
                AbreJogoGameOver();
                renderGameOver();
                //printf("\n     Campo com mina.");
                //printf("\n\n     --- Game Over ---\n\n");
                //campo_minado[indice].revelado = true;
            }
            else                                        // Campo sem mina
            {
                G_nao_revelados--;
                printf("\n Nao Revelados %d", G_nao_revelados);
                Revelado();
                Minas_Adjacentes(indice);
                campo_minado[indice].revelado = true;
                if(G_nao_revelados == 0){
                    G_Game_Over = true;
                    glutMouseFunc(NULL);
                    renderVenceu();
                }
                if(campo_minado[indice].minas_adja == 0){
                    ExpandeArea(indice);
                }
            }
        }
    }
}

static void Alternar_Protecao(int indice)
{// Funcao para alternar a protecao
    if(campo_minado[indice].protegido)                  // Campo protegido ?
    {
        Quadrado();
        G_bandeiras++;                                  // Incrementa o numero de Bandeiras disponiveis
        campo_minado[indice].protegido = false;
    }
    else                                                // Campo nao protegido
    {
        if(campo_minado[indice].revelado)               // Campo revelado ?
        {
            Revelado();
            Minas_Adjacentes(indice);
            printf("\n     Campo ja revelado.");
        }
        else                                            // Campo nao revelado
        {
            if(G_bandeiras > 0)                         // Bandeiras disponiveis ?
            {
                Bandeira();
                G_bandeiras--;                          // Decrementa o numero de Bandeiras disponiveis
                campo_minado[indice].protegido = true;
            }
            else
            {
                Quadrado();                             // Desenha um campo livre
            }
        }
    }
}

static void Timer(int value)
{// Função controlar o tempo de atualizacao
    if(!G_Game_Over)                                // Enquanto o jogo nao acabar, realizar a atualizacao
    {
        glutPostRedisplay();
        glutTimerFunc(INTERVALO_TEMPO, Timer, 0);
    }
}

static void Atualiza_tamanho(int largura, int altura)
{
    glViewport(0, 0, largura, altura);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D (-10.0f, 10.0f, -10.0f, 10.0f);
    //gluOrtho2D (0.0f, 10.0f, 12.0f, 0.0f);

    printf("\n[DEBUG] : Evento Atualiza tamanho\n");
}

static void Atualiza_desenho(void)
{
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

        mostraTempo();                                  // Mostra o tempo percorrido
        mostraMinas();                                  // Mostra o numero de minas/bandeiras disponiveis
        AbreJogoGameOver();
    glPopMatrix();

    //glFlush();                                        //
    glutSwapBuffers();                                  // funcao semelhante ao glFlush()

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
        default:
            printf("\nNenhum evento atribuido a tecla\n");
            break;
    }
}

static void mouse(int botao, int estado, int x, int y)
{
    int i;
    if(botao == GLUT_LEFT_BUTTON){
        if(estado == GLUT_DOWN){
            printf("\n[DEBUG]: Apertou botao esquerdo mouse");

            G_click_pos_x = x - (windowsSize_x / 2);                    // ajusta a posição do mouse para combinar com o viewport, posicao x real - janela X / 2
            G_click_pos_x = G_click_pos_x + ((float(windowsSize_x) * 0.05) * (float(G_colunas) / 2));
            G_click_pos_y = (windowsSize_y / 2) - y ;                   // ajusta a posição do mouse para combinar com o viewport, anela Y / 2 - posicao y real
            G_click_pos_y = G_click_pos_y + ((float(windowsSize_y) * 0.05) * (float(G_linhas) / 2));
            printf("\n X: %d,  Y:  %d\n", G_click_pos_x, G_click_pos_y);

            G_operacao_desenho = 1;
            glutPostRedisplay();
        }
    }else if(botao == GLUT_RIGHT_BUTTON){
        if(estado == GLUT_DOWN){
            printf("\n[DEBUG]: Apertou Botao direito mouse");

            G_click_pos_x = x - (windowsSize_x / 2);                    // ajusta a posição do mouse para combinar com o viewport, posicao x real - janela X / 2
            G_click_pos_x = G_click_pos_x + ((float(windowsSize_x) * 0.05) * (float(G_colunas) / 2));
            G_click_pos_y = (windowsSize_y / 2) - y ;                   // ajusta a posição do mouse para combinar com o viewport, anela Y / 2 - posicao y real
            G_click_pos_y = G_click_pos_y + ((float(windowsSize_y) * 0.05) * (float(G_linhas) / 2));
            printf("\n X: %d,  Y:  %d\n", G_click_pos_x, G_click_pos_y);

            G_operacao_desenho = 2;
            glutPostRedisplay();
        }
    }else{
        printf("\n[DEBUG]: Nao entendo o que vc quer clicar, pare de apertar esse botao por favor");
    }
}

static void renderGameOver()    //Realiza o desenho da escrita GAME OVER utilizando #'s
{
    int i; //Variável utilizada para os fors
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0); //Cor do texto a ser impresso
    renderText("===================================================================", 67, 0, 99); //Desenha uma faixa no topo da janela
    //G
    renderText("### ", 4, 19, 97);  //Desenha o Topo do G
    renderText("   #", 4, 19, 96);  //Desenha a ponta do topo do G
    for(i=96; i>=92; i--){          //Desenha a coluna do G
        renderText("#", 1, 18, i);
    }
    renderText("### ", 4, 19, 91);  //Desenha a base do G
    renderText("   #", 4, 19, 92);  //Desenha a coluna da curva que leva ao interior do G
    renderText("   #", 4, 19, 93);  //Desenha a coluna da curva que leva ao interior do G
    renderText("  #", 3, 19, 93);   //Desenha a ponta do interior do G
    //A
    renderText(" ###", 4, 26, 97);  //Desenha o topo do A
    for(i=96; i>=91; i--){          //Desenha a coluna da esquerda do A
        renderText("#", 1, 26, i);
    }
    for(i=96; i>=91; i--){           //Desenha a coluna da direita do A
        renderText("    #", 5, 26, i);
    }
    renderText(" ### ", 5, 26, 94);  //Desenha o traço do meio de A
    //M
    for(i=97; i>=91; i--){           //Desenha a coluna da esquerda de M
        renderText("#", 1, 34, i);
    }
    renderText(" #", 2, 34, 96);    //Desenha o primeiro # do meio de M
    renderText("  #", 3, 34, 95);   //Desenha o segundo # do meio de M
    renderText("   #", 5, 34, 96);  //Desenha o terceiro # do meio de M
    for(i=97; i>=91; i--){           //Desenha a coluna da direita de M
        renderText("    #", 5, 34, i);
    }
    //E
    for(i=97; i>=91; i--){          //Desenha a coluna do E
        renderText("#", 1, 42, i);
    }
    for(i=97; i>=91; i-=3){         //Desenha as 3 verticais de E
    renderText(" ###", 4, 42, i);
    }
    //O
    for(i=96; i>=92; i--){          //Desenha a coluna da esquerda de O
        renderText("#", 1, 54, i);
    }
    renderText(" ##", 3, 54, 97);  //Desenha o Topo de O
    for(i=96; i>=92; i--){          //Desenha a coluna da direita de O
        renderText("   #", 4, 54, i);
    }
    renderText(" ##", 3, 54, 91); //Desenha a Base de O
    //V
    for(i=97; i>=93; i--){        //Desenha a coluna da esquerda de V
        renderText("#", 1, 60, i);
    }
    renderText(" #", 2, 60, 92);  //Desenha a curva inferior de V
    renderText("  #", 3, 60, 91); //Desenha a curva inferior de V
    renderText("   #", 4, 60, 92); //Desenha a curva inferior de V
    for(i=97; i>=93; i--){        //Desenha a coluna da direita de V
        renderText("    #", 5, 60, i);
    }
    //E
    for(i=97; i>=91; i--){        //Desenha a coluna do E
        renderText("#", 1, 68, i);
    }
    for(i=97; i>=91; i-=3){       //Desenha as 3 verticais de E
    renderText(" ###", 4, 68, i);
    }
    //R
    for(i=97; i>=91; i--){         //Desenha a coluna do R
        renderText("#", 1, 75, i);
    }
    renderText(" ###", 4, 75, 97); //Desenha o topo de R
    renderText(" ###", 4, 75, 94); //Desenha o meio vertical de R
    for(i=96; i>=91; i--){         //desenha a coluna da direita de R
        if(i == 94)continue;
        renderText("    #", 5, 75, i);
    }
    renderText("Pressione q para sair", 21, 30, 89); //Escreve a frase abaixo do desenho
    renderText("===================================================================", 67, 0, 87); //Desenha uma faixa logo acima do tabuleiro
    glPopMatrix();

}

static void renderVenceu()
{
    int i;
    glPushMatrix();
    glColor3f(0.0, 0.0, 1.0);
    //V
    for(i=95; i>=91; i--){
        renderText("#", 1, 12, i);
    }
    renderText(" #", 2, 12, 90);
    renderText("  #", 3, 12, 89);
    renderText("   #", 4, 12, 90);
    for(i=95; i>=91; i--){
        renderText("    #", 5, 12, i);
    }
    //O
    for(i=94; i>=90; i--){
        renderText("#", 1, 20, i);
    }
    renderText(" ##", 3, 20, 95);
    for(i=94; i>=90; i--){
        renderText("   #", 4, 20, i);
    }
    renderText(" ##", 3, 20, 89);
    //C
    renderText(" ###", 4, 27, 95);
    for(i=94; i>=90; i--){
        renderText("#", 1, 27, i);
    }
    renderText(" ###", 4, 27, 89);
    //E
    for(i=95; i>=89; i--){
        renderText("#", 1, 34, i);
    }
    for(i=95; i>=89; i-=3){
    renderText(" ###", 4, 34, i);
    }
    //^
    renderText(" #", 2, 34, 97);
    renderText("  #", 3, 34, 98);
    renderText("   #", 4, 34, 97);
    //V
    for(i=95; i>=91; i--){
        renderText("#", 1, 43, i);
    }
    renderText(" #", 2, 43, 90);
    renderText("  #", 3, 43, 89);
    renderText("   #", 4, 43, 90);
    for(i=95; i>=91; i--){
        renderText("    #", 5, 43, i);
    }
    //E
    for(i=95; i>=89; i--){
        renderText("#", 1, 51, i);
    }
    for(i=95; i>=89; i-=3){
    renderText(" ###", 4, 51, i);
    }
    //N
    for(i=95; i>=89; i--){
        renderText("#", 1, 58, i);
    }
    renderText(" #", 2, 58, 93);
    renderText("  #", 3, 58, 92);
    renderText("   #", 4, 58, 91);
    for(i=95; i>=89; i--){
        renderText("    #", 5, 58, i);
    }
    //C
    renderText(" ###", 4, 66, 95);
    for(i=94; i>=90; i--){
        renderText("#", 1, 66, i);
    }
    renderText(" ###", 4, 66, 89);
    //E
    for(i=95; i>=89; i--){
        renderText("#", 1, 73, i);
    }
    for(i=95; i>=89; i-=3){
    renderText(" ###", 4, 73, i);
    }
    //U
    for(i=95; i>=90; i--){
        renderText("#", 1, 80, i);
    }
    renderText(" #", 2, 80, 89);
    renderText("  #", 3, 80, 89);
    renderText("   #", 4, 80, 89);
    for(i=95; i>=90; i--){
        renderText("    #", 5, 80, i);
    }
    glPopMatrix();
}

int main(){
    MenuTemporario();
    glutInitWindowSize(windowsSize_x, windowsSize_y);
    glutInitWindowPosition(300, 0);
    glutInitDisplayMode (GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    //glutInitDisplayMode (GLUT_RGB | GLUT_SINGLE);

    glutCreateWindow("Campo Minado");

    inicializaCampos();
    AcrescentaMina();
    Calc_Minas_Adjacentes();
    glutDisplayFunc(Atualiza_desenho);
    glutReshapeFunc(Atualiza_tamanho);

    glutKeyboardFunc(teclado);
    glutMouseFunc(mouse);
    G_nao_revelados = G_linhas*G_colunas - G_minas;
    glutTimerFunc(0, Timer, 0);

    glClearColor(1,1,1,1);
    glutMainLoop();

    return 0;
}
