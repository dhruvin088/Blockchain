#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <ctime>
using namespace std;
hash<string> to_hash;

class Transections
{
    string sender;
    string reciver;
    unsigned int amount;

public:
    Transections() {}

    Transections(string sender, string reciver, unsigned int amount)
    {
        this->sender = sender;
        this->reciver = reciver;
        this->amount = amount;
    }
    friend class BlockChain;
};

class Node
{
public:
    int index;
    string prevHash, currHash;
    int nonse;
    time_t timestamp;
    unsigned int markelRoot;
    vector<Transections> allTrans;

    Node() {}

    Node(int index, string prevHash, string currHash, time_t timestamp, int nonse, unsigned int markelRoot, vector<Transections> allTrans)
    {
        this->index = index;
        this->prevHash = prevHash;
        this->currHash = currHash;
        this->timestamp = timestamp;
        this->nonse = nonse;
        this->markelRoot = markelRoot;
        this->allTrans = allTrans;
    }
};

class BlockChain
{
private:
    list<Node> li;
    string previousHash;
    vector<unsigned int> transHashList;
    vector<Transections> transInfoList;

public:
    BlockChain()
    {
        time_t timeStamp = time(nullptr);
        Node *curr = new Node(0, "0", "", timeStamp, 0, 0, transInfoList);
        li.push_back(*curr);
        previousHash = "0";
    }

    // calculate current hash
    string calc_hash(int index, string prevHash, time_t timeStamp, unsigned int marleRoot)
    {
        string currentHash = to_string(index) + prevHash + to_string(timeStamp) + to_string(marleRoot);
        currentHash = to_string(to_hash(currentHash));
        return currentHash;
    }

    // take transection details
    void take_transection()
    {
        string sender, reciver;
        int amount;

        cout << "Enter Sender Name: ";
        cin >> sender;
        cout << "Enter Reciver Name: ";
        cin >> reciver;
        cout << "Enter Amount: ";
        cin >> amount;

        Transections t;
        t.amount = amount;
        t.sender = sender;
        t.reciver = reciver;

        unsigned int hash_transection = to_hash(to_string(amount) + sender + reciver);
        transHashList.push_back(hash_transection);
        transInfoList.push_back(t);
    }

    // create block for transection
    unsigned int markle_root(vector<unsigned int> vect)
    {
        cout << endl;
        for (auto t : vect)
        {
            cout << "Hash: " << t << endl;
        }
        cout << endl;

        if (vect.size() == 0)
        {
            return 0;
        }
        if (vect.size() == 1)
        {
            return vect[0];
        }

        vector<unsigned int> newGenerateHash;
        for (size_t i = 0; i < vect.size() - 1; i += 2)
        {
            // if more than 1 transection, make pair
            newGenerateHash.push_back(make_pair(transHashList[i], transHashList[i + 1]));
        }

        if (vect.size() % 2 == 1)
        {
            // if odd transection, same hash use two time
            newGenerateHash.push_back(make_pair(newGenerateHash.back(), newGenerateHash.back()));
        }

        return markle_root(newGenerateHash);
    }

    // current hash is combination of child two hash
    unsigned int make_pair(unsigned int h1, unsigned int h2)
    {
        return h1 + h2;
    }

    // add block
    void add_block()
    {
        unsigned int root = markle_root(transHashList);
        time_t timeStamp = time(nullptr);
        int nonse = rand();

        string currHash = calc_hash(li.size(), previousHash, timeStamp, root);
        Node *curr = new Node(li.size(), previousHash, currHash, timeStamp, nonse, root, transInfoList);
        li.push_back(*curr);
        previousHash = currHash;
        transHashList.clear();
        transInfoList.clear();
    }

    // display transection information
    void display_transection(vector<Transections> trans)
    {
        int count = 0;
        for (auto ti : trans)
        {
            count++;
            cout << "\n#Transection-" << count << endl;
            cout << "Sender: " << ti.sender << endl;
            cout << "Reciver: " << ti.reciver << endl;
            cout << "Amount: " << ti.amount << endl;
        }
    }

    // display actual blockchain
    void display_blockchain()
    {
        list<Node>::iterator itor;
        for (itor = li.begin(); itor != li.end(); itor++)
        {
            cout << "---------------- BLOCK ----------------- \n";
            cout << "Index: " << itor->index << endl;
            cout << "Previous Hash: " << itor->prevHash << endl;
            cout << "Current Hash: " << itor->currHash << endl;
            cout << "TimeStamp: " << asctime(localtime(&itor->timestamp));
            cout << "Nonse: " << itor->nonse << endl;
            cout << "Markle Root: " << itor->markelRoot << endl;

            display_transection(itor->allTrans);
            cout << "---------------------------------------- \n"
                 << endl;
        }
    }
};

int main()
{
    BlockChain chain;
    int choice;

    do
    {
        cout << "0. Exit" << endl;
        cout << "1. Minnig Block" << endl;
        cout << "2. Display Blockchain" << endl;
        cout << "ENTER YOUR CHOICE: ";
        cin >> choice;

        switch (choice)
        {
        case 0:
            cout << endl
                 << "Thank's for mining block." << endl;
            break;
        case 1:
            int ch;
            do
            {
                cout << "\n1. Add new Trasaction" << endl;
                cout << "0. End Trasaction" << endl;
                cout << "Enter your Choice : ";
                cin >> ch;

                switch (ch)
                {
                case 1:
                    chain.take_transection();
                    break;
                case 0:
                    cout << "TRANSECTION COMPLETED..! \n"
                         << endl;
                    break;
                default:
                    cout << "INVALID CHOICE, TRY AGAIN.! \n"
                         << endl;
                    break;
                }

            } while (ch != 0);
            chain.add_block();
            break;
        case 2:
            chain.display_blockchain();
            break;
        default:
            cout << "INVALID CHAOICE ENTRED, try again...." << endl;
            break;
        }
    } while (choice != 0);

    return 0;
}