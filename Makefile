CXX=g++
CXXFLAGS= -g  -O2
BIN := ./bin
LDFLAGS=-lz 
INCLUDE=-L./src/zlib/
all: $(BIN)/VCF2Dis

$(BIN)/VCF2Dis: $(BIN)/../src/VCF2Dis.o 
	$(CXX)   $^ -o $@   $(LDFLAGS)  $(INCLUDE) 

$(BIN)/%.o: %.cpp
	$(CXX)  -c $(CXXFLAGS) $< -o $@    $(INCLUDE) 

clean:
	$(RM) $(BIN)/*.o  $(BIN)/../src/*.o
