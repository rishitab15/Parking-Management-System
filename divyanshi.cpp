struct BSTNode
{
    int slot;
    BSTNode *left;
    BSTNode *right;
};

class OccupiedSlotBST
{
private:
    BSTNode* insertNode(BSTNode *root, int slot)
    {
        if (root == NULL)
        {
            BSTNode *node = new BSTNode;
            node->slot = slot;
            node->left = NULL;
            node->right = NULL;
            return node;
        }

        if (slot < root->slot)
            root->left = insertNode(root->left, slot);

        else if (slot > root->slot)
            root->right = insertNode(root->right, slot);

        return root;
    }

    BSTNode* minimumNode(BSTNode *root)
    {
        while (root != NULL && root->left != NULL)
            root = root->left;

        return root;
    }

    BSTNode* deleteNode(BSTNode *root, int slot)
    {
        if (root == NULL)
            return root;

        if (slot < root->slot)
            root->left = deleteNode(root->left, slot);

        else if (slot > root->slot)
            root->right = deleteNode(root->right, slot);

        else
        {
            if (root->left == NULL)
            {
                BSTNode *temp = root->right;
                delete root;
                return temp;
            }

            if (root->right == NULL)
            {
                BSTNode *temp = root->left;
                delete root;
                return temp;
            }

            BSTNode *temp = minimumNode(root->right);

            root->slot = temp->slot;
            root->right = deleteNode(root->right, temp->slot);
        }

        return root;
    }

    void inorder(BSTNode *root)
    {
        if (root == NULL)
            return;

        inorder(root->left);
        cout << root->slot << " ";
        inorder(root->right);
    }

    void destroy(BSTNode *root)
    {
        if (root == NULL)
            return;

        destroy(root->left);
        destroy(root->right);
        delete root;
    }

public:
    BSTNode *root;

    OccupiedSlotBST()
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

        cout << "\nOccupied slots in sorted order: ";
        inorder(root);
        cout << "\n";
    }

    ~OccupiedSlotBST()
    {
        destroy(root);
    }
};

struct AVLNode
{
    char plate[20];
    int slot;
    int height;

    AVLNode *left;
    AVLNode *right;
};

class PlateAVLTree
{
private:
    int height(AVLNode *node)
    {
        return (node == NULL) ? 0 : node->height;
    }

    int maxValue(int a, int b)
    {
        return (a > b) ? a : b;
    }

    int balanceFactor(AVLNode *node)
    {
        if (node == NULL)
            return 0;

        return height(node->left) - height(node->right);
    }

    AVLNode* createNode(const char plate[], int slot)
    {
        AVLNode *node = new AVLNode;

        strcpy(node->plate, plate);
        node->slot = slot;
        node->height = 1;

        node->left = NULL;
        node->right = NULL;

        return node;
    }

    AVLNode* rightRotate(AVLNode *y)
    {
        AVLNode *x = y->left;
        AVLNode *temp = x->right;

        x->right = y;
        y->left = temp;

        y->height = 1 + maxValue(height(y->left), height(y->right));
        x->height = 1 + maxValue(height(x->left), height(x->right));

        return x;
    }

    AVLNode* leftRotate(AVLNode *x)
    {
        AVLNode *y = x->right;
        AVLNode *temp = y->left;

        y->left = x;
        x->right = temp;

        x->height = 1 + maxValue(height(x->left), height(x->right));
        y->height = 1 + maxValue(height(y->left), height(y->right));

        return y;
    }

    AVLNode* insertNode(AVLNode *node, const char plate[], int slot)
    {
        if (node == NULL)
            return createNode(plate, slot);

        int comparison = strcmp(plate, node->plate);

        if (comparison < 0)
            node->left = insertNode(node->left, plate, slot);

        else if (comparison > 0)
            node->right = insertNode(node->right, plate, slot);

        else
        {
            node->slot = slot;
            return node;
        }

        node->height = 1 + maxValue(height(node->left), height(node->right));

        int balance = balanceFactor(node);

        if (balance > 1 && strcmp(plate, node->left->plate) < 0)
            return rightRotate(node);

        if (balance < -1 && strcmp(plate, node->right->plate) > 0)
            return leftRotate(node);

        if (balance > 1 && strcmp(plate, node->left->plate) > 0)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        if (balance < -1 && strcmp(plate, node->right->plate) < 0)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    AVLNode* minimumNode(AVLNode *node)
    {
        AVLNode *current = node;

        while (current->left != NULL)
            current = current->left;

        return current;
    }

    AVLNode* deleteNode(AVLNode *root, const char plate[])
    {
        if (root == NULL)
            return root;

        int comparison = strcmp(plate, root->plate);

        if (comparison < 0)
            root->left = deleteNode(root->left, plate);

        else if (comparison > 0)
            root->right = deleteNode(root->right, plate);

        else
        {
            if (root->left == NULL || root->right == NULL)
            {
                AVLNode *temp = root->left ? root->left : root->right;

                if (temp == NULL)
                {
                    delete root;
                    return NULL;
                }
                else
                {
                    *root = *temp;
                    delete temp;
                }
            }
            else
            {
                AVLNode *temp = minimumNode(root->right);

                strcpy(root->plate, temp->plate);
                root->slot = temp->slot;

                root->right = deleteNode(root->right, temp->plate);
            }
        }

        if (root == NULL)
            return root;

        root->height = 1 + maxValue(height(root->left), height(root->right));

        int balance = balanceFactor(root);

        if (balance > 1 && balanceFactor(root->left) >= 0)
            return rightRotate(root);

        if (balance > 1 && balanceFactor(root->left) < 0)
        {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        if (balance < -1 && balanceFactor(root->right) <= 0)
            return leftRotate(root);

        if (balance < -1 && balanceFactor(root->right) > 0)
        {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    void inorder(AVLNode *root)
    {
        if (root == NULL)
            return;

        inorder(root->left);
        cout << root->plate << " (Slot " << root->slot << ")  ";
        inorder(root->right);
    }

    void destroy(AVLNode *root)
    {
        if (root == NULL)
            return;

        destroy(root->left);
        destroy(root->right);
        delete root;
    }

public:
    AVLNode *root;

    PlateAVLTree()
    {
        root = NULL;
    }

    void insert(const char plate[], int slot)
    {
        root = insertNode(root, plate, slot);
    }

    void remove(const char plate[])
    {
        root = deleteNode(root, plate);
    }

    AVLNode* search(const char plate[])
    {
        AVLNode *temp = root;

        while (temp != NULL)
        {
            int comparison = strcmp(plate, temp->plate);

            if (comparison == 0)
                return temp;

            if (comparison < 0)
                temp = temp->left;
            else
                temp = temp->right;
        }

        return NULL;
    }

    void display()
    {
        if (root == NULL)
        {
            cout << "\nAVL Tree is empty.\n";
            return;
        }

        cout << "\nVehicles in AVL Tree: ";
        inorder(root);
        cout << "\n";
    }

    ~PlateAVLTree()
    {
        destroy(root);
    }
};


class ParkingSystem
{
private:
    VehicleManager vehicleManager;
    SlotManager slotManager;
    BillStack billStack;
    VehicleHashTable hashTable;
    WaitingQueue waitingQueue;
    OccupiedSlotBST occupiedBST;
    PlateAVLTree plateAVL;

    double totalRevenue;

    void saveParkingData()
    {
        ofstream file("parking_data.txt");

        if (!file)
        {
            cout << "\nUnable to save parking data.\n";
            return;
        }

        vehicleManager.saveToFile(file);
        file.close();
    }

    void saveRevenue()
    {
        ofstream file("revenue.txt");

        if (file)
        {
            file << totalRevenue;
            file.close();
        }
    }

    void addToHistory(const char text[])
    {
        ofstream file("parking_history.txt", ios::app);

        if (file)
        {
            file << text << "\n";
            file.close();
        }
    }

    void parkVehicle(const char plate[], int type)
    {
        if (hashTable.search(plate))
        {
            cout << "\nThis vehicle is already parked.\n";
            return;
        }

        if (waitingQueue.contains(plate))
        {
            cout << "\nThis vehicle is already in the waiting queue.\n";
            return;
        }

        int slot = slotManager.getFreeSlot(type);

        if (slot == -1)
        {
            cout << "\nNo designated slots available for this vehicle type.\n";
            waitingQueue.enqueue(plate, type);
            return;
        }

        time_t entryTime = time(NULL);

        if (type == 1)
        {
            Bike bike(plate);
            cout << "\nVehicle Rate: Rs. "
                 << bike.getRate() << " per hour\n";
        }
        else
        {
            Car car(plate);
            cout << "\nVehicle Rate: Rs. "
                 << car.getRate() << " per hour\n";
        }

        vehicleManager.insert(plate, type, slot, entryTime);

        hashTable.insert(plate);
        occupiedBST.insert(slot);
        plateAVL.insert(plate, slot);

        cout << "\n========== PARKING CONFIRMED ==========\n";
        cout << "Vehicle Number : " << plate << "\n";
        cout << "Vehicle Type   : " << (type == 1 ? "Bike" : "Car") << "\n";
        cout << "Allocated Slot : " << slot << "\n";
        cout << "Entry Time     : ";
        displayTime(entryTime);
        cout << "\n=======================================\n";

        char history[250];
        char timeText[50];

        getSystemTime(timeText);

        sprintf(history,
                "PARKED | Plate: %s | Type: %s | Slot: %d | Time: %s",
                plate, (type == 1 ? "Bike" : "Car"), slot, timeText);

        addToHistory(history);
        saveParkingData();
    }

    void processWaitingQueue()
    {
        if (waitingQueue.isEmpty())
            return;

        WaitingVehicle vehicle;

        if (!waitingQueue.dequeue(vehicle))
            return;

        if (slotManager.getAvailableCount(vehicle.type) == 0)
        {

            waitingQueue.enqueue(vehicle.plate, vehicle.type);
            return;
        }

        cout << "\nA waiting vehicle is being allocated a slot...\n";
        parkVehicle(vehicle.plate, vehicle.type);
    }

    void exitVehicle(const char plate[])
    {
        VehicleRecord *record = vehicleManager.find(plate);

        if (record == NULL)
        {
            cout << "\nVehicle not found in parking.\n";
            return;
        }

        time_t exitTime = time(NULL);

        double seconds = difftime(exitTime, record->entryTime);

        int hours = (int)(seconds / 3600);

        if (seconds > hours * 3600)
            hours++;

        if (hours < 1)
            hours = 1;

        int rate = (record->type == 1) ? 30 : 60;
        double bill = hours * rate;

        int slot = record->slot;
        int type = record->type;
        time_t entryTime = record->entryTime;

        cout << "\n========== PARKING BILL ==========\n";
        cout << "Vehicle Number : " << plate << "\n";
        cout << "Vehicle Type   : " << (type == 1 ? "Bike" : "Car") << "\n";
        cout << "Slot Number    : " << slot << "\n";

        cout << "Entry Time     : ";
        displayTime(entryTime);

        cout << "\nExit Time      : ";
        displayTime(exitTime);

        cout << "\nParking Hours  : " << hours;
        cout << "\nRate Per Hour  : Rs. " << rate;
        cout << "\nTotal Bill     : Rs. " << bill;
        cout << "\n==================================\n";

        totalRevenue += bill;

        char billText[250];
        char timeText[50];

        getSystemTime(timeText);

        sprintf(billText,
                "Plate: %s | Type: %s | Slot: %d | Hours: %d | Bill: Rs. %.2f | Exit: %s",
                plate, (type == 1 ? "Bike" : "Car"), slot,
                hours, bill, timeText);

        billStack.push(billText);
        addToHistory(billText);

        vehicleManager.remove(plate);
        hashTable.remove(plate);
        occupiedBST.remove(slot);
        plateAVL.remove(plate);

        slotManager.insertSlot(slot);

        saveParkingData();
        saveRevenue();

        cout << "\nVehicle exited successfully.\n";

        processWaitingQueue();
    }

    void loadData()
    {
        bool occupied[CAPACITY + 1] = {false};

        ifstream file("parking_data.txt");

        if (file)
        {
            char plate[20];
            int type, slot;
            long long entry;

            while (file >> setw(20) >> plate >> type >> slot >> entry)
            {
                if ((type != 1 && type != 2) ||
                    slot < 1 || slot > CAPACITY ||
                    occupied[slot] ||
                    hashTable.search(plate))
                    continue;

                // Keep each vehicle in its designated section.
                if ((type == 1 && slot > 100) ||
                    (type == 2 && slot <= 100))
                    continue;

                occupied[slot] = true;

                vehicleManager.insert(plate, type, slot, (time_t)entry);
                hashTable.insert(plate);
                occupiedBST.insert(slot);
                plateAVL.insert(plate, slot);
            }

            file.close();
        }

        // Empty both Min Heaps before rebuilding them.
        for (int i = 1; i <= 100; i++)
            slotManager.getFreeSlot(1);

        for (int i = 101; i <= 200; i++)
            slotManager.getFreeSlot(2);

        // Insert only the slots that are not occupied.
        for (int i = 1; i <= CAPACITY; i++)
        {
            if (!occupied[i])
                slotManager.insertSlot(i);
        }

        ifstream revenueFile("revenue.txt");

        if (revenueFile)
        {
            revenueFile >> totalRevenue;
            revenueFile.close();
        }

        cout << "\nSaved parking data loaded successfully.\n";
    }

public:
    ParkingSystem()
    {
        totalRevenue = 0;
        loadData();
    }

    void parkMenu()
    {
        char plate[20];
        int type;

        cout << "\nEnter vehicle plate number: ";
        cin >> setw(20) >> plate;

        cout << "\nSelect Vehicle Type:\n";
        cout << "1. Bike\n";
        cout << "2. Car\n";
        cout << "Enter choice: ";
        cin >> type;

        if (type != 1 && type != 2)
        {
            cout << "\nInvalid vehicle type.\n";
            return;
        }

        parkVehicle(plate, type);
    }

    void exitMenu()
    {
        char plate[20];

        cout << "\nEnter vehicle plate number: ";
        cin >> setw(20) >> plate;

        exitVehicle(plate);
    }

    void searchVehicle()
    {
        char plate[20];

        cout << "\nEnter plate number to search: ";
        cin >> setw(20) >> plate;

        AVLNode *found = plateAVL.search(plate);

        if (found == NULL)
        {
            cout << "\nVehicle not found.\n";
            return;
        }

        VehicleRecord *record = vehicleManager.find(plate);

        if (record == NULL)
        {
            cout << "\nVehicle record not found.\n";
            return;
        }

        cout << "\n========== VEHICLE DETAILS ==========\n";
        cout << "Plate Number : " << record->plate << "\n";
        cout << "Vehicle Type : "
             << (record->type == 1 ? "Bike" : "Car") << "\n";
        cout << "Slot Number  : " << found->slot << "\n";
        cout << "Entry Time   : ";
        displayTime(record->entryTime);
        cout << "\n==================================\n";
    }

    void displayVehicles()
    {
        vehicleManager.display();
    }

    void sortVehicles()
    {
        vehicleManager.sortVehicles();
        saveParkingData();
        vehicleManager.display();
    }

    void displayFreeSlots()
    {
        cout << "\nAvailable Bike Slots: "
             << slotManager.getAvailableCount(1);

        cout << "\nAvailable Car Slots: "
             << slotManager.getAvailableCount(2) << "\n";

        slotManager.displayFreeSlots();
    }

    void displayOccupiedSlots()
    {
        occupiedBST.display();
    }

    void displayWaitingQueue()
    {
        waitingQueue.display();
    }

    void displayRecentBills()
    {
        billStack.displayRecentBills();
    }

    void displayRevenue()
    {
        cout << fixed << setprecision(2);
        cout << "\nTotal Revenue: Rs. " << totalRevenue << "\n";
    }

    void displayAVL()
    {
        plateAVL.display();
    }

    void displayCurrentTime()
    {
        char text[50];
        getSystemTime(text);

        cout << "\nCurrent System Date and Time: " << text << "\n";
    }

    void run()
    {
        int choice;

        do
        {
           
            cout << "\n       PARKING MANAGEMENT SYSTEM";
            cout << "\n";
            cout << "\n1. Park Vehicle";
            cout << "\n2. Exit Vehicle and Generate Bill";
            cout << "\n3. Search Vehicle by Plate Number";
            cout << "\n4. Display Parked Vehicles";
            cout << "\n5. Sort Vehicles by Plate Number";
            cout << "\n6. Display Available Slots";
            cout << "\n7. Display Occupied Slots (BST)";
            cout << "\n8. Display Waiting Queue";
            cout << "\n9. Display Recent Bills (Stack)";
            cout << "\n10. Display Total Revenue";
            cout << "\n0. Save and Exit";
            cout << "\n";
            cout << "\nEnter your choice: ";

            cin >> choice;

            switch (choice)
            {
                case 1:
                    parkMenu();
                    break;

                case 2:
                    exitMenu();
                    break;

                case 3:
                    searchVehicle();
                    break;

                case 4:
                    displayVehicles();
                    break;

                case 5:
                    sortVehicles();
                    break;

                case 6:
                    displayFreeSlots();
                    break;

                case 7:
                    displayOccupiedSlots();
                    break;

                case 8:
                    displayWaitingQueue();
                    break;

                case 9:
                    displayRecentBills();
                    break;

                case 10:
                    displayRevenue();
                    break;

               
                case 0:
                    saveParkingData();
                    saveRevenue();

                    cout << "\nData saved successfully.\n";
                    cout << "Thank you for using Parking Management System.\n";
                    break;

                default:
                    cout << "\nInvalid choice. Please try again.\n";
            }

        } while (choice != 0);
    }
};


int main()
{
    ParkingSystem parking;
    parking.run();

    return 0;
}
