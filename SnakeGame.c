#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

// Definindo o tamanho do campo e as dire��es da cobra
#define WIDTH 40
#define HEIGHT 20
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

// Fun��o para esconder o cursor no console
//� basicamente pra n�o ficar com a tela piscando e passando o cursor do mouse loucamente
void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
    cursorInfo.bVisible = FALSE; // Define o cursor como invis�vel
    SetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
}

// Fun��o para definir a posi��o do cursor no console
//Resumindo para marcar onde vai atualizar a tela, nesse caso vai marcar somente a area de locomo��o da cobra 
//para n ficar piscando as bordas.
void setCursorPosition(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {x, y};
    SetConsoleCursorPosition(hConsole, pos);
}

// Fun��o principal
int main() {
	
	//Caracter que representa a cabe�a da cobra, botei pra direita mas poderia ser qualquer posi��o;
    char head = '>';
    
    //Chama a fun��o de esconder o cursor no console
	hideCursor();
    
	//Decla��o das Variaveis;
	int gameOver;
    int x, y, foodX, foodY, score;
    int tailX[100], tailY[100]; //Posi��o das caudas;
    int nTail; //Qtde de caudas;
    int dir; //Dire��o;

    // Fun��o para configurar o jogo;
    void setup() {
        gameOver = 0;
        dir = RIGHT; // Dire��o inicial, nesse caso come�a pra direita;
        x = WIDTH / 2; // Posi��o inicial da cabe�a da cobra (x) Ou seja no centro do campo;
        y = HEIGHT / 2; // Posi��o inicial da cabe�a da cobra (y) Ou seja no centro do campo;
        foodX = rand() % WIDTH; // Posi��o inicial da comida (x) Um valor aleatorio dentro do campo;
        foodY = rand() % HEIGHT; // Posi��o inicial da comida (y) Um valor aleatorio dentro do campo;
        score = 0; //Pontua��o come� zerada;
        nTail = 0; // Comprimento inicial da cauda, sempre 0;
        Beep(262, 300); // C4
    	Beep(294, 300); // D4
    	Beep(330, 300); // E4
    	Beep(349, 300); // F4
    	Beep(392, 300); // G4
    	Beep(440, 300); // A4
    	Beep(494, 300); // B4
    	Beep(523, 300); // C5
    }

    // Fun��o para desenhar a borda do campo;
    void drawBorder() {
        int i;
        setCursorPosition(0, 0); // Posi��o inicial do cursor, necessario pra saber de onde vai come�ar a borda;
        for (i = 0; i < WIDTH + 2; i++) printf("#"); // Desenha a borda superior;
        printf("\n");//Quebra de linha para come�ar as bordas laterais;

        for (i = 0; i < HEIGHT; i++) {
            setCursorPosition(0, i + 1);//Posi��o inicial do cursor, por�m uma abaixo para come�ar a borda lateral esquerda;
            printf("#"); // Desenha a borda esquerda;
            setCursorPosition(WIDTH + 1, i + 1);//Posi��o do curso ao final da largura m�xima do campo e tamb�m uma posi��o abaixo para come�ar a borda lateral direita;
            printf("#"); // Desenha a borda direita;
        }

        setCursorPosition(0, HEIGHT + 1);//Posi��o do curso ao final da aultra m�xima do campo e tamb�m uma posi��o abaixo para come�ar a borda inferior;
        for (i = 0; i < WIDTH + 2; i++) printf("#"); // Desenha a borda inferior, come�ando da esqueda para direita at� chegar nna largura m�xima do campo;
        printf("\n");

        printf("Score: %d\n", score); // Exibe a pontua��o;
    }

    // Fun��o para atualizar a pontua��o;
    void updateScore() {
        setCursorPosition(0, HEIGHT + 2); // Marca onde fica o placa na tela;
        printf("Score: %d", score); //Exibe o placar;
    }

    // Fun��o para desenhar a cobra e a comida;
    void drawSnakeAndFood() {
        int i, k;
        setCursorPosition(x + 1, y + 1); // Ajusta a posi��o para desenhar dentro da borda;
        printf("%c", head);//Desenha a cabe�a da cobra;
        setCursorPosition(foodX + 1, foodY + 1); // Ajusta a posi��o da comida;
        printf("M");//Desenha a comida;

        for (k = 0; k < nTail; k++) {
            setCursorPosition(tailX[k] + 1, tailY[k] + 1); // Ajusta a posi��o da cauda
            printf("O");//Desenha a calda
        }
    }

    // Fun��o para limpar a posi��o anterior da cobra e da cauda
    void clearPrevious() {
        int i;
        for (i = 0; i < nTail; i++) {
            setCursorPosition(tailX[i] + 1, tailY[i] + 1); // Ajusta a posi��o para limpar a cauda, faz parte da logica de locomo��o da cobra;
            printf(" ");//Limpa a ultima posi��o da cauda da cobra ao se mexer;
		}
        setCursorPosition(x + 1, y + 1); // Ajusta a posi��o para limpar a cabe�a, faz parte da logica de locomo��o da cobra;
        printf(" ");//Limpa a ultima posi��o da cabe�a da cobra ao se mexer;
    }

    // Fun��o para capturar a entrada do usu�rio, 
    void input() {
        if (_kbhit()) { //_kbhit retorna um valor diferente de 0 se uma tecla for pressionada. Caso n�o seja, ir� retornar 0;
            switch (_getch()) {//Seta as dire��es da cobra de acordo com a tecla pressionada;
                case 'a':
                    dir = LEFT;
                    head = '<';
                    break;
                case 'd':
                    dir = RIGHT;
                    head = '>';
                    break;
                case 'w':
                    dir = UP;
                    head = '^';
                    break;
                case 's':
                    dir = DOWN;
                    head = 'v';
                    break;
                case 'x':
                    gameOver = 1; // Termina o jogo se o usu�rio pressionar 'x';
                    break;
            }
        }
    }

    // Fun��o para a l�gica do jogo
    void logic() {
        int i;
        int prevX = tailX[0];
        int prevY = tailY[0];
        int prev2X, prev2Y;
        tailX[0] = x;
        tailY[0] = y;
        //Logica da locomo��o da cauda das cobras de acordo com as caudas de tr�s seguindo as caudas da frente/cabe�a
        for (i = 1; i < nTail; i++) {
            prev2X = tailX[i];
            prev2Y = tailY[i];
            tailX[i] = prevX;
            tailY[i] = prevY;
            prevX = prev2X;
            prevY = prev2Y;
        }
        switch (dir) {//Avan�a uma posi��o da cobra para a dire��o ativa;
            case LEFT:
                x--;
                break;
            case RIGHT:
                x++;
                break;
            case UP:
                y--;
                break;
            case DOWN:
                y++;
                break;
        }
        // Impede que a cobra atravesse as bordas;
        if (x >= WIDTH) x = 0; else if (x < 0) x = WIDTH - 1;
        if (y >= HEIGHT) y = 0; else if (y < 0) y = HEIGHT - 1;

        // Verifica se a cobra colidiu com a pr�pria cauda;
        for (i = 0; i < nTail; i++) {
            if (tailX[i] == x && tailY[i] == y)
                gameOver = 1;
        }

        // Verifica se a cobra comeu a comida;
        if (x == foodX && y == foodY) {
            score += 10;
            Beep(1500, 100);
            //Gera uma nova comida;
            foodX = rand() % WIDTH;
            foodY = rand() % HEIGHT;
            //Adiciona mais uma cauda;
            nTail++;
            //Atualiza as pontua��es;
            updateScore();
        }
    }

    // Chama as fun��es e inicializa e desenha a borda do campo de jogo;
    setup();
    drawBorder();
    while (!gameOver) {
        clearPrevious();
        logic();
        drawSnakeAndFood();
        input();
        Sleep(80); // recarregar a cada 80 milisegundos;
    }
    if (gameOver) {
        setCursorPosition(0, HEIGHT + 3);
        printf("PERDEU KKKKJ\n");
        Beep(392, 300); // G4
    	Beep(370, 300); // F#4
    	Beep(349, 300); // F4
    	Beep(330, 300); // E4
    	Beep(311, 300); // D#4
    	Beep(294, 300); // D4
        Sleep(5000);
    }
    return 0;
}

