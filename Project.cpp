// Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <list>
#include <iostream>
using namespace std;
typedef std::string String;

template <typename T>
struct Node
{
    T info;
    Node* next;
};

template <typename T>
class MyLinkedList
{
private:
    Node<T>* head;
    Node<T>* tail;
    int size;
public:
    MyLinkedList()
    {
        this->head = nullptr;
        this->tail = nullptr;
        this->size = 0;
    }

    ~MyLinkedList()
    {
        while (this->GetSize() > 0)
        {
            this->DeleteFirstElement();
        }
    }

    bool IsEmpty()
    {
        return this->size == 0;

        /*if (this->head == nullptr || this->tail == nullptr || this->size == 0)
        {
            return true;
        }
        else
        {
            return false;
        }*/
    }

    int GetSize() 
    {
        return this->size;
    }

    void InsertAtHead(T elem)
    {
        Node<T>* temp = new Node<T>();
        temp->info = elem;
        temp->next = nullptr;

        if (this->IsEmpty())
        {
            head = tail = temp;
        }
        else
        {
            temp->next = head;
            head = temp;
        }

        this->size++;
    }

    void InsertAtTail(T elem)
    {
        Node<T>* temp = new Node<T>();
        temp->info = elem;
        temp->next = nullptr;

        if (this->IsEmpty())
        {
            head = tail = temp;
            return;
        }
        else
        {
            tail->next = temp;
            tail = temp;
        }

        this->size++;
    }

    void InsertElement(T elem)
    {
        if (this->IsEmpty())
        {
            this->InsertAtHead(elem);
        }

        if (this->IsEmpty())
        {
            this->InsertAtTail(elem);
        }

        if (this->head->info <= elem)
        {
            this->InsertAtHead(elem);
            return;
        }

        if (elem >= this->tail->info)
        {
            this->InsertAtTail(elem);
            return;
        }

        //find the position of insertion
        Node<T>* p = head;
        while (elem >= p->info && elem >= p->next->info)
        {
            p = p->next;
        }

        Node<T>* temp = new Node<T>();
        temp->info = elem;
        temp->next = p->next;
        p->next = temp;
        this->size++;
    }

    void DeleteFirstElement()
    {
        //situation where there are no elements
        if (this->IsEmpty())
        {
            cout << "The list is empty...";
            return;
        }

        //situation where there are one or more elements, is the processing
        Node<T>* p = this->head;
        this->head = head->next;
        if (this->GetSize() == 1)
        {
            this->tail = nullptr;
        }
        delete (p);
        this->size--;
    }

    void DeleteLastElement()
    {
        if (this->IsEmpty())
        {
            cout << "The list is empty...";
            return;
        }
        if (this->GetSize() == 1)
        {
            delete(head);
            this->head = this->tail = nullptr;
            return;
        }

        Node<T>* p;
        for (p = head; p->next != tail; p = p->next);
        p->next = nullptr;
        delete(tail);
        this->tail = p;
        this->size--;
    }

    void DeleteElement(T elem)
    {
        if (this->GetSize() == 1)
        {
            cout << "No element to remove ... empty list" << endl;
            return;
        }

        if (this->head->info == elem)
        {
            this->DeleteFirstElement();
            return;
        }

        if (this->tail->info == elem)
        {
            this->DeleteLastElement();
            return;
        }

        Node<T>* p;
        for (p = head; p->next->info != elem && p->next != tail; p = p->next);

        // found the element
        if (p->next->info == elem)
        {
            // processing for last element
            // processing !lastElement
            Node<T>* toDel = p->next;
            p->next = p->next->next;;
            delete(toDel);
            this->size--;
            return;
        }

        cout << "No element found" << endl;
    }

    void DisplayElement()
    {
        for (Node<T>* p = head; p != nullptr; p = p->next)
        {
            cout << p->info << " ";
        }
    }
};

struct TreeNode
{
    String medicareId;
    int directContacts = 0;
    int totalCases = 1;
    TreeNode* parentPtr;
    list<TreeNode*> directContactsPtrList;

    TreeNode(const String& medId) 
    { 
        medicareId = medId; 
        parentPtr = nullptr; 
    };

    TreeNode(TreeNode* parentPtr, const String& medId) 
    { 
        medicareId = medId; 
        this->parentPtr = parentPtr; 
    };
};


class ContactTree
{
private:
    TreeNode* root;
    int size;
public:
    ContactTree();
    ~ContactTree();//Darshini

    bool IsEmpty();
    int GetSize();//Darshini
    void AddPatient0(const String& newMedId);//Sarah
    void AddContact(const String& newMedId, const String& parentMedId);//Sarah

    TreeNode* LookUpContact(const String& medId);//Mike and Huy

    void DeleteContact(const String& medId);//Mike
    void DisplayContact(const String& lookFor);//Mike
    void DisplayContact(TreeNode* lookFor);//Huy
    void TraceSource(const String& medId);//Mike

    void PrintContactCases(const String& medId);//Huy
    void PrintContactTree();//Huy
    void PrintHierarchicalTree();//huy
};

ContactTree::ContactTree()
{
    this->root = nullptr; //initialise the root to nullptr
    this->size = 0; //initialise the size to zero
}

ContactTree::~ContactTree()
{
    while (this->GetSize() > 0)
    {
        this->DeleteContact(this->root->medicareId);
    }
}

bool ContactTree::IsEmpty()
{
    return this->size == 0; //returnt the size of tree equal to zero

    /*if (this->root == nullptr || this->size == 0)
    {
        return true;
    }
    else
    {
        return false;
    }*/
}

int ContactTree::GetSize()
{
    return this->size; //return the size
}

void ContactTree::AddPatient0(const String& newMedId)
{
    if (root != nullptr)//if the root is not null
    {
        cout << "The tree already has a patient zero" << endl;
        return;
    }
    TreeNode* patientZero = new TreeNode(newMedId);
    patientZero->medicareId = newMedId;
    root = patientZero; //set the root to the new node (patient0)
    this->size++;//incrementation the size

}

void ContactTree::AddContact(const String& newMedId, const String& parentMedId)//takes in the new patientID and the parent of that patient
{
    TreeNode* parent = LookUpContact(parentMedId);//look up the parent
    if (parent == nullptr)//if the parent is null
    {
        cout << "Parent not found" << endl;
        return;
    }
    TreeNode* newContact = new TreeNode(parent, newMedId);//make a new node with the parent and the new medicare id
    parent->directContactsPtrList.push_back(newContact);//add the new contact to the parents contact list
    parent->directContacts++;//increment the no of direct contact
    parent->totalCases++;//increment the total cases of the parent
    this->size++;//incrementation of the size
}

//Check the description on Page 5
TreeNode* ContactTree::LookUpContact(const string& medId)
{
    if (root == nullptr)//if the root is null
    {
        return nullptr;//return null
    }
    if (root->medicareId == medId)//if the root medicare id is the same as the input
    {
        return root;//return the root
    }
    for (TreeNode* child : root->directContactsPtrList)//for every child in the contact list
    {
        if (medId == child->medicareId)//if the id matches the child's id from the contact list
        {
            return child;//return the child's id
        }
    }
    return nullptr;//return null
}

void ContactTree::DeleteContact(const String& medId)
{
    TreeNode* contact = LookUpContact(medId);//look up the contact
    if (contact == nullptr)//if the contact is nullptr
    {
        cout << "Contact not found" << endl;
        return;
    }
    if (contact->parentPtr == nullptr)//if the parent is null
    {
        root = nullptr; //set the root to null
        delete contact; //delete the contact
        return;
    }
    // Remove the contact from its parent's contact list
    contact->parentPtr->directContactsPtrList.remove(contact);
    contact->parentPtr->directContacts--; //decrease the direct contacts
    //Iterate until the root 
    contact->parentPtr->totalCases--; //decrease the no of cases
    this->size--; //decrease the size
    delete contact;
    //Check Page 5 
}

void ContactTree::DisplayContact(const String& lookFor)
{
    TreeNode* lookForNode = LookUpContact(lookFor);//look up the contact
    if (lookForNode == nullptr) //if the contact is null
    {
        cout << "Contact not found" << endl;
        return;
    }
    if (lookForNode == root) //if the contact is the root
    {
        cout << "Contact found: " << root->medicareId << endl; //print the medicare id of the contact
        return;
    }

    for (TreeNode* child : root->directContactsPtrList)
    {
        if (lookForNode == child)
        {
            cout << "Contact found: " << child->medicareId << endl;
            return;
        }
    }
}

/*
void ContactTree::DisplayContact(TreeNode* lookFor)
{
    //if the root is nullptr
    if (root == nullptr)
    {
        cout << "The tree is empty" << endl;
        return;
    }
    //if the contact we are looking for is equal to the root
    if (lookFor == root)
    {
        cout << "Contact found: " << root->medicareId << endl;
        return;
    }
    //iterate through the list of contacts
    for (TreeNode* child : root->directContactsPtrList)
    {
        if (lookFor == child) //if the contact is the same as the child
        {
            cout << "Contact found: " << child->medicareId << endl; //print the medicare id of the contact
            return;
        }
        DisplayContact(child); //display the contact
    }
}
*/

void ContactTree::DisplayContact(TreeNode* lookFor)
{
    // If the contact is null, print a message and return
    if (lookFor == nullptr)
    {
        cout << "Contact not found" << endl;
        return;
    }

    // Print the Medicare ID of the contact
    cout << "Medicare ID: " << lookFor->medicareId << endl;
}

void ContactTree::TraceSource(const String& medId)
{
    //find the node that corresponds to the given id
    TreeNode* lookForNode = LookUpContact(medId);

    //checks if the node exists
    if (lookForNode == nullptr)
    {
        cout << "Medicare ID: " << medId << " not found." << endl;
        return;
    }

    //start tracing from given person until it reaches the root node
    cout << "Tracing " << medId << "'s sources: " << endl;
    while (lookForNode != nullptr)
    {
        //pritns the information of trhe current node
        cout << "Contact: " << lookForNode->medicareId << endl;

        //move to the parent node
        lookForNode = lookForNode->parentPtr;
    }
}

void ContactTree::PrintContactCases(const String& medId)
{
    TreeNode* contact = LookUpContact(medId); // Look up the contact
    if (contact == nullptr) // If the contact is not found
    {
        cout << "Contact not found" << endl;
        return;
    }

    cout << "Direct cases caused by " << medId << ":" << endl;

    // Iterate through the direct contacts and print their details
    for (TreeNode* direct : contact->directContactsPtrList)
    {
        /*<< directContact->medicareId*/;
        DisplayContact(direct); // Print the contact's ID
        cout << " (Direct Contacts: " << contact->directContacts
            << ", Total Cases: " << contact->totalCases << ")" << endl;
    }
}

void PrintContactTreeRecursive(TreeNode* node, int size)
{
    // Print the current node's information with proper indentation
    cout << "MedID: " << node->medicareId << endl;

    // Recursively print the children of the current node
    for (TreeNode* child : node->directContactsPtrList)
    {
        PrintContactTreeRecursive(child, size + 1);
    }
}

void ContactTree::PrintContactTree()
{
    if (root == nullptr)
    {
        cout << "The tree is empty" << endl;
        return;
    }

    cout << "Printing Contact Tree:" << endl;
    PrintContactTreeRecursive(root, 0);
}

void PrintContactHierarchy(TreeNode* node, int size)
{
    if (node == nullptr) 
    {
        cout << "No contact node." << endl;
        return;
    }

    // Print current node
    for (int i = 0; i < size; i++)
    {
        cout << "-";
    }

    cout << "> " << node->medicareId << endl;

    // Recursively print children
    for (TreeNode* child : node->directContactsPtrList)
    {
        PrintContactHierarchy(child, size + 1);
    }
}

void ContactTree::PrintHierarchicalTree()
{
    if (root == nullptr)
    {
        cout << "The tree is empty" << endl;
        return;
    }

    cout << "Printing Hierarchical Contact Tree:" << endl;
    PrintContactHierarchy(root, 0);
}

int main()
{
    
    //std::cout << "Hello World!\n";
    //ContactTree* myFirstList = new ContactTree;
    //myFirstList->AddPatient0("A0");
    //myFirstList->AddContact("A1", "A0");
    //myFirstList->AddContact("A2", "A0");
    //myFirstList->AddContact("B1", "A1");

    //myFirstList->DisplayContact("A1");
    //myFirstList->DisplayContact("A2");

    ////myFirstList->DeleteContact("A2");
    ////myFirstList->DisplayContact("A2");
    //myFirstList->TraceSource("A1");

    //myFirstList->PrintContactCases("A1");
    //myFirstList->PrintContactTree();
    //myFirstList->PrintHierarchicalTree();
    
    //TEMPLATES
    /*MyLinkedList<char> myImplementList;
    myImplementList.InsertAtHead('!');
    myImplementList.InsertAtHead('@');
    myImplementList.DisplayElement();*/

    MyLinkedList<int> myImplementList;
    myImplementList.InsertAtHead(1);
    myImplementList.InsertAtHead(0);
    myImplementList.InsertAtTail(2);
    myImplementList.DisplayElement();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file