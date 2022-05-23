#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <iterator>
#include <cctype>
using namespace std;

class AVLTree {
public:
    //Creates node class to store data in tree
    class Node {
    public:
        Node() {
            name = "";
            gatorID = 0;
            left = nullptr;
            right = nullptr;
            parent = nullptr;
        }

        Node(string userName, int userID, Node* parentNode) {
            name = userName;
            gatorID = userID;
            left = nullptr;
            right = nullptr;
            parent = parentNode;
        }

        string name;
        int gatorID;
        Node* left;
        Node* right;
        Node* parent;
        bool lastElement;
    };

    int GetHeight(Node* root) {
        if (root == nullptr) {
            return 0;
        }
        //If not null then height is 0
        int leftHeight = 0;
        int rightHeight = 0;
        //Perform recursive summation of left and right heights
        leftHeight += GetHeight(root->left);
        rightHeight += GetHeight(root->right);

        //Return the larger height (Add 1 to compensate for fact we didn't start at head node)
        if (leftHeight > rightHeight) {
            return leftHeight + 1;
        }
        else {
            return rightHeight + 1;
        }
    }

    int NetHeight(Node* root)
    {
        //Find the height of left and right subtrees and then find the netHeight
        //If right or left is null then height is 0 
        if (root == nullptr) {
            return 0;
        }
        int leftHeight;
        int rightHeight;

        //Recursively get height of the right and left
        leftHeight = GetHeight(root->left);
        rightHeight = GetHeight(root->right);

        //The net height is the difference between the left and right heights
        int netHeight = leftHeight - rightHeight;
        return netHeight;
    }

    //If nullptr then create new node
    //If gatorId already exists then unsuccessful
    //If new ID less then current root's ID then insert to the left
    //If new ID more than current root's ID then insert to the right
    Node* InsertID(string userName, int userID, Node* root, Node* parent) {
        //If root is null, then we have reached the spot for insertion and insert a new node
        if (root == nullptr) {
            if (root == nullptr) {
                cout << "successful" << endl;
            }
            return new Node(userName, userID, parent);
        }
        //If root is not null then search tree for proper spot of insertion according to BST rule
        else {
            if (userID < root->gatorID) {
                root->left = InsertID(userName, userID, root->left, root);
            }
            else {
                root->right = InsertID(userName, userID, root->right, root);
            }
        }
        int netHeight = NetHeight(root);
        //Left Left Rotation
        if (netHeight > 1 && userID < root->left->gatorID) {
            return rotateRight(root);
        }
        //Left Right Rotation
        if (netHeight > 1 && userID > root->left->gatorID) {
            return rotateLeftRight(root);
        }
        //Right Left Rotation
        if (netHeight < -1 && userID < root->right->gatorID) {
            return rotateRightLeft(root);
        }
        //Right Right Rotation
        if (netHeight < -1 && userID > root->right->gatorID) {
            return rotateLeft(root);
        }
        return root;
    }

    //Same idea as Insertion function
    //If we find a match, delete
    //Otherwise search for it (greater elements to the right and lower elements to the left)
    Node* RemoveID(int userID, Node* root, Node* parent) {
        if (root == nullptr || root->gatorID == userID) {
            if (root == nullptr) {
                cout << "unsuccessful" << endl;
            }
            else if (root->gatorID == userID) {
                cout << "successful" << endl;
                return Deletor(root, parent);
            }
        }
        else {
            if (userID < root->gatorID) {
                root->left = RemoveID(userID, root->left, parent);
            }
            else {
                root->right = RemoveID(userID, root->right, parent);
            }
        }
        int netHeight = NetHeight(root);
        //Left Left Rotation
        if (netHeight > 1 && NetHeight(root->left) >= 0) {
            return rotateRight(root);
        }
        //Left Right Rotation
        else if (netHeight > 1 && NetHeight(root->left) < 0) {
            return rotateLeftRight(root);
        }
        //Right Left Rotation
        else if (netHeight < -1 && NetHeight(root->right) > 0) {
            return rotateRightLeft(root);
        }
        //Right Right Rotation
        else if (netHeight < -1 && NetHeight(root->right) <= 0) {
            return rotateLeft(root);
        }
        return root;
    }

    //If both children are null then delete node and return nullptr
    //If Left or Right child is not null then switch parent and child and delete the node to ensure we don't lose child
    //If we are trying to delete the top root, then we must switch the top root with its in order predecessor
    //If both have children then delete Inorder Successor
    Node* Deletor(Node* root, Node* parent) {
        if (root == nullptr) {
            return nullptr;
        }
        else if (root->left == nullptr && root->right == nullptr) {
            return nullptr;
        }
        else if (root->left == nullptr && root->right != nullptr) {
            Node* temp = root->right;
            delete root;
            return temp;
        }
        else if (root->left != nullptr && root->right == nullptr) {
            Node* temp = root->left;
            delete root;
            return temp;
        }
        else if (root == parent) {
            Node* temp = ParentInOrderSuccessor(root->left);
            root->name = temp->name;
            root->gatorID = temp->gatorID;
            root->left = ParentDeletor(root->left);
        }
        else {
            Node* temp = InOrderSuccessor(root->right);
            root->name = temp->name;
            root->gatorID = temp->gatorID;
            root->right = Deletor(root->right, parent);
        }
        return root;
    }

    //Helper function to delete the inorder predecessor (Which used to be the root we were trying to delete)
    Node* ParentDeletor(Node* root) {
        if (root->right == nullptr) {
            return nullptr;
        }
        else {
            root->right = ParentDeletor(root->right);
        }
        return root;
    }

    //Store tree in a vector and access the Nth element to delete it
    //If trying to delete an out of range index then this will be unsuccessful
    Node* DeleteNthNode(Node* root, int N, Node* parent) {
        vector<Node*> v;
        InOrderStorage(root, v);

        if (N > v.size()) {
            cout << "unsuccessful" << endl;
            return root;
        }
        else {
            return RemoveID(v.at(N)->gatorID, root, parent);
        }
    }

    //Perform inorder traversal and store each element in a vector so that vector stores all elements ordered Inorder
    void InOrderStorage(Node* root, vector<Node*>& v) {
        if (root != nullptr) {
            InOrderStorage(root->left, v);
            v.push_back(root);
            InOrderStorage(root->right, v);
        }
    }

    //In order successor is the root to the right of our current one
    //Then go all the way to the left of that root until we get to the last leaf
    //This would be next element in an in order traversal
    Node* InOrderSuccessor(Node* root) {
        Node* current = root;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    //Finds the Inorder predecessor of a root (Very similar to how InOrder Successor works)
    Node* ParentInOrderSuccessor(Node* root) {
        Node* current = root;
        while (current && current->right != nullptr) {
            current = current->right;
        }
        return current;
    }

    //Rotates node left
    Node* rotateLeft(Node* node)
    {
        Node* grandChild = node->right->left;
        Node* newParent = node->right;
        newParent->left = node;
        node->right = grandChild;
        return newParent;
    }

    //Rotates node right
    Node* rotateRight(Node* node)
    {
        Node* grandChild = node->left->right;
        Node* newParent = node->left;
        newParent->right = node;
        node->left = grandChild;
        return newParent;
    }

    //Rotates node right left
    Node* rotateRightLeft(Node* node)
    {
        Node* newRoot = rotateRight(node->right);
        node->right = newRoot;
        newRoot = rotateLeft(node);
        return newRoot;
    }

    //Rotates node left right
    Node* rotateLeftRight(Node* node)
    {
        Node* newRoot = rotateLeft(node->left);
        node->left = newRoot;
        newRoot = rotateRight(node);
        return newRoot;
    }

    //Place each level in a queue
    //Pop the level and fill up the queue with the next level by accessing left and right pointers of all elements in the current level
    //Keep track of number of levels and return this value
    int GetMaxLevel(Node* root) {
        if (root == nullptr) {
            return 0;
        }
        queue<Node*> q;
        q.push(root);
        int n;
        int levelCount = 0;

        while (!q.empty()) {
            n = (int)q.size();
            while (n > 0) {
                Node* current = q.front();
                q.pop();
                n -= 1;
                if (current->left != nullptr) {
                    q.push(current->left);
                }
                if (current->right != nullptr) {
                    q.push(current->right);
                }
            }
            levelCount += 1;
        }
        return levelCount;
    }


    //Tree is sorted by ID so just go left if value is less than current node or right if value is more
    //If a match then return name
    void SearchID(Node* root, int userID) {
        if (root == nullptr) {
            cout << "unsuccessful" << endl;
        }
        else if (root->gatorID == userID) {
            cout << root->name << endl;
        }

        else if (userID < root->gatorID) {
            SearchID(root->left, userID);
        }
        else {
            SearchID(root->right, userID);
        }
    }

    //Boolean version of above SearchID function. If we find a repeat value we return true otherwise we return false
    bool SearchRepeatID(Node* root, int userID) {
        if (root == nullptr) {
            return false;
        }
        else if (root->gatorID == userID) {
            return true;
        }

        else if (userID < root->gatorID) {
            SearchRepeatID(root->left, userID);
        }
        else {
            SearchRepeatID(root->right, userID);
        }
    }

    //Do a preorder traversal and if we find a match then store the node
    //Then output in preorder all the IDs
    void SearchName(Node* root, string userName) {
        vector<Node*> v;
        PreOrderStorage(v, root, userName);
        if (v.size() > 0) {
            for (int ii = 0; ii < v.size(); ii += 1) {
                cout << v.at(ii)->gatorID << endl;
            }
        }
        else {
            cout << "unsuccessful" << endl;
        }
    }

    //Storage by preorder traversal in a vector
    void PreOrderStorage(vector<Node*>& v, Node* root, string userName) {
        if (root != nullptr) {
            if (root->name == userName) {
                v.push_back(root);
            }
            PreOrderStorage(v, root->left, userName);
            PreOrderStorage(v, root->right, userName);
        }
    }

    //The following are standard traversals for preorder, inorder, and postorder
    string PreOrder(Node* root) {
        string result = "";
        if (root != nullptr) {
            result += root->name + ", ";
            result += PreOrder(root->left);
            result += PreOrder(root->right);
        }
        return result;
    }

    string InOrder(Node* root) {
        string result = "";
        if (root != nullptr) {
            result += InOrder(root->left);
            result += root->name + ", ";
            result += InOrder(root->right);
        }
        return result;
    }

    string PostOrder(Node* root) {
        string result = "";
        if (root != nullptr) {
            result += PostOrder(root->left);
            result += PostOrder(root->right);
            result += root->name + ", ";
        }
        return result;
    }

private:
    Node* root;
};

//Simply removes the last comma since the traversals add a comma to each element of the list but the last element does not need this comma
string RemoveLastComma(string list) {
    return list.substr(0, list.size() - 2);
}



int main()
{
    AVLTree tree;
    AVLTree::Node* root = nullptr;
    string input;
    getline(cin, input);
    int N;
    N = stoi(input);
    while (N > 0) {
        getline(cin, input);

        //Insert an element
        if (input.find("insert") != std::string::npos) {
            bool validName = true;
            string temp = "";
            string tempIntString;
            int tempInt;
            int kk = 8;
            //Getting the name from input
            for (int ii = 8; ii < input.size(); ii += 1) {
                kk += 1;
                //When we reach a quotation mark we are done reading in the name
                if (input.at(ii) == '\"') {
                    break;
                }
                //If we have names that include anything but letters and spaces then we do not have a valid name
                if (!isalpha(input.at(ii)) && !isspace(input.at(ii))) {
                    validName = false;
                }
                //Push back read in charachters into a string
                temp += input.at(ii);
            }
            //Now we read in the GatorID
            for (int ii = kk + 1; ii < input.size(); ii += 1) {
                tempIntString += input.at(ii);
            }
            tempInt = stoi(tempIntString);
            //Check if we have a valid input of only alphabetical name and 8-digit GatorID and no repeat IDs
            //If we do then insert into tree
            if (tempIntString.size() == 8 && validName && !tree.SearchRepeatID(root, tempInt)) {
                root = tree.InsertID(temp, tempInt, root, root);
                N -= 1;
            }
            //If we do not have valid input then print out unsuccessful
            else {
                cout << "unsuccessful" << endl;
                N -= 1;
            }
        }
        //Remove the Nth inorder element
        else if (input.find("removeInorder") != std::string::npos) {
            string temp = "";
            int tempInt;
            for (int ii = 13; ii < input.size(); ii += 1) {
                temp += input.at(ii);
            }
            tempInt = stoi(temp);
            root = tree.DeleteNthNode(root, tempInt, root);
            N -= 1;
        }

        //remove an element
        else if (input.find("remove") != std::string::npos) {
            string tempIntString;
            int tempInt;
            for (int ii = 7; ii < input.size(); ii += 1) {
                tempIntString += input.at(ii);
            }
            tempInt = stoi(tempIntString);
            root = tree.RemoveID(tempInt, root, root);
            N -= 1;
        }

        //search for an element
        else if (input.find("search") != std::string::npos) {
            string temp = "";
            //If there are quotations then we are searching for a name
            if (input.at(7) == '\"') {
                for (int ii = 8; ii < input.size(); ii += 1) {
                    if (input.at(ii) == '\"') {
                        break;
                    }
                    temp += input.at(ii);
                }
                tree.SearchName(root, temp);
            }
            //Otherwise we search for a GatorID
            else {
                for (int ii = 6; ii < input.size(); ii += 1) {
                    temp += input.at(ii);
                }
                int tempInt = stoi(temp);
                tree.SearchID(root, tempInt);
            }
            N -= 1;
        }
        //Print Inorder, Preorder, or Postorder traversal depending on input
        else if (input == "printInorder") {
            cout << RemoveLastComma(tree.InOrder(root)) << endl;
            N -= 1;
        }

        else if (input == "printPreorder") {
            cout << RemoveLastComma(tree.PreOrder(root)) << endl;
            N -= 1;
        }

        else if (input == "printPostorder") {
            cout << RemoveLastComma(tree.PostOrder(root)) << endl;
            N -= 1;
        }
        //Print the level count
        else if (input == "printLevelCount") {
            cout << tree.GetMaxLevel(root) << endl;
            N -= 1;
        }
        //Otherwise print out error code
        else {
            cout << "Error! This is not a valid command! Please try again! Beep Boop Bop" << endl;
        }

    }
}

