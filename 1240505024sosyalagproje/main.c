//
//  main.c
//  1240505024sosyalagproje
//
//  Created by Hatice Kübra Erdem on 28.04.2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ----------------- Red-Black Tree Yapısı -----------------

typedef struct RBNode {
    int user_id;
    struct RBNode* left;
    struct RBNode* right;
    int color; // 0 = Black, 1 = Red
} RBNode;

typedef struct RBTree {
    RBNode* root;
} RBTree;

RBTree* create_rbtree() {
    RBTree* tree = (RBTree*)malloc(sizeof(RBTree));
    tree->root = NULL;
    return tree;
}

RBNode* create_rbtree_node(int user_id) {
    RBNode* node = (RBNode*)malloc(sizeof(RBNode));
    node->user_id = user_id;
    node->left = node->right = NULL;
    node->color = 1; // Yeni node kırmızı
    return node;
}

// Basit ekleme (renk düzeltmesi yok, sade versiyon)
void insert_rbtree(RBTree* tree, int user_id) {
    RBNode* node = create_rbtree_node(user_id);
    if (tree->root == NULL) {
        node->color = 0; // root her zaman siyah
        tree->root = node;
        return;
    }
    RBNode* temp = tree->root;
    while (1) {
        if (user_id < temp->user_id) {
            if (!temp->left) {
                temp->left = node;
                return;
            }
            temp = temp->left;
        } else {
            if (!temp->right) {
                temp->right = node;
                return;
            }
            temp = temp->right;
        }
    }
}

void inorder_traversal(RBNode* root) {
    if (!root) return;
    inorder_traversal(root->left);
    printf("%d ", root->user_id);
    inorder_traversal(root->right);
}

// ----------------- Graph Yapısı -----------------

typedef struct FriendNode {
    int friend_id;
    struct FriendNode* next;
} FriendNode;

typedef struct GraphNode {
    int user_id;
    FriendNode* friends;
    struct GraphNode* next;
} GraphNode;

typedef struct Graph {
    GraphNode* head;
} Graph;

Graph* create_graph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->head = NULL;
    return graph;
}

void add_user(Graph* graph, int user_id) {
    GraphNode* node = (GraphNode*)malloc(sizeof(GraphNode));
    node->user_id = user_id;
    node->friends = NULL;
    node->next = graph->head;
    graph->head = node;
}

void add_friendship(Graph* graph, int user_id1, int user_id2) {
    GraphNode* node = graph->head;
    GraphNode* user1 = NULL;
    GraphNode* user2 = NULL;
    while (node) {
        if (node->user_id == user_id1) user1 = node;
        if (node->user_id == user_id2) user2 = node;
        node = node->next;
    }
    if (!user1 || !user2) return;

    FriendNode* friend1 = (FriendNode*)malloc(sizeof(FriendNode));
    friend1->friend_id = user_id2;
    friend1->next = user1->friends;
    user1->friends = friend1;

    FriendNode* friend2 = (FriendNode*)malloc(sizeof(FriendNode));
    friend2->friend_id = user_id1;
    friend2->next = user2->friends;
    user2->friends = friend2;
}

void print_graph(Graph* graph) {
    GraphNode* node = graph->head;
    while (node) {
        printf("User %d:", node->user_id);
        FriendNode* f = node->friends;
        while (f) {
            printf(" %d", f->friend_id);
            f = f->next;
        }
        printf("\n");
        node = node->next;
    }
}

void list_user_friends(Graph* graph, int user_id) {
    GraphNode* node = graph->head;
    while (node) {
        if (node->user_id == user_id) {
            printf("User %d's friends: ", user_id);
            FriendNode* f = node->friends;
            while (f) {
                printf("%d ", f->friend_id);
                f = f->next;
            }
            printf("\n");
            return;
        }
        node = node->next;
    }
    printf("User %d bulunamadı.\n", user_id);
}

void load_data_from_file(const char* filename, RBTree* tree, Graph* graph) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Dosya açılamadı!\n");
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "USER", 4) == 0) {
            int id;
            sscanf(line + 5, "%d", &id);
            add_user(graph, id);
            insert_rbtree(tree, id);
        } else if (strncmp(line, "FRIEND", 6) == 0) {
            int id1, id2;
            sscanf(line + 7, "%d %d", &id1, &id2);
            add_friendship(graph, id1, id2);
        }
    }

    fclose(file);
}

// ----------------- Analiz Fonksiyonları -----------------

void dfs_helper(Graph* graph, int user_id, int depth, int target_depth, int* visited) {
    if (depth == target_depth) {
        printf("%d ", user_id);
        return;
    }

    GraphNode* node = graph->head;
    while (node) {
        if (node->user_id == user_id) {
            FriendNode* f = node->friends;
            while (f) {
                if (!visited[f->friend_id]) {
                    visited[f->friend_id] = 1;
                    dfs_helper(graph, f->friend_id, depth + 1, target_depth, visited);
                }
                f = f->next;
            }
        }
        node = node->next;
    }
}

void find_friends_at_distance(Graph* graph, int start_id, int distance) {
    int visited[1000] = {0};
    visited[start_id] = 1;
    printf("Mesafe %d olan arkadaşlar: ", distance);
    dfs_helper(graph, start_id, 0, distance, visited);
    printf("\n");
}

void find_common_friends(Graph* graph, int id1, int id2) {
    int friends1[1000] = {0};
    int friends2[1000] = {0};

    GraphNode* node = graph->head;
    while (node) {
        if (node->user_id == id1) {
            FriendNode* f = node->friends;
            while (f) {
                friends1[f->friend_id] = 1;
                f = f->next;
            }
        }
        if (node->user_id == id2) {
            FriendNode* f = node->friends;
            while (f) {
                friends2[f->friend_id] = 1;
                f = f->next;
            }
        }
        node = node->next;
    }

    printf("Ortak arkadaşlar: ");
    for (int i = 0; i < 1000; i++) {
        if (friends1[i] && friends2[i]) {
            printf("%d ", i);
        }
    }
    printf("\n");
}

void calculate_influence(Graph* graph, int user_id) {
    int visited[1000] = {0};
    int queue[1000];
    int front = 0, rear = 0;

    visited[user_id] = 1;
    queue[rear++] = user_id;
    int count = 0;

    while (front < rear) {
        int current = queue[front++];
        GraphNode* node = graph->head;
        while (node) {
            if (node->user_id == current) {
                FriendNode* f = node->friends;
                while (f) {
                    if (!visited[f->friend_id]) {
                        visited[f->friend_id] = 1;
                        queue[rear++] = f->friend_id;
                        count++;
                    }
                    f = f->next;
                }
            }
            node = node->next;
        }
    }

    printf("User %d'nin etki alanı: %d kişi\n", user_id, count);
}

// ----------------- Main Fonksiyonu -----------------

int main() {
    RBTree* tree = create_rbtree();
    Graph* graph = create_graph();

    load_data_from_file("veriseti.txt", tree, graph);

    int choice;
    do {
        printf("\n--- SOSYAL AĞ MENÜSÜ ---\n");
        printf("1. Kullanıcıları Listele (Red-Black Tree Sıralı)\n");
        printf("2. Arkadaşlık Ağını Göster\n");
        printf("3. Belirli Kullanıcının Arkadaşlarını Listele\n");
        printf("4. Belirli Mesafedeki Arkadaşları Bul\n");
        printf("5. İki Kullanıcının Ortak Arkadaşlarını Bul\n");
        printf("6. Kullanıcının Etki Alanını Hesapla\n");
        printf("0. Çıkış\n");
        printf("Seçiminizi girin: ");
        scanf("%d", &choice);

        int id1, id2, distance;
        switch (choice) {
            case 1:
                printf("\nKullanıcılar (Inorder Traversal):\n");
                inorder_traversal(tree->root);
                printf("\n");
                break;
            case 2:
                printf("\nArkadaşlık Ağı:\n");
                print_graph(graph);
                break;
            case 3:
                printf("Kullanıcı ID'si girin: ");
                scanf("%d", &id1);
                list_user_friends(graph, id1);
                break;
            case 4:
                printf("Başlangıç Kullanıcı ID'si girin: ");
                scanf("%d", &id1);
                printf("Mesafe girin: ");
                scanf("%d", &distance);
                find_friends_at_distance(graph, id1, distance);
                break;
            case 5:
                printf("İlk kullanıcı ID'sini girin: ");
                scanf("%d", &id1);
                printf("İkinci kullanıcı ID'sini girin: ");
                scanf("%d", &id2);
                find_common_friends(graph, id1, id2);
                break;
            case 6:
                printf("Kullanıcı ID'si girin: ");
                scanf("%d", &id1);
                calculate_influence(graph, id1);
                break;
            case 0:
                printf("Çıkılıyor...\n");
                break;
            default:
                printf("Geçersiz seçim! Tekrar deneyin.\n");
        }
    } while (choice != 0);

    return 0;
}
