# hello world

```go
package main

import "fmt"

func main() {
    fmt.Println("Hello World!")
}
```

+ `package main` 语句中告诉 Go，我们将要创建的应用是一个可执行程序（可以运行的文件）。
+ `import` 语句使你的程序可以访问其他包中的其他代码。 在本例中，fmt 为标准库包。
+ `func` 语句是用于声明函数的保留字。 此第一个函数名为“main”，因为它是程序的起始点。 整个 `package main` 中只能有一个 `main()` 函数。 在 `main()` 函数中，调用了 `fmt` 包中的 `Println` 函数。

**import导入方式：**

1. 点操作
   我们有时候会看到如下的方式导入包

   ```go
   import(
       . "fmt"
   )
   ```

   这个点操作的含义就是这个包导入之后在你调用这个包的函数时，你可以省略前缀的包名，也就是前面你调

   用的`fmt.Println("hello world")`可以省略的写成`Println("hello world")`

2. 别名操作
   别名操作顾名思义我们可以把包命名成另一个我们用起来容易记忆的名字

   ```go
   import(
       f "fmt"
   )
   ```

   调用包函数时前缀变成了我们的前缀，即`f.Println("hello world")`

3. _操作
   这个操作经常是让很多人费解的一个操作符，请看下面这个import

   ```go
   import (
     "database/sql"
     _ "github.com/ziutek/mymysql/godrv"
   )
   ```

   _操作其实是引入该包，而不直接使用包里面的函数，而是调用了该包里面的init函数

   TODO:这点目前还不理解

++++++

若要运行 Go 应用：

```console
go run main.go
```

生成可执行文件（exe文件）:

```console
go build main.go
```

go install命令保存编译后的可执行程序的二进制版本，但保存在定义良好、易于访问的位置：Go工作区中的bin目录。只需给go install指定src中包含可执行程序代码（即，以package main开头的.go文件）的目录名。

```
go install
```

# 声明和使用变量

## 声明变量

若要声明变量，需要使用 `var` 关键字：

```go
var firstName string
```

声明 `string` 类型中的一个名为`firstName` 的变量。 如要声明其他变量，只需将类似的语句添加到上一个语句中。 但如果两个变量的类型相同，则可以在一行中声明多个变量：

```go
var firstName, lastName string
```

通过在变量名称后面添加逗号 (`,`)，就表示你将要声明其他变量。 在这种情况下就声明了 `string` 类型中的两个变量：`firstName`和`lastName`。 

如要添加 `int` 类型的第三个变量：

```go
var firstName, lastName string
var age int
```

另一种编写前一个语句的方法是在 `var` 关键字后面使用括号，就像通过一个专用块来声明变量一样，如下所示：

```go
var (
    firstName, lastName string
    age int
)
```

## 初始化变量

 在 Go 中，可以通过多种方式初始化变量。 

```go
var (
    firstName string = "John"
    lastName  string = "Doe"
    age       int    = 32
)
```

如果你决定初始化某个变量，则不需要指定其类型，因为当你使用具体值初始化该变量时，Go 会推断出其类型。 例如，你可以用此方式声明并初始化变量：

```go
var (
    firstName = "John"
    lastName  = "Doe"
    age       = 32
)
```

Go 将推断出变量 `firstName` 和 `lastName` 的类型为 `string`，并且变量 `age` 属于 `int` 类型。

## 通过多种方式初始化变量

在 Go 中，你可以在单行中声明和初始化变量。 使用逗号将每个变量名称隔开，并对每个值执行相同的操作（按同一顺序），如下所示：

```go
var (
    firstName, lastName, age = "John", "Doe", 32
)
```

还可以通过另一种方式来声明和初始化变量。 此方法是在 Go 中执行此操作的最常见方法:

```go
func main() {
    firstName, lastName := "John", "Doe"
    age := 32
    fmt.Println(firstName, lastName, age)
}
```

注意，在定义变量名称后，需要在此处加入一个**冒号等于号 (`:=`)** 和相应的值。 使用冒号等于号时，**要声明的变量必须是新变量**。 如果使用冒号等于号并已经声明该变量，将不会对程序进行编译。 

**能在函数内使用冒号等于号。 在声明函数外的变量时，必须使用 `var` 关键字执行此操作**。

## 声明常量

有时，你需要在代码中加入静态值，这称为 *常量*。 Go 支持使用常量。 用于声明常量的关键字是 `const`。

例如，可以按如下所示声明常量：

```go
const HTTPStatusOK = 200
```

与变量一样，Go 可以通过分配给常量的值推断出类型。 在 Go 中，常量名称通常以混合大小写字母或全部大写字母书写。

如果需要在一个块中声明多个常量，可以按如下所示执行：

```go
const (
    StatusOK              = 0
    StatusConnectionReset = 1
    StatusOtherError      = 2
)
```

常量和变量之间既有相似之处，也有一些重要差异。 例如，你可以在不使用常量的情况下声明常量。 你不会收到错误消息。 不能使用冒号等于号 (`:=`)来声明常量。 如果采用这种方式，Go 会发出警告。

## 如果声明了变量但未使用，Go 会抛出错误

需要记住的重要一点是，在 Go 中，当你声明一个变量但不使用它时，Go 会抛出错误，而不是像某些其他编程语言一样抛出警告。

例如，让我们回到前面提及的某个示例，然后删除 `fmt.println` 调用：

```go
func main() {
    firstName, lastName := "John", "Doe"
    age := 32
}
```

当运行此代码时，你将看到在要声明变量的多个行中存在错误。 你将看到以下错误消息：

输出复制

```output
./main.go:4:2: firstName declared but not used
./main.go:4:13: lastName declared but not used
./main.go:5:2: age declared but not used
```

请记住，对于在 Go 中声明的每个变量，你必须将其用于某处。

# 基本数据类型

Go 是一种强类型语言。 这意味着你声明的每个变量都绑定到特定的数据类型，并且只接受与此类型匹配的值。

Go 有四类数据类型：

- 基本类型：数字、字符串和布尔值
- 聚合类型：数组和结构
- 引用类型：指针、切片、映射、函数和通道
- 接口类型：接口

## 整数数字

一般来说，定义整数类型的关键字是 `int`。 但 Go 还提供了 `int8`、`int16`、`int32` 和 `int64` 类型，其大小分别为 8、16、32 或 64 位的整数。 

使用 32 位操作系统时，如果只是使用 `int`，则大小通常为 32 位。 在 64 位系统上，`int` 大小通常为 64 位。 但是，此行为可能因计算机而不同。 可以使用 `uint`。 但是，只有在出于某种原因需要将值表示为无符号数字的情况下，才使用此类型。 此外，Go 还提供 `uint8`、`uint16`、`uint32` 和 `uint64` 类型。

下面的示例演示如何在 Go 中使用各种整数类型：

```go
var integer8 int8 = 127
var integer16 int16 = 32767
var integer32 int32 = 2147483647
var integer64 int64 = 9223372036854775807
fmt.println(integer8, integer16, integer32, integer64)
```

大多数情况下，你将使用 `int`，但需要了解其他整数类型，因为在 Go 中，`int` 与 `int32` 不同，即使整数的自然大小为 32 位也是如此。 换句话说，需要强制转换时，你需要进行显式转换。 如果尝试在不同类型之间执行数学运算，将会出现错误。 例如，假定你具有下面的代码：

```go
var integer16 int16 = 127
var integer32 int32 = 32767
fmt.println(integer16 + integer32)
```

运行该程序时，你会收到以下错误：

```output
invalid operation: integer16 + integer32 (mismatched types int16 and int32)
```

在 Go 中将值从一种类型转换为另一种类型时，需要显式声明新类型。 

在学习 Go 过程中，你可能会收到有关 [runes](https://www.geeksforgeeks.org/rune-in-golang/) 的信息。 `rune` 只是 `int32` 数据类型的别名。 它用于表示 Unicode 字符（或 Unicode 码位）。 例如：

```go
rune := 'G'
fmt.println(rune)
```

运行前面的代码片段时，你可能会在命令提示符下看到程序打印符 `G`。 不过，你还会看到数字 `71`，它表示 `G` 的 Unicode 字符。 

## 浮点数字

Go 提供两种浮点数大小的数据类型：`float32` 和 `float64`。 

```go
var float32 float32 = 2147483647
var float64 float64 = 9223372036854775807
fmt.println(float32, float64)
```

你可以使用 `math` 包中提供的 `math.MaxFloat32` 和 `math.MaxFloat64` 常量来查找这两种类型的限制。 使用以下代码打印命令提示符中的最大浮点值：

```go
package main

import "math"

func main() {
    fmt.println(math.MaxFloat32, math.MaxFloat64)
}
```

当需要使用十进制数时，浮点类型也很有用：

```go
const e = 2.71828
const Avogadro = 6.02214129e23
const Planck = 6.62606957e-34
```

请注意，与前面的代码一样，Go 会从使用的值推断其数据类型。

## 布尔型

布尔类型仅可能有两个值：`true` 和 `false`。 你可以使用关键字 `bool` 声明布尔类型。 Go 不同于其他编程语言，在 Go 中，你不能将布尔类型隐式转换为 0 或 1。 你必须显式执行此操作。

因此，你可以按如下方式声明布尔变量：

```go
var featureFlag bool = true
```

## 字符串

在 Go 中，关键字 `string` 用于表示字符串数据类型。 若要初始化字符串变量，你需要在双引号（`"`）中定义值。 单引号（`'`）用于单个字符。

例如，声明和初始化字符串变量的两种方法：

```go
var firstName string = "John"
lastName := "Doe"
fmt.println(firstName, lastName)
```

有时，你需要对字符进行转义。 为此，在 Go 中，请在字符之前使用反斜杠 (`\`)。使用转义字符的最常见示例：

- `\n`：新行
- `\r`：回车符
- `\t`：选项卡
- `\'`：单引号
- `\"`：双引号
- `\\`：反斜杠

## 默认值

在 Go 中，如果你不对变量初始化，所有数据类型都有默认值。 

下面列出了我们目前浏览过类型的几个默认值：

- `int` 类型的 `0`（及其所有子类型，如 `int64`）
- `float32` 和 `float64` 类型的 `+0.000000e+000`
- `bool` 类型的 `false`
- `string` 类型的空值

运行以下代码片段以确认前面列出的默认值：

```go
var defaultInt int
var defaultFloat32 float32
var defaultFloat64 float64
var defaultBool bool
var defaultString string
fmt.println(defaultInt, defaultBool, defaultFloat32, defaultFloat64, defaultString)
```

你可以使用类似于此代码的代码来确定我们没有浏览到的数据类型默认值。

## 类型转换

在上一节中，我们确认在 Go 中隐式强制转换不起作用。 接下来，需要显式强制转换。 Go 提供了将一种数据类型转换为另一种数据类型的一些本机方法。 例如，一种方法是对每个类型使用内置函数，如下所示：

```go
var integer16 int16 = 127
var integer32 int32 = 32767
fmt.println(int32(integer16) + integer32)
```

Go 的另一种转换方法是使用 [strconv 包](https://golang.org/pkg/strconv/)。 例如，若要将 `string` 转换为 `int`，可以使用以下代码，反之亦然：

```go
package main

import "strconv"

func main() {
    i, _ := strconv.Atoi("-42")
    s := strconv.Itoa(-42)
    fmt.println(i, s)
}

<<-42 -42
```

注意，在前面的代码中，**有一个下划线 (`_`) 用作变量的名称。 在 Go 中，这意味着我们不会使用该变量的值，而是要将其忽略。** 否则，程序将不会进行编译，因为我们需要使用声明的所有变量。 

# 函数

## main 函数

与之交互的函数是 `main()` 函数。 Go 中的所有可执行程序都具有此函数，因为它是程序的起点。 你的程序中只能有一个 `main()` 函数。 如果创建的是 Go 包，则无需编写 `main()` 函数。 

你可能留意到，`main()` 函数没有任何参数，并且不返回任何内容。 但这并不意味着其不能从用户读取值，如命令行参数。 如要访问 Go 中的命令行参数，可以使用用于保存传递到程序的所有参数的 [os 包](https://golang.org/pkg/os/) 和 `os.Args` 变量来执行操作。

下面的代码从命令行读取两个数字，并为其求和：

```go
package main

import (
    "os"
    "strconv"
)

func main() {
    number1, _ := strconv.Atoi(os.Args[1])
    number2, _ := strconv.Atoi(os.Args[2])
    fmt.println("Sum:", number1+number2)
}
```

`os.Args` 变量包含传递给程序的每个命令行参数。 由于这些值的类型为 `string`，因此需要将它们转换为 `int` 以进行求和。

若要运行程序，请使用以下命令：

```go
go run main.go 3 5
```

输出如下：

```output
Sum: 8
```

## 自定义函数

下面是用于创建函数的语法：

```go
func name(parameters) (results) {
    body-content
}
```

使用 `func` 关键字来定义函数，然后为其指定名称。 在命名后，指定函数的参数列表。 你可以指定零个或多个参数。 你还可以定义函数的返回类型，该函数也可以是零个或多个。

重构上一节的代码，为自定义函数中的数字求和：

```go
package main

import (
    "os"
    "strconv"
)

func main() {
    sum := sum(os.Args[1], os.Args[2])
    fmt.println("Sum:", sum)
}

func sum(number1 string, number2 string) int {
    int1, _ := strconv.Atoi(number1)
    int2, _ := strconv.Atoi(number2)
    return int1 + int2
}
```

此代码创建一个名为 `sum` 的函数，该函数采用两个 `string` 参数，并将它们强制转换为 `int`，然后返回求和所得的结果。 定义返回类型时，函数需要返回该类型的值。

在 Go 中，你还可以为函数的返回值设置名称，将其当作一个变量。 例如，你可以重构如下 `sum` 函数：

```go
func sum(number1 string, number2 string) (result int) {
    int1, _ := strconv.Atoi(number1)
    int2, _ := strconv.Atoi(number2)
    result = int1 + int2
    return
}
```

请注意，你现在需要将函数的结果值括在括号中。 你还可以在函数中使用该变量，并且只需在末尾添加 `return` 行。 Go 将返回这些返回变量的当前值。 在函数末尾编写 `return` 关键字非常简单方便，尤其是在有多个返回值时。 但我们不建议采用这种方法，因为它不太清楚函数返回的内容。

## 返回多个值

在 Go 中，函数可以返回多个值。 

例如，假设你想要创建一个函数，将两个数字求和，又让它们相乘：

```go
func calc(number1 string, number2 string) (sum int, mul int) {
    int1, _ := strconv.Atoi(number1)
    int2, _ := strconv.Atoi(number2)
    sum = int1 + int2
    mul = int1 * int2
    return
}
```

你现在需要两个变量来存储函数的结果。（否则就不会进行编译）：

```go
func main() {
    sum, mul := calc(os.Args[1], os.Args[2])
    fmt.println("Sum:", sum)
    fmt.println("Mul:", mul)
}
```

Go 的另一个有趣功能是，如果不需要函数的某个返回值，可以通过将返回值分配给 `_` 变量来放弃该函数。 `_` 变量是 Go 忽略返回值的惯用方式。 它允许程序进行编译。 因此，如果只需要求和，则可以使用以下代码：

```go
func main() {
    sum, _ := calc(os.Args[1], os.Args[2])
    fmt.println("Sum:", sum)
}
```

## 更改函数参数值（指针）

 Go 是“按值传递”编程语言。 这意味着每次向函数传递值时，Go 都会使用该值并创建本地副本（内存中的新变量）。 在函数中对该变量所做的更改都不会影响你向函数发送的更改。

例如，假设你创建了一个用于更新人员姓名的函数：

```go
package main

func main() {
    firstName := "John"
    updateName(firstName)
    fmt.println(firstName)
}

func updateName(name string) {
    name = "David"
}
```

即使在函数中将该名称更改为 David，输出仍为 John。 由于 `updateName` 函数中的更改仅会修改本地副本，因此输出不会发生变化。 Go 传递变量的值，而不是变量本身。

如果你希望在 `updateName` 函数中进行的更改会影响 `main` 函数中的 `firstName` 变量，则需要使用指针。 *指针* 是包含另一个变量的内存地址的变量。 当你发送指向某个函数的指针时，不会传递值，而是传递地址内存。 因此，对该变量所做的每个更改都会影响调用方。

在 Go 中，有两个运算符可用于处理指针：

- `&` 运算符使用其后对象的地址。
- `*` 运算符取消引用指针。 也就是说，你可以前往指针中包含的地址访问其中的对象。

让我们修改前面的示例，以阐明指针的工作方式：

```go
package main

func main() {
    firstName := "John"
    updateName(&firstName)
    fmt.println(firstName)
}

func updateName(name *string) {
    *name = "David"
}
```

运行前面的代码。 请注意，输出现在显示的是 `David`，而不是 `John`。

首先要做的就是修改函数的签名，以指明你要接收指针。 为此，将参数类型从 `string` 更改为 `*string`。 （后者仍是字符串，但现在它是指向字符串的指针）。然后，将新值分配给该变量时，需要在该变量的左侧添加星号 (`*`) 以暂停该变量的值。 调用 `updateName` 函数时，系统不会发送值，而是发送变量的内存地址。 这就是前面的代码在变量左侧带有 `&` 符号的原因。

# 包

## main 包

在 Go 中，甚至最直接的程序都是包的一部分。 通常情况下，默认包是 `main` 包。 如果程序是 `main` 包的一部分，Go 会生成二进制文件。 运行该文件时，它将调用 `main()` 函数。

换句话说，当你使用 `main` 包时，程序将生成独立的可执行文件。 但当程序不是 `main` 包的一部分时，Go 不会生成二进制文件。 它生成包存档文件（扩展名为 .a 的文件）。

在 Go 中，包名称需遵循约定。 包使用其导入路径的最后一部分作为名称。 例如，Go 标准库包含名为 `math/cmplx` 的包，该包提供用于处理复数的有用代码。 此包的导入路径为 `math/cmplx`，导入包的方式如下所示：

```go
import "math/cmplx"
```

若要引用包中的对象，请使用包名称 `cmplx`，如下所示：

```go
cmplx.Inf()
```

## 创建包

在名为 `calculator` 的 `$GOPATH/src` 目录中创建新文件夹。 创建名为 `sum.go` 的文件。 树目录应如下列目录所示：

```output
src/
  calculator/
    sum.go
```

用包的名称初始化 `sum.go` 文件：

```go
package calculator
```

现在可以开始编写包的函数和变量。 不同于其他编程语言，Go 不会提供 `public` 或 `private` 关键字，以指示是否可以从包的内外部调用变量或函数。 但 Go 须遵循以下两个简单规则：

- 如需将某些内容设为专用内容，请以小写字母开始。
- 如需将某些内容设为公共内容，请以大写字母开始。

接下来，将以下代码添加到我们要创建的计算器包：

```go
package calculator

var logMessage = "[LOG]"

// Version of the calculator
var Version = "1.0"

func internalSum(number int) int {
    return number - 1
}

// Sum two integer numbers
func Sum(number1, number2 int) int {
    return number1 + number2
}
```

该代码中的一些事项：

- 只能从包内调用 `logMessage` 变量。
- 可以从任何位置访问 `Version` 变量。 建议你添加注释来描述此变量的用途。 （此描述适用于包的任何用户。）
- 只能从包内调用 `internalSum` 函数。
- 可以从任何位置访问 `Sum` 函数。 建议你添加注释来描述此函数的用途。

若要确认一切正常，可在 `calculator` 目录中运行 `go build` 命令。 如果执行此操作，请注意系统不会生成可执行的二进制文件。

## 创建模块

你已将计算器功能放入包中。 现在可以将包放到模块中。 Go 模块通常包含可提供相关功能的包。 包的模块还指定了 Go 运行你组合在一起的代码所需的上下文。 此上下文信息包括编写代码时所用的 Go 版本。

此外，模块还有助于其他开发人员引用代码的特定版本，并更轻松地处理依赖项。 另一个优点是，我们的程序源代码无需严格存在于 `$GOPATH/src` 目录中。 如果释放该限制，则可以更方便地在其他项目中同时使用不同包版本。

> Go module 构建模式为的是彻底解决 Go 项目复杂版本依赖的问题，在 Go 1.16 版本中，Go module 已经成为了 Go 默认的包依赖管理机制和 Go 源码构建机制。
>
> Go Module 的核心是一个名为 go.mod 的文件，在这个文件中存储了这个 module 对第三方依赖的全部信息。

因此，若要为 `calculator` 包创建模块，请在根目录 (`$GOPATH/src/calculator`) 中运行以下命令：

```output
go mod init github.com/myuser/calculator
```

运行此命令后，`github.com/myuser/calculator` 就会变成模块的名称。 在其他程序中，你将使用该名称进行引用。 命令还会创建一个名为 `go.mod` 的新文件。 最后，树目录现会如下列目录所示：

```output
src/
  calculator/
    go.mod
    sum.go
```

该文件的 `go.mod` 内容应该如下代码所示： （Go 版本可能不同。）

```output
module github.com/myuser/calculator

go 1.14
```

一个 module 就是一个包的集合，这些包和 module 一起打版本、发布和分发。

若要在其他程序中引用 `calculator` 包，需要使用模块名称进行导入。 在这种情况下，其名称为 `github.com/myuser/calculator`。 

## 引用本地包（模块）

现在，让我们了解如何使用此包。这一次，我们将使用之前在 `calculator` 包中创建的函数，而不是 `main` 包中的 `sum` 函数。

树文件结构现应如下所示：

```output
src/
  calculator/
    go.mod
    sum.go
  helloworld/
    main.go
```

我们会将此代码用于 `$GOPATH/src/helloworld/main.go` 文件：

```go
package main

import "github.com/myuser/calculator"

func main() {
    total := calculator.Sum(3, 5)
    fmt.println(total)
    fmt.println("Version: ", calculator.Version)
}
```

请注意，import 语句使用所创建包的名称：`calculator`。 若要从该包调用 `Sum` 函数，需要包含包名称，如 `calculator.Sum` 中一样。 最后，你现还可访问 `Version` 变量。 请按调用以下内容：`calculator.Version`。

如果立即尝试运行程序，它将不起任何作用。 你需要通知 Go，你会使用模块来引用其他包。 为此，请在 `$GOPATH/src/helloworld` 目录中运行以下命令：

```output
go mod init helloworld
```

在上述命令中，`helloworld` 是项目名称。 此命令会创建一个新的 `go.mod` 文件，因此，树目录会如下所示：

```output
src/
  calculator/
    go.mod
    sum.go
  helloworld/
    go.mod
    main.go
```

如果打开 `go.mod` 文件，则应看到类似于下面代码的内容： （Go 版本可能不同。）

```output
module helloworld

go 1.14
```

由于你引用的是该模块的本地副本，因此你需要通知 Go 不要使用远程位置。 因此，你需要手动修改 `go.mod` 文件，使其包含引用，如下所示：

```output
module helloworld

go 1.14

require github.com/myuser/calculator v0.0.0

replace github.com/myuser/calculator => ../calculator
```

`replace` 关键字指定使用本地目录，而不是模块的远程位置。 在这种情况下，由于 `helloworld` 和 `calculator` 程序在 `$GOPATH/src` 中，因此位置只能是 `../calculator`。 如果模块源位于不同的位置，请在此处定义本地路径。

> 如果不加require，会报`import_package.go:6:2: module github.com/myuser/calculator provides package github.com/myuser/calculator and is replaced but not required; to add it:
>         go get github.com/myuser/calculator`

使用以下命令运行程序：

```output
go run main.go
```

系统应输出如下内容：

```output
8
Version:  1.0
```

### 发布包

你可以轻松[发布 Go 包](https://github.com/golang/go/wiki/PackagePublishing)。 只需公开提供包源代码即可实现。 大多数开发人员都使用 GitHub 公开发布包。 这就是为什么有时会在 import 语句中找到对 `github.com` 的引用。

例如，如果想要将 `calculator` 包发布到 GitHub 帐户，则需要创建一个名为 `calculator` 的存储库。 URL 应与下述网址类似：

```output
https://github.com/myuser/calculator
```

你将通过标记存储库来对包进行版本化，如下所示：

```output
git tag v0.1.0
git push origin v0.1.0
```

如果是想要使用你的包的开发人员（包括你自己）引用如下所述内容：

```output
import "github.com/myuser/calculator"
```

让我们更详细地讨论如何引用第三方包。

## 引用外部（第三方）包

有时，程序需要引用其他开发人员编写的包。 你通常可以在 GitHub 上找到这些包。 不论你是要开发包（非 `main` 包）还是独立的程序（`main` 包），以下有关引用第三方包的说明均适用。

让我们添加对 `rsc.io/quote` 包的引用：

```go
package main

import (
    "github.com/myuser/calculator"
    "rsc.io/quote"
)

func main() {
    total := calculator.Sum(3, 5)
    fmt.println(total)
    fmt.println("Version: ", calculator.Version)
    fmt.println(quote.Hello())
}
```

如果使用 Visual Studio Code，则保存文件时将更新 `go.mod` 文件。 现在它的外观如下所示：

```output
module helloworld

go 1.14

require (
    github.com/myuser/calculator v0.0.0
    rsc.io/quote v1.5.2
)

replace github.com/myuser/calculator => ../calculator
```

请注意 `rsc.io/quote` 如何引用特定包版本。 如果需要升级程序的依赖项，则需要在此处更改版本。

使用以下命令再次运行程序：

```output
go run main.go
```

输出应如下所示：

```output
8
Version:  1.0
Hello, world.
```

日后对第三方包的所有引用都需要包含在 `go.mod` 文件中。 运行或编译应用程序时，Go 将下载其所有依赖项。

## go build编译引用三方包

Go 是一种编译型语言，这意味着只有你编译完 Go 程序之后，才可以将生成的可执行文件交付于其他人，并运行在没有安装 Go 的环境中。

**go run 这类命令更多用于开发调试阶段，真正的交付成果还是需要使用 go build 命令构建的。**

所以，go build引用包时与go run有区别。

[沿用上述例子](##引用外部（第三方）包)，go build时，会报错:

```
no required module provides package github.com/myuser/calculator: go.mod file not found in current directory or any parent directory; see 'go help modules'
```

Go 编译器提示源码依赖第三方包，但是 go.mod 中没有这个包的版本信息，我们需要按提示手工添加信息到 go.mod 中。

除了按提示手动添加外，我们也可以使用 go mod tidy 命令，让 Go 工具自动添加：

```
$go mod tidy       
go: downloading go.uber.org/zap v1.18.1
go: downloading github.com/valyala/fasthttp v1.28.0
go: downloading github.com/andybalholm/brotli v1.0.2
... ...
```

从输出结果中，我们看到 Go 工具不仅下载并添加了 hellomodule 直接依赖的 zap 和 fasthttp 包的信息，还下载了这两个包的相关依赖包。

`go mod tidy` 执行后，会更新 go.mod 的内容，go.mod 已经记录了 直接依赖的包的信息。

不仅如此，目录下还多了一个名为 go.sum 的文件，这个文件记录了直接依赖和间接依赖包的相关版本的 hash 值，用来校验本地包的真实性。在构建的时候，如果本地依赖包的 hash 值与 go.sum 文件中记录的不一致，就会被拒绝构建。

这次可以成功执行`go build`编译构建了。

# 数组

## 数组的排序(冒泡排序)

默认从小到大排序，比较相邻的两个数，如果左边的数值大于右边的数值，就进行交换。

```go
package main

import "fmt"

func main() {
    /*
    排序：从小到大(升序)，或者从大到小(降序)
        让数组中的数据按照从小到大排列。
     */
    arr1 := [5]int{15, 23, 8, 10, 7}
    fmt.Println(arr1) // 7 8 10 15 23

    for i:=1;i< len(arr1);i++{// i表示第几轮排序
        for j:=0;j<len(arr1) - i;j++ {
            if arr1[j] > arr1[j+1] {
                arr1[j], arr1[j+1] = arr1[j+1], arr1[j]
            }
        }
    }
    fmt.Println(arr1)


}
```

运行结果：

```
[15 23 8 10 7]
[7 8 10 15 23]
```

# 切片

切片是数组或另一个切片之上的数据结构。 我们将源数组或切片称为基础数组。 通过切片，可访问整个基础数组，也可仅访问部分元素。

切片只有 3 个组件：

- **指向基础数组中第一个可访问元素的指针**。 此元素不一定是数组的第一个元素 `array[0]`。
- **切片的长度**。 切片中的元素数目。
- **切片的容量**。 切片开头与基础数组结束之间的元素数目。

## 声明和初始化切片

```go
package main
import "fmt"

func main() {
    months := []string{"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"}
    fmt.Println(months)
    fmt.Println("Length:", len(months))
    fmt.Println("Capacity:", cap(months))
}
```

运行代码时，你会看到以下输出：

```output
[January February March April May June July August September October November December]
Length: 12
Capacity: 12
```

切片可以用内建函数 `make` 来创建，这也是你创建动态数组的方式。

`make` 函数会分配一个元素为零值的数组并返回一个引用了它的切片：

```go
a := make([]int, 5)  // len(a)=5
```

要指定它的容量，需向 `make` 传入第三个参数：

```go
b := make([]int, 0, 5) // len(b)=0, cap(b)=5
```

## 切片项

Go 支持切片运算符 `s[i:p]`，其中：

- `s` 表示数组。
- `i` 表示指向要添加到新切片的基础数组（或另一个切片）的第一个元素的指针。 变量 `i` 对应于数组 `array[i]` 中索引位置 `i` 处的元素。
- `p` 表示创建新切片时要使用的基础数组中的元素数目。 变量 `p` 对应于可用于新切片的基础数组中的最后一个元素。 可在位置 `array[i+1]` 找到基础数组中位置 `p` 处的元素。 

切片只能引用元素的子集。

假设你需要 4 个变量来表示一年的每个季度，并且你有一个包含 12 个元素的 `months` 切片。 下图演示了如何将 `months` 切片为 4 个新的 `quarter` 切片：

![显示多个切片在 Go 中的外观效果的图形。](images/显示多个切片在_Go_中的外观效果的图形。.png)

若要用代码表示在上图中看到的内容，可使用以下代码：

```go
package main
import "fmt"

func main() {
    months := []string{"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"}
    quarter1 := months[0:3]
    quarter2 := months[3:6]
    quarter3 := months[6:9]
    quarter4 := months[9:12]
    fmt.Println(quarter1, len(quarter1), cap(quarter1))
    fmt.Println(quarter2, len(quarter2), cap(quarter2))
    fmt.Println(quarter3, len(quarter3), cap(quarter3))
    fmt.Println(quarter4, len(quarter4), cap(quarter4))
}
```

运行代码时，你会获得以下输出：

```output
[January February March] 3 12
[April May June] 3 9
[July August September] 3 6
[October November December] 3 3
```

请注意，切片的长度不变，但容量不同。 

我们来了解 `quarter2` 切片。 声明此切片时，你指出希望切片从位置编号 3 开始，最后一个元素位于位置编号 6。 切片长度为 3 个元素，但容量为 9，原因是基础数组有更多元素或位置可供使用，但对切片而言不可见。 

例如，如果你尝试打印类似 `fmt.Println(quarter2[3])` 的内容，会出现以下错误：`panic: runtime error: index out of range [3] with length 3`。

切片容量仅指出切片可扩展的程度。 因此，你可从 `quarter2` 创建扩展切片，如下例所示：

```go
package main
import "fmt"

func main() {
    months := []string{"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"}
    quarter2 := months[3:6]
    quarter2Extended := quarter2[:4]
    fmt.Println(quarter2, len(quarter2), cap(quarter2))
    fmt.Println(quarter2Extended, len(quarter2Extended), cap(quarter2Extended))
}
```

运行上述代码时，你会获得以下输出：

```output
[April May June] 3 9
[April May June July] 4 9
```

请注意在声明 `quarter2Extended` 变量时，无需指定初始位置 (`[:4]`)。 执行此操作时，Go 会假定你想要切片的第一个位置。 你可对最后一个位置 (`[1:]`) 执行相同的操作。 Go 将假定你要引用所有元素，直到切片的最新位置 (`len()-1`)。

## 追加项

切片与数组之间有何不同。 第一个区别是切片的大小不是固定的，而是动态的。 创建切片后，可向其添加更多元素，这样切片就会扩展。 

Go 提供了内置函数 `append(slice, element)`，便于你向切片添加元素。 将要修改的切片和要追加的元素作为值发送给该函数。 然后，`append` 函数会返回一个新的切片，将其存储在变量中。 对于要更改的切片，变量可能相同。

```go
package main

import "fmt"

func main() {
    var numbers []int
    for i := 0; i < 10; i++ {
        numbers = append(numbers, i)
        fmt.Printf("%d\tcap=%d\t%v\n", i, cap(numbers), numbers)
    }
}
```

运行上述代码时，你应会看到以下输出：

```output
0       cap=1   [0]
1       cap=2   [0 1]
2       cap=4   [0 1 2]
3       cap=4   [0 1 2 3]
4       cap=8   [0 1 2 3 4]
5       cap=8   [0 1 2 3 4 5]
6       cap=8   [0 1 2 3 4 5 6]
7       cap=8   [0 1 2 3 4 5 6 7]
8       cap=16  [0 1 2 3 4 5 6 7 8]
9       cap=16  [0 1 2 3 4 5 6 7 8 9]
```

此输出很有意思。 特别是对于调用 `cap()` 函数所返回的内容。 一切看起来都很正常，直到第 3 次迭代，此时容量变为 4，切片中只有 3 个元素。 在第 5 次迭代中，容量又变为 8，第 9 次迭代时变为 16。

当切片容量不足以容纳更多元素时，Go 的容量将翻倍。 它将新建一个具有新容量的基础数组。 无需执行任何操作即可使容量增加。 Go 会自动扩充容量。 需要谨慎操作。 有时，一个切片具有的容量可能比它需要的多得多，这样你将会浪费内存。

## 删除项

Go 没有内置函数用于从切片中删除元素。 可使用上述切片运算符 `s[i:p]` 来新建一个仅包含所需元素的切片。

例如，以下代码会从切片中删除元素：

```go
package main
import "fmt"

func main() {
    letters := []string{"A", "B", "C", "D", "E"}
    remove := 2

    if remove < len(letters) {
        fmt.Println("Before", letters, "Remove ", letters[remove])
	//TODO ...需要再多研究
        letters = append(letters[:remove], letters[remove+1:]...)

        fmt.Println("After", letters)
    }
}
```

运行上述代码时，你会获得以下输出：

```output
Before [A B C D E]
After [A B D E]
```

此代码会从切片中删除元素。 它用切片中的下一个元素替换要删除的元素，如果删除的是最后一个元素，则不替换。

另一种方法是创建切片的新副本。 

## 创建切片的副本

Go 具有内置函数 `copy(dst, src []Type)` 用于创建切片的副本。 你需要发送目标切片和源切片。 

```go
slice2 := make([]string, 3)
copy(slice2, letters[1:4])
```

为何要创建副本？ 更改切片中的元素时，基础数组将随之更改。 引用该基础数组的任何其他切片都会受到影响。 

使用下述代码确认切片指向数组，而你在切片中所做的每个更改都会影响基础数组。

```go
package main

import "fmt"

func main() {
    letters := []string{"A", "B", "C", "D", "E"}
    fmt.Println("Before", letters)

    slice1 := letters[0:2]
    slice2 := letters[1:4]

    slice1[1] = "Z"

    fmt.Println("After", letters)
    fmt.Println("Slice2", slice2)
}
```

运行上述代码时，你会看到以下输出：

```output
Before [A B C D E]
After [A Z C D E]
Slice2 [Z C D]
```

请注意对 `slice1` 所做的更改如何影响 `letters` 数组和 `slice2`。 可在输出中看到字母 B 已替换为 Z，它会影响指向 `letters` 数组的每个切片。

若要解决此问题，你需要创建一个切片副本，它会在后台生成新的基础数组。 可以使用以下代码：

```go
package main

import "fmt"

func main() {
    letters := []string{"A", "B", "C", "D", "E"}
    fmt.Println("Before", letters)

    slice1 := letters[0:2]

    slice2 := make([]string, 3)
    copy(slice2, letters[1:4])

    slice1[1] = "Z"

    fmt.Println("After", letters)
    fmt.Println("Slice2", slice2)
}
```

运行上述代码时，你会看到以下输出：

```output
Before [A B C D E]
After [A Z C D E]
Slice2 [B C D]
```

请注意 `slice1` 中的更改如何影响基础数组，但它并未影响新的 `slice2`。

# 碎片

## rune

在学习 Go 过程中，你可能会收到有关 [runes](https://www.geeksforgeeks.org/rune-in-golang/) 的信息。 `rune` 只是 `int32` 数据类型的别名。 它用于表示 Unicode 字符（或 Unicode 码位）。 例如，假设你具有以下代码：

Go复制

```go
rune := 'G'
fmt.println(rune)
```

运行前面的代码片段时，你可能会在命令提示符下看到程序打印符 `G`。 不过，你还会看到数字 `71`，它表示 `G` 的 Unicode 字符。 

## 显示变量类型

+ %T：显示变量类型
+ %v：打印数据

```go
var ToBe bool = false
fmt.Printf("Type: %T Value: %v\n", ToBe, ToBe)

<< Type: bool Value: false
```

