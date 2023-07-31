#include <cstddef>
#include <iostream>
#include <memory>
using std::cout;
using std::endl;

struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

// 203. 移除链表元素
namespace _203 {

class Solution {
  public:
    ListNode* removeElements(ListNode* head, int val) {
        auto* dummy = new ListNode(0, head);
        auto* cur = dummy;

        while (cur && cur->next) {
            if (cur->next->val == val) {
                auto* old = cur->next;
                cur->next = old->next; // 'skip' next
                delete old;            // free memory
                old = nullptr;
            } else {
                // process next
                cur = cur->next;
            }
        }
        head = dummy->next;
        delete dummy;
        dummy = nullptr;
        return head;
    }
};

} // namespace _203

// 707. 设计链表
namespace _707 {

class MyLinkedList {
    size_t size;
    std::unique_ptr<ListNode> dummy{nullptr};

    ListNode* getAt(int index) {
        if (index >= size || index < 0)
            return nullptr;
        auto* cur = dummy->next;
        while (index--)
            cur = cur->next;
        return cur;
    }

  public:
    MyLinkedList() : dummy(std::make_unique<ListNode>(0, nullptr)), size{0U} {}

    int get(int index) {
        if (auto* p = getAt(index); p)
            return p->val;

        return -1;
    }

    void addAtHead(int val) {
        auto* node = new ListNode(val);
        node->next = dummy->next;
        dummy->next = node;
        size++;
    }

    void addAtTail(int val) {
        if (size == 0) {
            addAtHead(val);
            return;
        }

        getAt(size - 1)->next = new ListNode(val);
        size++;
    }

    void addAtIndex(int index, int val) {
        if (index > size)
            return;
        if (index == size) {
            addAtTail(val);
            return;
        }
        if (index == 0) {
            addAtHead(val);
            return;
        }

        auto* pre = getAt(index - 1);
        auto* node = new ListNode(val);
        node->next = pre->next;
        pre->next = node;
        size++;
    }

    void deleteAtIndex(int index) {
        if (index >= size || index < 0)
            return;

        if (index == 0) {
            auto* next = dummy->next;
            if (next)
                dummy->next = next->next;
            delete next;
            next = nullptr;
            size--;
            return;
        }

        auto* pre = getAt(index - 1);
        auto* del = pre->next;
        pre->next = del->next;
        delete del;
        del = nullptr;
        size--;
    }

    void printLinkedList() {
        auto* cur = dummy.get();
        while (cur->next != nullptr) {
            cout << cur->next->val << " ";
            cur = cur->next;
        }
        cout << endl;
    }
};

} // namespace _707

// 206. 反转链表
namespace _206 {

class Solution {
  public:
    ListNode* reverseList(ListNode* head) {
        ListNode* pre = nullptr;
        ListNode* cur = head;

        // 'init: p cur -> next
        while (cur) {

            // 保存 next，因为要断开了
            // p cur -> next
            auto* next = cur->next;

            // p <- cur next
            cur->next = pre;

            // goto: 'init
            pre = cur;
            cur = next;
        }

        return pre;
    }
};

} // namespace _206