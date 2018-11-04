#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "declarations.h"

bool floatcmp(double float1, double float2, int precision)  //compare 2 floats
{
    int int1, int2;

    if (float1 > 0)
        int1 = (int)(float1 * precision + .5);
    else
        int1 = (int)(float1 * precision - .5);

    if (float2 > 0)
        int2 = (int)(float2 * precision + .5);
    else
        int2 = (int)(float2 * precision - .5);

    return (int1 == int2);
}

int **simple_assign(struct point **curves, int *centers, int* curve_sizes, int *ids, int k, int input_size)
{
    int pos;
    int i, j, f, center, flag;
    double dist, min_dist;

    int **clusters = (int **)malloc(k * sizeof(int*));
    int *count = (int *)calloc(k, sizeof(int));

    for(i = 0; i < k; i++)
    {
        clusters[i] = (int*)malloc(input_size * sizeof(int));
    }
    for(i = 0; i < k; i++)
    {
        for(j = 0; j < input_size; j++)
            clusters[i][j] = -1;
    }

    for(i = 0; i < input_size; i++)
    {
        flag = 0;
        pos = 0;
        min_dist = 100000;

        for (j = 0; j < k; j++)
        {
            f = centers[j];
            dist = dfd(curve_sizes[f], curve_sizes[i], f, i, curves);
            if (dist < min_dist)
            {
                min_dist = dist;
                center = f;
            }
        }
        min_dist = 100000;

        for(j=0; j<k; j++)
        {
            if (center != centers[j])
                pos++;
            else
                break;
        }

        clusters[pos][count[pos]] = ids[i];
        count[pos]++;
    }

    return clusters;
}

int func(struct point **grid, struct point_r *r, int id, int max_size)
{
    int i;
    int sum = 0;
    unsigned int m = pow(2,32) - 5;
    for(i=0; i<max_size; i++)
    {
        sum = sum + (int)(grid[id][i].x * r[i].x) + (int)(grid[id][i].y * r[i].y) + (int)(grid[id][i].z * r[i].z) + (int)(grid[id][i].w * r[i].w);
    }
    sum = sum % m;

    return sum;
}

void insertion(int key, int id, int table_size)
{
    struct node* newnode;
    struct node* temp;

    int index = key % table_size;

    newnode = malloc(sizeof(struct node));

    newnode -> id = id;
    newnode -> visited = -1;
    newnode -> next = NULL;

    if (!hashTable[index].head)   //first element
    {
        hashTable[index].head = newnode;
        hashTable[index].count = 1;
    }
    else
    {
        temp = hashTable[index].head; //insert element at the end
        while(temp->next != NULL)
            temp = temp->next;
        temp->next = newnode;
        hashTable[index].count++;
    }
}

struct point *create_tvec(int grids_number, int dim)
{
    int i;
    srand(time(NULL));

    struct point* t_vec = malloc(grids_number*(sizeof(struct point)));

    for (i = 0; i < grids_number; i++)
    {
        point.x = dim * ((double)rand()/ (RAND_MAX + 1.0));
        point.y = dim * ((double)rand()/ (RAND_MAX + 1.0));
        point.z = dim * ((double)rand()/ (RAND_MAX + 1.0));
        point.w = dim * ((double)rand()/ (RAND_MAX + 1.0));
        t_vec[i] = point;
    }

    return t_vec;
};

struct point_r *create_rvec(int max_size)
{
    int i;
    srand(time(NULL));

    struct point_r *r_vec = malloc(max_size*(sizeof(struct point_r))); //r vector with max_size int points

    for (i = 0; i < max_size; i++)
    {
        point_r.x = rand() % 10000; //range [0,10000)
        point_r.y = rand() % 10000;
        point_r.z = rand() % 10000;
        point_r.w = rand() % 10000;

        r_vec[i] = point_r;
    }

    return r_vec;


};
struct point **get_centers(int *centers_index, struct point **curve_table, int *sizes, int k, int lines, int *c_sizes)
{

    int i, j, l;

    struct point **centers = malloc(k*sizeof(struct point*));

    for(i=0; i<lines; i++)
    {
        for(j = 0; j <k; j++)
        {
            if(centers_index[j] == i)
            {
                centers[j] = malloc(sizes[i]*sizeof(struct point));
                for (l = 0; l < sizes[i]; l++)
                    centers[j][l] = curve_table[i][l];
                c_sizes[j] = sizes[i];

            }
        }
    }

    return centers;
};

double grid_conversion(double t, double point_c )
{
    double v, grid_size = 0.0001;

    if (point_c > 0)
        v = ((int)((point_c - t) / grid_size + 0.5)) * grid_size + t;     //find coordinate and snap on grid
    else
        v = ((int)((point_c - t) / grid_size - 0.5)) * grid_size + t;

    return v;
}

struct point **concatenation(struct point **curves, int *sizes, int grids_number, int input_size, int max_size, int dim)
{
    int i, j, l;

    struct point* tvec = create_tvec(grids_number, dim);

    struct point** conc = malloc(input_size*sizeof(struct point*));

    for(i=0; i < input_size; i++)
        conc[i] = malloc(max_size*sizeof(struct point));

    struct point* grid_vec;
    int counter = 0;

    for (i=0; i<input_size; i++ )
    {
        for(l=0; l<grids_number; l++)
        {
            grid_vec = malloc(sizes[i]*sizeof(struct point));

            for (j=0; j<sizes[i]; j++)
            {

                point.x = grid_conversion(tvec[l].x, curves[i][j].x);
                point.y = grid_conversion(tvec[l].y, curves[i][j].y);
                point.z = grid_conversion(tvec[l].z, curves[i][j].z);
                point.w = grid_conversion(tvec[l].w, curves[i][j].w);


                grid_vec[j] = point;

                //printf("%f %f ", grid_vec[j].x, grid_vec[j].y);

                if (j>0 && floatcmp(grid_vec[j].x, grid_vec[j-1].x, 16) && floatcmp(grid_vec[j].y, grid_vec[j-1].y, 16)) //skip if same point
                    continue;
                else
                {
                    conc[i][counter] = grid_vec[j];
                    counter++;
                }
            }
            free(grid_vec);
        }

        while(counter < max_size)
        {
            point.x = point.y = point.z = point.w = 0;
            conc[i][counter] = point;
            counter++;
        }
        counter = 0;
    }

    return conc;
};

int ** lsh_assign(struct point **conc, struct point **centers, int *centers_index, struct point_r *rvec, int k, int *c_sizes, int *sizes, struct point **curves, int max_size, int table_size, int input_size, int *ids)
{

    bool allchecked = false;
    int i, j, key, index, position = 0;
    double dist = 1, prev_dist, radius, min_centers_dist = 100000;
    struct node* temp;
    int **clusters = (int **)malloc(k * sizeof(int*));
    int *index_in_use = (int *)malloc(k * sizeof(int));

    for(i=0; i < table_size; i++) //reset hash table
    {
        temp = hashTable[i].head;
        hashTable[i].marked = 0;
        while (temp != NULL)
        {
            temp -> visited = -1;
            temp = temp ->next;
        }
    }
    for(i = 0; i < k; i++)
    {
        clusters[i] = (int*)malloc((input_size) * sizeof(int));

        for (j = 0; j < k; j++)
        {
            if (j == i) continue;
            else
            {
                dist = dfd(c_sizes[i], c_sizes[j], i, j, centers);
                if (dist < min_centers_dist)
                    min_centers_dist = dist;
            }
        }
    }
    radius = min_centers_dist/2; //initial radius

    for(i = 0; i < k; i++)
    {
        for(j = 0; j < input_size; j++)
            clusters[i][j] = -1;
    }
    while(!allchecked)
    {

        for(i=0; i < k; i++)
        {
            key = func(conc, rvec, centers_index[i], max_size);
            index = key % table_size;
            index_in_use[i] = index;

            temp = hashTable[index].head; //temp points to the first element
            if (!temp)
            {
                printf("No buckets found with LSH\n");
                return 0;
            }

            while (temp != NULL)
            {
                while (ids[position] != temp -> id) //find position in array from curve id stored in node
                    position++;
                dist = dfd(c_sizes[i], sizes[position], centers_index[i], position, curves);
                if (dist < radius)
                {
                    if (temp -> visited == -1)
                    {
                        temp -> visited = centers_index[i];
                        hashTable[index].marked++;
                    }
                    else
                    {
                        prev_dist = dfd(sizes[temp -> visited], sizes[position], temp -> visited, position, curves);
                        if (dist < prev_dist)
                            temp -> visited = centers_index[i];
                    }
                }
                temp = temp -> next;
                position = 0;
            }
        }
        for (j =0; j < k; j++)
        {
            if (hashTable[index_in_use[j]].marked != hashTable[index_in_use[j]].count)  //check if query buckets have been visited
            {
                allchecked = false;
                break;
            }
            else
                allchecked = true;
        }
        radius *= 2;
    }
    int closest, min_dist = 10000;
    position = 0;

    for(i = 0; i < table_size; i++)
    {
        temp = hashTable[i].head;
        while (temp != NULL)
        {
            if(temp -> visited == -1)
            {
                while (ids[position] != temp -> id) //find position in array from curve id stored in node
                    position++;
                for (j = 0; j < k; j++)
                {
                    dist = dfd(c_sizes[j], sizes[position], centers_index[j], position, curves);
                    if (dist < min_dist)
                    {
                        min_dist = dist;
                        closest = centers_index[j];

                    }
                }
                min_dist = 10000;
                temp -> visited = closest;
                hashTable[i].marked++;
            }

            temp = temp -> next;
            position = 0;
        }
    }

    int *ptr = (int *)calloc(k, sizeof(int)); //holds last position of each cluster
    int pos;

    for (i = 0; i < table_size; i++)
    {
        temp = hashTable[i].head;
        while (temp != NULL)
        {
            for(j = 0; j < k; j++)
            {
                if(centers_index[j] == temp -> visited) //check if a curve has been assigned to a center
                    break;
            }
            pos = ptr[j];

            clusters[j][pos] = temp -> id;
            ptr[j]++;

            temp = temp -> next;

        }
    }

    free(index_in_use);
    free(ptr);

    return clusters;
}
