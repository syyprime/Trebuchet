#include "openglwidget.h"
#include "mainwindow.h"
#include <QWidget>
#include <QMouseEvent>
#include <QtOpenGL>
#include <math.h>
#include <GL/glu.h>

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    timerFlag = false;
    leftFlag = false;
    rightFlag = false;
    upFlag = false;
    downFlag = false;
    stopFlag=false;

    speed=0;
    tx=0;
    ty=0;
    pv=0;
    score=0;
    angle1=100;
    angle2=-100;
    angle3=0;

    t=0;
    px=0;
    py=0;

}


OpenGLWidget::~OpenGLWidget()
{

}


QSize OpenGLWidget::minimumSizeHint() const
{
    return QSize(220, 220);
}


QSize OpenGLWidget::sizeHint() const
{
    return QSize(500, 500);
}


void OpenGLWidget::initializeGL()
{
    qglClearColor(Qt::white);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat light_position[] = { 2.0, 0.0, 2.0,0.0 };
     glLightfv(GL_LIGHT0, GL_POSITION,light_position);
    GLfloat light_diffuse [] = { 1.0, 1.0, 1.0,1.0 };
    glLightfv(GL_LIGHT0,  GL_DIFFUSE,light_diffuse);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_LIGHT0, GL_DIFFUSE);

    glShadeModel(GL_SMOOTH);
glBlendFunc(GL_SRC_ALPHA, GL_ONE);



    //圆柱模型
    glNewList(1,GL_COMPILE);
    glPushMatrix();
    GLUquadricObj *st;
    st=gluNewQuadric();
    gluQuadricDrawStyle(st, GLU_FILL);
    mySolidCylinder(st,0.05,0.05,0.1);
    glPopMatrix();
    glEndList();
    //支架腿
    glNewList(2,GL_COMPILE);
    glPushMatrix();
    glRotatef(30,0,1,0);
    glScalef(1,1,15);
    glCallList(1);
    glPopMatrix();
    glEndList();

    glNewList(3,GL_COMPILE);
    glPushMatrix();
    glRotatef(30,0,-1,0);
    glScalef(1,1,15);
    glCallList(1);
    glPopMatrix();
    glEndList();

    glNewList(4,GL_COMPILE);
    glPushMatrix();
    glRotatef(90,0,1,0);
    glScalef(1,1,30);
    glCallList(1);
    glPopMatrix();
    glEndList();


    //支架
    glNewList(5,GL_COMPILE);
    glPushMatrix();
    glColor3f(0.96,0.77,0.05);
    glTranslatef(0,-0.3,0);
    glCallList(2);
    glTranslatef(0,0.6,0);
    glCallList(2);
    glTranslatef(0.1,0,0);
    glCallList(3);
    glTranslatef(0,-0.6,0);
    glCallList(3);
    glColor3f(1,1,1);
    glTranslatef(0,0.3,1.3);
    glScalef(17,17,1);
    glCallList(1);
//    glTranslatef(0,0.6,0);
//    glCallList(4);
    glPopMatrix();
    glEndList();

    glNewList(6,GL_COMPILE);
    glPushMatrix();
    glColor3f(0.96,0.77,0.05);
    glCallList(5);
    glRotatef(90,0,0,1);
    glTranslatef(-0.3,-0.03,0.1);
    glColor3f(0.96,0.77,0.05);
    glScalef(0.2,1,1);
    glCallList(4);
    glTranslatef(0,0.6,1.15);
    glCallList(4);
    glTranslatef(0,-1.3,0);
    glCallList(4);
    glPopMatrix();
    glEndList();

    //杆
    glNewList(7,GL_COMPILE);
    glPushMatrix();
    glColor3f(0.76,0.44,0.05);
    glScalef(0.8,0.8,25);
    glTranslatef(0,0,-0.07);
    glCallList(1);
    glPopMatrix();
//    glPushMatrix();
//    drawSphere(0.1,32,32);
//    glPopMatrix();
    glEndList();

    //绳
    glNewList(8,GL_COMPILE);
    glPushMatrix();
    glColor3f(0.54,0.24,0.05);
    glScalef(0.5,0.5,6);
    glCallList(1);
    glPopMatrix();
    glEndList();

    //抛前球

    glNewList(9,GL_COMPILE);
    glPushMatrix();
    glColor3f(1,0,0);
    glTranslatef(0,0,0.7);
    drawSphere(0.1,32,32);
    glPopMatrix();
    glEndList();

    //抛物球

    glNewList(10,GL_COMPILE);
    glPushMatrix();
    glTranslatef(-0.63,0,-2.36);
    drawSphere(0.1,32,32);
    glPopMatrix();
    glEndList();

    //靶子
    glNewList(11,GL_COMPILE);
    glPushMatrix();
    glColor3f(1,1,1);
    DrawCircleArea(0,0,0.999,1,32);
    glColor3f(0,0,0);
    DrawCircleArea(0,0,0.998,0.8,32);
    glColor3f(0,0,1);
    DrawCircleArea(0,0,0.997,0.6,32);
    glColor3f(1,0,0);
    DrawCircleArea(0,0,0.996,0.4,32);
    glColor3f(1,1,0);
    DrawCircleArea(0,0,0.995,0.2,32);
    glPopMatrix();
    glEndList();

    //围栏
    glNewList(12,GL_COMPILE);
    glPushMatrix();
    glColor3f(0,0,0);
    glScalef(3,3,30);
    glCallList(1);
    glPopMatrix();
    glEndList();

    glNewList(13,GL_COMPILE);
    glPushMatrix();
    glTranslatef(0,-12.5,-2);
    glCallList(12);
    glTranslatef(12.5,0,0);
    glCallList(12);
    glTranslatef(0,12.5,0);
    glCallList(12);
    glTranslatef(0,12.5,0);
    glCallList(12);
    glTranslatef(-12.5,0,0);
    glCallList(12);
    glPopMatrix();
    glEndList();

    //广告牌
    glNewList(14,GL_COMPILE);
    glColor3f(1,1,1);
    glEnable(GL_TEXTURE_2D);//启用纹理
    loadGLTextures();
    glBindTexture(GL_TEXTURE_2D, texture[0]);//绑定纹理目标
    glBegin(GL_QUADS);
        glTexCoord2f( 0.0, 0.0 ); glVertex3f( 0, 0.0,  1.0 );
        glTexCoord2f( 1.0, 0.0 ); glVertex3f( 0, 12.0,  1.0 );
        glTexCoord2f( 1.0, 1.0 ); glVertex3f( 0, 12.0,  -2.0 );
        glTexCoord2f( 0.0, 1.0 ); glVertex3f( 0, 0.0,  -2.0 );
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEndList();

    glNewList(15,GL_COMPILE);
    glTranslatef(12.5,0,0);
    glCallList(14);
    glTranslatef(0,-12.5,0);
    glCallList(14);
    glTranslatef(-12.5,12.5,0);
    glRotatef(90,0,0,1);
    glTranslatef(0,0,0);
    glTranslatef(12.5,-12.5,0);
    glCallList(14);
    glTranslatef(-12.5,12.5,0);
     glRotatef(-90,0,0,1);
     glRotatef(-90,0,0,1);
     glTranslatef(0,0,0);
     glTranslatef(12.5,0,0);
     glCallList(14);
     glTranslatef(-12.5,0,0);
      glRotatef(90,0,0,1);
    glEndList();

    //草


}
void OpenGLWidget::paintGL()
{


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(-90.0,1.0,0.0,0.0);
    glRotatef(-90, 0, 0.0, 1.0);

    if(px==0&&py==0){
    gluLookAt(2,0,0,  0,0,2,  0.0,1.0,0.0);}
    else{
        gluLookAt(px+3,py,-2.36+pv+0,  px,py,-2.36+pv+2,  0.0,1.0,0.0);
    }


    glDisable(GL_CULL_FACE);
//    glTranslatef(0.0, 0.0, 1.0);

    draw();
}


void OpenGLWidget::resizeGL(int width, int height)
{

    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    // setup
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, double(width)/double(height), 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if(px==0&&py==0){
    gluLookAt(2,0,0, 0,0,2,  0.0,1.0,0.0);}
    else{
        gluLookAt(px+3,py,-2.36+pv+0,  px,py,-2.36+pv+2,  0.0,1.0,0.0);
    }
    //gluLookAt(0.0,0.0,0.0,  0.0,0.0,0.0,  0.0,1.0,-1.0);

#ifdef QT_OPENGL_ES_1
    glOrthof(-2, +2, -2, +2, 1.0, 15.0);
#else
    glOrtho(-2, +2, -2, +2, 1.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);

}


void OpenGLWidget::mySolidCylinder( GLUquadric*   quad,
                     GLdouble base,
                     GLdouble top,
                     GLdouble height )
{

    GLint slices=32;
    GLint stacks =32;
    gluCylinder(quad, base, top, height, slices, stacks);
    //top
    DrawCircleArea(0.0, 0.0, height, top, slices);
    //base
    DrawCircleArea(0.0, 0.0, 0.0, base, slices);

}


GLvoid OpenGLWidget::DrawCircleArea(float cx, float cy, float cz, float r, int num_segments)
{
    GLfloat vertex[4];

    const GLfloat delta_angle = 2.0*M_PI/num_segments;
    glBegin(GL_TRIANGLE_FAN);

    vertex[0] = cx;
    vertex[1] = cy;
    vertex[2] = cz;
    vertex[3] = 1.0;
    glVertex4fv(vertex);

    //draw the vertex on the contour of the circle
    for(int i = 0; i < num_segments ; i++)
    {
        vertex[0] = cos(delta_angle*i) * r + cx;
        vertex[1] = sin(delta_angle*i) * r + cy;
        vertex[2] = cz;
        vertex[3] = 1.0;
        glVertex4fv(vertex);
    }

    vertex[0] = 1.0 * r + cx;
    vertex[1] = 0.0 * r + cy;
    vertex[2] = cz;
    vertex[3] = 1.0;
    glVertex4fv(vertex);
    glEnd();

}


void OpenGLWidget::drawSphere( GLfloat radius, GLfloat M, GLfloat N)
{

 float step_z = M_PI/M;
 float step_xy = 2*M_PI/N;
 float x[4],y[4],z[4];

 float angle_z = 0.0;
 float angle_xy = 0.0;
 int i=0, j=0;
 glBegin(GL_QUADS);
  for(i=0; i<M; i++)
  {
   angle_z = i * step_z;

   for(j=0; j<N; j++)
   {
    angle_xy = j * step_xy;

    x[0] = radius * sin(angle_z) * cos(angle_xy);
    y[0] = radius * sin(angle_z) * sin(angle_xy);
    z[0] = radius * cos(angle_z);

    x[1] = radius * sin(angle_z + step_z) * cos(angle_xy);
    y[1] = radius * sin(angle_z + step_z) * sin(angle_xy);
    z[1] = radius * cos(angle_z + step_z);

    x[2] = radius*sin(angle_z + step_z)*cos(angle_xy + step_xy);
    y[2] = radius*sin(angle_z + step_z)*sin(angle_xy + step_xy);
    z[2] = radius*cos(angle_z + step_z);

    x[3] = radius * sin(angle_z) * cos(angle_xy + step_xy);
    y[3] = radius * sin(angle_z) * sin(angle_xy + step_xy);
    z[3] = radius * cos(angle_z);

    for(int k=0; k<4; k++)
    {
     glVertex3f(x[k], y[k],z[k]);
    }
   }
  }
 glEnd();

}


void OpenGLWidget::redisplay()
{
    //杆转动幅度
    angle1+=-5;
    //绳转动幅度
    if(angle2>-180){
        angle2-=10;}
    //联动
    glClear(GL_COLOR_BUFFER_BIT);
    updateGL();
    if(angle1<-110)
        testTimer.stop();
}


void OpenGLWidget::rotate(){
        if(!stopFlag){
        if(leftFlag&&angle3<90){
            angle3+=5;
            leftFlag=false;
        }
        if(rightFlag&&angle3>-90){
            angle3-=5;
            rightFlag=false;
        }
        if(upFlag&&(angle1<110)){
            angle1+=3;
            if(angle1>97){
                angle2=acos((1-2*sin((angle1-90)*3.14/180))/0.9)*180/3.14-angle1;
            }else{
                angle2=-angle1;
            }

            upFlag=false;
        }
        if(downFlag&&(angle1>15)){
            angle1+=-3;
            if(angle1>97){
            angle2=acos((1-2*sin((angle1-90)*3.14/180))/0.9)*180/3.14-angle1;
            }
            else{
                angle2=-angle1;
            }
            downFlag=false;
        }}
        //联动
        glClear(GL_COLOR_BUFFER_BIT);
        updateGL();

}

void OpenGLWidget::setUpCible(){
    glClear(GL_COLOR_BUFFER_BIT);
    updateGL();
}

void OpenGLWidget::draw()
{
// grass
    glColor3f(1,1,1);
    glEnable(GL_TEXTURE_2D);//启用纹理
    loadGLTexturesGrass();
    glBindTexture(GL_TEXTURE_2D, texture[0]);//绑定纹理目标
    glBegin(GL_QUADS);
        glTexCoord2f( 0.0, 0.0 ); glVertex3f( 13, 13,  1.0 );
        glTexCoord2f( 1.0, 0.0 ); glVertex3f( 13, -13,  1.0 );
        glTexCoord2f( 1.0, 1.0 ); glVertex3f( -3, -13,  1 );
        glTexCoord2f( 0.0, 1.0 ); glVertex3f( -3, 13,  1 );
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glCallList(15);
//    glColor3f(0,1,0.5);
//    glBegin(GL_POLYGON);
//    glVertex3f(13,13,1);
//    glVertex3f(13,-13,1);
//    glVertex3f(-3,-13,1);
//    glVertex3f(-3,13,1);
//    glEnd();

//    for(int i=0;i<4;i++){
//        glColor3f(0,1,0);
//        glBegin(GL_POLYGON);
//        glVertex3f(13,12-i*8,0.9999);
//        glVertex3f(13,8-i*8,0.9999);
//        glVertex3f(-3,8-i*8,0.9999);
//        glVertex3f(-3,12-i*8,0.9999);
//        glEnd();
//    }

    glPushMatrix();
    glCallList(13);
    glPopMatrix();



    //ball movement
    if(tx!=0&&ty!=0)
    {
        glTranslatef(-tx,ty,0);
        glCallList(11);
        glTranslatef(tx,-ty,0);
    }

    if(angle1<15)
    {
        if(-2.36+pv<0.8)
        {
            ph=4*t*speed;
            pv=-t*speed+5*t*t;
        }
        else{
            score=10-sqrt((tx-px)*(tx-px)+(ty-py)*(ty-py))*10;
            if(score<0)
                score=0;
            emit gameOver();
        }

        glColor3f(1,0,0);
        glRotatef(angle3,0,0,1);
        glTranslatef(ph,0,pv);
        glPushMatrix();
        glCallList(10);
        glPopMatrix();
        glTranslatef(-ph,0,-pv);
        glRotatef(angle3,0,0,-1);

        t+=0.038;
        px=(-0.63+ph)*cos(angle3*3.14/180);
        py=(-0.63+ph)*sin(angle3*3.14/180);
    }

    glPushMatrix();
    glScalef(0.7,0.7,0.7);
    glTranslatef(-0.05,0,-0.1);
    glRotatef(angle3,0,0,1);
    glCallList(6);
    glPopMatrix();
    glRotatef(angle3,0,0,1);
    glRotatef(angle1,0,1,0);
    glPushMatrix();
    glCallList(7);
    glPopMatrix();

    glTranslatef(0,0,-1.75);
    glRotatef(-(angle2),0,-1,0);
    glPushMatrix();
    glCallList(8);
    glPopMatrix();
    if(angle1>=15){
        glPushMatrix();
        glCallList(9);
        glPopMatrix();}


}


void OpenGLWidget::throwout(){

    QObject::connect( &testTimer,SIGNAL(timeout()), this, SLOT(redisplay()) );

    if(!timerFlag)
    {
        testTimer.start(100);

    }

    speed=sqrt(20*(1-cos(angle1*3.14/180)))/2;
    timerFlag = true;

}

void OpenGLWidget::reset(){

    timerFlag = false;
    leftFlag = false;
    rightFlag = false;
    upFlag = false;
    downFlag = false;
    stopFlag=false;

    speed=0;
    tx=0;
    ty=0;
    score=0;
    px=0;
    py=0;
    pv=0;

    angle1=100;
    angle2=-100;
    angle3=0;
    t=0;

}


void OpenGLWidget::loadGLTextures()
{

    QImage tex, buf;
        if(!buf.load(":/logo/telecom_logo1.png")) //":/logo/logo_tse.png"
        {
            qWarning("Cannot open the image...");
            QImage dummy(128, 128, QImage::Format_RGB32);//当没找到所需打开的图片时，创建一副128*128大小，深度为32位的位图
            dummy.fill(Qt::green);
            buf = dummy;
        }
        tex = convertToGLFormat(buf);//将Qt图片的格式buf转换成opengl的图片格式tex
        glGenTextures(1, &texture[0]);//开辟3个纹理内存，索引指向texture[0]

        /*建立第一个纹理*/
        glBindTexture(GL_TEXTURE_2D, texture[0]);//将创建的纹理内存指向的内容绑定到纹理对象GL_TEXTURE_2D上，经过这句代码后，以后对
                                                //GL_TEXTURE_2D的操作的任何操作都同时对应与它所绑定的纹理对象
        glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());//开始真正创建纹理数据
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//当所显示的纹理比加载进来的纹理大时，采用GL_NEAREST的方法来处理


}

void OpenGLWidget::loadGLTexturesGrass()
{

    QImage tex, buf;
        if(!buf.load(":/logo/glass.jpg"))
        {
            qWarning("Cannot open the image...");
            QImage dummy(128, 128, QImage::Format_RGB32);//当没找到所需打开的图片时，创建一副128*128大小，深度为32位的位图
            dummy.fill(Qt::green);
            buf = dummy;
        }
        tex = convertToGLFormat(buf);//将Qt图片的格式buf转换成opengl的图片格式tex
        glGenTextures(1, &texture[0]);//开辟3个纹理内存，索引指向texture[0]

        /*建立第一个纹理*/
        glBindTexture(GL_TEXTURE_2D, texture[0]);//将创建的纹理内存指向的内容绑定到纹理对象GL_TEXTURE_2D上，经过这句代码后，以后对
                                                //GL_TEXTURE_2D的操作的任何操作都同时对应与它所绑定的纹理对象
        glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());//开始真正创建纹理数据
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//当所显示的纹理比加载进来的纹理大时，采用GL_NEAREST的方法来处理


}
