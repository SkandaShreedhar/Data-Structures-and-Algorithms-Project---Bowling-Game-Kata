#include <stdio.h>
#include <stdlib.h>

#define MAX_PLAYERS 5
#define MAX_ROUNDS 10

struct Node
{
    int data;
    struct Node* next;
};

struct Stack
{
    struct Node* top;
};

struct Queue
{
    struct Node* front;
    struct Node* rear;
};

int is_empty_stack(struct Stack* stack)
{ 
    return stack->top == NULL;
}

void init_stack(struct Stack* stack)
{
    stack->top = NULL;
}

void push(struct Stack* stack, int data) // push element into the stack (for strikes)
{
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    new_node->data = data;
    new_node->next = stack->top;
    stack->top = new_node;
}

int pop(struct Stack* stack) // pop element from the stack (for strikes)
{
    if (is_empty_stack(stack))
    {
        printf("Underflow Condition: Stack is empty\n");
        exit(EXIT_FAILURE);
    }
    struct Node* temp = stack->top;
    int data = temp->data;
    stack->top = temp->next;
    free(temp);
    return data;
}

int is_empty_queue(struct Queue* queue)
{
    return queue->front == NULL;
}

void init_queue(struct Queue* queue)
{
    queue->front = queue->rear = NULL;
}

void enqueue(struct Queue* queue, int data) // add element into the queue (for spares)
{
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    new_node->data = data;
    new_node->next = NULL;
    if (queue->rear == NULL)
    {
        queue->front = queue->rear = new_node;
        return;
    }
    queue->rear->next = new_node;
    queue->rear = new_node;
}

int dequeue(struct Queue* queue) // remove element from the queue (for spares)
{
    if (is_empty_queue(queue))
    {
        printf("Underflow Condition: Queue is empty\n");
        exit(EXIT_FAILURE);
    }
    struct Node* temp = queue->front;
    int data = temp->data;
    queue->front = temp->next;
    if (queue->front == NULL)
    {
        queue->rear = NULL;
    }
    free(temp);
    return data;
}

// main scorecard calculation logic

int cal_score(int rolls[])
{
    int total_score = 0;
    struct Stack strike_stack;
    struct Queue spare_queue;
    init_stack(&strike_stack);
    init_queue(&spare_queue);
    for (int i = 0; i < MAX_ROUNDS * 2; i++)
    {
        if (i == 18 && (rolls[i] == 10))
        {
            total_score += 30;
        }
        else
        {
        if (rolls[i] == 10 && rolls[i + 1] == 0) // strike
        {
            push(&strike_stack, rolls[i]);
        }
        else
        {
            if (!is_empty_stack(&strike_stack))
            {
                total_score += (rolls[i] + rolls[i + 1]);
                total_score += pop(&strike_stack);
            }
            else if (!is_empty_queue(&spare_queue))
            {
                total_score += rolls[i];
                total_score += dequeue(&spare_queue);
            }
            else 
            {
                total_score += rolls[i];
            }
            if (i % 2 == 1 && rolls[i - 1] + rolls[i] == 10) // spare
            {
                enqueue(&spare_queue, rolls[i + 1]);
            }
        }
        }
    }
    return total_score;
}

int main()
{
    int no_players;
    printf("Enter the number of players (1-%d): ", MAX_PLAYERS); // number of players input
    scanf("%d", &no_players);

    if (no_players < 1 || no_players > MAX_PLAYERS)
    {
        printf("Invalid input\n");
        return 1;
    }

    char player_names[MAX_PLAYERS][50];
    int rolls[MAX_PLAYERS][MAX_ROUNDS * 2];  // max rolls per player

    for (int i = 0; i < no_players; i++)
    {
        printf("Enter name for Player %d: ", i + 1); // player name input
        scanf("%s", player_names[i]);
    }

    for (int round = 0; round < MAX_ROUNDS; round++)
    {
        for (int player_no = 0; player_no < no_players; player_no++)
        {
            printf("Enter the scores for each roll for %s in Round %d (0-10), separated by space:\n", player_names[player_no], round + 1); // scores input
            scanf("%d %d", &rolls[player_no][round * 2], &rolls[player_no][round * 2 + 1]);
        }
    }

    int total_scores[MAX_PLAYERS] = {0}; // array to store total scores for each player

    for (int i = 0; i < no_players; i++)
    {
        total_scores[i] = cal_score(rolls[i]);
        printf("Total Score for %s: %d\n", player_names[i], total_scores[i]);
    }

    // to find the winner

    int max_score = total_scores[0];
    int winner_index = 0;
    for (int i = 1; i < no_players; i++)
    {
        if (total_scores[i] > max_score)
        {
            max_score = total_scores[i];
            winner_index = i;
        }
    }
    printf("The winner is: %s with a total score of %d\n", player_names[winner_index], max_score);

    return 0;
}
