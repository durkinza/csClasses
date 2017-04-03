/* Zane Durkin      Homework #4      Bruce Bolden
 * File: BTree.h
 * Date: 03/28/17
 */
#include <string>
class BinaryTree{
	public:
		BinaryTree();
		~BinaryTree();
		void printTitles();
		void printActorsOfShow(std::string show);
		void printShowsOfActor(std::string actor);
		void printTree();
		void addNode(std::string title, std::string genre, std::string url, std::string actors[], int numOfActors);
	private:
		typedef struct btree{
			btree *left;
			btree *right;
			std::string title;
			std::string genre;
			std::string url;
			int releaseYear;
			std::string actors[20];
			int numOfActors;
		} btree;
		btree *head;
		void _DeleteTree(btree *tree);
		void _printTitles(btree *tree, int spaces);
		void _printShowsOfActor(btree *tree, std::string actor);
		void _printActorsOfShow(btree *tree, std::string show);
		void _addNode(btree *tree, btree * data);

		
};
