TARGET		 = regex
CPPFLAGS	 = -fPIC -shared -O2
INSTALL_PATH	 = /usr/local/lib/berry/packages

ifeq ($(OS), Windows_NT) # Windows
    CPPFLAGS += berry.lib
    TARGET := $(TARGET).dll
else
    TARGET := $(TARGET).so
endif

$(TARGET):
	@ g++ $(CPPFLAGS) regex.cpp -o $(TARGET)

install: $(INSTALL_PATH)
	@ cp $(TARGET) $(INSTALL_PATH)

$(INSTALL_PATH):
	@ mkdir -p $(INSTALL_PATH)
