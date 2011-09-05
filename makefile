
# macro definition for program behavior
CONFIG =

# test 
TEST = kif --board 1,-1

# compile option
OPTIMAL_FLAGS = -DNDEBUG -O3
DEBUG_FLAGS = -g

# default program name 'make name=hoge' changes the name of program
name = kif

# add needed source files here
SOURCE = \
		main.cpp \
		kifio.cpp # treated special way

FILES = \
	$(SORCE) \
	kifio.h

STRING_TABLE_CHARSET=--input-charset=cp932 --exec-charset=cp932
HEADER = $(wildcard *.h)
CC = g++
LIBS=
WORK = Work
OBJECT = $(patsubst %.cpp,$(WORK)/%.obj,$(SOURCE))
DEPEND = $(patsubst %.cpp,$(WORK)/%.dep,$(SOURCE))

# create program with optimal option (default)
.PHONY: optimal
optimal: FLAGS = $(CONFIG) $(OPTIMAL_FLAGS)
optimal: $(name)

# create program with debug option
.PHONY: debug
debug: FLAGS = $(CONFIG) $(DEBUG_FLAGS)
debug: $(name)

.PHONY: clean
clean:
	rm -rf $(WORK)
	rm -f $(name)

tags: $(HEADER) $(SOURCE)
	ctags $(HEADER) $(SOURCE)


# create the program
$(name): $(DEPEND) $(OBJECT)
	$(CC) $(FLAGS) -o $(name) $(LIBS) $(OBJECT)

# create object files
$(WORK)/%.obj: %.cpp makefile
	@mkdir -p $(WORK)
#ifneq (kifio, $(subst $<,cpp,))
#	echo yes$(subst $<, .cpp, ,)yes
#	$(CC) $(FLAGS) -c $< -o $@
#else
	$(CC) $(FLAGS) $(if $(subst kifio.cpp,,$<),,$(STRING_TABLE_CHARSET)) -c $< -o $@
#endif

test:
	$(TEST)

doc: $(FILES)
	doxygen $(FILES)

# create dependencies
$(WORK)/%.dep: %.cpp makefile
	@mkdir -p $(WORK)
	@set -e; $(CC) -MM $(FLAGS) $< \
		| sed 's=\(.*\)\.o[ :]*=$(WORK)/\1.obj : $@ =g' > $@;
-include $(DEPEND)
