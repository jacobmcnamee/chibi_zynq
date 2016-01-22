.PHONY: all

all:
	$(MAKE) -C src $(MAKEFLAGS)
	
clean:
	$(MAKE) -C src $(MAKEFLAGS) clean
