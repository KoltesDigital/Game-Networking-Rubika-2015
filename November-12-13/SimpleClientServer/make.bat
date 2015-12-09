call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"
cl /I . client.cpp /link enet.lib ws2_32.lib winmm.lib
cl /I . server.cpp /link enet.lib ws2_32.lib winmm.lib
