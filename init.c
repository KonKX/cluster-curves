#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "declarations.h"

int binarySearch(double arr[], int l, int r, double x)
{
    int m = -1;
    while (l <= r)
    {
        m = l + (r-l)/2;

        if (arr[m] == x)
            break;
        else if (arr[m] < x)
            l = m + 1;
        else
            r = m - 1;
    }
    return m;
}

//initialize k random centers from input dataset
int *simple_init(int k, int input_size)
{

    srand(time(NULL));
    int i, j, *centers, new_center, flag = 0;

    centers = (int*)(malloc(k * sizeof(int)));

    for(i = 0; i < k; i++)
    {
        flag = 0;
        new_center = rand() % (input_size - 1);
        for(j = 0; j < k; j++){
            if(centers[j] == new_center)
                flag = 1;
        }
        if(flag == 0)
            centers[i] = new_center;
        else --i;
    }

    return centers;
}


int *kpp_init(struct point **curves, int *curve_sizes, int k, int input_size)
{

    srand(time(NULL));
    int flag, i, j, l, f, *ids, count = 0, count2 = 0, *centers;
    double dist, *dists, new_center, sum = 0, min_dist = 100000;

    flag = 0;
    centers = (int* )(malloc(k * sizeof(int))); //initialization of centers table

    centers[0] = rand() % (input_size - 1) ;//first random center
    count++;

    for (l = 1; l < k; l++)
    {
        dists = (double* )(malloc((input_size - count) * sizeof(double))); //initialization of distances table
        ids = (int* )(malloc((input_size - count) * sizeof(int))); // id of each table element


        for(i = 0; i < input_size; i++)
        {

            for (j = 0; j < count; j++)
            {

                if (i == centers[j])
                {
                    continue; //skip centers
                }
                else
                f = centers[j];
                dist = dfd(curve_sizes[f], curve_sizes[i], f, i, curves);
                if (dist < min_dist)
                    min_dist = dist;
            }
            for (j = 0; j < count; j++)
            {
                if (i == centers[j])
                    flag = 1;
            }
            if (flag == 0 && count2 < input_size - count)
            {
                sum += min_dist * min_dist;
                dists[count2] = sum;
                ids[count2] = i;
                count2++;
            }
            flag = 0;
            min_dist = 1;
        }
        count2 = 0;
        new_center = (rand() / (RAND_MAX + 1.0))*( dists[input_size - count - 1]+ 1);
        int pos = binarySearch(dists, 0, input_size - count - 1, new_center);
        centers[l] = ids[pos];
        count++;
    }

    free(dists);
    free(ids);

    return centers;
}
