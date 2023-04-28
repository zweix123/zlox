JAVAC = javac
JAVA = java

CORE = Lox.java
CORE_DIR = com/zlox/lox/
MAIN_CODE = $(CORE_DIR)$(CORE)
MAIN_CLASS = com.zlox.lox.Lox
GEN_CODE = com/zlox/tool/GenerateAst.java

LIB = lib/
JLINE = jline.jar

# CP = $(LIB)$(JLINE):./
CP = ./

.PHONY: all
all:
	echo "占位"

$(LIB):
	if [ ! -d "$(LIB)" ]; then mkdir $(LIB); fi

.PHONY: build
build: $(LIB)
# 下载第三方库
	if [ ! -f $(LIB)$(JLINE) ]; then wget -O $(LIB)$(JLINE) https://repo1.maven.org/maven2/org/jline/jline/3.21.0/jline-3.21.0.jar ; fi

.PHONY: run
run: $(LIB)
# javac
	$(JAVAC) -cp $(CP) $(MAIN_CODE)
# java
	$(JAVA) -cp $(CP) $(MAIN_CLASS)

	$(MAKE) clean

.PHONY: clean
clean:
	rm -rf $(CORE_DIR)*.class

###

# 由脚本生成代码, 只在开发中使用
.PHONY: gen
gen: $(GEN_CODE)
	$(JAVA) $(GEN_CODE) $(CORE_DIR)