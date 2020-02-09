#include<stdio.h>
#include<stdlib.h>

#define RED 0
#define BLACK 1

struct RBTreeNode {
	int value;
	int color;
    struct RBTreeNode* left, *right, *parent;
	};

void flipColor(struct RBTreeNode* node){
		(node -> color) = 1- (node-> color);
}

struct RBTreeNode * singleLeftRotation(struct RBTreeNode * gp){
   struct RBTreeNode* p = gp -> right;
   gp-> right = p-> left;
   if (p-> left != NULL) p-> left -> parent = gp;

   p -> parent = gp -> parent;

   p -> left = gp;
   gp -> parent =p;

  flipColor(gp);
   flipColor(p);
   return p;
}

struct RBTreeNode * singleRightRotation(struct RBTreeNode * gp){
   struct RBTreeNode* p = gp -> left;

   gp-> left = p-> right;
   if(p-> right !=NULL) p-> right -> parent = gp;

   p-> parent = gp-> parent;

   p -> right = gp;
   gp-> parent = p;

   flipColor(gp);
   flipColor(p);
   return p;
}

struct RBTreeNode * doubleLeftRightRotation(struct RBTreeNode * gp){
	struct RBTreeNode* c,  *p;
	p = gp-> left;
	c = p-> right;

	p-> right = c-> left;
	if (c-> left != NULL) c-> left -> parent = p;

	c->left = p;
	p-> parent  = c;

	gp-> left = c;
	c-> parent = gp;

	return singleRightRotation(gp);
}

struct RBTreeNode * doubleRightLeftRotation(struct RBTreeNode * gp){
	struct RBTreeNode *c, *p;
	p = gp-> right;

	c = p-> left;

	p-> left = c-> right;
	if (c-> right != NULL) c-> right -> parent = p;

	c->right = p;
	p-> parent  = c;

	gp-> right = c;
	c-> parent = gp;


	return singleLeftRotation(gp);
}


int isRoot (struct RBTreeNode * node){
	if (node-> parent == NULL) return 1;
	return 0;
}
int getColor (struct RBTreeNode * node){
	if (node == NULL) return BLACK;
	else return node-> color;
}

struct RBTreeNode * getUncle(struct RBTreeNode * node){
	struct RBTreeNode * p, *gp;
	p = node -> parent;
	gp = p -> parent;
	if (p == gp -> left) return gp-> right;
	else return gp-> left;
}
struct RBTreeNode * insert(struct RBTreeNode *root, int v) {
	struct RBTreeNode *newNode, *x, *p, *gp, *uncle;
	newNode = (struct RBTreeNode *) malloc(sizeof(struct RBTreeNode));
	if (newNode==NULL){
		perror("malloc");
		exit(1);
	}
	newNode-> value = v;
	newNode-> color = RED;
	newNode-> left = NULL;
	newNode-> right = NULL;

	if (root==NULL){
		newNode-> color = BLACK;
		newNode-> parent = NULL;
		return newNode;
	}
	p =root;
	while(1){
  	  if (v < p -> value) {
	    if (p-> left == NULL) {p-> left = newNode; newNode-> parent = p; break;}
	    p = p-> left;
	  } else {
	    if (p-> right == NULL) {p-> right = newNode; newNode-> parent =p; break;}
	    p = p-> right;
	  }
	}
	x= newNode;
	while(1) {
	  p = x -> parent;
	  if (p -> color == BLACK)  break;
	  gp = p -> parent;
	  uncle = getUncle(newNode);
	  if (getColor(uncle) == RED) {
	    p-> color = BLACK;
	    uncle->color = BLACK;
	    gp-> color =RED;
	    if (isRoot(gp)) {root=gp; gp-> color =BLACK; break;}
	    x = gp;
	    continue;
	  } else {
	    if (p == gp-> left) {
		if (newNode == p-> left){
		  if (isRoot(gp)) root = singleRightRotation(gp);
		  else if (gp == gp-> parent -> left) gp-> parent -> left = singleRightRotation(gp);
		  else gp-> parent -> right = singleRightRotation(gp);
		}
		else {
		  if (isRoot(gp)) root = doubleLeftRightRotation(gp);
		  else if (gp == gp -> parent -> left) gp -> parent -> left = doubleLeftRightRotation(gp);
		  else gp-> parent -> right = doubleLeftRightRotation(gp);
		}
	    } else {
		if (newNode == p-> right) {
		  if (isRoot(gp)) root = singleLeftRotation(gp);
		  else if (gp == gp -> parent -> left) gp -> parent -> left = singleLeftRotation(gp);
		  else gp -> parent -> right =  singleLeftRotation(gp);
		}
		else {
		  if (isRoot(gp)) root = doubleRightLeftRotation(gp);
		  else if (gp == gp -> parent -> left) gp -> parent -> left = doubleRightLeftRotation(gp);
		  else gp -> parent -> right = doubleRightLeftRotation(gp);
		}
	    }
	    break;
 	  }
	}
	return root;
}

struct RBTreeNode* getsibling(struct RBTreeNode* root)
{
    struct RBTreeNode* p;
    p= root->parent;
    if(root==p->left)
        return p->right;
    else
        return p->left;
}
/*struct RBTreeNode* dark(struct RBTreeNode* root)
{
    struct RBTreeNode* sibling = getsibling(root);
    struct RBTreeNode* parent = root->parent;
    root->color = (root->color-1);
    sibling ->color = (sibling->color)-1;
    parent->color = (parent->color)+1;

    if((parent->color == 2))
    {
        if((parent->parent!= NULL))
            a(parent);

        else
            parent->color == 1;
    }

}

struct RBTreeNode* delete(struct RBTreeNode* root, int val)
{
	struct RBTreeNode* ptr, *temp;
	struct RBTreeNode* parent = root->parent;
	struct RBTreeNode* sibling = getsibling(root);
	if(root == NULL)
		return root;

	else if(val<(root->value))
		root->left=delete(root->left,val);

	else if(val>(root->value))
		root->right = delete(root->right,val);

	else
	{
		if((root->left)==NULL && (root->right)==NULL && (root->color)==RED)
		{
			free(root);
			return NULL;
		}

		else if((root->color==BLACK) && (root->left==NULL) && (root->right)->color==RED)
		{
			temp=root->right;
			temp->parent = root->parent;
			free(root);
			temp->color = BLACK;
			return temp;
		}

		else if((root->color==BLACK) && (root->right==NULL) && (root->left)->color==RED)
		{
			temp=root->left;
			temp->parent = root->parent;
			free(root);
			temp->color = BLACK;
			return temp;
		}

		else if((root->color == BLACK) && ((((root->left)->color) == BLACK) || ((root->right)->color)==BLACK))
		{
            if((sibling->color == BLACK) && (((sibling->left)->color==RED)||((sibling->right)->color==RED)))
            {
                if((parent->left==sibling)&&(sibling->right==NULL))
                    return singleRightRotation(root);

                if((parent->left==sibling)&&(sibling->left==NULL))
                    return doubleLeftRightRotation(root);

                if((parent->right==sibling)&&(sibling->left==NULL))
                    return singleLeftRotation(root);

                if((parent->right==sibling)&&(sibling->right==NULL))
                    return doubleRightLeftRotation(root);
            }

            else if((sibling->color == BLACK) && (((sibling->left)->color==BLACK)||((sibling->right)->color==BLACK)))
            {
                a(root);
            }
		}
	}
return root;
}*/

void traverse(struct RBTreeNode* root){
	if (root==NULL)return;
	traverse(root->left);
	printf ("%d %s\n", root->value, (root->color)?"Black":"Red");
	traverse(root->right);
}

int main(){
	struct RBTreeNode * root=NULL;
	int value;

	printf("Enter node values, -1 to end\n");
	while(1){
		scanf("%d", &value);
		if (value==-1) break;

		root = insert(root, value);
		traverse(root);
	}
	printf("displaying tree\n");
	traverse(root);

	printf("Enter the node to be deleted : ");
	scanf("%d",&value);
	traverse(root);
}



/*OUTPUT
Enter node values, -1 to end
2
2 Black
4
2 Black
4 Red
6
2 Red
4 Black
6 Red
7
2 Black
4 Black
6 Black
7 Red
5
2 Black
4 Black
5 Red
6 Black
7 Red
9
2 Black
4 Black
5 Black
6 Red
7 Black
9 Red
-1
displaying tree
2 Black
4 Black
5 Black
6 Red
7 Black
9 Red
*/
