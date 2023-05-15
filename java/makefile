JAVAC = javac
JAVA = java

CORE = Lox.java
CORE_DIR = com/zlox/lox/
MAIN_CODE = $(CORE_DIR)$(CORE)
MAIN_CLASS = com.zlox.lox.Lox
GEN_CODE = com/zlox/tool/GenerateAst.java

LIB = lib
JLINE = jline.jar
CP = $(LIB)/$(JLINE):./

.PHONY: all
all: $(LIB)

$(LIB):
	if [ ! -d "$(LIB)" ]; then mkdir $(LIB); fi
	if [ ! -f $(LIB)/$(JLINE) ]; then wget -O $(LIB)/$(JLINE) https://repo1.maven.org/maven2/org/jline/jline/3.21.0/jline-3.21.0.jar ; fi

.PHONY: run
run: $(LIB)
	$(JAVAC) -cp $(CP) $(MAIN_CODE)
	$(JAVA) -cp $(CP) $(MAIN_CLASS) $(arg)

	$(MAKE) clean

.PHONY: clean
clean:
	rm -rf $(CORE_DIR)*.class

# 由脚本生成代码, 只在开发中使用
.PHONY: gen
gen: $(GEN_CODE)
	$(JAVA) $(GEN_CODE) $(CORE_DIR)