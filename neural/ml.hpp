#ifndef ML_HPP
#define ML_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <iostream>

namespace ml {

#define SQ(x)   ((x) * (x))
    

    struct kdhyperrect {
        int dim;
        double *min, *max;
    };

    struct kdnode {
        double *pos;
        int dir;
        void *data;

        struct kdnode *left, *right;
    };

    struct res_node {
        struct kdnode *item;
        double dist_sq;
        struct res_node *next;
    };

    struct kdtree {
        int dim;
        struct kdnode *root;
        struct kdhyperrect *rect;
        void (*destr)(void*);
    };

    struct kdres {
        struct kdtree *tree;
        struct res_node *rlist, *riter;
        int size;
    };

    struct kdtree *kd_create(int k);

    void kd_free(struct kdtree *tree);

    void kd_data_destructor(struct kdtree *tree, void (*destr)(void*));

    int kd_insert(struct kdtree *tree, const double *pos, void *data);

    struct kdres *kd_nearest(struct kdtree *tree, const double *pos);


}

#endif
