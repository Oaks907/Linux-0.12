
> 大兄弟们，周五了，赶紧看篇文章摸会鱼，帮忙对文章提提意见

> 本文依赖 openjdk 官方文档以及博客《Java8+特性知识体系详解》对JDK9对JDK17的新特性进行了整理。我在此文中主要是将一些晦涩的特性说明，进行了一些资料查证，转换为易懂的说明，同时增添一些测试用例帮助理解。此文能帮助你对新特性有个初入门的了解，具体的还需要亲身验证学习深入

# 前言

2017年9月后，Java 发布了 Java9 版本，至2023年9月，已经累计发布了13 个版本，对比 Java8 之前几年一个版本的迭代周期，这个发布速度无疑加快了很多。

另外在2023年4月20日，New Relic 最新发布了一份 “2023 年 Java 生态系统状况报告”。报告中显示 56% 的应用程序在生产中使用了 Java11 版本（高于2022年的 48% 与 2020年的 11% ），Java8 紧随其后，采用比例为33%（低于2022年的 46%）。

2023年，尽管 Java8 与 Java11 的采用率合计达到了 89%，但是 Java17 的的采用率却表现的足够亮眼，远远超过了Java11 出现时的情形，Java17 的采用率达到了 9% （较去年的不到1%，提升了8%）。

基于此，本次分享介绍下Java的版本更新周期以及 Java9 至 Java17 版本新特性。

![image.png](https://p6-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/2e1de91ab2ff407ea817ea8d57121e38~tplv-k3u1fbpfcp-jj-mark:0:0:0:0:q75.image#?w=1577&h=1098&s=264100&e=png&b=fbfbfb)
<p align=center>图-1：“2023 年 Java 生态系统状况报告”中近几年的 Java 版本使用情况</p>

# Java版本说明

从Java9开始，Java改变了之前以功能特性为导向的发布周期。而是转为固定时间间隔的火车发布模式（Release Train）。火车定时发车，赶不上这次车的乘客，就只能等下一班火车。Java的固定发布时间是每年的3月和9月。除了每年定期的两个版本之外，Java还引入了**LTS（Long Term Support）** 版本的概念。LTS表示长期支持版本，Ubuntu和NodeJS都有类似的概念。目前Java 8、Java 11、Java17是LTS版本。除了LTS版本之外的其他版本，都认为是在上一个LTS版本之上的小的功能改进。

两次发布的间隔只有6个月。对于一些改动比较大的功能来说，6个月的时间有些短了。因此，Java引入了预览功能的概念。一些改动会以预览功能的形式在某个版本加入，并在后续的版本中不断的更新，直到成为正式功能。当一个新功能在 Java 版本N中推出之后，就可以由开发人员来试用，并提供反馈。根据反馈的结果，该功能可以继续 Java 版本N+1中预览，直到 Java 版本 N+2 中稳定下来。比如，Record 记录类型，最早在 Java 14 中以预览功能的形式引入，经过 Java 15 的再次预览，在 Java 16 中成为正式功能。

| 版本               | 发布日期        | 最终免费公开更新时间                                                                                                                                                                                                   | 最后延伸支持日期                                                                   |
| ---------------- | ----------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | -------------------------------------------------------------------------- |
| JDK Beta         | 1995        | ?                                                                                                                                                                                                            | ?                                                                          |
| JDK 1.0          | 1996 年 1 月  | ?                                                                                                                                                                                                            | ?                                                                          |
| JDK 1.1          | 1997 年 2 月  | ?                                                                                                                                                                                                            | ?                                                                          |
| J2SE 1.2         | 1998 年 12 月 | ?                                                                                                                                                                                                            | ?                                                                          |
| J2SE 1.3         | 2000 年 5 月  | ?                                                                                                                                                                                                            | ?                                                                          |
| J2SE 1.4         | 2002 年 2 月  | 2008 年 10 月                                                                                                                                                                                                  | 2013 年 2 月                                                                 |
| J2SE 5.0         | 2004 年 9 月  | 2009 年 11 月                                                                                                                                                                                                  | 2015 年 4 月                                                                 |
| Java SE 6        | 2006 年 12 月 | 2013 年 4 月                                                                                                                                                                                                   | Oracle 于 2018 年 12 月停止更新 Azul 于 2026 年 12 月停止更新                            |
| Java SE 7        | 2011 年 7 月  | OpenJDK 于 2022 年 9 月停止更新（2015 年 5 月前由 Oracle 维护） Red Hat 于 2020 年 8 月停止更新 Azul 于 2022 年 9 月停止更新                                                                                                              | Oracle 于 2022 年 7 月停止更新 Red Hat 于 2020 年 6 月停止更新 Azul 于 2027 年 12 月停止更新    |
| Java SE 8 (LTS)  | 2014 年 3 月  | OpenJDK 目前由 Red Hat 维护 Oracle 于 2022 年 3 月停止更新（商用） Oracle 于 2030 年 12 月停止更新（非商用） Azul 于 2030 年 12 月停止更新 IBM Semeru 于 2026 年 5 月停止更新 Eclipse Adoptium 于 2026 年 5 月或之后停止更新 Amazon Corretto 于 2026 年 5 月或之后停止更新 | Oracle 于 2030 年 12 月停止更新 Red Hat 于 2026 年 11 月停止更新                         |
| Java SE 9        | 2017 年 9 月  | OpenJDK 于 2018 年 3 月停止更新                                                                                                                                                                                     | 不适用                                                                        |
| Java SE 10       | 2018 年 3 月  | OpenJDK 于 2018 年 9 月停止更新                                                                                                                                                                                     | 不适用                                                                        |
| Java SE 11 (LTS) | 2018 年 9 月  | OpenJDK 目前由 Red Hat 维护 Azul 于 2026 年 9 月停止更新 IBM Semeru 于 2024 年 10 月停止更新 Eclipse Adoptium 于 2024 年 10 月或之后停止更新 Amazon Corretto 于 2027 年 9 月或之后停止更新 微软于 2024 年 10 月或之后停止更新                                   | Oracle 于 2026 年 9 月停止更新 Azul 于 2026 年 9 月停止更新 Red Hat 于 2024 年 10 月停止更新    |
| Java SE 12       | 2019 年 3 月  | OpenJDK 于 2019 年 9 月停止更新                                                                                                                                                                                     | 不适用                                                                        |
| Java SE 13       | 2019 年 9 月  | OpenJDK 目前由 Azul 维护 Azul 于 2023 年 3 月停止更新                                                                                                                                                                    | 不适用                                                                        |
| Java SE 14       | 2020 年 3 月  | OpenJDK 于 2020 年 9 月停止更新                                                                                                                                                                                     | 不适用                                                                        |
| Java SE 15       | 2020 年 9 月  | OpenJDK 目前由 Azul 维护 Azul 于 2023 年 3 月停止更新                                                                                                                                                                    | 不适用                                                                        |
| Java SE 16       | 2021 年 3 月  | OpenJDK 于 2021 年 9 月停止更新                                                                                                                                                                                     | 不适用                                                                        |
| Java SE 17 (LTS) | 2021 年 9 月  | OpenJDK 目前由 SAP 维护 Azul 于 2029 年 9 月停止更新 IBM Semeru 于 2027 年 10 月停止更新 微软于 2027 年 9 月或之后停止更新 Eclipse Adoptium 于 2027 年 9 月或之后停止更新                                                                             | Oracle 于 2029 年 9 月或之后停止更新 Azul 于 2029 年 9 月停止更新 Red Hat 于 2027 年 10 月停止更新 |
| Java SE 18       | 2022 年 3 月  | OpenJDK 于 2022 年 9 月停止更新 Eclipse Adoptium 于 2022 年 9 月或之后停止更新                                                                                                                                                | 不适用                                                                        |
| Java SE 19       | 2022 年 9 月  | OpenJDK 于 2023 年 3 月停止更新                                                                                                                                                                                     | 不适用                                                                        |
| **Java SE 20**   | 2023 年 3 月  | Oracle 于 2023 年 9 月停止更新                                                                                                                                                                                      | 不适用                                                                        |

## 不同的Java发行版本

另外一个常见的困惑是为什么 Java 有这么多的发行版本，除了 OpenJDK 之外，还有 AdoptOpenJDK ？这其实也和 Java 版本更新方式的变化有关。

OpenJDK是一个开源项目，源代码就在GitHub上。但是对一般用户来说，需要的不是源代码，而是可以直接运行的二进制包。这就需要有相应的基础设施，负责对OpenJDK的构建，运行自动化测试，以及提供下载等。这些成本一直都是Oracle在承担。出于一些原因，Oracle不再提供对OpenJDK的LTS版本的安全更新支持。Oracle对OpenJDK的发布版本只提供6个月的支持，也就是到下一个版本发布为止。这6个月的支持包括两个按季度的安全更新。以 Java15 为例，只有最初发布的 15 版本，以及后续的两个安全更新版本 15.0.1 和 15.0.2。之后就不再有 15 版本的更新。如果要获取更新，只能升级到 Java 16。对于非LTS版本来说，这样还算合理。但是对LTS版本来说，不提供持续的安全更新是很大的问题。要获取更新，必须使用Oracle JDK。Oracle JDK 对个人用户是免费的，对商业用户是收费的。绝大多数人是不想掏钱的。因此，有很多的社区和公司就承担了提供LTS版本的更新的职责，就形成了非常多的Java发行版本。

## 火车发布模式的优缺点

优点：

1.  新特性和改进：Java的版本更新频繁，每个版本都包含了大量的新特性和改进，这可以提高Java应用程序的性能、安全性和可靠性。

2.  反馈和改进：Java的版本更新频繁，这可以让Java开发者和Java平台提供商更快地获取用户反馈，并进行改进和优化。

3.  生态系统健康：Java的版本更新频繁，这可以保持Java生态系统的健康和活力，吸引更多的开发者和用户参与到Java生态系统中来。

4.  技术更新：Java的版本更新频繁，这可以让Java开发者和Java平台提供商更快地掌握新的技术和趋势，保持技术的领先优势。

5.  社区活跃：Java的版本更新频繁，这可以促进Java社区的活跃和发展，让Java开发者和用户更加紧密地联系在一起。

缺点：

1.  版本更新频繁：Java的版本更新比较频繁，每年都会发布新的版本，这会给Java开发者带来一定的学习成本和适应压力。

2.  兼容性问题：由于Java的版本更新比较频繁，不同版本之间可能存在兼容性问题，这会给Java开发者带来一定的麻烦。

3.  集成测试难度大：Java的版本更新频繁，每个版本都包含了大量的新特性和改进，这会给Java开发者带来集成测试的难度，需要进行大量的测试和调试工作。

4.  稳定性问题：Java的版本更新频繁，每个版本都可能存在一些新的bug，这会影响Java应用程序的稳定性和可靠性。

5.  维护成本高：Java的版本更新频繁，每个版本都需要进行维护和支持，这会给Java开发者和Java平台提供商带来一定的成本压力。

总结：

在 JDK版本的选择上，如果是学习目的，那么无脑最新版。如果是生产需要，尽量选择长期维护的版本，不要使用最新版本的。因为新版本的 JDK，新功能没有经过生产环境的验证，如果想成为第一个吃螃蟹的人，一定要三思而后行。

# Java 9

## Jigsaw 模块系统

什么是模块系统？官方的定义是 A uniquely named, reusable group of related packages, as well as resources (such as images and XML files) and a module descriptor.

模块的载体是 jar 文件，一个模块就是一个 jar 文件，但相比于传统的 jar 文件，模块的根目录下多了一个 module-info.java 文件，即 module descriptor。 module descriptor 包含以下信息：

*   模块名称
*   依赖哪些模块
*   导出模块内的哪些包（允许直接 import 使用）
*   开放模块内的哪些包（允许通过 Java 反射访问）
*   提供哪些服务
*   依赖哪些服务


![image.png](https://p9-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/b2f8136fb46345f6985f5aeed1d69269~tplv-k3u1fbpfcp-jj-mark:0:0:0:0:q75.image#?w=1532&h=640&s=293563&e=png&a=1&b=c6dbed)
<p align=center>图-2：Java9 module</p>

Jigsaw 模块系统使用示例：

项目A中 module-info.java:
```java
    // 模块A 对外暴露了包org.a.example.a1与org.a.example.a2路径的类， 其中org.a.example.a2包指定仅能模块B可用
    module A {
        exports org.a.example.a2; // 模块 A 对外暴露org.a.example.a2中的内容
        exports org.a.example.a1 to B; // 模块 A 仅对模块 B 暴露org.a.example.a1中的内容
    }
```

项目B中 module-info.java：
```java
    // 模块B依赖模块A，能使用 org.a.example.a1 与 org.a.example.a2 下的类
    module B {
        requires A;
    }
```

项目C中 module-info.java：
```java
    // 模块C依赖模块A，其只能使用包org.a.example.a2下的类
    module C {
        requires A;
    }
```

![image.png](https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/fb7d38e6f9d34c3d9ea828dbed3044f4~tplv-k3u1fbpfcp-jj-mark:0:0:0:0:q75.image#?w=400&h=500&s=101221&e=png&b=2c313a)

<p align=center>图-3 模块系统使用示例</p>


模块系统的优点：

1、原生的依赖管理。有了模块系统，Java 可以根据 module descriptor 计算出各个模块间的依赖关系，具有了在编译期和执行期加以识别模块之间依赖项的能力。系统可以通过这些依赖项确保所有模块的子集合能满足程序的需求

2、精简 JRE 。引入模块系统之后，JDK 自身被划分为 94 个模块。通过 Java 9 新增的 jlink 工具，开发者可以根据实际应用场景随意组合这些模块，去除不需要的模块，生成自定义 JRE，从而有效缩小 JRE 大小。得益于此，JRE 11 的大小仅为 JRE 8 的 53%，从 218.4 MB缩减为 116.3 MB，JRE 中广为诟病的巨型 jar 文件 rt.jar 也被移除。更小的 JRE 意味着更少的内存占用，这让 Java 对嵌入式应用开发变得更友好。


![image.png](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/fc68962ac80145db8ec26f08b3e68adf~tplv-k3u1fbpfcp-jj-mark:0:0:0:0:q75.image#?w=1822&h=950&s=492078&e=png&b=fdfcfc)

<p align=center>图-4：Modular JDK</p>

3、更好的兼容性。自打 Java 出生以来，就只有 4 种包可见性，这让 Java 对面向对象的三大特征之一封装的支持大打折扣。Java 9 之后，利用 module descriptor 中的 exports 关键词，模块维护者就精准控制哪些类可以对外开放使用，哪些类只能内部使用，换句话说就是不再依赖文档，而是由编译器来保证。


![image.png](https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/d77f27c3a5544757a0057ae941f12c7f~tplv-k3u1fbpfcp-jj-mark:0:0:0:0:q75.image#?w=1810&h=690&s=378547&e=png&b=fefefe)

<p align=center>图-5：Java可访问级别</p>

4、提升 Java 语言开发效率。Java 9 之后，JDK 被拆分为 94 个模块，每个模块有清晰的边界和独立的单元测试，对于每个 Java 语言的开发者而言，每个人只需要关注其所负责的模块，开发效率因此大幅提升。

## String 优化

JDK9 之前的库的 String 类的实现使用了 char 数组来存放字符串，char 占用16位，即两字节。而在Java 9中，String类引入了一种称为"Compact Strings"的新实现方式，将字符串的表示方式从char数组改为byte数组，根据情况使用Latin1编码将Unicode字符映射到一个或两个字节的表示。这种实现方式可以大大减少内存使用，尤其是对于包含大量ASCII字符的字符串。


![image.png](https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/00bf3928b4454804874996f19cf7aa58~tplv-k3u1fbpfcp-jj-mark:0:0:0:0:q75.image#?w=1632&h=636&s=153705&e=png&b=e2ebf8)

<p align=center>图-6：Java字符串结构变化</p>

Latin1编码在ASCII字符集的基础上进行了扩展，它使用单字节编码方式，也就是说最多只能表示256个字母或符号，并且前128个和ASCII完全吻合。Latin1编码保存字符串示例如下：

![image.png](https://p9-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/6ec8ac7032bf4a92aab1a64bec35b037~tplv-k3u1fbpfcp-jj-mark:0:0:0:0:q75.image#?w=856&h=299&s=33817&e=png&b=fefefe)

<p align=center>图-7：Java9 不同编码格式下保存字符串 abcd 示例</p>

Java9中对于符合 Latin1 编码的字符，会使用 byte 数组进行保存，并且标记编码格式coder为LATIN1。

Java9字符串保存逻辑源码如下：
```java
        /*
         * Package private constructor. Trailing Void argument is there for
         * disambiguating it against other (public) constructors.
         *
         * Stores the char[] value into a byte[] that each byte represents
         * the8 low-order bits of the corresponding character, if the char[]
         * contains only latin1 character. Or a byte[] that stores all
         * characters in their byte sequences defined by the {@code StringUTF16}.
         */
        String(char[] value, int off, int len, Void sig) {
            if (len == 0) {
                this.value = "".value;
                this.coder = "".coder;
                return;
            }
            if (COMPACT_STRINGS) { // 判断能否使用 Latin1 编码保存字符串 
                byte[] val = StringUTF16.compress(value, off, len);
                if (val != null) {
                    this.value = val;
                    this.coder = LATIN1;
                    return;
                }
            }
            this.coder = UTF16;
            this.value = StringUTF16.toBytes(value, off, len);
        }
```

## JShell REPL

JShell 是 Java 9 新增的一个交互式的编程环境工具。它允许你无需使用类或者方法包装来执行 Java 语句。它与 Python 的解释器类似，可以直接输入表达式并查看其执行结果。

![image.png](https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/892b6054712044e2a4154f70e25e1625~tplv-k3u1fbpfcp-jj-mark:0:0:0:0:q75.image#?w=1848&h=852&s=135782&e=png&b=0d2a35)

<p align=center>图-8 JShell REPL 使用示例</p>

## 集合、Stream 和 Optional

*   集合

    *   不可变集合 ：`List.of()`、`Set.of()`、`Map.of()`、`Map.ofEntries()` 等方法

*   Stream

    *   **takeWhile**：`default Stream<T> takeWhile(Predicate\<? super T> predicate)`

        返回从开头开始的尽量多的元素。

    *   **dropWhile**：`default Stream<T> dropWhile(Predicate\<? super T> predicate)`

        和 takeWhile 作用相反的，使用一个断言作为参数，直到断言语句第一次返回 false 才返回给定 Stream 的子集

    *   **iterate**：`static <T> Stream<T> iterate(T seed, Predicate\<? super T> hasNext, UnaryOperator<T> next)`

        方法允许使用初始种子值创建顺序（可能是无限）流，并迭代应用指定的下一个方法。 当指定的 hasNext 的 predicate 返回 false 时，迭代停止

takeWhile、dropWhile、iterate 示例如下：
```java
public class Test {
    public static void main(String[] args) {
        final List<String> list1 = Stream.of("a", "b", "c", "", "e", "f")
                .takeWhile(s -> !s.isBlank())
                .collect(Collectors.toList());
        System.out.println(list1);

        final List<String> list2 = Stream.of("a", "b", "c", "", "e", "f")
                .dropWhile(s -> !s.isBlank())
                .collect(Collectors.toList());
        System.out.println(list2);

        final List<Integer> list3 = IntStream.iterate(3, x -> x < 10, x -> x + 3).boxed().collect(Collectors.toList());
        System.out.println(list3);
    }
}
```
执行结果：
```text
[a, b, c]
[, e, f]
[3, 6, 9]
```

*   Optional

*   **Optional.ifPresentOrElse()**：`public void ifPresentOrElse(Consumer\<? super T> action, Runnable emptyAction)`

        如果一个 Optional 包含值，则对其包含的值调用函数 action，即 action.accept(value)，这与 ifPresent 一致； 如果 Optional 不包含值，那么 ifPresentOrElse 便会调用 emptyAction，即 emptyAction.run()。

ifPresentOrElse 示例如下：
```java
            public class Main {
                public static void main(String[] args) {
                    final Optional<Object> notEmptyOpt = Optional.of(1);
                    notEmptyOpt.ifPresentOrElse(o -> {
                        System.out.println("value is not null. value is " + o);}, () -> System.out.println("Value is empty."));

                    final Optional<Object> emptyOpt = Optional.empty();
                    emptyOpt.ifPresentOrElse(o -> {
                        System.out.println("value is not null" + o);}, () -> System.out.println("Value is empty."));
                }
            }
```

执行结果：

```text
value is not null. value is 1
Value is empty.
```


*   **Optional.or()**：`public Optional<T> or(Supplier\<? extends Optional\<? extends T>> supplier)`

        如果值存在，返回 Optional 指定的值，否则返回一个预设的值。

or 示例如下：
```java
            public class Tester {

                public static void main(String[] args) {
                    Optional<String> optional1 = Optional.of("Mahesh");

                    Supplier<Optional<String>> supplierString = () -> Optional.of("Not Present");
                    optional1 = optional1.or( supplierString);
                    optional1.ifPresent( x -> System.out.println("Value: " + x));

                    optional1 = Optional.empty();
                    optional1 = optional1.or( supplierString);
                    optional1.ifPresent( x -> System.out.println("Value: " + x));
                }
            }
```
执行结果：
```text
Value: Mahesh
Value: Not Present
```

*  **Optional.stream()**：`public Stream<T> stream()将 Optional 转为一个 Stream，如果该 Optional 中包含值，那么就返回包含这个值的 Stream，否则返回一个空的 Stream`

stream 示例如下：
```java
public class Tester {
                public static void main(String[] args) {
                    List<Optional<String>> list = Arrays.asList (
                            Optional.empty(),
                            Optional.of("A"),
                            Optional.empty(),
                            Optional.of("B"));
                    
                    List<String> filteredListJava9 = list.stream()
                            .flatMap(Optional::stream)
                            .collect(Collectors.toList());

                    System.out.println(filteredListJava9);
                }
}
```
执行结果：
```text
[A, B]
```


## 接口支持私有方法与私有静态方法

从Java 9开始，我们可以在Interface接口中添加private的私有方法和私有静态方法，这些私有方法将改善接口内部的代码可重用性。

示例如下：
```java
    interface Test {

        public default void defaultMethod() {
            privateMethod(); 
            privateStaticMethod(); 
            System.out.println("普通方法被调用");
        }

        public static void staticMethod() {
            privateStaticMethod();
            System.out.println("静态方法被调用");
        }

        // private的私有方法
        private void privateMethod() {
            System.out.println("private私有方法被调用");
        }
    		// private的静态私有方法
        private static void privateStaticMethod() {
            System.out.println("private私有静态方法被调用");
        }
    }
```

## 进程API

在 Java5 之前，生成新进程的唯一方法是使用该Runtime.getRuntime().exec()方法。然后在 Java5 中，ProcessBuilder引入了 API，它支持一种更简洁的方式来生成新进程。现在，Java9 添加了一种获取有关当前进程和任何衍生进程的信息的新方法。支持的功能包括：

*   从Java代码中创建新的本地进程
*   获取本地进程的进程句柄，无论它们是由Java代码还是通过其他方式创建
*   销毁运行进程
*   查询活动的进程及其属性
*   获取进程的子进程和父进程的列表
*   获取本地进程的进程ID（PID）
*   获取新创建的进程的输入，输出和错误流
*   等待进程终止
*   当进程终止时执行任务

示例如下：
```java
public class Main {

    public static void main(String[] args) {
        // 获取进程句柄
        ProcessHandle current = ProcessHandle.current();
        printInfo(current);
    }
    public static void printInfo(ProcessHandle handle) {

        long pid = handle.pid();
        boolean isAlive = handle.isAlive();

        ProcessHandle.Info info = handle.info();
        String command = info.command().orElse("");
        String[] args = info.arguments().orElse(new String[]{});
        String commandLine = info.commandLine().orElse("");
        ZonedDateTime startTime = info.startInstant().orElse(Instant.now()).atZone(ZoneId.systemDefault());
        Duration duration = info.totalCpuDuration().orElse(Duration.ZERO);
        String owner = info.user().orElse("Unknown");
        long childrenCount = handle.children().count();

        // 进程ID
        System.out.printf("PID: %d%n", pid);
        // 进程是否存活
        System.out.printf("IsAlive: %b%n", isAlive);
        // 可执行路径名
        System.out.printf("Command: %s%n", command);
        // 启动参数
        System.out.printf("Arguments: %s%n", Arrays.toString(args));
        // 启动命令
        System.out.printf("CommandLine: %s%n", commandLine);
        // 启动时间
        System.out.printf("Start Time: %s%n", startTime);
        // 进程占用CPU时间
        System.out.printf("CPU Time: %s%n", duration);
        // 进程的拥有者
        System.out.printf("Owner: %s%n", owner);
        // 子进程数量
        System.out.printf("Children Count: %d%n", childrenCount);
    }
}
```
执行结果：

```text
PID: 67970
IsAlive: true
Command: /Users/lihaifei/Library/Java/JavaVirtualMachines/corretto-17.0.7/Contents/Home/bin/java
Arguments: [-javaagent:/Applications/IntelliJ IDEA.app/Contents/lib/idea_rt.jar=54903:/Applications/IntelliJ IDEA.app/Contents/bin, -Dfile.encoding=UTF-8, -classpath, /Users/lihaifei/meituan/java_test/java_9_test_d/target/classes, org.example.Main]
CommandLine: /Users/lihaifei/Library/Java/JavaVirtualMachines/corretto-17.0.7/Contents/Home/bin/java -javaagent:/Applications/IntelliJ IDEA.app/Contents/lib/idea_rt.jar=54903:/Applications/IntelliJ IDEA.app/Contents/bin -Dfile.encoding=UTF-8 -classpath /Users/lihaifei/meituan/java_test/java_9_test_d/target/classes org.example.Main
Start Time: 2023-07-05T17:09:08.031+08:00[Asia/Shanghai]
CPU Time: PT0.182734S
Owner: lihaifei
Children Count: 0
```

## 平台日志API和服务

Java 9 允许为 JDK 和应用配置同样的日志实现

## 改进@Deprecated注解

增加注解参数 since 和 forRemoval

*   since: 元素指定已注解的API元素已被弃用的版本
*   forRemoval: 元素表示注解的 API 元素在将来的版本中被删除，应该迁移 API

```java
@Deprecated(since="9", forRemoval = true)
public Integer(int value) {
    this.value = value;
}
```



## I/O流新特性

*   readAllBytes：读取 InputStream 中的所有剩余字节。
*   readNBytes：从 InputStream 中读取指定数量的字节到数组中。
*   transferTo：读取 InputStream 中的全部字节并写入到指定的 OutputStream 中 。

# Java10

## 局部变量类型推断

Java 10 增加了局部类型推断（Local-Variable Type Inference）功能，这让 Java 可以像 Js 里的 var 或者其他语言的 auto 一样可以自动推断数据类型。这其实只是一个新的语法糖，底层并没有变化，在编译时就已经把 var 转化成具体的数据类型了，这样可以减少代码编写。

示例如下：
```java
    public class Main {
        public static void main(String[] args) {
            var list = List.of(1, 2, 3, 4, 5);
            var str = "Current value:";

            for (var integer : list) {
                System.out.println(str + integer);
            }
        }
    }
```
## 整合JDK代码仓库

在已发布的 Java 版本中，JDK 的整套代码根据不同功能已被分别存储在多个 Mercurial 版本控制存储库，这八个 Mercurial 存储库分别是：root、corba、hotspot、jaxp、jaxws、jdk、langtools、nashorn

虽然以上八个存储库之间相互独立以保持各组件代码清晰分离，但同时管理这些存储库存在许多缺点，并且无法进行相关联源代码的管理操作。其中最重要的一点是，涉及多个存储库的变更集无法进行原子提交 （atomic commit）。例如，如果一个 bug 修复时需要对独立存储两个不同代码库的代码进行更改，那么必须创建两个提交：每个存储库中各一个。这种不连续性很容易降低项目和源代码管理工具的可跟踪性和加大复杂性，特别是相互依赖的变更集的存储库执行原子提交。

为了解决这个问题，JDK 10 中将所有现有存储库合并到一个 Mercurial 存储库中。

## 适用于G1的多线程完全垃圾回收

之前 Java 版本中的 G1 垃圾回收器执行 GC 时采用的是基于单线程标记扫描压缩算法（mark-sweep-compact）。为了最大限度地减少 Full GC 造成的应用停顿的影响，Java 10 中将为 G1 引入多线程并行 GC，同时会使用与年轻代回收和混合回收相同的并行工作线程数量，从而减少了 Full GC 的发生，以带来更好的性能提升、更大的吞吐量。

具体并行 GC 线程数量可以通过： -XX：ParallelGCThreads 参数来调节，但这也会影响用于年轻代和混合收集的工作线程数。

## 应用程序类数据共享

在 Java 5 中就已经引入了类数据共享机制 (Class Data Sharing，简称 CDS)，允许将一组类预处理为共享归档文件，以便在运行时能够进行内存映射以减少 Java 程序的启动时间，当多个 Java 虚拟机（JVM）共享相同的归档文件时，还可以减少动态内存的占用量，同时减少多个虚拟机在同一个物理或虚拟的机器上运行时的资源占用。简单来说，Java 安装程序会把 rt.jar 中的核心类提前转化成内部表示，转储到一个共享存档（shared archive）中。多个 Java 进程（或者说 JVM 实例）可以共享这部分数据。为改善启动和占用空间，Java 10 在现有的 CDS 功能基础上再次拓展，以允许应用类放置在共享存档中。

CDS 特性在原来的 bootstrap 类基础之上，扩展加入了应用类的 CDS (Application Class-Data Sharing) 支持。

其原理为：在启动时记录加载类的过程，写入到文本文件中，再次启动时直接读取此启动文本并加载。设想如果应用环境没有大的变化，启动速度就会得到提升。可以想像为类似于操作系统的休眠过程，合上电脑时把当前应用环境写入磁盘，再次使用时就可以快速恢复环境。

## Thread-Local Handshakes 线程本地握手

允许线程在执行某些操作时不再需要进入 safepoints，而 safepoints需要在特定时间点停止JVM中的所有线程。通过 ThreadLocal Handshake 机制，允许在线程本地执行就行了，而不用必须要求所有的线程都达到安全点。

增加的参数为：-XX:ThreadLocalHandshakes (默认为开启)，将允许用户在支持的平台上选择安全点。

## 统一的垃圾回收接口

在当前的 Java 结构中，组成垃圾回收器（GC）实现的组件分散在代码库的各个部分。尽管这些惯例对于使用 GC 计划的 JDK 开发者来说比较熟悉，但对新的开发人员来说，对于在哪里查找特定 GC 的源代码，或者实现一个新的垃圾收集器常常会感到困惑。更重要的是，随着 Java modules 的出现，我们希望在构建过程中排除不需要的 GC，但是当前 GC 接口的横向结构会给排除、定位问题带来困难。

为解决此问题，需要整合并清理 GC 接口，以便更容易地实现新的 GC，并更好地维护现有的 GC。

Java 10 中，hotspot/gc 代码实现方面，引入一个干净的 GC 接口，改进不同 GC 源代码的隔离性，多个 GC 之间共享的实现细节代码应该存在于辅助类中。这种方式提供了足够的灵活性来实现全新 GC 接口，同时允许以混合搭配方式重复使用现有代码，并且能够保持代码更加干净、整洁，便于排查收集器问题。

## 更多的Unicode语言标签扩展

Java10 中扩展了Unicode语言标签，扩展类型如下：

*   cu（货币类型）

<!---->

*   fw（星期几的第一天）

<!---->

*   rg（区域覆盖）

<!---->

*   tz（时区）

为了支持扩展，将对以下API进行更新：

*   java.text.DateFormat :: get \*Instance 将基于扩展ca，rg和/或tz返回实例
*   java.text.DateFormatSymbols :: getInstance 将基于扩展rg返回实例
*   java.text.DecimalFormatSymbols :: getInstance 将根据扩展名rg返回实例
*   java.text.NumberFormat :: get\* 实例将根据扩展名nu和/或rg返回实例
*   java.time.format.DateTimeFormatter :: localizedBy将根据扩展名ca，rg和/或tz返回DateTimeFormatter实例
*   java.time.format.DateTimeFormatterBuilder :: getLocalizedDateTimePattern将根据rg扩展名返回模式字符串。
*   java.time.format.DecimalStyle :: of 将根据扩展名nu和/或rg返回DecimalStyle实例
*   java.time.temporal.WeekFields :: of 将基于扩展fw和/或rg返回WeekFields实例
*   java.util.Calendar :: {getFirstDayOfWeek，getMinimalDaysInWeek} 将根据扩展fw和/或rg返回值
*   java.util.Currency :: getInstance 将根据扩展名cu和/或rg返回Currency实例
*   java.util.Locale :: getDisplayName 将返回一个包含这些U扩展名的显示名称的字符串
*   java.util.spi.LocaleNameProvider 将为这些U扩展的键和类型提供新的SPI

```java
    public class Main {
        public static void main(final String[] args) {
            Date date = new Date();
            DateFormat df = DateFormat.getDateInstance(DateFormat.FULL, Locale.CHINA);
            DateFormat df2 = DateFormat.getDateInstance(DateFormat.FULL, Locale.US);
            System.out.println(df.format(date));
            System.out.println(df2.format(date));
        }
    }
```
执行结果：
```text
2023年7月10日星期一
Monday, July 10, 2023
```

## 基于 Java 的 实验性 JIT 编译器

Java 10 中开启了基于 Java 的 JIT 编译器 Graal，并将其用作 Linux/x64 平台上的实验性 JIT 编译器开始进行测试和调试工作，另外 Graal 将使用 Java 9 中引入的 JVM 编译器接口（JVMCI）。

Graal 是一个以 Java 为主要编程语言、面向 Java bytecode 的编译器。与用 C++实现的 C1 及 C2 相比，它的模块化更加明显，也更加容易维护。Graal 既可以作为动态编译器，在运行时编译热点方法；亦可以作为静态编译器，实现 AOT 编译。在 Java 10 中，Graal 作为试验性 JIT 编译器一同发布（JEP 317）。将 Graal 编译器研究项目引入到 Java 中，或许能够为 JVM 性能与当前 C++ 所写版本匹敌（或有幸超越）提供基础。

Java 10 中默认情况下 HotSpot 仍使用的是 C2 编译器，要启用 Graal 作为 JIT 编译器，请在 Java 命令行上使用以下参数：

**-XX：+ UnlockExperimentalVMOptions -XX：+ UseJVMCICompiler**

## 基于时间的版本发布模式

Java 10 中将重新编写之前 JDK 版本中引入的版本号方案，将使用基于时间模型定义的版本号格式来定义新版本。使开发人员或终端用户能够轻松找出版本的发布时间，以便开发人员能够判断是否将其升级到具有最新安全修补程序或可能的附加功能的新版本。版本号格式如下：

$FEATURE.$INTERIM.$UPDATE.$PATCH

*   **\$FEATURE**：每次版本发布加 1，不考虑具体的版本内容。2018 年 3 月的版本是 JDK 10，9 月的版本是 JDK 11，依此类推。
*   **\$INTERIM**：中间版本号，在大版本中间发布的，包含问题修复和增强的版本，不会引入非兼容性修改。因为新版本发布模型不包含临时中间版本，这里始终为0
*   **\$UPDATE:** 更新版本计数，针对修复新功能中的安全问题、回归和错误的兼容更新版本而递增
*   **\$PATCH:** 紧急补丁发布计数，仅在需要生成紧急版本来解决关键问题时才会增加。

# Java11

## 基于嵌套关系的访问控制优化

Java11之前，java通过桥接方法来实现嵌套关系的内部互相访问，但是这种实现方式会造成源码与编译结果访问控制权限不一致。为解决这一问题，保持Java 语言中现有的嵌套类型概念一致, Java11 进行了优化，允许逻辑上属于同一代码实体，但被编译之后分为多个分散的 class 文件的类，无需编译器额外的创建可扩展的桥接访问方法，即可访问彼此的私有成员，并且这种改进是在 Java 字节码级别的。

示例如下：
```java
public class Outer {
        public void print1() {
            System.out.println("print1");
        }

        private void print11() {
            System.out.println("print11");
        }


        public void callInnerMethod() {
            System.out.println("callInnerMethod");
            final Inner inner = new Inner();
            inner.print4();
        }

       
        public void callInnerReflectionMethod()
                throws InvocationTargetException, NoSuchMethodException, IllegalAccessException {
            final Inner inner = new Inner();
            inner.callOuterPrivateMethod(this);
        }

        public class Inner {

            public void print3() {
                System.out.println("print3 调用 Outer print1");
                print1();
            }

            public void print4() {
                System.out.println("print4 调用 Outer print11");
                print11();
            }

          	// 反射调用
            public void callOuterPrivateMethod(Outer outer)
                    throws NoSuchMethodException, InvocationTargetException, IllegalAccessException {
                // 内部类反射调用外部类的私有方法
              	// Java11 之前调用会报 java.lang.IllegalAccessException
                final Method method = outer.getClass().getDeclaredMethod("print11");
                method.invoke(outer);
            }
        }

        public static void main(String[] args) throws InvocationTargetException, NoSuchMethodException, IllegalAccessException {
            final Outer outer = new Outer();

         		// 正常方法调用
            outer.callInnerMethod();
            // 通过反射访问
            outer.callInnerReflectionMethod();
        }
    }
```

## HTTP Client 标准化

Java 11 对 Java 9 中引入并在 Java 10 中进行了更新的 Http Client API 进行了标准化，在前两个版本中进行孵化的同时，Http Client 几乎被完全重写，并且现在完全支持异步非阻塞。

示例如下：
```java
    var request = HttpRequest.newBuilder()
        .uri(URI.create("https://javastack.cn"))
        .GET()
        .build();
        var client = HttpClient.newHttpClient();

        // 同步
        HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());
        System.out.println(response.body());

        // 异步
        client.sendAsync(request, HttpResponse.BodyHandlers.ofString())
        .thenApply(HttpResponse::body)
        .thenAccept(System.out::println);
```

## **Epsilon：低开销垃圾回收器**

Epsilon 垃圾回收器的目标是开发一个控制内存分配，但是不执行任何实际的垃圾回收工作。它提供一个完全消极的 GC 实现，分配有限的内存资源，最大限度的降低内存占用和内存吞吐延迟时间。

下面是 no-op GC 的几个使用场景：

*   **性能测试**：什么都不执行的 GC 非常适合用于 GC 的差异性分析。no-op （无操作）GC 可以用于过滤掉 GC 诱发的性能损耗，比如 GC 线程的调度，GC 屏障的消耗，GC 周期的不合适触发，内存位置变化等。此外有些延迟者不是由于 GC 引起的，比如 scheduling hiccups, compiler transition hiccups，所以去除 GC 引发的延迟有助于统计这些延迟。
*   **内存压力测试**：在测试 Java 代码时，确定分配内存的阈值有助于设置内存压力常量值。这时 no-op 就很有用，它可以简单地接受一个分配的内存分配上限，当内存超限时就失败。例如：测试需要分配小于 1G 的内存，就使用-Xmx1g 参数来配置 no-op GC，然后当内存耗尽的时候就直接 crash。
*   **VM 接口测试**：以 VM 开发视角，有一个简单的 GC 实现，有助于理解 VM-GC 的最小接口实现。它也用于证明 VM-GC 接口的健全性。
*   **极度短暂 job 任务**：一个短声明周期的 job 任务可能会依赖快速退出来释放资源，这个时候接收 GC 周期来清理 heap 其实是在浪费时间，因为 heap 会在退出时清理。并且 GC 周期可能会占用一会时间，因为它依赖 heap 上的数据量。 延迟改进：对那些极端延迟敏感的应用，开发者十分清楚内存占用，或者是几乎没有垃圾回收的应用，此时耗时较长的 GC 周期将会是一件坏事。
*   **吞吐改进**：即便对那些无需内存分配的工作，选择一个 GC 意味着选择了一系列的 GC 屏障，所有的 OpenJDK GC 都是分代的，所以他们至少会有一个写屏障。避免这些屏障可以带来一点点的吞吐量提升。

Epsilon 垃圾回收器和其他 OpenJDK 的垃圾回收器一样，可以通过参数 -XX:+UseEpsilonGC 开启。

## **ZGC：可伸缩低延迟垃圾收集器**

ZGC 即 Z Garbage Collector（垃圾收集器或垃圾回收器），这应该是 Java 11 中最为瞩目的特性，没有之一。ZGC 是一个可伸缩的、低延迟的垃圾收集器，主要为了满足如下目标进行设计：

*   GC 停顿时间不超过 10ms
*   即能处理几百 MB 的小堆，也能处理几个 TB 的大堆
*   应用吞吐能力不会下降超过 15%（与 G1 回收算法相比）
*   方便在此基础上引入新的 GC 特性和利用 colord指针
*   针以及 Load barriers 优化奠定基础

当前只支持 Linux/x64 位平台 停顿时间在 10ms 以下，10ms 其实是一个很保守的数据，即便是 10ms 这个数据，也是 GC 调优几乎达不到的极值。根据 SPECjbb 2015 的基准测试，128G 的大堆下最大停顿时间才 1.68ms，远低于 10ms，和 G1 算法相比，改进非常明显。

**测试平台配置：16 cores，堆大小设置为128G**

![image.png](https://p6-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/4a2b11e41f654478a00ffd22c957ede9~tplv-k3u1fbpfcp-jj-mark:0:0:0:0:q75.image#?w=1196&h=581&s=67533&e=png&b=ffffff)

<p align=center>图-9 GC Pause Time 测试结果</p>

## **简化启动单个源代码文件的方法**

此功能允许使用 Java 解释器直接执行 Java 源代码。源代码在内存中编译，然后由解释器执行，不需要在磁盘上生成 .class 文件了。唯一的约束在于所有相关的类必须定义在同一个 Java 文件中。

## **用于 Lambda 参数的局部变量语法**

从 Java 10 开始，便引入了局部变量类型推断这一关键特性。类型推断允许使用关键字 var 作为局部变量的类型而不是实际类型，编译器根据分配给变量的值推断出类型。这一改进简化了代码编写、节省了开发者的工作时间，因为不再需要显式声明局部变量的类型，而是可以使用关键字 var，且不会使源代码过于复杂。

Java11版本支持在 Lambda 表达式中使用局部变量类型推断。

示例如下：
```java
    public class Main {

        public static void main(String[] args) {
            List<String> list = Arrays.asList("a", "b", "c");
            String result = list.stream()
                    .map((@NonNull var x) -> x.toUpperCase())
                    .collect(Collectors.joining(","));
            System.out.println(result);
        }
    }
```

## **低开销的 Heap Profiling**

Java 11 中提供一种低开销的 Java 堆分配采样方法，能够得到堆分配的 Java 对象信息，并且能够通过 JVMTI 访问堆信息。

引入这个低开销内存分析工具是为了达到如下目的：

*   足够低的开销，可以默认且一直开启
*   能通过定义好的程序接口访问
*   能够对所有堆分配区域进行采样
*   能给出正在和未被使用的 Java 对象信息

对用户来说，了解它们堆里的内存分布是非常重要的，特别是遇到生产环境中出现的高 CPU、高内存占用率的情况。目前有一些已经开源的工具，允许用户分析应用程序中的堆使用情况，比如：Java Flight Recorder、jmap、YourKit 以及 VisualVM tools.。但是这些工具都有一个明显的不足之处：无法得到对象的分配位置，headp dump 以及 heap histogram 中都没有包含对象分配的具体信息，但是这些信息对于调试内存问题至关重要，因为它能够告诉开发人员他们的代码中发生的高内存分配的确切位置，并根据实际源码来分析具体问题，这也是 Java 11 中引入这种低开销堆分配采样方法的原因。

## **飞行记录器**

飞行记录器之前是商业版 JDK 的一项分析工具，但在 Java 11 中，其代码被包含到公开代码库中，这样所有人都能使用该功能了。

Java 语言中的飞行记录器类似飞机上的黑盒子，是一种低开销的事件信息收集框架，主要用于对应用程序和 JVM 进行故障检查、分析。飞行记录器记录的主要数据源于应用程序、JVM 和 OS，这些事件信息保存在单独的事件记录文件中，故障发生后，能够从事件记录文件中提取出有用信息对故障进行分析。

启用参数：-XX:StartFlightRecording

示例如下：
```java
    public class EventLog {
        
        static class HelloWorld extends Event {
            @Label("Message")
            String message;
        }

        public static void main(String[] args) {
            HelloWorld event = new HelloWorld();
            event.message = "hello, world!";
            event.commit();
        }
    }
```

# Java12

## **Shenandoah：一个低停顿垃圾收集器（实验阶段）**

Java 12 中引入一个新的垃圾收集器：Shenandoah，它是作为中低停顿时间的垃圾收集器而引入到 Java12 中的，其工作原理是通过与 Java 应用程序中的执行线程同时运行，用以执行其垃圾收集、内存回收任务，通过这种运行方式，给虚拟机带来短暂的停顿时间。Shenandoah 的暂停时间与堆大小无关，无论堆栈是 200 MB 还是 200 GB，都具有相同的一致暂停时间。

Shenandoah是OpenJDK中的垃圾处理器，但相比较Oracle JDK中根正苗红的ZGC，Shenandoah可以说更像是G1的继承者，很多方面与G1非常相似，甚至共用了一部分代码。Shenandoah和G1有三点主要区别：

1.  G1的回收是需要STW的，而且这部分停顿占整体停顿时间的80%以上，Shenandoah则实现了并发回收。
2.  Shenandoah不再区分年轻代和年老代。
3.  Shenandoah使用连接矩阵替代G1中的卡表。

## **增加JMH微基准测试套件**

Java 12 中添加一套新的基本的微基准测试套件（microbenchmarks suite），此功能为JDK源代码添加了一套微基准测试（大约100个），简化了现有微基准测试的运行和新基准测试的创建过程。使开发人员可以轻松运行现有的微基准测试并创建新的基准测试，其目标在于提供一个稳定且优化过的基准，可以轻松测试JDK性能。

## **Switch 表达式扩展（预览功能）**

**Java12** 扩展了现有的 Switch 语句。

*   可省略 break
*   支持返回值

Java12 之前示例如下：
```java
    private static String getText(int number) {
        String result = "";
        switch (number) {
            case 1, 2:
                result = "one or two";
                break;
            case 3:
                result = "three";
                break;
            case 4, 5, 6:
                result = "four or five or six";
                break;
            default:
                result = "unknown";
                break;
        };
        return result;
    }

```
Java12 示例如下：
```java
    private static String getText(int number) {
        String result = switch (number) {
            case 1, 2 -> "one or two";
            case 3 -> "three";
            case 4, 5, 6 -> "four or five or six";
            default -> "unknown";
        };
        return result;
    }
```

## JVM常量API

Java12 在新包 java.lang.invoke.constant 中定义了一系列基于值的符号引用（JVMS 5.1）类型，它们能够描述每种可加载常量。符号引用以纯字面的形式描述了可加载常量，与类加载或可访问性上下文分开。某些类可以充当自己的符号引用（例如String）；对于可链接常量，JDK12定义了一系列符号引用类型（ClassDesc，MethodTypeDesc，MethodHandleDesc 和 DynamicConstantDesc），来包含描述这些常量的信息。

## 移除多余ARM64实现

当前 Java 11 中存在两套 64 位 AArch64 端口，它们主要存在于 src/hotspot/cpu/arm 和 open/src/hotspot/cpu/aarch64 目录中。这两套代码中都实现了 AArch64，Java 12 中将删除目录 open/src/hotspot/cpu/arm 中关于 64-bit 的这套实现，只保留其中有关 32-bit 的实现，余下目录的 open/src/hotspot/cpu/aarch64 代码部分就成了 AArch64 的默认实现。

## **使用默认类数据共享（CDS）存档**

通过在64位平台上的默认类列表的帮助下生成CDS归档来改进JDK构建过程，从而有效地消除了运行java -Xshare：dump。 此功能的目标包括：

*   改进开箱即用的启动时间
*   摆脱使用-Xshare：dump，直接默认开启CDS

##  G1回收器增强：可中断 mixed GC

G1垃圾收集器的主要目标之一是满足用户设置的暂停时间。G1采用一个分析引擎来选择在收集期间要处理的工作量。此选择过程的结果是一组称为GC集的区域。一旦GC集建立并且GC已经开始，那么G1就无法停止。

如果G1发现GC集选择了错误的区域，它会将GC区域的拆分为两部分（强制部分和可选部分）来切换到处理Mix GC的增量模式。如果未达到暂停时间目标，则停止对可选部分的垃圾收集。

##  G1回收器增强：**自动返回未用堆内存给操作系统**

目前 Java11 版本中包含的 G1 垃圾收集器 暂时无法及时将已提交的 Java 堆内存返回给操作系统， G1 垃圾收集器仅在进行完整 GC (Full GC) 或并发处理周期时才能将 Java 堆返回内存。由于 G1 回收器尽可能避免完整 GC，并且只触发基于 Java 堆占用和分配活动的并发周期，因此在许多情况下 G 1 垃圾回收器不能回收 Java 堆内存，除非有外部强制执行。

Java12 版本中，为了尽可能的向操作系统返回空闲内存，G1 垃圾收集器将在应用程序不活动期间定期生成或持续循环检查整体 Java 堆使用情况，以便 GC 垃圾收集器能够更及时的将 Java 堆中不使用内存部分返还给操作系统。对于长时间处于空闲状态的应用程序，此项改进将使 JVM 的内存利用率更加高效。

# Java13

## **动态应用程序类-数据共享** Dynamic CDS Archives

在 Java10 中，为了改善应用启动时间和内存空间占用，通过使用 APP CDS，加大了 CDS 的使用范围，允许自定义的类加载器也可以加载自定义类给多个 JVM 共享使用。

Java10 中的 App CDS 主要是为了将不变的类数据，进行一次创建，然后存储到归档中，以便在应用重启之后可以对其进行内存映射而直接使用，同时也可以在运行的 JVM 实例之间共享使用。但是在 Java 10 中使用 App CDS 需要进行如下操作：

*   创建需要进行类归档的类列表
*   创建归档
*   使用归档方式启动

Java 13 中对 Java 10 中引入的 应用程序类数据共享进行了进一步的简化、改进和扩展。即可以不用提供归档类列表，允许在 Java 应用程序执行结束时动态进行类归档。具体能够被归档的类包括默认CDS归档中不存在的所有加载的应用程序类和库类。

## **ZGC 释放未使用内存**

ZGC 是 Java 11 中引入的最为瞩目的垃圾回收特性，是一种可伸缩、低延迟的垃圾收集器，不过在 Java 11 中是实验性的引入，主要用来改善 GC 停顿时间，并支持几百 MB 至几个 TB 级别大小的堆，并且应用吞吐能力下降不会超过 15%，目前只支持 Linux/x64 位平台的这样一种新型垃圾收集器。

通过在实际中的使用，发现 ZGC 收集器中并没有像 Hotspot 中的 G1 和 Shenandoah 垃圾收集器一样，能够主动将未使用的内存释放给操作系统的功能。对于大多数应用程序来说，CPU 和内存都属于有限的紧缺资源，特别是现在使用的云上或者虚拟化环境中。如果应用程序中的内存长期处于空闲状态，并且还不能释放给操作系统，这样会导致其他需要内存的应用无法分配到需要的内存，而这边应用分配的内存还处于空闲状态，处于”忙的太忙，闲的太闲”的非公平状态，并且也容易导致基于虚拟化的环境中，因为这些实际并未使用的资源而多付费的情况。由此可见，将未使用内存释放给系统主内存是一项非常有用且亟需的功能。

ZGC 堆由一组称为 ZPages 的堆区域组成。在 GC 周期中清空 ZPages 区域时，它们将被释放并返回到页面缓存 ZPageCache 中，此缓存中的 ZPages 按最近最少使用（LRU）的顺序，并按照大小进行组织。在 Java 13 中，ZGC 将向操作系统返回被标识为长时间未使用的页面，这样它们将可以被其他进程重用。同时释放这些未使用的内存给操作系统不会导致堆大小缩小到参数设置的最小大小以下，如果将最小和最大堆大小设置为相同的值，则不会释放任何内存给操作系统。

Java 13 中对 ZGC 的改进，主要体现在下面几点：

*   释放未使用内存给操作系统
*   支持最大堆大小为 16TB
*   添加参数：-XX:SoftMaxHeapSize 来软限制堆大小

这里提到的是软限制堆大小，是指 GC 应努力使堆大小不要超过指定大小，但是如果实际需要，也还是允许 GC 将堆大小增加到超过 SoftMaxHeapSize 指定值。主要用在下面几种情况：当希望降低堆占用，同时保持应对堆空间临时增加的能力，亦或想保留充足内存空间，以能够应对内存分配，而不会因为内存分配意外增加而陷入分配停滞状态。不应将 SoftMaxHeapSize 设置为大于最大堆大小（-Xmx 的值，如果未在命令行上设置，则此标志应默认为最大堆大小。

Java 13 中，ZGC 内存释放功能，默认情况下是开启的，不过可以使用参数：-XX：-ZUncommit 显式关闭，同时如果将最小堆大小 (-Xms) 配置为等于最大堆大小 (-Xmx)，则将隐式禁用此功能。

还可以使用参数：-XX：ZUncommitDelay = <seconds>（默认值为 300 秒）来配置延迟释放，此延迟时间可以指定释放多长时间之前未使用的内存。

## **Socket API 重构**

现在已有的 java.net.Socket 和 java.net.ServerSocket 以及它们的实现类，都可以回溯到 JDK 1.0 时代了。原始socket的维护和调试都很痛苦。实现类还使用了线程栈作为 I/O 的缓冲，导致在某些情况下还需要增加线程栈的大小。该实现还存在几个并发问题，需要彻底解决。在未来的网络世界，要快速响应，不能阻塞本地方法线程，当前的实现不适合使用了。

在 Java 13 之前，通过使用 PlainSocketImpl 作为 SocketImpl 的具体实现。Java 13 中的新底层实现，引入 NioSocketImpl 的实现用以替换 SocketImpl 的 PlainSocketImpl 实现，此实现与 NIO（新 I/O）实现共享相同的内部基础结构，并且与现有的缓冲区高速缓存机制集成在一起，因此不需要使用线程堆栈。除了这些更改之外，还有其他一些更便利的更改，如使用 java.lang.ref.Cleaner 机制来关闭套接字（如果 SocketImpl 实现在尚未关闭的套接字上被进行了垃圾收集），以及在轮询时套接字处于非阻塞模式时处理超时操作等方面。

全新实现的 NioSocketImpl 来替换JDK1.0的PlainSocketImpl。

*   它便于维护和调试，与 NewI/O (NIO) 使用相同的 JDK 内部结构，因此不需要使用系统本地代码。
*   它与现有的缓冲区缓存机制集成在一起，这样就不需要为 I/O 使用线程栈。
*   它使用 java.util.concurrent 锁，而不是 synchronized 同步方法，增强了并发能力。
*   新的实现是Java 13中的默认实现，但是旧的实现还没有删除，可以通过设置系统属性 jdk.net.usePlainSocketImpl 来切换到旧版本。

## **Switch 表达式扩展（预览功能）**

Java13中对于Switch表达式继续扩展，Switch 表达式中就多了一个关键字用于跳出 Switch 块的关键字 yield，主要用于返回一个值，它和 return 的区别在于：return 会直接跳出当前循环或者方法，而 yield 只会跳出当前 Switch块，同时在使用 yield 时，需要有 default 条件。

示例如下：
```java
    private static String getText(int number) {
        return switch (number) {
            case 1, 2:
                yield "one or two";
            case 3:
                yield "three";
            case 4, 5, 6:
                yield "four or five or six";
            default:
                yield "unknown";
        };
    }
```

## **文本块（预览功能）**

一直以来，Java 语言在定义字符串的方式是有限的，字符串需要以双引号开头，以双引号结尾，这导致字符串不能够多行使用，而是需要通过换行转义或者换行连接符等方式来变通支持多行，但这样会增加编辑工作量，同时也会导致所在代码段难以阅读、难以维护。

Java 13 引入了文本块来解决多行文本的问题，文本块以三重双引号开头，并以同样的以三重双引号结尾终止，它们之间的任何内容都被解释为字符串的一部分，包括换行符，避免了对大多数转义序列的需要，并且它仍然是普通的 java.lang.String 对象，文本块可以在 Java 中可以使用字符串文字的任何地方使用，而与编译后的代码没有区别，还增强了 Java 程序中的字符串可读性。并且通过这种方式，可以更直观地表示字符串，可以支持跨越多行，而且不会出现转义的视觉混乱，将可以广泛提高 Java 类程序的可读性和可写性。

Java13 之前示例如下：
```java
    String html ="<html>\n" +
                  "   <body>\n" +
                  "      <p>Hello, World</p>\n" +
                  "   </body>\n" +
                  "</html>\n";


     String json ="{\n" +
                  "   "name":"mkyong",\n" +
                  "   "age":38\n" +
                  "}\n";
```

Java13 示例如下：
```java
    String html = """
                    <html>
                        <body>
                            <p>Hello, World</p>
                        </body>
                    </html>
                    """;

     String json = """
                    {
                        "name":"mkyong",
                        "age":38
                    }
                    """;
```

# Java14

## Switch 表达式（正式版）

Switch 表达式由预览版本转为正式版

## **改进 NullPointerExceptions 提示信息**

Java 14 改进 NullPointerException 的可查性、可读性，能更准确地定位 null 变量的信息。该特性能够帮助开发者和技术支持人员提高生产力，以及改进各种开发工具和调试工具的质量，能够更加准确、清楚地根据动态异常与程序代码相结合来理解程序。

示例如下：
```java
    public class Main {

        public static void main(String[] args) {
            Person person = null;
            person.name = "张三";
        }


        public static class Person {
            public String name;
        }
    }
```

执行结果：

```text
    -- Java15 之前运行
    Exception in thread "main" java.lang.NullPointerException
    	at com.sankuai.powerbank.crm.activity.util.LambdaExample.main(LambdaExample.java:12)
      
    -- Java15及以后
    Exception in thread "main" java.lang.NullPointerException: Cannot assign field "name" because "person" is null
    	at C/org.c.example.Test.main(Test.java:11)
```


## G**1 的 NUMA 可识别内存分配**

Java 14 改进非一致性内存访问（NUMA）系统上的 G1 垃圾收集器的整体性能，主要是对年轻代的内存分配进行优化，从而提高 CPU 计算过程中内存访问速度。

NUMA 是 **non-unified memory access** 的缩写，主要是指在当前的多插槽物理计算机体系中，比较普遍是多核的处理器，并且越来越多的具有 NUMA 内存访问体系结构，即内存与每个插槽或内核之间的距离并不相等。同时套接字之间的内存访问具有不同的性能特征，对更远的套接字的访问通常具有更多的时间消耗。这样每个核对于每一块或者某一区域的内存访问速度会随着核和物理内存所在的位置的远近而有不同的时延差异。

Java 中，堆内存分配一般发生在线程运行的时候，当创建了一个新对象时，该线程会触发 G1 去分配一块内存出来，用来存放新创建的对象，在 G1 内存体系中，其实就是一块 region（大对象除外，大对象需要多个 region），在这个分配新内存的过程中，如果支持了 NUMA 感知内存分配，将会优先在与当前线程所绑定的 NUMA 节点空闲内存区域来执行 allocate 操作，同一线程创建的对象，尽可能的保留在年轻代的同一 NUMA 内存节点上，因为是基于同一个线程创建的对象大部分是短存活并且高概率互相调用的。


![image.png](https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/0ea3c1292ecd4a519e7ca0c6c0cb1ea0~tplv-k3u1fbpfcp-jj-mark:0:0:0:0:q75.image#?w=527&h=361&s=94671&e=png&b=146f90)

<p align=center>图-10 多核CPU内存访问架构</p>

## **删除 CMS 垃圾回收器**

CMS 是老年代垃圾回收算法，通过标记-清除的方式进行内存回收，在内存回收过程中能够与用户线程并行执行。CMS 回收器可以与 Serial 回收器和 Parallel New 回收器搭配使用，CMS 主要通过并发的方式，适当减少系统的吞吐量以达到追求响应速度的目的，比较适合在追求 GC 速度的服务器上使用。

因为 CMS 回收算法在进行 GC 回收内存过程中是使用并行方式进行的，如果服务器 CPU 核数不多的情况下，进行 CMS 垃圾回收有可能造成比较高的负载。同时在 CMS 并行标记和并行清理时，应用线程还在继续运行，程序在运行过程中自然会创建新对象、释放不用对象，所以在这个过程中，会有新的不可达内存地址产生，而这部分的不可达内存是出现在标记过程结束之后，本轮 CMS 回收无法在周期内将它们回收掉，只能留在下次垃圾回收周期再清理掉。这样的垃圾就叫做浮动垃圾。由于垃圾收集和用户线程是并发执行的，因此 CMS 回收器不能像其他回收器那样进行内存回收，需要预留一些空间用来保存用户新创建的对象。由于 CMS 回收器在老年代中使用标记-清除的内存回收策略，势必会产生内存碎片，内存当碎片过多时，将会给大对象分配带来麻烦，往往会出现老年代还有空间但不能再保存对象的情况。

所以，早在几年前的 Java 9 中，就已经决定放弃使用 CMS 回收器了，而这次在 Java14 中，是继之前 Java9 中放弃使用 CMS 之后，彻底将其禁用，并删除与 CMS 有关的选项，同时清除与 CMS 有关的文档内容，至此曾经辉煌一度的 CMS 回收器，也将成为历史。

当在 Java14 版本中，通过使用参数： -XX:+UseConcMarkSweepGC，尝试使用 CMS 时，将会收到下面信息：

```java
    Java HotSpot(TM) 64-Bit Server VM warning: Ignoring option UseConcMarkSweepGC; 
    support was removed in <version>
```

## **ZGC 支持 MacOS 和 Windows 系统（实验阶段）**

ZGC 是最初在 Java11 中引入，同时在后续几个版本中，不断进行改进的一款基于内存 Region，同时使用了内存读屏障、染色指针和内存多重映射等技，并且以可伸缩、低延迟为目标的内存垃圾回收器器，不过在 Java 14 之前版本中，仅仅只支持在 Linux/x64 位平台。

此次 Java14，同时支持 MacOS 和 Windows 系统，解决了开发人员需要在桌面操作系统中使用 ZGC 的问题。

## **弃用 ParallelScavenge 和 SerialOld GC 的组合使用**

由于 Parallel Scavenge 和 Serial Old 垃圾收集算法组合起来使用的情况比较少，并且在年轻代中使用并行算法，而在老年代中使用串行算法，这种并行、串行混搭使用的情况，本身已属罕见同时也很冒险。由于这两 GC 算法组合很少使用，却要花费巨大工作量来进行维护，所以在 Java14 版本中，考虑将这两 GC 的组合弃用。

## **instanceof 模式匹配（预览阶段）**

instanceof 语法优化

Java14 之前示例如下：
```java
    if (person instanceof Student) {
        Student student = (Student) person;
        student.say();
    } else if (person instanceof Teacher) {
        Teacher teacher = (Teacher) person;
        teacher.say();
    }
```

Java14 示例如下：
```java
    if (person instanceof Student student) {
        student.say();
    } else if (person instanceof Teacher teacher) {
        teacher.say();
    }
```


## **Record 类型（预览功能）**

Java 16开始，我们将多一个关键词record，它也可以用来定义类。record关键词的引入，主要是为了提供一种更为简洁、紧凑的final类的定义方式。它支持

1.  单独文件声明
2.  在类内部声明
3.  函数内部声明

示例如下：
```java
    public class Test {
        public static void main(String[] args) {
          
            record PersonRecord(String name, int age) {
            }

            PersonRecord personRecord = new PersonRecord("name", 2222);
            System.out.println(personRecord.name());
        }
    }
```


## 外部存储器访问 API（孵化器版）

在 Java14 之前，如果开发人员想要操作堆外内存，通常的做法就是使用 ByteBuffer 或者 Unsafe，甚至是 JNI 等方式，但无论使用哪一种方式，均无法同时有效解决安全性和高效性等 2 个问题，并且堆外内存的释放也是一个令人头痛的问题；

从 Java14 开始，Java 的设计者们在语法层面为大家带来了崭新的 Memory Access API，极大程度上简化了开发难度，并得以有效的解决了安全性和高效性等 2 个核心问题

示例如下：
```java
    public class Main {
        public static void main(String[] args) {
            var handle = MemoryHandles.varHandle(char.class,
                    ByteOrder.nativeOrder());
           // 申请200字节的堆外内存
            final MemorySegment segment = MemorySegment.allocateNative(200, ResourceScope.globalScope());
            for (int i = 0; i < 4; i++) {
                handle.set(segment, i << 2, (char) (i + 1 + 64));
                System.out.println(handle.get(segment, i << 2));
            }
        }
    }
```
执行结果：
```text
    WARNING: Using incubator modules: jdk.incubator.foreign
    A
    B
    C
    D
```


# Java15

## Edwards-Curve 数字签名算法 (EdDSA)

与 JDK 中的现有签名方案相比，EdDSA 具有更高的安全性和性能，因此备受关注。它已经在OpenSSL和BoringSSL等加密库中得到支持，在区块链领域用的比较多。

示例如下：
```java
    public class EdDSATest {

        public static void main(String[] args) throws Exception {
            KeyPairGenerator kpg = KeyPairGenerator.getInstance("Ed25519");
            KeyPair kp = kpg.generateKeyPair();

            byte[] msg = "abcd".getBytes(StandardCharsets.UTF_8);

            Signature sig = Signature.getInstance("Ed25519");
            sig.initSign(kp.getPrivate());
            sig.update(msg);
            byte[] sign = sig.sign();

            String encodedString = Base64.getEncoder().encodeToString(sign);
            System.out.println(encodedString);
        }
    }
```

## 隐藏类 Hidden Classes

隐藏类定义：Hidden Classes 就是不能直接被其他class的二进制代码使用的class。Hidden Classes主要被一些框架用来生成运行时类，但是这些类不是被用来直接使用的，而是通过反射机制来调用。比如在JDK8中引入的lambda表达式，JVM并不会在编译的时候将lambda表达式转换成为专门的类，而是在运行时将相应的字节码动态生成相应的类对象。

隐藏类是为框架（frameworks）所设计的，隐藏类不能直接被其他类的字节码使用，只能在运行时生成类并通过反射间接使用它们。

*   隐藏类天生为框架设计的，在运行时生成内部的class。
*   隐藏类只能通过反射访问，不能直接被其他类的字节码访问。
*   隐藏类可以独立于其他类加载、卸载，这可以减少框架的内存占用。

普通类是通过调用ClassLoader::defineClass创建的，而隐藏类是通过调用Lookup::defineHiddenClass创建的

示例如下：
```java
    /**
     * 定义普通类，并生成用于加载的 code
     */
    public class JEP371HiddenClasses {

        public static String hello() {
            return "abcd";
        }

        public static void main(String[] args) throws IOException {

            Class clazz = JEP371HiddenClasses.class;
            URL url = clazz.getResource("/" + clazz.getName().replace(".", "/") + ".class");
            assert url != null;
            String filePath = url.getPath();

            byte[] b = Files.readAllBytes(Paths.get(filePath));
            System.out.println("生成Encode类信息" + Base64.getEncoder().encodeToString(b));
        }
    }

```
执行结果：
```text
生成Encode类信息：yv66vgAAAD0AgwoAAgADBwAEDAAFAAYBABBqYXZhL2xhbmcvT2JqZWN0AQAGPGluaXQ+AQADKClWCAAIAQAEYWJjZAcACgEAH29yZy9leGFtcGxlL0pFUDM3MUhpZGRlbkNsYXNzZXMKAAwADQcADgwADwAQAQAPamF2YS9sYW5nL0NsYXNzAQAHZ2V0TmFtZQEAFCgpTGphdmEvbGFuZy9TdHJpbmc7CAASAQABLggAFAEAAS8KABYAFwcAGAwAGQAaAQAQamF2YS9sYW5nL1N0cmluZwEAB3JlcGxhY2UBAEQoTGphdmEvbGFuZy9DaGFyU2VxdWVuY2U7TGphdmEvbGFuZy9DaGFyU2VxdWVuY2U7KUxqYXZhL2xhbmcvU3RyaW5nOxIAAAAcDAAdAB4BABdtYWtlQ29uY2F0V2l0aENvbnN0YW50cwEAJihMamF2YS9sYW5nL1N0cmluZzspTGphdmEvbGFuZy9TdHJpbmc7CgAMACAMACEAIgEAC2dldFJlc291cmNlAQAiKExqYXZhL2xhbmcvU3RyaW5nOylMamF2YS9uZXQvVVJMOwkACQAkDAAlACYBABMkYXNzZXJ0aW9uc0Rpc2FibGVkAQABWgcAKAEAGGphdmEvbGFuZy9Bc3NlcnRpb25FcnJvcgoAJwADCgArACwHAC0MAC4AEAEADGphdmEvbmV0L1VSTAEAB2dldFBhdGgKADAAMQcAMgwAMwA0AQATamF2YS9uaW8vZmlsZS9QYXRocwEAA2dldAEAOyhMamF2YS9sYW5nL1N0cmluZztbTGphdmEvbGFuZy9TdHJpbmc7KUxqYXZhL25pby9maWxlL1BhdGg7CgA2ADcHADgMADkAOgEAE2phdmEvbmlvL2ZpbGUvRmlsZXMBAAxyZWFkQWxsQnl0ZXMBABgoTGphdmEvbmlvL2ZpbGUvUGF0aDspW0IJADwAPQcAPgwAPwBAAQAQamF2YS9sYW5nL1N5c3RlbQEAA291dAEAFUxqYXZhL2lvL1ByaW50U3RyZWFtOwoAQgBDBwBEDABFAEYBABBqYXZhL3V0aWwvQmFzZTY0AQAKZ2V0RW5jb2RlcgEAHCgpTGphdmEvdXRpbC9CYXNlNjQkRW5jb2RlcjsKAEgASQcASgwASwBMAQAYamF2YS91dGlsL0Jhc2U2NCRFbmNvZGVyAQAOZW5jb2RlVG9TdHJpbmcBABYoW0IpTGphdmEvbGFuZy9TdHJpbmc7EgABABwKAE8AUAcAUQwAUgBTAQATamF2YS9pby9QcmludFN0cmVhbQEAB3ByaW50bG4BABUoTGphdmEvbGFuZy9TdHJpbmc7KVYKAAwAVQwAVgBXAQAWZGVzaXJlZEFzc2VydGlvblN0YXR1cwEAAygpWgEABENvZGUBAA9MaW5lTnVtYmVyVGFibGUBABJMb2NhbFZhcmlhYmxlVGFibGUBAAR0aGlzAQAhTG9yZy9leGFtcGxlL0pFUDM3MUhpZGRlbkNsYXNzZXM7AQAFaGVsbG8BAARtYWluAQAWKFtMamF2YS9sYW5nL1N0cmluZzspVgEABGFyZ3MBABNbTGphdmEvbGFuZy9TdHJpbmc7AQAFY2xhenoBABFMamF2YS9sYW5nL0NsYXNzOwEAA3VybAEADkxqYXZhL25ldC9VUkw7AQAIZmlsZVBhdGgBABJMamF2YS9sYW5nL1N0cmluZzsBAAFiAQACW0IBAA1TdGFja01hcFRhYmxlAQAKRXhjZXB0aW9ucwcAbQEAE2phdmEvaW8vSU9FeGNlcHRpb24BAAg8Y2xpbml0PgEAClNvdXJjZUZpbGUBABhKRVAzNzFIaWRkZW5DbGFzc2VzLmphdmEBABBCb290c3RyYXBNZXRob2RzDwYAcwoAdAB1BwB2DAAdAHcBACRqYXZhL2xhbmcvaW52b2tlL1N0cmluZ0NvbmNhdEZhY3RvcnkBAJgoTGphdmEvbGFuZy9pbnZva2UvTWV0aG9kSGFuZGxlcyRMb29rdXA7TGphdmEvbGFuZy9TdHJpbmc7TGphdmEvbGFuZy9pbnZva2UvTWV0aG9kVHlwZTtMamF2YS9sYW5nL1N0cmluZztbTGphdmEvbGFuZy9PYmplY3Q7KUxqYXZhL2xhbmcvaW52b2tlL0NhbGxTaXRlOwgAeQEACC8BLmNsYXNzCAB7AQAZ55Sf5oiQRW5jb2Rl57G75L+h5oGv77yaAQEADElubmVyQ2xhc3NlcwEAB0VuY29kZXIHAH8BACVqYXZhL2xhbmcvaW52b2tlL01ldGhvZEhhbmRsZXMkTG9va3VwBwCBAQAeamF2YS9sYW5nL2ludm9rZS9NZXRob2RIYW5kbGVzAQAGTG9va3VwACEACQACAAAAARAYACUAJgAAAAQAAQAFAAYAAQBYAAAALwABAAEAAAAFKrcAAbEAAAACAFkAAAAGAAEAAAAOAFoAAAAMAAEAAAAFAFsAXAAAAAkAXQAQAAEAWAAAABsAAQAAAAAAAxIHsAAAAAEAWQAAAAYAAQAAABEACQBeAF8AAgBYAAAAywAEAAUAAABQEglMKyu2AAsSERITtgAVugAbAAC2AB9NsgAjmgAPLMcAC7sAJ1m3ACm/LLYAKk4tA70AFrgAL7gANToEsgA7uABBGQS2AEe6AE0AALYATrEAAAADAFkAAAAeAAcAAAAWAAMAFwAYABgAKgAZAC8AGwA8ABwATwAdAFoAAAA0AAUAAABQAGAAYQAAAAMATQBiAGMAAQAYADgAZABlAAIALwAhAGYAZwADADwAFABoAGkABABqAAAACwAB/QAqBwAMBwArAGsAAAAEAAEAbAAIAG4ABgABAFgAAAA0AAEAAAAAABESCbYAVJoABwSnAAQDswAjsQAAAAIAWQAAAAYAAQAAAA4AagAAAAUAAgxAAQADAG8AAAACAHAAcQAAAA4AAgByAAEAeAByAAEAegB8AAAAEgACAEgAQgB9AAkAfgCAAIIAGQ==
```

```java
    /**
     * 加载隐藏类，调用其中方法
     */
    public class HiddenClassTest {

        public static void main(String[] args) throws Throwable {
            // 1. 加载encode之后隐藏类
            String CLASS_INFO = "yv66vgAAAD0AgAoAAgADBwAEDAAFAAYBABBqYXZhL2xhbmcvT2JqZWN0AQAGPGluaXQ+AQADKClWCAAIAQAEYWJjZAcACgEAH29yZy9leGFtcGxlL0pFUDM3MUhpZGRlbkNsYXNzZXMKAAwADQcADgwADwAQAQAPamF2YS9sYW5nL0NsYXNzAQAHZ2V0TmFtZQEAFCgpTGphdmEvbGFuZy9TdHJpbmc7CAASAQABLggAFAEAAS8KABYAFwcAGAwAGQAaAQAQamF2YS9sYW5nL1N0cmluZwEAB3JlcGxhY2UBAEQoTGphdmEvbGFuZy9DaGFyU2VxdWVuY2U7TGphdmEvbGFuZy9DaGFyU2VxdWVuY2U7KUxqYXZhL2xhbmcvU3RyaW5nOxIAAAAcDAAdAB4BABdtYWtlQ29uY2F0V2l0aENvbnN0YW50cwEAJihMamF2YS9sYW5nL1N0cmluZzspTGphdmEvbGFuZy9TdHJpbmc7CgAMACAMACEAIgEAC2dldFJlc291cmNlAQAiKExqYXZhL2xhbmcvU3RyaW5nOylMamF2YS9uZXQvVVJMOwkACQAkDAAlACYBABMkYXNzZXJ0aW9uc0Rpc2FibGVkAQABWgcAKAEAGGphdmEvbGFuZy9Bc3NlcnRpb25FcnJvcgoAJwADCgArACwHAC0MAC4AEAEADGphdmEvbmV0L1VSTAEAB2dldFBhdGgKADAAMQcAMgwAMwA0AQATamF2YS9uaW8vZmlsZS9QYXRocwEAA2dldAEAOyhMamF2YS9sYW5nL1N0cmluZztbTGphdmEvbGFuZy9TdHJpbmc7KUxqYXZhL25pby9maWxlL1BhdGg7CgA2ADcHADgMADkAOgEAE2phdmEvbmlvL2ZpbGUvRmlsZXMBAAxyZWFkQWxsQnl0ZXMBABgoTGphdmEvbmlvL2ZpbGUvUGF0aDspW0IJADwAPQcAPgwAPwBAAQAQamF2YS9sYW5nL1N5c3RlbQEAA291dAEAFUxqYXZhL2lvL1ByaW50U3RyZWFtOwoAQgBDBwBEDABFAEYBABBqYXZhL3V0aWwvQmFzZTY0AQAKZ2V0RW5jb2RlcgEAHCgpTGphdmEvdXRpbC9CYXNlNjQkRW5jb2RlcjsKAEgASQcASgwASwBMAQAYamF2YS91dGlsL0Jhc2U2NCRFbmNvZGVyAQAOZW5jb2RlVG9TdHJpbmcBABYoW0IpTGphdmEvbGFuZy9TdHJpbmc7CgBOAE8HAFAMAFEAUgEAE2phdmEvaW8vUHJpbnRTdHJlYW0BAAdwcmludGxuAQAVKExqYXZhL2xhbmcvU3RyaW5nOylWCgAMAFQMAFUAVgEAFmRlc2lyZWRBc3NlcnRpb25TdGF0dXMBAAMoKVoBAARDb2RlAQAPTGluZU51bWJlclRhYmxlAQASTG9jYWxWYXJpYWJsZVRhYmxlAQAEdGhpcwEAIUxvcmcvZXhhbXBsZS9KRVAzNzFIaWRkZW5DbGFzc2VzOwEABWhlbGxvAQAEbWFpbgEAFihbTGphdmEvbGFuZy9TdHJpbmc7KVYBAARhcmdzAQATW0xqYXZhL2xhbmcvU3RyaW5nOwEABWNsYXp6AQARTGphdmEvbGFuZy9DbGFzczsBAAN1cmwBAA5MamF2YS9uZXQvVVJMOwEACGZpbGVQYXRoAQASTGphdmEvbGFuZy9TdHJpbmc7AQABYgEAAltCAQANU3RhY2tNYXBUYWJsZQEACkV4Y2VwdGlvbnMHAGwBABNqYXZhL2lvL0lPRXhjZXB0aW9uAQAIPGNsaW5pdD4BAApTb3VyY2VGaWxlAQAYSkVQMzcxSGlkZGVuQ2xhc3Nlcy5qYXZhAQAQQm9vdHN0cmFwTWV0aG9kcw8GAHIKAHMAdAcAdQwAHQB2AQAkamF2YS9sYW5nL2ludm9rZS9TdHJpbmdDb25jYXRGYWN0b3J5AQCYKExqYXZhL2xhbmcvaW52b2tlL01ldGhvZEhhbmRsZXMkTG9va3VwO0xqYXZhL2xhbmcvU3RyaW5nO0xqYXZhL2xhbmcvaW52b2tlL01ldGhvZFR5cGU7TGphdmEvbGFuZy9TdHJpbmc7W0xqYXZhL2xhbmcvT2JqZWN0OylMamF2YS9sYW5nL2ludm9rZS9DYWxsU2l0ZTsIAHgBAAgvAS5jbGFzcwEADElubmVyQ2xhc3NlcwEAB0VuY29kZXIHAHwBACVqYXZhL2xhbmcvaW52b2tlL01ldGhvZEhhbmRsZXMkTG9va3VwBwB+AQAeamF2YS9sYW5nL2ludm9rZS9NZXRob2RIYW5kbGVzAQAGTG9va3VwACEACQACAAAAARAYACUAJgAAAAQAAQAFAAYAAQBXAAAALwABAAEAAAAFKrcAAbEAAAACAFgAAAAGAAEAAAAOAFkAAAAMAAEAAAAFAFoAWwAAAAkAXAAQAAEAVwAAABsAAQAAAAAAAxIHsAAAAAEAWAAAAAYAAQAAABEACQBdAF4AAgBXAAAAxgAEAAUAAABLEglMKyu2AAsSERITtgAVugAbAAC2AB9NsgAjmgAPLMcAC7sAJ1m3ACm/LLYAKk4tA70AFrgAL7gANToEsgA7uABBGQS2AEe2AE2xAAAAAwBYAAAAHgAHAAAAFgADABcAGAAYACoAGQAvABsAPAAcAEoAHQBZAAAANAAFAAAASwBfAGAAAAADAEgAYQBiAAEAGAAzAGMAZAACAC8AHABlAGYAAwA8AA8AZwBoAAQAaQAAAAsAAf0AKgcADAcAKwBqAAAABAABAGsACABtAAYAAQBXAAAANAABAAAAAAAREgm2AFOaAAcEpwAEA7MAI7EAAAACAFgAAAAGAAEAAAAOAGkAAAAFAAIMQAEAAwBuAAAAAgBvAHAAAAAIAAEAcQABAHcAeQAAABIAAgBIAEIAegAJAHsAfQB/ABk=";
            byte[] classInBytes = getDecoder().decode(CLASS_INFO);
            // 生成类
            Class<?> proxy = MethodHandles.lookup()
                    .defineHiddenClass(classInBytes, true, MethodHandles.Lookup.ClassOption.NESTMATE)
                    .lookupClass();


            // 输出类名
            System.out.println(proxy.getName());
            // 输出类有哪些函数
            for (Method method : proxy.getDeclaredMethods()) {
                System.out.println(method.getName());
            }

            // 2. 调用hello函数
            MethodHandle mh = MethodHandles.lookup().findStatic(proxy, "hello", MethodType.methodType(String.class));
            String result = (String) mh.invokeExact();
            System.out.println("执行结果：" + result);
        }
    }
```

执行结果：
```text
    org.example.JEP371HiddenClasses/0x0000000801001800
    main
    hello
    执行结果：abcd
```

## **移除Nashorn JavaScript引擎**

Nashorn 是 JDK 1.8 引入的一个 JavaScript 脚本引擎，用来取代 Rhino 脚本引擎。Nashorn 是 ECMAScript-262 5.1 的完整实现，增强了 Java 和 JavaScript 的兼容性，并且大大提升了性能。官方的废弃的理由是随着 ECMAScript 脚本语言的结构、API 的改编速度越来越快，维护 Nashorn 太有挑战性了

## **重新实现 DatagramSocket API**

重新实现了老的 DatagramSocket API 接口，更改了 java.net.DatagramSocket 和 java.net.MulticastSocket 为更加简单、现代化的底层实现，更易于维护和调试。

## **禁用偏向锁定**

在默认情况下禁用偏向锁定，并弃用所有相关命令行选项。目标是确定是否需要继续支持偏置锁定的 **高维护成本** 的遗留同步优化， HotSpot虚拟机使用该优化来减少非竞争锁定的开销。 尽管某些Java应用程序在禁用偏向锁后可能会出现性能下降，但偏向锁的性能提高通常不像以前那么明显。

## **instanceof 自动匹配模式（继续预览）**

Java 14 中对 instanceof 的改进，主要目的是为了让创建对象更简单、简洁和高效，并且可读性更强、提高安全性。

Java14 之前示例如下：
```java
    if (person instanceof Student) {
        Student student = (Student) person;
        student.say();
    } else if (person instanceof Teacher) {
        Teacher teacher = (Teacher) person;
        teacher.say();
    }
```

Java14及以后示例如下：
```java
    if (person instanceof Student student) {
        student.say();
    } else if (person instanceof Teacher teacher) {
        teacher.say();
    }
```

## **ZGC: 可伸缩低延迟垃圾收集器**

成为正式特性

## **文本块**

成为正式特性

## Shenandoah：低暂停时间垃圾收集器

成为正式特性

## **外部存储器访问 API（二次孵化器版）**

在 Java14 之前，如果开发人员想要操作堆外内存，通常的做法就是使用 ByteBuffer 或者 Unsafe，甚至是 JNI 等方式，但无论使用哪一种方式，均无法同时有效解决安全性和高效性等 2 个问题，并且堆外内存的释放也是一个令人头痛的问题；

从 Java14 开始，Java 的设计者们在语法层面为大家带来了崭新的 Memory Access API，极大程度上简化了开发难度，并得以有效的解决了安全性和高效性等 2 个核心问题

示例如下：
```java
    public class Main {
        public static void main(String[] args) {
            var handle = MemoryHandles.varHandle(char.class,
                    ByteOrder.nativeOrder());
           // 申请200字节的堆外内存
            final MemorySegment segment = MemorySegment.allocateNative(200, ResourceScope.globalScope());
            for (int i = 0; i < 4; i++) {
                handle.set(segment, i << 2, (char) (i + 1 + 64));
                System.out.println(handle.get(segment, i << 2));
            }
        }
    }
```

```text
WARNING: Using incubator modules: jdk.incubator.foreign
A
B
C
D
```

## **废除 RMI 激活**

RMI激活在 jdk1.2引入，在RMI系统中，我们使用延迟激活。延迟激活将激活对象推迟到客户第一次使用（即第一次方法调用）之前。 既然RMI Activation这么好用，为什么要废弃呢？

因为对于现代应用程序来说，分布式系统大部分都是基于Web的，web服务器已经解决了穿越防火墙，过滤请求，身份验证和安全性的问题，并且也提供了很多延迟加载的技术。

所以在现代应用程序中，RMI Activation已经很少被使用到了。并且在各种开源的代码库中，也基本上找不到RMI Activation的使用代码了。 为了减少RMI Activation的维护成本，在JDK8中，RMI Activation被置为可选的。现在在JDK15中，终于可以废弃了。

## 密封类（预览版本）

Java15 引入了sealed class或interfaces，这些class或者interfaces只允许被指定的类或者interface进行扩展和实现。使用修饰符sealed，您可以将一个类声明为密封类。密封的类使用关键字permits列出可以直接扩展它的类，子类可以使用 final、sealed、non-sealed修饰。

*   sealed：修饰类/接口，用来描述这个类/接口为密封类/接口
*   non-sealed：修饰类/接口，用来描述这个类/接口为非密封类/接口
*   permits：用在extends和implements之后，指定可以继承或实现的类

Java15 之前示例如下：
```java

    public class Person { } //人
     
    class Teacher extends Person { }//教师
     
    class Worker extends Person { }  //工人
     
    class Student extends Person{ } //学生
```
Java15 之后示例如下：
```java
    // 添加sealed修饰符，permits后面跟上只能被继承的子类名称
    public sealed class Person permits Teacher, Worker, Student{ } //人

    // 子类可以被修饰为 final
    final class Teacher extends Person { }//教师

    // 子类可以被修饰为 non-sealed，此时 Worker类就成了普通类，谁都可以继承它
    non-sealed class Worker extends Person { }  //工人
    // 任何类都可以继承Worker
    class AnyClass extends Worker{}

    //子类可以被修饰为 sealed,同上
    sealed class Student extends Person permits MiddleSchoolStudent,GraduateStudent{ } //学生


    final class MiddleSchoolStudent extends Student { }  //中学生

    final class GraduateStudent extends Student { }  //研究生
```

# **Java16**

## instanceof 模式匹配（正式版）

成为正式特性

## Records (正式版)

成为正式特性

## Unix-Domain 套接字通道

Unix-domain 套接字一直是大多数 Unix 平台的一个特性，现在在 Windows 10 和 Windows Server 2019 也提供了支持。此特性为 java.nio.channels 包的套接字通道和服务器套接字通道 API 添加了 Unix-domain（AF\_UNIX）套接字支持。它扩展了继承的通道机制以支持 Unix-domain 套接字通道和服务器套接字通道。Unix-domain 套接字用于同一主机上的进程间通信（IPC）。它们在很大程度上类似于 TCP/IP，区别在于套接字是通过文件系统路径名而不是 Internet 协议（IP）地址和端口号寻址的。对于本地进程间通信，Unix-domain 套接字比 TCP/IP 环回连接更安全、更有效。

## 对基于值的类发出警告

Valhalla 项目希望通过引入原始类（primitive class）来对 Java 编程模型进行重大改进。原始类的实例是 Identity-free 的，能够进行内联和扁平化的表示。这样的实例可以在内存不同位置之间自由复制，也可以仅仅用字段的值进行编码（这里编码可能指内存中的布局）。

JDK16中对@jdk.internal.ValueBased 注解加入了基于值的类的告警，迁移的候选者在 API 规范中被非正式的指定为基于值的，这意味着

*   不可变的
*   对象的 Identity 对自身的行为来说不重要（就是之前说的 Identity-free）
*   不提供生成独立 Identity 实例的机制，例如公开的构造函数（每次调用返回的对象都是一个新的 Identity）

java.lang 中的包装类（Byte、Short、Integer、Long、Float、Double、Boolean 和 Character）已被指定为基于值的。为了防止基于值的实例被错误的使用：

*   包装类的构造函数，已经被标记为废弃，并且增加了 forRemoval。javac 在编译时默认都会提示警告信息。jdeprscan 工具也可以识别二进制文件中使用已废弃 API 的情况。

```java
            /*
        * <p>This is a <a href="{@docRoot}/java.base/java/lang/doc-files/ValueBased.html">value-based</a>
         * class; programmers should treat instances that are
         * {@linkplain #equals(Object) equal} as interchangeable and should not
         * use instances for synchronization, or unpredictable behavior may
         * occur. For example, in a future release, synchronization may fail.
         *
         * <p>Implementation note: The implementations of the "bit twiddling"
         * methods (such as {@link #highestOneBit(int) highestOneBit} and
         * {@link #numberOfTrailingZeros(int) numberOfTrailingZeros}) are
         * based on material from Henry S. Warren, Jr.'s <i>Hacker's
         * Delight</i>, (Addison Wesley, 2002).
         *
         * @author  Lee Boynton
         * @author  Arthur van Hoff
         * @author  Josh Bloch
         * @author  Joseph D. Darcy
         * @since 1.0
         */
        @jdk.internal.ValueBased
        public final class Integer extends Number
                implements Comparable<Integer>, Constable, ConstantDesc {

        // ... 
          /**
            * Constructs a newly allocated {@code Integer} object that
            * represents the specified {@code int} value.
            *
            * @param   value   the value to be represented by the
            *                  {@code Integer} object.
            *
            * @deprecated
            * It is rarely appropriate to use this constructor. The static factory
            * {@link #valueOf(int)} is generally a better choice, as it is
            * likely to yield significantly better space and time performance.
            */
          @Deprecated(since="9", forRemoval = true)
          public Integer(int value) {
              this.value = value;
          }
        // ...
```


![image.png](https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/f5cf6a21877243cfa795f964c0eb6b65~tplv-k3u1fbpfcp-jj-mark:0:0:0:0:q75.image#?w=1458&h=988&s=249707&e=png&b=272b32)

<p align=center>图-11 Integer基于值的警告</p>


*   javac 新增了同步（synchronized）警告类型，在基于值的类或者所有子类都是基于值的类的实例上使用同步时会给出警告。默认是开启的，可以通过 -Xlint:synchronization 手动选择。

<!---->

*   HotSpot 实现了一个运行时检测手段，可以检测在基于值的实例上使用 monitorenter 指令。命令行选项 -XX:DiagnoseSyncOnValueBasedClasses=1 会将操作视为致命错误。命令行选项 -XX:DiagnoseSyncOnValueBasedClasses=2 将通过控制台和 JDK Flight Recorder 事件打开日志记录。

![image.png](https://p6-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/eac10c0f976c4c2495f769ac992cd26f~tplv-k3u1fbpfcp-jj-mark:0:0:0:0:q75.image#?w=1472&h=542&s=120173&e=png&b=2a2e36)

<p align=center>图-12 synchronized 基于值的警告</p>

在 JDK 中，@jdk.internal.ValueBased 注解用于向 javac 和 HotSpot 表明类是基于值的，或者表明抽象类或接口需要基于值的子类。\
Java API 和 JDK 中下列类型被标记为 @ValueBased

1.  包装类 `Byte`、`Short`、`Integer`、`Long`、`Float`、`Double`、`Boolean` 和 `Character`
2.  `java.lang.Runtime.Version`
3.  java.util 中的 optional 类：`Optional`、`OptionalInt`、 `OptionalLong` 和 `OptionalDouble`
4.  java.time 中的 API：`Instant`、 `LocalDate`、 `LocalTime`、 `LocalDateTime`、 `ZonedDateTime`、 `ZoneId`、 `OffsetTime`、 `OffsetDateTime`、 `ZoneOffset`、 `Duration`、 `Period`、 Year、 `YearMonth`、`MonthDay`、`MinguoDate`、 `HijrahDate`、 `JapaneseDate` 和 `ThaiBuddhistDate`
5.  java.lang.ProcessHandle 接口和实现类
6.  java.util 中的集合工厂方法的实现类：`List.of`、 `List.copyOf`、 `Set.of`、 `Set.copyOf`、 `Map.of`、 `Map.copyOf`、 `Map.ofEntries` 和 `Map.entry`

在抽象类或者接口中使用该注解，意味着所有子类也使用了该注解。

## 打包工具（正式版）

提供了 jpackage 打包工具，可用于打包独立的 Java 应用程序。jpackage 打包工具是在 JDK 14 中首次作为孵化工具引入的新特性，到了 JDK 15 它仍然还在孵化中，现在它终于转正了。

## 默认强封装 JDK 内部元素

一些流行的库使用非标准、不稳定和不受支持的 API，这些 API 是 JDK 的内部实现细节，从未打算供外部使用。在模块化 JDK ( JEP 200 ) 中，通过利用模块系统 ( JEP 261 ) 限制对这些 API 的访问可以提高平台的完整性和安全性，因为其中许多内部 API 定义了特权的、安全敏感的操作。从长远来看，这种变化将减少 JDK 本身的维护者以及库和应用程序的维护者（无论是否有意使用这些内部 API）所承担的成本。

强封装适用于编译时和运行时，包括编译代码试图在运行时通过反射访问元素时。导出包的非public元素和未导出包的所有元素被称为*强封装*。\
在 JDK 9 及更高版本中，我们强封装了所有新的内部元素，从而限制了对它们的访问。然而，为了帮助迁移，我们有意选择不在运行时强封装 JDK 8 中存在的包的内容。类路径上的库和应用程序代码因此可以继续使用反射来访问非public元素包的java.*所有元素和sun.*其他内部包，用于 JDK 8 中存在的包。这种安排称为*宽松的强封装*。

我们于 2017 年 9 月发布了 JDK 9。JDK 的大部分常用内部元素现在都有标准替换。开发人员已经有三年多的时间从 JDK 的内部元素迁移到标准 API，例如java.lang.invoke.MethodHandles.Lookup::defineClass、java.util.Base64和java.lang.ref.Cleaner。许多库、框架和工具维护者已经完成了迁移并发布了其组件的更新版本。我们现在准备采取下一步措施，对 JDK 的所有内部元素进行强封装。

## ZGC 并发线程处理

ZGC 线程栈处理从安全点转移到一个并发阶段，甚至在大堆上也允许在毫秒内暂停 GC 安全点。消除 ZGC 垃圾收集器中最后一个延迟源可以极大地提高应用程序的性能和效率

## 弹性元空间

弹性元空间是一种新的元空间管理机制。在Java16之前，Java 应用程序的元空间大小是固定的，无法根据应用程序的需要进行动态调整。这导致一些复杂的应用程序需要手动调整元空间的大小，从而增加了开发和维护的难度。Java16之后，可以根据应用程序的内存使用情况、元空间的使用情况、系统资源的使用情况自动调整。

Hotspot虚拟机中方法区变更历史

1.  JDK 1.6 及之前：方法区使用永久代实现，**静态变量存放在永久代**；
2.  JDK 1.7 ：“去永久代”的前置版本，还存在永久代，不过**已经将字符串常量池和静态变量从永久代移到了堆上**；
3.  JDK 1.8 及以后：**废除永久代，使用元空间（存放在本地内存中）实现方法区，常量保存在元空间，但字符串常量池和静态变量依然保存在堆中**。

## 向量 Vector API（孵化器）

AVX（Advanced Vector Extensions，高级向量扩展）实际上是 x86-64 处理器上的一套 SIMD（Single Instruction Multiple Data，单指令多数据流）指令集，相对于 SISD（Single instruction, Single dat，单指令流但数据流）而言，SIMD 非常适用于 CPU 密集型场景，因为向量计算允许在同一个 CPU 时钟周期内对多组数据批量进行数据运算，执行性能非常高效，甚至从某种程度上来看，向量运算似乎更像是一种并行任务，而非像标量计算那样，在同一个 CPU 时钟周期内仅允许执行一组数据运算，存在严重的执行效率低下问题。


![image.png](https://p6-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/07c34142ac784f3785624dd02734c785~tplv-k3u1fbpfcp-jj-mark:0:0:0:0:q75.image#?w=1618&h=614&s=369302&e=png&b=ffffff)

<p align=center>图-12 向量计算示例</p>

随着 Java16 的正式来临，开发人员可以在程序中使用 Vector API 来实现各种复杂的向量计算，由 JIT 编译器 Server Compiler(C2)在运行期将其编译为对应的底层 AVX 指令执行

```java
    void scalarComputation() {
        var a = new float[10000000];
        var b = new float[10000000];
        // 省略数组a和b的赋值操作
        var c = new float[10000000];
        for (int i = 0; i < a.length; i++) {
            c[i] = (a[i] * a[i] + b[i] * b[i]) * -1.0f;
        }
    }
```

在上述程序示例中，循环体内每次只能执行一组浮点运算，总共需要执行约 1000 万次才能够获得最终的运算结果，可想而知，这样的执行效率必然低效。值得庆幸的是，从 Java6 的时代开始，Java 的设计者们就在 HotSpot 虚拟机中引入了一种被称之为 SuperWord 的自动向量优化算法，该算法缺省会将循环体内的标量计算自动优化为向量计算，以此来提升数据运算时的执行效率。当然，我们可以通过虚拟机参数-XX:-UseSuperWord来显式关闭这项优化（从实际测试结果来看，如果不开启自动向量优化，存在约 20%\~22%之间的性能下降）。

在此大家需要注意，尽管 HotSpot 缺省支持自动向量优化，但局限性仍然非常明显，首先，JIT 编译器 Server Compiler(C2)仅仅只会对循环体内的代码块做向量优化，并且这样的优化也是极不可靠的；其次，对于一些复杂的向量运算，SuperWord 则显得无能为力。因此，在一些特定场景下（比如：机器学习，线性代数，密码学等），建议大家还是尽可能使用 Java16 为大家提供的 Vector API 来实现复杂的向量计算。示例：

```java
    // 定义256bit的向量浮点运算
    static final VectorSpecies<Float> SPECIES = FloatVector.SPECIES_256;
    void vectorComputation(float[] a, float[] b, float[] c) {
        var i = 0;
        var upperBound = SPECIES.loopBound(a.length);
        for (; i < upperBound; i += SPECIES.length()) {
            var va = FloatVector.fromArray(SPECIES, a, i);
            var vb = FloatVector.fromArray(SPECIES, b, i);
            var vc = va.mul(va).
                    add(vb.mul(vb)).
                    neg();
            vc.intoArray(c, i);
        }
        for (; i < a.length; i++) {
            c[i] = (a[i] * a[i] + b[i] * b[i]) * -1.0f;
        }
    }
```

## 外部链接器 API（孵化器）

外部链接器 API 允许 Java 应用程序与外部链接器进行交互，从而实现更加灵活和高效的编译和链接过程。

## 外部存储器访问 API（第三次孵化）

在 Java14 之前，如果开发人员想要操作堆外内存，通常的做法就是使用 ByteBuffer 或者 Unsafe，甚至是 JNI 等方式，但无论使用哪一种方式，均无法同时有效解决安全性和高效性等 2 个问题，并且堆外内存的释放也是一个令人头痛的问题；

从 Java14 开始，Java 的设计者们在语法层面为大家带来了崭新的 Memory Access API，极大程度上简化了开发难度，并得以有效的解决了安全性和高效性等 2 个核心问题
示例如下：
```java
    public class Main {
        public static void main(String[] args) {
            var handle = MemoryHandles.varHandle(char.class,
                    ByteOrder.nativeOrder());
           // 申请200字节的堆外内存
            final MemorySegment segment = MemorySegment.allocateNative(200, ResourceScope.globalScope());
            for (int i = 0; i < 4; i++) {
                handle.set(segment, i << 2, (char) (i + 1 + 64));
                System.out.println(handle.get(segment, i << 2));
            }
        }
    }
```
执行结果：
```text
WARNING: Using incubator modules: jdk.incubator.foreign
A
B
C
D
```


## 密封类（预览）

Java15 引入了sealed class或interfaces，这些class或者interfaces只允许被指定的类或者interface进行扩展和实现。使用修饰符sealed，您可以将一个类声明为密封类。密封的类使用关键字permits列出可以直接扩展它的类，子类可以使用 final、sealed、non-sealed修饰。

*   sealed：修饰类/接口，用来描述这个类/接口为密封类/接口
*   non-sealed：修饰类/接口，用来描述这个类/接口为非密封类/接口
*   permits：用在extends和implements之后，指定可以继承或实现的类
Java15 之前示例如下：
```java
    public class Person { } //人
     
    class Teacher extends Person { }//教师
     
    class Worker extends Person { }  //工人
     
    class Student extends Person{ } //学生
```
Java15 之后示例如下：
```java

    // 添加sealed修饰符，permits后面跟上只能被继承的子类名称
    public sealed class Person permits Teacher, Worker, Student{ } //人
     
    // 子类可以被修饰为 final
    final class Teacher extends Person { }//教师
     
    // 子类可以被修饰为 non-sealed，此时 Worker类就成了普通类，谁都可以继承它
    non-sealed class Worker extends Person { }  //工人
    // 任何类都可以继承Worker
    class AnyClass extends Worker{}
     
    //子类可以被修饰为 sealed,同上
    sealed class Student extends Person permits MiddleSchoolStudent,GraduateStudent{ } //学生

     
    final class MiddleSchoolStudent extends Student { }  //中学生
     
    final class GraduateStudent extends Student { }  //研究生
```


## 启用 C++14 语言特性（在 JDK 源代码中）

它允许在 JDK C++ 源代码中使用 C++14 语言特性。在 JDK 15 中，JDK 中 C++ 代码使用的语言特性仅限于 C++98/03 语言标准。

## 从 Mercurial 迁移到 Git & 迁移到 GitHub

OpenJDK 社区的源代码存储库从 Mercurial（hg）迁移到 Git，并将它们托管在 GitHub 上以供 JDK 11 及更高版本使用

## JEP 386 AlpineLinux 移植 & JEP 388：Windows/AArch64 移植

# **Java17**

## 密封的类和接口

成为正式特性

## 恢复始终严格的浮点语义

1990年附近，Java 虚拟机为了保持原有的浮点型语义，在兼容 x86 架构的处理器上执行 x87 指令集。在所有情况下匹配精确的浮点语义，包括低于标准的操作数和结果，需要大量额外的指令开销。所以在JDK1.2之后，对浮点语义进行了微调，对默认的严格浮点语义进行了修改。加入 strictfp 来表示原有的浮点型语义，而默认的浮点型则采用了更加宽松的语义。

然而，SSE2（Streaming SIMD Extensions 2）扩展，大约从 2001 年开始在 Pentium 4 和更高版本的处理器中发布，可以以直接的方式支持严格的 JVM 浮点运算，而不会产生过多的开销。Intel 和 AMD 都长期支持 SSE2 和更高版本的扩展，这些扩展允许自然支持严格的浮点语义，因此在 Java17 恢复为严格的浮点语义。

## 增强的伪随机数生成器

对伪随机数生成器进行增强

1、提供了一个新接口 RandomGenerator，RandomGenerators 提供名为 ints、longs、doubles、nextBoolean、nextInt、nextLong、nextDouble 和 nextFloat 的方法，以及它们当前的所有参数的变化。从而更容易在应用程序中互换使用各种 PRNG 算法。

```java
    static void testRandomGenerator (RandomGenerator randomGenerator){
            IntStream ints = randomGenerator.ints(50,0,10);
            int[] randoms = ints.toArray();
            for (int i:randoms) {
                System.out.println(i);
            }
            System.out.println("random count = "+randoms.length);
    }

    public static void main(String[] args) {
            testRandomGenerator(new Random());
            testRandomGenerator(new SplittableRandom());
            testRandomGenerator(ThreadLocalRandom.current());
    }
```

2、提供了一个新类 RandomGeneratorFactory，用于定位和构造 RandomGenerator

```java
    static void testRandomGeneratorFactory(String randomGeneratorName){
            RandomGeneratorFactory<RandomGenerator> factory = RandomGeneratorFactory.of(randomGeneratorName);
            RandomGenerator generator = factory.create();
            testRandomGenerator(generator);
    }


    public static void main(String[] args) {
            testRandomGeneratorFactory("Random");
            testRandomGeneratorFactory("L128X128MixRandom");
            testRandomGeneratorFactory("Xoshiro256PlusPlus");
    }
```

3、提供了四个新的专用 RandomGenerator 接口。SplittableRandomGenerator、JumpableRandomGenerator、LeapableRandomGenerator、ArbitrarilyJumpableRandomGenerator

## 新增switch模式匹配（预览版）

如 instanceof 一样， switch 也紧跟着增加了类型匹配自动转换功能。

Java17 之前示例如下：
```java
    static String formatter(Object o) {
        String formatted = "unknown";
        if (o instanceof Integer i) {
            formatted = String.format("int %d", i);
        } else if (o instanceof Long l) {
            formatted = String.format("long %d", l);
        } else if (o instanceof Double d) {
            formatted = String.format("double %f", d);
        } else if (o instanceof String s) {
            formatted = String.format("String %s", s);
        }
        return formatted;
    }
```

Java17 之后示例如下：
```java
    static String formatterPatternSwitch(Object o) {
        return switch (o) {
            case Integer i -> String.format("int %d", i);
            case Long l    -> String.format("long %d", l);
            case Double d  -> String.format("double %f", d);
            case String s  -> String.format("String %s", s);
            default        -> o.toString();
        };
    }
```

## 新的macOS渲染管道

之前Java 2D 内部渲染管道在MacOS系统上使用的是已弃用的 Apple OpenGL API，Java17 对现有管道进行了替换，使用 Apple Metal API 。

## 支持macOS AArch64

将 JDK 移植到 macOS/AArch64 平台。Apple 计划将Macintosh电脑系列从x64架构过渡到AArch64结构，考虑到使用的用户数量，为了避免JDK移植可能产生性能问题，Java17版本特别支持了 AArch64。

## 弃用 Applet API

Applet 是一种在网页上运行的小型程序，它可以提供丰富的交互和动态功能。Java Applet API 是 Java 编程语言中用于开发和部署 applet 的一套接口。当前所有的浏览器厂商不是已经放弃了对于Java Applet API的支持，就是已经宣布未来会这样做，因此在Java17版本选择删除Java Applet API

## 删除 RMI 激活

Java15已经废弃了 RMI 的远程激活功能，Java17中正式删除了远程方法调用 (RMI) 激活机制，同时保留 RMI 的其余部分。

## 删除实验性 AOT 和 JIT 编译器

在 Java 9 的 JEP 295 ,引入了实验性的提前 (AOT) 编译器，在启动虚拟机之前将 Java 类编译为本机代码。

Java 17，删除实验性的提前 (AOT) 和即时 (JIT) 编译器，因为该编译器自推出以来很少使用，维护它所需的工作量很大。保留实验性的 Java 级 JVM 编译器接口 (JVMCI)，以便开发人员可以继续使用外部构建的编译器版本进行 JIT 编译。

## 弃用安全管理器以进行删除

安全管理器可追溯到 Java 1.0，多年来，它一直不是保护客户端 Java 代码的主要方法，也很少用于保护服务器端代码。为了推动 Java 向前发展，Java 17 弃用安全管理器，以便与旧版 Applet API ( JEP 398 ) 一起移除。

## 外部函数和内存API （第一轮孵化）

改进了 JDK14 和 JDK15 中引入的孵化 API，使 Java 程序能够与 Java 运行时之外的代码和数据进行互操作。通过有效地调用外部函数（即 JVM 之外的代码）和安全地访问外部内存，这些 API 使 Java 程序能够调用本地库和处理本地数据，而不会像 Java 本地接口 (JNI) 那样脆弱和复杂。

在没有外部函数和内存 API 之前：

*   Java 通过 sun.misc.Unsafe 提供一些执行低级别、不安全操作的方法（如直接访问系统内存资源、自主管理内存资源等），Unsafe 类让 Java 语言拥有了类似 C 语言指针一样操作内存空间的能力的同时，也增加了 Java 语言的不安全性，不正确使用 Unsafe 类会使得程序出错的概率变大。
*   Java 1.1 就已通过 Java 原生接口（JNI）支持了原生方法调用，但并不好用。JNI 实现起来过于复杂，步骤繁琐，不受 JVM 的语言安全机制控制，影响 Java 语言的跨平台特性。并且，JNI 的性能也不行，因为 JNI 方法调用不能从许多常见的 JIT 优化(如内联)中受益。虽然JNA、JNR和JavaCPP等框架对 JNI 进行了改进，但效果还是不太理想

引入外部函数和内存 API 就是为了解决 Java 访问外部函数和外部内存存在的一些痛点。Foreign Function & Memory API (FFM API) 定义了类和接口：

*   分配外部内存：MemorySegment、、MemoryAddress和SegmentAllocator）；
*   操作和访问结构化的外部内存：MemoryLayout, VarHandle；
*   控制外部内存的分配和释放：MemorySession；
*   调用外部函数：Linker、FunctionDescriptor和SymbolLookup。

下面是 FFM API 使用示例，这段代码获取了 C 库函数的 radixsort 方法句柄，然后使用它对 Java 数组中的四个字符串进行排序。

```java
    // 1. 在C库路径上查找外部函数
    Linker linker = Linker.nativeLinker();
    SymbolLookup stdlib = linker.defaultLookup();
    MethodHandle radixSort = linker.downcallHandle(
                                 stdlib.lookup("radixsort"), ...);
    // 2. 分配堆上内存以存储四个字符串
    String[] javaStrings   = { "mouse", "cat", "dog", "car" };
    // 3. 分配堆外内存以存储四个指针
    SegmentAllocator allocator = implicitAllocator();
    MemorySegment offHeap  = allocator.allocateArray(ValueLayout.ADDRESS, javaStrings.length);
    // 4. 将字符串从堆上复制到堆外
    for (int i = 0; i < javaStrings.length; i++) {
        // 在堆外分配一个字符串，然后存储指向它的指针
        MemorySegment cString = allocator.allocateUtf8String(javaStrings[i]);
        offHeap.setAtIndex(ValueLayout.ADDRESS, i, cString);
    }
    // 5. 通过调用外部函数对堆外数据进行排序
    radixSort.invoke(offHeap, javaStrings.length, MemoryAddress.NULL, '\0');
    // 6. 将(重新排序的)字符串从堆外复制到堆上
    for (int i = 0; i < javaStrings.length; i++) {
        MemoryAddress cStringPtr = offHeap.getAtIndex(ValueLayout.ADDRESS, i);
        javaStrings[i] = cStringPtr.getUtf8String(0);
    }
    assert Arrays.equals(javaStrings, new String[] {"car", "cat", "dog", "mouse"});  // true
```

## Vector API（第二轮孵化）

Java16引入，Java17第二次孵化

## 外部存储器访问 API（第三次孵化）

在 Java14 之前，如果开发人员想要操作堆外内存，通常的做法就是使用 ByteBuffer 或者 Unsafe，甚至是 JNI 等方式，但无论使用哪一种方式，均无法同时有效解决安全性和高效性等 2 个问题，并且堆外内存的释放也是一个令人头痛的问题；

从 Java14 开始，Java 的设计者们在语法层面为大家带来了崭新的 Memory Access API，极大程度上简化了开发难度，并得以有效的解决了安全性和高效性等 2 个核心问题
示例如下：
```java
    public class Main {
        public static void main(String[] args) {
            var handle = MemoryHandles.varHandle(char.class,
                    ByteOrder.nativeOrder());
           // 申请200字节的堆外内存
            final MemorySegment segment = MemorySegment.allocateNative(200, ResourceScope.globalScope());
            for (int i = 0; i < 4; i++) {
                handle.set(segment, i << 2, (char) (i + 1 + 64));
                System.out.println(handle.get(segment, i << 2));
            }
        }
    }
```
执行结果：
```java
WARNING: Using incubator modules: jdk.incubator.foreign 
A
B
C
D
```


# 参考

1.  [Wikipedia:Java版本历史](https://zh.wikipedia.org/wiki/Java%E7%89%88%E6%9C%AC%E6%AD%B7%E5%8F%B2)
2.  <https://openjdk.org/>
3. [Java8+特性知识体系详解](https://pdai.tech/md/java/java8up/java-8-up-overview.html)
4. [Java 版本更新周期与 Java 发行](https://vividcode.cc/java-versions-and-releases/)
5. [10 w+字总结！Java 8---Java 17 特性详解](https://zhuanlan.zhihu.com/p/408593640)
6. new-relic-2023-state-of-the-java-ecosystem-2023-04-20
7. [【JDK 11】关于 Java 模块系统，看这一篇就够了](https://juejin.cn/post/6847902216590721031)


