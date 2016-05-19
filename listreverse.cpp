/*
 * list sort test
 * platform: win32, visual studio 2005/2010; Linux, gcc4.1.2
 */

#include <iostream>
using namespace std;

struct List_Node
{
    int index_;
    int data_;
    List_Node* next_;

public:
    List_Node(int index = 0, int data = 0):
        index_(index), data_(data), next_(0)
    {}

    void dump()
    {
        cout << index_ << "(" << data_ << ")";
        if (next_)
            cout << " -> ";
        else
            cout << endl;
    }
};

/*
 * simple insertion sort
 */
List_Node* sort2(List_Node* head)
{
    if (NULL == head)
    {
        cout << "parameter is null" << endl;
        return NULL;
    }

    List_Node *cur = NULL, *tail = head;
    while (tail)
    {
        cur = tail->next_;  //from the second node on

        if (!cur)  //tail is the last node
            break;

        //less than the head, and insert cur node before head, in this case, don't need to move tail
        if (cur->index_ < head->index_)
        {
            tail->next_ = cur->next_;
            cur->next_ = head;
            head = cur;
            continue;
        }

        //else, greater than the tail, only need to move tail pointer
        if (cur->index_ >= tail->index_)
        {
            tail = tail->next_;
            continue;
        }

        //else, greater than the head, and less than the tail, then find the position to insert
        //and in this case, don't need to move tail
        List_Node *node = head;
        List_Node *prev = node;
        while (node->index_ < cur->index_ && node != tail)  
        {
            prev = node;
            node = node->next_;
        }

        //insert this cur node, after prev and before node
        prev->next_ = cur;
        tail->next_ = cur->next_;
        cur->next_ = node;
    }

   return head;
}

List_Node* reverse(List_Node* head)
{
    if (head == NULL)
    {
    	cout << "Invalid list" << endl;
        return NULL;
    }

	if (head->next_ == NULL)
		return head;  //no need to reverse since only 1 node

	List_Node *prev = head;
    List_Node *cur = head->next_;
	List_Node *next = cur->next_;

    if (next == NULL)  // 2 nodes
    {
        cur->next_ = prev;
        prev->next_ = NULL;
        head = cur;
        return head;        
    }

    head->next_ = NULL;
    while (cur != NULL)
    {
		cur->next_ = prev;  //put cur node right after tail
		prev = cur;
		cur = next;
        if (next)
            next = next->next_;
	}

	head = prev;
    return head;
}

List_Node* create(int index[], int data[], int n)
{
    List_Node *head = new List_Node(index[0], data[0]);
    List_Node *tail = head;

    for (int loop = 1; loop < n; loop++)
    {
        List_Node *node = new List_Node(index[loop], data[loop]);
        tail->next_ = node;
        tail = node;
    }
    return head;
}

List_Node* tail(List_Node* head)
{
    List_Node *t = head;
	while (t->next_)
		t = t->next_;

    return t;
}

void dump(List_Node* head)
{
    List_Node *p = head;
    while (p != NULL)
    {
        p->dump();
        p = p->next_;
    }
}

void test(int index[], int data[], int n)
{
    List_Node *head = create(index, data, n);

    cout << "original  : ";
    dump(head);

    if ((head = reverse(head)) == NULL)
        cout << "failed to reverse the list" << endl;
    else
    {
        cout << "reverse: ";
        dump(head);
    }
    cout << endl;
}

void test1()
{
    int index[] = {49};
    int data[] = {490};
    test(index, data, 1);
}

void test2()
{
    int index[] = {49, 38};
    int data[] = {490, 380};    
    test(index, data, 2);
}

void test22()
{
    int index[] = {38, 49};
    int data[] = {380, 490};    
    test(index, data, 2);
}

void test3()
{
    int index[] = {49, 38, 65};
    int data[] = {490, 380, 650};
    test(index, data, 3);
}

void test10()
{
    int index[] = {49, 38, 65, 97, 76, 13, 27, 49, 55, 4};
    int data[] = {490, 380, 650, 970, 760, 130, 270, 490, 550, 40};
    test(index, data, 10);    
}

int main(int argc, char** argv)
{
    test1();
    test2();
    test22();
    test3();
    test10();
    return 0;
}

