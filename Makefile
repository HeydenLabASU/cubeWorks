CC=gcc
CFLAGS=-lm -O3
FT=-lfftw3f

IDIR=include
ODIR=obj
BDIR=bin

_DEPS = types.h get.h matvec.h grids.h pdbio.h qsort.h cubeWorks.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJS = get.o matvec.o grids.o pdbio.o qsort.o cubeWorks.o
OBJS = $(patsubst %,$(ODIR)/functions/%,$(_OBJS))

$(shell mkdir -p obj)
$(shell mkdir -p obj/functions)
$(shell mkdir -p bin)

all: cube2dat cubeAdd cubeAver cubeCenter cubeDelAtoms cubeDiv cubeFilter \
cubeMax cubeMirror cubeMove cubeMult cubeRot cubeScale cubeScaleInv \
cubeSub cubeSum cubeTitle resolvate

noFT: cube2dat cubeAdd cubeAver cubeCenter cubeDelAtoms cubeDiv \
cubeMax cubeMirror cubeMove cubeMult cubeRot cubeScale cubeScaleInv \
cubeSub cubeSum cubeTitle resolvate

cube2dat: $(ODIR)/cube2dat.o $(OBJS)
	$(CC) -o bin/$@ $^ $(CFLAGS)

cubeAdd: $(ODIR)/cubeAdd.o $(OBJS)
	$(CC) -o bin/$@ $^ $(CFLAGS)

cubeAver: $(ODIR)/cubeAver.o $(OBJS)
	$(CC) -o bin/$@ $^ $(CFLAGS)

cubeCenter: $(ODIR)/cubeCenter.o $(OBJS)
	$(CC) -o bin/$@ $^ $(CFLAGS)

cubeDelAtoms: $(ODIR)/cubeDelAtoms.o $(OBJS)
	$(CC) -o bin/$@ $^ $(CFLAGS)

cubeDiv: $(ODIR)/cubeDiv.o $(OBJS)
	$(CC) -o bin/$@ $^ $(CFLAGS)

cubeFilter: $(ODIR)/cubeFilter.o $(OBJS) obj/functions/filter.o
	$(CC) -o bin/$@ $^ $(CFLAGS) $(FT)

cubeMax: $(ODIR)/cubeMax.o $(OBJS)
	$(CC) -o bin/$@ $^ $(CFLAGS)

cubeMirror: $(ODIR)/cubeMirror.o $(OBJS)
	$(CC) -o bin/$@ $^ $(CFLAGS)

cubeMove: $(ODIR)/cubeMove.o $(OBJS)
	$(CC) -o bin/$@ $^ $(CFLAGS)

cubeMult: $(ODIR)/cubeMult.o $(OBJS)
	$(CC) -o bin/$@ $^ $(CFLAGS)

cubeRot: $(ODIR)/cubeRot.o $(OBJS)
	$(CC) -o bin/$@ $^ $(CFLAGS)

cubeScale: $(ODIR)/cubeScale.o $(OBJS)
	$(CC) -o bin/$@ $^ $(CFLAGS)

cubeScaleInv: $(ODIR)/cubeScaleInv.o $(OBJS)
	$(CC) -o bin/$@ $^ $(CFLAGS)

cubeSub: $(ODIR)/cubeSub.o $(OBJS)
	$(CC) -o bin/$@ $^ $(CFLAGS)

cubeSum: $(ODIR)/cubeSum.o $(OBJS)
	$(CC) -o bin/$@ $^ $(CFLAGS)

cubeTitle: $(ODIR)/cubeTitle.o $(OBJS)
	$(CC) -o bin/$@ $^ $(CFLAGS)

resolvate: $(ODIR)/resolvate.o $(OBJS)
	$(CC) -o bin/$@ $^ $(CFLAGS)

$(ODIR)/functions/%.o: src/functions/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/functions/filter.o: src/functions/filter.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(FT)

$(ODIR)/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)