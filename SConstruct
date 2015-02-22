print '\x1b[31m' + "\n--------------------No-Motivation--------------------" + '\033[0m'

libs = Split("""
sfml-graphics
sfml-window
sfml-system
""")

sources = Split("""
src/application/main.cpp
src/application/Application.cpp
src/navigation/AStar.cpp
""")

Program(target = 'No-Motivation', source = sources, LIBS = libs, CXXFLAGS = "-std=c++11")

