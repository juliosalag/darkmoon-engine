# Main configuration
ENGINE   := darkmoon
GAME     := game

ifeq ($(OS),Windows_NT)
    EXT   := dll
else
    EXT   := so
endif

.PHONY: clean engine game run

engine:
	$(MAKE) -C $(ENGINE)
	cp $(ENGINE)/lib$(ENGINE).$(EXT) $(GAME)/

game:
	$(MAKE) -C $(GAME)

run:
	$(MAKE) -C $(GAME) run

clean:
	$(MAKE) -C $(ENGINE)/ clean
	$(MAKE) -C $(GAME)/ clean