/**
 * @file hashtable.h
 * @author lzc (liuzechen.coder@qq.com)
 * @brief hashtale， 是一个链式哈希
 * @version 0.1
 * @date 2023-12-24
 *
 * @copyright Copyright (c) 2023
 */

#ifndef MY_REDIS_HASHTABLE_H
#define MY_REDIS_HASHTABLE_H

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <cstdlib>

namespace my_redis {

class HashTable;
class HashMap;
struct HashNode;

constexpr size_t k_resizing_work = 128; ///< 调整大小用的
constexpr size_t k_max_load_factor = 8;
/**
 * @brief HashTable 上的节点
 *
 */
struct HashNode {
    HashNode* m_next { nullptr }; ///< 下一个HashNode
    uint64_t m_hash_code { 0 };   ///< 编码
};

/**
 * @brief Hash表
 *
 */
class HashTable {
public:
    /**
     * @brief 声明成友元函数，能否访问 private 类型变量
     *
     */
    friend HashMap;
    friend HashTable;

public:
    HashTable() = default;

    /**
     * @brief 构建一个新的HashTable的对象
     * @param n 数组的大小
     * @note n 必须是 2 的 幂
     */
    HashTable(size_t n);

    /**
     * @brief Destroy the Hash Table object
     *
     */
    ~HashTable();

    /**
     * @brief 插入一个节点到表里面
     *
     * @param node 一个节点
     */
    void insert(HashNode* node);

    /**
     * @brief 根据key遍历一遍这个HashTable中某一个数组
     *
     * @param key map<k, v> 中的 k
     * @param cmp 比较函数
     * @return HashNode**
     */
    HashNode** lookup(HashNode* key, bool (*cmp)(HashNode*, HashNode*));

    /**
     * @brief 分离一个节点
     *
     * @param from
     * @return HashNode*
     */
    HashNode* detach(HashNode** from);

private:
    HashNode** m_table { nullptr }; ///< HashTable的槽
    size_t m_mask { 0 };            ///< 掩码
    size_t m_size { 0 };            ///< 大小
};

/**
 * @brief 最终的HashMap
 *
 */
class HashMap {
public:
    friend HashNode;
    friend HashTable;

public:
    /**
     * @brief 构建一个新的HashMap的对象
     * @note default 就行，等到用的时候再进行新疆扩容等
     */
    HashMap() = default;

    /**
     * @brief 在Map上进行查找相关信息
     *
     * @param key 得到的key
     * @param cmp 比较函数
     * @return HashNode*
     */
    HashNode* lookup(HashNode* key, bool (*cmp)(HashNode*, HashNode*));

    /**
     * @brief 往这个map中插入一个节点
     *
     * @param node 节点
     */
    void insert(HashNode* node);

    /**
     * @brief 删除这个节点
     * @note 试探table_1 和 table_2,分别从这里面删除对应的key就行了
     * @param node 节点
     * @param cmp 比较函数
     */
    void erase(HashNode* node, bool (*cmp)(HashNode*, HashNode*));
private:
    /**
     * @brief 负载系数变得太高了，于是就开始慢慢的从2号表移动到1号表
     *
     */
    void help_resizing();

    /**
     * @brief 首次开始调整
     * 
     */
    void start_resizing();

private:
    HashTable* m_table_1 { nullptr }; ///< 第一个要调整的表结构
    HashTable* m_table_2 { nullptr }; ///< 第二个要调整的表结构
    size_t m_resizing_pos { 0 };      ///< 渐进式大小
};
} // namespace my_redis

#endif