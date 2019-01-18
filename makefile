CXX = g++
CFLAGS = -I.
LIBS = -lm -lbcm2835
DEPS = Ks0108pi.h GaragePi.h
OBJ = main.o Ks0108pi.o GaragePi.o

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm main.o main
