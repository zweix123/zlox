JAVAC = javac
JAVA = java

# 生成AST节点数据类
GEN_SCRIPT = java/com/zlox/tool/GenerateAst.java
GEN_DIR = java/com/zlox/lox/
ARGS = $(GEN_DIR)

.PHONY: all

all: $(GEN_SCRIPT)
	$(JAVA) $(GEN_SCRIPT) $(ARGS)

# 运行jlox
.PHONY: jlox

CODE = java/com/zlox/lox/Lox.java

jlox: 
	$(JAVA) $(CODE) $(FILE)