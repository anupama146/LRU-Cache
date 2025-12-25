#include <iostream>
#include <unordered_map>
using namespace std;

/*
 Node structure for doubly linked list
*/
class Node {
public:
    int key;
    int value;
    Node* prev;
    Node* next;

    Node(int k, int v) {
        key = k;
        value = v;
        prev = nullptr;
        next = nullptr;
    }
};

class LRUCache {
private:
    int capacity;
    unordered_map<int, Node*> cache;

    Node* head; // Most recently used
    Node* tail; // Least recently used

    // Remove a node from the doubly linked list
    void removeNode(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    // Insert a node right after head (MRU position)
    void insertAtFront(Node* node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }

public:
    LRUCache(int cap) {
        capacity = cap;

        // Dummy head and tail nodes
        head = new Node(0, 0);
        tail = new Node(0, 0);

        head->next = tail;
        tail->prev = head;
    }

    int get(int key) {
        if (cache.find(key) == cache.end()) {
            return -1; // Key not found
        }

        Node* node = cache[key];

        // Move accessed node to front (most recently used)
        removeNode(node);
        insertAtFront(node);

        return node->value;
    }

    void put(int key, int value) {
        // If key already exists, update value and move to front
        if (cache.find(key) != cache.end()) {
            Node* node = cache[key];
            node->value = value;

            removeNode(node);
            insertAtFront(node);
        }
        else {
            // If cache is full, remove LRU item
            if (cache.size() == capacity) {
                Node* lru = tail->prev;

                removeNode(lru);
                cache.erase(lru->key);
                delete lru;
            }

            // Insert new node
            Node* newNode = new Node(key, value);
            insertAtFront(newNode);
            cache[key] = newNode;
        }
    }
};

// Driver code for testing
int main() {
    LRUCache cache(2);

    cache.put(1, 10);
    cache.put(2, 20);
    cout << cache.get(1) << endl; // 10

    cache.put(3, 30);             // Evicts key 2
    cout << cache.get(2) << endl; // -1

    cache.put(4, 40);             // Evicts key 1
    cout << cache.get(1) << endl; // -1
    cout << cache.get(3) << endl; // 30
    cout << cache.get(4) << endl; // 40

    return 0;
}
