MFLAGS =
Q := @

all:
	$(Q)$(MAKE) $(MFLAGS) -C test

clean:
	$(Q)$(MAKE) $(MFLAGS) -C test $@

