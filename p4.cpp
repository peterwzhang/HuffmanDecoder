#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

class treeNode {
   public:
    treeNode *left = NULL, *right = NULL;
    int v;
};

treeNode *BuildTree(int inOrderArr[], int postOrderArr[], int currNum,
                    int maxNum, int *postNum) {
    if (currNum > maxNum) return NULL;  // base case

    treeNode *newNode = new treeNode;
    newNode->v = postOrderArr[*postNum];  // check post order first
    (*postNum)--;

    if (currNum == maxNum) return newNode;  // ret at end

    int inNum = 0;  // find  index for inorder trav
    for (int i = currNum; i <= maxNum; i++) {
        if (inOrderArr[i] == newNode->v) {
            inNum = i;
            break;
        }
    }
    // construct the left and right trees recursively
    newNode->right =
        BuildTree(inOrderArr, postOrderArr, inNum + 1, maxNum, postNum);
    newNode->left =
        BuildTree(inOrderArr, postOrderArr, currNum, inNum - 1, postNum);
    return newNode;
    // temp
}

void printPreOrder(treeNode *Node)  // print tree out in pre order for testing
{
    if (Node == NULL) return;
    cout << Node->v << " ";
    printPreOrder(Node->left);
    printPreOrder(Node->right);
}

int FileLen(ifstream &file) {
    int count = 0;
    string text;
    while (file >> text) {
        count++;
    }
    file.clear();
    file.seekg(0);
    return count;
}

void ReadData(ifstream &file, int arr[]) {
    int count = 0;
    while (file >> arr[count]) {
        count++;
    }
    file.close();
}

void PrintArr(int arr[], int size, int i) {
    if (i == size) {
        cout << endl;
        return;
    }
    cout << arr[i] << endl;
    return PrintArr(arr, size, i + 1);
}

void PrintCodes(treeNode *root, string code) {
    if (!root) return;
    char ch = root->v;
    if (root->v < 128) cout << ch << ":" << code << endl;
    PrintCodes(root->left, code + "0");
    PrintCodes(root->right, code + "1");
}

string Decode(treeNode *root, string encodedmsg) {
    treeNode *cur = root;
    string msg = "";
    for (int i = 0; i < encodedmsg.length(); i++) {
        if (encodedmsg[i] == '0') {
            cur = cur->left;
        } else {
            cur = cur->right;
        }
        if (cur->right == NULL && cur->left == NULL) {
            msg += cur->v;
            cur = root;
        }
    }

    return msg;
}

void StoreCodes(treeNode *root, string code, vector<char> arrCh,
                vector<string> arrCode) {
    if (!root) return;
    char ch = root->v;
    if (root->v < 128) {
        arrCh.push_back(ch);
        arrCode.push_back(code);
        // cout << ch << ":" << code << endl;
    }
    StoreCodes(root->left, code + "0", arrCh, arrCode);
    StoreCodes(root->right, code + "1", arrCh, arrCode);
}

int main(int argc, char *argv[]) {
    ifstream fileOne, fileTwo, fileThree;

    fileOne.open(argv[1]);  // in order
    fileTwo.open(argv[2]);  // post order
    int inSize = FileLen(fileOne);
    int postSize = FileLen(fileTwo);
    int in[inSize];
    int post[postSize];
    ReadData(fileOne, in);
    ReadData(fileTwo, post);
    // PrintArr(in, inSize, 0);
    // PrintArr(post, postSize, 0);
    int size = inSize;
    int pSize = postSize - 1;
    treeNode *rootNode = BuildTree(in, post, 0, size - 1, &pSize);
    // printPreOrder(rootNode);
    // cout << endl;
    // vector<char> chKey;
    // vector<string> codeKey;
    // StoreCodes(rootNode, "", chKey, codeKey);
    string encodedMsg;
    fileThree.open(argv[3]);  // decode text
    while (!fileThree.eof()) {
        string temp;
        fileThree >> temp;
        encodedMsg += temp;
    }
    // cout << encodedMsg;
    string decodedmsg;
    decodedmsg = Decode(rootNode, encodedMsg);
    cout << decodedmsg;
    return 0;
}