# C++ json解析器

## 项目功能

*   从json字符串中解析出全部键值对
*   将json对象按照层级缩进后打印
*   待完成：
    *   查找特定key的类型及内容
    *   修改特定key的值

## 运行环境

*   操作系统：Windows10
*   编译工具：VS2022，msvc

## 项目结构

*   json数据类型的表达格式

    *   JsonData ->所数据格式的祖宗类型
    *   JsonObj ->包含dict、list两种数据类型
    *   JsonStr ->字符串类型
    *   JsonNum ->数字类型
    *   JsonBool ->布尔类型
    *   JsonNull ->null类型

*   数据对象的实例化存储

    *   ……

*   字符串中单词的读取

    >   1.   遇见空白字符（空格、制表符、换行符，三者均小于0x21）
    >   2.   遇见""，会产生字符串，在检测该字符串后，再检测一个非空白字符，以确定该字符串作为key，还是value
    >   3.   遇见b、f、t，检测到确定值，则跳过该键值对，此处允许有错误字符，但不允许第一个字符错误
    >   4.   遇见数字，将直接读取该值的所有字符，调用库函数进行转换
    >   5.   **该单词的长度不允许超过1024个字符**

*   json单词的解析

    >   在读取一个单词后，会记录该单词的内容，数据类型，根据数据类型，会创建不同的对象。

## 执行流程

>   ```mermaid
>   graph TB
>   
>   A(begin)
>   B{read}
>   C(FAILD)
>   D{TYPE}
>   E(operate)
>   
>   A-->B
>   B-->|faild| C
>   B-->|success| D
>   
>   D-->|dict| E
>   D-->|list| E
>   D-->|key| E
>   D-->|str| E
>   D-->|bool| E
>   D-->|num| E
>   D-->|null| E
>   D-->|invalid| E
>   D-->|objend| E
>   D-->|end| E
>   
>   E-->B
>   ```
>
>   //