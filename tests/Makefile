# determine kind of OS that is running
ifeq ($(OS),Windows_NT)
    ifeq ($(shell uname -o 2> /dev/null),Cygwin)
        unix_env := true
    else
        win_env := true
    endif
else
    unix_env := true
endif

# enumerate test suites
suites := colmgr commands column_view completion env escape fileops filetype
suites += filter keys misc options parsing undo variables viewcolumns_parser
suites += ioeta ionotif iop ior

# obtain list of sources that are being tested
vifm_src := ./ cfg/ compat/ engine/ io/ io/private/ modes/dialogs/ menus/ modes/
vifm_src += ui/ utils/
vifm_src := $(wildcard $(addprefix ../src/, $(addsuffix *.c, $(vifm_src))))

# filter out generally non-testable or sources for another platform
vifm_src := $(filter-out %/vifm.c %/vifmrc-converter.c %/win_helper.c, \
                         $(vifm_src))
ifndef unix_env
    vifm_src := $(filter-out %/desktop.c %/mntent.c %_nix.c, $(vifm_src))
endif
ifndef win_env
    vifm_src := $(filter-out %/wcwidth.c %/volumes_menu.c %_win.c, $(vifm_src))
endif

vifm_bin := ../src/vifm$(exe_suffix)
vifm_obj := $(vifm_src:.c=.o) bin/build/stubs.o

ifdef win_env
    make_arg   := -f Makefile.win
    exe_suffix := .exe
endif

# setup compile and link flags (partially depends on OS)
CFLAGS := -MMD -g -Wall -Werror -Iseatest -DTEST -include ../config.h
CFLAGS += -Wno-char-subscripts
LDFLAGS := -lpthread -g
ifdef unix_env
    LDFLAGS += $(shell sed -n     '/LIBS =/{s/^[^=]*=//p;q}' ../src/Makefile)
    LDFLAGS += $(shell sed -n  '/LDFLAGS =/{s/^[^=]*=//p;q}' ../src/Makefile)
    CFLAGS  += $(shell sed -n '/CPPFLAGS =/{s/^[^=]*=//p;q}' ../src/Makefile)
    CFLAGS  += -I/usr/include/ncursesw
endif
ifdef win_env
    LDFLAGS += $(shell sed -n '/LIBS :=/{s/^[^=]\+=//p;q}' ../src/Makefile.win)
endif

.PHONY: check build clean $(suites)

# check and build targets are defined mostly in suite_template
check: build

clean:
	-$(RM) -r bin/

$(vifm_src:.c=.o): | $(vifm_bin)

$(vifm_bin): $(vifm_src)
	$(MAKE) -C ../src/ $(make_arg) vifm$(exe_suffix)

bin/build/stubs.o: stubs.c
	gcc -c -o $@ $(CFLAGS) $<

bin/build/seatest.o: seatest/seatest.c
	gcc -c -o $@ $(CFLAGS) $<

# suite definition template, takes single argument: name of the suite
define suite_template

$1.src := $$(wildcard $1/*.c)
$1.obj := $$($1.src:%.c=bin/build/%.o)

deps += $$($1.obj:.o=.d)

bin/$1$(exe_suffix): $$($1.obj) bin/build/seatest.o $(vifm_obj) | $(vifm_bin)
	gcc -o $$@ $$^ $(LDFLAGS)

bin/build/$1/%.o: $1/%.c | bin/build/$1
	gcc -c -o $$@ $(CFLAGS) $$<

bin/build/$1:
	mkdir -p $$@

$1 $1.run: bin/$1$(exe_suffix)
	@$$^

build: bin/$1$(exe_suffix)

check: $1.run

endef

# walk throw list of suites and instantiate template for each one
$(foreach suite, $(suites), $(eval $(call suite_template,$(suite))))

# import dependencies calculated by the compiler
include $(wildcard $(deps) bin/build/seatest.d bin/build/stubs.d)
