/*  Trabalho Final de Computação Gráfica - Batalha Espacial
    Acadêmicos: André, Eduarda e Maria Eduarda
    Matrículas: 173702, 173705, 173720
*/


#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


GLint gameOver = 0; // 1 = Fim de jogo e 0 = Jogo em andamento
GLint menu = 1; //Inicio no menu
GLint placar = 0;
GLint vidas = 3;
GLint win = 0;
GLint lvl = 0;

//Move a nave para esquerda e direita
GLfloat posNaveX = 50; //Comecar a nave no centro da tela
GLfloat posTiroY = 100; 
GLfloat posTiroX = -10; 

GLint sentidoInimigoX = 1; //Sentido de movimento do eixo x: esquerda = 0 e direita = 1

GLuint NavePlayer;

GLuint Fundo;

GLuint NaveInimiga;

GLuint Vida;

GLuint Asteroide;


GLuint carregaTextura(const char* arquivo) {
    GLuint idTextura = SOIL_load_OGL_texture(arquivo, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    return idTextura;
}


struct inimigo {
  GLfloat posX;
  GLfloat posY;
  GLint estaVivo;
};

struct tiroinimigo {
  GLfloat posX;
  GLfloat posY;
};

struct inimigo inimigos[4][10]; // Matriz de inimigos 3x7
struct tiroinimigo tiroinimigo;

void iniciainimigos() {
  int i, j;
  for(i=0; i<3; i++) {
    for(j=0; j<7; j++) {
      inimigos[i][j].posX = j*10 + 10;
      inimigos[i][j].posY = 100-(i*10 + 10);
      inimigos[i][j].estaVivo = 1;
    }
  }

  tiroinimigo.posX = 0;
  tiroinimigo.posY = -10; 
}

void desenhainimigos() {
    int i, j;
    for(i=0; i<3; i++) {
      for(j=0; j<7; j++) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, NaveInimiga);
        glBegin(GL_POLYGON);
            if(inimigos[i][j].estaVivo) { //Só mostra o inimigo se ele estiver vivo

              glTexCoord2f(0, 0);
              glVertex3f(inimigos[i][j].posX-3, inimigos[i][j].posY-3, 0);
              glTexCoord2f(0, 1);
              glVertex3f(inimigos[i][j].posX-3, inimigos[i][j].posY+3, 0);
              glTexCoord2f(1, 1);
              glVertex3f(inimigos[i][j].posX+3, inimigos[i][j].posY+3, 0);
              glTexCoord2f(1, 0);
              glVertex3f(inimigos[i][j].posX+3, inimigos[i][j].posY-3, 0);
            }
            
        glEnd();
        glDisable(GL_TEXTURE_2D);
      }
    }
}

void verificainimigoChegouFim() {
  //Verifica se algum inimigo conseguiu descer todo o eixo y 
  int i, j;
  for(i=0; i<3; i++) {
    for(j=0; j<7; j++) {
      if(inimigos[i][j].estaVivo) {
        if(inimigos[i][j].posY < 12) {
          //Inimigo ganhou
          gameOver = 1;
        }
      }
    }
  }
}

void deslocainimigos(GLfloat eixoX, GLfloat eixoY) {
  int i, j;
  //Faz o movimento dos inimigos na horizontal
  for(i=0; i<3; i++) {
    for(j=0; j<7; j++) {
      inimigos[i][j].posX += eixoX;
      inimigos[i][j].posY += eixoY;
    }
  }
}

void movimentainimigos() {
  int i, j;
  
  int menorX = 100;
  int maiorX = 0;
  for(i=0; i<3; i++) {
    for(j=0; j<7; j++) {
      if(inimigos[i][j].estaVivo) {
        if(inimigos[i][j].posX > maiorX) {
          maiorX = inimigos[i][j].posX;
        }
        if(inimigos[i][j].posX < menorX) {
          menorX = inimigos[i][j].posX;
        }
      }
    }
  }
  

  if(maiorX >= 97) { 
    sentidoInimigoX = 0; //Inverte o sentido (para de ir para direita e comeca ir pra esquerda)
    deslocainimigos(0, -3);
    verificainimigoChegouFim();
  }
  if(menorX <= 3) {
    sentidoInimigoX = 1;//Inverte o sentido (para de ir para esquerda e comeca ir pra direita)
    deslocainimigos(0, -3); 
    verificainimigoChegouFim();
  }
  
  //Faz o movimento dos inimigos na horizontal
  if(sentidoInimigoX == 1) {
    deslocainimigos(0.5, 0); //Desloca os inimigos na horizontal em 0.3 posicoes
  } else {
    deslocainimigos(-0.5, 0); //Desloca os inimigos na horizontal em -0.3 posicoes
  }
    
}

void jogaAsteroides() {

  int asteroide;

  if(lvl == 1){
    asteroide = -100;
  }
  if(lvl == 2){
    asteroide = -50;
  }
  if(lvl == 3){
    asteroide = 10;
  }

  if(tiroinimigo.posY <= asteroide) { //So dar outro tiro se o ultimo tiro estiver oculto (y menor que -10 = fora da tela)
    int deuTiro = 0;
    int xAleatorio, yAleatorio;
    int index = 0;
    while(deuTiro == 0 && index <= 50) {
      xAleatorio = rand() % 3; 
      yAleatorio = rand() % 7; 
      if(inimigos[xAleatorio][yAleatorio].estaVivo) {
        tiroinimigo.posX = inimigos[xAleatorio][yAleatorio].posX;
        tiroinimigo.posY = inimigos[xAleatorio][yAleatorio].posY -3;
        deuTiro = 1;
      }
      index++;
    }
  } else {
    tiroinimigo.posY--;
    
    // Verificar se colidiu o tiro do inimigo com a nave
    if(tiroinimigo.posX >= posNaveX-3 && tiroinimigo.posX <= posNaveX+3) {
      if(tiroinimigo.posY >= 2 && tiroinimigo.posY <= 10) {
        //Se acertou, joga o tiro para fora da tela
        tiroinimigo.posY = asteroide;
        vidas--;
        if(vidas == 0) {
          gameOver = 1;
        }
      }
    }
  }
}



void darTiroinimigos() {

  int tiro;

  if(lvl == 1){
    tiro = -100;
  }
  if(lvl == 2){
    tiro = -50;
  }
  if(lvl == 3){
    tiro = 10;
  }

  if(tiroinimigo.posY <= tiro) { //So dar outro tiro se o ultimo tiro estiver oculto (y menor que -10 = fora da tela)
    int deuTiro = 0;
    int xAleatorio, yAleatorio;
    int index = 0;
    while(deuTiro == 0 && index <= 50) {
      xAleatorio = rand() % 3; 
      yAleatorio = rand() % 7; 
      if(inimigos[xAleatorio][yAleatorio].estaVivo) {
        tiroinimigo.posX = inimigos[xAleatorio][yAleatorio].posX;
        tiroinimigo.posY = inimigos[xAleatorio][yAleatorio].posY -3;
        deuTiro = 1;
      }
      index++;
    }
  } else {
    tiroinimigo.posY--;
    
    // Verificar se colidiu o tiro do inimigo com a nave
    if(tiroinimigo.posX >= posNaveX-3 && tiroinimigo.posX <= posNaveX+3) {
      if(tiroinimigo.posY >= 2 && tiroinimigo.posY <= 10) {
        //Se acertou, joga o tiro para fora da tela
        tiroinimigo.posY = tiro;
        vidas--;
        if(vidas == 0) {
          gameOver = 1;
        }
      }
    }
  }
}

void desenhaNave() {
  
    // Habilita o uso de texturas
    glColor3f(1, 1, 1); 
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, NavePlayer);

    glBegin(GL_POLYGON);
        
        glTexCoord2f(0, 0);
        glVertex3f(posNaveX-4, 2, 0);
        
        glTexCoord2f(0, 1);
        glVertex3f(posNaveX-4, 11, 0);
        
        glTexCoord2f(1, 1);
        glVertex3f(posNaveX+4, 11, 0);
        
        glTexCoord2f(1, 0);
        glVertex3f(posNaveX+4, 2, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void movimentaTiro() {
  int i, j;
  posTiroY += 2;
  
  //Verifica se o tiro encostou em algum inimigos. Verifica todos os inimigos
  if(posTiroY >= 0 && posTiroY <= 100) { 
    for(i=0; i<3; i++) {
      for(j=0; j<7; j++) {
        if(posTiroY >= inimigos[i][j].posY-3 && posTiroY <= inimigos[i][j].posY+3) {
          if(posTiroX >= inimigos[i][j].posX-3 && posTiroX <= inimigos[i][j].posX+3) {
            //Verifica se esse inimigo esta vivo
            if(inimigos[i][j].estaVivo) {
              //Tiro encostou no inimigo
              inimigos[i][j].estaVivo = 0;
              posTiroX = -10;
              posTiroY = 100;
              placar++;
              if(placar == 21) {
                tiroinimigo.posY = -10; //Quando vencer o jogo, sumir com o tiro da nave
                win = 1;
              }
            }
          }
        }
      }
    }
  }
}

void movimentaItems(int) {
  if(!gameOver && !menu) { //Nao movimenta os items se gameover ou se estiver no menu
    movimentaTiro();
    movimentainimigos();
    darTiroinimigos();
    jogaAsteroides();
  }
  glutPostRedisplay();
  glutTimerFunc(33, movimentaItems, 1);
}

void desenhaTiro() {
    glBegin(GL_POLYGON);
        glColor3f(1, 1, 0); // Cor do tiro
        glVertex3f(posTiroX-0.5, posTiroY-2, 0);
        glVertex3f(posTiroX-0.5, posTiroY+2, 0);
        glVertex3f(posTiroX+0.5, posTiroY+2, 0);
        glVertex3f(posTiroX+0.5, posTiroY-2, 0);
    glEnd();
}

void desenhaAsteroide() { 
  glBegin(GL_POLYGON);
  glColor3f(1, 1, 0); // Cor do tiro
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, Asteroide);
        glVertex3f(posTiroX-0.5, posTiroY-2, 0);
        glVertex3f(posTiroX-0.5, posTiroY+2, 0);
        glVertex3f(posTiroX+0.5, posTiroY+2, 0);
        glVertex3f(posTiroX+0.5, posTiroY-2, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void desenhaTiroinimigo() {
    glBegin(GL_POLYGON);
        glColor3f(1, 0, 0); // Cor do tiro do inimigo
        glVertex3f(tiroinimigo.posX-0.5, tiroinimigo.posY-2, 0);
        glVertex3f(tiroinimigo.posX-0.5, tiroinimigo.posY+2, 0);
        glVertex3f(tiroinimigo.posX+0.5, tiroinimigo.posY+2, 0);
        glVertex3f(tiroinimigo.posX+0.5, tiroinimigo.posY-2, 0);
    glEnd();
}

void desenhaTexto(char string[], int x, int y, int fonte) {
  glColor3f(1,1,1); //Texto branco
  glRasterPos2f(x, y);
  int i;
  for (i = 0; i < (int)strlen(string); i++) {
    if(fonte == 1) {
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
    } else if (fonte == 2) {
      glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);
    }
  }
}

void desenhaPlacar()
{
  char string[20];
  snprintf(string, 20, "Score: %d", placar);
  desenhaTexto(string, 75, 95, 2);
}


void desenhaVidas()
{
  int i;
  for(i=0;i<vidas;i++) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Vida);

    glBegin(GL_POLYGON);

        glTexCoord2f(0, 0);
        glVertex3f(18+(i*5), 94, 0);
        
        glTexCoord2f(0, 1);
        glVertex3f(18+(i*5), 98, 0);
        
        glTexCoord2f(1, 1);
        glVertex3f(22+(i*5), 98, 0);
        
        glTexCoord2f(1, 0);
        glVertex3f(22+(i*5), 94, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
  }
}


void desenhaFundo() {
  // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Fundo);

    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(0, 0, 0);
        
        glTexCoord2f(0, 1);
        glVertex3f(0, 100, 0);
        
        glTexCoord2f(1, 1);
        glVertex3f(100, 100, 0);
        
        glTexCoord2f(1, 0);
        glVertex3f(100, 0, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void desenhaGameOver() {
    if(gameOver) {
      glBegin(GL_POLYGON);
          glColor3f(0, 0, 0);
          glVertex3f(0, 0, 0);
          
          glVertex3f(0, 100, 0);
          
          glVertex3f(100, 100, 0);
          
          glVertex3f(100, 0, 0);
      glEnd();
      glDisable(GL_TEXTURE_2D);
      desenhaTexto("GAME OVER", 33, 55, 1);
    }
}

void desenhawin() {
  if(win) {
    desenhaTexto("Parabens! Voce salvou a Galaxia!", 15, 60, 1);
  }
}

void desenhamenu() {
 if(menu) {
    glBegin(GL_POLYGON);
        glColor3f(0, 0, 0);
        glVertex3f(0, 0, 0);
        
        glVertex3f(0, 100, 0);
        
        glVertex3f(100, 100, 0);
        
        glVertex3f(100, 0, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    desenhaTexto("Bem vindo a Batalha Espacial!", 16, 60, 1);
    desenhaTexto("Escolha seu nivel de dificuldade:", 15, 54, 1);
    desenhaTexto("1 - Facil  2 - Normal  3 - Dificil", 15, 40, 2);
    desenhaTexto("Instrucoes:", 10, 25, 2);
    desenhaTexto("Atirar - Barra de Espaco", 10, 20, 2);
    desenhaTexto("Mover - setas ( <-  -> )", 10, 15, 2);
  }
}

void desenhaMinhaCena() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 100, 0, 100, -1, 1); // Tela divida em 100 "pedacos" no eixo x e y
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    desenhaFundo();
    desenhainimigos();
    desenhaTiro();
    desenhaTiroinimigo();
    desenhaAsteroide();
    desenhaNave();
    
    desenhaPlacar();
    desenhaVidas();
    desenhawin();
    
    desenhamenu();
    desenhaGameOver();
    
    glFlush();
}

void inicializa() {
    // Background preto
    glClearColor(0, 0, 0, 1);

    glEnable(GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    NavePlayer = carregaTextura("img/NavePlayer.png");
    Fundo = carregaTextura("img/fundo.png");
    NaveInimiga = carregaTextura("img/NaveInimiga.png");
    Vida = carregaTextura("img/Vida.png");
    Asteroide = carregaTextura("img/Asteroide.png");
  
}

void reiniciaVar(){
  posNaveX = 50; // Reinicia a posicao da nave
  posTiroY = 100; // Reinicia a posicao do tiro
  posTiroX = -10; // Reinicia a posicao do tiro
  sentidoInimigoX = 1;
  gameOver = 0;
  vidas = 3;
  placar = 0;
  win = 0;
}

void reiniciaJogolvl1() {
  iniciainimigos();
  reiniciaVar();
}

void reiniciaJogolvl2() {
  iniciainimigos();
  reiniciaVar();
}

void reiniciaJogolvl3() {
  iniciainimigos();
  reiniciaVar();
}


void tecla(unsigned char key, int x, int y) {
    // Verifica qual tecla foi pressionada
    switch(key) {
    case 27:      // "ESC"
        exit(0);  // Encerra a aplicacao
        break;
    case 32:// Logica do tiro
          if(posTiroY >= 100) {
            posTiroY = 11; 
            posTiroX = posNaveX;
          }    
    case 49:// nível 1
      if(menu) {
        menu = 0;
        lvl = 1;
        reiniciaJogolvl1();
      }
    case 50:// nível 2
      if(menu) { 
          menu = 0;
          lvl = 2;
          reiniciaJogolvl2();
      }
    case 51:// nível 3
      if(menu) { 
          menu = 0;
          lvl = 3;
          reiniciaJogolvl3();
      }
}
}

void teclaEspecial(GLint key, int x, int y) {
    int i;
    switch(key) {
      case GLUT_KEY_LEFT: //Seta para esquerda
        if(posNaveX >=4) {
          posNaveX -= 4;
        }
        break;
        
      case GLUT_KEY_RIGHT: //Seta para direita
        if(posNaveX <= 96) {
          posNaveX += 4;
        }
        break;
          
      default:
        break;
    }
    desenhaMinhaCena();
}

// Função principal
int main(int argc, char** argv) {
   glutInit(&argc, argv);
   
   glutInitContextVersion(1, 1);
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);

   glutCreateWindow("Trabalho final de C. Grafica - Batalha espacial");
   

   glutDisplayFunc(desenhaMinhaCena);
   glutKeyboardFunc(tecla);
   glutSpecialFunc(teclaEspecial);
   glutTimerFunc(33, movimentaItems, 1); // 1000/33 = 30 frames por segundo

   inicializa();
   
   glutMainLoop();
   return 0;
}