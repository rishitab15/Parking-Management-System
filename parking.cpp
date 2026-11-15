
#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <cstdio>
#include <cstdlib>

using namespace std;

const int CAPACITY = 100;
const int HASH_SIZE = 101;
const int WAIT_SIZE = 200;
const int BILL_SIZE = 1000;

// ============================================================
// MEMBER 1: OOP, INHERITANCE, POLYMORPHISM
// DOUBLY LINKED LIST
// ============================================================

// Abstract Base Class
class Vehicle
{
public:
    virtual double getRate() const = 0;
    virtual const char* getType() const = 0;

    virtual ~Vehicle() {}
};

// Derived Class: Bike
class Bike : public Vehicle
{
public:
    double getRate() const
    {
        return 30.0;
    }

    const char* getType() const
    {
        return "Bike";
    }
};

// Derived Class: Car
class Car : public Vehicle
{
public:
    double getRate() const
    {
        return 60.0;
    }

    const char* getType() const
    {
        return "Car";
    }
};

// Polymorphism
double getVehicleRate(char type)
{
    if (type == 'B')
    {
        Bike bike;
        Vehicle* v = &bike;
        return v->getRate();
    }
    else
    {
        Car car;
        Vehicle* v = &car;
        return v->getRate();
    }
}

const char* getVehicleName(char type)
{
    return (type == 'B') ? "Bike" : "Car";
}

// System Date and Time
void getSystemTimestamp(time_t value, char* buffer)
{
    struct tm* timeInfo = localtime(&value);

    if (timeInfo != NULL)
        strftime(buffer, 30, "%d-%m-%Y %I:%M:%S %p", timeInfo);
    else
        strcpy(buffer, "Unknown");
}

// Doubly Linked List Node
struct VehicleRecord
{
    char plate[20];
    char type;
    int slot;
    time_t entryTime;

    VehicleRecord* prev;
    VehicleRecord* next;
};

// Doubly Linked List
class VehicleManager
{
private:
    VehicleRecord* head;
    VehicleRecord* tail;
    int count;

public:
    VehicleManager()
    {
        head = NULL;
        tail = NULL;
        count = 0;
    }

    VehicleRecord* getHead()
    {
        return head;
    }

    void setHead(VehicleRecord* newHead)
    {
        head = newHead;
    }

    int getCount()
    {
        return count;
    }

    bool add(const char* plate, char type, int slot, time_t entry)
    {
        VehicleRecord* node = new VehicleRecord;

        if (node == NULL)
            return false;

        strcpy(node->plate, plate);

        node->type = type;
        node->slot = slot;
        node->entryTime = entry;

        node->prev = tail;
        node->next = NULL;

        if (head == NULL)
            head = node;
        else
            tail->next = node;

        tail = node;
        count++;

        return true;
    }

    VehicleRecord* findBySlot(int slot)
    {
        VehicleRecord* temp = head;

        while (temp != NULL)
        {
            if (temp->slot == slot)
                return temp;

            temp = temp->next;
        }

        return NULL;
    }

    bool removeBySlot(int slot)
    {
        VehicleRecord* temp = findBySlot(slot);

        if (temp == NULL)
            return false;

        if (temp == head)
            head = temp->next;

        if (temp == tail)
            tail = temp->prev;

        if (temp->prev != NULL)
            temp->prev->next = temp->next;

        if (temp->next != NULL)
            temp->next->prev = temp->prev;

        delete temp;
        count--;

        return true;
    }

    void display()
    {
        if (head == NULL)
        {
            cout << "\nNo vehicles are currently parked.\n";
            return;
        }

        cout << "\n";
        cout << left << setw(20) << "Vehicle Number"
             << setw(12) << "Type"
             << setw(10) << "Slot"
             << "Entry Time\n";

        cout << "---------------------------------------------------------------\n";

        VehicleRecord* temp = head;

        while (temp != NULL)
        {
            char timeBuffer[30];

            getSystemTimestamp(temp->entryTime, timeBuffer);

            cout << left << setw(20) << temp->plate
                 << setw(12) << getVehicleName(temp->type)
                 << setw(10) << temp->slot
                 << timeBuffer << "\n";

            temp = temp->next;
        }
    }

    ~VehicleManager()
    {
        VehicleRecord* temp = head;

        while (temp != NULL)
        {
            VehicleRecord* nextNode = temp->next;

            delete temp;
            temp = nextNode;
        }
    }
};

// ============================================================
// MEMBER 2: MERGE SORT AND MIN HEAP
// ============================================================

// Merge two sorted linked lists
VehicleRecord* mergeLists(VehicleRecord* a, VehicleRecord* b)
{
    if (a == NULL)
        return b;

    if (b == NULL)
        return a;

    VehicleRecord* result = NULL;

    if (strcmp(a->plate, b->plate) <= 0)
    {
        result = a;
        result->next = mergeLists(a->next, b);
    }
    else
    {
        result = b;
        result->next = mergeLists(a, b->next);
    }

    return result;
}

// Merge Sort
VehicleRecord* mergeSort(VehicleRecord* start)
{
    if (start == NULL || start->next == NULL)
        return start;

    VehicleRecord* slow = start;
    VehicleRecord* fast = start->next;

    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    VehicleRecord* second = slow->next;
    slow->next = NULL;

    VehicleRecord* left = mergeSort(start);
    VehicleRecord* right = mergeSort(second);

    return mergeLists(left, right);
}

// Sort vehicle records by vehicle number
void sortVehicles(VehicleManager& vehicles)
{
    VehicleRecord* head = vehicles.getHead();

    if (head == NULL || head->next == NULL)
    {
        cout << "\nNot enough vehicles to sort.\n";
        return;
    }

    head = mergeSort(head);

    VehicleRecord* temp = head;
    VehicleRecord* previous = NULL;

    while (temp != NULL)
    {
        temp->prev = previous;
        previous = temp;
        temp = temp->next;
    }

    vehicles.setHead(head);

    cout << "\nVehicles sorted using Merge Sort.\n";
}

// ============================================================
// MIN HEAP FOR PARKING SLOT ALLOCATION
// ============================================================

class SlotManager
{
private:
    int heap[CAPACITY + 1];
    int size;

    void heapifyUp(int index)
    {
        while (index > 1 && heap[index] < heap[index / 2])
        {
            int temp = heap[index];
            heap[index] = heap[index / 2];
            heap[index / 2] = temp;

            index = index / 2;
        }
    }

    void heapifyDown(int index)
    {
        while (2 * index <= size)
        {
            int child = 2 * index;

            if (child + 1 <= size &&
                heap[child + 1] < heap[child])
                child++;

            if (heap[index] <= heap[child])
                break;

            int temp = heap[index];
            heap[index] = heap[child];
            heap[child] = temp;

            index = child;
        }
    }

public:
    SlotManager()
    {
        size = CAPACITY;

        for (int i = 1; i <= CAPACITY; i++)
            heap[i] = i;

        for (int i = size / 2; i >= 1; i--)
            heapifyDown(i);
    }

    bool isFull()
    {
        return size == 0;
    }

    int availableCount()
    {
        return size;
    }

    int getFreeSlot()
    {
        if (size == 0)
            return -1;

        int minimum = heap[1];

        heap[1] = heap[size];
        size--;

        if (size > 0)
            heapifyDown(1);

        return minimum;
    }

    void releaseSlot(int slot)
    {
        if (size >= CAPACITY)
            return;

        size++;
        heap[size] = slot;

        heapifyUp(size);
    }

    bool occupySlot(int slot)
    {
        int index = -1;

        for (int i = 1; i <= size; i++)
        {
            if (heap[i] == slot)
            {
                index = i;
                break;
            }
        }

        if (index == -1)
            return false;

        heap[index] = heap[size];
        size--;

        if (index <= size)
        {
            if (index > 1 && heap[index] < heap[index / 2])
                heapifyUp(index);
            else
                heapifyDown(index);
        }

        return true;
    }

    void displayFreeSlots()
    {
        if (size == 0)
        {
            cout << "\nNo free parking slots available.\n";
            return;
        }

        cout << "\nAvailable Parking Slots:\n";

        for (int i = 1; i <= size; i++)
            cout << heap[i] << " ";

        cout << "\nTotal Free Slots: " << size << "\n";
    }
};

// ============================================================
// MEMBER 3: CIRCULAR QUEUE FOR WAITING VEHICLES
// ============================================================

struct WaitingVehicle
{
    char plate[20];
    char type;
};

class WaitingQueue
{
private:
    WaitingVehicle queue[WAIT_SIZE];

    int front;
    int rear;
    int count;

public:
    WaitingQueue()
    {
        front = 0;
        rear = -1;
        count = 0;
    }

    bool isEmpty()
    {
        return count == 0;
    }

    bool isFull()
    {
        return count == WAIT_SIZE;
    }

    bool contains(const char* plate)
    {
        for (int i = 0; i < count; i++)
        {
            int index = (front + i) % WAIT_SIZE;

            if (strcmp(queue[index].plate, plate) == 0)
                return true;
        }

        return false;
    }

    bool enqueue(const char* plate, char type)
    {
        if (isFull())
            return false;

        rear = (rear + 1) % WAIT_SIZE;

        strcpy(queue[rear].plate, plate);
        queue[rear].type = type;

        count++;

        return true;
    }

    bool dequeue(WaitingVehicle& vehicle)
    {
        if (isEmpty())
            return false;

        vehicle = queue[front];

        front = (front + 1) % WAIT_SIZE;

        count--;

        return true;
    }

    void display()
    {
        if (isEmpty())
        {
            cout << "\nWaiting queue is empty.\n";
            return;
        }

        cout << "\nWaiting Vehicles:\n";

        for (int i = 0; i < count; i++)
        {
            int index = (front + i) % WAIT_SIZE;

            cout << i + 1 << ". "
                 << queue[index].plate
                 << " - "
                 << getVehicleName(queue[index].type)
                 << "\n";
        }
    }
};

// ============================================================
// HASH TABLE WITH SEPARATE CHAINING
// ONLY ONE SEARCH: VEHICLE NUMBER
// ============================================================

struct HashNode
{
    char plate[20];
    int slot;

    HashNode* next;
};

class VehicleHashTable
{
private:
    HashNode* table[HASH_SIZE];

    int hashFunction(const char* plate)
    {
        unsigned long hash = 0;

        for (int i = 0; plate[i] != '\0'; i++)
            hash = (hash * 31 + plate[i]) % HASH_SIZE;

        return (int)hash;
    }

public:
    VehicleHashTable()
    {
        for (int i = 0; i < HASH_SIZE; i++)
            table[i] = NULL;
    }

    bool insert(const char* plate, int slot)
    {
        int index = hashFunction(plate);

        HashNode* temp = table[index];

        while (temp != NULL)
        {
            if (strcmp(temp->plate, plate) == 0)
                return false;

            temp = temp->next;
        }

        HashNode* node = new HashNode;

        strcpy(node->plate, plate);
        node->slot = slot;

        node->next = table[index];
        table[index] = node;

        return true;
    }

    // Search by vehicle number
    int search(const char* plate)
    {
        int index = hashFunction(plate);

        HashNode* temp = table[index];

        while (temp != NULL)
        {
            if (strcmp(temp->plate, plate) == 0)
                return temp->slot;

            temp = temp->next;
        }

        return -1;
    }

    void remove(const char* plate)
    {
        int index = hashFunction(plate);

        HashNode* temp = table[index];
        HashNode* previous = NULL;

        while (temp != NULL)
        {
            if (strcmp(temp->plate, plate) == 0)
            {
                if (previous == NULL)
                    table[index] = temp->next;
                else
                    previous->next = temp->next;

                delete temp;
                return;
            }

            previous = temp;
            temp = temp->next;
        }
    }

    void clear()
    {
        for (int i = 0; i < HASH_SIZE; i++)
        {
            HashNode* temp = table[i];

            while (temp != NULL)
            {
                HashNode* nextNode = temp->next;

                delete temp;
                temp = nextNode;
            }

            table[i] = NULL;
        }
    }

    ~VehicleHashTable()
    {
        clear();
    }
};

// ============================================================
// MEMBER 4: BINARY SEARCH TREE
// ============================================================

struct TreeNode
{
    int slot;

    TreeNode* left;
    TreeNode* right;
};

class ParkingTree
{
private:
    TreeNode* root;

    TreeNode* insertNode(TreeNode* node, int slot)
    {
        if (node == NULL)
        {
            TreeNode* newNode = new TreeNode;

            newNode->slot = slot;
            newNode->left = NULL;
            newNode->right = NULL;

            return newNode;
        }

        if (slot < node->slot)
            node->left = insertNode(node->left, slot);

        else if (slot > node->slot)
            node->right = insertNode(node->right, slot);

        return node;
    }

    TreeNode* findMinimum(TreeNode* node)
    {
        while (node != NULL && node->left != NULL)
            node = node->left;

        return node;
    }

    TreeNode* deleteNode(TreeNode* node, int slot)
    {
        if (node == NULL)
            return NULL;

        if (slot < node->slot)
            node->left = deleteNode(node->left, slot);

        else if (slot > node->slot)
            node->right = deleteNode(node->right, slot);

        else
        {
            if (node->left == NULL)
            {
                TreeNode* temp = node->right;

                delete node;
                return temp;
            }

            if (node->right == NULL)
            {
                TreeNode* temp = node->left;

                delete node;
                return temp;
            }

            TreeNode* temp = findMinimum(node->right);

            node->slot = temp->slot;

            node->right = deleteNode(node->right, temp->slot);
        }

        return node;
    }

    void inorder(TreeNode* node)
    {
        if (node == NULL)
            return;

        inorder(node->left);

        cout << node->slot << " ";

        inorder(node->right);
    }

    void destroy(TreeNode* node)
    {
        if (node == NULL)
            return;

        destroy(node->left);
        destroy(node->right);

        delete node;
    }

public:
    ParkingTree()
    {
        root = NULL;
    }

    void insert(int slot)
    {
        root = insertNode(root, slot);
    }

    void remove(int slot)
    {
        root = deleteNode(root, slot);
    }

    void display()
    {
        if (root == NULL)
        {
            cout << "\nNo occupied slots.\n";
            return;
        }

        cout << "\nOccupied Slots (BST Inorder):\n";

        inorder(root);

        cout << "\n";
    }

    ~ParkingTree()
    {
        destroy(root);
    }
};

// ============================================================
// MEMBER 4: STACK FOR RECENT BILLS
// ============================================================

class BillStack
{
private:
    double bills[BILL_SIZE];
    int top;

public:
    BillStack()
    {
        top = -1;
    }

    void push(double bill)
    {
        if (top == BILL_SIZE - 1)
        {
            cout << "\nBill history stack is full.\n";
            return;
        }

        bills[++top] = bill;
    }

    void displayRecentBill()
    {
        if (top == -1)
        {
            cout << "\nNo bills generated yet.\n";
            return;
        }

        cout << "\nMost Recent Bill: Rs. "
             << fixed << setprecision(2)
             << bills[top] << "\n";
    }
};

// ============================================================
// MEMBER 4: PARKING SYSTEM
// INTEGRATION OF ALL DATA STRUCTURES
// ============================================================

class ParkingSystem
{
private:
    VehicleManager vehicles;
    SlotManager slots;
    WaitingQueue waiting;
    VehicleHashTable hashTable;
    ParkingTree parkingTree;
    BillStack billStack;

    double totalRevenue;

    bool plateIsValid(const char* plate)
    {
        int length = strlen(plate);

        return length > 0 && length < 20;
    }

    bool parkAtSlot(const char* plate, char type,
                    int slot, time_t entry)
    {
        if (!vehicles.add(plate, type, slot, entry))
            return false;

        if (!hashTable.insert(plate, slot))
        {
            vehicles.removeBySlot(slot);
            return false;
        }

        parkingTree.insert(slot);

        char timeBuffer[30];

        getSystemTimestamp(entry, timeBuffer);

        cout << "\nVehicle parked successfully!\n";
        cout << "Vehicle Number : " << plate << "\n";
        cout << "Vehicle Type   : " << getVehicleName(type) << "\n";
        cout << "Parking Slot   : " << slot << "\n";
        cout << "Entry Time     : " << timeBuffer << "\n";

        return true;
    }

    // Automatically park waiting vehicles
    void processWaitingVehicle()
    {
        while (!slots.isFull() && !waiting.isEmpty())
        {
            WaitingVehicle vehicle;

            if (!waiting.dequeue(vehicle))
                break;

            int slot = slots.getFreeSlot();

            if (slot == -1)
                break;

            if (!parkAtSlot(vehicle.plate, vehicle.type,
                            slot, time(NULL)))
            {
                slots.releaseSlot(slot);

                cout << "\nCould not park waiting vehicle: "
                     << vehicle.plate << "\n";
            }
            else
            {
                cout << "Waiting vehicle has been assigned a slot.\n";
            }
        }
    }

    // Save parking records
    void saveData()
    {
        ofstream file("parking_data.txt");

        if (!file)
        {
            cout << "\nError saving parking data.\n";
            return;
        }

        VehicleRecord* temp = vehicles.getHead();

        while (temp != NULL)
        {
            file << temp->plate << " "
                 << temp->type << " "
                 << temp->slot << " "
                 << (long long)temp->entryTime << "\n";

            temp = temp->next;
        }

        file.close();

        ofstream revenueFile("revenue.txt");

        if (revenueFile)
        {
            revenueFile << totalRevenue;
            revenueFile.close();
        }

        cout << "\nParking data and revenue saved successfully.\n";
    }

    // Load saved parking records
    void loadData()
    {
        ifstream file("parking_data.txt");

        if (file)
        {
            char plate[20];
            char type;
            int slot;
            long long entry;

            while (file >> setw(20) >> plate
                        >> type >> slot >> entry)
            {
                if ((type != 'B' && type != 'C') ||
                    slot < 1 || slot > CAPACITY)
                    continue;

                if (vehicles.findBySlot(slot) != NULL)
                    continue;

                if (hashTable.search(plate) != -1)
                    continue;

                if (!slots.occupySlot(slot))
                    continue;

                if (vehicles.add(plate, type, slot, (time_t)entry))
                {
                    hashTable.insert(plate, slot);
                    parkingTree.insert(slot);
                }
                else
                {
                    slots.releaseSlot(slot);
                }
            }

            file.close();
        }

        ifstream revenueFile("revenue.txt");

        if (revenueFile)
        {
            revenueFile >> totalRevenue;
            revenueFile.close();
        }

        cout << "\nSaved parking records loaded successfully.\n";
    }

public:
    ParkingSystem()
    {
        totalRevenue = 0;

        loadData();
    }

    // Park vehicle
    void parkVehicle()
    {
        char plate[20];
        int choice;
        char type;

        cout << "\nEnter Vehicle Number: ";

        cin.width(20);
        cin >> plate;

        if (!plateIsValid(plate))
        {
            cout << "\nInvalid vehicle number.\n";
            return;
        }

        if (hashTable.search(plate) != -1 ||
            waiting.contains(plate))
        {
            cout << "\nThis vehicle is already parked or waiting.\n";
            return;
        }

        cout << "\nSelect Vehicle Type:\n";
        cout << "1. Bike\n";
        cout << "2. Car\n";
        cout << "Enter choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\nInvalid vehicle type.\n";
            return;
        }

        if (choice == 1)
            type = 'B';

        else if (choice == 2)
            type = 'C';

        else
        {
            cout << "\nInvalid vehicle type.\n";
            return;
        }

        if (slots.isFull())
        {
            if (waiting.enqueue(plate, type))
            {
                cout << "\nParking is full.\n";
                cout << "Vehicle added to waiting queue.\n";
            }
            else
            {
                cout << "\nWaiting queue is also full.\n";
            }

            return;
        }

        int slot = slots.getFreeSlot();

        if (slot == -1)
        {
            cout << "\nNo free slot available.\n";
            return;
        }

        if (!parkAtSlot(plate, type, slot, time(NULL)))
        {
            slots.releaseSlot(slot);

            cout << "\nUnable to park vehicle.\n";
        }
    }

    // Exit vehicle and generate bill
    void exitVehicle()
    {
        char plate[20];

        cout << "\nEnter Vehicle Number to Exit: ";

        cin.width(20);
        cin >> plate;

        int slot = hashTable.search(plate);

        if (slot == -1)
        {
            cout << "\nVehicle not found.\n";
            return;
        }

        VehicleRecord* record = vehicles.findBySlot(slot);

        if (record == NULL)
        {
            cout << "\nParking record not found.\n";
            return;
        }

        time_t exitTime = time(NULL);

        double seconds = difftime(exitTime, record->entryTime);

        if (seconds < 0)
            seconds = 0;

        // Minimum billing period is one hour
        int hours = (int)((seconds + 3599) / 3600);

        if (hours < 1)
            hours = 1;

        double rate = getVehicleRate(record->type);

        double bill = hours * rate;

        char entryBuffer[30];
        char exitBuffer[30];

        getSystemTimestamp(record->entryTime, entryBuffer);
        getSystemTimestamp(exitTime, exitBuffer);

        cout << "\n========== PARKING BILL ==========\n";

        cout << "Vehicle Number : " << record->plate << "\n";
        cout << "Vehicle Type   : " << getVehicleName(record->type) << "\n";
        cout << "Parking Slot   : " << slot << "\n";
        cout << "Entry Time     : " << entryBuffer << "\n";
        cout << "Exit Time      : " << exitBuffer << "\n";
        cout << "Parking Hours  : " << hours << "\n";
        cout << "Rate Per Hour  : Rs. " << rate << "\n";

        cout << "Total Bill     : Rs. "
             << fixed << setprecision(2)
             << bill << "\n";

        cout << "==================================\n";

        totalRevenue += bill;

        billStack.push(bill);

        // Save exit details in history file
        ofstream history("parking_history.txt", ios::app);

        if (history)
        {
            history << "Vehicle: " << record->plate
                    << " | Type: " << getVehicleName(record->type)
                    << " | Slot: " << slot
                    << " | Entry: " << entryBuffer
                    << " | Exit: " << exitBuffer
                    << " | Bill: Rs. " << bill << "\n";

            history.close();
        }

        // Remove vehicle from all active records
        hashTable.remove(plate);

        parkingTree.remove(slot);

        vehicles.removeBySlot(slot);

        slots.releaseSlot(slot);

        cout << "\nVehicle exited successfully.\n";

        // Assign the released slot to a waiting vehicle
        processWaitingVehicle();
    }

    // Only user-facing search: vehicle number
    void searchVehicle()
    {
        char plate[20];

        cout << "\nEnter Vehicle Number to Search: ";

        cin.width(20);
        cin >> plate;

        int slot = hashTable.search(plate);

        if (slot == -1)
        {
            cout << "\nVehicle not found in parking records.\n";
            return;
        }

        VehicleRecord* record = vehicles.findBySlot(slot);

        if (record == NULL)
        {
            cout << "\nVehicle record not found.\n";
            return;
        }

        char timeBuffer[30];

        getSystemTimestamp(record->entryTime, timeBuffer);

        cout << "\n========== VEHICLE DETAILS ==========\n";

        cout << "Vehicle Number : " << record->plate << "\n";
        cout << "Vehicle Type   : " << getVehicleName(record->type) << "\n";
        cout << "Parking Slot   : " << record->slot << "\n";
        cout << "Entry Time     : " << timeBuffer << "\n";

        cout << "=====================================\n";
    }

    void displayVehicles()
    {
        vehicles.display();
    }

    void displayFreeSlots()
    {
        slots.displayFreeSlots();
    }

    void sortAndDisplayVehicles()
    {
        sortVehicles(vehicles);
        vehicles.display();
    }

    void showRevenue()
    {
        cout << "\n========== REVENUE DETAILS ==========\n";

        cout << "Total Revenue: Rs. "
             << fixed << setprecision(2)
             << totalRevenue << "\n";

        billStack.displayRecentBill();

        cout << "=====================================\n";
    }

    void displayWaitingQueue()
    {
        waiting.display();
    }

    void displayHistory()
    {
        ifstream file("parking_history.txt");

        if (!file)
        {
            cout << "\nNo parking history available.\n";
            return;
        }

        cout << "\n========== PARKING HISTORY ==========\n";

        char line[300];

        while (file.getline(line, sizeof(line)))
            cout << line << "\n";

        file.close();

        cout << "=====================================\n";
    }

    void displayOccupiedSlots()
    {
        parkingTree.display();
    }

    // Menu-driven execution
    void run()
    {
        int choice;

        do
        {
            cout << "\n\n";
            cout << "====================================\n";
            cout << "       PARKING MANAGEMENT SYSTEM\n";
            cout << "====================================\n";

            cout << "1. Park Vehicle\n";
            cout << "2. Exit Vehicle\n";
            cout << "3. Search Vehicle by Plate\n";
            cout << "4. Display Parked Vehicles\n";
            cout << "5. Display Free Slots\n";
            cout << "6. Sort Vehicles (Merge Sort)\n";
            cout << "7. Show Revenue\n";
            cout << "8. Waiting Queue\n";
            cout << "9. Parking History\n";
            cout << "10. Display Occupied Slots (BST)\n";
            cout << "0. Save and Exit\n";

            cout << "====================================\n";
            cout << "Enter your choice: ";

            if (!(cin >> choice))
            {
                cin.clear();
                cin.ignore(1000, '\n');

                cout << "\nInvalid input. Enter a number.\n";

                continue;
            }

            switch (choice)
            {
                case 1:
                    parkVehicle();
                    break;

                case 2:
                    exitVehicle();
                    break;

                case 3:
                    searchVehicle();
                    break;

                case 4:
                    displayVehicles();
                    break;

                case 5:
                    displayFreeSlots();
                    break;

                case 6:
                    sortAndDisplayVehicles();
                    break;

                case 7:
                    showRevenue();
                    break;

                case 8:
                    displayWaitingQueue();
                    break;

                case 9:
                    displayHistory();
                    break;

                case 10:
                    displayOccupiedSlots();
                    break;

                case 0:
                    saveData();

                    cout << "\nThank you for using Parking Management System!\n";
                    break;

                default:
                    cout << "\nInvalid choice. Please try again.\n";
            }

        } while (choice != 0);
    }
};

// ============================================================
// MEMBER 4: MAIN FUNCTION
// ============================================================

int main()
{
    ParkingSystem parking;

    parking.run();

    return 0;
}