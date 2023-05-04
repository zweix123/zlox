# zlox
Lox语言的zweix实现

## jlox
这里是Lox的Java的实现，代码在目录`java`目录下

思路是建立抽象语法树AST，然后将Lox语义下的数据放入Java的内存中，然后Java直接在AST上运行Lox

如果您开发过一门语言，一定知道在编译前端中不可或缺的就是Token和语法的表示
+ Token：在文件`Token.java`和`TokenType.java`中，其中`TokeType.java`是所有的Token类型枚举，在注释中表明其含义。
+ 语法：在文件`Expr.Java`和`Stmt.Java`中，使用上下文无关语法，这里两个文件是由`tool/GenerateAst.java`生成的。
  + 生成式在代码中体现是：head为类名，body为类属性
  + 代码上，两个文件是抽象基类，然后各个“生成式类”派生自它，代码在其内部（这样只是为了把这些东西放在一个文件中）
  + 抽象基类有一个接口`Visitor<R>`，它的意义是这样的，想一下，如果我们想为这些生成式类增加一类动作，比如输出，每个类的输出内容、格式确实是不一样的，但是输出这个行为是一致的。有了这个接口，我们可以实现它，然后每个生成式类都有一个`accept`方法，它的参数就是`Visitor<R>`接口，即如果想让类使用对应的行为将对应的实现类放入即可。

整个过程分成一下几个部分

1. 词法分析：在文件`Scanner.java`中，即一个正则语言表达式状态机的实现，我个人的感觉没有刻意实现的像个状态机，还是比较自然的处理。
2. 语法分析：在文件`Parser.java`中，使用递归下降方法建立抽象语法树，值得一提的是
  + 上面提到`Expr.Java`和`Stmt.Java`中的各个生成式类，其在代码层次上的意义是作为抽象语法树的结点，其成员其实是指向其他结点的指针（只是由于Java的语法特性不像C++的指针那样直接）
  + 如何建立？按照语法的形式化语言（消除歧义的、在下面有讨论），每个非终端都是一个函数。
3. 在抽象语法树上递归下降执行：在文件`Interpreter.java`中，就像在最开始说的，jlox的执行是直接执行在抽象语法树上的
  + 还记得上面的`Visitor<R>`接口嘛，解释器只需要分别实现每个结点的行为即可。

### 语法的形式化表达

上下文无关语法这个工具在jlox以多种的方式来使用了
1. 描述生成式类的结构，即抽象语法树中的每个结点的结构
2. 精准的描述Lox语法，继而通过递归下降技术建立抽象语法树

为什么会这样呢？比如二元运算符，这么多的二元运算符都可以用一个生成式表达，继而形成代码中的一个生成式类（抽象语法树的结点），但是这样的语法是有歧义的，因为不同的运算符有不同的优先级和结合性，需要把这部分同样用上下文无关语法体现出来才是没有歧义的语法，而这样的语法肯定会有更多的生成式，这些生成式就没有必要分别对应到生成式类了，依然只用那一个表达，其区别在抽象语法树上执行代码时再体现。

还记得每个结点的数据类是由什么生成的嘛？在`tool/GenerateAst.java`里有上下文无关语法的伪代码。下面则是描述Lox语法的、消除了歧义的上下文无关语法。

```
program        → declaration* EOF ;
declaration    → varDecl
               | statement ;
varDecl        → "var" IDENTIFIER ( "=" expression )? ";" ;
statement      → exprStmt
               | ifStmt
               | printStmt
               | whileStmt
               | block ;
exprStmt       → expression ";" ;
ifStmt         → "if" "(" expression ")" statement
               ( "else" statement )? ;
printStmt      → "print" expression ";" ;
whileStmt      → "while" "(" expression ")" statement ;
block          → "{" declaration* "}" ;
expression     → assignment ;
assignment     → IDENTIFIER "=" assignment
               | logic_or ;
logic_or       → logic_and ( "or" logic_and )* ;
logic_and      → equality ( "and" equality )* ;
equality       → comparison ( ( "!=" | "==" ) comparison )* ;
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term           → factor ( ( "-" | "+" ) factor )* ;
factor         → unary ( ( "/" | "*" ) unary )* ;
unary          → ( "!" | "-" ) unary
               | primary ;
primary        → "true" | "false" | "nil"
               | NUMBER | STRING
               | "(" expression ")"
               | IDENTIFIER ;
```
