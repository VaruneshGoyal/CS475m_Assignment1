#ifndef _MYDRAW_CLASS_HPP_
#define _MYDRAW_CLASS_HPP_

#define _WIDTH_ 512
#define _HEIGHT_ 512

#include <GL/glut.h>
#include <queue>
#include <string>
#include <fstream>

//Define all classes like the color class, adding appropriate methods and data members. 
//Implementation of the methods go into the corresponding cpp file

//------------------------
//color_t class

class color_t {
private:
  float r,g,b;
public:
  color_t();
  color_t(const float _r, const float _g, const float _b);

  void set(const float _r, const float _g, const float _b);
  float R(void);
  float G(void);
  float B(void);
};

//------------------------

//point_t class
class point_t {
private: 
	int x,y;
public:
	point_t();
	point_t(int x1, int y1);
	int X(void);
	int Y(void);
	void set(int _x, int _y);
	
	void draw_pt();

};
//------------------------

//line_t class
class line_t {
private: 
	point_t p1,p2;
public: 
	line_t();
	line_t(point_t _p1, point_t _p2);
	point_t P1(void);
	point_t P2(void);

	void draw_line();
};
//------------------------

//pen_t class
class pen_t {
private:
	color_t pen_col;
	int size;
	bool mode;			//mode if 1 indicates, we have to draw; if 0 indicates erase
public:
	pen_t();
	pen_t(int size1, bool mode1, color_t col1);
	color_t getColor();
	int getSize();
	void setColor(color_t x);
	void setSize(int x);
	void setMode(bool _mode);

};
//------------------------

//traingle_t class
class triangle_t {
private:
	point_t p1, p2, p3;
public:
	triangle_t();
	triangle_t(point_t _p1, point_t _p2, point_t _p3);
	point_t P1(void);
	point_t P2(void);
	point_t P3(void);

	void draw_triangle();
};
//------------------------

//fill_t class
class fill_t {
private:
	color_t fillColor;
public:
	fill_t();
//	fill_t(color_t color);
	void setColor(color_t color);
	color_t getColor();
	void floodFill(point_t beginPoint);		//the color with which to fill is fillColor
	color_t color(void);
};
//------------------------

//drawing_t class
class drawing_t {
// private:
	// std::queue<std::string> history;
public:
	std::vector<std::string> history;
	drawing_t();
	void save(std::string);
	void load(std::string);

};
//------------------------

//canvas_t class
class canvas_t {
private:
	//const int w;// = 500;
	//const int h;// = 500;
	color_t bg_color;
public:
	canvas_t();
	canvas_t(color_t background);
	GLubyte pixels[_HEIGHT_][_WIDTH_][3];
	drawing_t current_drawing;
	color_t getColor(int x, int y);
	void clear();
	void setBackgroundColor(color_t);

};
//------------------------

//GLOBAL VARIABLES
extern pen_t currentPen;
extern canvas_t currentCanvas;
extern fill_t currentFill;

#endif