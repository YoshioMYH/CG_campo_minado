#include <gl/glut.h>
#include "primitivas.h"

void Menus()
{// Cria os campos dos Menus
    glBegin(GL_LINE_LOOP);
        glVertex2f(0, 0);
        glVertex2f(0, 1);
        glVertex2f(1, 1);
        glVertex2f(1, 0);
    glEnd();
}

void Quadrado()
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

void Revelado()
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

void Mina()
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

void Bandeira()
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
