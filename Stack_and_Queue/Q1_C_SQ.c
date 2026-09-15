//////////////////////////////////////////////////////////////////////////////////

/* CE1007/CZ1007 Data Structures
Lab Test: Section C - Stack and Queue Questions
Purpose: Implementing the required functions for Question 1 */

//////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////////

typedef struct _listnode
{
	int item;
	struct _listnode *next;
} ListNode;	// You should not change the definition of ListNode

typedef struct _linkedlist
{
	int size;
	ListNode *head;
} LinkedList;	// You should not change the definition of LinkedList


typedef struct _queue
{
	LinkedList ll;
} Queue;  // You should not change the definition of Queue

///////////////////////// function prototypes ////////////////////////////////////

// You should not change the prototypes of these functions
void createQueueFromLinkedList(LinkedList *ll, Queue *q);
void removeOddValues(Queue *q);

void enqueue(Queue *q, int item);
int dequeue(Queue *q);
int isEmptyQueue(Queue *q);
void removeAllItemsFromQueue(Queue *q);

void printList(LinkedList *ll);
ListNode * findNode(LinkedList *ll, int index);
int insertNode(LinkedList *ll, int index, int value);
int removeNode(LinkedList *ll, int index);
void removeAllItems(LinkedList *ll);

//////////////////////////// main() //////////////////////////////////////////////

int main()
{
	int c, i;
	LinkedList ll;
	Queue q;

	c = 1;

	// Initialize the linked list as an empty linked list
	ll.head = NULL;
	ll.size = 0;

	// Initialize the Queue as an empty queue
	q.ll.head = NULL;
	q.ll.size = 0;


	printf("1: Insert an integer into the linked list:\n");
	printf("2: Create the queue from the linked list:\n");
	printf("3: Remove odd numbers from the queue:\n");
	printf("0: Quit:\n");


	while (c != 0)
	{
		printf("Please input your choice(1/2/3/0): ");
		scanf("%d", &c);

		switch (c)
		{
		case 1:
			printf("Input an integer that you want to insert into the List: ");
			scanf("%d", &i);
			insertNode(&ll, ll.size, i);
			printf("The resulting linked list is: ");
			printList(&ll);
			break;
		case 2:
			createQueueFromLinkedList(&ll, &q); // You need to code this function
			printf("The resulting queue is: ");
			printList(&(q.ll));
			break;
		case 3:
			removeOddValues(&q); // You need to code this function
			printf("The resulting queue after removing odd integers is: ");
			printList(&(q.ll));
			removeAllItemsFromQueue(&q);
			removeAllItems(&ll);
			break;
		case 0:
			removeAllItemsFromQueue(&q);
			removeAllItems(&ll);
			break;
		default:
			printf("Choice unknown;\n");
			break;
		}

	}

	return 0;
}


//////////////////////////////////////////////////////////////////////////////////

/*
 * createQueueFromLinkedList
 *   n = 리스트 노드 수, m = 기존 큐 노드 수
 *   시간: O(m + n)
 *     - 비우기: 기존 큐를 한 번 순회하며 free       -> O(m)
 *     - 만들기: 리스트 한 번 순회, 노드마다 malloc 후 qtail 뒤에 O(1)로 연결 -> O(n)
 *       (qtail을 유지하므로 매번 꼬리를 다시 찾지 않음. qtail 없이 head부터 걸으면 O(n^2))
 *   공간: 결과 큐 노드 n개 = O(n) (문제가 요구하는 출력이므로 불가피)
 *         보조 공간은 포인터 변수 몇 개 = O(1)
 */
void createQueueFromLinkedList(LinkedList *ll, Queue *q)
{
	if (ll == NULL || q == NULL) { return; }

	// 기존 큐 비우기
	ListNode *del = q->ll.head;
	// for (ListNode *cur = q->ll.head, *next; cur != NULL; cur = next) {
	// 	next = cur->next;
	// 	free(cur);
	// }
	while (del != NULL) {
		ListNode *next = del->next;
		free(del);
		del = next;
	}
	q->ll.head = NULL;
	q->ll.size = 0;

	// ll의 노드를 순서대로 복사
	ListNode *tail = NULL;
	for (ListNode *cur = ll->head; cur != NULL; cur = cur->next) {
		ListNode *newNode = malloc(sizeof(ListNode));
		if (newNode == NULL) { return; }

		newNode->item = cur->item;
		newNode->next = NULL;

		if (tail == NULL) {
			q->ll.head = newNode;   // 첫 노드
		} else {
			tail->next = newNode;
		}

		tail = newNode;
		q->ll.size++;
	}
}

/*
void createQueueFromLinkedList(LinkedList *ll, Queue *q)
{
	if (ll == NULL || q == NULL) { return; }

	// q의 헤드가 NULL이 아니면 q 초기화
	ListNode *del = q->ll.head;

	while (del != NULL) {
	    ListNode *next = del->next;
		free(del);
		del = next;
	}

	// 초기화 완료
	q->ll.head = NULL;
	q->ll.size = 0;

	ListNode *qtail = NULL; 

	for (ListNode *cur = ll->head; cur != NULL; cur = cur->next) {
		ListNode *newNode = malloc(sizeof(ListNode));
		if (q->ll.head == NULL) {
			q->ll.head = newNode;
			newNode->item = cur->item;
			newNode->next = NULL;
		}
		else {
			newNode->item = cur->item;
			newNode->next = NULL;
			qtail->next = newNode;
		}
		qtail = newNode;
		q->ll.size++;
	}
}
*/


/*
 * removeOddValues — 방식 비교 (n = 큐 노드 수)
 *
 *   [A] 같은 체인 위 회전 (아래 활성 코드, 그리고 맨 아래 주석의 원본)
 *       시간 O(n): 꼬리 찾기 1회 순회 + n번 회전(각 O(1)) = 2n번 노드 방문
 *       공간 O(1): tail, node, n 뿐. 새 노드 없음, 제자리(in-place)
 *
 *   [B] 새 큐 재구성 (주석 처리된 첫 번째 버전)
 *       시간 O(n): 단일 순회. 꼬리 찾기 pass가 없어 A보다 상수배 빠름(n번 방문)
 *       공간 O(1): cur, tail, next 뿐. 노드는 옮기기만 하고 malloc 없음
 *
 *   [C] prev/cur로 제자리 삭제 (연결 리스트 표준형, 파일엔 없음)
 *       시간 O(n): 단일 순회, 공간 O(1)
 *
 *   [D] 제공 함수 사용: n번 { v = dequeue(q); 짝수면 enqueue(q, v); }
 *       시간 O(n^2): dequeue는 O(1)이지만 enqueue가 insertNode(size) -> findNode(size-1)로
 *                    매번 head부터 꼬리까지 걸음. 짝수 k개면 sum O(k) = O(n*k), 전부 짝수면 O(n^2)
 *       공간 O(1): 보조 변수만. 단 dequeue/enqueue마다 free/malloc 1회씩 발생
 *
 *   정리: 점근 복잡도는 A=B=C=O(n), O(1). 실제 방문 횟수는 B=C(n) < A(2n). D만 O(n^2).
 */


void removeOddValues(Queue *q)
{
	if (q == NULL || q->ll.head == NULL) return;

	ListNode *tail = q->ll.head;               // 현재 큐의 꼬리 찾기 -> 순회 1회, O(n)
	while (tail->next != NULL) tail = tail->next;

	for (int n = q->ll.size; n > 0; n--) {     // 원래 개수만큼만 회전 -> n회, 각 회 O(1)
		ListNode *node = q->ll.head;           // dequeue: 앞 노드 떼기
		q->ll.head = node->next;

		if (node->item % 2 != 0) {             // 홀수: 버림
			free(node);
			q->ll.size--;
		}
		else {                                 // 짝수: 뒤에 다시 붙임 (enqueue)
			node->next = NULL;
			if (q->ll.head == NULL)
				q->ll.head = node;             // 떼고 나니 빈 큐 -> node가 유일한 노드
			else
				tail->next = node;
			tail = node;
		}
	}
}



/*
[B] 새 큐 재구성 — 시간 O(n) 단일 순회, 공간 O(1)
    옛 체인은 cur로 들어내고 q를 빈 큐로 리셋한 뒤, 짝수만 tail 뒤에 O(1)로 다시 붙임.
    꼬리를 미리 찾을 필요가 없어(tail = NULL에서 시작) 순회가 한 번으로 끝남.

void removeOddValues(Queue *q)
{
	if (q == NULL) return;

	ListNode *cur = q->ll.head;
	ListNode *tail = NULL;
	q->ll.head = NULL;
	q->ll.size = 0;

	while (cur != NULL) { 
		ListNode *next = cur->next;
		cur->next = NULL;

		if (cur->item % 2 != 0) {
			free(cur);
		}
		else {
			if (tail == NULL)
				q->ll.head = cur;
			else
				tail->next = cur;
			tail = cur;
			q->ll.size++;
		}

		cur = next;
	}
}


[A-원본] 같은 체인 위 회전 — 시간 O(n) (꼬리 찾기 n + 회전 n = 2n), 공간 O(1)
    활성 코드와 같은 알고리즘. cur == q->ll.head가 항상 성립하므로 cur는 중복 변수.
    노드 1개가 짝수일 때 tail->next = cur로 자기 자신을 가리키는 순간이 생기지만
    바로 다음 줄에서 head를 다시 읽고 next를 NULL로 끊어 결과는 올바름.

void removeOddValues(Queue *q)
{
	if (q->ll.head == NULL) { return; } // q is NULL -> return

	ListNode *tail = q->ll.head;
	ListNode *cur = q->ll.head;
	int n = q->ll.size;

	while(tail->next != NULL) { tail = tail->next; } // tail->next is NULL -> exit

	for (int i = 0; i < n; i++) {
		if(cur->item % 2 != 0) { // odd
			ListNode *temp = cur;
			cur = cur->next;
			q->ll.head = cur;
			free(temp);
			q->ll.size--;
		}
		else { // even
			tail->next = cur;
			tail = tail->next;
			q->ll.head = cur->next;
			cur->next = NULL;
			cur = q->ll.head;
		} 
	}
}
*/

//////////////////////////////////////////////////////////////////////////////////

void enqueue(Queue *q, int item) {
	insertNode(&(q->ll), q->ll.size, item);
}

int dequeue(Queue *q) {
	int item;

	if (!isEmptyQueue(q)) {
		item = ((q->ll).head)->item;
		removeNode(&(q->ll), 0);
		return item;
	}
	return -1;
}

int isEmptyQueue(Queue *q) {
	if ((q->ll).size == 0)
		return 1;
	return 0;
}

void removeAllItemsFromQueue(Queue *q)
{
	int count, i;
	if (q == NULL)
		return;
	count = q->ll.size;

	for (i = 0; i < count; i++)
		dequeue(q);
}


void printList(LinkedList *ll){

	ListNode *cur;
	if (ll == NULL)
		return;
	cur = ll->head;
	if (cur == NULL)
		printf("Empty");
	while (cur != NULL)
	{
		printf("%d ", cur->item);
		cur = cur->next;
	}
	printf("\n");
}


void removeAllItems(LinkedList *ll)
{
	ListNode *cur = ll->head;
	ListNode *tmp;

	while (cur != NULL){
		tmp = cur->next;
		free(cur);
		cur = tmp;
	}
	ll->head = NULL;
	ll->size = 0;
}


ListNode * findNode(LinkedList *ll, int index){

	ListNode *temp;

	if (ll == NULL || index < 0 || index >= ll->size)
		return NULL;

	temp = ll->head;

	if (temp == NULL || index < 0)
		return NULL;

	while (index > 0){
		temp = temp->next;
		if (temp == NULL)
			return NULL;
		index--;
	}

	return temp;
}

int insertNode(LinkedList *ll, int index, int value){

	ListNode *pre, *cur;

	if (ll == NULL || index < 0 || index > ll->size + 1)
		return -1;

	// If empty list or inserting first node, need to update head pointer
	if (ll->head == NULL || index == 0){
		cur = ll->head;
		ll->head = malloc(sizeof(ListNode));
		if (ll->head == NULL)
		{
			exit(0);
		}
		ll->head->item = value;
		ll->head->next = cur;
		ll->size++;
		return 0;
	}


	// Find the nodes before and at the target position
	// Create a new node and reconnect the links
	if ((pre = findNode(ll, index - 1)) != NULL){
		cur = pre->next;
		pre->next = malloc(sizeof(ListNode));
		if (pre->next == NULL)
		{
			exit(0);
		}
		pre->next->item = value;
		pre->next->next = cur;
		ll->size++;
		return 0;
	}

	return -1;
}


int removeNode(LinkedList *ll, int index){

	ListNode *pre, *cur;

	// Highest index we can remove is size-1
	if (ll == NULL || index < 0 || index >= ll->size)
		return -1;

	// If removing first node, need to update head pointer
	if (index == 0){
		cur = ll->head->next;
		free(ll->head);
		ll->head = cur;
		ll->size--;
		return 0;
	}

	// Find the nodes before and after the target position
	// Free the target node and reconnect the links
	if ((pre = findNode(ll, index - 1)) != NULL){

		if (pre->next == NULL)
			return -1;

		cur = pre->next;
		pre->next = cur->next;
		free(cur);
		ll->size--;
		return 0;
	}

	return -1;
}
