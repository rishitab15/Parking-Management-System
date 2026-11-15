class SlotManager
{
private:
    int bikeHeap[101];
    int carHeap[101];

    int bikeSize;
    int carSize;

    void swapValues(int &a, int &b)
    {
        int temp = a;
        a = b;
        b = temp;
    }

    // Insert into a Min Heap
    void insertHeap(int heap[], int &size, int slot)
    {
        if (size >= 100)
            return;

        size++;
        heap[size] = slot;

        int i = size;

        while (i > 1 && heap[i] < heap[i / 2])
        {
            swapValues(heap[i], heap[i / 2]);
            i = i / 2;
        }
    }

    // Remove the minimum element from a Min Heap
    int removeMinimum(int heap[], int &size)
    {
        if (size == 0)
            return -1;

        int smallest = heap[1];

        heap[1] = heap[size];
        size--;

        int i = 1;

        while (true)
        {
            int left = 2 * i;
            int right = 2 * i + 1;
            int smallestIndex = i;

            if (left <= size &&
                heap[left] < heap[smallestIndex])
                smallestIndex = left;

            if (right <= size &&
                heap[right] < heap[smallestIndex])
                smallestIndex = right;

            if (smallestIndex == i)
                break;

            swapValues(heap[i], heap[smallestIndex]);
            i = smallestIndex;
        }

        return smallest;
    }

    // Display a sorted copy without changing the heap
    void displayHeap(int heap[], int size)
    {
        if (size == 0)
        {
            cout << "No free slots.\n";
            return;
        }

        int temp[101];

        for (int i = 1; i <= size; i++)
            temp[i] = heap[i];

        for (int i = 1; i <= size; i++)
        {
            for (int j = i + 1; j <= size; j++)
            {
                if (temp[i] > temp[j])
                {
                    int x = temp[i];
                    temp[i] = temp[j];
                    temp[j] = x;
                }
            }
        }

        for (int i = 1; i <= size; i++)
            cout << temp[i] << " ";

        cout << "\n";
    }

public:
    SlotManager()
    {
        bikeSize = 0;
        carSize = 0;

        for (int i = 1; i <= 100; i++)
            insertHeap(bikeHeap, bikeSize, i);

        for (int i = 101; i <= 200; i++)
            insertHeap(carHeap, carSize, i);
    }

    // Type 1 = Bike, Type 2 = Car
    int getFreeSlot(int type)
    {
        if (type == 1)
            return removeMinimum(bikeHeap, bikeSize);

        if (type == 2)
            return removeMinimum(carHeap, carSize);

        return -1;
    }

    void insertSlot(int slot)
    {
        if (slot >= 1 && slot <= 100)
            insertHeap(bikeHeap, bikeSize, slot);

        else if (slot >= 101 && slot <= 200)
            insertHeap(carHeap, carSize, slot);
    }

    int getAvailableCount(int type)
    {
        return (type == 1) ? bikeSize : carSize;
    }

    void displayFreeSlots()
    {
        cout << "\n========== AVAILABLE SLOTS ==========\n";

        cout << "\nBIKE PARKING (1-100):\n";
        displayHeap(bikeHeap, bikeSize);

        cout << "\nCAR PARKING (101-200):\n";
        displayHeap(carHeap, carSize);
    }
};

// Stack stores recent bills
class BillStack
{
private:
    char bills[BILL_SIZE][250];
    int top;

public:
    BillStack()
    {
        top = -1;
    }

    void push(const char bill[])
    {
        if (top == BILL_SIZE - 1)
        {
            for (int i = 0; i < BILL_SIZE - 1; i++)
                strcpy(bills[i], bills[i + 1]);

            top--;
        }

        top++;
        strcpy(bills[top], bill);
    }

    void displayRecentBills()
    {
        if (top == -1)
        {
            cout << "\nNo bills generated yet.\n";
            return;
        }

        cout << "\n========== RECENT BILLS ==========\n";

        for (int i = top; i >= 0; i--)
            cout << bills[i] << "\n";
    }
};
