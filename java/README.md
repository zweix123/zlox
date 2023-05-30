这是Lox语言的Java的实现。

个人觉得阅读源码的核心在于逻辑和文件结构，逻辑即具体的代码片段，文件结构则是表示逻辑的递进关系，这里文件结构一方面和项目设计有关，另一方面和语言也有关系，比如Java一个文件就表示一个类，它“限制"了文件结构设计。  
下面的讲解会随着讲解标记各个文件的内容。

```
lox                     
├── Environment.java      // ?
├── Expr.java             // ?
├── Interpreter.java      // ?
├── LoxCallable.java      // ?
├── LoxClass.java         // ?
├── LoxFunction.java      // ?
├── LoxInstance.java      // ?
├── Lox.java              // ?
├── NativeFunctions.java  // ?
├── Parser.java           // ?
├── Resolver.java         // ?
├── Return.java           // ?
├── RuntimeError.java     // ?
├── Scanner.java          // ?
├── Stmt.java             // ?
├── Token.java            // ?
└── TokenType.java        // ?
```

项目入口在文件`Lox.java`中，有两种使用方式，一个是提供一个文件，另一个是REPL。  
两个方法都调用`run`方法解释传入的代码，在这个方法中体现了jlox的运转方式：先是词法分析得到Token的列表，然后语法分析建立抽象语法树，最后分别是静态检查和解释。更高的一个抽象层次解释是**先建立抽象语法树，然后将Lox语义下的数据放入Java的内存中，然后Java直接在AST上运行Lox代码对应的逻辑**。这里的每个步骤都有一个类实现，分别对应一个文件。

```
lox                     
├── Environment.java      // ?
├── Expr.java             // ?
├── Interpreter.java      //+解释器
├── LoxCallable.java      // ?
├── LoxClass.java         // ?
├── LoxFunction.java      // ?
├── LoxInstance.java      // ?
├── Lox.java              //+项目入口
├── NativeFunctions.java  // ?
├── Parser.java           //+语法分析解析器
├── Resolver.java         //+静态检查解析器
├── Return.java           // ?
├── RuntimeError.java     // ?
├── Scanner.java          //+词法分析扫描器
├── Stmt.java             // ?
├── Token.java            // ?
└── TokenType.java        // ?
```

如果您开发过一门语言，一定知道在编译前端中不可或缺的就是Token和语法的表示

+ Token：在文件`Token.java`和`TokenType.java`中，其中`TokeType.java`是所有的Token类型枚举，在注释中表明其含义。
+ 语法：在文件`Expr.Java`和`Stmt.Java`中，使用上下文无关语法，这里两个文件是由`tool/GenerateAst.java`生成的。
	+ 生成式在代码中体现是：head为类名，body为类属性
	+ 代码上，两个文件是抽象基类，然后各个“生成式类”派生自它，代码在其内部（这样只是为了把这些东西放在一个文件中）
	+ 抽象基类有一个接口`Visitor<R>`，它的意义是这样的，想一下，如果我们想为这些生成式类增加一类动作，比如输出，每个类的输出内容、格式确实是不一样的，但是输出这个行为是一致的。有了这个接口，我们可以实现它，然后每个生成式类都有一个`accept`方法，它的参数就是`Visitor<R>`接口，即如果想让类使用对应的行为将对应的实现类放入即可，这个也就是访问者设计模式。

	关于语法的更多解释在语法分析一节进一步解释

```
lox                     
├── Environment.java      // ?
├── Expr.java             //+表达式AST结点
├── Interpreter.java      // 解释器
├── LoxCallable.java      // ?
├── LoxClass.java         // ?
├── LoxFunction.java      // ?
├── LoxInstance.java      // ?
├── Lox.java              // 项目入口
├── NativeFunctions.java  // ?
├── Parser.java           // 语法分析解析器
├── Resolver.java         // 静态检查解析器
├── Return.java           // ?
├── RuntimeError.java     // ?
├── Scanner.java          // 词法分析扫描器
├── Stmt.java             //+语句AST结点
├── Token.java            //+Token类
└── TokenType.java        //+Token枚举
```

## 词法分析
即文件`Scanner.java`，是一个正则表达式状态机的实现，但是并没有建立一个正式的写结点数据结构、图数据结构等等完备的建立一个有向无环图，而是**通过分支模拟建立好的词法分析状态机的行为**。

## 语法分析
即文件`Parser.java`，使用下降递归方法建立抽象语法树。

+ 上面提到`Expr.Java`和`Stmt.Java`中的各个生成式类，其在代码层次上的意义是作为抽象语法树的结点，其成员其实是指向其他结点的指针（只是由于Java的语法特性不像C++的指针那样直接）

+ 如何建立？按照语法的形式化语言（消除歧义的、在下面有讨论），每个非终端都是一个函数。

### 语法的形式化表达

上下文无关语法这个工具在jlox以多种的方式来使用了
1. 描述生成式类的结构，即抽象语法树中的每个结点的结构
2. 精准的描述Lox语法，继而通过递归下降技术建立抽象语法树

为什么会这样呢？比如二元运算符，这么多的二元运算符都可以用一个生成式表达，继而形成代码中的一个生成式类（抽象语法树的结点），但是这样的语法是有歧义的，因为不同的运算符有不同的优先级和结合性，需要把这部分同样用上下文无关语法体现出来才是没有歧义的语法，而这样的语法肯定会有更多的生成式，这些生成式就没有必要分别对应到生成式类了，依然只用那一个表达，其区别在抽象语法树上执行代码时再体现。

还有一个问题，比如有个重重组合的类，我们可能以这样的方式访问成员`1.2.3.4.5 = 6`，这是一个普通的赋值语句，但是如果把左边的词素粗糙的直接放在一个结点在后续不好处理，对于这样的处理也进行划分，上面例子`"Get : Expr object, Token name"`，这样对于上面的代码也能对每个名称作为结点建立树枝了。

还记得每个结点的数据类是由什么生成的嘛？在`tool/GenerateAst.java`里有上下文无关语法的伪代码。下面则是描述Lox语法的、消除了歧义的上下文无关语法。

```
program        → declaration* EOF ;
declaration    → classDecl
               | funDecl
               | varDecl
               | statement ;

classDecl      → "class" IDENTIFIER ( "<" IDENTIFIER )? "{" function* "}" ;

funDecl        → "fun" function ;
function       → IDENTIFIER "(" parameters? ")" block ;
parameters     → IDENTIFIER ( "," IDENTIFIER )* ;

varDecl        → "var" IDENTIFIER ( "=" expression )? ";" ;

statement      → exprStmt
               | forStmt
               | ifStmt
               | printStmt
               | returnStmt
               | whileStmt
               | block ;

exprStmt       → expression ";" ;  // 前者是表达式语句, 后者是表达式
forStmt        → "for" "(" ( varDecl | exprStmt | ";" ) expression? ";" expression? ")" statement ;
ifStmt         → "if" "(" expression ")" statement ( "else" statement )? ;
printStmt      → "print" expression ";" ;
returnStmt     → "return" expression? ";" ;
whileStmt      → "while" "(" expression ")" statement ;
block          → "{" declaration* "}" ;

expression     → assignment ;

assignment     → ( call "." )? IDENTIFIER "=" assignment
               | logic_or ;
logic_or       → logic_and ( "or" logic_and )* ;
logic_and      → equality ( "and" equality )* ;
equality       → comparison ( ( "!=" | "==" ) comparison )* ;
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term           → factor ( ( "-" | "+" ) factor )* ;
factor         → unary ( ( "/" | "*" ) unary )* ;
unary          → ( "!" | "-" ) unary | call ;
call           → primary ( "(" arguments? ")" | "." IDENTIFIER )* ;

primary        → "true" | "false" | "nil" | "this"
               | NUMBER | STRING | IDENTIFIER | "(" expression ")"
               | "super" "." IDENTIFIER ;

arguments      → expression ( "," expression )* ;
```

上下文无关语法是怎么转换成代码的呢——**非终端为一个函数，终端是一个变量**。所以上面的上下文无关语法和代码非常容易的就对应起来了。

+ Function的代码复用：函数和方法是类似的，这里使用同一个结点表示、同一个方法处理。
+ `for`语句的desugaring：在抽象语法树上，for语句的存储使用的是while语句的结点
	>来让后端更简单


## 执行
在抽象语法树上递归下降执行，是的，现在就执行了，就像开始说的，我们直接在抽象语法树上执行，还记得上面的`Visitor<R>`接口嘛，解释器只需要分别实现每个结点的行为即可。但是并不简单，我们要仔细分析。

+ 环境：所有的数据都是存在Java的内存中，我们需要知道一个名称对应的数据是什么。在代码`Environment.java`中，有一个键为String、值为Object的Map，在这里保存所有的数据。
	+ 为什么键是String而不是Token，因为同一个名称很可能表示同一个数据，但是每个词素都是单独的Token，不能用。

	这里还有一个关键变量`enclosing`，这里就很有意思，最后的环境是一个树的形态，这里的每个结点都是一个上面说的Map。深度更小的结点表示更大的作用域。这个结点其实是一个指针。

	+ 同名覆盖
		+ 对于当前作用域的名称，直接在当前的Map中找
		+ 如果当前找不到，就可以顺着“环境树”往上找

```
lox                     
├── Environment.java      //+环境类
├── Expr.java             // 表达式AST结点
├── Interpreter.java      // 解释器
├── LoxCallable.java      // ?
├── LoxClass.java         // ?
├── LoxFunction.java      // ?
├── LoxInstance.java      // ?
├── Lox.java              // 项目入口
├── NativeFunctions.java  // ?
├── Parser.java           // 语法分析解析器
├── Resolver.java         // 静态检查解析器
├── Return.java           // ?
├── RuntimeError.java     // ?
├── Scanner.java          // 词法分析扫描器
├── Stmt.java             // 语句AST结点
├── Token.java            // Token类
└── TokenType.java        // Token枚举
```

### 静态分析

在正式执行抽象语法树之间，我们还需要提前先遍历一遍树，上面虽然能搞清块作用域的变量名问题，但是实际情况更加复杂，比如类名、函数名的从属关系，比如类中的属性、变量的从属关系、一个函数是函数还是方法、一个对象调用基类的方法等等。

在代码`Resolver.java`中，这里的核心是：
```java
private final Stack<Map<String, Boolean>> scopes = new Stack<>();
```
这里的思想其实和上面很像，当进入一个新的作用域，则在栈中压入一个新Map

+ 值的类型是布尔，是这样设置的，如果名称声明，则放入Map，但是值初始化为假，如果名称定义，再修改值为真。
+ 上面提到一个问题，如果确定一个函数是函数还是方法，我们维护一个全局的两个枚举量表示当前的类是什么层次、函数是什么类型（注意递归下降是DFS！）

最后通过`Interpreter`（还没讲）的`resolve`方法将“需要在环境树往上走的步数”（相当于指明了名称具体指的是什么（数据或代码））

```
lox                     
├── Environment.java      // 环境类
├── Expr.java             // 表达式AST结点
├── Interpreter.java      // 解释器
├── LoxCallable.java      // ?
├── LoxClass.java         // ?
├── LoxFunction.java      // ?
├── LoxInstance.java      // ?
├── Lox.java              // 项目入口
├── NativeFunctions.java  // ?
├── Parser.java           // 语法分析解析器
├── Resolver.java         // 静态检查解析器
├── Return.java           // ?
├── RuntimeError.java     // ?
├── Scanner.java          // 词法分析扫描器
├── Stmt.java             // 语句AST结点
├── Token.java            // Token类
└── TokenType.java        // Token枚举
```

### 解释