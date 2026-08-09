a2ps -o library.ps --font-size=10 -R --columns=1 Library/*.h Library/*.cpp
a2ps -o examples.ps --font-size=10 -R --columns=1 examples/*.h examples/*.cpp
ps2pdf library.ps
ps2pdf examples.ps
rm library.ps examples.ps
