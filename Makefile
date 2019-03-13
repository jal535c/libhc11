SOURCES := $(wildcard *.cpp)
OBJECTS := $(subst .cpp,.o,$(SOURCES))
INCLUDE := -I.
LIBS := 

all: emu

emu: $(OBJECTS)
	ar rs libemu.a $^ $(LIBS)

%.o: %.cpp
	g++ -g -c $^ $(INCLUDE)

clean:
	rm *.o libemu.a

info:
	$(info $(SOURCES))