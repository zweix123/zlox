# zlox
Lox语言的zweix实现

+ Reference
    + [《Crafting Interpreters》](http://www.craftinginterpreters.com/)：全开源，相当棒，我愿称之为PL(Program Language)领域的最好的第一本书
    + [手撸解释器教程](https://readonly.link/books/https://raw.githubusercontent.com/GuoYaxiang/craftinginterpreters_zh/main/book.json)：上面的中文翻译，中英文对照的形式

如果您在学习编译原理的过程中觉得缺少实践, 强烈推荐使用这本书,
但是不建议作为学习C语言和Java语言的练手项目, 因为需要较多的PL知识, 以学习语言为目的会抓不到重点;
值得一提的是, 这本书提到的很多知识和技术很有利于对计算机体系结构方面的理解, 毕竟“程序就是状态机”。

## Introduction

我们可以以这个视角把"语言处理器"分成
1. 编译成机器码, 然后直接在物理机器上运行, 比如C
2. 编译成字节码, 然后在虚拟机上运行, 比如Java
	+ 基于堆栈的虚拟机
	+ 基于寄存器的虚拟机
3. 直接在抽象语法树上运行
	+ 性能低

在这个视角看, 编译型语言和解释型语言的划分是不明显的, 上面第三种很像解释型的; 
但是实际上, Java是编译成字节码, 然后在虚拟机JVM上运行, 我们通常称之为编译型; 
          Python是编译成字节码, 然后在虚拟机上运行，我们通常称之为解释型.

这本书中实现的jlox即为第三种类型, clox即为第二种类型。

这本书的架构是自底向上的, 这样有好处就是学习曲线更平缓, 但是在开发过程中会缺失某些概念在全局发挥的作用(而且随着开发这些概念往往是变化的, 逐渐到最终的含义的), 我这里自顶向下的对关键概念做阐述
+ [jlox introduction](./java/README.md)（去看clox吧, 这个不建议阅读）
+ [clox introduction](./c/README.md)

## Lox Grammer

+ style：
    + like js
    + dynamic type
    + garbage collection

+ commit：`//`（只提供单行注释）

+ build-in statement：
    + `print`

+ native function:
    + `clock()`

+ date types
    + `nil`
    + Boolean: `true` and `false`  
      `false` and `nil` is false, else is true
    + Numbers: double-precision floating point
    + Strings: enclosed in double qutes

+ Expressions:
	+ Arithmetic:
		+ binary operators/infix operator: `+`、`-`、`*`、`/`
		+ prefix operators: `-`
		+ the operator `+` support Strings
	+ Comparison and equality
		+ `<`、`<=`、`>`、`>=`
		+ `==`、`!=`
			+ 禁止隐式转换
	+ Logical operators
		+ `!`
		+ `and` `or`
			+ short-circuit
	+ Precedence and grouping：`()`

    优先级和结合性同C

+ Statements: end with `;`
	+ expression statement
	+ block: `{}`

+ Variables: `var`：动态类型，不需要指明类型，也不需要初始化，但是这样的语句相当于声明

+ Control Flow：
	+ `if`: 同C
	+ `while`: 同C
	+ `for`: 同C

+ Functions: 
	+ 定义：`fun ...`，其余同C
	+ `return` statement
		+ 没有默认`nil`
	+ Closures: 函数在Lox中是**first class**一等公民：可以赋值给变量

+ Class：
	+ 定义：`class ...`，在类中的方法不用关键字`func`
	+ 属性：可以直接向实例中添加属性
		+ 在类内部通过`this.`访问
	+ 构造函数`init`
	+ Inheritance：`class NewClass < BaseClass`
		+ 通过`super`访问

	+ 类在Lox中也是一等公民

+ misc
	+ 变量存在滞后性，先声明的函数可以使用在后面声明的全局变量，为了递归的方便实现
	+ 在全局变量中允许相同名称的变量重复声明, 这在REPL中很有用, 但是在局部作用域就认为是错误。

## 完整语法

```
// 语法
program        → declaration* EOF ; // Lox是一系列“声明”, 有一个《特殊》的声明是“语句”
// 声明
declaration    → classDecl
               | funDecl
               | varDecl
               | statement ;

classDecl      → "class" IDENTIFIER ( "<" IDENTIFIER )?
                 "{" function* "}" ;
funDecl        → "fun" function ;
varDecl        → "var" IDENTIFIER ( "=" expression )? ";" ;
// 语句: 声明会产生绑定, 语句则不会
statement      → exprStmt
               | forStmt
               | ifStmt
               | printStmt
               | returnStmt
               | whileStmt
               | block ;

exprStmt       → expression ";" ;
forStmt        → "for" "(" ( varDecl | exprStmt | ";" )
                           expression? ";"
                           expression? ")" statement ;
ifStmt         → "if" "(" expression ")" statement
                 ( "else" statement )? ;
printStmt      → "print" expression ";" ;
returnStmt     → "return" expression? ";" ;
whileStmt      → "while" "(" expression ")" statement ;
block          → "{" declaration* "}" ;
// 实用规则: 上面有些的表达比较复杂, 下面是一些中间量
function       → IDENTIFIER "(" parameters? ")" block ;
parameters     → IDENTIFIER ( "," IDENTIFIER )* ;
arguments      → expression ( "," expression )* ;
// 表达式
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
// 词法, 注意这里的非递归的, 所以是正则的
NUMBER         → DIGIT+ ( "." DIGIT+ )? ;
STRING         → "\"" <any char except "\"">* "\"" ;
IDENTIFIER     → ALPHA ( ALPHA | DIGIT )* ;
ALPHA          → "a" ... "z" | "A" ... "Z" | "_" ;
DIGIT          → "0" ... "9" ;
```

## develop

添加`switch`
```
switchStmt     → "switch" "(" expression ")"
                 "{" switchCase* defaultCase? "}" ;
switchCase     → "case" expression ":" statement* ;
defaultCase    → "default" ":" statement* ;
```