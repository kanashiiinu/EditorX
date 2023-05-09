#include "head.hpp"
using namespace std;

struct ListNode {
	int val;
	struct ListNode *next;
	ListNode(int x) :
			val(x), next(NULL) {
	}
};

// 推荐
ListNode* reverseList(ListNode* preHead, ListNode* nowHead = nullptr) {
    if (!preHead) { return nowHead; }
    auto next = std::exchange(preHead->next, nowHead);
    return reverseList(next, preHead);
}

class Solution {
public:

ListNode* ReverseList(ListNode* head) {
    return reverseListInt_1(head);
}

private:

ListNode* reverseListInt_1(ListNode* preHead, ListNode* nowHead = nullptr) {
    if (!preHead) { return nowHead; }
    // 操作
    auto next = std::exchange(preHead->next, nowHead);
    // 下一步递归
    // 调整递归参数
    nowHead = std::exchange(preHead, next);
    return reverseListInt_1(preHead, nowHead);
}

ListNode* reverseListInt_2(ListNode* preHead, ListNode* nowHead = nullptr) {
    if (!preHead) { return nowHead; }
    auto next = std::exchange(preHead->next, nowHead);
    // =>
    return reverseListInt_2(next, preHead);
}

ListNode* reverseListInt_for_1(ListNode* preHead, ListNode* nowHead = nullptr) {
    while (preHead) {
        // 操作
        auto nextNode = std::exchange(preHead->next, nowHead);
        // 调整下一次循环/迭代
        nowHead = std::exchange(preHead, nextNode);
    }
    return nowHead;
}

ListNode* reverseListInt_for_2(ListNode* preHead, ListNode* nowHead = nullptr) {
    // =>
    for (ListNode* nextNode; preHead; nowHead = std::exchange(preHead, nextNode)) {
        nextNode = std::exchange(preHead->next, nowHead);
    }
    return nowHead;
}

};