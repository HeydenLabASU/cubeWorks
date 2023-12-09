CC=gcc
CFLAGS=-O3 -w
FT=-lfftw3f
OMP=-fopenmp

IDIR=include
ODIR=obj
BDIR=bin

_DEPS = types.h get.h matvec.h grids.h pdbio.h qsort.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJS = get.o matvec.o grids.o pdbio.o qsort.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

$(shell mkdir -p obj)
$(shell mkdir -p bin)

all: cube2dat cubeAdd cubeDiv cubeFilter cubeMax \
cubeMove cubeMult cubeRot cubeScale cubeScaleInv \
cubeSub cubeSum cubeTitle resolvate

cube2dat: $(ODIR)/cube2dat.o $(OBJS)
	$(CC) -o bin/$@ $^ $(OBJS) $(CFLAGS)

cubeAdd: $(ODIR)/cubeAdd.o $(OBJS)
	$(CC) -o bin/$@ $^ $(OBJS) $(CFLAGS)

cubeDiv: $(ODIR)/cubeDiv.o $(OBJS)
	$(CC) -o bin/$@ $^ $(OBJS) $(CFLAGS)

cubeFilter: $(ODIR)/cubeFilter.o $(OBJS) obj/filter.o
	$(CC) -o bin/$@ $^ $(OBJS) obj/filter.o $(CFLAGS) $(FT)

cubeMax: $(ODIR)/cubeMax.o $(OBJS)
	$(CC) -o bin/$@ $^ $(OBJS) $(CFLAGS)

cubeMove: $(ODIR)/cubeMove.o $(OBJS)
	$(CC) -o bin/$@ $^ $(OBJS) $(CFLAGS)

cubeMult: $(ODIR)/cubeMult.o $(OBJS)
	$(CC) -o bin/$@ $^ $(OBJS) $(CFLAGS)

cubeRot: $(ODIR)/cubeRot.o $(OBJS)
	$(CC) -o bin/$@ $^ $(OBJS) $(CFLAGS)

cubeScale: $(ODIR)/cubeScale.o $(OBJS)
	$(CC) -o bin/$@ $^ $(OBJS) $(CFLAGS)

cubeScaleInv: $(ODIR)/cubeScaleInv.o $(OBJS)
	$(CC) -o bin/$@ $^ $(OBJS) $(CFLAGS)

cubeSub: $(ODIR)/cubeSub.o $(OBJS)
	$(CC) -o bin/$@ $^ $(OBJS) $(CFLAGS)

cubeSum: $(ODIR)/cubeSum.o $(OBJS)
	$(CC) -o bin/$@ $^ $(OBJS) $(CFLAGS)

cubeTitle: $(ODIR)/cubeTitle.o $(OBJS)
	$(CC) -o bin/$@ $^ $(OBJS) $(CFLAGS)

reolvate: $(ODIR)/resolvate.o $(OBJS)
	$(CC) -o bin/$@ $^ $(OBJS) $(CFLAGS)

$(ODIR)/%.o: src/functions/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/filter.o: src/functions/filter.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(FT)

$(ODIR)/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)