#include "declarations.h"
#include <stdlib.h>
#include <math.h>

double dist(struct point j, struct point k)
{
    double dx = (j.x - k.x);
    double dy = (j.y - k.y);
    double dw = (j.w - k.w);
    double dz = (j.z - k.z);
    double d = sqrt(dx*dx + dy*dy + dw*dw + dz*dz);
    return d;
}

double dfd(int n, int m, int id, int q_id, struct point** curves_table) //n,m curve sizes
{
    double L[n][m], min;
    int i, j;
    for (i=0; i<n; i++)
    {
        for (j=0; j < m; j++)
        {
            if (i == 0 && j == 0)
                L[i][j] = dist(curves_table[id][i], curves_table[q_id][j]);
            else if (i == 0)
            {
                if (dist(curves_table[id][i], curves_table[q_id][j]) > L[0][j-1])
                    L[i][j] = dist(curves_table[id][i], curves_table[q_id][j]);
                else
                    L[i][j] = L[0][j-1];
            }
            else if (j == 0)
            {
                if (dist(curves_table[id][i], curves_table[q_id][j]) > L[i-1][0])
                    L[i][j] = dist(curves_table[id][i], curves_table[q_id][j]);
                else
                    L[i][j] = L[i-1][0];
            }
            else
            {
                if((L[i-1][j] < L[i][j-1]) && (L[i-1][j] < L[i-1][j-1]))
                    min = L[i-1][j];
                else if ((L[i][j-1] < L[i-1][j]) && (L[i][j-1] < L[i-1][j-1]))
                    min = L[i][j-1];
                else
                    min = L[i-1][j-1];

                if (dist(curves_table[id][i], curves_table[q_id][j]) > min)
                    L[i][j] = dist(curves_table[id][i], curves_table[q_id][j]);
                else
                    L[i][j] = min;
            }
        }
    }
    return L[n-1][m-1];
}

struct traversal *head = NULL;

void addFront(int point1, int point2)
{

    struct traversal *node = (struct traversal*) malloc(sizeof(struct traversal));

    node->point1 = point1;
    node->point2 = point2;

    node->next = head;

    head = node;
}

int length()
{
    int length = 0;
    struct traversal *current;

    for(current = head; current != NULL; current = current->next)
    {
        length++;
    }

    return length;
}

//mean computation (2 curves)
struct point *get_mean(int n, int m, int id, int q_id, struct point** curves_table) //n,m curve sizes
{
    double L[n][m], min;
    int i, j;


    for (i=0; i<n; i++)
    {
        for (j=0; j < m; j++)
        {
            if (i == 0 && j == 0)
                L[i][j] = dist(curves_table[id][i], curves_table[q_id][j]);
            else if (i == 0)
            {
                if (dist(curves_table[id][i], curves_table[q_id][j]) > L[0][j-1])
                    L[i][j] = dist(curves_table[id][i], curves_table[q_id][j]);
                else
                    L[i][j] = L[0][j-1];
            }
            else if (j == 0)
            {
                if (dist(curves_table[id][i], curves_table[q_id][j]) > L[i-1][0])
                    L[i][j] = dist(curves_table[id][i], curves_table[q_id][j]);
                else
                    L[i][j] = L[i-1][0];
            }
            else
            {
                if((L[i-1][j] < L[i][j-1]) && (L[i-1][j] < L[i-1][j-1]))
                    min = L[i-1][j];
                else if ((L[i][j-1] < L[i-1][j]) && (L[i][j-1] < L[i-1][j-1]))
                    min = L[i][j-1];
                else
                    min = L[i-1][j-1];

                if (dist(curves_table[id][i], curves_table[q_id][j]) > min)
                    L[i][j] = dist(curves_table[id][i], curves_table[q_id][j]);
                else
                    L[i][j] = min;
            }
        }
    }

    //find optimal traversal
    int p = n;
    int q = m;
    int minIndex;
    int meanSize;

    addFront(p, q);

    while(p != 0 && q != 0)
    {
        if (L[p - 1][q] < L[p][q - 1] && L[p - 1][q] < L[p - 1][q - 1])
            minIndex = 0;
        else if (L[p][q - 1] < L[p - 1][q] && L[p][q - 1] < L[p - 1][q - 1])
            minIndex = 1;
        else
            minIndex = 2;

        if (minIndex == 0)
            addFront(--p, q);
        else if (minIndex == 1)
            addFront(p, --q);
        else
            addFront(--p, --q);

    }

    meanSize = length();
    i = 0;

    struct point *mean = (struct point*)malloc(meanSize * sizeof(struct point));

    struct traversal *temp = head;

    while (temp != NULL){

        mean[i].x = (curves_table[id][temp -> point1].x + curves_table[q_id][temp -> point2].x) / 2;
        mean[i].y = (curves_table[id][temp -> point1].y + curves_table[q_id][temp -> point2].y) / 2;
        mean[i].z = (curves_table[id][temp -> point1].z + curves_table[q_id][temp -> point2].z) / 2;
        mean[i].w = (curves_table[id][temp -> point1].w + curves_table[q_id][temp -> point2].w) / 2;

        i++;
        temp = temp -> next;
    }

    return mean;
};
