#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "declarations.h"

#define BUFFER_SIZE 80000

static time_t t1, t2;

int main(int argc, char *argv[])
{
    int opt = 0;
    char *in_fname = NULL;
    char *out_fname = NULL;
    char *conf_fname = NULL;
    int complete = 0;

//-i <input file> -c <configuration file> -o <output file>

    while ((opt = getopt(argc, argv, "i:c:o:a")) != -1)
    {
        switch(opt)
        {
        case 'i':
            in_fname = optarg;
            break;
        case 'c':
            conf_fname = optarg;
            break;
        case 'o':
            out_fname = optarg;
            break;
        case 'a':
            complete = 1;
            break;
        }
    }

    int *config_values = get_conf(conf_fname);
    int k = config_values[0];
    int grids = config_values[1];
    int L = config_values[2];

    //printf("%d %d %d\n", k, grids, L);


    int dim, lines = 1, flag = 0;
    char ch, buffer[BUFFER_SIZE];
    FILE* fp = fopen(in_fname, "r");
    if (fp == NULL)
    {
        printf("Couldn't open input file.");
        return -1;
    }
    else
    {
        char buffer2[64];

        ch = fgetc(fp);
        if (ch == '@')
        {
            while(ch != '2' && ch != '3' && ch != '4')
                ch = fgetc(fp);
            dim = ch - '0';
            flag = 1;
        }
        else
        {
            dim = 2; //default dimension
        }

        printf("DIMENSION: %d\n", dim);

        ch = fgetc(fp);

        while(ch != EOF)
        {
            ch = fgetc(fp);

            if(ch == '\n')
            {
                lines++;
            }
        }
        rewind(fp);
    }

    printf("INPUT SIZE: %d\n", lines);
    printf("Processing...\n");
    int total_n = 0;
    int n, id, size, max_size = 1, i = 0, j = 0;

    double x, y, z, w;
    int sizes[lines]; //size of each curve
    int ids[lines]; //id of each curve
    struct point **curve_table = (struct point **)malloc(lines*sizeof(struct point*));

    if(flag)
    {
        while(ch!='\n')
            ch = fgetc(fp);
    }

    while(fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        sscanf(buffer, "%d %d %n", &id, &size, &n);
        //printf("%d %d\n", id, size);

        if (size > max_size) // find curve max size
            max_size = size;

        total_n += n;

        ids[i] = id;
        sizes[i] = size;
        curve_table[i] = (struct point *)malloc(sizes[i]*sizeof(struct point));
        if (dim == 2)
        {
            while (sscanf(buffer + total_n, "%*[^-0123456789]%lf%*[^-0123456789]%lf%n", &x, &y, &n)==2) // read a 2d point
            {

                point.x = x;
                point.y = y;
                point.z = 0;
                point.w = 0;

                curve_table[i][j] = point;

                total_n += n;
                //printf("%1.16f %1.15f ", curve_table[i][j].x, curve_table[i][j].y);
                j++;
            }
        }
        else if (dim == 3)
        {
            while (sscanf(buffer + total_n, "%*[^-0123456789]%lf%*[^-0123456789]%lf*[^-0123456789]%lf%n", &x, &y, &z, &n)==3) // read a 3d point
            {

                point.x = x;
                point.y = y;
                point.z = z;
                point.w = 0;

                curve_table[i][j] = point;

                total_n += n;
                //printf("%1.16f %1.15f ", x, y);
                j++;
            }
        }
        else if (dim ==4 )
        {
            while (sscanf(buffer + total_n, "%*[^-0123456789]%lf%*[^-0123456789]%lf*[^-0123456789]%lf%*[^-0123456789]%lf%n", &x, &y, &z, &w, &n)==4) // read a 4d point
            {

                point.x = x;
                point.y = y;
                point.z = z;
                point.w = w;

                curve_table[i][j] = point;

                total_n += n;
                //printf("%1.16f %1.15f ", x, y);
                j++;
            }
        }
        //printf("\n\n");
        total_n = j = 0;
        i++;
    }

    struct point **conc = concatenation(curve_table, sizes, grids, lines, max_size, dim);

    int key;
    int table_size = lines/8;

    hashTable = calloc(table_size, sizeof(struct hash));
    struct point_r *rvec = create_rvec(max_size);

    for(i=0; i<lines; i++)
    {

        key = func(conc, rvec, i, max_size);//get key
        insertion(key, ids[i], table_size);//insert to hash table

    }
    //end of preprocessing



    //simple initialization - LSH assignment - PAM

    t1 = time(0);

    int *counts = calloc(k, sizeof(int));

    int *s; //silhouette

    int *centers_index = simple_init(k, lines);

    int *c_sizes = malloc(k * sizeof(int));

    struct point **centers = get_centers(centers_index, curve_table, sizes, k, lines, c_sizes);

    struct point** centers_co;
    int *new_centers;
    int **lsh_clusters;
    int flag1;

    do
    {
        lsh_clusters = lsh_assign(conc, centers, centers_index, rvec, k, c_sizes, sizes, curve_table, max_size, table_size, lines, ids);
        new_centers = update(lsh_clusters, centers_index, ids, lines, k, sizes, curve_table);
        flag1 = get_flag(centers_index, new_centers, k);
        memcpy(centers_index, new_centers, k * sizeof(int));
        centers = get_centers(new_centers, curve_table, sizes, k, lines, c_sizes);
    }
    while(!flag1);

    t2 = time(0);

    fp = fopen(out_fname, "a");

    if (fp == NULL)
    {
        printf("Couldn't open output file.");
        return -1;
    }

    s = get_silhouette(lsh_clusters, centers_index, curve_table, ids, sizes, counts, k, lines);

    fprintf(fp, "Algorithm: Simple initialization - LSH assignment - PAM\n");
    fprintf(fp, "Metric: Frechet\n");
    for (i = 0; i < k; i++)
    {
        fprintf(fp, "CLUSTER-%d  ", i);
        fprintf(fp, "{size: %4d,  centroid: %4d} ", counts[i], ids[centers_index[i]]);
        if(complete)
        {
            for( j = 0; j < counts[i]; j++)
                fprintf(fp, "curve_%d ", lsh_clusters[i][j]);
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "Clustering time (in seconds): %ld\n", t2 - t1);
    fprintf(fp, "Silhouette: [ ");
    for (i = 0; i < k + 1; i++)
    {
        fprintf(fp, "%d ", s[i]);
    }
    fprintf(fp, "]\n\n");

    free(c_sizes);
    for (i = 0; i < k; i++)
    {
        free(lsh_clusters[i]);
    }
    free(lsh_clusters);
    free(centers_index);

    //KMEANS++ initialization - LSH assignment - PAM

    t1 = time(0);

    centers_index = kpp_init(curve_table, sizes, k, lines);

    c_sizes = malloc(k * sizeof(int));

    centers = get_centers(centers_index, curve_table, sizes, k, lines, c_sizes);


    do
    {
        lsh_clusters = lsh_assign(conc, centers, centers_index, rvec, k, c_sizes, sizes, curve_table, max_size, table_size, lines, ids);
        new_centers = update(lsh_clusters, centers_index, ids, lines, k, sizes, curve_table);
        flag1 = get_flag(centers_index, new_centers, k);
        memcpy(centers_index, new_centers, k * sizeof(int));
        centers = get_centers(new_centers, curve_table, sizes, k, lines, c_sizes);

    }
    while(!flag1);
    t2 = time(0);

    s = get_silhouette(lsh_clusters, centers_index, curve_table, ids, sizes, counts, k, lines);

    fprintf(fp, "Algorithm: KMEANS++ initialization - LSH assignment - PAM\n");
    fprintf(fp, "Metric: Frechet\n");
    for (i = 0; i < k; i++)
    {
        fprintf(fp, "CLUSTER-%d  ", i);
        fprintf(fp, "{size: %4d,  centroid: %4d} ", counts[i], ids[centers_index[i]]);
        if(complete)
        {
            for( j = 0; j < counts[i]; j++)
                fprintf(fp, "curve_%d ", lsh_clusters[i][j]);
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "Clustering time (in seconds): %ld\n", t2 - t1);
    fprintf(fp, "Silhouette: [ ");
    for (i = 0; i < k + 1; i++)
    {
        fprintf(fp, "%d ", s[i]);
    }
    fprintf(fp, "]\n\n");

    free(c_sizes);
    for (i = 0; i < k; i++)
    {
        free(lsh_clusters[i]);
    }
    free(lsh_clusters);
    free(centers_index);

    //simple initialization - Lloyd's assignment - PAM

    t1 = time(0);

    int **clusters;
    centers_index = simple_init(k, lines);
    do
    {
        clusters = simple_assign(curve_table, centers_index, sizes, ids, k, lines);
        new_centers = update(clusters, centers_index, ids, lines, k, sizes, curve_table);
        flag1 = get_flag(centers_index, new_centers, k);
        memcpy(centers_index, new_centers, k * sizeof(int));
    }
    while(!flag1);

    t2 = time(0);

    s = get_silhouette(clusters, centers_index, curve_table, ids, sizes, counts, k, lines);

    fprintf(fp, "Algorithm: Simple initialization - Lloyd's assignment - PAM\n");
    fprintf(fp, "Metric: Frechet\n");
    for (i = 0; i < k; i++)
    {
        fprintf(fp, "CLUSTER-%d  ", i);
        fprintf(fp, "{size: %4d,  centroid: %4d} ", counts[i], ids[centers_index[i]]);
        if(complete)
        {
            for( j = 0; j < counts[i]; j++)
                fprintf(fp, "curve_%d ", clusters[i][j]);
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "Clustering time (in seconds): %ld\n", t2 - t1);
    fprintf(fp, "Silhouette: [ ");
    for (i = 0; i < k + 1; i++)
    {
        fprintf(fp, "%d ", s[i]);
    }
    fprintf(fp, "]\n\n");

    for (i = 0; i < k; i++)
    {
        free(clusters[i]);
    }
    free(clusters);
    free(centers_index);

    //KMEANS++ initialization - Lloyd's assignment - PAM

    t1 = time(0);

    centers_index = kpp_init(curve_table, sizes, k, lines);
    do
    {
        clusters = simple_assign(curve_table, centers_index, sizes, ids, k, lines);
        new_centers = update(clusters, centers_index, ids, lines, k, sizes, curve_table);
        flag1 = get_flag(centers_index, new_centers, k);
        memcpy(centers_index, new_centers, k * sizeof(int));
    }
    while(!flag1);

    t2 = time(0);

    s = get_silhouette(clusters, centers_index, curve_table, ids, sizes, counts, k, lines);

    fprintf(fp, "Algorithm: KMEANS++ initialization - Lloyd's assignment - PAM\n");
    fprintf(fp, "Metric: Frechet\n");
    for (i = 0; i < k; i++)
    {
        fprintf(fp, "CLUSTER-%d  ", i);
        fprintf(fp, "{size: %4d,  centroid: %4d} ", counts[i], ids[centers_index[i]]);
        if(complete)
        {
            for( j = 0; j < counts[i]; j++)
                fprintf(fp, "curve_%d ", clusters[i][j]);
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "Clustering time (in seconds): %ld\n", t2 - t1);
    fprintf(fp, "Silhouette: [ ");
    for (i = 0; i < k + 1; i++)
    {
        fprintf(fp, "%d ", s[i]);
    }
    fprintf(fp, "]\n\n");

    printf("%s has been created\n", out_fname);

    free(centers_index);
    for(i=0; i<lines; i++)
    {
        free(curve_table[i]);
        free(conc[i]);
    }
    free(curve_table);
    free(conc);
    for (i = 0; i < k; i++)
    {
        free(clusters[i]);

    }
    free(clusters);
    free(hashTable);
    fclose(fp);

    return 0;
}
