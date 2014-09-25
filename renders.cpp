#include <gl/glut.h>
#include "renders.h"

void renderText(const char *text, int length, int x, int y)
{// Função para renderizar o texto
    glMatrixMode(GL_PROJECTION);
    double *matrix = new double[16];
    glGetDoublev(GL_PROJECTION_MATRIX, matrix);
    glLoadIdentity();
    glOrtho(0, 100, 0, 100, -5, 5); //Cria um ortho apenas para a escrita contendo a suas próprias coordenadas para ficar com maior precisão
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glRasterPos2i(x, y);
    for(int i=0; i<length; i++){
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(matrix);
    glMatrixMode(GL_MODELVIEW);
}

void renderGameOver()
{ //Realiza o desenho da escrita GAME OVER utilizando #'s
    int i; //Variável utilizada para os fors
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0); //Cor do texto a ser impresso
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
    renderText("Pressione q para sair ou r para reiniciar", 41, 25, 89); //Escreve a frase abaixo do desenho
    glPopMatrix();

}

void renderVenceu()
{// Função para desenha mensagem Você Venceu quando o jogador vencer a partida
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
    //renderText("Pressione q para sair", 21, 30, 89); //Escreve a frase abaixo do desenho
    glPopMatrix();
}
