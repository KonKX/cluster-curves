#include "declarations.h"
#include <stdlib.h>

int get_center(int **clusters, int *centers_index, int k, int input_size, int id)
{
    int i, j;

    for(i = 0; i < k; i++)
    {
        for (j = 0; j < input_size; j++)
        {
            if(id == clusters[i][j])
                return centers_index[i];
        }
    }
    return 0;
}


double obj(int **clusters, int *centers_index, int *ids, int input_size, int k, int *sizes, struct point **curves_table)
{
    int i, j, center_index;
    double obj = 0;

    for (i = 0; i < input_size; i++)
    {
        center_index = get_center(clusters, centers_index, k, input_size, ids[i]);
        obj += dfd(sizes[center_index], sizes[i], center_index, i, curves_table);

    }
    return obj;
}

//k-med (swap centers with every other curve from the same cluster and find min obj for each cluster)
int *update(int **clusters, int *centers_index, int *ids, int input_size, int k, int *sizes, struct point **curves_table)
{
    int i, j, l, m, opt = -1, closest, flag, query, *new_centers, *updated_centers;
    double objec = 0, dist, min_dist = 1;

    flag = 0;

    new_centers = (int *)malloc(k * sizeof(int));
    updated_centers = (int *)malloc(k * sizeof(int));

    for (i = 0; i < k; i++)
    {
        new_centers[i] = centers_index[i];
        updated_centers[i] = centers_index[i];
    }

    double min_obj = obj(clusters, centers_index, ids, input_size, k, sizes, curves_table); //objective function after assignment

    for(i = 0; i < k; i++)
    {
        for(j = 0; j < input_size; j++)
        {
            if (clusters[i][j] != -1)
            {
                for (query = 0; query < input_size; query++)
                {
                    if(clusters[i][j] == ids[query])
                    {
                        new_centers[i] = query;
                        break;
                    }
                }
                for (l = 0; l < input_size; l++)
                {
                    for(m = 0; m < k; m++)  //skip centers
                    {
                        if(new_centers[m] == l)
                            flag = 1;
                    }
                    if(flag == 0)
                    {
                        for(m = 0; m < k; m++)
                        {
                            dist = dfd(sizes[new_centers[m]], sizes[l], new_centers[m], l, curves_table); //for each curve find closest center
                            if(dist < min_dist)
                            {
                                min_dist = dist;
                                closest = new_centers[m];
                            }
                        }
                        objec +=  dfd(sizes[closest], sizes[l], closest, l, curves_table);
                    }
                    flag = 0;
                }
                if(objec < min_obj)
                {
                    min_obj = objec;
                    opt = query;
                }
            }
            else break;
        }
        if(opt != -1)
        {
            for(j = 0; j < k; j++)
            {
                if (updated_centers[j] == opt)
                    flag = 1;
            }
            if(flag == 0)
                updated_centers[i] = opt;
        }

        min_obj = 10000;
        objec = 0;
        flag = 0;
    }

    free(new_centers);

    return updated_centers;
}

int *get_silhouette(int **clusters, int *centers_index, struct point **curves, int *ids, int *sizes, int *counts, int k, int input_size)
{
    int i, j, m, l, nextbest, pos1 = 0, pos2 = 0;
    double dist, a = 0, b = 0, max, min = 1, total_s = 0;

    int *silhouette = malloc((k + 1)*(sizeof(int))); //silhouette of each cluster and total
    double sil;

    for (i = 0; i < k; i++)
        counts[i] = 0;

    for (i = 0; i < k; i++){
        for (j = 0; j < input_size; j++){
            if(clusters[i][j] != -1)
                counts[i]++;
            else break;
        }
    }

    for(i = 0; i < k; i++)
    {
        for(j = 0; j < input_size; j++)
        {
            if(clusters[i][j] == -1)
                break;

            for(m = 0; m < input_size; m++)
            {
                if(ids[m] != clusters[i][j])
                    pos1++;
                else break;
            }
            for(l = 0; l < input_size; l++) //calculate a
            {
                if(clusters[i][l] == -1)
                    break;
                if(clusters[i][j] == clusters[i][l]) //skip same curve in cluster
                    continue;
                for(m = 0; m < input_size; m++)
                {
                    if(ids[m] != clusters[i][l])
                        pos2++;
                    else break;
                }
                a += dfd(sizes[pos1], sizes[pos2], pos1, pos2, curves);
                pos2 = 0;
            }

            a /= counts[i];

            for(l = 0; l < k; l++) //find next best center
            {
                if(centers_index[i] == centers_index[l])
                    continue;
                dist = dfd(sizes[pos1], sizes[centers_index[l]], pos1, centers_index[l], curves);
                if(dist < min)
                {
                    min = dist;
                    nextbest = l; //index of center in centers_index
                }
            }
            min = 1;
            for(l = 0; l < input_size; l++) //calculate b
            {
                if(clusters[nextbest][l] == -1)
                    break;
                for(m = 0; m < input_size; m++)
                {
                    if(ids[m] != clusters[nextbest][l])
                        pos2++;
                    else break;
                }
                b += dfd(sizes[pos1], sizes[pos2], pos1, pos2, curves);
                pos2 = 0;
            }

            pos1 = 0;

            b /= counts[nextbest];

            if(a > b)
                max = a;
            else
                max = b;

            total_s += (b - a)/max;

            a = b = 0;
        }

        sil = (total_s/counts[i]);
        if(sil > 0.3)
            silhouette[i] = 1;
        else if(sil < -0.3)
            silhouette[i] = -1;
        else
            silhouette[i] = 0;

        total_s = 0;
    }

    int total = 0;

    for(i = 0; i < k; i++){
        total += silhouette[i];
    }

    silhouette[k] = (int)total/k;

    return silhouette;
}
