shared:
	g++ testCV.cpp -lopencv_contrib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_objdetect -lopencv_ocl -fPIC -shared -o libFaceDetectGPU.so -ljson

all:
	g++ testCV.cpp json.cpp PracticalSocket.cpp -lopencv_core -lopencv_highgui -lopencv_contrib -lopencv_imgproc -lopencv_gpu -lopencv_objdetect -ljson -std=c++11 
