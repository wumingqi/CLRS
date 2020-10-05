/*
* 红黑树
*/

#include <iostream>

enum class COLOR
{
    RED,
    BLACK
};

inline bool operator==(COLOR left, COLOR right) { return (int)(left) == (int)right; };

template <typename _KeyType>
struct rb_tree_node
{
	using key_type = _KeyType;
	using key_reference_type = _KeyType&;
	using key_const_reference_type = const _KeyType&;

	key_type key;
	rb_tree_node* p;
	rb_tree_node* left;
	rb_tree_node* right;
	COLOR color;
};

template<typename _KeyType>
struct rb_tree
{
	// 基本类型定义
	using node_type = rb_tree_node<_KeyType>;
	using node_pointer_type = rb_tree_node<_KeyType>*;

	using key_type = typename node_type::key_type;
	using key_reference_type = typename node_type::key_reference_type;
	using key_const_reference_type = typename node_type::key_const_reference_type;

	// 根节点
	node_pointer_type root;

	// 空节点
	node_pointer_type nil_node;

	rb_tree()
	{
		nil_node = new node_type();
		nil_node->p = nullptr;
		nil_node->left = nullptr;
		nil_node->right = nullptr;
		nil_node->key = {};
		nil_node->color = COLOR::BLACK;

		root = nil_node;
	}

	void insert_key(key_const_reference_type key)
	{
		node_pointer_type z = new node_type();
		z->p = nil_node;
		z->left = nil_node;
		z->right = nil_node;
		z->key = key;
		z->color = COLOR::RED;

		node_pointer_type y = nil_node;
		node_pointer_type x = root;
		while (x != nil_node)
		{
			y = x;
			if (key < x->key)
				x = x->left;
			else
				x = x->right;
		}
		z->p = y;
		if (y == nil_node)
			root = z;
		else if (z->key < y->key)
			y->left = z;
		else
			y->right = z;

		// 插入新的节点可能会导致红黑性质被破坏，调用此函数修复
		insert_key_fixup(z);
	}

	node_pointer_type search(key_const_reference_type key)
	{
		node_pointer_type x = root;
		while (x != nil_node && x->key != key)
		{
			if (key < x->key)
				x = x->left;
			else
				x = x->right;
		}
		return x;
	}

	void delete_key(key_const_reference_type key)
	{
		auto z = search(key);
		if (z != nil_node)
		{
			auto y = z;
			decltype(z) x;
			COLOR y_original_color = z->color;
			if (z->left == nil_node)
			{
				x = z->right;
				trans_plant(z, z->right);
			}
			else if (z->right == nil_node)
			{
				x = z->left;
				trans_plant(z, z->left);
			}
			else
			{
				y = tree_minimum(z->right);
				y_original_color = y->color;
				x = y->right;
				if (y->p == z)
				{
					x->p = y;
				}
				else
				{
					trans_plant(y, y->right);
					y->right = z->right;
					y->right->p = y;
				}
				trans_plant(z, y);
				y->left = z->left;
				y->left->p = y;
				y->color = z->color;
			}
			if (y_original_color == COLOR::BLACK)
			{
				// 删除的节点颜色是黑色时，会破坏红黑性质，需要进行修复
				delete_key_fixup(x);
			}
		}
	}

private:
    /* （不限于红黑树的）二叉搜索树左旋操作，即使用某个节点的右孩子（y）取代该节点（x）
     原节点（x）成为其右孩子（y）的左孩子，右孩子的左孩子成为原节点新的右孩子
         x                                         y
        / \                                       / \
       /   \       --------------------->        /   \
      a     y          (left rotate)            x     c
           / \                                 / \
          b   c                               a   b
    */
	void left_rotate(node_pointer_type x)
	{
		// x的右孩子
		node_pointer_type y = x->right;

		// 1、使用y的左孩子代替y成为x的右孩子
		x->right = y->left;
		if (y->left != nil_node)
			y->left->p = x;

		// 2、使用y代替x，占有x原来的位置
		y->p = x->p;
		if (x->p == nil_node)
			root = y;
		else if (x == x->p->left)
			x->p->left = y;
		else
			x->p->right = y;

		// 3、让x成为y的左孩子
		y->left = x;
		x->p = y;
	}

    /* （不限于红黑树的）二叉搜索树右旋操作，即使用某个节点的左孩子（x）取代该节点（y）
     原节点（y）成为其左孩子（x）的右孩子，左孩子的右孩子成为原节点新的左孩子
          y                                      x     
         / \                                    / \     
        /   \      --------------------->      /   \     
       x     c        (right rotate)          a     y     
      / \                                          / \ 
     a   b                                        b   c
    */
	void right_rotate(node_pointer_type y)
	{
		// y的左孩子
		node_pointer_type x = y->left;
		
		// 1、使用x的右孩子代替x成为y的左孩子
		y->left = x->right;
		if (x->right)
			x->right->p = y;

		// 2、使用x代替y，占有y原来的位置
		x->p = y->p;
		if (y->p == nil_node)
			root = x;
		else if (y == y->p->left)
			y->p->left = x;
		else
			y->p->right = x;

		// 3、让y成为x的右孩子
		x->right = y;
		y->p = x;
	}

	// 插入修复，通过旋转和着色保持红黑性质
	void insert_key_fixup(node_pointer_type z)
	{
		while (z->p->color == COLOR::RED)
		{
			// 当z的父节点是祖父节点的左孩子时
			if (z->p == z->p->p->left)
			{
				// y是z的叔节点
				node_pointer_type y = z->p->p->right;
				if (y->color == COLOR::RED)
				{
					z->p->color = COLOR::BLACK;
					y->color = COLOR::BLACK;
					z->p->p->color = COLOR::RED;
					z = z->p->p;
				}
				else
				{
					if (z == z->p->right)
					{
						z = z->p;
						left_rotate(z);
					}
					z->p->color = COLOR::BLACK;
					z->p->p->color = COLOR::RED;
					right_rotate(z->p->p);
				}
			}
			// 当z的父节点是祖父节点的右孩子时
			else
			{
				// y是z的叔节点
				node_pointer_type y = z->p->p->left;
				if (y->color == COLOR::RED)
				{
					z->p->color = COLOR::BLACK;
					y->color = COLOR::BLACK;
					z->p->p->color = COLOR::RED;
					z = z->p->p;
				}
				else
				{
					if (z == z->p->left)
					{
						z = z->p;
						right_rotate(z);
					}
					z->p->color = COLOR::BLACK;
					z->p->p->color = COLOR::RED;
					left_rotate(z->p->p);
				}
			}
		}
		root->color = COLOR::BLACK;
	}

	void trans_plant(node_pointer_type u, node_pointer_type v)
	{
		if (u->p == nil_node)
		{
			root = v;
		}
		else if (u == u->p->left)
			u->p->left = v;
		else
			u->p->right = v;

		// 此处无论何时都会给节点v重新赋新父节点，即使v是“空节点”
		// 因为delete_key_fixup函数需要访问节点x的父节点，需要保证x->p始终存在
		v->p = u->p;
	}

	node_pointer_type tree_minimum(node_pointer_type root)
	{
		node_pointer_type y = nil_node;
		while (root != nil_node)
		{
			y = root;
			root = root->left;
		}
		return y;
	}

	void delete_key_fixup(node_pointer_type x)
	{
		while (x != root && x->color == COLOR::BLACK)
		{
			if (x == x->p->left)
			{
				// w是x的兄弟节点
				node_pointer_type w = x->p->right;
				if (w->color == COLOR::RED)
				{
					w->color = COLOR::BLACK;
					x->p->color = COLOR::RED;
					left_rotate(x->p);
					w = x->p->right;
				}

				// 兄弟w的两个孩子都是黑色的
				if (w->left->color == COLOR::BLACK && w->right->color == COLOR::BLACK)
				{
					w->color = COLOR::RED;
					x = x->p;
				}
				else
				{
					// 兄弟w的左孩子是红的，右孩子的黑的
					if (w->right->color == COLOR::BLACK)
					{
						w->left->color = COLOR::BLACK;
						w->color = COLOR::RED;
						right_rotate(w);
						w = x->p->right;
					}

					// 兄弟w的左孩子是红的或者黑的，右孩子是红的
					w->color = x->p->color;
					x->p->color = COLOR::BLACK;
					w->right->color = COLOR::BLACK;
					left_rotate(x->p);
					x = root;
				}
			}
			else
			{
				// w是x的兄弟节点
				node_pointer_type w = x->p->left;
				if (w->color == COLOR::RED)
				{
					w->color = COLOR::BLACK;
					x->p->color = COLOR::RED;
					right_rotate(x->p);
					w = x->p->left;
				}

				// 兄弟w的两个孩子都是黑色的
				if (w->left->color == COLOR::BLACK && w->right->color == COLOR::BLACK)
				{
					w->color = COLOR::RED;
					x = x->p;
				}
				else
				{
					// 兄弟w的左孩子是黑的，右孩子的红的
					if (w->left->color == COLOR::BLACK)
					{
						w->right->color = COLOR::BLACK;
						w->color = COLOR::RED;
						left_rotate(w);
						w = x->p->left;
					}

					// 兄弟w的左孩子是红的，右孩子是红的或者黑的
					w->color = x->p->color;
					x->p->color = COLOR::BLACK;
					w->left->color = COLOR::BLACK;
					right_rotate(x->p);
					x = root;
				}
			}
		}
		x->color = COLOR::BLACK;
	}
};

int main()
{
	rb_tree<int> rb;
	int datas[] = { 1,2,3,4,5,6,7,8 };
	for (auto data : datas)
	{
		rb.insert_key(data);
	}

	// 逐次删除根节点
	rb.delete_key(4);
	rb.delete_key(5);
	rb.delete_key(6);
	rb.delete_key(7);
	rb.delete_key(2);
	rb.delete_key(3);
	rb.delete_key(8);
	rb.delete_key(1);
	return 0;
}
