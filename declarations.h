#include <stdbool.h>
#include <stdio.h>

struct point
{
    double x;
    double y;
    double z;
    double w;
} point;

struct point_r
{
    int x;
    int y;
    int z;
    int w;
} point_r ;

struct hash
{
    int marked;
    int count;
    struct node* head;
} * hashTable;

struct node
{
    int visited;
    int id;
    struct node* next;
};

struct traversal
{
    int point1;
    int point2;
    struct traversal *next;
};


int length();
void addFront(int, int);
int get_dimension(FILE*, int*);
int get_flag(int *, int *, int);
int *get_conf(char*);
int *simple_init(int, int);
int *kpp_init(struct point **, int *, int, int );
double dfd(int, int, int, int, struct point**);
struct point *get_mean(int, int, int, int, struct point**);
int **simple_assign(struct point **, int *, int*, int*, int, int);
int func(struct point **, struct point_r*, int, int );
void insertion(int, int, int );
double grid_conversion(double, double );
bool floatcmp(double, double, int);
struct point **get_centers(int *, struct point **, int *, int, int, int *);
struct point *create_tvec(int, int);
int ** lsh_assign(struct point **, struct point **, int *, struct point_r *, int, int *, int *, struct point **, int, int, int, int *);
struct point_r *create_rvec(int);
struct point **concatenation(struct point **, int *, int, int, int, int);
int *update(int **, int *, int *, int , int , int *, struct point **);
int get_center(int **, int *, int , int , int );
double obj(int **, int *, int *, int , int , int *, struct point **);
int *get_silhouette(int **, int *, struct point **, int *, int *, int *, int, int);
