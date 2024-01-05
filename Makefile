CC=g++
test_led:
	$(CC) Main.cpp FileManagement.cpp LedCube.cpp Containers/Coordinate.cpp Containers/Color.cpp instructions/LedData.cpp instructions/Repeat.cpp instructions/Instruction.cpp Log.cpp -o ./bin/ledCube
	./bin/ledCube