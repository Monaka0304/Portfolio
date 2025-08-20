#include <stdio.h>
#include <stdlib.h>

// ノード構造体を定義
struct node {
    int key;
    struct node *parent, *left, *right;
};

// ノード作成関数
struct node *new_node(int key) {
    struct node *p = (struct node *)malloc(sizeof(struct node));
    p->key = key;
    p->parent = NULL;
    p->left = NULL;
    p->right = NULL;
    return p;
}

//ツリー状に表示する関数
void print_tree(struct node *t, int depth) {
    if (t == NULL) return;
    print_tree(t->right, depth + 1);
    for (int i = 0; i < depth; i++) printf("    ");
    printf("%2d\n", t->key);
    print_tree(t->left, depth + 1);
}

// メモリ解放をする関数
void free_tree(struct node *t) {
    if (t == NULL) return;
    free_tree(t->left);
    free_tree(t->right);
    free(t);
}

// 挿入関数（key=0の外点も付ける）
int insert(int key, struct node *root) {
    struct node *current = root;

    while (1) {
        if (key == current->key) {
            // すでに存在
            return 0;
        } else if (key < current->key) {
            if (current->left->key == 0) {
                struct node *new = new_node(key);
                new->parent = current;
                new->left = new_node(0);  // 外点
                new->right = new_node(0); // 外点
                new->left->parent = new;
                new->right->parent = new;
                current->left = new;
                return 1;
            } else {
                current = current->left;
            }
        } else {
            if (current->right->key == 0) {
                struct node *new = new_node(key);
                new->parent = current;
                new->left = new_node(0);  // 外点
                new->right = new_node(0); // 外点
                new->left->parent = new;
                new->right->parent = new;
                current->right = new;
                return 1;
            } else {
                current = current->right;
            }
        }
    }
}

int main() {
    struct node *root = NULL;  // 最初は空の木
    int key;

    while (1) {
        printf("挿入するキーを入力してください: ");
        scanf("%d", &key);
        if (key <= 0) break;

        if (root == NULL) {
            // 最初の入力値でルートを作成
            root = new_node(key);
            root->left = new_node(0);   // 外点
            root->right = new_node(0);  // 外点
            root->left->parent = root;
            root->right->parent = root;
            print_tree(root, 0);
            continue;
        }

        int result = insert(key, root);
        if (result == 0) {
            printf("エラー: %d はすでに存在します。\n", key);
        } else {
            print_tree(root, 0);
        }
    }

    free_tree(root);
    return 0;
}