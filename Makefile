TARGET       = regex
CPPFLAGS     = -fPIC -shared -std=c++11 -O2
INSTALL_PATH = /usr/local/lib/berry/packages

ifeq ($(OS), Windows_NT) # Windows
    CPPFLAGS += berry.lib
    TARGET := $(TARGET).dll
else
    TARGET := $(TARGET).so
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S), Darwin)
        CPPFLAGS += -undefined dynamic_lookup
    endif
endif

$(TARGET):
	@ g++ regex.cpp $(CPPFLAGS) -o $(TARGET)

install: $(INSTALL_PATH)
	@ cp $(TARGET) $(INSTALL_PATH)

$(INSTALL_PATH):
	@ mkdir -p $(INSTALL_PATH)

clean:
	rm $(TARGET)
