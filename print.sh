a2ps -o cpp.ps --font-size=10 -R --columns=1 \
    CppLibrary/*.h CppLibrary/*.cpp CppExamples/*.h CppExamples/*.cpp

a2ps -o cs.ps --font-size=10 -R --columns=1 \
    CsLibrary/*.cs CppExamples/*/*.cs

ps2pdf cpp.ps
ps2pdf cs.ps

rm *.ps
cp ../format.py .
python3 format.py CsLibrary/*.cs CsExamples/*/*.cs
rm format.py
