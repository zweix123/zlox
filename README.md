# zlox

太优雅了，我要是能在低年级的时候遇到该多好呀。

## jlox
>在这里，我们的实现更像是一个转换器，并不是代码和代码之间的转换，而是将Lox对应的语义转换到Java下的语义，然后由Java去运行。所以这里的代码有很多trick。

### 词法分析

程序的入口为`Lox.java`，从main方法可以看到这个程序就像Python一样，既能解释一个文件，又能进入REPL交互模式。两个方法都调用了方法`run`，在这里通过`Scanner`从代码中解析出Token列表。

在这里出也定义了两个方法`error`和`report`用于打印错误。

一个token所必要的的数据结构：token type、lexeme、字面量和location，这些在`Token.java`中被组织，这里的token type是枚举量（在代码`TokenType.java`中）

这里提到的Scanner即为一个正则表达式状态机的实现。

+ 关于错误处理，这里是先全部解析在决定是否要运行，这样能为用户报错出所有错误。
    >这在过去很有意义，因为那时的编译时间可能和运行时间一样慢。

### 句法分析和语法分析

首先我们需要一种方式表达语法——上下文无关语法。
```
expression     → literal
               | unary
               | binary
               | grouping ;

literal        → NUMBER | STRING | "true" | "false" | "nil" ;
grouping       → "(" expression ")" ;
unary          → ( "-" | "!" ) expression ;
binary         → expression operator expression ;
operator       → "==" | "!=" | "<" | "<=" | ">" | ">="
               | "+"  | "-"  | "*" | "/" ;
```

想一下这种形式化语言中的单元是什么？一个生成式，这些生成式如何在代码中被表达呢？我们将每个生成式定义为一个类，类名即为生成式的head，类的成员即为body中的终端或非终端。

这些代码在`Expr.java`中，我们看所有的“生成式类”都派生自一个抽象类`Expr`
>这里之所以把所有的生成式类都放在这个抽象类里面是为了把这些都放在一个文件中。

对于每个生成式类的定义就非常有规律了，实际上，这些都是由脚本`tool/GenerateAst.java`生成的。

在后续的开发中，这里的每个类表示抽象语法树中的一个节点，我们想要为这些结点赋予统一的行为（比如想打印，要让每个节点都打印，虽然打印内容的格式可能不一样，但是“打印”这个动作是统一的）

+ 使用设计模式The Visitor pattern
  + 在抽象基类`Expr`中，定义泛型接口`Visitor<R>`，并且有一个抽象方法`accept`，这个方法接受一个`Visitor`的参数，所有的生成式类都要实现这个方法。
  + 如果可以有一个类实现这个接口的所有方法，那么这些生成式类的对象可以通过accept的传参来实现不同的行为。而且这个类对不同的生成式类实现了不同的行为。
  + 即实现：不修改类代码，有为所有的类添加“一种”行为。

下面就要建立抽象语法树AST，使用的方法是——递归下降

在此之前，我们发现Lox之前的语法没有优先级和结合性，还有歧义，修改为
```
expression     → equality ;
equality       → comparison ( ( "!=" | "==" ) comparison )* ;
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term           → factor ( ( "-" | "+" ) factor )* ;
factor         → unary ( ( "/" | "*" ) unary )* ;
unary          → ( "!" | "-" ) unary
               | primary ;
primary        → NUMBER | STRING | "true" | "false" | "nil"
               | "(" expression ")" ;
```

解释器的代码在`Parse.java`中，在这里体现了递归下降方法是怎么实现的：每个生成式的head是一个函数，右边的每个非终端则还是一个函数，递归进去，终端则是一个节点。值得一提的是，对于`*`和`+`这样正则语言的概念，这里直接将之前建立好的结点作为新结点的儿子，用新节点去替换，也有“递归”的味道。

错误处理：

### 执行

可以直接在AST上执行代码。我们上面已经可以为每个AST上的节点赋予行为了，这里只要赋予和语义相同的行为就可以了。代码在`Interpreter.java`中

在上面我们得到了语法树的根，从这里进入，这里对照泛型接口`Visitor`的要求实现一个一个看即可。值得一提的是，在Lox的所有类型在Java中由Java的Object承接，然后在Java的语义下处理。

错误处理：

### 添加语句

在上面的语法中，AST的顶点是表达式，下面引入语句

```
program        → statement* EOF ;

statement      → exprStmt
               | printStmt ;

exprStmt       → expression ";" ;
printStmt      → "print" expression ";" ;
```
这里直接把print作为语法
>至少现在终于把program作为语法起点

在这里主要是AST的顶点变化了，我们新增一个数据类`Stmt`（`Stmt.java`）,相应的在解析器和解释器中对应的变化。