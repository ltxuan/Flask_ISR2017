CC=gcc
OBJS2=example-demon.o subagentObject.o read_config_snmp.o
TARGETS=example-demon

CFLAGS=-I. `net-snmp-config --cflags`
BUILDLIBS=`net-snmp-config --libs`
BUILDAGENTLIBS=`net-snmp-config --agent-libs`

# shared library flags (assumes gcc)
DLFLAGS=-fPIC -shared

all: $(TARGETS)

example-demon: $(OBJS2)
#example-demon: example-demon.c
	$(CC) -o example-demon $(OBJS2)  $(BUILDAGENTLIBS) -lpthread

clean:
	rm $(OBJS2) $(TARGETS)

