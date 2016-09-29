BIN = mydraw 
GLLIBS = -lGL -lGLU -lglut

.SUFFIXES: .cpp .hpp

all: $(BIN)

clean:
	rm -f $(BIN)
	rm -f *.o
	rm -f *~

mydraw: clean
	g++ mydraw_class.cpp mydraw.cpp -o $@ $(GLLIBS) -std=c++0x


