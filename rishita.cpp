
#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <cstdio>
#include <cstdlib>

using namespace std;

const int CAPACITY = 200;
const int HASH_SIZE = 211;
const int WAIT_SIZE = 200;
const int BILL_SIZE = 1000;

void getSystemTime(char text[])
{
    time_t currentTime = time(NULL);
    tm *localTime = localtime(&currentTime);

    if (localTime != NULL)
        strftime(text, 50, "%d-%m-%Y %I:%M:%S %p", localTime);
    else
        strcpy(text, "Time unavailable");
}

void displayTime(time_t value)
{
    char text[50];
    tm *localTime = localtime(&value);

    if (localTime != NULL)
        strftime(text, sizeof(text), "%d-%m-%Y %I:%M:%S %p", localTime);
    else
        strcpy(text, "Time unavailable");

    cout << text;
}

class Vehicle
{
protected:
    char plate[20];

public:
    Vehicle(const char p[])
    {
        strcpy(plate, p);
    }

    virtual int getRate() = 0;
    virtual const char* getType() = 0;

    const char* getPlate()
    {
        return plate;
    }

    virtual ~Vehicle() {}
};

class Bike : public Vehicle
{
public:
    Bike(const char p[]) : Vehicle(p) {}

    int getRate()
    {
        return 30;
    }

    const char* getType()
    {
        return "Bike";
    }
};

class Car : public Vehicle
{
public:
    Car(const char p[]) : Vehicle(p) {}

    int getRate()
    {
        return 60;
    }

    const char* getType()
    {
        return "Car";
    }
};

struct VehicleRecord
{
    char plate[20];
    int type;
    int slot;
    time_t entryTime;

    VehicleRecord *prev;
    VehicleRecord *next;
};

class VehicleManager
{
public:
    VehicleRecord *head;
    VehicleRecord *tail;

    VehicleManager()
    {
        head = NULL;
        tail = NULL;
    }

    VehicleRecord* find(const char plate[])
    {
        VehicleRecord *temp = head;

        while (temp != NULL)
        {
            if (strcmp(temp->plate, plate) == 0)
                return temp;

            temp = temp->next;
        }

        return NULL;
    }

    void insert(const char plate[], int type, int slot, time_t entry)
    {
        VehicleRecord *node = new VehicleRecord;

        strcpy(node->plate, plate);
        node->type = type;
        node->slot = slot;
        node->entryTime = entry;

        node->prev = tail;
        node->next = NULL;

        if (head == NULL)
            head = tail = node;
        else
        {
            tail->next = node;
            tail = node;
        }
    }

    void remove(const char plate[])
    {
        VehicleRecord *temp = find(plate);

        if (temp == NULL)
            return;

        if (temp == head)
            head = temp->next;

        if (temp == tail)
            tail = temp->prev;

        if (temp->prev != NULL)
            temp->prev->next = temp->next;

        if (temp->next != NULL)
            temp->next->prev = temp->prev;

        delete temp;
    }

    void display()
    {
        if (head == NULL)
        {
            cout << "\nNo vehicles are currently parked.\n";
            return;
        }

        cout << "\n========== PARKED VEHICLES ==========\n";

        VehicleRecord *temp = head;

        while (temp != NULL)
        {
            cout << "\nPlate Number : " << temp->plate;
            cout << "\nVehicle Type : "
                 << (temp->type == 1 ? "Bike" : "Car");
            cout << "\nSlot Number  : " << temp->slot;
            cout << "\nEntry Time   : ";
            displayTime(temp->entryTime);
            cout << "\n-------------------------------------\n";

            temp = temp->next;
        }
    }

    VehicleRecord* split(VehicleRecord *start)
    {
        VehicleRecord *slow = start;
        VehicleRecord *fast = start;

        while (fast->next != NULL && fast->next->next != NULL)
        {
            slow = slow->next;
            fast = fast->next->next;
        }

        VehicleRecord *second = slow->next;
        slow->next = NULL;

        if (second != NULL)
            second->prev = NULL;

        return second;
    }

    VehicleRecord* mergeLists(VehicleRecord *a, VehicleRecord *b)
    {
        if (a == NULL)
            return b;

        if (b == NULL)
            return a;

        if (strcmp(a->plate, b->plate) <= 0)
        {
            a->next = mergeLists(a->next, b);

            if (a->next != NULL)
                a->next->prev = a;

            a->prev = NULL;
            return a;
        }
        else
        {
            b->next = mergeLists(a, b->next);

            if (b->next != NULL)
                b->next->prev = b;

            b->prev = NULL;
            return b;
        }
    }

    VehicleRecord* mergeSort(VehicleRecord *start)
    {
        if (start == NULL || start->next == NULL)
            return start;

        VehicleRecord *second = split(start);

        start = mergeSort(start);
        second = mergeSort(second);

        return mergeLists(start, second);
    }

    void sortVehicles()
    {
        if (head == NULL || head->next == NULL)
        {
            cout << "\nNot enough vehicles to sort.\n";
            return;
        }

        head = mergeSort(head);

        tail = head;

        while (tail != NULL && tail->next != NULL)
            tail = tail->next;

        cout << "\nVehicles sorted by plate number.\n";
    }

    void saveToFile(ofstream &file)
    {
        VehicleRecord *temp = head;

        while (temp != NULL)
        {
            file << temp->plate << " "
                 << temp->type << " "
                 << temp->slot << " "
                 << (long long)temp->entryTime << "\n";

            temp = temp->next;
        }
    }

    ~VehicleManager()
    {
        VehicleRecord *temp = head;

        while (temp != NULL)
        {
            VehicleRecord *nextNode = temp->next;
            delete temp;
            temp = nextNode;
        }
    }
};
