set folder="build"
if not exist %folder% mkdir %folder% 
cd %folder% 
for /F "delims=" %%i in ('dir /b') do (rmdir "%%i" /s/q || del "%%i" /s/q)
cmake ..