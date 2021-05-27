#ifndef SHAPES
#define SHAPES

#include <GL/glut.h>

// Function to draw a filled square
void filledSquare(float user_x, float user_y, float size) {
    // Using half, since OpenGL takes the cordinates from the center
    float half = size / 2;
    float x = user_x + half;
    float y = user_y + half;
    glBegin(GL_POLYGON);
    glVertex2d(x - half, y + half);
    glVertex2d(x - half, y - half);
    glVertex2d(x + half, y - half);
    glVertex2d(x + half, y + half);
    glEnd();
}

void line(float x1, float y1, float x2, float y2, float lineWidth) {
    glLineWidth(lineWidth);
    glBegin(GL_LINE_STRIP);
    glVertex2d(x1, y1);
    glVertex2d(x2, y2);
    glEnd();
}

void hollowSquare(float x, float y, float size) {
    glBegin(GL_LINES);
    glVertex2d(x, y);
    glVertex2d(x, y + size);
    glVertex2d(x, y + size);
    glVertex2d(x + size, y + size);
    glVertex2d(x + size, y + size);
    glVertex2d(x + size, y);
    glVertex2d(x + size, y);
    glVertex2d(x, y);
    glEnd();
}

void drawGrid(int windowLen, int windowHeight, int rows, int columns,float lineWidth) {
    float rowLen = windowHeight / rows;
    float columnLen = windowLen / columns;
    
    // drawing rows
    float y = 0;
    for (int counter = 1; counter <= rows; counter++) {
        line(0, y, windowLen, y, lineWidth);
        y += columnLen;
    }

    // drawing columns
    float x = 0;
    for (int counter = 1; counter <= columns; counter++) {
        line(x, 0, x, windowHeight, lineWidth);
        x += rowLen;
    }
}

#endif /* SHAPES */