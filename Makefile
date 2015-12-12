
CXXFLAGS = -g -Wall -Werror -fPIC
LDFLAGS =

MODULES = Block Space Coordinates Serpent
OBJS = $(MODULES:%=%.o)
DEPENDS = $(MODULES:%=%.d)
PROG = serpentSolver

PY_MODULE = Serpent.py
PY_LIB = _Serpent.so

SWIG = swig
SWIG_WRAP = Serpent_wrap
SWIG_WRAP_OBJ = $(SWIG_WRAP).o
SWIG_WRAP_SRC = $(SWIG_WRAP).cxx

RESIDUE = \
	$(OBJS) \
	$(DEPENDS) \
	$(PROG) \
	$(PY_MODULE) \
	$(PY_LIB) \
	$(SWIG_WRAP_OBJ) \
	$(SWIG_WRAP_SRC) \
	Serpent.py \
	Serpent.pyc


$(PROG) : $(OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $(OBJS) Main.cpp $(LDFLAGS)

.PHONY : python
$(PY_LIB) python : $(OBJS) $(SWIG_WRAP_OBJ)
	$(LD) -shared $^ -o $(PY_LIB)

$(SWIG_WRAP_OBJ) : $(SWIG_WRAP_SRC)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@ -I/usr/include/python2.7

$(SWIG_WRAP_SRC) : Serpent.i
	$(SWIG) -c++ -python Serpent.i

-include $(DEPENDS)

$(DEPENDS) : %.d : %.cpp
	$(CXX) $(CPPFLAGS) -MM $< > $@

clean :
	rm -rf $(RESIDUE)
