TARGET       = regex
CPPFLAGS     = -fPIC -shared -std=c++11 -O2
INSTALL_PATH = /usr/local/lib/berry/packages

ifeq ($(OS), Windows_NT) # Windows
    CPPFLAGS += berry.lib
    TARGET := $(TARGET).dll
else
    UNAME_S := $(shell uname -s)
    TARGET := $(TARGET).so
    ifeq ($(UNAME_S), Darwin)
        CPPFLAGS += -undefined dynamic_lookup
    endif
endif

$(TARGET):
	@ g++ $(CPPFLAGS) regex.cpp -o $(TARGET)

install: $(INSTALL_PATH)
	@ cp $(TARGET) $(INSTALL_PATH)

$(INSTALL_PATH):
	@ mkdir -p $(INSTALL_PATH)

clean:
	rm $(TARGET)
