/*
   Copyright 2018 - The OPRECOMP Project Consortium, Alma Mater Studiorum
   Università di Bologna. All rights reserved.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "flexfloat.hpp"

#define TH 0.001f
#define MAX_ITERATIONS 500

#ifndef CLUSTERS
#define CLUSTERS  4
#endif
#ifndef OBJS
#define OBJS      100
#endif
#ifndef COORDS
#define COORDS    18
#endif

#ifndef FLOAT
#define FLOAT double
#endif


FLOAT distance(int ndims, FLOAT *a, FLOAT *b) {
    flexfloat<EXP_DIST, FRAC_DIST> dist = 0.0;

    for(int i=0; i<ndims; ++i)
        dist += flexfloat<EXP_DIST, FRAC_DIST>((flexfloat<EXP_TEMP1, FRAC_TEMP1>(flexfloat<EXP_OBJECT, FRAC_OBJECT>(a[i]))-flexfloat<EXP_TEMP1, FRAC_TEMP1>(flexfloat<EXP_CLUSTER, FRAC_CLUSTER>(b[i]))))*
               flexfloat<EXP_DIST, FRAC_DIST>((flexfloat<EXP_TEMP2, FRAC_TEMP2>(flexfloat<EXP_OBJECT, FRAC_OBJECT>(a[i]))-flexfloat<EXP_TEMP2, FRAC_TEMP2>(flexfloat<EXP_CLUSTER, FRAC_CLUSTER>(b[i]))));

    return FLOAT(dist);
}

int find_nearest(int nclusters, int ndims, FLOAT  *object, FLOAT **clusters) {
    int nearest_index = 0;
    flexfloat<EXP_DIST, FRAC_DIST> min_dist = flexfloat<EXP_DIST, FRAC_DIST>(distance(ndims, object, clusters[0]));
    flexfloat<EXP_DIST, FRAC_DIST> dist;

    for(int i=1; i<nclusters; ++i) {
        dist = flexfloat<EXP_DIST, FRAC_DIST>(distance(ndims, object, clusters[i]));
        if (dist < min_dist) {
            nearest_index = i;
            min_dist = dist;
        }
    }
    return nearest_index;
}

void kmeans(FLOAT **objects, int nobjs, int nclusters, int ndims,
            FLOAT threshold, int *memberof, FLOAT **clusters) {
    int index;
    int iterations=0;
    FLOAT delta;
    FLOAT **iterClusters;
    int *iterClustersSize;

    iterClusters    = (FLOAT**) malloc(nclusters * sizeof(FLOAT*));
    iterClusters[0] = (FLOAT*)  malloc(nclusters * ndims * sizeof(FLOAT));
    for(int i=1; i<nclusters; ++i)
        iterClusters[i] = iterClusters[i-1] + ndims;
    iterClustersSize = (int*) malloc(nclusters * sizeof(int));

    for (int i=0; i<nobjs; ++i)
        memberof[i] = -1;

    do {
        delta = 0.0;
        for(int i=0; i<nobjs; ++i) {
            index = find_nearest(nclusters, ndims, objects[i], clusters);
            if(memberof[i] != index) delta += 1.0;
            memberof[i] = index;
            for(int j=0; j<ndims; ++j)
                iterClusters[index][j] += objects[i][j];
            iterClustersSize[index]++;
        }

        for(int i=0; i<nclusters; ++i) {
            for (int j=0; j<ndims; ++j) {
                if(iterClustersSize[i] > 0)
                    clusters[i][j] = iterClusters[i][j] / iterClustersSize[i];
                iterClusters[i][j] = 0.0;
            }
            iterClustersSize[i] = 0;
        }

        delta /= nobjs;
    } while(delta > threshold && iterations++ < MAX_ITERATIONS);

    free(iterClusters[0]);
    free(iterClusters);
    free(iterClustersSize);
}

int main()
{
    int nclusters = CLUSTERS;
    int ndims = COORDS;
    int nobjs = OBJS;
    int *memberof;
    FLOAT **objects;
    FLOAT **clusters;
    FLOAT threshold = TH; 

    objects = (FLOAT**)malloc(nobjs * sizeof(FLOAT*));
    objects[0] = (FLOAT*) malloc(nobjs * ndims * sizeof(FLOAT));
    for(int i=1; i<nobjs; ++i)
        objects[i] = objects[i-1] + ndims;
    for(int i=0; i < nobjs; ++i)
        for(int j=0; j<ndims; ++j)
          objects[i][j] = ((FLOAT)rand() / (FLOAT)RAND_MAX) * i; 

    memberof = (int*) malloc(nobjs * sizeof(int));

    clusters = (FLOAT**) malloc(nclusters * sizeof(FLOAT*));
    clusters[0] = (FLOAT*)  malloc(nclusters * ndims * sizeof(FLOAT));
    for (int i=1; i<nclusters; ++i)
        clusters[i] = clusters[i-1] + ndims;

    kmeans(objects, nobjs, nclusters, ndims, threshold, memberof, clusters);

    for(int i=0; i < nclusters * ndims; ++i)
        printf("%.15f,", clusters[0][i]);

    //print_flexfloat_stats();

    free(objects[0]);
    free(objects);
    free(clusters[0]);
    free(clusters);
    free(memberof);

    return 0;
}