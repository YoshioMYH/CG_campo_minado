/*
Programa desenvolvido com prop�sito de realizar o trabalho de Computa��o Gr�fica
Universidade Federal da Grande Dourados - UFGD
2�Semestre 2014

Autores: Alex Yudi Oshiro e Marcelo Yoshio Hasegawa

Data: 02/10/2014

Vers�o atual: 1.11.5

*/

#include <gl/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <time.h>
#include "primitivas.h"
#include "renders.h"

// CONSTANTES
#define INTERVALO_TEMPO 1000

// ESTRUTURAS, TIPOS CUSTOMIZADOS
struct Campo{
    bool campo_mina;            // Campo cont�m uma mina ?
    bool revelado;              // Campo revalado ?
    bool protegido;             // Campo com Bandeira
    int minas_adja;             // Quantidade de Minas Adjacentes
    int pos_x;                  // Posi��o X do Campo
    int pos_y;                  // Posi��o Y do Campo
};

// VARIAVEIS GLOBAIS, colocar um 'G_'
GLint windowsSize_x = 700;      // Largura da janela
GLint windowsSize_y = 700;      // Altura da janela

int G_minas = 5;                // Quantidade de minas no campo
int G_nao_revelados = 0;        // Quantidade de campos n�o revelados e que n�o s�o Minas
int G_bandeiras = 0;            // Quantidade de bandeiras no jogo. No Bandeiras = No Minas

int G_linhas = 5;               // Quantidade de Linhas do tabuleiro
int G_colunas = 5;              // Quantidade de Colunas do tabuleiro

int G_click_pos_x = 0.0;        // Posicao X do clique do mouse
int G_click_pos_y = 0.0;        // Posicao Y do clique do mouse
int G_operacao_desenho = 0;     // Determina qual operacao o desenho deve fazer
/*
0 = nao modifica tabuleiro [nao possivel via E/S]
1 = revelar um campo [Mouse LB]
2 = posicionar bandeira [Mouse RB]
*/

int G_estado_jogo = 0;          // Determina em qual estado o jogo se encontra
/*
0 = inicio do jogo, escolha do menu
1 = jogo iniciado, esperando primeiro clique
2 = jogo com cliques "normais"
3 = fim do jogo
*/

int G_timer = 0;                // Mant�m o tempo decorrido de um jogo, G_timer = G_timer_count - G_timer_reset
int G_timer_count = 0;          // Mantem o tempo decorrido do programa, desde o primeiro glutGet(GLUT_ELAPSED_TIME)
int G_timer_reset = 0;          // Mantem o tempo em que um jogo foi iniciado

int G_dificuldade = 0;          // Mant�m a dificuldade selecionada do jogo

bool G_regras = false;          // Estado para entrar/mostrar o Quadro de Regras do jogo

std::vector<Campo> campo_minado;// Vetor para armazenar as informacoes dos campos do Tabuleiro


template <typename T>
std::string to_string(T value){ // fun��o to_string criada na m�o
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

// PROTOTIPOS DAS FUNCOES
static void inicializaCampos();                                         // OK
static void AcrescentaMina(int indice);                                 // OK
static void MenuTemporario();                                           // OK []

static void mostraMinas();                                              // OK
static void mostraTempo(int value);                                     // OK

static void Minas_Adjacentes(int indice);                               // OK
static void Tabuleiro();                                                // OK

static void Calculo_Desenho(int linha, int coluna, int indice);         // OK
static bool Calculo_Posicao(int linha, int coluna);                     // OK

static void Revelar_Campo(int indice);                                  // OK []
static void Calc_Minas_Adjacentes();                                    // OK
static void ExpandeArea(int indice);                                    // OK
static void Alternar_Protecao(int indice);                              // OK

static void AbreJogoGameOver();                                         // OK

static void Atualiza_tamanho(int largura, int altura);                  // OK
static void Atualiza_desenho(void);                                     // OK []
static void Menu_grafico();
static void Quadro_Regra();

static void teclado(unsigned char tecla, int x, int y);                 // OK
static void mouse(int botao, int estado, int x, int y);                 // OK
static void mouse_menu(int botao, int estado, int x, int y);            // OK
static void Timer(int value);                                           // OK
static void Reset_Timer();                                              // OK

// FUN��ES
static void inicializaCampos()
{// Funcao que inicializa todos os campos com valores nulos
    int i = 0;  // iterador das linhas
    int j = 0;  // iterador das colunas
    int cont = 0;   // contador do indice no vetor
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

static void  AcrescentaMina(int indice)
{// Funcao para gerar as posicoes das Minas no Tabuleiro
    srand(time(NULL));
    int i = 0;      // variavel que ira armazenar um numero randomico
    int minimo = 0; // posicao minima da mina no tabuleiro (indice 0)
    int maximo = 0; // posicao maxima da mina no tabuleiro (indice n - 1)
    int cont = 0;   // contador do numero de minas posicionadas
    maximo = (G_linhas * G_colunas) - 1;
    while(cont != G_minas){
        i = rand() % (maximo - minimo + 1) + minimo;    // Cria um i randomico de 0 ao n�mero m�ximo
        //printf("\n\n[DEBUG]: Cont: %d  Minas: %d", cont, G_minas);
        if(i != indice)                                 // Verifica se o i � diferente ao indice clicado pela primeira vez pelo jogador
        {
            if(campo_minado[i].campo_mina == false)     // Verifica se o campo n�o contem mina
            {
               campo_minado[i].campo_mina = true;
               cont++;
            }
        }
        //printf("\n[DEBUG]:i: %d  Campo: %d  Mina? : %d", i, i, campo_minado[i].campo_mina);
    }
    // G_bandeiras = G_minas;  // quantidade de bandeiras = numero de minas posicionadas
}

static void MenuTemporario()
{// Menu para selecionar o n�vel de dificuldade, ajustando as variveis do numero de linhas, colunas, minas e bandeiras do jogo
    //printf("Iniciando Menu Temporario \nEscolha a dificuldade(0-Facil, 1-Normal, 2-Dificil): \n");
    //scanf("%d", &G_dificuldade);
    switch(G_dificuldade){
        case 1: // Dificuldade Normal
            G_linhas = 10;
            G_colunas = 10;
            G_minas = 15;
            G_bandeiras = 15;
            break;
        case 2: // Dificuldade Dificil
            G_linhas = 15;
            G_colunas = 15;
            G_minas = 30;
            G_bandeiras = 30;
            break;
        case 0: // Dificuldade Facil
        default:
            G_linhas = 5;
            G_colunas = 5;
            G_minas = 5;
            G_bandeiras = 5;
            break;
    }
}


static void mostraMinas()
{// Fun��o para mostra a quantidade de bombas iniciais em campo.
    std::string text;
    text = to_string(G_bandeiras);
    glColor3f(0.0, 0.0, 0.0);
    renderText("Minas", 5, 59, 6);
    renderText(text.data(), text.size(), 61, 2);
}

static void mostraTempo()
{// Fun��o para mostrar o tempo decorrido durante o jogo
    std::string text;
    if(G_estado_jogo == 2)                                  // Jogo esta no estado de "jogo com cliques normais" [jogando] ?
    {
        G_timer_count = glutGet(GLUT_ELAPSED_TIME);         // Ajusta a variavel do tempo decorrido do programa
        G_timer = G_timer_count - G_timer_reset;            // Realiza o calculo do tempo decorrido de um jogo = tempo decorrido do programa - tempo em que o jogo foi iniciado
        text = to_string(G_timer/1000);                     // Converte o G_timer para uma string
        glColor3f(0.0, 0.0, 0.0);                           // Seta a cor do texto como preto
        renderText("Tempo Decorrido", 15, 21, 6);
        renderText(text.data(), text.size(), 32, 2);
    }
    else
    {
        text = to_string(G_timer/1000);                     // Converte o G_timer para uma string
        glColor3f(0.0, 0.0, 0.0);                           // Seta a cor do texto como preto
        renderText("Tempo Decorrido", 15, 21, 6);
        renderText(text.data(), text.size(), 32, 2);
    }
}

static void Minas_Adjacentes(int indice)
{// Mostrar o numero de Minas adjacentes do campo
    std::string text;                                               // texto a ser desenhado, quantidade de minas adjacentes
    text = to_string(campo_minado[indice].minas_adja);              // guarda o numero de minas adjacentes do campo, em formato de string
    glPushMatrix();
        glColor3f(0.0, 0.0, 0.0);                                   // Cor: preto
        glRasterPos2f(0.3, 0.3);                                    // desloca o local a ser renderizado o texto ...

        void * font = GLUT_BITMAP_9_BY_15;                          // Seleciona a fonte do texto -> http://openglut.sourceforge.net/group__bitmapfont.html
        for(std::string::iterator iterador_string = text.begin(); iterador_string != text.end(); ++iterador_string) // intera entre os caracteres do texto
        {
            char caractere = *iterador_string;                      // converte o iterador para caractere
            glutBitmapCharacter(font, caractere);                   // desenha o caractere
        }
    glPopMatrix();
}

static void Tabuleiro()
{// Funcao que gera o Tabuleiro
    float posStart_x = (float(G_colunas) / 2) * (-1);           // posicao x de onde comecar a desenhar o tabuleiro
    float posStart_y = (float(G_linhas) / 2) * (-1);            // posicao y de onde comecar a desenhar o tabuleiro

    int linha = 0;                                              // iterador das linhas
    int coluna = 0;                                             // iterador das colunas

    int cont = 0;                                               // contador para indicar o indice do campo

    glPushMatrix();
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
        G_click_pos_x = 0;                                      // reseta a posicao do clique no eixo x
        G_click_pos_y = 0;                                      // reseta a posicao do clique no eixo y
        glPopMatrix();
    glPopMatrix();
}

static void Calculo_Desenho(int linha, int coluna, int indice)
{// Funcao para determinar o que desenhar, com base no clique do Mouse realizado
    int operador = Calculo_Posicao(linha, coluna) ? G_operacao_desenho : 0;
    // operador ternario,
    // se o clique corresponder ao campo da interacao do atual do Tabuleiro, fazer operacoes do clique do mouse (revelar um campo, operador = 1, ou colocar/retirar a protecao(bandeira), operador = 2))
    // senao apenas mostrar as proprieades dos campos (operador = 0)
    switch(operador)
    {
        case 0:                                                         // apenas mostrar o campo conforme sua propriedade, Campo Revelado, Campo Protegido, Campo Mina ou Campo nao Revelado
            if(campo_minado[indice].revelado)                           // condicao: campo revelado ?
            {
                Revelado();
                Minas_Adjacentes(indice);
            }
            else if(campo_minado[indice].protegido)                     // condicao: campo com protecao ?
            {
                Bandeira();
            }
            // Condicao para fins de DEBUG
            /*
            else if(campo_minado[indice].campo_mina)                    // condicao: campo com mina?
            {
                // Mina();
                Quadrado();
            }
            */
            else                                                        // campo nao revelado
            {
                Quadrado();
            }
            break;
        case 1:                                                         // Funcao para revelar um campo
            if(G_estado_jogo == 1)                                      // condicao: jogo esperando 1o clique
            {
                Reset_Timer();                                          // No inicio do jogo, estar fun��o que � utilizado para resetar o tempo decorrido
                glutTimerFunc(0, Timer, 0);

                AcrescentaMina(indice);
                Calc_Minas_Adjacentes();
                G_nao_revelados = G_linhas * G_colunas - G_minas;
                G_estado_jogo = 2;
            }
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
{// Calcula se o local clicado, corresponde ao campo da itera��o na funcao Tabuleiro
    float coluna_minimo = coluna * (windowsSize_x * 0.05);                          // representa o "pixel" minimo no eixo x
    float coluna_maximo = coluna * (windowsSize_x * 0.05) + (windowsSize_y * 0.05); // representa o "pixel" maximo no eixo x, pixel minimo + tamanho x do campo (5% da largura da tela)
    float linha_minimo = linha * (windowsSize_y * 0.05);                            // representa o "pixel" minimo do eixo y
    float linha_maximo = linha * (windowsSize_y * 0.05) + (windowsSize_y * 0.05);   // representa o "pixel" maximo do eixo y, pixel minimo + tamanho y do campo (5% da altura da tela)

    if((G_click_pos_x > coluna_minimo) && (G_click_pos_x < coluna_maximo))          // Verificao do clique no eixo X
     {
         if((G_click_pos_y > linha_minimo) && (G_click_pos_y < linha_maximo))       // Verificao do clique no eixo Y
        {
            //printf("\n[DEBUG]: Entrou Calculo Posicao. Retorno True");
            return true;
        }
    }
    return false;
}

static void Revelar_Campo(int indice)
{// Funcao para revelar um campo
    if(campo_minado[indice].revelado)                   // condicao: Campo ja revelado ?
    {
        Revelado();
        Minas_Adjacentes(indice);
        //printf("\n     Campo ja revelado.");
    }
    else                                                // Campo nao revelado
    {
        if(campo_minado[indice].protegido)              // condicao: Campo com bandeira ?
        {
            Bandeira();
            //printf("\n     Campo protegido.");
        }
        else                                            // Campo sem bandeira
        {
            if(campo_minado[indice].campo_mina)         // condicao: Campo com mina ?
            {
                G_estado_jogo = 3;
                Mina();
                AbreJogoGameOver();
                renderGameOver();
                //printf("\n     Campo com mina.");
                //printf("\n\n     --- Game Over ---\n\n");
                //campo_minado[indice].revelado = true;
            }
            else                                         // Campo sem mina
            {
                G_nao_revelados--;                       // Decrescenta a quantidade de quadrados ainda n�o revelados
                //printf("\n[DEBUG] Nao Revelados: %d", G_nao_revelados);
                Revelado();                              // Cria um quadrado revelado de fundo cinza
                Minas_Adjacentes(indice);                // Mostra a quantidade de minas que o quadrado revelado possui
                campo_minado[indice].revelado = true;    // Marca o quadrado como revelado
                if(G_nao_revelados == 0){                // Condi��o para fim de jogo, se todos os quadrados que n�o s�o minas foram revelados
                    G_estado_jogo = 3;                   // Marca como fim de jogo
                    renderVenceu();                      // Mostra Mensagem de Vencedor
                }
                if(campo_minado[indice].minas_adja == 0){// Verifica se o quadrado aberto possui 0 minas adjacentes para chamar a fun��o que expande e mostra uma ilha
                    ExpandeArea(indice);
                }
            }
        }
    }
}

static void Calc_Minas_Adjacentes()
{//Realiza o calcula das minas existentes nos quadrado adjacentes ao atual
    int i; // iterador utilizado para percorrer todo o tabuleiro
    int j; // iterador utilizar para percorrer todo o tabuleiro em busca dos campos adjacentes a i.
    int tmpx; // vari�vel tempor�ria que armazena a localiza��o x do campo de indice i
    int tmpy; // vari�vel tempor�ria que armazena a localiza��o y do campo de indice i
    for(i = 0; i < G_linhas * G_colunas; i++){ //interage em todo o tabuleiro
        if(campo_minado[i].campo_mina){
            tmpx = campo_minado[i].pos_x;
            tmpy = campo_minado[i].pos_y;
            for(j=0; j<G_linhas*G_colunas; j++){ //interage em todo o tabuleiro
                if(campo_minado[j].campo_mina){ //Se o campo atual tiver mina coloca como adjacentes -1
                    campo_minado[j].minas_adja = -1;
                }else{ //Se n�o faz o calculo de minas adjacentes a ele
                    if(campo_minado[j].pos_x == tmpx-1 && campo_minado[j].pos_y == tmpy-1){  //Esquerdp e inferior
                        campo_minado[j].minas_adja += 1;
                    }
                    if(campo_minado[j].pos_x == tmpx && campo_minado[j].pos_y == tmpy-1){    //inferior
                        campo_minado[j].minas_adja += 1;
                    }
                    if(campo_minado[j].pos_x == tmpx+1 && campo_minado[j].pos_y == tmpy-1){  //Direitp e inferior
                        campo_minado[j].minas_adja += 1;
                    }
                    if(campo_minado[j].pos_x == tmpx-1 && campo_minado[j].pos_y == tmpy){    //Direitp
                        campo_minado[j].minas_adja += 1;
                    }
                    if(campo_minado[j].pos_x == tmpx+1 && campo_minado[j].pos_y == tmpy){    //Esquerdo
                        campo_minado[j].minas_adja += 1;
                    }
                    if(campo_minado[j].pos_x == tmpx-1 && campo_minado[j].pos_y == tmpy+1){  //Esquerdo e Superior
                        campo_minado[j].minas_adja += 1;
                    }
                    if(campo_minado[j].pos_x == tmpx && campo_minado[j].pos_y == tmpy+1){     //Superior
                        campo_minado[j].minas_adja += 1;
                    }
                    if(campo_minado[j].pos_x == tmpx+1 && campo_minado[j].pos_y == tmpy+1){   //Direito e Superior
                        campo_minado[j].minas_adja += 1;
                    }
                }
            }
        }
    }
    //Utilizado apenas para quest�es de DEBUG
    /*for(i=0; i<G_linhas*G_colunas; i++){
        printf("\nCampo: %d Minas Adja: %d", i, campo_minado[i].minas_adja);

    }*/
}

static void ExpandeArea(int indice)
{// Fun��o para revelar a ilha que possuir como 0 minas em suas adjacentes
    int j; // iterador utilizado para percorrer todo o tabuleiro
    int tmpx; // vari�vel tempor�ria que armazena a localiza��o x do campo de indice i
    int tmpy; // vari�vel tempor�ria que armazena a localiza��o y do campo de indice i
    if(campo_minado[indice].minas_adja == 0){
        tmpx = campo_minado[indice].pos_x;
        tmpy = campo_minado[indice].pos_y;
        for(j=0; j<G_linhas*G_colunas; j++){ //Fun��o para Interagir em todo o tabuleiro para achar as adjacentes ao quadrado clicado
            if(campo_minado[j].protegido == false && campo_minado[j].revelado == false){ // Condi��o para n�o revelar caso o quadrado tenha sido protegido ou revelado
                if(campo_minado[j].pos_x == tmpx-1 && campo_minado[j].pos_y == tmpy-1){  // Verifica se o quadrado atual da intera��o � o adjcente da esquerda inferior
                    //Revelado();
                    Minas_Adjacentes(j);  // Chama fun��o que revela a quantidade de minas adjacentes ao quadrado que ser� revelado
                    campo_minado[j].revelado = true; // Coloca o quadrado revelado
                    G_nao_revelados--;              // Decrescenta a quantidade de quadrados n�o revelados
                    if(campo_minado[j].minas_adja == 0) ExpandeArea(j); // Se o quadrado possui 0 minas adjacentes chama recursivamente a fun��o
                }
                if(campo_minado[j].pos_x == tmpx && campo_minado[j].pos_y == tmpy-1){ // Verifica se o quadrado atual da itera��o � o adjcente inferior
                    //Revelado();
                    Minas_Adjacentes(j);
                    campo_minado[j].revelado = true;
                    G_nao_revelados--;
                    if(campo_minado[j].minas_adja == 0) ExpandeArea(j);
                }
                if(campo_minado[j].pos_x == tmpx+1 && campo_minado[j].pos_y == tmpy-1){ // Verifica se o quadrado atual da itera��o � o adjcente da direita inferior
                    //Revelado();
                    Minas_Adjacentes(j);
                    campo_minado[j].revelado = true;
                    G_nao_revelados--;
                    if(campo_minado[j].minas_adja == 0) ExpandeArea(j);
                }
                if(campo_minado[j].pos_x == tmpx-1 && campo_minado[j].pos_y == tmpy){ // Verifica se o quadrado atual da itera��o � o adjcente da esderda
                    //Revelado();
                    Minas_Adjacentes(j);
                    campo_minado[j].revelado = true;
                    G_nao_revelados--;
                    if(campo_minado[j].minas_adja == 0) ExpandeArea(j);
                }
                if(campo_minado[j].pos_x == tmpx+1 && campo_minado[j].pos_y == tmpy){ // Verifica se o quadrado atual da itera��o � o adjcente da direita
                    //Revelado();
                    Minas_Adjacentes(j);
                    campo_minado[j].revelado = true;
                    G_nao_revelados--;
                    if(campo_minado[j].minas_adja == 0) ExpandeArea(j);
                }
                if(campo_minado[j].pos_x == tmpx-1 && campo_minado[j].pos_y == tmpy+1){ // Verifica se o quadrado atual da itera��o � o adjcente da esquerda superior
                    //Revelado();
                    Minas_Adjacentes(j);
                    campo_minado[j].revelado = true;
                    G_nao_revelados--;
                    if(campo_minado[j].minas_adja == 0) ExpandeArea(j);
                }
                if(campo_minado[j].pos_x == tmpx && campo_minado[j].pos_y == tmpy+1){ // Verifica se o quadrado atual da intera��o � o adjcente superior
                    //Revelado();
                    Minas_Adjacentes(j);
                    campo_minado[j].revelado = true;
                    G_nao_revelados--;
                    if(campo_minado[j].minas_adja == 0) ExpandeArea(j);
                }
                if(campo_minado[j].pos_x == tmpx+1 && campo_minado[j].pos_y == tmpy+1){ // Verifica se o quadrado atual da intera��o � o adjcente da direita superior
                    //Revelado();
                    Minas_Adjacentes(j);
                    campo_minado[j].revelado = true;
                    G_nao_revelados--;
                    if(campo_minado[j].minas_adja == 0) ExpandeArea(j);
                }
            }
        }
    }
    glutPostRedisplay();
}

static void Alternar_Protecao(int indice)
{// Funcao para alternar a protecao
    if(campo_minado[indice].protegido)                  // condicao: Campo protegido ?
    {
        Quadrado();
        G_bandeiras++;                                  // Incrementa o numero de Bandeiras disponiveis
        campo_minado[indice].protegido = false;
    }
    else                                                // Campo nao protegido
    {
        if(campo_minado[indice].revelado)               // condicao: Campo revelado ?
        {
            Revelado();
            Minas_Adjacentes(indice);
            //printf("\n     Campo ja revelado.");
        }
        else                                            // Campo nao revelado
        {
            if(G_bandeiras > 0)                         // condicao: Bandeiras disponiveis ?
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

static void AbreJogoGameOver()
{ // Se o jogo finalizar com o jogador clicando em uma mina, a fun��o � chamada para revelar todo o jogo
    int i; // iterador utilizado para percorrer todo o tabuleiro
    int controlador; // utilizado para realizar o posicionamento da impress�o dos n�meros que aparecem ao final do jogo
    std::string text;
    switch(G_linhas){  // Crontrolador de posicionamento dos n�meros(N�o Otimizado)
        case 5:
            controlador = 39;
            break;
        case 10:
            controlador = 27;
            break;
        case 15:
            controlador = 14;
    }

    for(i = 0; i < G_linhas * G_colunas; i++){  // Itera��o para revelar todo o tabuleiro
        text = to_string(campo_minado[i].minas_adja);
        glPushMatrix();
        if(campo_minado[i].minas_adja != -1){   // Condi��o para escolher a cor que o n�mero receber�
            glColor3f(0.0, 0.0, 0.0);           // Cor preto se for um quadrado sem mina
        }else{
            glColor3f(1.0, 0.0, 0.0);           // Cor vermelho se for um quadrado com mina
        }
        if(!campo_minado[i].revelado){          // Se o campo ainda n�o foi revelado mostra o n�mero
            renderText(text.data(), text.size(), campo_minado[i].pos_y*5+controlador, campo_minado[i].pos_x*5+controlador);
        }
        glPopMatrix();
    }
}

static void Atualiza_tamanho(int largura, int altura)
{ //Fun��o para ajustar o tamanho da janela
    glViewport(0, 0, largura, altura);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D (-10.0f, 10.0f, -10.0f, 10.0f);
    //gluOrtho2D (0.0f, 10.0f, 12.0f, 0.0f);
    glutReshapeWindow(windowsSize_x, windowsSize_y); //Fun��o que n�o deixa o usu�rio alterar o tamanho da tela.

    //printf("\n[DEBUG] : Evento Atualiza tamanho\n");
}

static void Atualiza_desenho(void)
{ // Fun��o para renderizar o conte�do da janela
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();

        // Desenhar Tabuleiro
        glPushMatrix();
            Tabuleiro();
        glPopMatrix();

        // Desenhar o menu de comandos para reiniciar ou sair do jogo
        glPushMatrix();
            glTranslatef(-10.0, -10.0, 0.0);
            glScalef(3.5, 2.0, 0);
            glColor3f(0.0, 0.0, 0.0);
            Menus();
        glPopMatrix();
        renderText("q: Sair", 7, 1, 8);
        renderText("r: Novo Jogo", 12, 1, 5);

        // Desenhar o menu para tempo
        glPushMatrix();
            glTranslatef(-6.0, -10.0, 0);
            glScalef(5.0, 1.8, 0);
            glColor3f(1.0f, 0.0f, 0.0f);
            Menus();
        glPopMatrix();

        // Desenhar o menu para bombas
        glPushMatrix();
            glTranslatef(0.0, -10.0, 0);
            glScalef(5.0, 1.8, 0);
            glColor3f(0.0f, 1.0f, 0.0f);
            Menus();
        glPopMatrix();

        mostraTempo();                                  // Mostra o tempo percorrido
        mostraMinas();                                  // Mostra o numero de minas/bandeiras disponiveis
    glPopMatrix();

    //glFlush();                                        // funcao para que os desenhos sejam renderizados
    glutSwapBuffers();                                  // funcao semelhante ao glFlush(), para atualizar p buffer duplo

    //printf("\n[DEBUG] : Evento Atualiza desenho\n");
}

static void Menu_grafico(void)
{ // Fun��o que dispar� um menu gr�fico para a escolha de dificuldade e visualiza��o das regras
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
        glColor3f(0.0, 0.0, 0.0);
        if(G_regras)
        {
            Quadro_Regra();
        }
        else
        {
            glPushMatrix();
                glTranslatef(-6.0, 3.0, 0);
                glScalef(12.0, 5.0, 0.0);
                Menus();
                renderText("Dificuldade", 11, 42, 87);
                renderText("Escolha uma para iniciar o jogo", 31, 27, 85);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(-4.0, 5.0, 0);
                glScalef(3.0, 1.0, 0.0);
                Menus();
                renderText("Facil", 5, 34, 77);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(1.0, 5.0, 0);
                glScalef(3.0, 1.0, 0.0);
                Menus();
                renderText("Normal", 6, 59, 77);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(-1.5, 3.5, 0);
                glScalef(3.0, 1.0, 0.0);
                Menus();
                renderText("Dificil", 7, 46, 69);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(-6.0, 1.5, 0);
                glScalef(3.0, 1.0, 0.0);
                Menus();
                renderText("Regras", 6, 23, 59);
            glPopMatrix();
        }
    glPopMatrix();
    glutSwapBuffers();
}

static void Quadro_Regra()
{// Fun��o para mostrar as regras do jogo
    glPushMatrix();
        glColor3f(0.0, 0.0, 0.0);
        glPushMatrix();
            glTranslatef(-9.0, -8.0, 0.0);
            glScalef(18, 17, 0);
            Menus();
        glPopMatrix();
    glPopMatrix();
    renderText("Regras do Jogo", 14, 40, 90);
    renderText("1. Um quadrado e revelado ao ser clicado;", 41, 6, 85);
    renderText("2. O primeiro quadrado aberto nao contem mina;", 46, 6, 80);
    renderText("3. Ao ser revelado ira aparecer um numero indicando a", 53, 6, 75);
    renderText("quantidade de minas adjacentes;", 31, 11, 73);
    renderText("4. Ao clicar em um quadrado com mina o jogo termina;", 52, 6, 70);
    renderText("5. O jogo acaba quando o jogador revelar todos os quadrados", 59, 6, 65);
    renderText("que nao sao minas", 17, 11, 63);
    glColor3f(0.0, 0.0, 1.0);
    glPushMatrix();
        glTranslatef(-8.0, -7.0, 0.0);
        glScalef(7, 1, 0);
        Menus();
    glPopMatrix();
    renderText("Voltar ao Menu Inicial", 22, 11, 16);
}

static void teclado(unsigned char tecla, int x, int y)
{
    switch (tecla)
    {
        case 27 :   // Tecla utilizada para sair do jogo
        case 'q':
            exit(0);
            break;
        case 'r':   // Tecla utilizada para reiniciar o jogo
            G_regras = false;
            G_estado_jogo = 0;
            G_timer = 0;
            glutDisplayFunc(Menu_grafico);
            glutPostRedisplay();
            glutMouseFunc(mouse_menu);
            break;
        default:
            //printf("\nNenhum evento atribuido a tecla\n");
            break;
    }
}

static void mouse(int botao, int estado, int x, int y)
{
    if(botao == GLUT_LEFT_BUTTON){
        if(estado == GLUT_DOWN){
            //printf("\n[DEBUG]: Apertou botao esquerdo mouse");
            if(G_estado_jogo > 0 && G_estado_jogo < 3)                      // condicao: jogo ja iniciao (0), esperando 1o clique (1), esperando cliques normais (2), jogo nao terminado (3)
            {
                G_click_pos_x = x - (windowsSize_x / 2);                    // ajusta a posi��o do mouse para combinar com o viewport, posicao x real - janela X / 2
                G_click_pos_x = G_click_pos_x + ((float(windowsSize_x) * 0.05) * (float(G_colunas) / 2));
                G_click_pos_y = (windowsSize_y / 2) - y ;                   // ajusta a posi��o do mouse para combinar com o viewport, anela Y / 2 - posicao y real
                G_click_pos_y = G_click_pos_y + ((float(windowsSize_y) * 0.05) * (float(G_linhas) / 2));
                //printf("\n X: %d,  Y:  %d\n", G_click_pos_x, G_click_pos_y);

                G_operacao_desenho = 1;                                     // operacao de revelar um campo
                glutPostRedisplay();
            }
        }
    }else if(botao == GLUT_RIGHT_BUTTON){
        if(estado == GLUT_DOWN){
            //printf("\n[DEBUG]: Apertou Botao direito mouse");
            if(G_estado_jogo > 0 && G_estado_jogo < 3)
            {
                G_click_pos_x = x - (windowsSize_x / 2);                    // ajusta a posi��o do mouse para combinar com o viewport, posicao x real - janela X / 2
                G_click_pos_x = G_click_pos_x + ((float(windowsSize_x) * 0.05) * (float(G_colunas) / 2));
                G_click_pos_y = (windowsSize_y / 2) - y ;                   // ajusta a posi��o do mouse para combinar com o viewport, anela Y / 2 - posicao y real
                G_click_pos_y = G_click_pos_y + ((float(windowsSize_y) * 0.05) * (float(G_linhas) / 2));
                //printf("\n X: %d,  Y:  %d\n", G_click_pos_x, G_click_pos_y);

                G_operacao_desenho = 2;                                     // operacao de alternar a Protecao
                glutPostRedisplay();
            }
        }
    }//else{
        //printf("\n[DEBUG]: Nao entendo o que vc quer clicar, pare de apertar esse botao por favor");
    //}
}

static void mouse_menu(int botao, int estado, int x, int y)
{
    if(botao == GLUT_LEFT_BUTTON){
        if(estado == GLUT_DOWN){
            //printf("\n[DEBUG]: Apertou botao esquerdo mouse");

            G_click_pos_x = x - (windowsSize_x / 2);                    // ajusta a posi��o do mouse para combinar com o viewport, posicao x real - janela X / 2
            G_click_pos_y = (windowsSize_y / 2) - y ;                   // ajusta a posi��o do mouse para combinar com o viewport, anela Y / 2 - posicao y real
            //printf("\n[DEBUG]: X: %d,  Y:  %d\n", G_click_pos_x, G_click_pos_y);
            if(G_estado_jogo == 0)                                      // condicao: jogo nao iniciado ?
            {
                if(G_regras == true)                                    // conbdicao: o Menu_Grafico esta mostrando as regras ?
                {
                    //printf("[DEBUG]: Entrou no Quadro_Regras");
                    if( (G_click_pos_x > (-8.0 * (windowsSize_x * 0.05))) &&
                             (G_click_pos_x < (-8.0 * (windowsSize_x * 0.05) + 7.0 * (windowsSize_x * 0.05))) )
                    {
                        if( (G_click_pos_y > (-7.0 * (windowsSize_y * 0.05))) &&
                           (G_click_pos_y < (-7.0 * (windowsSize_y * 0.05)) + 1.0 * (windowsSize_y * 0.05)))
                        {
                           //printf("\nVolta ao Menu");
                           glutDisplayFunc(Menu_grafico);
                           G_regras = false;                            // sair do menu das regras
                        }
                    }
                }
                else
                {
                    // Tratamento do clique para a dificuldade "Novato"
                    if( G_click_pos_x > ( ((windowsSize_x * 0.05) * (-4.0)) ) &&
                        G_click_pos_x < ( ((windowsSize_x * 0.05) * (-4.0)) + ((windowsSize_x * 0.05) * (3.0)) ) )
                    {
                        if( G_click_pos_y > ( ((windowsSize_y * 0.05) * (5.0)) ) &&
                            G_click_pos_y < ( ((windowsSize_y * 0.05) * (5.0)) + ((windowsSize_y * 0.05) * (3.0)) ) )
                        {
                            G_estado_jogo = 1;
                            glutMouseFunc(mouse);
                            G_dificuldade = 0;
                            MenuTemporario();
                            glutDisplayFunc(Atualiza_desenho);
                        }
                    }

                    // Tratamento do clique para a dificuldade "Moderado"
                    if( G_click_pos_x > ( ((windowsSize_x * 0.05) * (1.0)) ) &&
                             G_click_pos_x < ( ((windowsSize_x * 0.05) * (1.0)) + ((windowsSize_x * 0.05) * (3.0)) ) )
                    {
                        if( G_click_pos_y > ( ((windowsSize_y * 0.05) * (5.0)) ) &&
                            G_click_pos_y < ( ((windowsSize_y * 0.05) * (5.0)) + ((windowsSize_y * 0.05) * (1.0)) ) )
                        {
                            G_estado_jogo = 1;
                            glutMouseFunc(mouse);
                            G_dificuldade = 1;
                            MenuTemporario();
                            glutDisplayFunc(Atualiza_desenho);
                        }
                    }

                    // Tratamento do clique para a dificuldade "Normal"
                    if( G_click_pos_x > ( ((windowsSize_x * 0.05) * (-1.5)) ) &&
                             G_click_pos_x < ( ((windowsSize_x * 0.05) * (-1.5)) + ((windowsSize_x * 0.05) * (3.0)) ) )
                    {
                        if( G_click_pos_y > ( ((windowsSize_y * 0.05) * (3.5)) ) &&
                            G_click_pos_y < ( ((windowsSize_y * 0.05) * (3.5)) + ((windowsSize_y * 0.05) * (1.0)) ) )
                        {
                            G_estado_jogo = 1;
                            glutMouseFunc(mouse);
                            G_dificuldade = 2;
                            MenuTemporario();
                            glutDisplayFunc(Atualiza_desenho);
                        }
                    }

                    // Tratamento do clique para as regras
                    if( G_click_pos_x > ( ((windowsSize_x * 0.05) * (-6.0)) ) &&
                             G_click_pos_x < ( ((windowsSize_x * 0.05) * (-6.0)) + ((windowsSize_x * 0.05) * (3.0)) ) )
                    {
                        if( G_click_pos_y > ( ((windowsSize_y * 0.05) * (1.5)) ) &&
                            G_click_pos_y < ( ((windowsSize_y * 0.05) * (1.5)) + ((windowsSize_y * 0.05) * (1.0)) ) )
                        {
                            G_regras = true;                            // entrar no menu de regras
                        }
                    }
                }
            }
            inicializaCampos();  // Inixializa os campos do tabuleiro
            glutPostRedisplay();
        }
    }
}

static void Timer(int value)
{// Fun��o controlar o tempo de atualizacao
    if(G_estado_jogo != 3)                                // condicao: Enquanto o jogo nao acabar, realizar a atualizacao
    {
        glutPostRedisplay();
        glutTimerFunc(INTERVALO_TEMPO, Timer, 0);
    }
}

static void Reset_Timer()
{// Funcao que armazena o momento em que a funcao � chamada, no inicio de um jogo
    G_timer_reset = glutGet(GLUT_ELAPSED_TIME);             // Ajusta a variavel do tempo de inicio de um jogo, com base no tempo decorrido desde a criacao da janela, a variavel � utilizada no calculo do tempo decorrido de um jogo
    //printf("\n[DEBUG]: Reset Timer: %d", G_timer_reset);
    //printf("\n[DEBUG]: Timer Count: %d", G_timer_count);
}

int main(){

    glutInitWindowSize(windowsSize_x, windowsSize_y);   // Define o tamanho da janela
    glutInitWindowPosition(300, 0);                     // Define a posicao da janela
    glutInitDisplayMode (GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    /* GLUT_DOUBLE - modo de buffer duplo,
    a renderizacao mostrada se encontra em um buffer "frontal",
    enquanto a proxima renderizacao � preparada em um buffer "traseiro"
    suaviza anima��es e proporcionar menor utiliza��o de processamento e memoria [em nossos testes]
    - trocar glFlush por glutSwapBuffers
    */

    glutCreateWindow("Campo Minado");                   // Define o nome da janela

    glutDisplayFunc(Menu_grafico);                      // Atualizar tela
    glutReshapeFunc(Atualiza_tamanho);                  // Atualiza tamanho

    glutKeyboardFunc(teclado);                          // Acrescenta fun��o de tecla
    glutMouseFunc(mouse_menu);                          // Acrescenta fun��o de clique

    glClearColor(1,1,1,1);                              // Cria tela de fundo Branca
    glutMainLoop();                                     // Loop infinito at� finalizar a janela

    return 0;
}
