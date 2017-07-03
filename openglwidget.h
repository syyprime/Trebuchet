#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H


#include <QWidget>
#include <QGLWidget>
#include <QTimer>
#include <GL/glu.h>
#include <QWidget>
#include <QtOpenGL>
#include <QGLFunctions>
#include <QGLShaderProgram>
#include <QWidget>
#include <QtOpenGL>
//#include <GLU.h>
//#include <gl.h>
#include<cmath>
#include<QTime>

class OpenGLWidget: public QGLWidget
{
    Q_OBJECT


 public:
     explicit OpenGLWidget(QWidget *parent = 0);
     ~OpenGLWidget();
     void reset();


     bool timerFlag;
     bool leftFlag;
     bool rightFlag;
     bool upFlag;
     bool downFlag;
     bool stopFlag;

     double speed;
     double tx;
     double ty;
     double pv;
     int score;


 protected:
     void initializeGL();
     void paintGL();
     void resizeGL(int width, int height);

     QSize minimumSizeHint() const;
     QSize sizeHint() const;

     void mySolidCylinder( GLUquadric*   quad,
                          GLdouble base,
                          GLdouble top,
                          GLdouble height);
     GLvoid DrawCircleArea(float cx, float cy, float cz, float r, int num_segments);
     void drawSphere(GLfloat radius, GLfloat M, GLfloat N);

     void loadGLTextures ();
     void loadGLTexturesGrass();
     GLuint texture[1];


 public slots:
     void redisplay();
     void rotate();
     void throwout();
      void setUpCible();


 signals:
    void gameOver();


 private:
     void draw();

     double angle1;
     double angle2;
     double angle3;
     double ph;

     double t;
     QTimer testTimer;
     double px;
     double py;

};

#endif // OPENGLWIDGET_H
