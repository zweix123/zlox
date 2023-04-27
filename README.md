# zlox

## jlox
>这个更像是一个转换器，读入代码后，会将对应的数据转换到Java内存下，然后在Java的语义下运行。  
>所以在代码中也能看到各种trick

### Scanner

程序的入口为`Lox.java`，从main方法可以看到这个程序就像Python一样，既能解释一个文件，又能进入交互模式。两个方法都调用了方法`run`，在这里通过`Scanner`从代码中解析出Token列表。剩下的两个方法和error有关。

一个Token所必要的的数据结构：token type, lexeme, 字面量和location，这些在`Token.java`中被组织，这里的token type是枚举量，相信你肯定知道它们在哪里。

上面提到的Scanner即一个正则表达式状态机的实现，最后我们将会得到一个token的列表


截止到目前为止有几个妙处：
+ 先全部解析再决定是否要运行，这样能为用户报告出所有的错误
    >这在过去很有意义，因为那时编译代码可能和运行代码一样的慢，一次性报告所有错误是有必要的。

### Parser

#### 上下文无关语法

对于生成式的head和body，怎么在实现中表示呢？

每个生成式都是一个类，类名即为head，类成员即为body，那么body中如果有其他生成式的head也能表达

+ 这些代码在`Expr.java`中，所有的生成式类都在一个抽象类`Expr`中，以继承自它，所以所有的生成式都可以说是`Expr`类型的。

+ `Expr.java`的代码并不是手动编辑的，而是由`tool/GenerateAst`生成的。

+ 在后续的实现中，还有其他的问题，我们想让所有的生成式都支持某种一样的操作，这在面向对象的编程范式中是复杂的，因为这需要将这些代码添加到每个类中，并不优雅。  

    使用设计模式The Visitor pattern：
    
    + 在抽象基类`Expr`中，定义一个泛型接口`Visitor<R>`，然后所有的Expr派生类都有一个`accept`方法，它接受一个Visitor的实现，然后调用接口中和自己对应的方法。
    + 我们只需要实现接口Visitor，在调用时将对应的实现作为参数给到各个类的accept方法中，即可在不再改变类内部的情况添加新的方法且共同于所有的Expr派生类。
    + 这里有一个Visitor的简单实现，代码在`AstPrinter.java`中，用于打印一个建立好的抽象语法树。

#### 递归下降

在上一个commit的提交中的Expr.java所表达的上下文无关语法是有歧义，我们需要加入优先级和结合性。在这样的情况建立的语法上下文无关语法就可以建立抽象语法树了。

解析器的实现在`Parse.java`，运用的方法是递归下降。
>在这里我们发现我们上一节建立的类其实是数据结构来保存AST中的每个节点。

在这里，每个生成式的head是一个函数，然后在每个函数中会通过上面的数据结构创建一个节点。这里，对扩展语法的解释是将已经有的节点纳入new出来的节点当中（非常的精妙）。
