@echo off

BoomslangCompiler boomslang\filesystem\filesystem.boomslang -package -output filesystem-windows.branch
BoomslangCompiler boomslang\image\image.boomslang           -package -output image-windows.branch
BoomslangCompiler boomslang\math\math.boomslang             -package -output math-windows.branch
BoomslangCompiler boomslang\random\random.boomslang         -package -output random-windows.branch
BoomslangCompiler boomslang\user\user.boomslang             -package -output user-windows.branch

BoomslangCompiler opengl\gl.wrap.boomslang                  -package -output opengl-windows.branch
BoomslangCompiler glew\glew.wrap.boomslang                  -package -output glew-windows.branch
BoomslangCompiler glfw\glfw.boomslang                       -package -output glfw-windows.branch