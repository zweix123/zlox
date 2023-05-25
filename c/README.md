+ 关键字`static`声明的函数具体静态链接, 只能在当前文件中使用


```
.
├── common.h    // 通用声明, 比如用true和false表示bool
├── debug.h     // 反汇编器打印内存和解释过程
└── chunk.c     // 
```

## 词法分析

```
.
├── common.h    // 通用声明, 比如用true和false表示bool
├── debug.h     // 反汇编器打印内存和解释过程
├── scanner.h   // 词法分析器
└── scanner.c   // 
```

这里展示从命令行读入的代码

```c
char line[1024];
for (;;) {
  printf("> ");
  if (!fgets(line, sizeof(line), stdin)) {
    printf("\n");  break;
  }
  initScanner(line);
  for (Token token = scanToken(); token.type != TOKEN_EOF; token = scanToken()) {
    showToken(token);
  }
}
```

从scanner的结构体可以看出它维护的是一个双指针, line属性表示当前代码的行数, 在错误提示时发挥作用  
代码很简单就是一个正则表达式的实现, 在这里对于保留字的匹配使用的是Trie树  
注意这里并没有实现结点、Trie等数据结构, 而是通过分支循环模拟对应状态机的行为  

## 编译

### 字节码的存储

```
.
├── common.h    // 通用声明, 比如用true和false表示bool
├── debug.h     // 反汇编器打印内存和解释过程
├── scanner.h   // 词法分析器
├── scanner.c   // 
├── memory.h    // 标准库的封装, 用于服务vector的宏
├── memory.c    // 
├── value.h     // 常量池的实现, 相当于一个元素类型为`Value`的vector
├── value.c     // 
└── chunk.h     // 内存的实现
```

我们来看一下常量是怎么存储的
```c
typedef struct {
  ValueType type; // 4 byte
  union {         // 取max type byte
    bool boolean;
    double number;
  } as;
} Value;

typedef struct {
  int capacity;
  int count;
  Value *values;
} ValueArray;
```
是不是一下子就明了了?很近似vector的实现, 而这里对泛型的实现也很炫酷

这里内存到底是怎么实现的呢？
```c
typedef struct {
  int count;
  int capacity;
  uint8_t *code;
  int *lines;
  ValueArray constants; // 常量池
} Chunk;
```
和上面还是很像的, 这里的`code`即为字节码, `lines`是字节码对应的源代码的行, 以相同的索引对应。那么为什么不和常量对应呢？

### Vaughan Pratt自顶向下算符优先解析
我们在jlox中使用递归下降算法实现解析器, 在这里使用Pratt
>递归下降需要标志位在前面, 所以对于表达式(表达式有在中间的符号), 变换语法使之在中间

Pratt Parsing，又称Top Down Operator Precedence Parsing，是一种很巧妙的算法，它实现简单、性能好，而且很容易定制扩展，尤其擅长解析表达式，擅长处理表达式操作符优先级(precedence)和结合性(associativity)。

+ Token分成两种
  + nul, 即prefix
  + led, 即infix

算了，我不能理解。

## 解释

```
.
├── common.h    // 通用声明, 比如用true和false表示bool
├── debug.h     // 反汇编器打印内存和解释过程
├── scanner.h   // 词法分析器
├── scanner.c   // 
├── memory.h    // 标准库的封装, 用于服务vector的宏
├── memory.c    // 
├── value.h     // 常量池的实现, 相当于一个元素类型为`Value`的vector
├── value.c     // 
└── chunk.h     // 内存的实现
├── vm.h        // 解释器
└── vm.c        //
```

