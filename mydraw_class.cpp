#include "mydraw_class.hpp"
#include <iostream>
#include <cmath>
#include <stack>
#include <queue>
#include <GL/glut.h>

pen_t currentPen;
canvas_t currentCanvas;
fill_t currentFill;

//color_t methods

color_t::color_t():r(0.0),g(0.0),b(0.0) { }

color_t::color_t(const float _r, const float _g, const float _b)
		 :r(_r),g(_g),b(_b) { }

void color_t::set(const float _r, const float _g, const float _b)
{ 
  r=_r; g=_g; b=_b;
}

float color_t::R(void) { return r; }
float color_t::G(void) { return g; }
float color_t::B(void) { return b; }


//---------------------
//pen_t methods

pen_t::pen_t(){
		size = 1;		//default size
		pen_col = color_t(0.0, 0.0, 0.0);		//drawing with black on white background
		mode = 1;		//1=drawing mode
	}

pen_t::pen_t(int size1, bool mode1, color_t col1){
		size = size1;
		mode = mode1;
		pen_col = col1;
	}

color_t pen_t::getColor(){
	return pen_col;
}

int pen_t::getSize(){
	return size;
}

void pen_t::setColor(color_t x){
	pen_col = x;
}

void pen_t::setSize(int x){
	size = x;
}

void pen_t::setMode(bool _mode){
	mode = _mode;
}

//---------------------

//point_t methods
point_t::point_t(){
	;
}

point_t::point_t(int x1,int y1){
	x = x1;
	y = y1;
}

int point_t::X(void){
	return this->x;
}

int point_t::Y(void){
	return this->y;
}

void point_t::set(int _x, int _y){
	x = _x;
	y = _y;
}

void point_t::draw_pt(){
	//plots (x,y) on canvas
	//change appropriate pixel's color
	//std::cout<<"Point:"<<x<<","<<y;
	//std::cout<<" Color:"<<currentPen.getColor().R()<<","<<currentPen.getColor().G()<<","<<currentPen.getColor().B()<<std::endl;
	for (int j = x; j < std::min(_WIDTH_,x+currentPen.getSize()); j++) {
       	for (int i = y; i < std::min(_HEIGHT_,y+currentPen.getSize()); i++) {
        //color_t* color = image[i][j]->get_color();
       	//std::cout<<i<<" "<<j<<std::endl;
        currentCanvas.pixels[i][j][0] = (GLubyte) (int) 255*currentPen.getColor().R();
        currentCanvas.pixels[i][j][1] = (GLubyte) (int) 255*currentPen.getColor().G();
        currentCanvas.pixels[i][j][2] = (GLubyte) (int) 255*currentPen.getColor().B();
    	}
    }
}

//---------------------

//canvas_t methods
canvas_t::canvas_t(){
	bg_color.set(1.0, 1.0, 1.0);
	//std::cout<<bg_color.R()<<","<<bg_color.G()<<","<<bg_color.B()<<"\n";
	clear();
}

canvas_t::canvas_t(color_t background){
	bg_color = background;
}

void canvas_t::setBackgroundColor(color_t t){
	bg_color = t;
}

color_t canvas_t::getColor(int x, int y){
	return color_t(pixels[y][x][0]/255.0, pixels[y][x][1]/255.0, pixels[y][x][2]/255.0);
}

void canvas_t::clear(){
	//std::cout<<"Clearing... ";
	for(int i=0;i<_WIDTH_;i++){
		for(int j=0;j<_HEIGHT_;j++){
			pixels[i][j][0] = (GLubyte) (int) 255*bg_color.R();
			pixels[i][j][1] = (GLubyte) (int) 255*bg_color.G();
			pixels[i][j][2] = (GLubyte) (int) 255*bg_color.B();
		}
	}
}

//---------------------

//line_t methods

	
line_t::line_t(point_t p1a, point_t p2a){
	p1 = p1a;
	p2 = p2a;
}

point_t line_t::P1(void){
	return p1;
}

point_t line_t::P2(void){
	return p2;
}

//Pseudo code taken from Computer Graphics : A programming Approach (2nd Ed) by Steven Harrington; Pgs 18-20  
//This version uses only integer calculations
//Another benefit is that it has totally avoided any multiplication or division 
void line_t::draw_line(void){
	//implement bresenhems here

	int dx = p2.X() - p1.X();
	int dy = p2.Y() - p1.Y();
	int absdx = std::abs(dx);
	int absdy = std::abs(dy);

	bool isSlopePositive = (dx > 0);
	isSlopePositive ^= (dy < 0);				//if dy<0,  isSlopePositive = !isSlopePositive
	//determining the slope's sign is needed to know whether we have to increment or decrement y as x increases.

	int x1, x2, y1, y2;

	point_t PointToDraw(0,0);

	//now if |slope| <45 then step along x-axis
	if(absdx > absdy){
		if (dx > 0){
			x1 = p1.X();
			y1 = p1.Y();
			x2 = p2.X();
		} else {
			x1 = p2.X();
			y1 = p2.Y();
			x2 = p1.X();
		}

		//G is kinda representation of the error function
		//H > 0.5 => increment y
		//2H - 1 > 0 => increment y
		//but H initially = slope = dy/dx
		//2dy - dx > 0 => increment y        ... call 2dy - dx = G
		//if increment y; add (slope - 1) to H i.e. add 2*(dy - dx) to G...call this inc1
		//else add slope i.e. 2*(dy) to G...call this inc0
		int G = 2*absdy - absdx;
		int inc1 = 2*(absdy - absdx);
		int inc0 = 2*absdy;

		//if isSlopePositive, increment y on crossing error margin; if !isSlopePositive, decrement y
		if(isSlopePositive){
			for(int i=x1; i<=x2; i++){
				PointToDraw.set(i, y1);
				PointToDraw.draw_pt(); // draw_pt(i,y1);
				if(G > 0){
					y1++;
					G += inc1;
				} else {
					G += inc0;
				}
			}
		} else {
			for(int i=x1; i<=x2; i++){
				PointToDraw.set(i, y1);
				PointToDraw.draw_pt(); // draw_pt(i,y1);
				if(G > 0){
					y1--;
					G += inc1;
				} else {
					G += inc0;
				}
			}			
		}


	} else {			//|slope| >45 so swap x and y and then same algo as above

		if (dy > 0){
			y1 = p1.Y();
			x1 = p1.X();
			y2 = p2.Y();
		} else {
			y1 = p2.Y();
			x1 = p2.X();
			y2 = p1.Y();
		}

		int G = 2*absdx - absdy;
		int inc1 = 2*(absdx - absdy);
		int inc0 = 2*absdx;

		//if isSlopePositive, increment y on crossing error margin; if !isSlopePositive, decrement y
		if(isSlopePositive){
			for(int i=y1; i<=y2; i++){
				PointToDraw.set(x1,i);
				PointToDraw.draw_pt();			//draw_pt(x1,i);
				if(G > 0){
					x1++;
					G += inc1;
				} else {
					G += inc0;
				}
			}
		} else {
			for(int i=y1; i<=y2; i++){
				PointToDraw.set(x1,i);
				PointToDraw.draw_pt();			//draw_pt(x1,i);
				if(G > 0){
					x1--;
					G += inc1;
				} else {
					G += inc0;
				}
			}			
		}
	}
}

//---------------------

//triangle_t methods
triangle_t::triangle_t(point_t _p1, point_t _p2, point_t _p3){
	p1 = _p1;
	p2 = _p2;
	p3 = _p3;
}

point_t triangle_t::P1(void){
	return p1;
}

point_t triangle_t::P2(void){
	return p2;
}

point_t triangle_t::P3(void){
	return p3;
}

void triangle_t::draw_triangle(){
	//first create 3 lines using the points. Then draw_line on each of them
	line_t l1 = line_t(p1, p2);
	line_t l2 = line_t(p2, p3);
	line_t l3 = line_t(p3, p1);
	l1.draw_line();
	l2.draw_line();
	l3.draw_line();
}

//---------------------

//fill_t methods
fill_t::fill_t(){
	color_t color(0, 0 ,0);
	fillColor = color;
}

void fill_t::setColor(color_t color){
	fillColor = color;
}

color_t fill_t::getColor(){
	return fillColor;
}

color_t fill_t::color(void){
	return fillColor;
}

void fill_t::floodFill(point_t beginPoint){
	//4 side floodfill algorithm
	/*getColor function acting on the 2D matrix takes the integer values for R, G and B as stored in the matrix
	 *and divides by 255 to give corresponding float value. This is not accurate and the strict equality used in the loop
	 *following the stack might give output as false (i.e. say that the two are unequal). However as far as this code goes, 
	 *this won't affect as both: color of beginPoint and of each point checked is obtained in same fashion.*/
	int xx, yy;

	/*If the color of the selected already matches that of fillColor then we stop here itself*/
	/*Here we have taken the typical method used for comparing floats i.e. checking if their difference is smaller 
	  than some permissible error margin. This is required as one of the float values has been regenerated using
	  canvas_t.getColor() but other is the original float value. Choice of 0.004 as the epsilon suits our purpose
	  as colors that are actually not equal will report higher difference.*/
	color_t thisColor = currentCanvas.getColor(beginPoint.X(), beginPoint.Y());
	if((0.004 < abs(thisColor.R() - fillColor.R())) && (0.004 < abs(thisColor.G() - fillColor.G())) 
			&& (0.004 < (thisColor.B() == fillColor.B()))) 		return;

	//a stack is used
	std::stack<point_t> fillStack;
	fillStack.push(beginPoint);

	while(!fillStack.empty()){
		point_t ye = fillStack.top();
		fillStack.pop();
		xx = ye.X(); yy = ye.Y();
		if(currentCanvas.getColor(xx, yy).R() == thisColor.R() &&
				currentCanvas.getColor(xx, yy).G() == thisColor.G() &&
					currentCanvas.getColor(xx, yy).B() == thisColor.B())
		{
			currentCanvas.pixels[yy][xx][0] = (GLubyte) (int) 255*fillColor.R();
        	currentCanvas.pixels[yy][xx][1] = (GLubyte) (int) 255*fillColor.G();
        	currentCanvas.pixels[yy][xx][2] = (GLubyte) (int) 255*fillColor.B();

        	//Here we aren't checking if these new points being pushed into the stack are already filled or not.
        	//In general this could lead to an unending recursion but this is avoided as we have checked that the 
        	//fillColor isnt equal to the color of the region to be filled.
        	if(xx+1<_WIDTH_) 	fillStack.push(point_t(xx+1,yy));
        	if(xx-1>=0) 		fillStack.push(point_t(xx-1,yy));
        	if(yy+1<_HEIGHT_)	fillStack.push(point_t(xx,yy+1));
        	if(yy-1>=0)			fillStack.push(point_t(xx,yy-1));

		}
	
	}
}

//---------------------

//drawing_t methods
drawing_t::drawing_t(){
	;
}

void drawing_t::save(std::string yolo){
	std::ofstream outfile(yolo);
	std::vector<std::string>::iterator it = history.begin();
	while(it!=history.end()){
		outfile<<*it<<std::endl;
		it++;
	}
	std::cout<<"SAVED!"<<std::endl;
	outfile.close();
}

void drawing_t::load(std::string yolo){
	std::ifstream infile(yolo);
	history.clear();
	char x;
	float rc, gc, bc;
	int size;
	int x1, y1;
	point_t P1, P2, P3;
	//line_t L1;
	//triangle_t T1;

	while(infile>>x){
		//std::cout<<x<<std::endl;
		if(x=='G'){
			//float rc, gc, bc;
      		infile>>rc>>gc>>bc;
      		currentFill.setColor(color_t(rc,gc,bc));
      		yolo = "G " + std::to_string(rc) + " " + std::to_string(gc)+ " " + std::to_string(bc);
      		currentCanvas.current_drawing.history.push_back(yolo);
		} 

		else if(x=='H'){
			//float rc, gc, bc;
    	  	infile>>rc>>gc>>bc;
      		currentPen.setColor(color_t(rc,gc,bc));
      		yolo = "H " + std::to_string(rc) + " " + std::to_string(gc) + " " + std::to_string(bc);
      		currentCanvas.current_drawing.history.push_back(yolo);
		}

		else if(x=='I'){
			//float rc, gc, bc;
      		infile>>rc>>gc>>bc;
      		currentCanvas.setBackgroundColor(color_t(rc,gc,bc));
      		yolo = "I " + std::to_string(rc) + " " + std::to_string(gc) + " " + std::to_string(bc);
      		currentCanvas.current_drawing.history.push_back(yolo);
		}

		else if(x=='J'){
			//int size;
      		infile>>size;
      		currentPen.setSize(size);
      		yolo = "J " + std::to_string(size);
      		currentCanvas.current_drawing.history.push_back(yolo);
		}

		else if(x=='C'){
    		currentCanvas.clear();
    		yolo = "C";
    		currentCanvas.current_drawing.history.push_back(yolo);
		}

		/*else if(x=='N'){
			currentCanvas.setBackgroundColor(color_t(1.0,1.0,1.0));
    		currentCanvas.clear();
    	    yolo = "N";
    		currentCanvas.current_drawing.history.push_back(yolo);
		}*/

		else if(x=='L'){
			infile>>x1>>y1;
			P1.set(x1,y1);
			infile>>x1>>y1;
			P2.set(x1,y1);
			line_t L1 = line_t(P1,P2);
			L1.draw_line();
			yolo = "L " + std::to_string(P1.X()) + " " + std::to_string(P1.Y()) + 
                    " " + std::to_string(P2.X()) + " " + std::to_string(P2.Y());
          	currentCanvas.current_drawing.history.push_back(yolo);
		}

		else if(x=='T'){
			infile>>x1>>y1;
			P1.set(x1,y1);
			infile>>x1>>y1;
			P2.set(x1,y1);
			infile>>x1>>y1;
			P3.set(x1,y1);
			triangle_t T1 = triangle_t(P1, P2, P3);
			T1.draw_triangle();
			yolo = "T " + std::to_string(P1.X()) + " " + std::to_string(P1.Y())
                  + " " + std::to_string(P2.X()) + " " + std::to_string(P2.Y())
                  + " " + std::to_string(P3.X()) + " " + std::to_string(P3.Y());
          	currentCanvas.current_drawing.history.push_back(yolo);
		}

		else if(x=='F'){
			infile>>x1>>y1;
			P1.set(x1,y1);
			currentFill.floodFill(P1);

        	yolo = "F " + std::to_string(P1.X()) + " " + std::to_string(P1.Y());
        	currentCanvas.current_drawing.history.push_back(yolo);
		}

		else{
			std::cout<<"ERROR READING FILE"<<std::endl;
			break;
		}
	}
	
	infile.close();
	std::cout<<"LOADED!"<<std::endl;
}

//---------------------