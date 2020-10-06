/*
* 二叉搜索树
*/

#include <iostream>
#include <vector>
#include <queue>
#include <stack>

template <typename _KeyType>
struct binary_tree_node
{
	using key_type = _KeyType;
	using key_reference_type = _KeyType&;
	using key_const_reference_type = const _KeyType&;

	key_type key;
	binary_tree_node* p;
	binary_tree_node* left;
	binary_tree_node* right;


	binary_tree_node() :
		p{ nullptr },
		left{ nullptr },
		right{ nullptr },
		key{}
	{
	}
};

template<typename _KeyType>
struct binary_search_tree
{
	// 基本类型定义
	using node_type = binary_tree_node<_KeyType>;
	using node_pointer_type = binary_tree_node<_KeyType>*;

	using key_type = typename node_type::key_type;
	using key_reference_type = typename node_type::key_reference_type;
	using key_const_reference_type = typename node_type::key_const_reference_type;

	// 根节点
	node_pointer_type root;

	binary_search_tree() :
		root{ nullptr }
	{
	}

	void insert(key_const_reference_type key)
	{
		node_pointer_type z = new node_type();
		z->key = key;

		node_pointer_type y = nullptr;
		node_pointer_type x = root;
		while (x != nullptr)
		{
			y = x;
			if (key < x->key)
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
	}

	node_pointer_type search(key_const_reference_type key)
	{
		node_pointer_type x = root;
		while (x!=nullptr && x->key != key)
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
			if (z->left == nullptr)
			{
				trans_plant(z, z->right);
			}
			else if (z->right == nullptr)
			{
				trans_plant(z, z->left);
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
			}
		}
	}

	node_pointer_type minimum()
	{
		return tree_minimum(root);
	}

private:
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

template<typename _KeyType>
void level_tree_walk(const binary_tree_node<_KeyType>* root)
{
	std::queue<const binary_tree_node<_KeyType>*> q;
	if (root)
		q.push(root);
	while (!q.empty())
	{
		auto p = q.front();
		std::cout << p->key << " ";
		
		if (p->left != nullptr)
			q.push(p->left);

		if (p->right != nullptr)
			q.push(p->right);

		q.pop();
	}
	std::cout << "\n";
}

template<typename _KeyType>
void inorder_tree_walk(const binary_tree_node<_KeyType>* root)
{
	if (root)
	{
		inorder_tree_walk(root->left);
		std::cout << root->key << " ";
		inorder_tree_walk(root->right);
	}
}

int main()
{
	binary_search_tree<int> tree;
	int data[] = { 15,6,18,3,7,17,20,2,4,13,9 };
	for (auto a : data)
	{
		tree.insert(a);
	}

	for (size_t i = 0; i < _countof(data); i++)
	{
		std::cout << "层级遍历：";
		level_tree_walk(tree.root);
		std::cout << "中序遍历：";
		inorder_tree_walk(tree.root);
		std::cout << "\n";
		auto p_min = tree.minimum();
		std::cout << "删除最小值：" << p_min->key << "\n";
		
		tree.delete_key(p_min->key);
	}

	return 0;
}