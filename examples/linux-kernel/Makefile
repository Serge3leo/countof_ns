# vim:set sw=4 ts=8 noet fileencoding=utf8:
# SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0
# SPDX-FileCopyrightText: 2026 Сергей Леонтьев (leo@sai.msu.ru)

CFLAGS_test_array_size.o += -O0  # For two dimensions test fails, without need 3
ifdef PROPOSAL_LINUX_ARRAY_SIZE
    CFLAGS_test_array_size.o += -DPROPOSAL_LINUX_ARRAY_SIZE=1
endif
ifdef MIN_FIX
    CFLAGS_test_array_size.o += -DMIN_FIX
endif

obj-m += test_array_size.o

PWD := $(CURDIR)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

$(obj)/test_array_size.o: $(obj)/proposal_linux_array_size.h
