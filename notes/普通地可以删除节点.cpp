#include "head.hpp"
using namespace std;
struct ListNode {
	int val;
	struct ListNode *next;
	ListNode(int x) :
			val(x), next(NULL) {
	}
};

auto remove_node(ListNode* node, int n = 1)-> ListNode* {
    while (n--) {
        if (not node) {return node;}
        delete std::exchange(node, node->next);
    }
    return node;
};