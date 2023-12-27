#include "data_structure/hash_map.h"
#include <cassert>
#include <endian.h>
#include <sys/types.h>

namespace my_redis {
HashTable::HashTable(size_t n) : m_mask(n - 1), m_size(0) {

    assert(n > 0 && ((n && (n - 1)) == 0) && "n必须是 2 的幂");
}

void HashTable::insert(HashNode* node) {
    // e[idx] = b, ne[idx] = h[a], h[a]++;
    // 找到table的节点，然后将节点接到 node 后面,
    size_t pos = node->m_hash_code & m_mask;
    HashNode* next = m_table[pos];
    node->m_next = next;
    m_table[pos]->m_next = node;
    m_size++;
}
HashNode** HashTable::lookup(HashNode* key, bool (*cmp)(HashNode*, HashNode*)) {
    // 如果没有这个表
    if (!m_table) {
        return nullptr;
    }

    // 计算的位置
    size_t pos = key->m_hash_code & m_mask;
    // 从这个位置开始遍历
    HashNode** from = &m_table[pos];
    while (*from) {
        if (cmp(*from, key)) {
            return from;
        }
        // 移动到下一位
        from = &((*from)->m_next);
    }

    return nullptr;
}

HashNode* HashTable::detach(HashNode** from) {
    // what the fuck ? 好像有点点怪
    /// TODO: 想一下
    HashNode* deatch_node = *from;
    *from = (*from)->m_next;
    m_size--;
    return deatch_node;
}

HashNode* HashMap::lookup(HashNode* key, bool (*cmp)(HashNode*, HashNode*)) {
    // 帮助调整链表的
    start_resizing();

    HashNode** from = m_table_1->lookup(key, cmp);
    if (!from) {
        from = m_table_2->lookup(key, cmp);
    }

    if (!from) {
        return nullptr;
    }
    return *from;
}
void HashMap::help_resizing() {
    // 从 2 号的开始转移到 1 号
    if (m_table_2->m_table == nullptr) {
        return;
    }

    size_t new_work { 0 };
    while(new_work < k_resizing_work && m_table_2->m_size > 0) {
        HashNode** from = &m_table_2->m_table[m_resizing_pos];
        if (!(*from)) {
            m_resizing_pos++;
            continue;
        }
        m_table_1->insert(m_table_2->detach(from));
        new_work++;
    }
    if (m_table_2->m_size == 0) {
        free(m_table_2->m_table);
        m_table_2 = new HashTable();
    }
}
void HashMap::insert(HashNode* node) {
    // 如果第一个表没有的话
    if (!m_table_1) {
        m_table_1 = new HashTable(4);
    }
    m_table_1->insert(node);
    
    if (!m_table_2) {
        size_t loader_factor = m_table_1->m_size / (m_table_1->m_mask + 1);
        if (loader_factor > k_max_load_factor) {
            start_resizing();
        } 
    }
    help_resizing();
    
}
void HashMap::start_resizing() {
    assert(m_table_2 == nullptr && "m_table_2 必须是空的");
    m_table_2 = m_table_1;
    // 进行扩大
    m_table_1 = new HashTable((m_table_1->m_mask + 1) * 2);
    m_resizing_pos = 0;
}

void HashMap::erase(HashNode* node, bool (*cmp)(HashNode*, HashNode*)) {
    help_resizing();
    HashNode** cur = m_table_1->lookup(node,cmp);
    if (cur) {
        m_table_1->detach(cur);
    }
    cur = m_table_2->lookup(node,cmp);

    if (cur) {
        m_table_2->detach(cur);
    }

}
} // namespace my_redis