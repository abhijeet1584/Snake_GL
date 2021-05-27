#include <GL/glut.h>
#include <iostream>
#include <GL/freeglut.h>
using namespace std;

#include "shapes.h"

// constants
#define WINDOW_HEIGHT 700
#define WINDOW_WIDTH 700
#define GRID_UNIT 700/20

typedef struct {
    float *x;
    float *y;
    float snakeBlockDimension;
    char direction;
    int bodyLength;
} Snake;

typedef struct {
    float x;
    float y;
    int eaten;
} Apple;

Snake snake;
Apple apple;

bool paused = false;

// Function signatures 
void timer(int);
void display(void);
void processSpecialKeys(int, int, int);
void initSnake(void);
void snakeUpdate(void);
void randomApple(void);
float* randomCordinate(void);
void checkApple(void);
void placeApple(void);
void reshape(int, int);
void renderText(float, float, string);
void keyPressed(unsigned char, int x, int y);

// Initialization function
void init(void) {
    // background color
    glClearColor(0, 0, 0, 1.0);
    // glClearColor(0.360, 0.360, 0.360, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Projeting the graph
    gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
}

// starting point
int main(int argc, char** argv) {
    initSnake();
    placeApple();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(0, 0);

    glutCreateWindow("Snake Game");

    // changing the status from paused to running
    paused = false;

    glutDisplayFunc(display);
    glutSpecialFunc(processSpecialKeys);
    glutKeyboardFunc(keyPressed);
    glutTimerFunc(0, timer, 0);
    init();

    glutMainLoop();
}

// Timer function (controls FPS)
void timer(int a) {
    glutPostRedisplay();
    glutTimerFunc(90, timer, 0);
}

// display function
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.490, 0.490, 0.490);
    drawGrid(WINDOW_WIDTH, WINDOW_HEIGHT, 20, 20, 1);

    if (paused == true) {
        return;
    }

    glColor3f(0, 0, 0);
    glColor3f(1.0, 0, 0);
    filledSquare(apple.x, apple.y, GRID_UNIT - 1);
    
    // Snake update and rendering
    snakeUpdate();
    for (int parts = 0; parts < snake.bodyLength; parts++) {
        if (parts == 0) {
            // glColor3f(0.4, 0.8, 1);
            glColor3f(0, 1, 0);
            filledSquare(snake.x[parts], snake.y[parts], snake.snakeBlockDimension);
        } else {
            glColor3f(0, 1, 0);
            filledSquare(snake.x[parts], snake.y[parts], snake.snakeBlockDimension);
        }
    }

    // checking for apple collision
    checkApple();
    string score = "Score: ";
    score.append(to_string(apple.eaten));
    renderText(8, 17, score);

    glutSwapBuffers();
}

// function to process speical keyPresses
void processSpecialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT && snake.direction != 'L')
        snake.direction = 'R';

    if (key == GLUT_KEY_LEFT && snake.direction != 'R')
        snake.direction = 'L';
    
    if (key == GLUT_KEY_UP && snake.direction != 'D')
        snake.direction = 'U';

    if (key == GLUT_KEY_DOWN && snake.direction != 'U')
        snake.direction = 'D';
}

// Function to process normal key presses
void keyPressed(unsigned char key, int x, int y) {
    // handling pause
    if (key == ' ' && paused == false) {
        paused = true;
        return;
    }
    /**
     * using return, since the key can be pressed for more than the specified time and would reslut in calling the below if
     * which would be true, since the first if make the paused true.
     */
    if (key == ' ' && paused == true) {
        paused = false;
    }
}

// random direction for the snake
char randomDirection() {
    char directions[] = {'U', 'D', 'L', 'R'};
    int r = random() % 4;
    return directions[r];
}

// Initialization of the snake
void initSnake() {
    snake.x = (float*) malloc(sizeof(float) * WINDOW_WIDTH);
    snake.y = (float*) malloc(sizeof(float) * WINDOW_HEIGHT);
    // initial position of the snake
    snake.x[0] = GRID_UNIT * 11;
    snake.y[0] = GRID_UNIT * 11;
    snake.snakeBlockDimension = GRID_UNIT - 1; // since the width of the grid lines is 1 pixel
    snake.direction = randomDirection();
    snake.bodyLength = 2;
}

void snakeUpdate() {
    for (int i = snake.bodyLength; i > 0; i--) {
        if ((snake.x[0] == snake.x[i]) && (snake.y[0] == snake.y[i])) {
            snake.bodyLength = 2;
            std::cout << "Snake reset" << std::endl;
            apple.eaten = 0;
        }
    }
    for (int i = snake.bodyLength; i > 0; i--) {
        snake.x[i] = snake.x[i - 1];
        snake.y[i] = snake.y[i - 1];
    }

    switch(snake.direction) {
        case 'U' : snake.y[0] -= GRID_UNIT;
            break;

        case 'D' : snake.y[0] += GRID_UNIT;
            break;

        case 'L' : snake.x[0] -= GRID_UNIT;
            break;

        case 'R' : snake.x[0] += GRID_UNIT;
            break;
    }

    // Window constraints
    if (snake.x[0] + GRID_UNIT >= WINDOW_WIDTH + GRID_UNIT)
        snake.x[0] = 0;
    
    if (snake.x[0] <= -GRID_UNIT)
        snake.x[0] = WINDOW_WIDTH;

    if (snake.y[0] >= WINDOW_HEIGHT)
        snake.y[0] = 0;

    if (snake.y[0] <= -GRID_UNIT)
        snake.y[0] = WINDOW_HEIGHT;
}

void checkApple() {
    if (snake.x[0] == apple.x && snake.y[0] == apple.y) {
        snake.bodyLength++;
        apple.eaten++;
        placeApple();
    }
}

void placeApple(void) {
    float x = random() % (20);
    apple.x = x * GRID_UNIT;
    float y = random() % (20);
    apple.y = y * GRID_UNIT;
}

// function to render text on the screen
void renderText(float x, float y, string text) {
    glColor3f(1, 1, 1);
    glRasterPos2f(x, y);
    for (int i = 0; i < text.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}