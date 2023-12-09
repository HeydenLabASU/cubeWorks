CC=gcc
CFLAGS=-O3 -w
FT=-lfftw3f
OMP=-fopenmp

IDIR=include
ODIR=obj
BDIR=bin

_DEPS = types.h get.h matvec.h grids.h pdbio.h qsort.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJS = get.o matvec.o grids.o pdbio.o qsort.o \
cube2dat.o cubeAdd.o cubeDiv.o cubeFilter.o cubeMax.o \
cubeMove.o cubeMult.o cubeRot.o cubeScale.o cubeScaleInv.o \
cubeSub.o cubeSum.o cubeTitle.o resolvate.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

$(shell mkdir -p obj)
$(shell mkdir -p bin)

all: cube2dat cubeAdd cubeDiv cubeFilter cubeMax \
cubeMove cubeMult cubeRot cubeScale cubeScaleInv \
cubeSub cubeSum cubeTitle resolvate

cube2dat: $(OBJS)
	$(CC) -o bin/$@ $^ $(OBJS) $(CFLAGS)

cubeAdd: $(OBJS)
	$(CC) -o bin/$@ $^ $(OBJS) $(CFLAGS)

cubeDiv: $(OBJS)
	$(CC) -o bin/$@ $^ $(OBJS) $(CFLAGS)

cubeFilter: $(OBJS) obj/filter.o
	$(CC) -o bin/$@ $^ $(OBJS) $(CFLAGS) $(FT)

cubeMax: $(OBJS)
	$(CC) -o bin/$@ $^ $(OBJS) $(CFLAGS)

cubeMove: $(OBJS)
	$(CC) -o bin/$@ $^ $(OBJS) $(CFLAGS)

cubeMult: $(OBJS)
	$(CC) -o bin/$@ $^ $(OBJS) $(CFLAGS)

cubeRot: $(OBJS)
	$(CC) -o bin/$@ $^ $(OBJS) $(CFLAGS)

cubeScale: $(OBJS)
	$(CC) -o bin/$@ $^ $(OBJS) $(CFLAGS)

cubeScaleInv: $(OBJS)
	$(CC) -o bin/$@ $^ $(OBJS) $(CFLAGS)

cubeSub: $(OBJS)
	$(CC) -o bin/$@ $^ $(OBJS) $(CFLAGS)

cubeSum: $(OBJS)
	$(CC) -o bin/$@ $^ $(OBJS) $(CFLAGS)

cubeTitle: $(OBJS)
	$(CC) -o bin/$@ $^ $(OBJS) $(CFLAGS)

reolvate: $(OBJS)
	$(CC) -o bin/$@ $^ $(OBJS) $(CFLAGS)

$(ODIR)/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/filter.o: src/filter.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(FT)