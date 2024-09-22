#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct InventoryItem
{
    char name[50];
    int quantity;
    float price;
    struct InventoryItem *next;
};
typedef struct InventoryItem node;

struct CartItem
{
    char name[50];
    int quantity;
    float price;
    struct CartItem *next;
};
typedef struct CartItem cart_node;

node *accept(char name[50], int quantity, float price)
{
    node *temp = (node *)malloc(sizeof(node));
    if (temp == NULL)
    {
        printf("\nMemory not allocated!\n");
        exit(1);
    }
    strncpy(temp->name, name, sizeof(temp->name));
    temp->quantity = quantity;
    temp->price = price;
    temp->next = NULL;
    return temp;
}

node *addItem(node *head, char name[50], int quantity, float price)
{
    node *temp = accept(name, quantity, price);

    if (head == NULL)
    {
        return temp;
    }
    else
    {
        node *p = head;
        while (p->next != NULL)
        {
            p = p->next;
        }
        p->next = temp;
        return head;
    }
}

node *removeItem(node *head, char nameToRemove[50])
{
    node *p = head;
    node *q = NULL;
    while (p != NULL)
    {
        if (strcmp(p->name, nameToRemove) == 0)
        {
            if (q == NULL)
            {
                head = p->next;
                free(p);
                return head;
            }
            else
            {
                q->next = p->next;
                free(p);
                return head;
            }
        }
        q = p;
        p = p->next;
    }
    printf("\nItem with name \"%s\" not found in the inventory.\n", nameToRemove);
    return head;
}


void updateItem(node *head, char itemName[50], int newQuantity, float newPrice)
{
    node *p = head;
    while (p != NULL)
    {
        if (strcmp(p->name, itemName) == 0)
        {
            p->quantity = newQuantity;
            p->price = newPrice;
            printf("\nItem information updated successfully:\n");
            printf("Name: %s\n", p->name);
            printf("New Quantity: %d\n", p->quantity);
            printf("New Price: %.2f\n", p->price);
            return;
        }
        p = p->next;
    }
    printf("\nItem with name \"%s\" not found in the inventory.\n", itemName);
}

void displayCart(cart_node *cart) {
    if (cart == NULL) {
        printf("Your cart is empty.\n");
        return;
    }

    printf("Displaying Cart:\n");
    printf("\tName\t Quantity\t Price of 1 item\t Total price");
    printf("\n\t------------------------------------------------------------\n");

    while (cart != NULL) {
        float itemTotal = cart->quantity * cart->price;
        printf("%10s %15d %15.2f %15.2f\n", cart->name, cart->quantity, cart->price, itemTotal);
        cart = cart->next;
    }
}


float displayInventory(node *head)
{
    node *p = head;
    printf("\nInventory:\n\n");
    printf("\tName\t Quantity\t Price of 1 item\t Total Price");
    printf("\n\t------------------------------------------------------------\n");

    float total = 0.0;
    while (p != NULL)
    {
        float itemTotal = p->quantity * p->price;
        total += itemTotal;
        printf("%10s %15d %15.2f %20.2f\n", p->name, p->quantity, p->price, itemTotal);
        p = p->next;
    }
    printf("\n\n\nTotal Price of all the items in the inventory: %.2f\n", total);
    return total;
}


cart_node *acceptCartItem(char name[50], int quantity, float price)
{
    cart_node *temp = (cart_node *)malloc(sizeof(cart_node));
    if (temp == NULL)
    {
        printf("\nMemory not allocated!\n");
        exit(1);
    }
    strncpy(temp->name, name, sizeof(temp->name));
    temp->quantity = quantity;
    temp->price = price;
    temp->next = NULL;
    return temp;
}

void restock(node *head, char itemName[50])
{
    node *p = head;
    while (p != NULL)
    {
        if (strcmp(p->name, itemName) == 0)
        {
            p->quantity += 50;
            printf("\nRestocked %s by 50 units. New quantity: %d\n", p->name, p->quantity);
            return;
        }
        p = p->next;
    }

    // If the item is not found in the inventory
    printf("Item with name \"%s\" not found in the inventory.\n", itemName);
}


cart_node *addItemToCart(cart_node *cart, node *inventory, char name[50], int quantity)
{
    if (inventory == NULL)
    {
        printf("\nInventory is empty!\n");
        return cart;
    }

    int itemFound = 0;

    cart_node *temp = cart;
    while (inventory != NULL)
    {
        if (strcmp(inventory->name, name) == 0)
        {
            if (inventory->quantity >= quantity)
            {
                if (cart == NULL)
                {
                    cart = acceptCartItem( inventory->name, quantity, inventory->price);
                }
                else
                {
                    temp = cart;
                    while (temp->next != NULL)
                    {
                        temp = temp->next;
                    }
                    temp->next = acceptCartItem( inventory->name, quantity, inventory->price);
                }
                inventory->quantity -= quantity;
                printf("\n%s added to your cart.\n", name);
                itemFound = 1;
                break;
            }
            else
            {
                printf("\nInsufficient quantity in the inventory for item: %s\n", name);
                printf("We don't have the required quantity at the moment. Restocking...\n");
                restock(inventory, name);
                itemFound = 1;
                break;
            }
        }
        inventory = inventory->next;
    }

    if (!itemFound)
    {
        printf("\nThere is no such item available in our inventory: %s\n", name);
    }

    return cart;
}




cart_node *removeFromCart(cart_node *cart, node **inventory, char nameToRemove[50])
{
    int quantityToRemove;

    printf("\nEnter the quantity of %s to remove from your cart: ", nameToRemove);
    scanf("%d", &quantityToRemove);

    cart_node *p = cart;
    cart_node *q = NULL;

    while (p != NULL)
    {
        if (strcmp(p->name, nameToRemove) == 0)
        {
            // Find the corresponding item in the inventory and update its quantity.
            node *inventoryPtr = *inventory;
            while (inventoryPtr != NULL)
            {
                if (strcmp(inventoryPtr->name, nameToRemove) == 0)
                {
                    inventoryPtr->quantity += quantityToRemove;
                    break;
                }
                inventoryPtr = inventoryPtr->next;
            }

            // Update the cart quantity or remove the item if necessary.
            if (p->quantity > quantityToRemove)
            {
                p->quantity -= quantityToRemove;
                printf("%d %s removed from your cart.\n", quantityToRemove, nameToRemove);
            }
            else
            {
                // Remove the entire item from the cart.
                if (q == NULL)
                {
                    cart = p->next;
                    free(p);
                    printf("%s removed from your cart.\n", nameToRemove);
                    return cart;
                }
                else
                {
                    q->next = p->next;
                    free(p);
                    printf("%s removed from your cart.\n", nameToRemove);
                    return cart;
                }
            }
            return cart;
        }
        q = p;
        p = p->next;
    }

    printf("Item with name \"%s\" not found in your cart.\n", nameToRemove);
    return cart;
}



void generateBill(cart_node *cart)
{
    printf("Generating Bill:\n");
    printf("\tName\t Quantity\t Price of 1 item\t Total price");
    printf("\n\t------------------------------------------------------------\n");
    float totalCost = 0.0;
    cart_node *temp = cart;
    while (temp != NULL)
    {
        float itemTotal = temp->quantity * temp->price;
        totalCost += itemTotal;
        printf("%10s %15d %15.2f %15.2f\n", temp->name, temp->quantity, temp->price, itemTotal);
        temp = temp->next;
    }
    printf("Total cost of all items: %.2f\n", totalCost);
    printf("Thank you for shopping!\n");
}




int checkout(cart_node *cart)
{
    if (cart == NULL)
    {
        printf("Your cart is empty.\n");
        return 0;
    }

    printf("Generating Bill:\n");
    printf("\tName\t Quantity\t Price of 1 item\t Total price");
    printf("\n\t------------------------------------------------------------\n");
    float totalCost = 0.0;
    cart_node *temp = cart;
    while (temp != NULL)
    {
        float itemTotal = temp->quantity * temp->price;
        totalCost += itemTotal;
        printf("%10s %15d %15.2f %15.2f\n", temp->name, temp->quantity, temp->price, itemTotal);
        temp = temp->next;
    }
    printf("Total cost of all items: %.2f\n", totalCost);
    printf("Thank you for shopping!\n");

    while (cart != NULL)
    {
        cart_node *temp = cart;
        cart = cart->next;
        free(temp);
    }
    return 0;
}

void addItemToInventory(node **head, char name[50], int quantity, float price)
{
    *head = addItem(*head, name, quantity, price);
    printf("%s added to the inventory.\n", name);
}


int main()
{
    node *head = NULL;
    cart_node *customerCart = NULL;
    int choice;
    char nameToRemove[50];
    char name[50];
    int quantity;
    float price;
    printf("\tStationary Shopee");
    do
    {
        printf("\n");
        printf("**************************************\n");
        printf("*    1. Display Inventory            *\n");
        printf("*    2. Add an item to Inventory     *\n");
        printf("*    3. Add an item to Cart          *\n");
        printf("*    4. Display Cart                 *\n");
        printf("*    5. Delete an item from Cart     *\n");
        printf("*    6. Checkout                     *\n");
        printf("*    7. Exit                         *\n");
        printf("**************************************\n\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
            {
                displayInventory(head);
                break;
            }
            case 2:
            {
                getchar();
                printf("Enter item name: ");
                gets(name);
                printf("Enter quantity: ");
                scanf("%d", &quantity);
                printf("Enter price: ");
                scanf("%f", &price);
                addItemToInventory(&head, name, quantity, price);
                break;
            }
            case 3:
            {
                printf("Enter item name to add to your cart: ");
                scanf("%s", name);
                printf("Enter the quantity you want to buy: ");
                scanf("%d", &quantity);
                customerCart = addItemToCart(customerCart, head, name, quantity);
                break;
            }
            case 4:
            {
                displayCart(customerCart);
                break;
            }
            case 5:
            {
                printf("Enter item name to remove from your cart: ");
                scanf("%s", nameToRemove);
                customerCart = removeFromCart(customerCart, &head, nameToRemove);
                printf("%s removed from your cart.\n", nameToRemove);
                break;
            }
            case 6:
            {
                checkout(customerCart);
                break;
            }
            case 7:
            {
                break;
            }
            default:
            {
                printf("Invalid choice. Please try again.\n");
            }
        }
    }
    while(choice != 7);

    return 0;
}
