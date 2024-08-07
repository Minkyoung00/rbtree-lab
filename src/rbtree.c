#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
//////////////////////////////////////////////////////////////////
void printTree(rbtree *t, node_t *node){
    if(t == NULL || node == t->nil) return;

    printf("%d(%d) ", node->key,node->color);
    printTree(t, node->left);
    printTree(t, node->right);
}
//////////////////////////////////////////////////////////////////
rbtree *new_rbtree(void) {
  // TODO: initialize struct if needed
  rbtree *t = (rbtree *)calloc(1, sizeof(rbtree));

  t->nil = (node_t *)malloc(sizeof(node_t));
  (t->nil)->color = RBTREE_BLACK;
  (t->nil)->parent = (t->nil)->left = (t->nil)->right = t->nil;

  t->root = t->nil;

  return t;
} 

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if (t == NULL || t->root == t->nil){
    // free(t->nil);
    free(t);
    return;
  }

  node_t *cur = t->root;

  // 왼쪽이 없을 때
  if (cur->left == t->nil){     
    t->root = cur->right;             // 오른쪽 자식이 없으면 nil, 있으면 그 노드 포인터값
    cur->right->parent = cur->parent; // 오른쪽 자식의 parent를 현재 노드의 parent로
  }
  // 오른쪽이 없을 때
  else if (cur->right == t->nil){ 
    t->root = cur->left;            
    cur->left->parent = cur->parent; 
  }
  // 자식이 다 있을 때
  else{                           
    node_t *successor = cur->right;
    while (successor->left != t->nil){
      successor = successor->left;
    }
  
    // 후임자가 현재노드의 다음 노드가 아닐 때
    if (successor->parent != cur){
      successor->parent->left = successor->right;
      successor->right->parent = successor->parent;
      successor->right = cur->right;
      cur->right->parent = successor;
    }

    t->root = successor;   
    successor->parent = cur->parent;

    successor->left = cur->left; 
    cur->left->parent = successor;
  }
  free(cur);
  delete_rbtree(t);
}

void rotate_left(rbtree *t, node_t *p){
  node_t *v;
  v = p->right;
  p->right = v->left;
  
  if (v->left != t->nil){
    v->left->parent = p;
  }

  v->parent = p->parent;
  if (p->parent == t->nil)
    t->root = v;
  else if(p->parent->left == p)
    p->parent->left = v;
  else p->parent->right = v;

  v->left = p;
  p->parent = v;
}

void rotate_right(rbtree *t, node_t *p){
  node_t *v;
  v = p->left;
  p->left = v->right;
  
  if (v->right != t->nil){
    v->right->parent = p;
  }

  v->parent = p->parent;
  if (p->parent == t->nil)
    t->root = v;
  else if(p->parent->left == p)
    p->parent->left = v;
  else p->parent->right = v;

  v->right = p;
  p->parent= v;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  if (t == NULL) return NULL;

  //// 일반적인 이진트리와 동일한 방식으로 삽입 
  // 트리 구조를 따라 삽입할 위치 탐색
  node_t *cur = t->root, *p = t->nil;
    
  while (cur != t->nil){    
    p = cur;

    if (key < cur->key ) cur = cur->left;
    else cur = cur->right;
  }

  // 삽입할 노드 메모리 할당
  node_t *new = (node_t *)malloc(sizeof(node_t));
  new->key = key;
  new->color = RBTREE_RED;
  new->left = new->right = t->nil;

  new->parent = p;

  // 포인터 조정
  if (p == t->nil) t->root = new;
  else if (key < p->key) p->left = new;
  else p->right = new;

  //// RB트리 속성 위반 여부 확인 & 조정
  // 부모 노드가 RED인 경우(4번 속성 위반)
  while (new->parent->color == RBTREE_RED){ 
    ///부모 노드가 조부모의 좌측 노드
    if (new->parent == new->parent->parent->left){
      node_t *uncle;
      uncle = new->parent->parent->right;

      // 삼촌 노드가 RED(case 1)
      if (uncle->color == RBTREE_RED){
        new->parent->color = RBTREE_BLACK;
        uncle-> color =RBTREE_BLACK;
        new->parent->parent->color = RBTREE_RED;
        new = new->parent->parent;
      }

      else{
        // case 2
        if (new == new->parent->right){
          
          // 부모 노드로 왼쪽 회전
          rotate_left(t, new->parent);
          new = new->left;
        }
        // case 3
        new->parent->color = RBTREE_BLACK;
        new->parent->parent->color = RBTREE_RED;

        // 오른쪽 회전 
        new = new->parent;
        rotate_right(t, new->parent);
        new = new->left;
      }
    }
    ///부모 노드가 조부모의 우측 노드
    else{
      node_t *uncle;
      uncle = new->parent->parent->left;

      // 삼촌 노드가 RED(case 1)
      if (uncle->color == RBTREE_RED){
        new->parent->color = RBTREE_BLACK;
        uncle-> color =RBTREE_BLACK;
        new->parent->parent->color = RBTREE_RED;
        new = new->parent->parent;
      }

      else{
        // case 2
        if (new == new->parent->left){
          // 부모 노드로 오른쪽 회전
          rotate_right(t, new->parent);
          new = new->right;
        }

        // case 3
        new->parent->color = RBTREE_BLACK;
        new->parent->parent->color = RBTREE_RED;
        
        // 왼쪽 회전 
        new = new->parent;
        rotate_left(t, new->parent);
        new = new->right;
      }
    }
  }
  
  // root 노드 BLACK으로 설정(2번 속성 만족)
  t->root->color = RBTREE_BLACK;
  
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  if (t == NULL || t->root == t->nil) return NULL;

  node_t *cur = t->root;
  while (cur != t->nil){
    if (cur->key == key) return cur;
  
    if (key < cur->key ) cur = cur->left;
    else cur = cur->right;
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  if(t == NULL || t->root == t->nil) return NULL;

  node_t *cur = t->root;
  while (cur->left != t->nil) cur = cur->left;
  return cur;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  if(t == NULL || t->root == t->nil) return NULL;

  node_t *cur = t->root;
  while (cur->right != t->nil) cur = cur->right;
  return cur;
}

void transplant(rbtree *t, node_t* x, node_t* y){
  if (x->parent == t->nil){ 
      t->root = y;             
  }
  else if (x == x->parent->left){
    x->parent->left = y;
  }
  else{
    x->parent->right = y;
  }
  y->parent = x->parent;   
}

void RB_Delete_Fix(rbtree *t, node_t *x){
  while(x->color == RBTREE_BLACK && x != t->root)
    // 부모 노드의 좌측 노드일 때
    if (x == x->parent->left) {
      node_t *sibling = x->parent->right;
      // case 1 (형제가 RED)
      if (sibling->color == RBTREE_RED){
        sibling->color = RBTREE_BLACK;
        sibling->parent->color = RBTREE_RED;
        rotate_left(t, x->parent);
        sibling = x->parent->right;
      }
      // 형제가 BLACK
      // case 2 (형제의 자식이 모두 BLACK)
      if (sibling->left->color == RBTREE_BLACK && sibling->right->color ==RBTREE_BLACK){
        sibling->color = RBTREE_RED;
        x = x->parent;
      } 
      else{
        // case 3
        if (sibling->right->color == RBTREE_BLACK){
          sibling->color = RBTREE_RED;
          sibling->left->color = RBTREE_BLACK;
          rotate_right(t, sibling);
          sibling = x->parent->right;
        }
        // case 4
        sibling->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        sibling->right->color = RBTREE_BLACK;
        rotate_left(t, x->parent);
        x = t->root;
      }
    }
    // 부모 노드의 우측노드일 때
    else{
      node_t *sibling = x->parent->left;
      // case 1
      if (sibling->color == RBTREE_RED){
        sibling->color = RBTREE_BLACK;
        sibling->parent->color = RBTREE_RED;
        rotate_right(t, x->parent);
        sibling = x->parent->left;
      }
      // case 2
      if (sibling->left->color == RBTREE_BLACK && sibling->right->color ==RBTREE_BLACK){
        sibling->color = RBTREE_RED;
        x = x->parent;
      }
      else{
        // case 3
        if (sibling->left->color == RBTREE_BLACK){
          sibling->color = RBTREE_RED;
          sibling->right->color = RBTREE_BLACK;
          rotate_left(t, sibling);
          sibling = x->parent->left;
        }
        // case 4
        sibling->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        sibling->left->color = RBTREE_BLACK;
        rotate_right(t, x->parent);
        x = t->root;
      }
    }
  // RED_and_BLACK 노드 OR 루트 노드 BLACK 처리
  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  if (t == NULL || t->root == t->nil){
    return 0;
  }

  color_t del_color;
  node_t *replace;

  // 왼쪽이 없을 때
  
  if (p->left == t->nil){  
    replace = p->right;  

    transplant(t, p, replace);
    
    del_color = p->color;
  }
  // 오른쪽이 없을 때
  else if (p->right == t->nil){ 
    replace = p->left;

    transplant(t, p, replace);
    
    del_color = p->color;   
  }
  // 자식이 다 있을 때
  else{                           
    // replace = p->right;
    // while (replace->left != t->nil){
    //   replace = replace->left;
    // }

    // 후임자가 현재노드의 다음 노드가 아닐 때
    // if (replace->parent != p){
    //   replace->parent->left = replace->right;
    //   replace->right->parent = replace->parent;
    //   replace->right = p->right;
    //   p->right->parent = replace;
    // }

    // transplant(t, p, replace);      

    // replace->left = p->left; 
    // p->left->parent = replace;
    
    // del_color = replace->color;
    // replace->color = p->color;

    node_t *predecessor;
    predecessor = p->left;
    while (predecessor->right != t->nil){
      predecessor = predecessor->right;
    }

    del_color = predecessor->color;

    replace = predecessor->left;

    if (predecessor->parent != p){
      predecessor->parent->right = replace;
      replace->parent = predecessor->parent;
      
      predecessor->left = p->left;
      p->left->parent = predecessor;
    }
    else{
      replace->parent = predecessor;   // replace가 nil 노드일 수 있기 때문에 필요
    }

    transplant(t, p, predecessor);      

    predecessor->right = p->right; 
    p->right->parent = predecessor;
    
    predecessor->color = p->color;
  }

  if (del_color == RBTREE_BLACK) RB_Delete_Fix(t, replace);
  
  free(p);
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {  // arr : 빈 배열 
  // TODO: implement to_array
  // 중위순회
  if (t == NULL || t->root == t->nil) return 0;

	node_t *cur = t->root, *pre;
  int i = 0;

	while(cur != t->nil){
    if (cur->left == t->nil){
      arr[i++] = cur->key;
      cur = cur->right;
    }
    else{
      pre = cur->left;

      while(pre->right != t->nil && pre->right != cur){
        pre = pre->right;
      }
      
      if (pre->right == t->nil){
        pre->right = cur;
        cur = cur->left;
      }
      else {
        pre->right = t->nil;
        arr[i++] = cur->key;
        cur = cur->right;
      }
    }
	}

  return 0;
}