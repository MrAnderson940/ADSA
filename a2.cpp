#include <bits/stdc++.h>
using namespace std;

// === AVL NODE ===
struct Node {
    int key;
    Node* left;
    Node* right;
    int height;

    Node(int val) {
        key = val;
        left = right = nullptr;
        height = 1;
    }
};

// === HELPER FUNCTIONS ===
int getHeight(Node* node) {
    return node ? node->height : 0;
}

int getBalance(Node* node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

Node* updateHeight(Node* node) {
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    return node;
}

// === ROTATIONS ===
Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);

    return x;
}

Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);

    return y;
}

// === REBALANCE ===
Node* rebalance(Node* node) {
    updateHeight(node);
    int balance = getBalance(node);

    // Left heavy
    if (balance > 1) {
        if (getBalance(node->left) < 0)
            node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right heavy
    if (balance < -1) {
        if (getBalance(node->right) > 0)
            node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

// === INSERT ===
Node* insert(Node* root, int key) {
    if (!root) return new Node(key);

    if (key < root->key)
        root->left = insert(root->left, key);
    else if (key > root->key)
        root->right = insert(root->right, key);
    else
        return root; // no duplicates

    return rebalance(root);
}

// === FIND MIN ===
Node* getMin(Node* node) {
    while (node->left) node = node->left;
    return node;
}

// === DELETE ===
Node* deleteNode(Node* root, int key) {
    if (!root) return root;

    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        // node with one or no child
        if (!root->left || !root->right) {
            Node* temp = root->left ? root->left : root->right;

            if (!temp) {
                delete root;
                return nullptr;
            } else {
                Node* old = root;
                root = temp;
                delete old;
            }
        } else {
            Node* temp = getMin(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
    }

    return rebalance(root);
}

// === TRAVERSALS ===
vector<int> preOrder(Node* root) {
    vector<int> res;
    if (!root) return res;

    res.push_back(root->key);
    vector<int> left = preOrder(root->left);
    vector<int> right = preOrder(root->right);

    res.insert(res.end(), left.begin(), left.end());
    res.insert(res.end(), right.begin(), right.end());

    return res;
}

vector<int> inOrder(Node* root) {
    vector<int> res;
    if (!root) return res;

    vector<int> left = inOrder(root->left);
    vector<int> right = inOrder(root->right);

    res.insert(res.end(), left.begin(), left.end());
    res.push_back(root->key);
    res.insert(res.end(), right.begin(), right.end());

    return res;
}

vector<int> postOrder(Node* root) {
    vector<int> res;
    if (!root) return res;

    vector<int> left = postOrder(root->left);
    vector<int> right = postOrder(root->right);

    res.insert(res.end(), left.begin(), left.end());
    res.insert(res.end(), right.begin(), right.end());
    res.push_back(root->key);

    return res;
}

// === MAIN ===
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string line;
    getline(cin, line);

    stringstream ss(line);
    vector<string> tokens;
    string token;

    while (ss >> token) {
        tokens.push_back(token);
    }

    Node* root = nullptr;

    // last token is traversal type
    string traversalType = tokens.back();
    tokens.pop_back();


    // process operations
    for (string &t : tokens) {
        char op = t[0];
        int value = stoi(t.substr(1));
        if (op == 'A') {
            root = insert(root, value);
        } else if (op == 'D') {
            root = deleteNode(root, value);
        }
    }

    vector<int> result;

    if (traversalType == "PRE")
        result = preOrder(root);
    else if (traversalType == "IN")
        result = inOrder(root);
    else if (traversalType == "POST")
        result = postOrder(root);

    if (result.empty()) {
        cout << "EMPTY\n";
    } else {
        for (int i = 0; i < result.size(); i++) {
            if (i) cout << " ";
            cout << result[i];
        }
        cout << "\n";
    }

    return 0;
}