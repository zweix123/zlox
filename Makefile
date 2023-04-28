JAVAC = javac
JAVA = java

GEN_SCRIPT = java/com/zlox/tool/GenerateAst.java
GEN_DIR = java/com/zlox/lox/
ARGS = $(GEN_DIR)

.PHONY: all

all: $(GEN_SCRIPT)
	$(JAVA) $(GEN_SCRIPT) $(ARGS)

