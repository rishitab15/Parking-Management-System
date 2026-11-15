struct HashNode
{
    char plate[20];
    HashNode *next;
};

class VehicleHashTable
{
private:
    HashNode *table[HASH_SIZE];

    int hashFunction(const char plate[])
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

    bool search(const char plate[])
    {
        int index = hashFunction(plate);
        HashNode *temp = table[index];

        while (temp != NULL)
        {
            if (strcmp(temp->plate, plate) == 0)
                return true;

            temp = temp->next;
        }

        return false;
    }

    void insert(const char plate[])
    {
        if (search(plate))
            return;

        int index = hashFunction(plate);

        HashNode *node = new HashNode;
        strcpy(node->plate, plate);

        node->next = table[index];
        table[index] = node;
    }

    void remove(const char plate[])
    {
        int index = hashFunction(plate);

        HashNode *temp = table[index];
        HashNode *previous = NULL;

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

    ~VehicleHashTable()
    {
        for (int i = 0; i < HASH_SIZE; i++)
        {
            HashNode *temp = table[i];

            while (temp != NULL)
            {
                HashNode *nextNode = temp->next;
                delete temp;
                temp = nextNode;
            }
        }
    }
};

// Circular Queue for waiting vehicles
struct WaitingVehicle
{
    char plate[20];
    int type;
};

class WaitingQueue
{
private:
    WaitingVehicle items[WAIT_SIZE];
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

    bool contains(const char plate[])
    {
        for (int i = 0; i < count; i++)
        {
            int index = (front + i) % WAIT_SIZE;

            if (strcmp(items[index].plate, plate) == 0)
                return true;
        }

        return false;
    }

    bool enqueue(const char plate[], int type)
    {
        if (isFull())
        {
            cout << "\nWaiting queue is full.\n";
            return false;
        }

        if (contains(plate))
        {
            cout << "\nVehicle is already in the waiting queue.\n";
            return false;
        }

        rear = (rear + 1) % WAIT_SIZE;

        strcpy(items[rear].plate, plate);
        items[rear].type = type;

        count++;

        cout << "\nVehicle added to waiting queue.\n";
        return true;
    }

    bool dequeue(WaitingVehicle &vehicle)
    {
        if (isEmpty())
            return false;

        vehicle = items[front];

        front = (front + 1) % WAIT_SIZE;
        count--;

        return true;
    }

    void display()
    {
        if (isEmpty())
        {
            cout << "\nNo vehicles are waiting.\n";
            return;
        }

        cout << "\n========== WAITING QUEUE ==========\n";

        for (int i = 0; i < count; i++)
        {
            int index = (front + i) % WAIT_SIZE;

            cout << i + 1 << ". "
                 << items[index].plate << " - "
                 << (items[index].type == 1 ? "Bike" : "Car")
                 << "\n";
        }
    }
};