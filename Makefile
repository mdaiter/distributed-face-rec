shared:
	g++ testCV.cpp -lopencv_contrib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_objdetect -lopencv_ocl -fPIC -shared -o libFaceDetectGPU.so -ljson

all:
<<<<<<< HEAD
	g++ testCV.cpp json.cpp PracticalSocket.cpp -lopencv_core -lopencv_highgui -lopencv_contrib -lopencv_imgproc -lopencv_ocl -lopencv_objdetect -ljson -std=c++11 
=======
	g++ testCV.cpp -lopencv_core -lopencv_highgui -lopencv_contrib -lopencv_imgproc -lopencv_ocl -lopencv_objdetect
>>>>>>> 9108755a8713f59559ecee239372909223f1e047
