# Warehouse Management System

## 项目简介

这是一个基于C++开发的仓库管理系统，具有以下功能：
- 管理员可以添加新商品、更新商品价格和库存、列出所有商品。
- 用户可以订阅商品的更新通知、查看通知、列出所有商品和用户信息。

## 功能列表

### 管理员功能
- 添加新商品：输入商品名称、库存数量和价格。
- 更新商品价格：输入商品名称和新价格。
- 更新商品库存：输入商品名称和库存变化量。
- 列出所有商品：显示所有商品的名称、库存和价格。

### 用户功能
- 创建新用户：输入用户名和联系方式。
- 输入用户ID登录：根据用户ID进行操作。
- 订阅商品更新：输入商品名称和事件类型（库存更新或价格更新）。
- 查看通知：查看所有未读通知并清除。
- 列出所有商品：显示所有商品的名称、库存和价格。
- 列出所有用户：显示所有用户的ID、用户名和联系方式。

## 安装步骤

1. **安装依赖**
   - 安装wxWidgets库
   - 需要安装C++编译器（如g++）。

2. **克隆仓库**
   ```sh
   git clone https://github.com/yourusername/warehouse_management_system.git
   cd warehouse_management_system

3. **编译源代码**
    ```sh
    g++ -std=c++11 -o item_ui item_ui.cpp `wx-config --cxxflags --libs`
    g++ -std=c++11 -o event_ui event_ui.cpp `wx-config --cxxflags --libs`
    g++ -std=c++11 -o main main.cpp
