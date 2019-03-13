SOURCES := $(wildcard *.cpp)
OBJECTS := $(subst .cpp,.o,$(SOURCES))
INCLUDE := -I.
LIBS := 

all: core


core: $(OBJECTS)
	ar rs libcore.a $^ $(LIBS)

%.o: %.cpp
	g++ -c $^ $(INCLUDE)

clean:
	rm *.o libcore.a

info:
	$(info $(SOURCES))