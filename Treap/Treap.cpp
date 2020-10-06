/*
* Treap
*/

#include <iostream>
#include <random>

using priority_type = int;

priority_type random_proiority()
{
	return rand();
}

template <typename _KeyType>
struct treap_node
{
	using key_type = _KeyType;
	using key_reference_type = _KeyType&;
	using key_const_reference_type = const _KeyType&;

	key_type key;
	priority_type priority;

	treap_node* p;
	treap_node* left;
	treap_node* right;

	treap_node() :
		p{ nullptr },
		left{ nullptr },
		right{ nullptr },
		key{}
	{
		priority = random_proiority();
	}
};

template<typename _KeyType>
struct treap
{
	// 基本类型定义
	using node_type = treap_node<_KeyType>;
	using node_pointer_type = treap_node<_KeyType>*;

	using key_type = typename node_type::key_type;
	using key_reference_type = typename node_type::key_reference_type;
	using key_const_reference_type = typename node_type::key_const_reference_type;

	// 根节点
	node_pointer_type root;

	treap() :
		root{ nullptr }
	{
	}

	void insert_key(key_const_reference_type key)
	{
		node_pointer_type z = new node_type();
		z->key = key;
		insert_key(z);
	}

	void insert_key(node_pointer_type z)
	{
		node_pointer_type y = nullptr;
		node_pointer_type x = root;
		while (x != nullptr)
		{
			y = x;
			if (z->key < x->key)
				x = x->left;
			else
				x = x->right;
		}
		z->p = y;
		if (y == nullptr)
			root = z;
		else if (z->key < y->key)
			y->left = z;
		else
			y->right = z;

		// 插入操作会引起（最小）堆性质被破坏，需要进行修复
		treap_insert_fixup(z);
	}

	node_pointer_type search(key_const_reference_type key)
	{
		node_pointer_type x = root;
		while (x != nullptr && x->key != key)
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
		if (z != nullptr)
		{
			decltype(z) x = {};
			if (z->left == nullptr)
			{
				trans_plant(z, z->right);
				x = z->right;
			}
			else if (z->right == nullptr)
			{
				trans_plant(z, z->left);
				x = z->left;
			}
			else
			{
				auto y = tree_minimum(z->right);
				if (y->p != z)
				{
					trans_plant(y, y->right);
					y->right = z->right;
					y->right->p = y;
				}
				trans_plant(z, y);
				y->left = z->left;
				y->left->p = y;
				x = y;
			}

			// 赋予占有z原来位置的节点x的优先级为z的优先级来保持最小堆性质
			if (x != nullptr)
				x->priority = z->priority;
		}
	}

	node_pointer_type minimum()
	{
		return tree_minimum(root);
	}

private:
	void treap_insert_fixup(node_pointer_type z)
	{
		while (z->p && z->priority < z->p->priority)
		{
			if (z == z->p->left)
			{
				// 右旋
				right_rotate(z->p);
			}
			else
			{
				// 左旋
				left_rotate(z->p);
			}
		}
	}

	void trans_plant(node_pointer_type u, node_pointer_type v)
	{
		if (u->p == nullptr)
		{
			root = v;
		}
		else if (u == u->p->left)
			u->p->left = v;
		else
			u->p->right = v;

		if (v != nullptr)
			v->p = u->p;
	}

	void left_rotate(node_pointer_type x)
	{
		// x的右孩子
		node_pointer_type y = x->right;

		// 1、使用y的左孩子代替y成为x的右孩子
		x->right = y->left;
		if (y->left != nullptr)
			y->left->p = x;

		// 2、使用y代替x，占有x原来的位置
		y->p = x->p;
		if (x->p == nullptr)
			root = y;
		else if (x == x->p->left)
			x->p->left = y;
		else
			x->p->right = y;

		// 3、让x成为y的左孩子
		y->left = x;
		x->p = y;
	}

	void right_rotate(node_pointer_type y)
	{
		// y的左孩子
		node_pointer_type x = y->left;

		// 1、使用x的右孩子代替x成为y的左孩子
		y->left = x->right;
		if (x->right != nullptr)
			x->right->p = y;

		// 2、使用x代替y，占有y原来的位置
		x->p = y->p;
		if (y->p == nullptr)
			root = x;
		else if (y == y->p->left)
			y->p->left = x;
		else
			y->p->right = x;

		// 3、让y成为x的右孩子
		x->right = y;
		y->p = x;
	}

	static node_pointer_type tree_minimum(node_pointer_type root)
	{
		node_pointer_type y = nullptr;
		while (root)
		{
			y = root;
			root = root->left;
		}
		return y;
	}
};

int main()
{
	treap<char> ch_treap;

	// 构建一个原始的treap
	char keys[] = { 'G', 'B', 'H', 'A', 'E', 'K', 'I' };
	priority_type priorities[] = { 4, 7, 5, 10, 23, 65, 73 };

	for (size_t i = 0; i < _countof(keys); i++)
	{
		treap<char>::node_pointer_type z = new treap<char>::node_type();
		z->key = keys[i];
		z->priority = priorities[i];
		ch_treap.insert_key(z);
	}

	// 插入C:25
	treap<char>::node_pointer_type z = new treap<char>::node_type();
	z->key = 'C';
	z->priority = 25;
	ch_treap.insert_key(z);

	// 插入D:9
	z = new treap<char>::node_type();
	z->key = 'D';
	z->priority = 9;
	ch_treap.insert_key(z);

	// 插入F:2
	z = new treap<char>::node_type();
	z->key = 'F';
	z->priority = 2;
	ch_treap.insert_key(z);

	// 删除G
	ch_treap.delete_key('G');
	// 删除F
	ch_treap.delete_key('F');
	return 0;
}