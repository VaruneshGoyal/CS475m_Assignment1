/******************************
 *
 * CS475m Assignment 1 Basecode 
 *
 * Parag Chaudhuri, 2016
 *
 *******************************/

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <GL/glut.h>

#include "mydraw_class.hpp"

//Window width
int win_width = 512;
//Window height
int win_height = 512;

//0 means line drawing mode
//1 means traingle drawing mode
//2 means fill color mode
//3 means mouse keep quiet
int whatToDo = 0;     

//to keep count of number of points selected in line or triangle drawing
int pointsSelected = 0;

point_t P1(0,0), P2(0,0), P3(0,0);

std::string yolo;

//Display callback
void display( void ){
  //This clears the colorbuffer (current window)
  glClear(GL_COLOR_BUFFER_BIT);

  glRasterPos2i(0,0);
  glDrawPixels(_WIDTH_, _HEIGHT_, GL_RGB, GL_UNSIGNED_BYTE, currentCanvas.pixels);

  //Flush the framebuffer to the screen
  glutSwapBuffers();
}

//Reshape callback
void reshape( int w, int h ){
  if  ( h == 0 ) h = 1;

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();

  glOrtho( 0.0, (GLdouble)w, 0.0, (GLdouble)h, -1., 1. );
  glViewport( 0, 0, w, h );

  win_width = w;
  win_height = h;

  glutPostRedisplay();
}

//Keyboard callback
void keyboard( unsigned char key, int x, int y ) {
  switch(key) {
  //Exit on pressing escape
  case 27:
    exit(0);
    break;

  case 'N' :
    //new canvas...didnt understand this much
    currentCanvas.setBackgroundColor(color_t(1.0,1.0,1.0));
    currentPen.setColor(color_t(0.0, 0.0, 0.0));
    currentPen.setSize(1);
    currentFill.setColor(color_t(0.0, 0.0, 0.0));
    currentCanvas.clear();
    glutPostRedisplay();
    std::cout<<"New canvas created.\n";

    currentCanvas.current_drawing.history.clear();
    break;

  case 'C' :
    //clear canvas to current background color
    currentCanvas.clear();
    glutPostRedisplay();
    std::cout<<"Canvas cleared.\n";
    yolo = "C";
    currentCanvas.current_drawing.history.push_back(yolo);
    break;

  case 'S' : 
    //save the array to file
    whatToDo = 3;
    std::cout<<"Enter file name to save into: ";
    std::cin >> yolo;
    std::cout<<"Saving...\n";
    
    currentCanvas.current_drawing.save(yolo);
    break;

  case 'L' :
    //load array from file
    whatToDo = 3;
    std::cout<<"Enter file name to load from: ";
    std::cin >> yolo;
    std::cout<<"Loading...\n";

    currentCanvas.setBackgroundColor(color_t(1.0,1.0,1.0));
    currentPen.setColor(color_t(0.0, 0.0, 0.0));
    currentPen.setSize(1);
    currentFill.setColor(color_t(0.0, 0.0, 0.0));
    currentCanvas.clear();

    currentCanvas.current_drawing.load(yolo);
    glutPostRedisplay();
    break;

  case '1' :
    //Line drawing mode
    whatToDo = 0;
    pointsSelected = 0;
    std::cout<<"Line mode\n";
    break;

  case '2' :
    //Triangle drawing mode
    whatToDo = 1;
    pointsSelected = 0;
    std::cout<<"Triangle mode\n";
    break;

  case 'F' :
    //Fill
    whatToDo = 2;
    std::cout<<"Fill mode (Current color: "<<currentFill.getColor().R()<<" "<<currentFill.getColor().G()
                    <<" "<<currentFill.getColor().B()<<")\n";
    break;

  case 'G' :
    //Change current fill color. Input color from terminal
    whatToDo = 3;
    std::cout<<"Enter new fill color (R G B): ";
    {
      float rc, gc, bc;
      std::cin>>rc>>gc>>bc;
      currentFill.setColor(color_t(rc,gc,bc));
      yolo = "G " + std::to_string(rc) + " " + std::to_string(gc)+ " " + std::to_string(bc);
      currentCanvas.current_drawing.history.push_back(yolo);
    }
    break;
  
  case 'H' :
    //Change current pen color. Input color from terminal
    whatToDo = 3;
    std::cout<<"Enter new pen color (R G B): ";
    {
      float rc, gc, bc;
      std::cin>>rc>>gc>>bc;
      currentPen.setColor(color_t(rc,gc,bc));
      yolo = "H " + std::to_string(rc) + " " + std::to_string(gc) + " " + std::to_string(bc);
      currentCanvas.current_drawing.history.push_back(yolo);
    }
    break;

  case 'I' :
    //Change current background color. Input color from terminal
    whatToDo = 3;
    std::cout<<"Enter new background color (R G B): ";
    {
      float rc, gc, bc;
      std::cin>>rc>>gc>>bc;
      currentCanvas.setBackgroundColor(color_t(rc,gc,bc));
      yolo = "I " + std::to_string(rc) + " " + std::to_string(gc) + " " + std::to_string(bc);
      currentCanvas.current_drawing.history.push_back(yolo);
    }
    break;

  case 'J' :
    //Change current pen width. Input width from terminal
    whatToDo = 3;
    std::cout<<"Enter new pen size: ";
    {
      int x;
      std::cin>>x;
      currentPen.setSize(x);
      yolo = "J " + std::to_string(x);
      currentCanvas.current_drawing.history.push_back(yolo);
    }
    break;

    case 'U' :
    whatToDo = 3;
    if(currentCanvas.current_drawing.history.empty()) break;
    currentCanvas.current_drawing.history.pop_back();
    currentCanvas.current_drawing.save("temp.txt");
    currentCanvas.setBackgroundColor(color_t(1.0,1.0,1.0));
    currentPen.setColor(color_t(0.0, 0.0, 0.0));
    currentPen.setSize(1);
    currentFill.setColor(color_t(0.0, 0.0, 0.0));
    currentCanvas.clear();
    currentCanvas.current_drawing.load("temp.txt");
    glutPostRedisplay();
    break;



    //Ignore all other keypresses
  default:
    break;
  }
}


//Mouse callback
void mouse(int button, int state, int x, int y) 
{
  if(x<0) x = 0;
  if(y<0) y = 0;
  if(x>_WIDTH_) x = _WIDTH_ - 1;
  if(y>_HEIGHT_) y = _HEIGHT_ - 1;
  
  if (state == GLUT_DOWN) 
  {
    if (button == GLUT_LEFT_BUTTON) 
    {
      //Do something when the left mouse button is clicked
      if(whatToDo == 0){
        if(pointsSelected == 0){
          P1.set(x,_HEIGHT_ - y);
          pointsSelected++;
          //std::cout<<x<<" "<<y<<"P1";
        }
        else if (pointsSelected == 1){
          P2.set(x,_HEIGHT_ - y);
          pointsSelected = 0;
          //std::cout<<x<<" "<<y<<"P2";
          line_t L = line_t(P1, P2);
          L.draw_line();
          
          yolo = "L " + std::to_string(P1.X()) + " " + std::to_string(P1.Y()) + 
                    " " + std::to_string(P2.X()) + " " + std::to_string(P2.Y());
          currentCanvas.current_drawing.history.push_back(yolo);
        }

      } else if(whatToDo == 1){
        if(pointsSelected == 0){
          P1.set(x,_HEIGHT_ - y);
          pointsSelected++;
          //std::cout<<x<<" "<<y<<"P1";
        }
        else if (pointsSelected ==1){
          P2.set(x,_HEIGHT_ - y);
          pointsSelected++;
          //std::cout<<x<<" "<<y<<"P2";        
        }
        else if (pointsSelected == 2){
          P3.set(x,_HEIGHT_ - y);
          pointsSelected = 0;
          //std::cout<<x<<" "<<y<<"P3";
          triangle_t T = triangle_t(P1, P2, P3);
          T.draw_triangle();

          yolo = "T " + std::to_string(P1.X()) + " " + std::to_string(P1.Y())
                  + " " + std::to_string(P2.X()) + " " + std::to_string(P2.Y())
                  + " " + std::to_string(P3.X()) + " " + std::to_string(P3.Y());
          currentCanvas.current_drawing.history.push_back(yolo);
        }

      } else if(whatToDo == 2){
        P1.set(x,_HEIGHT_ - y);
        currentFill.floodFill(P1);

        yolo = "F " + std::to_string(P1.X()) + " " + std::to_string(P1.Y());
        currentCanvas.current_drawing.history.push_back(yolo);
      }
	       
    }

    if (button == GLUT_RIGHT_BUTTON) 
    {
      //Do something when the left mouse button is clicked
      if(whatToDo == 0){
        if(pointsSelected == 1){
          pointsSelected--;
        }
      }
      else if(whatToDo == 1){
        if(pointsSelected == 1 || pointsSelected == 2){
          pointsSelected--;
        }
      }
         
    }
  }
   glutPostRedisplay();
}       


int main (int argc, char *argv[]) 
{
  std::cout<<"******************************\n  Welcome to mydraw program!  \n******************************\n";
  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
  glutInitWindowSize( win_width, win_height );

  //Open a GLUT window
  glutCreateWindow( "MyDraw" );

  glutDisplayFunc( display );
  glutReshapeFunc( reshape );
  glutKeyboardFunc( keyboard );
  glutMouseFunc( mouse );

  glutMainLoop();
}