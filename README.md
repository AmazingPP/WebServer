# 开发部署环境

+ 操作系统: Ubuntu 20.04.2 LTS
+ 编译器: g++ 9.3.0
+ 版本控制: git
+ 自动化构建: CMake
+ 集成开发工具: CLion
+ 编辑器: Vim
+ 压测工具：[WebBench](https://github.com/EZLippi/WebBench)

# 核心功能及技术

+ 状态机解析HTTP请求，目前支持HTTP GET、HEAD方法

+ 添加定时器支持HTTP长连接，定时回调handler处理超时连接

+ 使用 `priority_queue` 实现的最小堆结构管理定时器，使用标记删除，以支持惰性删除，提高性能

+ 使用`epoll` + 非阻塞IO + 边缘触发(ET) 实现高并发处理请求，使用Reactor编程模型

+ `epoll`使用`EPOLLONESHOT`保证一个`socket`连接在任意时刻都只被一个线程处理

+ 使用面向对象思想封装`ptrhead`

+ 使用线程池提高并发度，并降低频繁创建线程的开销

+ 使用RAII手法封装互斥器(`pthrea_mutex_t`)、 条件变量(`pthread_cond_t`)等线程同步互斥机制，使用RAII管理文件描述符等资源

+ 使用`shared_ptr`、`weak_ptr`管理指针，防止内存泄漏

# 并发模型

程序使用Reactor模型，主线程使用epoll作为IO多路复用的实现方式，只负责监听文件描述符上是否有事件发生，有的话就将对应的文件描述符交给工作线程来处理，
工作线程是在程序开始时便创建的固定数量线程池，避免了频繁创建线程带来的开销。

## 项目采用的Reactor模型

在项目中有一个主线程和四个工作线程，主线程将任务添加到线程池，任务即就绪待处理的文件描述符,epoll使用EPOLLONESHOT保证一个socket连接在任意时刻都只被一个线程处理

## epoll工作模式的选择

epoll的触发模式选择了ET模式，ET模式要比高效很多，不会被同一事件触发多次，每次读都必须循环读取直到EAGIN错误，确保处理完。

# 并发性能

- [ ] TODO 待测试、分析
