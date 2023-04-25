# zlox

## jlox
>这个更像是一个转换器，读入代码后，会将对应的数据转换到Java内存下，然后在Java的语义下运行。  
>所以在代码中也能看到各种trick

### Scanner

程序的入口为`Lox.java`，从main方法可以看到这个程序就像Python一样，既能解释一个文件，又能进入交互模式。两个方法都调用了方法`run`，在这里通过`Scanner`从代码中解析出Token列表。剩下的两个方法和error有关。

一个Token所必要的的数据结构：token type, lexeme, 字面量和location，这些在`Token.java`被组织，这里的token type是枚举量，相信你肯定知道他们在哪里。

上面提到的Scanner即一个正则表达式状态机的实现

截止到目前为止有几个妙处：
+ 先全部解析再决定是否要运行，这样能为用户报告出所有的错误
    >这在过去很有意义，因为那时编译代码可能和运行代码一样的慢，一次性报告所有错误是有必要的。

### Parser

+ 关于语法解析讨论到了不同编程范式的差别

    比如我们想为“类”添加“行为”
    + 在面向对象编程中，需要在类中添加方法，如果想对多个类都添加，只能一个个手动添加
    + 在函数式编程中，则只需要写一份代码，然后它是“与所有类型模式匹配”的函数
    
    反过来如果我们想添加一个类呢？
    + 面向对象是简单的，单独写一个就好
    + 而函数式编程则是麻烦，要对这个类匹配所有的已有函数

+ 怎么解决上面讨论的面向对象方法的缺点呢？The Visitor pattern
    >It does this the same way we solve almost every problem in computer science: by adding a layer of indirection





