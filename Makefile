
CXXFLAGS = -g -Wall -Werror

MODULES = Block Serpent Main
OBJS = $(MODULES:%=%.o)
DEPENDS = $(MODULES:%=%.d)
PROG = serpentSolver

RESIDUE = $(OBJS) $(DEPENDS) $(PROG)


$(PROG) : $(OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

-include $(DEPENDS)

$(DEPENDS) : %.d : %.cpp
	$(CXX) $(CPPFLAGS) -MM $< > $@

clean :
	rm -rf $(RESIDUE)